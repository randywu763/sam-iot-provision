/*
 * uart_cmds.h
 *
 * Created: 10/1/2014 11:30:12 AM
 *  Author: aali
 */ 

/*
	For Security Key for WEP and Enterprise you Must use '*' as a separator as defined in 
	AT+
		CONN=5[SSID, SSID_LENGTH, SEC_TYPE, SEC_KEY, CHANNEL]//AT+CONN=5[IOT_10,14,2,1234567890,255]
		DISCONN
		SCAN=1[CHANNEL]
		RSSI
		AP_EN=5[SSID, SSID_VISIBLE_MODE, SEC_TYPE, SEC_KEY, CHANNEL]//AP_EN=6[SSID_ssid,0,1,NK,255,192.168.1.1]
		AP_DIS
		WPS_START=2[TRIGGER_TYPE,PIN_CODE]
		WPS_DIS
		PROV_EN=8[DEMO_AP,0,3,1*1234567890,1,192.168.0.1,ATMELCONFIG.COM,1] //[Awad_Prov,0,1,0*1234567890,1,192*168*0*1,awadconfig.com,1]
		PROV_DIS
		WDTEST_INIT
		WDTEST_DEINIT
		
		CFG=2[COMMAND,VALUE]
			 AS:
				[ECHO,0:1]
				[RST,0:1]
				[HINT,0:1]
	\r\n
*/
#ifndef _AT_CMDS_H_
#define _AT_CMDS_H_

#include "common/include/nm_common.h"
#include "socket/include/socket.h"
#include "Hash.h"

#define ENABLE_PRINT 0

#if (ENABLE_PRINT == 1)
#	define PRINT printf
#else
#	define PRINT M2M_DBG
#endif

#define AT_SEND_ERROR(val,cmdTxt)	printf("\r\n+ERROR: %d\r\n,%s\r\n", val, (char *)cmdTxt)
#define AT_SEND_OK(cmdTxt)			printf("\r\n+OK\r\n,%s\r\n", (char *)cmdTxt)

#define AT_HEADER					"AT+"


#define AT_MAX_PARAMETERS_COUNT		(8)
#define AT_MIN_CMD_LENGTH			(3)
#define AT_MAX_CMD_LENGTH			(15)
#define AT_MAX_PARAM_LENGTH			(200)
#define AT_MAX_HOST_NAME			(64)

#define AT_MAX_RX_BUFF_SIZE ((AT_MAX_PARAM_LENGTH * AT_MAX_PARAMETERS_COUNT) + AT_MAX_CMD_LENGTH)
#define BASE_16						(16)

enum at_cmd_status
{
	AT_CMD_STATUS_IDLE	= 0,
	AT_CMD_STATUS_CHECK,
	AT_CMD_STATUS_RUN,
	AT_CMD_STATUS_SBMODE
};

enum at_cmd_error
{
	AT_ERR_CMD_IS_INQUIRY		= 111,
	AT_ERR_NO_ERROR				= M2M_SUCCESS,
	AT_ERR_INVALID_OPERATION	= -13,
	AT_ERR_INVALID_CMD,
	AT_ERR_NOT_REGISTERED_CMD,
	AT_ERR_CHR_NUM_OF_CMD,
	AT_ERR_NUM_OF_PARAMS,
	AT_ERR_INVALID_ARGUMENTS,
	AT_ERR_OPERATION_NOT_COMPLETED,
	AT_ERR_NOT_REGISTERED_CFG,
};

enum at_cmd_index {
	AT_INDEX_CONFIG		= 0,
	AT_INDEX_LIST,
	AT_INDEX_HELP,
	AT_INDEX_CONN,
	AT_INDEX_DEF_CONN,
	AT_INDEX_DISCONN,
	AT_INDEX_SCAN,
	AT_INDEX_SCAN_REGION,
	AT_INDEX_SCAN_OPTIONS,
	AT_INDEX_SCAN_SSID_LIST,
	AT_INDEX_RSSI,
	AT_INDEX_AP_ENABLE,
	AT_INDEX_AP_DISABLE,
	AT_INDEX_PROV_ENABLE,
	AT_INDEX_PROV_DISABLE,
#ifdef LED_BTN_DEMO
	AT_INDEX_WDTEST_INIT,
	AT_INDEX_WDTEST_DEINIT,
#endif
	AT_INDEX_WPS_START,
	AT_INDEX_WPS_DISABLE,
	AT_INDEX_IPCON,
	AT_INDEX_SDS,
	AT_INDEX_DELCID,
	AT_INDEX_OTA,
	AT_INDEX_GETVER,
	AT_INDEX_RESET,
	AT_INDEX_MON_EN,
	AT_INDEX_MON_DIS,
	AT_INDEX_PS_MODE,
	AT_INDEX_STATIC_IP,
	AT_INDEX_GET_CONN_INFO,
	AT_INDEX_SET_PWR_PRO,
	AT_INDEX_PIPERF_START,
	AT_INDEX_PIPERF_STOP,
	AT_INDEX_PIPERF_CREATE,
	AT_INDEX_HTTPCON,	
	AT_FILEDOWNLOAD,
	AT_SCAN_PASSIVE,
	AT_INDEX_ENABLE_EXP_CHECK,
	AT_TLS_CRL_RESET,
	AT_TLS_CRL_ADD,
	AT_TLS_CRL_SEND,
	AT_INDEX_TLS_SET_CS,
	AT_INDEX_TLS_WCERT,
	AT_SSL_OPT,
	AT_PING,
	AT_INDEX_GETIME,
    AT_TX_PWR,
    AT_INDEX_SET_TX_RATE,
    AT_INDEX_HFD_GET,
    AT_INDEX_HFD_READ,
    AT_INDEX_HFD_ERASE,
	AT_INDEX_ROAMING,
	AT_INDEX_NTP,
    AT_INDEX_1X_TLS_CRED,
    AT_INDEX_1X_CONN,
    AT_INDEX_1X_OPTIONS,
	AT_INDEX_TLS_START,
	AT_INDEX_API_CONFIG,
	AT_INDEX_API_APCONN,
	AT_INDEX_XOSLEEP_TOGGLE,
	AT_MAX_COMMANDS_COUNT /* Always keep this at the last entry */
};
enum
{
	AT_CFG_INDEX_ECHO,
	AT_CFG_INDEX_HINT,
	AT_CFG_INDEX_RST,
	AT_MAX_CFG_CMD_COUNT /* Always keep this at the last entry */
};

// Local enum for connection types in AT cmd app, needs to be mapped to the specific driver in use, eg 5==2 on Winc but not Rio
typedef enum {
	CONN_WIFI_SEC_INVALID = 0,
	/*!< Invalid security type. */
	CONN_WIFI_SEC_OPEN,
	/*!< Wi-Fi network is not secured. */
	CONN_WIFI_SEC_WPA_PSK,
	/*!< Wi-Fi network is secured with WPA/WPA2 personal(PSK). */
	CONN_WIFI_SEC_WEP,
	/*!< Security type WEP (40 or 104) OPEN OR SHARED. */
	CONN_WIFI_SEC_802_1X,
	/*!< Wi-Fi network is secured with WPA/WPA2 Enterprise.IEEE802.1x. */
	CONN_WIFI_SEC_WPA2_PSK,
	/*!< Wi-Fi network is secured with WPA2 personal(PSK) - RIO platforms process WPA and WPA2 differently. */
	CONN_WIFI_SEC_NUM_AUTH_TYPES
	/*!< Upper limit for enum value. */
} tenuSecType;


typedef struct at_cmd_parse_chrs
{
	uint8 u8Header_sep[2];
	uint8 u8Num_ofParams_sep[2];
	uint8 u8Params_enc_Start_sep[2];
	uint8 u8Params_enc_End_sep[2];
	uint8 u8Params_sep[2];
	uint8 u8Help_param_sep[3];
	//uint8 in_param_sep[2];
}tstrAt_cmd_parse_chrs;

typedef struct cmd
{
	uint8 au8Cmd[AT_MAX_CMD_LENGTH];
	uint8 u8NumOfParameters;
	uint8 au8ParamsList[AT_MAX_PARAMETERS_COUNT][AT_MAX_PARAM_LENGTH];
}tstrAt_cmd_content;

typedef sint8 (*tpf_at_cmd_handler)(tstrAt_cmd_content*, void *, uint8);

typedef struct str_At_Cmd
{
	uint8 au8AtCMD[AT_MAX_CMD_LENGTH];
	tpf_at_cmd_handler at_cmd_handler;
	uint8 u8NoOfParameters;
	uint32_t* moredata;
}strAtCMD_Handler;

typedef struct SockConn
{
	uint8				u8CID;
	SOCKET				sock;
	uint32				addr;
	uint16				u16port;
	struct SockConn	*next_SockConn;
}SockConnInfo;


#define _AT_MAX_SUBTYPE 15
#define _AT_MAX_CONTROL_SUBTYPE 8
#define _AT_MANAGEMENT_MAX_SUBTYPR 12
typedef struct {sint8 s8Enabled; sint32 s32NumPkt;} tstrFilterStat;
typedef struct {tstrFilterStat strFilterSubtype[_AT_MAX_SUBTYPE];
sint32 s32AnyDataFrame;
sint32 s32Enabled;} tstrMonstats;


typedef struct HTTP_RX
{
    uint32          rx_count;
	uint16			content_len;
    uint8           first_chunk;
    tstrHashContext sha1_ctxt;
    uint8           digest[160/8];
}HTTP_RXInfo;

uint8 AddSockConn(SockConnInfo **conn, SOCKET csock, uint16 port, uint32 saddr);
void DeleteSockConn(SockConnInfo **conn, SOCKET csock);
SockConnInfo* RetrieveConnectionFromCID(SockConnInfo **conn, uint8 cid);
SockConnInfo* RetrieveConnectionFromSocket(SockConnInfo **conn, uint8 sock);
uint8 GetCID(void);
void FreeCID(uint8 cid);
void PrintSockConnections(SockConnInfo *conn);
uint8 UpdateSockConn(SockConnInfo *conn, SOCKET csock, uint16 port, uint32 saddr);

sint8 atCmd_AppConfig_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_Connect_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_DefaultConnect_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_DisConnect_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_DhcpReq_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_Scan_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_ScanRegion_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_Scan_Options_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_ScanSsidList_Handler(tstrAt_cmd_content *data,void* moreData,uint8 useStoredValue);
sint8 atCmd_CurRSSI_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_ApEnable_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_ApDisable_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_P2pEnable_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_P2pDisable_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_ProvisionEnable_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_ProvisionDisable_Handler(tstrAt_cmd_content *data, void *, uint8);
#ifdef LED_BTN_DEMO
sint8 atCmd_WifiDirectInit_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_WifiDirectDeinit_Handler(tstrAt_cmd_content *data, void *, uint8);
#endif
sint8 atCmd_WpsConnect_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_WpsDisable_Handler(tstrAt_cmd_content *data, void *, uint8);
sint8 atCmd_List_Handler(tstrAt_cmd_content *data, void*, uint8);
sint8 atCmd_Help_Handler(tstrAt_cmd_content *data, void*, uint8);
sint8 atCmd_IPCon_Handler(tstrAt_cmd_content *data, void*, uint8);
sint8 atCmd_SocketDataStream_Handler(tstrAt_cmd_content *data, void*, uint8); 
sint8 atCmd_DeleteCID_Handler(tstrAt_cmd_content *data, void*, uint8);
sint8 atCmd_OTA_Handler(tstrAt_cmd_content *data, void*, uint8);
sint8 atCmd_GetVer_Handler(tstrAt_cmd_content *data, void*, uint8);
sint8 atCmd_Reset_Handler(tstrAt_cmd_content *data, void*, uint8);
sint8 atCmd_SetStaticIP_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_PIperf_Start_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_PIperf_Stop_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_PIperf_Create_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_HTTPCon_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_TLS_SetCipherSuite_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_TLS_CertTransfer_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_GetSysTime_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_TxPwr_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_SetTxRate_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_HostFileGet_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_HostFileRead_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_HostFileErase_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_Roaming_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_NTP_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_1X_TLS_Cred_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_1X_Connect_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_1X_Options_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_TlsStart_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_APIConfig_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_APIAPConn_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_XOEnable_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);

sint8 atCmd_Inquiries_Handler(tstrAt_cmd_content *, strAtCMD_Handler *);
sint8 atCmd_IsParameterNumericOnly(uint8* pData);

void init_at_cmd_handler(strAtCMD_Handler *);
void print_cmd_handler_arr(strAtCMD_Handler *);

sint8 parse_cmd(uint8 *, tstrAt_cmd_content *, tstrAt_cmd_parse_chrs);
sint8 get_index_cmdHandler(tstrAt_cmd_content *);
void print_Handle_status(sint8, uint8 *);
void get_cmd_txt(uint8, uint8 *);
sint8 atCmd_MonEn_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_MonDis_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_PsMode_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_GetConnInfo_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_SetPowerProfile_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_FileDownload_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_ScanPassive_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_TlsCrlReset_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_TlsCrlAdd_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
sint8 atCmd_TlsCrlSend_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
uint8 GetFiledownloadStarted(void);
uint32 GetScanTimeStart(void);

sint8 atCmd_SslEnableCertExpirationCheck(tstrAt_cmd_content *data, void* moreData, uint8 useStoreValue);
sint8 atCmd_Ping(tstrAt_cmd_content *data, void* moreData, uint8 useStoreValue);
uint32 GetDownloadStartTime(void);
sint8 atCmd_SslGlobalOption_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue);
void at_cmd_init(void *param);
sint8 at_cmd_run(const uint8 *input);
int at_cmd_cfg_is_echo_on(void);
#endif /* _AT_CMDS_H_ */