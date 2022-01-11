/*!
@file
		

@brief
		

@author		NMI
@date
@version	1.0
*/

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "socket/include/socket.h"
#include "m2m_test_config.h"

#ifdef UDP_TEST


#ifdef CORTUS_APP
//include "m2m_fw_iface.h"
#else
#ifdef __APP_APS3_CORTUS__
//#include "m2m_fw_iface.h"
#endif
#endif

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define TEST_CMD_TCP_TX_START		0x80
#define TEST_CMD_TCP_RX_START		0x81

#define TEST_STATE_UDP_TX			1
#define TEST_STATE_UDP_RX			2
#define TEST_STATE_TCP_TX			3
#define TEST_STATE_TCP_RX			4

#define TEST_BUFFER_SIZE			1400

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

typedef struct{
	uint32	u32SockID;
	uint32	u32PktSQN;
}tstrSockTestPktHdr;


typedef struct{
	uint8		bIsActive;
	uint8		u8Retries;
	uint16		u16TxSQN;
	uint16		u16RxSQN;
	uint16		u16Timeout;
}tstrSocketTestInfo;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
GLOBALS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/* Receive buffers for test sockets. 
*/
#ifdef __MSP430FR5739__
#pragma DATA_SECTION(gau8SocketTestBuffer, ".FRAM_DATA")
#endif

static uint8	gau8SocketTestBuffer[TEST_BUFFER_SIZE];

/* IP Address of the remote peer. 
*/
static uint32	gu32RemoteIPAddress = 0;


/* Statistics for received packets.
*/
static uint32				gu32TestPacketCount	 = 0;
static tstrSocketTestInfo	gastrTestSocketInfo[MAX_SOCKET];

/* Sockets.
*/
static SOCKET	udpBcastSocket			= -1; 

static uint8	gu8Retx = 0;

static SOCKET	gUdpTxSocket	= -1;
static SOCKET	gUdpRxSocket	= -1;
static SOCKET	gTcpCtrlSocket	= -1;
static SOCKET	gTcpTestSocket	= -1;
static uint8	gu8CurrentState	= 0;


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

NMI_API void UDP_SendBcast(void);

NMI_API void TCP_SocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg);

NMI_API void UDP_SocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg);

NMI_API void SetRemoteIPAddress(uint32 u32IP);

NMI_API SOCKET TCP_StartClient(void);

NMI_API SOCKET UDP_StartClient(void);

NMI_API SOCKET UDP_StartServer(void);

NMI_API void SocketSendTestPacket(SOCKET sockID);

NMI_API void UDP_StartThroughputTest(void);

NMI_API void SocketSendCtrlCMD(uint32 u32Cmd, uint32 u32Arg);

#if defined CORTUS_APP || defined __APP_APS3_CORTUS__

tstrOsTimer		gstrUDPTimer;

#define UDP_TIMER_START()		app_os_timer_start(&gstrUDPTimer, "udp", UDP_Timer, 2, 1, NULL, 0)
#define UDP_TIMER_STOP()		app_os_timer_stop(&gstrUDPTimer)
#define GET_TIME()				app_os_timer_get_tickcount()

NMI_API void UDP_Timer(void *pvArg)
{
	if(gu8Retx)
	{
		gu8Retx = 0;
		SocketSendTestPacket(gUdpTxSocket);
	}
}
#else

#define UDP_TIMER_START()
#define UDP_TIMER_STOP()

#ifdef __MSP430FR5739__
#define GET_TIME()				gu32Jiffies
#else
#define GET_TIME()				(uint16)nm_bsp_get_tick()
#endif

#endif


/*********************************************************************
Function
		SetRemoteIPAddress

Description

Return
		None.

Author
		Ahmed Ezzat

Version
		1.0

Date
	
*********************************************************************/
void SetRemoteIPAddress(uint32 u32IP)
{
	/* Set the Remote IP Address.
	*/
	gu32RemoteIPAddress = u32IP;
	M2M_PRINT("Peer IP Address %u.%u.%u.%u\n",
		(gu32RemoteIPAddress		&	0x000000FF), ((gu32RemoteIPAddress >> 8) &	0x000000FF),
		((gu32RemoteIPAddress >> 16) &	0x000000FF), ((gu32RemoteIPAddress >> 24) &	0x000000FF));

	/* Start Throughput Test. */
	UDP_StartThroughputTest();
}
/*********************************************************************
Function
		UDP_StartServer

Description

Return
		None.

Author
		Ahmed Ezzat

Version
		1.0

Date

*********************************************************************/
void UDP_StartThroughputTest(void)
{
	gUdpTxSocket 	= UDP_StartClient();
	gUdpRxSocket	= UDP_StartServer();
	gTcpCtrlSocket 	= TCP_StartClient();
	UDP_TIMER_START();
	gu8CurrentState	= TEST_STATE_UDP_TX;
}
/*********************************************************************
Function
		UDP_StartServer

Description

Return
		None.

Author
		Ahmed Ezzat

Version
		1.0

Date

*********************************************************************/
SOCKET UDP_StartServer(void)
{
	struct sockaddr_in	addr;
	SOCKET				UdpServerSocket;

	/* Create the Rx Socket.
	*/
	UdpServerSocket = socket(AF_INET,SOCK_DGRAM,0);
	if(UdpServerSocket >= 0)
	{
		gastrTestSocketInfo[UdpServerSocket].bIsActive		= 1;
		gastrTestSocketInfo[UdpServerSocket].u16TxSQN		= 0;
		gastrTestSocketInfo[UdpServerSocket].u16RxSQN		= 0;
		gastrTestSocketInfo[UdpServerSocket].u16Timeout		= 0;

		addr.sin_family			= AF_INET;
		addr.sin_port			= _htons(UDP_SERVER_PORT);
		addr.sin_addr.s_addr	= 0;

		if(bind(UdpServerSocket, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))
		{
			M2M_ERR("Bind Error\n");
		}
	}
	return UdpServerSocket;
}
/*********************************************************************
Function
		UDP_StartClient

Description

Return
		None.

Author
		Ahmed Ezzat

Version
		1.0

Date

*********************************************************************/
SOCKET UDP_StartClient(void)
{
	SOCKET				UdpClientSocket = -1;
	struct sockaddr_in	addr;

	UdpClientSocket = socket(AF_INET,SOCK_DGRAM,0);
	if(UdpClientSocket >= 0)
	{
		tstrSockTestPktHdr	*pstrPktHdr = (tstrSockTestPktHdr*)gau8SocketTestBuffer;

		gastrTestSocketInfo[UdpClientSocket].bIsActive	= 1;
		gastrTestSocketInfo[UdpClientSocket].u16TxSQN	= 0;
		gastrTestSocketInfo[UdpClientSocket].u16RxSQN	= 0;
		gastrTestSocketInfo[UdpClientSocket].u16Timeout	= 0;

		addr.sin_family			= AF_INET;
		addr.sin_port			= _htons(UDP_CLIENT_PORT);
		addr.sin_addr.s_addr	= gu32RemoteIPAddress;

		/* Build the TEST Packet header.
		*/
		pstrPktHdr->u32SockID	=	((uint32)UdpClientSocket) & 0x000000FF;
		pstrPktHdr->u32PktSQN	=	((uint32)gastrTestSocketInfo[UdpClientSocket].u16TxSQN) & 0x0000FFFF;
		pstrPktHdr->u32SockID	=	_htonl(pstrPktHdr->u32SockID);
		pstrPktHdr->u32PktSQN	=	_htonl(pstrPktHdr->u32PktSQN);

		if(sendto(UdpClientSocket, gau8SocketTestBuffer, TEST_BUFFER_SIZE,0,
			(struct sockaddr*)&addr,sizeof(addr)) < 0)
		{
			M2M_ERR("ERROR Sock %d\n",UdpClientSocket);
		}
		else
		{
			gastrTestSocketInfo[UdpClientSocket].u16TxSQN ++;
			SocketSendTestPacket(UdpClientSocket);
		}
		recvfrom(UdpClientSocket, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 0);
	}
	return UdpClientSocket;
}
/*********************************************************************
Function
		UDP_SendBcast

Description

Return
		None.

Author
		Ahmed Ezzat

Version
		1.0

Date

*********************************************************************/
void UDP_SendBcast(void)
{
	static struct sockaddr_in	addr;
	tstrSockTestPktHdr			*pstrPktHDR = (tstrSockTestPktHdr*)gau8SocketTestBuffer;

	/* Create the socket for the first time.
	*/
	if(udpBcastSocket < 0)
	{
		udpBcastSocket = socket(AF_INET,SOCK_DGRAM,0);
		if(udpBcastSocket < 0)
		{
			M2M_ERR("UDP Client Socket Creation Failed\n");
			return;
		}
	}
	pstrPktHDR->u32SockID	= 0xFFFFFFFF;
	pstrPktHDR->u32PktSQN	= _htonl(gu32TestPacketCount);

	addr.sin_family			= AF_INET;
	addr.sin_port			= _htons(UDP_CLIENT_PORT);
	addr.sin_addr.s_addr	= 0xFFFFFFFF;

	sendto(udpBcastSocket, gau8SocketTestBuffer, sizeof(tstrSockTestPktHdr), 0, (struct sockaddr*)&addr,sizeof(addr));
	recvfrom(udpBcastSocket, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 1000);
}
/*********************************************************************
Function
		TCP_StartClient

Description

Return

Author
		Ahmed Ezzat

Version
		1.0

Date:
*********************************************************************/
SOCKET TCP_StartClient(void)
{
	struct sockaddr_in	addr;
	SOCKET				TcpClientSocket;

	TcpClientSocket = socket(AF_INET,SOCK_STREAM,0);
	if(TcpClientSocket >= 0)
	{
		gastrTestSocketInfo[TcpClientSocket].bIsActive	= 1;
		gastrTestSocketInfo[TcpClientSocket].u16TxSQN	= 0;
		gastrTestSocketInfo[TcpClientSocket].u16RxSQN	= 0;
		gastrTestSocketInfo[TcpClientSocket].u16Timeout	= 0;
		gastrTestSocketInfo[TcpClientSocket].u8Retries	= 2;

		addr.sin_family			= AF_INET;
		addr.sin_port			= _htons(TCP_SERVER_PORT);
		addr.sin_addr.s_addr	= gu32RemoteIPAddress;
		connect(TcpClientSocket,(struct sockaddr*)&addr,sizeof(addr));
	}
	return TcpClientSocket;
}
/*********************************************************************
Function
		SocketSendCtrlCMD

Description

Return
		None.

Author
		Ahmed Ezzat

Version
		1.0

Date

*********************************************************************/
void SocketSendCtrlCMD(uint32 u32Cmd, uint32 u32Arg)
{
	tstrSockTestPktHdr		*pstrPktHdr = (tstrSockTestPktHdr*)gau8SocketTestBuffer;

	/* Build the TEST Packet header.
	*/
	pstrPktHdr->u32SockID	=	_htonl(u32Cmd);
	pstrPktHdr->u32PktSQN	=	_htonl(u32Arg);
	if(send(gTcpCtrlSocket, gau8SocketTestBuffer, sizeof(tstrSockTestPktHdr), 0) < 0)
	{
		M2M_ERR("Send ERROR\n");
	}
	recv(gTcpCtrlSocket, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 10 * 1000);
}
/*********************************************************************
Function
		SocketBuildTestPacket

Description

Return
		None.

Author
		Ahmed Ezzat

Version
		1.0

Date

*********************************************************************/
void SocketSendTestPacket(SOCKET sockID)
{
	if((sockID >= 0) && (sockID < MAX_SOCKET))
	{
		if(gastrTestSocketInfo[sockID].bIsActive)
		{
			uint8				*pu8UdpTxBuffer = gau8SocketTestBuffer;
			tstrSockTestPktHdr	*pstrPktHdr = (tstrSockTestPktHdr*)pu8UdpTxBuffer;

			/* Build the TEST Packet header.
			*/
			pstrPktHdr->u32SockID	=	((uint32)sockID) & 0x000000FF;
			pstrPktHdr->u32PktSQN	=	((uint32)gastrTestSocketInfo[sockID].u16TxSQN) & 0x0000FFFF;
			pstrPktHdr->u32SockID	=	_htonl(pstrPktHdr->u32SockID);
			pstrPktHdr->u32PktSQN	=	_htonl(pstrPktHdr->u32PktSQN);
			if(send(sockID, pu8UdpTxBuffer, TEST_BUFFER_SIZE, 0) < 0)
			{
				//M2M_ERR("ERROR Sock %d\n",sockID);
				gu8Retx = 1;
			}
			else
			{
				/* Update the TX SQN.
				*/
				gastrTestSocketInfo[sockID].u16TxSQN ++;
				gu8Retx = 0;
			}
		}
	}
}
/*********************************************************************
Function
		UDP_SocketEventHandler

Description


Return
		None.

Author
		Ahmed Ezzat

Version
		1.0

Date

*********************************************************************/
void UDP_SocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg)
{
	if(u8Msg == SOCKET_MSG_SENDTO)
	{
		if(sock == gUdpTxSocket)
		{
			SocketSendTestPacket(sock);
		}
	}
	else if(u8Msg == SOCKET_MSG_RECVFROM)
	{
		uint8				bCloseSocket = 0;
		tstrSocketRecvMsg	*pstrRx = (tstrSocketRecvMsg*)pvMsg;

		if(pstrRx->pu8Buffer != NULL)
		{
			/* Set the remote IP Address. 
			*/
			if(gu32RemoteIPAddress == 0)
			{
				close(udpBcastSocket);
				SetRemoteIPAddress(pstrRx->strRemoteAddr.sin_addr.s_addr);
			}
			else
			{
				if(sock == gUdpTxSocket)
				{
					uint32	u32PacketStart;

					m2m_memcpy((uint8*)&u32PacketStart, pstrRx->pu8Buffer, 4);
					u32PacketStart = _htonl(u32PacketStart);
					if(u32PacketStart != 0xDEADBEAF)
					{
						recvfrom(sock, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 0);
					}
					else
					{
						bCloseSocket = 1;
					}
				}
				else if(sock == gUdpRxSocket)
				{
					if(gastrTestSocketInfo[sock].u16RxSQN == 0)
					{
						gastrTestSocketInfo[sock].u16Timeout = GET_TIME();
					}

					if(++ gastrTestSocketInfo[sock].u16RxSQN == gu32TestPacketCount)
					{
						gastrTestSocketInfo[sock].u16Timeout =
								GET_TIME() - gastrTestSocketInfo[sock].u16Timeout;

						bCloseSocket = 1;
					}
					else
					{
						recvfrom(sock, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 10 * 1000);
					}
				}
			}
		}
		else
		{
			/* Recv failed. */
			if(pstrRx->s16BufferSize == SOCK_ERR_TIMEOUT)
			{
				if(gu32RemoteIPAddress == 0)
				{
					UDP_SendBcast();
					recvfrom(sock, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 1000);
				}
				else if(sock == gUdpRxSocket)
				{
					M2M_ERR("TIMEOUT\n");
					bCloseSocket = 1;
					gastrTestSocketInfo[sock].u16Timeout = 1;
				}
			}
			else
			{
				bCloseSocket = 1;
			}
		}

		if(bCloseSocket)
		{
			if(sock == gUdpTxSocket)
			{
				M2M_INFO("SOCKET   %02d    Next TX SQN = %04d\n",sock,gastrTestSocketInfo[sock].u16TxSQN);
				m2m_memset((uint8*)&gastrTestSocketInfo[sock], 0, sizeof(tstrSocketTestInfo));
				UDP_TIMER_STOP();

				close(gUdpTxSocket);
				gUdpTxSocket	= -1;

				recvfrom(gUdpRxSocket, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 15 * 1000);
			}
			else if(sock == gUdpRxSocket)
			{
				SocketSendCtrlCMD(TEST_CMD_TCP_TX_START, gastrTestSocketInfo[sock].u16Timeout);
				M2M_INFO("(%02d)RX = %04d  Time = %u  SIZE = %d\n",
						sock, gastrTestSocketInfo[sock].u16RxSQN,
						gastrTestSocketInfo[sock].u16Timeout, pstrRx->s16BufferSize);

				close(gUdpRxSocket);
				gUdpRxSocket = -1;
				gu8CurrentState = TEST_STATE_UDP_RX;
			}
		}
	}
}
/*********************************************************************
Function 
		TCP_SocketEventHandler

Description


Return
		None.

Author
		Ahmed Ezzat

Version
		1.0

Date

*********************************************************************/
void TCP_SocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg)
{
	if(u8Msg == SOCKET_MSG_CONNECT)
	{
		tstrSocketConnectMsg	*pstrConnect = (tstrSocketConnectMsg*)pvMsg;
		if(pstrConnect->s8Error == 0)
		{
			if(sock != gTcpCtrlSocket)
				recv(sock, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 15 * 1000);
		}
		else
		{
			if(gastrTestSocketInfo[sock].u8Retries > 0)
			{
				struct sockaddr_in	addr;

				addr.sin_family			= AF_INET;
				addr.sin_port			= _htons(TCP_SERVER_PORT);
				addr.sin_addr.s_addr	= gu32RemoteIPAddress;
				connect(sock, (struct sockaddr*)&addr, sizeof(addr));
				gastrTestSocketInfo[sock].u8Retries --;
			}
			else
			{
				M2M_ERR("Connect Error %d  SOCKET = %d\n",pstrConnect->s8Error,sock);
				close(sock);
			}
		}
	}
	else if(u8Msg == SOCKET_MSG_SEND)
	{
		if(sock == gTcpCtrlSocket)
		{
			if(gu8CurrentState == TEST_STATE_UDP_RX)
			{
				gTcpTestSocket = TCP_StartClient();
				recv(sock, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 0);
				gu8CurrentState = TEST_STATE_TCP_RX;
			}
			else if(gu8CurrentState == TEST_STATE_TCP_RX)
			{
				recv(sock, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 0);
			}
			else if(gu8CurrentState == TEST_STATE_TCP_TX)
			{
				close(gTcpCtrlSocket);
				close(gTcpTestSocket);
				gTcpCtrlSocket 	= -1;
				gTcpTestSocket 	= -1;
				gbUdpTestActive = 0;
			}
		}
		else
		{
			SocketSendTestPacket(sock);
			recv(sock, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 15 * 1000);
		}
	}
	else if(u8Msg == SOCKET_MSG_RECV)
	{
		tstrSocketRecvMsg	*pstrRecv = (tstrSocketRecvMsg*)pvMsg;
		if(gastrTestSocketInfo[sock].bIsActive)
		{
			if(pstrRecv->s16BufferSize <= 0)
			{
				close(gTcpCtrlSocket);
				close(gTcpTestSocket);
				gTcpCtrlSocket 	= -1;
				gTcpTestSocket 	= -1;
				gUdpRxSocket 	= -1;
				m2m_memset((uint8*)gastrTestSocketInfo, 0, MAX_SOCKET * sizeof(tstrSocketTestInfo));
				gbUdpTestActive = 0;
			}
			else
			{
				if(sock != gTcpCtrlSocket)
				{
					if(gastrTestSocketInfo[sock].u16RxSQN == 0)
					{
						gastrTestSocketInfo[sock].u16Timeout = GET_TIME();
					}

					if(++ gastrTestSocketInfo[sock].u16RxSQN == gu32TestPacketCount)
					{
						uint16					u16Time = GET_TIME() - gastrTestSocketInfo[sock].u16Timeout;

						gastrTestSocketInfo[sock].u16Timeout = u16Time;

						M2M_INFO("(TCP_%02d)RX = %04d  Time = %u  SIZE = %d\n",	sock,
								gastrTestSocketInfo[sock].u16RxSQN,
								u16Time, pstrRecv->s16BufferSize);

						SocketSendCtrlCMD(TEST_CMD_TCP_RX_START, u16Time);
						SocketSendTestPacket(sock);
					}
					else
					{
						recv(sock, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 15 * 1000);
					}
				}
			}
		}
	}
}
/*********************************************************************
Function 

Description

Return
		None.

Author
		Ahmed Ezzat

Version
		1.0

Date

*********************************************************************/
static void TEST_APPSocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg)
{
	if(sock < TCP_SOCK_MAX)
	{
		TCP_SocketEventHandler(sock,u8Msg,pvMsg);
	}
	else
	{
		UDP_SocketEventHandler(sock,u8Msg,pvMsg);
	}
}
/*********************************************************************
Function 

Description

Return
		None.

Author
		Ahmed Ezzat

Version
		1.0

Date

*********************************************************************/
void SocketTest(void)
{
	M2M_PRINT("(APP)Starting Socket Test...\n");
	gu32TestPacketCount		= TEST_PACKET_COUNT;
	udpBcastSocket			= -1;
	gu32RemoteIPAddress		= 0;
	socketInit();
	registerSocketCallback(TEST_APPSocketEventHandler, NULL);
	UDP_SendBcast();
}
#endif
