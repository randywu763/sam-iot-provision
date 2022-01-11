/**
 * \file
 *
 * \brief Main File of AT Commands Project
 *
 */
#include "main.h"
#include "iperf/Iperf.h"
#include "uart_handler/uart_handler.h"
#include "serial_bridge.h"
#include "at_cmd_handler/at_cmd_handler.h"
#include "socket/include/m2m_socket_host_if.h"

tstr_test_conn_status gstrAllStatus;
tstr_app_config gtstrApp_config;
tstrWifiInitParam gstrWifiParam;

static tstrAt_cmd_content		gstrAt_cmdContent;
static tstrAt_cmd_parse_chrs	gstrAt_cmd_parse_chrs = {"+","=","[","]",",","/?"};//,"*"};	
								//Header, numOfParams, encStart, encStop, paramSep, HelpSep AT+CFG=2[ECHO,1] or AT+CFG /?
static strAtCMD_Handler			gastrAtCMD_Handler[AT_MAX_COMMANDS_COUNT];
tpf_at_cmd_handler				pf_at_cmd_handler;

#define TEST_TLS
#define TEST_IPERF
#define TEST_POWER_SAVE

//#define UART_BAUDRATE	9600
//#define UART_BAUDRATE	19200

#define STRING_HEADER	STRING_EOL"-- Atmel Corporation"\
						STRING_EOL"-- Wifi Atmel UART APP (DEMO)"STRING_EOL \
						"-- "BOARD_NAME""STRING_EOL \
						"-- Compiled: "__DATE__" "__TIME__""STRING_EOL

#define ESC_DATA_SEQ		"\033SDS:"
#define USART_TIMEOUT_M_SEC		(25 * 1000)
#define UART_BAUDRATE			115200
#define USART_MODE				USART_TRANSFER_ASYNCHRONOUSLY	//USART_TRANSFER_SYNCHRONOUSLY


struct usart_module		cdc_uart_module;
volatile uint8_t gau8Rx_buffer[MAX_RX_BUFFER_LENGTH];

static uint8	gau8DataBuffer[AT_MAX_RX_BUFF_SIZE]			={0};
volatile static uint8	gu8CmdStatus		= AT_CMD_STATUS_IDLE;
volatile static uint16	gu16Data_Index		= 0;
volatile static uint8	gu8IsrFired			= false;
volatile static uint8	gu8InvalidPrinted	= true;
volatile static uint8   escape_command		= false;
volatile static uint8   carriage_return 	= false;
volatile static uint8	gu8TO = 0;

extern uint32_t gu32UDPSendTime;
extern uint32   gu32IperfPacketsToSend;
extern uint8_t  gu8UDPIperfRX, gu8TCPIperfRX;
extern uint32   gu32IPAddress;
extern uint8		gHTTPport[AT_MAX_PARAM_LENGTH];
extern uint8		gTLS;
extern HTTP_RXInfo	gHTTPInfo;
extern uint8		gHTTPChecksum;
extern SOCKET		gHTTPsock;
extern uint8		gPingRequired;
extern uint32		gu32PingCount;

#define HEX2ASCII(x) (((x)>=10)? (((x)-10)+'A') : ((x)+'0'))

void uart_rx_isr(const struct usart_module *const uart_module)
{
	uint8	u8RxChar = gau8Rx_buffer[0];
	gu8IsrFired = true;

	if(u8RxChar == 0x12)// || u8RxChar == 'S')
	{
		gu8CmdStatus = AT_CMD_STATUS_SBMODE;
		gu8InvalidPrinted = true;
		return;
	}

	//Check for Starting
	switch(gu8CmdStatus)
	{
		case AT_CMD_STATUS_IDLE:
		{
			switch(strlen((const char *)gau8DataBuffer))
			{
				case 0:
				{
					if (u8RxChar == ESCAPE_CHAR)
					{
						escape_command = true;
						break;
					}
					if(('A' != u8RxChar) && ('H' != u8RxChar))
					{
						goto INVALID_CH;
					}
					break;
				}//End case 0 of strlen(gau8DataBuffer)
				case 1:
				{
					if(!escape_command) 
					{
						if(('T' != u8RxChar) && ('E' != u8RxChar)) 
						{
							goto INVALID_CH;
						}
					}
					break;
				}//End case 1 of strlen(gau8DataBuffer)
				case 2:
				{
					if(!escape_command) 
					{
						if(('+' != u8RxChar) && ('L' != u8RxChar)) 
						{
							goto INVALID_CH;
						}
					}
					break;
				}//End case 2 of strlen(gau8DataBuffer)
				case 3:
				{
					if(!escape_command)
					{
						escape_command = false;
						if(('L' == gau8DataBuffer[2]) && ('P' != u8RxChar)) {
							goto INVALID_CH;
						}
					}
					break;
				}//End case 3 of strlen(gau8DataBuffer)
			}
			//Fill in Buffer
			switch(u8RxChar) 
			{
				case '\r':
					carriage_return = true;
				case '\n':
				{
					if(carriage_return == true)
					{
						carriage_return = false;
						if(gu8CmdStatus != AT_CMD_STATUS_CHECK)
						{
							gu8CmdStatus = AT_CMD_STATUS_CHECK;
							RESET_VAR_TO_0(gu16Data_Index);
							if(0 == strlen((const char *)gau8DataBuffer))
							{
								gu8InvalidPrinted = false;
							}
						}
					} 
					else
					{
						gau8DataBuffer[gu16Data_Index++] = u8RxChar;
					}
					goto EXIT;
				}
				default:
				{
					gau8DataBuffer[gu16Data_Index++] = u8RxChar;
					goto EXIT;
				}
			}
			break;
		}
		default:
		{
			/*Nothing*/
			goto EXIT;
		}
	}

INVALID_CH:
	gu8InvalidPrinted = false;
//	memset(gau8Rx_buffer, 0, MAX_RX_BUFFER_LENGTH);
EXIT:
	usart_read_buffer_job((struct usart_module *)uart_module, (uint8_t *)gau8Rx_buffer, 1);
	return;
}

void uart_tx_isr(const struct usart_module *const uart_module)
{
	/* 
	* Nothing to handle after sending data, this function is also disabled
	* to enable it, kindly goto configure_uart_callbacks in uart_handler.c and enable it
	*/
}

/*Interrupt Variables*/
static volatile uint8 gu8Interrupt;
/*Settings Variables*/
static uint8 gau8MacAddr[] = MAC_ADDRESS;
static sint8 gacDeviceName[] = M2M_DEVICE_NAME;
/*Power Save Variables*/
static volatile uint8 gu8Sleep;
/*Scan Variables*/
static volatile uint8 gu8Index = 0;
/*Prov. Mode Data*/
tstrM2MProvisionInfo	*pstrProvInfo;

static volatile uint8 gu8Prov;
static volatile uint8 u8Socket_Opened = 0;
static void timer_cb(void)
{
	gu8TO = 1;
}

static void tcp_cb(uint8 cmd)
{
	if(cmd == 'A') {
		AT_SEND_OK("LED 1 ON");
	}
	if(cmd == 'B') {
		AT_SEND_OK("LED 1 OFF");
	}
	if(cmd == 'C') {
		AT_SEND_OK("LED 2 ON");
	}
	if(cmd == 'D') {
		AT_SEND_OK("LED 2 OFF");
	}
	if(cmd == 'E') {
		AT_SEND_OK("LED 3 ON");
	}
	if(cmd == 'F') {
		AT_SEND_OK("LED 3 OFF");
	}
}

void pingCb(uint32 u32IPAddr, uint32 u32RTT, uint8 u8ERR)
{
    static const char* const pu8ERR_str[] = {"PING_SUCCESS","PING_ERR_DEST_UNREACH","PING_ERR_TIMEOUT"};
    M2M_INFO("(%lu)Ping status %s\n", gu32PingCount, pu8ERR_str[u8ERR]);
    gu32PingCount--;
    if(gu32PingCount > 0)
    {
        m2m_ping_req(u32IPAddr, 0, pingCb);
    }
    else
    {
        M2M_INFO("Test Finished\n");
    }
    
    if(PING_ERR_SUCCESS == u8ERR)
    {
        AT_SEND_OK("PING");
    }
    else
        AT_SEND_ERROR(gu32PingCount,pu8ERR_str[u8ERR]);
}

void m2m_wifi_resolve_handler(uint8* pu8DomainName, uint32 u32ServerIP)
{
	uint8 host_ip[4];

	if(u32ServerIP == 0)
	{
		M2M_INFO("ERROR: DNS lookup failed\r\n");
	}
	else
	{
		host_ip[0] = u32ServerIP & 0xFF;
		host_ip[1] = (u32ServerIP >> 8) & 0xFF;
		host_ip[2] = (u32ServerIP >> 16) & 0xFF;
		host_ip[3] = (u32ServerIP >> 24) & 0xFF;

		M2M_INFO( "DNS lookup -> host: %s, IP addr:%u.%u.%u.%u\r\n", pu8DomainName, host_ip[0], host_ip[1], host_ip[2], host_ip[3]);

		if (gPingRequired == 1)
		{
			m2m_ping_req(u32ServerIP, 0, pingCb);
		}
		else
		{
			uint8 next_command[] ="IPCON";
			uint8 protocol[] ="TCP";

			memcpy(gstrAt_cmdContent.au8Cmd,next_command,sizeof(next_command));
			gstrAt_cmdContent.u8NumOfParameters = 5;
			memcpy(gstrAt_cmdContent.au8ParamsList[0],protocol,sizeof(protocol));
			sprintf((char *)gstrAt_cmdContent.au8ParamsList[1],"%u.%u.%u.%u", host_ip[0],host_ip[1],host_ip[2],host_ip[3]);
			memcpy(gstrAt_cmdContent.au8ParamsList[2],gHTTPport, AT_MAX_PARAM_LENGTH);
			gstrAt_cmdContent.au8ParamsList[3][0] = HEX2ASCII(gTLS);
			gstrAt_cmdContent.au8ParamsList[4][0]= HEX2ASCII(gHTTPChecksum);
			gu8CmdStatus = AT_CMD_STATUS_RUN;
		}
	}
	gPingRequired = 0;
}

void m2m_wifi_socket_handler(SOCKET sock, uint8_t u8Msg, void *pvMsg)
{
	// Check for socket events
	if (u8Msg == SOCKET_MSG_BIND) {
		tstrSocketBindMsg *pstrBind = (tstrSocketBindMsg *)pvMsg;
		if (pstrBind && pstrBind->status == 0) {
			M2M_INFO("Bind success!");
			if(sock < TCP_SOCK_MAX)
				//TCP socket
				listen(sock,0);
			else 
			{
				//UDP socket
				M2M_INFO("Available CIDs:");
				PrintSockConnections(first_SockConn);
				recvfrom(sock, gau8SocketTestBuffer, MAIN_WIFI_M2M_BUFFER_SIZE, 0);
			}
		} else {
			close(sock);
			DeleteSockConn(&first_SockConn,sock);
			M2M_INFO("Available CIDs:");
			PrintSockConnections(first_SockConn);
			M2M_INFO("ERROR: Bind error!");
		}
	} else if (u8Msg == SOCKET_MSG_LISTEN) {
		tstrSocketListenMsg *pstrListen = (tstrSocketListenMsg *)pvMsg;
		if (pstrListen && pstrListen->status == 0) {
			M2M_INFO("Listen success!");
			M2M_INFO("Available CIDs:");
			PrintSockConnections(first_SockConn);
			accept(sock, NULL, NULL);
		} else {
			M2M_INFO("ERROR: Listen error!");
			close(sock);
			DeleteSockConn(&first_SockConn,sock);
			printf("Available CIDs:\n");
			PrintSockConnections(first_SockConn);
		}
	} else if (u8Msg == SOCKET_MSG_ACCEPT) {
		tstrSocketAcceptMsg *pstrAccept = (tstrSocketAcceptMsg *)pvMsg;
		if (pstrAccept) {
			uint8 sockCID;
			M2M_INFO("Accept success!");
			sockCID = AddSockConn(&first_SockConn, pstrAccept->sock, pstrAccept->strAddr.sin_port, pstrAccept->strAddr.sin_addr.s_addr);

			if(sockCID == 0xFF)
				M2M_INFO("ERR:Failed to add CID");
			else
			{
				M2M_INFO("Adding CID %d", sockCID);
				M2M_INFO("Socket Peer IP = %u.%u.%u.%u\r\n", pstrAccept->strAddr.sin_addr.s_addr & 0xff, (pstrAccept->strAddr.sin_addr.s_addr >> 8) & 0xff, (pstrAccept->strAddr.sin_addr.s_addr >> 16) & 0xff, pstrAccept->strAddr.sin_addr.s_addr >> 24);
				PrintSockConnections(first_SockConn);
				recv(pstrAccept->sock, gau8SocketTestBuffer, MAIN_WIFI_M2M_BUFFER_SIZE, 0);
			}
		} else {
			M2M_INFO("ERROR: Accept error!");
			close(sock);
		}
	} else if (u8Msg == SOCKET_MSG_CONNECT) {
		tstrSocketConnectMsg *pstrConnect = (tstrSocketConnectMsg *)pvMsg;
		if (pstrConnect) {
			if(pstrConnect->s8Error)
			{
				tstrSockErr Err;
				get_error_detail(pstrConnect->sock, &Err);
				M2M_ERR("Socket connection failed! %d %d %d\n", pstrConnect->s8Error, Err.enuErrSource, Err.u8ErrCode);
				close(pstrConnect->sock);
				DeleteSockConn(&first_SockConn,pstrConnect->sock);
			}
			else
			{
				SockConnInfo *pDestSockConn = NULL;
				pDestSockConn = RetrieveConnectionFromSocket(&first_SockConn, pstrConnect->sock);

				if(gTLS == 1)
					M2M_INFO("Alpn: %d\n", get_alpn_index(pstrConnect->sock));
				if(pDestSockConn->u16port == MQTT_PORT)
				{
					// Hardcoded MQTT connect message
					uint8 data_connect[26] = {0x10,0x18,0x00,0x04,0x4d,0x51,0x54,0x54,0x04,0x02,0x00,0x0a,0x00,0x0c,0x57,0x49,0x4e,0x43,0x33,0x34,0x30,0x30,0x5f,0x50,0x75,0x62};

					if(send(pstrConnect->sock, data_connect, sizeof(data_connect), 0) == M2M_SUCCESS)
					M2M_INFO("MQTT Connect packet sent");
					else
					M2M_INFO("ERROR: MQTT Connect error!");
				}
				recv(pstrConnect->sock, gau8SocketTestBuffer, MAIN_WIFI_M2M_BUFFER_SIZE, 0);
			}
			M2M_INFO("Available CIDs:");
			PrintSockConnections(first_SockConn);
		}
	} else if (u8Msg == SOCKET_MSG_SECURE) {
		tstrSocketConnectMsg *pstrConnect = (tstrSocketConnectMsg *)pvMsg;
		if (pstrConnect) {
			if(pstrConnect->s8Error)
			{
				tstrSockErr Err;
				get_error_detail(pstrConnect->sock, &Err);
				M2M_ERR("Socket connection failed! %d %d %d\n", pstrConnect->s8Error, Err.enuErrSource, Err.u8ErrCode);
				close(pstrConnect->sock);
				DeleteSockConn(&first_SockConn,pstrConnect->sock);
			}
			else
			{
				M2M_INFO("Socket securing succeeded! Sock:%d\n", pstrConnect->sock);
				M2M_INFO("Alpn: %d\n", get_alpn_index(pstrConnect->sock));
			}
		}
	} else if (u8Msg == SOCKET_MSG_RECV || u8Msg == SOCKET_MSG_RECVFROM) {
		uint8 u8RemoteIP[4];
		uint32 rmt_ip;
		uint16 rmt_port;
		tstrSocketRecvMsg *pstrRx = (tstrSocketRecvMsg *)pvMsg;
		SockConnInfo *pRemoteSockConn = NULL;

		if (pstrRx->pu8Buffer && pstrRx->s16BufferSize) {
			*(pstrRx->pu8Buffer+pstrRx->s16BufferSize) = '\0';

			pRemoteSockConn = RetrieveConnectionFromSocket(&first_SockConn, sock);

			if(pRemoteSockConn==NULL)
				M2M_INFO("ERROR: Receiving data from an unknown CID");

			if(sock < TCP_SOCK_MAX) 
			{
				// TCP socket
				if(gHTTPChecksum && sock == gHTTPsock)
				{
					uint8 * recv_chunk_ptr;
					uint16 recv_chunk_len;

					if(gHTTPInfo.first_chunk)
					{
						char* tmp_ptr;
						//Get the content-length
						tmp_ptr = strstr((char *)pstrRx->pu8Buffer, "Content-Length: ");
						//point to space before length
						tmp_ptr = strstr((char *)tmp_ptr, " "); 
						gHTTPInfo.content_len = atoi(tmp_ptr+1);

						tmp_ptr = strstr((char *)pstrRx->pu8Buffer, "\r\n\r\n");
						if(tmp_ptr) {
							recv_chunk_ptr = (uint8 *)(tmp_ptr+sizeof("\r\n\r\n")-1);
							recv_chunk_len =  pstrRx->s16BufferSize - ( (tmp_ptr - (char *)pstrRx->pu8Buffer) + 4);
						} else {
							M2M_ERR("ERROR: Unexpected condition in HTTP RSP!\r\n");
							recv_chunk_ptr = NULL;
							recv_chunk_len = 0;
						}
						gHTTPInfo.first_chunk = 0;
					}
					else {
						recv_chunk_ptr = pstrRx->pu8Buffer;
						recv_chunk_len = pstrRx->s16BufferSize;
					}
					if(recv_chunk_len > 0) {
						gHTTPInfo.rx_count += recv_chunk_len;
						M2M_INFO( "%lu bytes received\n", gHTTPInfo.rx_count);
						SHA1_UPDATE(&gHTTPInfo.sha1_ctxt, recv_chunk_ptr, (uint32)recv_chunk_len);
					}

					// Prepare next buffer reception
					memset(gau8SocketTestBuffer,0,sizeof(gau8SocketTestBuffer));
					recv(sock, gau8SocketTestBuffer, MAIN_WIFI_M2M_BUFFER_SIZE, 0);

					if(gHTTPInfo.rx_count >= gHTTPInfo.content_len)
					{
						uint32 i;
						M2M_INFO( "EOF: Total bytes received = %lu\n", gHTTPInfo.rx_count);
						SHA1_FINISH(&gHTTPInfo.sha1_ctxt, gHTTPInfo.digest);
						for(i=0; i<sizeof(gHTTPInfo.digest); i++) {
							printf("%02x ", gHTTPInfo.digest[i]);
						}
						printf("\n\n");
					}
				} else
				{
					M2M_INFO("%s3[%d, %u, %s]", ESC_DATA_SEQ, pRemoteSockConn!=NULL?pRemoteSockConn->u8CID:0xff , (pstrRx->s16BufferSize), pstrRx->pu8Buffer);
					// Prepare next buffer reception
					memset(gau8SocketTestBuffer,0,sizeof(gau8SocketTestBuffer));
					recv(sock, gau8SocketTestBuffer, MAIN_WIFI_M2M_BUFFER_SIZE, 0);
				}
			} else {
				// UDP socket
				rmt_ip = _ntohl(pstrRx->strRemoteAddr.sin_addr.s_addr);
				rmt_port = _ntohs(pstrRx->strRemoteAddr.sin_port);
				u8RemoteIP[0] = (uint8) ((rmt_ip & 0xFF000000)>>24);
				u8RemoteIP[1] = (uint8) ((rmt_ip & 0xFF0000)>>16);
				u8RemoteIP[2] = (uint8) ((rmt_ip & 0xFF00)>>8);
				u8RemoteIP[3] = (uint8) (rmt_ip & 0xFF);

				M2M_INFO("%s5[%d, %d.%d.%d.%d, %u, %u, %s]", ESC_DATA_SEQ, pRemoteSockConn->u8CID, u8RemoteIP[0], u8RemoteIP[1], u8RemoteIP[2], u8RemoteIP[3], rmt_port, (pstrRx->s16BufferSize), pstrRx->pu8Buffer);

				// Prepare next buffer reception
				memset(gau8SocketTestBuffer,0,sizeof(gau8SocketTestBuffer));
				recvfrom(sock, gau8SocketTestBuffer, MAIN_WIFI_M2M_BUFFER_SIZE, 0);
			}
		} else {
			if (pstrRx->s16BufferSize == SOCK_ERR_TIMEOUT) {
				// Prepare next buffer reception
				memset(gau8SocketTestBuffer,0,sizeof(gau8SocketTestBuffer));
				if(sock < TCP_SOCK_MAX) 
					//TCP socket
					recv(sock, gau8SocketTestBuffer, MAIN_WIFI_M2M_BUFFER_SIZE, 0);
				else 
					//UDP socket
					recvfrom(sock, gau8SocketTestBuffer, MAIN_WIFI_M2M_BUFFER_SIZE, 0);
			}
			if(
				(pstrRx->s16BufferSize == SOCK_ERR_CONN_ABORTED)
				|| (pstrRx->s16BufferSize == 0)
			) {
				tstrSockErr Err;
				get_error_detail(sock, &Err);
				M2M_ERR("Socket %d closed%s. %d %d\n",
						sock,
						(pstrRx->s16BufferSize == SOCK_ERR_CONN_ABORTED) ? " by remote server" : "",
						Err.enuErrSource,
						Err.u8ErrCode);
				close(sock);
				DeleteSockConn(&first_SockConn,sock);
			}
		}
	}
}

static void OtaUpdateCb(uint8 u8OtaUpdateStatusType ,uint8 u8OtaUpdateStatus)
{
	M2M_INFO("%d %d\n",u8OtaUpdateStatusType,u8OtaUpdateStatus);

	if(u8OtaUpdateStatus == OTA_STATUS_SUCCESS) {
		if(u8OtaUpdateStatusType == DL_STATUS) {
			sint8 s8tmp;
			tstrM2mRev strtmp;

//			M2M_INFO("Curr driver HIF Level: %u\n", M2M_HIF_LEVEL);
			s8tmp = m2m_ota_get_firmware_version(&strtmp);
			if (M2M_ERR_FW_VER_MISMATCH == s8tmp)
			{
				// TODO application should update the host driver before switching to the upgraded firmware
			}

			M2M_INFO("OTA Firmware ver   : %u.%u.%u Svnrev %u\n", strtmp.u8FirmwareMajor, strtmp.u8FirmwareMinor, strtmp.u8FirmwarePatch,strtmp.u16FirmwareSvnNum);
			M2M_INFO("OTA Min driver ver : %u.%u.%u\n", strtmp.u8DriverMajor, strtmp.u8DriverMinor, strtmp.u8DriverPatch);
			M2M_INFO("OTA Curr driver ver: %u.%u.%u Svnrev %u\n", M2M_RELEASE_VERSION_MAJOR_NO, M2M_RELEASE_VERSION_MINOR_NO, M2M_RELEASE_VERSION_PATCH_NO,M2M_RELEASE_VERSION_SVN_VERSION);
			M2M_INFO("OTA Firmware Build %s Time %s\n",strtmp.BuildDate,strtmp.BuildTime);

			//switch to the upgraded firmware
			m2m_ota_switch_firmware();
		} else if(u8OtaUpdateStatusType == SW_STATUS) {
			M2M_INFO("Now OTA successfully done\n");
			M2M_INFO("Now will reset the system...");
			AT_SEND_OK("OTA_Update");
			system_reset();	
		} else if (u8OtaUpdateStatusType == RB_STATUS) {
			M2M_INFO("Rollback successfully done\n");
			M2M_INFO("Now will reset the system...");
			AT_SEND_OK("OTA_RollBack");
			system_reset();	
        } else if (u8OtaUpdateStatusType == HFD_STATUS) {
            AT_SEND_OK("HOST_OTA_Update started");
        }
	} else if (u8OtaUpdateStatus == OTA_STATUS_ABORTED) {
		M2M_INFO("OTA aborted by application");
	} else if (u8OtaUpdateStatus == OTA_STATUS_UPDATE_INPROGRESS){
		M2M_INFO("OTA status update in progress");
	} else if (u8OtaUpdateStatus == OTA_STATUS_ALREADY_ENABLED) {
		M2M_INFO("OTA status is already enabled");
	} else {
		M2M_INFO("ERROR:OTA Status:%d", u8OtaUpdateStatus);
		AT_SEND_ERROR(u8OtaUpdateStatus,"OTA");
	}
}

static void OtaNotifCb(tstrOtaUpdateInfo *pv)
{
    M2M_INFO("\n");
}

volatile uint32_t ms_systicks = 0;

void SysTick_Handler(void)
{
	ms_systicks++;
}

uint32 get_systicks()
{
	return ms_systicks;
}

sint32 GetScanTime(void)
{
	return (GetScanTimeStart()?(sint32)(ms_systicks - GetScanTimeStart()):-1);
}

sint32 GetDownloadTime(void)
{
	return (GetDownloadStartTime()?(sint32)(ms_systicks - GetDownloadStartTime()):-1);
}
static void wifi_cb(uint8 u8MsgType, void * pvMsg)
{
	uint8 au8CmdTxt[AT_MAX_CMD_LENGTH]={0};
	sint32 s32Scantime = 0;
	//AT_SEND_ERROR(u8MsgType,"WIFI CB");
	if (u8MsgType == M2M_WIFI_RESP_SCAN_DONE){
		tstrM2mScanDone *pstrInfo = (tstrM2mScanDone*) pvMsg;
		get_cmd_txt(AT_INDEX_SCAN, (uint8 *)au8CmdTxt);

		if(pstrInfo->s8ScanState == M2M_SUCCESS){
			s32Scantime = GetScanTime();
			uint8 index = strlen((const char *)au8CmdTxt);
			au8CmdTxt[index++]= '#';
			au8CmdTxt[index++]=((pstrInfo->u8NumofCh/10)+'0');
			au8CmdTxt[index++]=((pstrInfo->u8NumofCh%10)+'0');
			au8CmdTxt[index++]='\0';
			AT_SEND_OK(au8CmdTxt);
			M2M_INFO(">> Scan Time: %ld ms\r\n",s32Scantime);
			if(pstrInfo->u8NumofCh > 0) {
				M2M_INFO("\r#%02d%-9s\r\n",pstrInfo->u8NumofCh," ");
				M2M_INFO(">>#  %-3s %-4s %s %-17s SSID\r\n","RI","SEC","CH","BSSID");
			}
			gu8Index = 0;
			gu8Sleep = PS_WAKE;
			if (pstrInfo->u8NumofCh >= 1) {
				m2m_wifi_req_scan_result(gu8Index);
				gu8Index++;
			}
			else {
				M2M_INFO("No AP Found Rescan\n");
			}
		}
		else {
			AT_SEND_ERROR(pstrInfo->s8ScanState, au8CmdTxt);
		}
	}
	else if (u8MsgType == M2M_WIFI_RESP_SCAN_RESULT) {
		tstrM2mWifiscanResult		*pstrScanResult =(tstrM2mWifiscanResult*)pvMsg;
		uint8						u8NumFoundAPs = m2m_wifi_get_num_ap_found();

		char* authType;
		switch(pstrScanResult->u8AuthType)
		{
			case M2M_WIFI_SEC_OPEN:
				authType = "OPEN";
				break;
			case M2M_WIFI_SEC_WPA_PSK:
				authType = "WPA ";
				break;
			case M2M_WIFI_SEC_WEP:
				authType = "WEP ";
				break;
			case M2M_WIFI_SEC_802_1X:
				authType = "EAP ";
				break;
			default:
				authType = "?   ";
				break;
		}

		M2M_INFO(">>%02d %d %s %02d %02X:%02X:%02X:%02X:%02X:%02X %s\r\n",
			pstrScanResult->u8index+1,pstrScanResult->s8rssi,
			authType,pstrScanResult->u8ch,
			pstrScanResult->au8BSSID[0], pstrScanResult->au8BSSID[1], pstrScanResult->au8BSSID[2],
			pstrScanResult->au8BSSID[3], pstrScanResult->au8BSSID[4], pstrScanResult->au8BSSID[5],
			pstrScanResult->au8SSID
		);

		if(gu8Index < u8NumFoundAPs) {
			gu8Sleep = PS_WAKE;
			m2m_wifi_req_scan_result(gu8Index);
			gu8Index++;
		}
		else {
			gu8Sleep = PS_REQ_SLEEP;
		}
	}
	else if (u8MsgType == M2M_WIFI_RESP_CON_STATE_CHANGED) {
		tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged*) pvMsg;
		//M2M_INFO("Wifi State :: %s ::\r\n", pstrWifiState->u8CurrState ? "CONNECTED" : "DISCONNECTED");

		switch(pstrWifiState->u8CurrState)
		{
			case M2M_WIFI_CONNECTED:
				M2M_INFO("Wifi State :: CONNECTED ::\r\n");
				break;
			case M2M_WIFI_DISCONNECTED:
				M2M_INFO("Wifi State :: DISCONNECTED ::\r\n");
				break;
			case M2M_ERR_CONN_INPROGRESS:
				M2M_INFO("Wifi Error :: CONNECTION IN PROGRESS ::\r\n");
				break;
			case M2M_WIFI_ROAMED:
				M2M_INFO("Wifi State :: ROAMED ::\r\n");
				break;
		}

		if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
			gu8Sleep = PS_WAKE;

			if(use_static_IP)
			{
				get_cmd_txt(AT_INDEX_CONN, (uint8 *)au8CmdTxt);

				if(M2M_SUCCESS != m2m_wifi_set_static_ip(&StaticIPcfg)) 
					AT_SEND_ERROR(AT_ERR_OPERATION_NOT_COMPLETED, (uint8 *)au8CmdTxt);
				else
				{
					gstrAllStatus.u8Sta_status = SERVICE_IS_RUNNING;
					M2M_INFO("STA IP is static\r\n");
					AT_SEND_OK(au8CmdTxt);

					gu32IPAddress = StaticIPcfg.u32StaticIP;
				}
				use_static_IP = 0;
			}
#ifdef ETH_MODE
			if(gstrWifiParam.strEthInitParam.u8EthernetEnable == M2M_WIFI_MODE_ETHERNET) {
				gstrAllStatus.u8Sta_status = SERVICE_IS_RUNNING;
			}
#endif
		}
		else if(pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
			gu8Sleep = PS_WAKE;
			if(gstrAllStatus.u8Sta_status == SERVICE_IS_RUNNING) {
				gstrAllStatus.u8Sta_status = SERVICE_IS_STOPPED;
				get_cmd_txt(AT_INDEX_DISCONN, (uint8 *)au8CmdTxt);
				AT_SEND_OK(au8CmdTxt);
			}
			else {
				if(gstrAllStatus.u8Ap_status == SERVICE_DEV_IS_CONNECTED) {
					get_cmd_txt(AT_INDEX_DISCONN, (uint8 *)au8CmdTxt);
					gstrAllStatus.u8Ap_status = SERVICE_IS_RUNNING;
					AT_SEND_OK(au8CmdTxt);
				}
				else {
					if (gstrAllStatus.u8Ap_status == SERVICE_IS_UNDEFINED) {
						get_cmd_txt(AT_INDEX_AP_DISABLE, (uint8 *)au8CmdTxt);
						AT_SEND_OK(au8CmdTxt);
						gstrAllStatus.u8Ap_status = SERVICE_IS_STOPPED;
					}
					else {
						get_cmd_txt(AT_INDEX_CONN, (uint8 *)au8CmdTxt);
						AT_SEND_ERROR(AT_ERR_OPERATION_NOT_COMPLETED, (uint8 *)au8CmdTxt);
					}
				}
			}
		}
	}
	else if(u8MsgType == M2M_WIFI_RESP_DEFAULT_CONNECT)	{
		tstrM2MDefaultConnResp	*pstrResp = (tstrM2MDefaultConnResp*)pvMsg;
		get_cmd_txt(AT_INDEX_DEF_CONN, (uint8 *)au8CmdTxt);
		if((pstrResp->s8ErrorCode == M2M_DEFAULT_CONN_EMPTY_LIST) || (pstrResp->s8ErrorCode == M2M_DEFAULT_CONN_SCAN_MISMATCH))	{
			M2M_INFO("Cached Networks not found or empty");
			AT_SEND_ERROR(pstrResp->s8ErrorCode,au8CmdTxt);
		}
	}
	else if(u8MsgType == M2M_WIFI_REQRSP_DELETE_APID) {
		tstrM2MGenericResp *pstrResp = (tstrM2MGenericResp*)pvMsg;
		if(pstrResp->s8ErrorCode == M2M_SUCCESS)
			M2M_INFO("Credential Delete Req succeeded");
		else
			M2M_INFO("Credential Delete Req failed");
	}
	else if (u8MsgType == M2M_WIFI_REQ_DHCP_CONF) {
		char	ipAddr[20];
		tstrM2MIPConfig* pstrM2MIpConfig = (tstrM2MIPConfig*) pvMsg;
		uint8 *pu8IPAddress = (uint8*) &pstrM2MIpConfig->u32StaticIP;

		gu32IPAddress = pstrM2MIpConfig->u32StaticIP;

		if(gstrAllStatus.u8Ap_status == SERVICE_IS_RUNNING) {
			gstrAllStatus.u8Ap_status = SERVICE_DEV_IS_CONNECTED;
			M2M_INFO("STA IP Address \"%u.%u.%u.%u\"\r\n",pu8IPAddress[0],pu8IPAddress[1],pu8IPAddress[2],pu8IPAddress[3]);
		}
		else {
			gstrAllStatus.u8Sta_status = SERVICE_IS_RUNNING;
			M2M_INFO("DHCP IP Address \"%u.%u.%u.%u\"\r\n",pu8IPAddress[0],pu8IPAddress[1],pu8IPAddress[2],pu8IPAddress[3]);
			gu8Sleep = PS_REQ_SLEEP;
		}

		/* Send the IP Address along with the '+OK' Response.
		*/
		sprintf(ipAddr, "%u.%u.%u.%u",pu8IPAddress[0],pu8IPAddress[1],pu8IPAddress[2],pu8IPAddress[3]);
		AT_SEND_OK(ipAddr);
	}
	else if (u8MsgType == M2M_WIFI_REQ_DHCP_FAILURE) {
		AT_SEND_ERROR(u8MsgType, "Failed to renew or obtain DHCP IP Address\r\n");
	}
	else if(u8MsgType == M2M_WIFI_RESP_CONN_INFO)
	{
		tstrM2MConnInfo		*pstrConnInfo = (tstrM2MConnInfo*)pvMsg;

		M2M_INFO("SSID  : %s\n",pstrConnInfo->acSSID);
		M2M_INFO("RSSI  : %d\n",pstrConnInfo->s8RSSI);
		M2M_INFO("SEC   : %d\n",pstrConnInfo->u8SecType);
		M2M_INFO("IP ADDR: %u.%u.%u.%u\n",pstrConnInfo->au8IPAddr[0],pstrConnInfo->au8IPAddr[1],
			pstrConnInfo->au8IPAddr[2],pstrConnInfo->au8IPAddr[3]);
		M2M_INFO("MAC ADDRESS : [%2x:%2x:%2x:%2x:%2x:%2x] \n",pstrConnInfo->au8MACAddress[0],pstrConnInfo->au8MACAddress[1],
			pstrConnInfo->au8MACAddress[2],pstrConnInfo->au8MACAddress[3],pstrConnInfo->au8MACAddress[4],
			pstrConnInfo->au8MACAddress[5]);
		char u8Buf[85];
		sprintf(u8Buf,"GET_CONN_INFO,%s,%d,%d,%u.%u.%u.%u,%02X:%02X:%02X:%02X:%02X:%02X", pstrConnInfo->acSSID,pstrConnInfo->s8RSSI,pstrConnInfo->u8SecType,pstrConnInfo->au8IPAddr[0],pstrConnInfo->au8IPAddr[1],
			pstrConnInfo->au8IPAddr[2],pstrConnInfo->au8IPAddr[3],pstrConnInfo->au8MACAddress[0],pstrConnInfo->au8MACAddress[1],
			pstrConnInfo->au8MACAddress[2],pstrConnInfo->au8MACAddress[3],pstrConnInfo->au8MACAddress[4],
			pstrConnInfo->au8MACAddress[5]);
		AT_SEND_OK(u8Buf);
	}
	else if (u8MsgType == M2M_WIFI_RESP_IP_CONFLICT) {
		if (gstrAllStatus.u8Sta_status != SERVICE_IS_STOPPED) {
			gu8Sleep = PS_WAKE;
		}
	}
	else if (u8MsgType == M2M_WIFI_RESP_CURRENT_RSSI) {
		sint8	*rssi = (sint8*)pvMsg;
		get_cmd_txt(AT_INDEX_RSSI, (uint8 *)au8CmdTxt);
		
		gu8Sleep = PS_REQ_SLEEP;
		M2M_INFO("ch rssi %d\r\n",*rssi);
		AT_SEND_OK(au8CmdTxt);
	}
	else if(u8MsgType == M2M_WIFI_RESP_PROVISION_INFO) {
		pstrProvInfo = (tstrM2MProvisionInfo*)pvMsg;

		gstrAllStatus.u8Prov_status = SERVICE_IS_STOPPED;
		if(pstrProvInfo->u8Status == M2M_SUCCESS) {
			get_cmd_txt(AT_INDEX_PROV_ENABLE, (uint8 *)au8CmdTxt);
			AT_SEND_OK(au8CmdTxt);
			nm_bsp_sleep(5000); // Delay 1 - 5 seconds so that the HTTP provisioning server has time to send back the "OK" status response to the client
			m2m_wifi_connect((char*)pstrProvInfo->au8SSID, strlen((const char *)pstrProvInfo->au8SSID), pstrProvInfo->u8SecType,
				pstrProvInfo->au8Password, M2M_WIFI_CH_ALL);
		}
		else if(pstrProvInfo->u8Status == 255) {
			M2M_ERR("Provisioning Terminated\n");
		}
		else {
			M2M_ERR("Provisioning Failed\n");
			AT_SEND_ERROR(pstrProvInfo->u8Status,"PROV_MODE");
		}
	}
	else if(u8MsgType == M2M_WIFI_REQ_WPS) {
		tstrM2MWPSInfo	*pstrWPS = (tstrM2MWPSInfo*)pvMsg;
		gu8Sleep = PS_REQ_SLEEP;
		if(0 != pstrWPS->u8AuthType) {
			get_cmd_txt(AT_INDEX_WPS_START, (uint8 *)au8CmdTxt);
			AT_SEND_OK(au8CmdTxt);
			printf("WPS SSID           : %s\r\n",pstrWPS->au8SSID);
			printf("WPS PSK            : %s\r\n",pstrWPS->au8PSK);
			printf("WPS SSID Auth Type : %s\r\n",pstrWPS->u8AuthType == M2M_WIFI_SEC_OPEN ? "OPEN" : "WPA/WPA2");
			printf("WPS Channel        : %d\r\n",pstrWPS->u8Ch);
			m2m_wifi_connect((char*)pstrWPS->au8SSID, (uint8)m2m_strlen(pstrWPS->au8SSID),
				pstrWPS->u8AuthType, pstrWPS->au8PSK, pstrWPS->u8Ch);
		}
		else {
			M2M_ERR("WPS Is not enabled OR Timedout\n");
			AT_SEND_ERROR(u8MsgType,"WPS_MODE");
		}
	}
	else if(u8MsgType == M2M_WIFI_RESP_GET_SYS_TIME) {
		tstrSystemTime *pstrRsp = (tstrSystemTime *)pvMsg;
		M2M_INFO("Time: %02d:%02d:%02d of %02d/%02d/%02d", pstrRsp->u8Hour,pstrRsp->u8Minute,pstrRsp->u8Second,pstrRsp->u8Day,pstrRsp->u8Month, pstrRsp->u16Year);
	}
	else if(u8MsgType == M2M_WIFI_RESP_GET_PRNG)
	{
		
	}
}

#ifdef TEST_TLS
static void ssl_cb(uint8 u8MsgType, void * pvMsg)
{
	switch(u8MsgType)
	{
		case M2M_SSL_REQ_ECC:
		{
			tstrEccReqInfo *pstrEccREQ = (tstrEccReqInfo *)pvMsg;
			eccProcessREQ(pstrEccREQ);
		}
		break;
		case M2M_SSL_RESP_SET_CS_LIST:
		{
			tstrSslSetActiveCsList *pstrCsList = (tstrSslSetActiveCsList *)pvMsg;
			M2M_INFO("ActiveCS bitmap:%04x\n", pstrCsList->u32CsBMP);
		}
		break;
		case M2M_SSL_RESP_WRITE_OWN_CERTS:
		{
			tenuTlsFlashStatus enuStatus = *(tenuTlsFlashStatus *)pvMsg;
			M2M_INFO("Write TLS Client auth certificate:%d", enuStatus);
		}
		break;
	}
}
#endif

static void disable_pullups(void)
{
	uint32 pinmask;
	pinmask = (
	M2M_PERIPH_PULLUP_DIS_HOST_WAKEUP|
	M2M_PERIPH_PULLUP_DIS_SD_CMD_SPI_SCK|
	M2M_PERIPH_PULLUP_DIS_SD_DAT0_SPI_TXD);

	m2m_periph_pullup_ctrl(pinmask, 0);
}

static void set_dev_name_to_mac(uint8 * name, uint8 * mac_addr)
{
	/* Name must be in the format WINC1500_00:00 */
	uint16 len;

	len = m2m_strlen(name);
	if(len >= 5) {
		name[len-1] = HEX2ASCII((mac_addr[5] >> 0) & 0x0f);
		name[len-2] = HEX2ASCII((mac_addr[5] >> 4) & 0x0f);
		name[len-4] = HEX2ASCII((mac_addr[4] >> 0) & 0x0f);
		name[len-5] = HEX2ASCII((mac_addr[4] >> 4) & 0x0f);
	}
}


#define N_HTTPS_FILES									(sizeof(gpacHTTPSFileList)/ 256)

typedef struct{
	tstrHTTPClient	*hHTTPClientHandle;
	uint32			u32RxCount;
	char			acFileName[256];
}tstrHTTPSession;
#define URL_MAX_LEN 256
static volatile uint8	gbHttpClientTestActive	= 0;
static volatile uint8	gbIsHTTPSSessionRunning	= 0;
static volatile	uint8	gbIsTestFinished		= 0;
static tstrHTTPSession	gstrHTTPSSession;
static char				gpacHTTPSFileList[][URL_MAX_LEN] =
{
	"http://www.google.com",
	//"http://192.168.0.103:8090/ncf_ota/m2m_ota_3a0.bin",
	//"https://www.hsbc.com.eg/1/PA_esf-ca-app-content/content/pws/eg/egypt/pdf/electronic-banking-terms-and-conditions.pdf",
	//"https://ssl.gstatic.com/gb/images/b_8d5afc09.png",
	//"https://www.google.com.eg/images/srpr/logo11w.png",
	//"https://sni.velox.ch",
};

void set_download_url(char* u8Buf)
{
	memcpy(gpacHTTPSFileList[0],u8Buf,URL_MAX_LEN);
}
/***********************************************************/
void http_client_test_init(void)
{
	memset(&gstrHTTPSSession, 0, sizeof(tstrHTTPSession));
	gbIsHTTPSSessionRunning	= 0;
	gbIsTestFinished		= 0;
	gbHttpClientTestActive	= 1;
	HTTP_ClientInit();
	gstrSslOptions.bIsSessionCachingEnabled = 1;
	
	M2M_ERR("Test Init\r\n");
}

/***********************************************************/
static sint8 httpsClientCallback(uint8 *pu8Chunk, sint16 s16ChunkSize, uint32 u32Arg)
{
	tstrHTTPSession		*pstrSession = (tstrHTTPSession*)u32Arg;
	static uint32 u32FileSize = 0;
	float fDownloadTime = 0.0;
	float fBitRate = 0.0;

	if(pu8Chunk == NULL)
	{
		if(s16ChunkSize == HTTP_CLIENT_CONNECTED)
		{
			/* Add Call back action */
		}
		else if(s16ChunkSize == HTTP_CLIENT_DOWNLOADING)
		{
			u32FileSize = gstrHTTPSSession.hHTTPClientHandle->strRxMsg.u32ContentLength;
			M2M_INFO("File size: %lu B\n",u32FileSize);
		}
		else if(s16ChunkSize == HTTP_CLIENT_FINISHED)
		{
			fDownloadTime = (float)GetDownloadTime()/(float)1000;
			fBitRate = u32FileSize/(1024 * fDownloadTime);
			M2M_INFO("\"%s\" Success\n", pstrSession->acFileName);
			HTTP_ClientStop(pstrSession->hHTTPClientHandle);
			M2M_INFO("Download time = %10.3f s @ Rate: %5.3f KB/s\n", fDownloadTime,fBitRate);
			gbIsHTTPSSessionRunning = 0;
			AT_SEND_OK("FILE_DOWNLOAD");
		}
		else if(s16ChunkSize == HTTP_CLIENT_CONNECTION_ERROR)
		{
			M2M_INFO("\"%s\" Fail!\n", pstrSession->acFileName);
			HTTP_ClientStop(pstrSession->hHTTPClientHandle);
			gbIsHTTPSSessionRunning = 0;
			AT_SEND_ERROR(s16ChunkSize, "FILE_DOWNLOAD");
		}
	}
	return 0;
}

/***********************************************************/
static void at_http_test_task(void)
{
	static uint32	u32ServerIdx = 0;

	if(!GetFiledownloadStarted())
	return;

	if(!gbHttpClientTestActive)
	return;

	if(!gbIsTestFinished)
	{
		if(!gbIsHTTPSSessionRunning)
		{
			if(u32ServerIdx < N_HTTPS_FILES)
			{
				gbIsHTTPSSessionRunning		= 1;
				gstrHTTPSSession.u32RxCount = 0;
				strcpy(gstrHTTPSSession.acFileName, gpacHTTPSFileList[u32ServerIdx]);
				gstrHTTPSSession.hHTTPClientHandle =
				HTTP_ClientConnect(gpacHTTPSFileList[u32ServerIdx], HTTP_CLIENT_ACTION_DOWNLOAD, 0, httpsClientCallback,
				(uint32)&gstrHTTPSSession);
				u32ServerIdx ++;
			}
			else
			{
#ifndef __REPEAT_TEST_CONTINUOUSLY__
				M2M_DBG("Test Finished\n");
				system("start .\\Download");
				gbIsTestFinished = 1;
				gbHttpClientTestActive = 0;
#endif
				u32ServerIdx = 0;
			}
		}
	}
}

sint8 winc_reset(uint8 reinit, uint8 bConnect, uint8 mode)
{
    uint8 mac_addr[6];
    uint8 u8IsMacAddrValid;
    tstrM2mLsnInt strM2mLsnInt;
    sint8 s8Ret = M2M_ERR_FAIL;

#ifdef ETH_MODE
    gstrWifiParam.strEthInitParam.u8EthernetEnable = mode;
    M2M_INFO("Mode:%d\n", mode);
#endif

    /* WIFI re-init, forces a deinit before the init */
    if(reinit)
        s8Ret = m2m_wifi_reinit(&gstrWifiParam);
    else
        s8Ret = m2m_wifi_init(&gstrWifiParam);

    if (M2M_SUCCESS != s8Ret) {
        /* Could be really bad, but we will hope it is just a version mismatch
           so we cannot do the AT cmd app stuff, but we can do Serial bridge mode. */
        M2M_ERR("Initialization error(%d): Switching to recovery mode (SB)\r\n", s8Ret);
        if(s8Ret != M2M_ERR_FW_VER_MISMATCH)
        {
            /* Just get framework in place for upgrade. No need to reinit. */
            m2m_wifi_init_hold();
        }
        enter_sb_from_at(); /* Never returns... */
        while(1);
    }

    /* Reconfigure the callbacks and reconnect to the network after completing the WINC reinit */
    m2m_ota_init(OtaUpdateCb, OtaNotifCb);

    disable_pullups();

#ifdef TEST_TLS
    s8Ret = m2m_ssl_init(ssl_cb);
    if (M2M_SUCCESS != s8Ret)
    {
        M2M_ERR("m2m_ssl_init() failed with ret=%d", s8Ret);
        goto EXIT;
    }
#endif

#ifdef TEST_IPERF
    IperfInit();
#endif

    if(IsSocketReady())
        socketDeinit();

    socketInit();

    m2m_wifi_get_otp_mac_address(mac_addr, &u8IsMacAddrValid);
    if(!u8IsMacAddrValid) {
        M2M_INFO("Default MAC\r\n");
        m2m_wifi_set_mac_address(gau8MacAddr);
    } else {
        M2M_INFO("OTP MAC\r\n");
    }
    m2m_wifi_get_mac_address(gau8MacAddr);
    M2M_INFO("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\r\n",
    gau8MacAddr[0],gau8MacAddr[1],gau8MacAddr[2],
    gau8MacAddr[3],gau8MacAddr[4],gau8MacAddr[5]);

    set_dev_name_to_mac((uint8*)gacDeviceName, gau8MacAddr);
//    set_dev_name_to_mac((uint8*)gstrM2MAPConfig.au8SSID, gau8MacAddr);
    m2m_wifi_set_device_name((uint8*)gacDeviceName, (uint8)m2m_strlen((uint8*)gacDeviceName));

#ifdef TEST_POWER_SAVE
    M2M_INFO("M2M_PS_DEEP_AUTOMATIC\r\n");
    m2m_wifi_set_sleep_mode(M2M_PS_DEEP_AUTOMATIC, 1);
#else
    M2M_PRINT("NO POWER SAVE mode\n");
    m2m_wifi_set_sleep_mode(M2M_NO_PS, 1);
#endif

    strM2mLsnInt.u16LsnInt = M2M_LISTEN_INTERVAL;
    m2m_wifi_set_lsn_int(&strM2mLsnInt);

    memset((char *)&gstrAllStatus, 0, sizeof(tstr_test_conn_status));

    if(bConnect)
        if(M2M_SUCCESS != m2m_wifi_default_connect())     /* Don't change s8Ret */
            M2M_ERR("Failed to connect!\n");

EXIT:
    return s8Ret;
}
/***********************************************************/

#ifdef ETH_MODE
static void eth_mode_cb(uint8 u8MsgType, void * pvMsg)
{
    //Handle raw frames
}
#endif

static sint8 init_m2m_app(void)
{
    sint8 s8Ret = M2M_SUCCESS;
	gu8Interrupt = 0;
//    gstrAllStatus.u8Sta_status = SERVICE_IS_STOPPED;
    gu8Sleep = PS_REQ_SLEEP;
    gstrAllStatus.u8Wps_status = WPS_DISABLED;
    gu8Prov = PROV_DISABLED;
    aws_data_t cryptodata;
    uint8 iSockCID;

    memset(gau8SocketTestBuffer,0,sizeof(gau8SocketTestBuffer));
    memset(&gHTTPInfo,0,sizeof(HTTP_RXInfo));
    memset(gHTTPport,0,AT_MAX_PARAM_LENGTH);

    gHTTPChecksum = 0;
    gTLS = 0;
    gHTTPsock = -1;

    nm_bsp_start_timer(timer_cb, USART_TIMEOUT_M_SEC);

#ifdef _STATIC_PS_
    nm_bsp_register_wake_isr(wake_cb, PS_SLEEP_TIME_MS);
#endif
    m2m_memset((uint8*)&gstrWifiParam, 0, sizeof(gstrWifiParam));
    gstrWifiParam.pfAppWifiCb = wifi_cb;

#ifdef ETH_MODE
    static eth_buff[30]; //This needs to be larger but code needs removing to make space (for ETH_MODE only)
    gstrWifiParam.strEthInitParam.pfAppWifiCb = wifi_cb;
    gstrWifiParam.strEthInitParam.pfAppEthCb = eth_mode_cb;
    gstrWifiParam.strEthInitParam.au8ethRcvBuf = eth_buff;
    gstrWifiParam.strEthInitParam.u16ethRcvBufSize = 30;
    /* Start in normal mode. Use winc_reset() to restart WINC in ethernet mode. */
    gstrWifiParam.strEthInitParam.u8EthernetEnable = M2M_WIFI_MODE_NORMAL;
#endif

#ifdef TEST_TLS
    ATCAIfaceCfg tls_atecc508a;

    // Initialize the ATECC508A
    tls_atecc508a = cfg_ateccx08a_i2c_default;

#if defined(__SAML21J18B__) || defined(__SAMD21J18A__)
    // Use SERCOM2 as the WINC uses SERCOM0
    tls_atecc508a.atcai2c.bus = 2;
#elif defined(__SAM4SD32C__)
    // Use TWI0
    tls_atecc508a.atcai2c.bus = 0;
#endif

    tls_atecc508a.atcai2c.slave_address = 0xB0; // AWS provisioned devices use the B0 address

    s8Ret = eccInit(&tls_atecc508a);
    if (M2M_SUCCESS == s8Ret) {
        if(eccReadAWSData(&cryptodata) != ATCA_SUCCESS)
        {
            s8Ret = M2M_ERR_FAIL;
            printf("CryptoReadAWSData() failed with ret=%d\r\n", s8Ret);
            return s8Ret;
        }
    }
#endif

#ifdef TEST_IPERF
    use_static_IP = 0;
    m2m_memset((uint8*)&StaticIPcfg, 0, sizeof(StaticIPcfg));

    first_SockConn = NULL;

    for(iSockCID=0; iSockCID<MAX_SOCKET; iSockCID++)
        sCID[iSockCID] = iSockCID;
#endif

	/*Default Configuration of APP*/
	gtstrApp_config.u8Echo_status  = ENABLED;
	gtstrApp_config.u8Display_hint = ENABLED;

    /* Even if ETH_MODE is defined, start up in normal mode. Use command AT+RESET to switch mode. */
    s8Ret = winc_reset(0, 0, M2M_WIFI_MODE_NORMAL);
    if(M2M_SUCCESS != s8Ret)
        return s8Ret;

    init_at_cmd_handler(gastrAtCMD_Handler);
#ifdef DEBUG_CMD
	print_cmd_handler_arr(gastrAtCMD_Handler);
#endif
	printf("CFG    State"
	   "\r\n------------\r\n");
	printf("ECHO   %s \r\n",(gtstrApp_config.u8Echo_status)?("ENABLED"):("DISABLED"));
	printf("HINT   %s \r\n",(gtstrApp_config.u8Display_hint)?("ENABLED"):("DISABLED"));
	printf("------------\r\n\r\n");

    return M2M_SUCCESS;
}

static void start_m2m_app(void)
{
	sint8 s8Ret = -1;
	uint8 *pu8Data = NULL;

    while (true)
    {
        /*---------------------------------------------------------------------*
         *                         Process AT Command                          *
         *---------------------------------------------------------------------*
         */
		if(gu8IsrFired) 
		{
			nm_bsp_stop_timer();

			if(at_cmd_cfg_is_echo_on())
			{
				printf("%c",gau8Rx_buffer[0]);
            }
			if(!gu8InvalidPrinted)
			{
				printf(":Invalid Input\r\n");
				memset(gau8DataBuffer,0,sizeof(gau8DataBuffer));
				RESET_VAR_TO_0(gu16Data_Index);
				gu8InvalidPrinted = true;
            }
			gu8IsrFired = false;
			nm_bsp_start_timer(timer_cb, USART_TIMEOUT_M_SEC);
        }
		else if(gu8TO)
		{
			printf("TimeOut Current Data Buffer <%s>\r\n",gau8DataBuffer);
			gu8TO = 0;
		}

		uart_readBuff(cdc_uart_module,gau8Rx_buffer);
		switch(gu8CmdStatus){
			case AT_CMD_STATUS_SBMODE:
				m2m_wifi_deinit(NULL);
				enter_sb_from_at();
                break;
			case AT_CMD_STATUS_CHECK:
				printf("\r\n");
#ifdef DEBUG_CMD
					M2M_INFO("\r\n>>Received Data: %s\r\n",gau8DataBuffer);
#endif
				memset((char *)&gstrAt_cmdContent, 0, sizeof(gstrAt_cmdContent));
				s8Ret = parse_cmd(gau8DataBuffer, &gstrAt_cmdContent, gstrAt_cmd_parse_chrs);
#ifdef DEBUG_CMD
					M2M_INFO(">>CMD           : %s\r\n",('\0' == gstrAt_cmdContent.au8Cmd[0])?(char *)"No CMD":(char *)gstrAt_cmdContent.au8Cmd);
					M2M_INFO(">>#Parameters   : %d\r\n",gstrAt_cmdContent.u8NumOfParameters);
#endif
				if((M2M_SUCCESS == s8Ret) || (AT_ERR_CMD_IS_INQUIRY == s8Ret)){
					gu8CmdStatus = AT_CMD_STATUS_RUN;
				} else {
					gu8CmdStatus = AT_CMD_STATUS_IDLE;
					AT_SEND_ERROR(s8Ret, "Handling Error");
					if(AT_ERR_NUM_OF_PARAMS == s8Ret) {//if(AT_ERR_NUM_OF_PARAMS == ret)
						if(ENABLED == gtstrApp_config.u8Display_hint) {
							//uint8 index = get_index_cmdHandler(gstrAt_cmdContent, &gastrAtCMD_Handler);
							atCmd_Inquiries_Handler(&gstrAt_cmdContent, gastrAtCMD_Handler);
						} else {
							M2M_ERR("CMD parameters are more or less than expectation\r\n");
						}
					} else if(AT_ERR_CHR_NUM_OF_CMD == s8Ret) {//if(AT_ERR_NUM_OF_PARAMS == ret)
						M2M_ERR("CMD characters are not as excepected\r\n");
					} else {//if(AT_ERR_NUM_OF_PARAMS == ret)
						M2M_ERR("Invalid CMD\r\n");
					}//if(AT_ERR_NUM_OF_PARAMS == ret)
					memset((char *)gau8DataBuffer, 0, sizeof(gau8DataBuffer));
				}
				break;
			case AT_CMD_STATUS_RUN:
				if(AT_ERR_CMD_IS_INQUIRY != s8Ret) {
					s8Ret = get_index_cmdHandler(&gstrAt_cmdContent);
				}
				switch(s8Ret){
					case AT_INDEX_CONFIG:
						pu8Data = (uint8 *)&gtstrApp_config;
						break;
					case AT_INDEX_WDTEST_INIT:
						pu8Data = (uint8 *)&tcp_cb;
						gu8Sleep = PS_WAKE;
						break;
					default:
						pu8Data = (uint8 *)&gstrAllStatus;
						break;
				}

				if((0 <= s8Ret) && (AT_ERR_CMD_IS_INQUIRY != s8Ret)) {
					/* TCP/UDP have variable number of parameters */
					if(gstrAt_cmdContent.u8NumOfParameters <= gastrAtCMD_Handler[s8Ret].u8NoOfParameters)
					{
						s8Ret = gastrAtCMD_Handler[s8Ret].at_cmd_handler(&gstrAt_cmdContent, pu8Data, 0);
					}
					else
					{
						s8Ret = AT_ERR_NUM_OF_PARAMS;
					}
				} else if (AT_ERR_CMD_IS_INQUIRY == s8Ret) {
					s8Ret = atCmd_Inquiries_Handler(&gstrAt_cmdContent, gastrAtCMD_Handler);
				}
				print_Handle_status(s8Ret, gstrAt_cmdContent.au8Cmd);
				memset((char *)gau8DataBuffer, 0, sizeof(gau8DataBuffer));
				memset((char *)&gstrAt_cmdContent, 0, sizeof(gstrAt_cmdContent));
				gu8CmdStatus = AT_CMD_STATUS_IDLE;
				break;
			case AT_CMD_STATUS_IDLE:
				/* handle Nothing*/
				break;
			default:/*Corruption in memory*/
				M2M_ERR(">_< Unknown Command Status: %d >_< \r\n",gu8CmdStatus); 
				memset((char *)gau8DataBuffer, 0, sizeof(gau8DataBuffer));
				memset((char *)&gstrAt_cmdContent, 0, sizeof(gstrAt_cmdContent));
				gu8CmdStatus = AT_CMD_STATUS_IDLE;
				break;
		}

#if 0
        if(ENABLED == gtstrApp_config.restart_app){
            break;
        }
#endif

#ifdef TEST_IPERF
        IperfUpdate();
#endif

        while(m2m_wifi_handle_events(NULL) != M2M_SUCCESS) {
        }

        at_http_test_task();

#if (defined _STATIC_PS_)||(defined _DYNAMIC_PS_)
        if (
            (gstrAllStatus.u8Sta_status == SERVICE_IS_RUNNING) &&
            (gstrAllStatus.u8Wps_status == WPS_DISABLED) &&
            (gu8Prov == PROV_DISABLED)
        ) {
            if (gu8Sleep == PS_REQ_SLEEP)
            {
#ifdef _STATIC_PS_
                m2m_wifi_request_sleep();
                gu8Sleep = PS_SLEEP;
                nm_bsp_wake_ctrl(1);
#endif
                nm_bsp_enable_mcu_ps();
            }
        }
#endif
    }
}

static void init_uart(void)
{
	configure_uart(&cdc_uart_module, USART_MODE, UART_BAUDRATE);
	configure_uart_callbacks(&cdc_uart_module,uart_tx_isr,uart_rx_isr);
	system_interrupt_enable_global();
}

int main (void)
{
    sint8 ret = M2M_SUCCESS;

    system_init();

    /* Initialize the UART console. */
    init_uart();

    delay_init();

    /* Enable SysTick interrupt for non busy wait delay. */
    if (SysTick_Config(system_cpu_clock_get_hz() / 1000)) {
        puts("main: SysTick configuration error!");
        return 0;
    }

    /*Print Welcome Message and starting of initialization*/
    printf(STRING_HEADER);
    printf("Initializing BSP...");

    ret = nm_bsp_init();
    if (ret != M2M_SUCCESS) {
        M2M_ERR("\r\nFailed to initialize BSP.");
        AT_SEND_ERROR(ret,"BSP Init");
        goto HALT;
    }

    ret = nm_bsp_app_init();
    if (ret != M2M_SUCCESS) {
        M2M_ERR("\r\nFailed to initialize BSP App.");
        AT_SEND_ERROR(ret,"BSP App Init");
        goto HALT;
    }

    puts("DONE\r\n");
    printf("Initializing APP...\n");

    ret = init_m2m_app();
    if(M2M_SUCCESS != ret) {
        M2M_ERR("\r\nFailed to initialize APP.");
        AT_SEND_ERROR(ret,"APP Init");

        /* If all else fails, try to load serial bridge */
        if(ret != M2M_ERR_FW_VER_MISMATCH)
        {
            /* Just get framework in place for upgrade */
            m2m_wifi_init_hold();
        }
        enter_sb_from_at(); /* Never returns */
        goto HALT;
    }

    puts("DONE\r\n");
    AT_SEND_OK("Init");
    printf("\rNow,You can send CMD or use HELP for help.\r\n");

    // Main loop
    start_m2m_app();

#if 0
    if(ENABLED == gtstrApp_config.u8Restart_app){
        printf("\r\n\n Restarting APP...\r\n");
        gtstrApp_config.u8Restart_app = DISABLED;
        goto START;
    }
#endif

HALT:
    while(1){
        /*Infinite Loop*/
    }
}
