/*
 * at_cmd_Handler.c
 *
 * Created: 10/2/2014 5:46:05 PM
 *  Author: aali
 */ 

#include "at_cmd_Handler.h"
#include "main.h"
#include "iperf/Iperf.h"
 
HTTP_RXInfo     gHTTPInfo;
uint8           gHTTPport[AT_MAX_PARAM_LENGTH];
uint8           gHTTPChecksum;
uint8           gTLS;
SOCKET          gHTTPsock;
uint32          gu32IPAddress;
uint32          gu32PingCount = 0;
uint8           gPingRequired = 0;
uint8           gHostNameSNI[AT_MAX_HOST_NAME]="";
uint8           gAlpnList[ALPN_LIST_MAX_APP_LENGTH]="";

// This is shared between monitoring mode payload buffer (originally 1500 bytes, called 'gmgmt') and
// enterprise private key buffer (1700 bytes) since these two are unlikely to be used together.
static uint8 someSharedMemory[1700];

static tstr_app_config          at_cmd_config;
static tstrAt_cmd_parse_chrs    AT_CMD_KEYWORDS = {"+","=","[","]",",","/?"};//,"*"};
tstrSslGlobalOptions            gstrSslOptions;
extern tstrWifiInitParam        gstrWifiParam;

#define KEY_CERT_MAX_SIZE 64
struct str_uart_enterprise
{
	char	urlKey[KEY_CERT_MAX_SIZE];
	char	urlCert[KEY_CERT_MAX_SIZE];
	uint8	state;
	uint8*	keyBuff;
	uint16	keyLen;
	uint8	certBuff[1200];
	uint16	certLen;
	uint8*	ptrModulus;
	uint16	modulusLength;
	uint8*	ptrPrivateExponent;
	uint16	privateExponentLength;
}gstrEnterprise;

/************************************************************************/
/* Note: if you want to add a new handler, add its CMD string           */
/*       in gaAt_CMD_Values_arr as last element and its handler function*/
/*	     gaAt_CMD_handler_fn also its number of parameters.             */
/* Also, Add its index in at_cmd_index@.h file							*/
/*                                                                      */
/* Please, Make sure of all indexes of handlers and their commands and  */
/*         everything else. (Its your own responsibility).              */
/************************************************************************/

/************************************************************************/
/* The following uncompilable code is template of handler function      */
/* Included Comment too                                                 */
/************************************************************************/
#if 0
/************************************************************************/
/* atCmd_NameOf_Handler Handler                                                      */
/************************************************************************/
sint8 atCmd_NameOf_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;
	
	PRINT("atCmd_NameOf_Handler\r\n");
	if(gaAt_CMD_handler_fn[AT_INDEX_ofEvent_In_at_cmd_index__enum].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	if(useStoredValue) {
		goto RUN;
	}
	/* Parsing and assignments */
RUN:
	/* Executive Code Here */
EXIT:
	return s8Ret;
}
#endif
static uint8	gau8DataBufferTemp[AT_MAX_RX_BUFF_SIZE]			={0};
static strAtCMD_Handler gaAt_CMD_handler_fn[AT_MAX_COMMANDS_COUNT]=	/*Handler and no of parameters */
{
	{"CFG"			,atCmd_AppConfig_Handler,2},
	{"LIST"			,atCmd_List_Handler,0},
	{"HELP"			,atCmd_Help_Handler,0},
	{"CONN"			,atCmd_Connect_Handler,5},
	{"DEF_CONN"		,atCmd_DefaultConnect_Handler,0},
	{"DISCONN"		,atCmd_DisConnect_Handler,0},
	{"SCAN"			,atCmd_Scan_Handler,1},
	{"SCAN_REGION"	,atCmd_ScanRegion_Handler,1},
	{"SCAN_OPTIONS"	,atCmd_Scan_Options_Handler,0xff},
	{"SCAN_SSIDLIST",atCmd_ScanSsidList_Handler,6},
	{"RSSI"			,atCmd_CurRSSI_Handler,0},
	{"AP_EN"		,atCmd_ApEnable_Handler,6},
	{"AP_DIS"		,atCmd_ApDisable_Handler,0},
	{"PROV_EN"		,atCmd_ProvisionEnable_Handler,8},
	{"PROV_DIS"		,atCmd_ProvisionDisable_Handler,0},
#ifdef LED_BTN_DEMO
	{"WDTEST_INIT"	,atCmd_WifiDirectInit_Handler,0},
	{"WDTEST_DEINIT",atCmd_WifiDirectDeinit_Handler,0},
#endif
	{"WPS_START"	,atCmd_WpsConnect_Handler,2},
	{"WPS_DIS"		,atCmd_WpsDisable_Handler,0},
	{"IPCON"		,atCmd_IPCon_Handler, 5},
	{"SDS"			,atCmd_SocketDataStream_Handler,5},
	{"DELCID"		,atCmd_DeleteCID_Handler, 1},
	{"OTA"			,atCmd_OTA_Handler, 1},
	{"GETVER"		,atCmd_GetVer_Handler, 0},
	{"RESET"		,atCmd_Reset_Handler, 1},
	{"MON_EN"		,atCmd_MonEn_Handler,7},
	{"MON_DIS"		,atCmd_MonDis_Handler,0},
	{"PS_MODE"		,atCmd_PsMode_Handler,2},
	{"STATIC_IP"	,atCmd_SetStaticIP_Handler, 2},
	{"GET_CONN_INFO", atCmd_GetConnInfo_Handler,0},
	{"SET_PWR_PRO", atCmd_SetPowerProfile_Handler,1},
	{"PIPERF_START" ,atCmd_PIperf_Start_Handler, 0xff},
	{"PIPERF_STOP"  ,atCmd_PIperf_Stop_Handler, 0xff},
	{"PIPERF_CREATE",atCmd_PIperf_Create_Handler, 0xff},
	{"HTTPCON"	    ,atCmd_HTTPCon_Handler, 4},		
	{"FILE_DOWNLOAD", atCmd_FileDownload_Handler,1},
	{"SCAN_PASSIVE" ,atCmd_ScanPassive_Handler,2},
	{"EN_CERT_EXPCHK",atCmd_SslEnableCertExpirationCheck,1},
	{"TLS_CRL_RESET",atCmd_TlsCrlReset_Handler, 1},
	{"TLS_CRL_ADD"	,atCmd_TlsCrlAdd_Handler, 2},
	{"TLS_CRL_SEND" ,atCmd_TlsCrlSend_Handler, 0},
	{"TLS_SET_CS"	,atCmd_TLS_SetCipherSuite_Handler, 1},
	{"TLS_WCERT"	,atCmd_TLS_CertTransfer_Handler, 0},
	{"SSL_OPTIONS"  ,atCmd_SslGlobalOption_Handler,5},
	{"PING"         ,atCmd_Ping,2},
	{"GET_TIME"		,atCmd_GetSysTime_Handler,0},
    {"TX_PWR"       ,atCmd_TxPwr_Handler,1},
	{"SET_TX_RATE"	,atCmd_SetTxRate_Handler,1},
    {"HFD_GET"      ,atCmd_HostFileGet_Handler,1},
    {"HFD_READ"     ,atCmd_HostFileRead_Handler,4},
    {"HFD_ERASE"    ,atCmd_HostFileErase_Handler,1},
	{"ROAMING"      ,atCmd_Roaming_Handler,0xff},
	{"NTP"			,atCmd_NTP_Handler,2},
	{"1X_TLS_CRED"  ,atCmd_1X_TLS_Cred_Handler,2},
	{"1X_CONN"		,atCmd_1X_Connect_Handler,5},
	{"1X_OPTIONS"	,atCmd_1X_Options_Handler,4},
	{"TLS_START"	,atCmd_TlsStart_Handler,1},
	{"API_CONFIG"	,atCmd_APIConfig_Handler, 0},
	{"API_APCONN"	,atCmd_APIAPConn_Handler, 0},
	{"XOSLEEP_TOGGLE",atCmd_XOEnable_Handler, 0},
};
static uint8 gaAt_CFG_Values_arr[AT_MAX_CFG_CMD_COUNT][5]=
{
	"ECHO",
	"HINT",
	"RST",
};

extern sint32	gu32IperfPacketCount;
extern uint8	gu8UDPIperfRX, gu8TCPIperfRX, gu8ReceivingUDP;

enum enum_scanlist {
	SCAN_LIST_CH=0,
	SCAN_LIST_NUM_SSID,
	SCAN_LIST_SSID0,
	SCAN_LIST_SSID1,
	SCAN_LIST_SSID2,
	SCAN_LIST_SSID3};
/************************************************************************/
/* Initialization of handler array                                      */
/************************************************************************/
void init_at_cmd_handler(strAtCMD_Handler *pstrAtCMD_Handler)
{
	uint8 u8LoopCntr=0;
	for(u8LoopCntr=0; u8LoopCntr<AT_MAX_COMMANDS_COUNT;u8LoopCntr++) {
		pstrAtCMD_Handler[u8LoopCntr] = gaAt_CMD_handler_fn[u8LoopCntr];
	}
}

/************************************************************************/
/* Run the AT Command Application                                       */
/************************************************************************/
sint8 at_cmd_run(const uint8 *input)
{
    sint8 r = -1, inquiry = 0;
    static tstrAt_cmd_content at_cmd_ctx;
    memset((char *)&at_cmd_ctx, 0, sizeof(at_cmd_ctx));
    M2M_INFO("\r\n>>Received Data: %s\r\n", input);
    r = parse_cmd(input, &at_cmd_ctx, AT_CMD_KEYWORDS);
    M2M_INFO(">>CMD           : %s\r\n",('\0' == at_cmd_ctx.au8Cmd[0])?(char *)"No CMD":(char *)at_cmd_ctx.au8Cmd);
    M2M_INFO(">>#Parameters   : %d\r\n",at_cmd_ctx.u8NumOfParameters);

    if (r == AT_ERR_CMD_IS_INQUIRY){
        inquiry = 1;
    }

    if ((r != M2M_SUCCESS) && (inquiry == 0)){
        AT_SEND_ERROR(r, "Handling Error");
        if (r == AT_ERR_NUM_OF_PARAMS) {
            if (at_cmd_config.u8Display_hint) {
                inquiry = 1;
                } else {
                M2M_ERR("CMD parameters are more or less than expectation\r\n");
            }
            } else if (r == AT_ERR_CHR_NUM_OF_CMD){
            M2M_ERR("CMD characters are not as expected\r\n");
            } else {
            M2M_ERR("Invalid CMD\r\n");
        }
    }
    if ((r == M2M_SUCCESS) || (inquiry == 1)) {
        r = get_index_cmdHandler(&at_cmd_ctx);
        if ( (r == AT_ERR_NOT_REGISTERED_CMD) || ((r == AT_ERR_NUM_OF_PARAMS) && (inquiry == 0)) ) {
            //do nothing
            } else{
            sint8 index = r;
            
            if (1 == inquiry){
                M2M_PRINT("\r\n>>Description of [%s] command:\r\n\r\n", at_cmd_ctx.au8Cmd);
            }
            
            r = gaAt_CMD_handler_fn[index].at_cmd_handler(&at_cmd_ctx, gaAt_CMD_handler_fn[index].moredata, inquiry);
        }
        print_Handle_status(r, at_cmd_ctx.au8Cmd);
    }

    return r;
}

/************************************************************************/
/* Initialization of handler array                                      */
/************************************************************************/
void at_cmd_init(void *param)
{
    uint8 u8LoopCntr=0;

    at_cmd_config.u8Restart_app  = 0;
    at_cmd_config.u8Echo_status  = 1;
    at_cmd_config.u8Display_hint = 1;
    
    for(u8LoopCntr=0; u8LoopCntr<AT_MAX_COMMANDS_COUNT;u8LoopCntr++) {
        if (gaAt_CMD_handler_fn[u8LoopCntr].moredata == NULL) {
            gaAt_CMD_handler_fn[u8LoopCntr].moredata = param;
        }
    }
//    print_cmd_handler_arr();

    M2M_PRINT("CFG    State\r\n" "------------\r\n");
    M2M_PRINT("ECHO   %s \r\n",(at_cmd_config.u8Echo_status)?("ENABLED"):("DISABLED"));
    M2M_PRINT("HINT   %s \r\n",(at_cmd_config.u8Display_hint)?("ENABLED"):("DISABLED"));
    M2M_PRINT("------------\r\n\r\n");
}

int at_cmd_cfg_is_echo_on(void)
{
    return 	(int)(at_cmd_config.u8Echo_status);
}

/************************************************************************/
/* parsing incoming data                                                */
/************************************************************************/
#define HELP_STR    "HELP"
sint8 parse_cmd(uint8 *data,tstrAt_cmd_content * strCmd, tstrAt_cmd_parse_chrs parseChrs)
{
	sint8 s8RetValue				= AT_ERR_NO_ERROR;
	uint8 u8NumOfColon				= 0;
	uint8 *pu8Tok					= NULL;
	uint8 *pu8TempPtrofEq			= NULL;
	uint8 *pu8TempPtrofLeftBrack	= NULL;
	uint8  param_offset				= 0;
	memcpy(gau8DataBufferTemp,data,strlen((const char *)data)+1);// size plus null terminator
	memset((char *)strCmd,0,sizeof(tstrAt_cmd_content));
	
	if(!memcmp(data, HELP_STR, strlen(HELP_STR))){
		M2M_PRINT("HELP COMMAND\r\n");
		memcpy(strCmd->au8Cmd, data, strlen(HELP_STR));
		goto EXIT;
	}

	if(memcmp(data, AT_HEADER, strlen(AT_HEADER))){
		if ((*data) != ESCAPE_CHAR)
		{			
			PRINT("Invalid AT CMD \r\n");
			s8RetValue = AT_ERR_INVALID_CMD;
			goto EXIT;
		} else {
			pu8Tok = (uint8 *)strtok((char *)&data[1], (const char *)parseChrs.u8Num_ofParams_sep);//Ignore <ESC>
		}
	} else {
		pu8Tok = (uint8 *)strtok((char *)&data[3], (const char *)parseChrs.u8Num_ofParams_sep);//Get CMD after AT_CMD_HEADER
	}
	
	if(NULL != pu8Tok) {
		memcpy(strCmd->au8Cmd, pu8Tok, strnlen((const char *)pu8Tok, AT_MAX_CMD_LENGTH)); /*Get CMD string*/
	} else {
		PRINT("Invalid AT CMD \r\n");
		s8RetValue = AT_ERR_INVALID_CMD;
		goto EXIT;
	}
	
	if
	(
		(strlen((const char *)strCmd->au8Cmd) < AT_MIN_CMD_LENGTH) || 
		(strlen((const char *)strCmd->au8Cmd) > AT_MAX_CMD_LENGTH) 
	)
	{ /*Check length of CMD*/
		s8RetValue = AT_ERR_CHR_NUM_OF_CMD;
		goto EXIT;
	}

	pu8TempPtrofEq			= (uint8 *)strstr((const char *)gau8DataBufferTemp, (const char *)parseChrs.u8Num_ofParams_sep);		/*Check if there is = or not*/
	pu8TempPtrofLeftBrack	= (uint8 *)strstr((const char *)gau8DataBufferTemp, (const char *)parseChrs.u8Params_enc_Start_sep);	/*check if there is [ or not*/

	if(pu8TempPtrofEq) 
	{
		if(!pu8TempPtrofLeftBrack) {
			s8RetValue = AT_ERR_NUM_OF_PARAMS;
			goto EXIT;
		}
	} else{
		if(pu8TempPtrofLeftBrack){
			s8RetValue = AT_ERR_NUM_OF_PARAMS;
			goto EXIT;
		}
	}

	if(!pu8TempPtrofEq) /*Check if there are parameters or not*/
	{/*if not set number of parameters = 0 and clear their buffer*/
		strCmd->u8NumOfParameters = 0;
		memset((char *)&strCmd->au8ParamsList[0],0,AT_MAX_PARAM_LENGTH);
		pu8TempPtrofEq	= (uint8 *)strstr((const char *)strCmd->au8Cmd, (const char *)parseChrs.u8Help_param_sep);		/*Check if this command is inquiry or not*/
		if(pu8TempPtrofEq) 
		{
			uint8 cntr = 0;
			while((strCmd->au8Cmd[cntr] != '/') && (strCmd->au8Cmd[cntr] != ' ')) { /*skip spaces and '/'*/
				cntr++;
				if(cntr >= AT_MAX_CMD_LENGTH){
					s8RetValue = AT_ERR_INVALID_CMD;
					goto EXIT;
				}
			}
			strCmd->au8Cmd[cntr]='\0';
			s8RetValue = AT_ERR_CMD_IS_INQUIRY;
		} else {
			/*printf("/? Not found\r\n");*/
		}
		goto EXIT;
	}

	/*Check place of = closer than [ by 1chr at least*/
	if((pu8TempPtrofEq+1) >= pu8TempPtrofLeftBrack){
		/*Invalid Number of parameters*/
		s8RetValue = AT_ERR_INVALID_CMD;
		goto EXIT;
	}

	/*If you are here, there are some parameters*/
	pu8Tok = (uint8 *)strtok(NULL, (const char *)parseChrs.u8Params_enc_Start_sep);/*get number of parameters*/
	if(NULL != pu8Tok) 	{
		strCmd->u8NumOfParameters = (unsigned char)atoi((const char *)pu8Tok);
	} else {
		/**/
	}

	pu8Tok = (uint8 *)strtok(NULL, (const char *)parseChrs.u8Params_enc_Start_sep);
	if(NULL != pu8Tok){
		/*If there are some parameters*/
		if(strCmd->u8NumOfParameters >= 1) 
		{
			if(parseChrs.u8Params_enc_End_sep[0] == pu8Tok[strlen((const char *)pu8Tok)-1]) {
				pu8Tok[strlen((const char *)pu8Tok)-1] = 0;
			} 
			else {
				s8RetValue = AT_ERR_INVALID_CMD;	/*but if there is no end of parameters enclosure, something is wrong*/
				goto EXIT;
			}
		} else {
			s8RetValue = AT_ERR_NUM_OF_PARAMS;	/*but if there is number, something is wrong*/
			goto EXIT;
		}
	} 
	else {
		/*If there are not any parameters*/
		if(0 == strCmd->u8NumOfParameters) 
		{	/*and if there is number of them but is zero*/
			s8RetValue = AT_ERR_NO_ERROR;			/*suppose it is OK*/
		} 
		else {
			s8RetValue = AT_ERR_NUM_OF_PARAMS;	/*but if there is number, something is wrong*/
		}
		goto EXIT;
	}

	pu8Tok = (uint8 *)strtok((char *)pu8Tok, (const char *)parseChrs.u8Params_sep);
	while(NULL != pu8Tok)
	{
		/* If any argument contains a ',' it will be back-slashed. It shouldn't be treated as
		   a parameter separator.
		*/
		memcpy(strCmd->au8ParamsList[u8NumOfColon]+param_offset,pu8Tok,strlen((const char *)pu8Tok));
		if(*(pu8Tok + strlen((const char *)pu8Tok)-1) == '\\')
		{
			param_offset += strlen((const char *)pu8Tok);
			/* Substitute the backslash for the comma */
			*(strCmd->au8ParamsList[u8NumOfColon]+param_offset-1) = 0x2c;
		}
		else
		{
			param_offset = 0;	
			u8NumOfColon++;
		}
		if(strCmd->u8NumOfParameters == u8NumOfColon){
			/* All expected parameters are received.
			*/
			break;
		}
		pu8Tok = (uint8 *)strtok(NULL, (const char *)parseChrs.u8Params_sep);
	}

	if(strCmd->u8NumOfParameters != u8NumOfColon) {
		s8RetValue = AT_ERR_NUM_OF_PARAMS;	/*but if there is number, something is wrong*/
		goto EXIT;
	}
	//free(pu8DupData);
EXIT:
	return s8RetValue;
}

sint8 get_index_cmdHandler(tstrAt_cmd_content *strCmd)
{
	sint8 ret = AT_ERR_NO_ERROR;
	uint8 loopCntr;
	uint8 cmdLength = strlen((const char *)strCmd->au8Cmd);
	M2M_DBG("Searching for CMD handler...\r\n");
	for(loopCntr = 0; loopCntr<AT_MAX_COMMANDS_COUNT; loopCntr++) {
		if(cmdLength != strlen((const char *)gaAt_CMD_handler_fn[loopCntr].au8AtCMD))
			continue;
		if(!memcmp(strCmd->au8Cmd,gaAt_CMD_handler_fn[loopCntr].au8AtCMD,cmdLength)) {
			/*pf_at_cmd_handler = atCMD_Handler[loopCntr].at_cmd_handler;*/
			ret  = loopCntr; 
			break;
		}
	}
	
	if(AT_MAX_COMMANDS_COUNT > loopCntr) {
		/*ret = pf_at_cmd_handler(strCmd);*/
	} else {
		ret = AT_ERR_NOT_REGISTERED_CMD;
	}

//EXIT:
	return ret;
}

void print_cmd_handler_arr(strAtCMD_Handler *astrAtCMD_Handler)
{
	uint8 loopCntr=0;
	printf("\r\n%-12s\t%-20s\t%s\r\n","#","CMD","Handler Ptr.");
	printf("-------------------------------------------------------\r\n");
	for(loopCntr=0; loopCntr<AT_MAX_COMMANDS_COUNT;loopCntr++) {
		printf("%02d%-10s\t%-20s\t%p\r\n",loopCntr+1," ",
		astrAtCMD_Handler[loopCntr].au8AtCMD,astrAtCMD_Handler[loopCntr].at_cmd_handler);
	}
}

void print_Handle_status(sint8 ret, uint8 * cmdTxt)
{
	if(AT_ERR_NO_ERROR == ret) {
		//PRINT("CMD sent Successfully.\r\n");
		/*(NOTE:)Send OK only if handling from callback succeeded*/
	} else {
		AT_SEND_ERROR(ret,cmdTxt);
		switch(ret){
			case AT_ERR_NOT_REGISTERED_CMD:{
				M2M_ERR("Not Registered AT-Command.\r\n");
				break;
			}//case AT_ERR_NOT_REGISTERED_CMD
			case AT_ERR_NOT_REGISTERED_CFG:{
				M2M_ERR("Not Registered CFG-Command.\r\n");
				break;
			}//case AT_ERR_NOT_REGISTERED_CMD
			case AT_ERR_INVALID_CMD:{
				M2M_ERR("Invalid CMD.\r\n");
				break;
			}//case AT_ERR_INVALID_CMD
			case AT_ERR_CHR_NUM_OF_CMD:{
				M2M_ERR("AT-CMD characters are not as expected.\r\n");
				break;
			}//case AT_ERR_TOO_SHORT
			case AT_ERR_INVALID_OPERATION:{
				M2M_ERR("Invalid Operation to Handle.\r\n");
				break;
			}//case AT_ERR_INVALID_OPERATION
			case AT_ERR_NUM_OF_PARAMS:{
				M2M_ERR("Invalid number of Parameters.\r\n");
				break;
			}//case AT_ERR_NOT_EXPECTED_PARAMS
			case AT_ERR_INVALID_ARGUMENTS:{
				M2M_ERR("Invalid Arguments.\r\n");
				break;
			}//case AT_ERR_INVALID_ARGUMENTS
		}//switch(ret)
	}
}

void get_cmd_txt(uint8 ndx, uint8 *txt)
{
	if(ndx >= AT_MAX_COMMANDS_COUNT){
		txt=NULL;
		return;
	}
	//txt = gaAt_CMD_Values_arr[ndx];
	memcpy(txt,gaAt_CMD_handler_fn[ndx].au8AtCMD,strlen((const char *)gaAt_CMD_handler_fn[ndx].au8AtCMD));
}


//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*    Application Runtime Configuration                                 */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Runtime Configuration Handler                                        */
/************************************************************************/
sint8 atCmd_AppConfig_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	tstr_app_config *ptstrApp_config=(tstr_app_config*)moreData;
	uint8 u8LoopCntr = 0, u8CfgCmdLength=0;
	
	PRINT("atCmd_AppConfig_Handler\r\n");
	if(gaAt_CMD_handler_fn[AT_INDEX_CONFIG].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	u8CfgCmdLength = strlen((const char *)gaAt_CFG_Values_arr[u8LoopCntr]);
	for(u8LoopCntr = 0; u8LoopCntr<AT_MAX_CFG_CMD_COUNT; u8LoopCntr++) {
		if(strlen((const char *)data->au8ParamsList[0]) != u8CfgCmdLength) {
			continue;
		}
		if(!memcmp(data->au8ParamsList[0],gaAt_CFG_Values_arr[u8LoopCntr],u8CfgCmdLength)) {
			break;
		}
	}
	
//RUN:
	M2M_INFO("%02d: SET %s TO %s \r\n",u8LoopCntr,gaAt_CFG_Values_arr[u8LoopCntr],
		(atoi((const char *)data->au8ParamsList[1])?("ENABLED"):("DISABLED")));
		
	switch(u8LoopCntr)
	{
		case AT_CFG_INDEX_ECHO: {
			ptstrApp_config->u8Echo_status = atoi((const char *)data->au8ParamsList[1]);
			break;
		}
		case AT_CFG_INDEX_RST: {
			ptstrApp_config->u8Restart_app = atoi((const char *)data->au8ParamsList[1]);
			break;
		}
		case AT_CFG_INDEX_HINT: {
			ptstrApp_config->u8Display_hint = atoi((const char *)data->au8ParamsList[1]);
			break;
		}
		default: {
			s8Ret = AT_ERR_NOT_REGISTERED_CFG;
			break;
		}
	}
	
EXIT:
	if(AT_ERR_NO_ERROR == s8Ret) {
		AT_SEND_OK(gaAt_CFG_Values_arr[u8LoopCntr]);
	}
	
	return s8Ret;
}

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Handlers in Station Mode                                             */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* STATION related Enums, Stucts, and variables                         */
/************************************************************************/
enum enum_connIndexes{
	CONN_INDEX_SSID = 0,
	CONN_INDEX_SSID_LENGTH,
	CONN_INDEX_SEC_TYPE,
	CONN_INDEX_SEC_KEY,
	CONN_INDEX_CHANNEL
};
struct str_uart_conn
{
	uint8	ssid[32];
	uint8	ssidLen;
	uint8	secType;
	uint8	secKey[65];
	uint8*	domain;
	uint8*	username;
	uint8*	password;
	uint8	domainLen;
	uint8	usernameLen;
	uint8	passwordLen;
	uint8	channel;
}gstrConn;

enum enum_conn1XIndexes{
	CONN1X_INDEX_SSID = 0,
	CONN1X_INDEX_SSID_LENGTH,
	CONN1X_INDEX_USERNAMEDOMAIN,
	CONN1X_INDEX_PASSWORD,
	CONN1X_INDEX_CHANNEL
};
/************************************************************************/
/* Disconnect Handler                                                   */
/************************************************************************/
sint8 atCmd_DisConnect_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;
	
	if(gaAt_CMD_handler_fn[AT_INDEX_DISCONN].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	if(M2M_SUCCESS != (s8Ret=m2m_wifi_disconnect())) {
		s8Ret = AT_ERR_OPERATION_NOT_COMPLETED;
	}
	
	if(SERVICE_IS_STOPPED == status->u8Sta_status) {
		AT_SEND_OK(data->au8Cmd);
		printf("Already Disconnected\r\n");
	}
	
EXIT:
	return s8Ret;
}

static sint8 at_wifi_perform_connect(void)
{
	char * pu8Dat;
	sint8 s8Ret = M2M_ERR_FAIL;

	if(CONN_WIFI_SEC_WPA2_PSK == gstrConn.secType)
	{
		gstrConn.secType = CONN_WIFI_SEC_WPA_PSK;
	}
	
	if(CONN_WIFI_SEC_WEP == gstrConn.secType) {
		tstrM2mWifiWepParams wepParams;
		memset(&wepParams, 0, sizeof(wepParams));
		pu8Dat = strtok((char *)gstrConn.secKey,"*");
		wepParams.u8KeyIndx=atoi((const char *)pu8Dat);
		pu8Dat = strtok(NULL,"*");
		wepParams.u8KeySz=strlen((const char *)pu8Dat) + 1;
		memcpy(wepParams.au8WepKey,pu8Dat,strlen((const char *)pu8Dat));
		PRINT("WEP Key Index: %d\r\n"
				"WEP Key      : %s\r\n"
				"WEP Key Sz   : %d\r\n",wepParams.u8KeyIndx,wepParams.au8WepKey,wepParams.u8KeySz);
		s8Ret = m2m_wifi_connect((char *)gstrConn.ssid, gstrConn.ssidLen, gstrConn.secType, (void *)&wepParams, gstrConn.channel);
	} else if(CONN_WIFI_SEC_802_1X == gstrConn.secType) {
		tstr1xAuthCredentials enterParams;
		memset(&enterParams, 0, sizeof(enterParams));
		pu8Dat = strtok((char *)gstrConn.secKey,"*");
		memcpy(enterParams.au8UserName,pu8Dat,strlen((const char *)pu8Dat));
		pu8Dat = strtok(NULL,"*");
		memcpy(enterParams.au8Passwd,pu8Dat,strlen((const char *)pu8Dat));
		PRINT("Enterprise Username: %s\r\n"
		"Enterprise Password: %s\r\n",enterParams.au8UserName,enterParams.au8Passwd);
		s8Ret = m2m_wifi_connect((char *)gstrConn.ssid, gstrConn.ssidLen, gstrConn.secType, (void *)&enterParams, gstrConn.channel);
	} else if((CONN_WIFI_SEC_WPA_PSK == gstrConn.secType) || (CONN_WIFI_SEC_OPEN == gstrConn.secType)) {
		M2M_INFO("WPA key: %s",gstrConn.secKey);
		s8Ret = m2m_wifi_connect((char *)gstrConn.ssid, gstrConn.ssidLen, gstrConn.secType, gstrConn.secKey, gstrConn.channel);
	}
	return s8Ret;
}

/************************************************************************/
/* Connect Handler                                                      */
/************************************************************************/
sint8 atCmd_Connect_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = M2M_ERR_FAIL;
	tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;
	
	PRINT("atCmd_Connect_Handler\r\n");
	
	if(gaAt_CMD_handler_fn[AT_INDEX_CONN].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	if(SERVICE_IS_RUNNING == status->u8Sta_status) { /*WiFi is already connected to AP*/
		s8Ret = AT_ERR_INVALID_OPERATION;
		goto EXIT;
	}
	
	if(SERVICE_IS_RUNNING == status->u8Ap_status) /*Check if AP mode is running*/
	{
		s8Ret = AT_ERR_INVALID_OPERATION;
		goto EXIT;
	}
	
	/*Clear gstrConn first*/
	memset((char *)&gstrConn,0,sizeof(gstrConn));
	/*Get SSID*/
	memcpy(gstrConn.ssid,data->au8ParamsList[CONN_INDEX_SSID],strnlen((const char *)data->au8ParamsList[CONN_INDEX_SSID], sizeof(gstrConn.ssid)));
	/*Get SSID Length*/
	gstrConn.ssidLen = atoi((const char *)data->au8ParamsList[CONN_INDEX_SSID_LENGTH]);
	/*Get SEC Type*/
	gstrConn.secType = atoi((const char *)data->au8ParamsList[CONN_INDEX_SEC_TYPE]);
	/*Get SEC KEY*/
	memcpy(gstrConn.secKey,data->au8ParamsList[CONN_INDEX_SEC_KEY],strnlen((const char *)data->au8ParamsList[CONN_INDEX_SEC_KEY], sizeof(gstrConn.secKey)));
	/*Get CHANNEL*/
	gstrConn.channel = atoi((const char *)data->au8ParamsList[CONN_INDEX_CHANNEL]);
	
	PRINT("\r>%s\r\n>%s\r\n>%d\r\n>%d\r\n>%d\r\n",
		gstrConn.ssid,
		gstrConn.secKey,
		gstrConn.ssidLen,
		gstrConn.secType,
		gstrConn.channel);
	
    s8Ret = at_wifi_perform_connect();
EXIT:
	return s8Ret;
}

sint8 atCmd_SetStaticIP_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = M2M_SUCCESS;
	int parse_static[4];
	uint8 u8LoopCntr, *pu8Dat = NULL;

	PRINT("atCmd_SetStaticIP_Handler\r\n");
	
	if((data->u8NumOfParameters < gaAt_CMD_handler_fn[AT_INDEX_STATIC_IP].u8NoOfParameters)){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	pu8Dat = (uint8 *)strtok((char *)data->au8ParamsList[0],".");

	for(u8LoopCntr=0; u8LoopCntr<4; u8LoopCntr++) {
		if (((parse_static[u8LoopCntr] = atoi((const char *)pu8Dat)) == 0) && (*pu8Dat != 0x30))
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			goto EXIT;
		}
		if(parse_static[u8LoopCntr]>255)
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			goto EXIT;
		}
		pu8Dat = (uint8 *)strtok(NULL,".");
	}
	
	StaticIPcfg.u32StaticIP = _htonl((uint32_t) (parse_static[0]<<24 | parse_static[1]<<16 | parse_static[2]<<8 | parse_static[3]));
	
	pu8Dat = (uint8 *)strtok((char *)data->au8ParamsList[1],".");

	for(u8LoopCntr=0; u8LoopCntr<4; u8LoopCntr++) {
		if (((parse_static[u8LoopCntr] = atoi((const char *)pu8Dat)) == 0) && (*pu8Dat != 0x30))
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			goto EXIT;
		}
		if(parse_static[u8LoopCntr]>255)
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			goto EXIT;
		}
		pu8Dat = (uint8 *)strtok(NULL,".");
	}	
	
	StaticIPcfg.u32SubnetMask = _htonl((uint32_t) (parse_static[0]<<24 | parse_static[1]<<16 | parse_static[2]<<8 | parse_static[3]));// 0xFFFF0000;
	
	use_static_IP = 1;
	AT_SEND_OK(data->au8Cmd);
	
EXIT:
	return s8Ret;
}

static sint8 PIperf_BuildInitConfig(tstrAt_cmd_content *data, tstrIperfInit *init_config)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	uint8 u8LoopCntr, *pu8Dat = NULL;
	uint32_t stream_len;
	
	memset((char *)init_config, 0 , sizeof(tstrIperfInit));
	 
	init_config->port = atoi((char *)data->au8ParamsList[1]);
	
	if(init_config->port == 0)
	{
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT;		
	}
	
	// If command is of the type: AT+PIPERF_START=4[TCP,<port>,<ip>,<stream_len>] or AT+PIPERF_START=6[UDP,<port>,<ip>,<number_pkts>,<pkt_len>,<rate>]
	if(data->u8NumOfParameters>2) 	
	{
		// Extract Server's IP address
		pu8Dat = (uint8 *)strtok((char *)data->au8ParamsList[2],".");
		
		for(u8LoopCntr=0; u8LoopCntr<4; u8LoopCntr++) {
			if(((init_config->ip[u8LoopCntr] = atoi((const char *)pu8Dat)) == 0) && (*pu8Dat != 0x30))
			{
				s8Ret = AT_ERR_INVALID_ARGUMENTS;
				goto EXIT;				
			}
			if(init_config->ip[u8LoopCntr]>255)
			{		
				s8Ret = AT_ERR_INVALID_ARGUMENTS;
				goto EXIT;
			}			
			pu8Dat = (uint8 *)strtok(NULL,".");
		}
	}

	if(data->u8NumOfParameters == 2)
	{
		if(strcmp((const char *)data->au8ParamsList[0],"UDP") == 0)
		{
			init_config->operating_mode = MODE_UDP_SERVER;
		} 
		else if(strcmp((const char *)data->au8ParamsList[0],"TCP") == 0)
		{
			init_config->operating_mode = MODE_TCP_SERVER;
		} 
		else
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			M2M_INFO("ERROR: Protocol should be TCP or UDP!");
			goto EXIT;
		}
	}

	if(strcmp((const char *)data->au8ParamsList[0],"TLS") == 0)
	{
		if(data->u8NumOfParameters != 3)
		{
			s8Ret = AT_ERR_NUM_OF_PARAMS;
			goto EXIT;
		}
		init_config->operating_mode = MODE_TLS_CLIENT;
		init_config->tls = 1;
	}

	if((data->u8NumOfParameters == 4) || (data->u8NumOfParameters == 5))
	{
		if(strcmp((const char *)data->au8ParamsList[0],"TCP") != 0)
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			M2M_INFO("ERROR: Protocol should be TCP!");
			goto EXIT;
		}
		init_config->operating_mode = MODE_TCP_CLIENT;
		stream_len = atoi((char *)data->au8ParamsList[3]);
		
		if(stream_len != 0 && stream_len < IPERF_TX_BUFFER_SIZE)
		{
			init_config->packet_len = stream_len;
		} 
		else
		{
			init_config->packet_len = IPERF_TX_BUFFER_SIZE;
		}
		if(stream_len != 0)
		{
			// TODO: If it is not divisible, for now don't take care of the remaining
			init_config->packets_to_send = stream_len/init_config->packet_len; 
		} 
		else
		{
			// Send indefinitely
			init_config->packets_to_send = -1; 
		}
			
		if (data->u8NumOfParameters == 5)
		{
			init_config->tls = atoi((char *)data->au8ParamsList[4]);
		}
	}
	
	if(data->u8NumOfParameters == 6)
	{
		if(strcmp((const char *)data->au8ParamsList[0],"UDP") != 0)
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			M2M_INFO("ERROR: Protocol should be UDP!");
			goto EXIT;
		}
		init_config->operating_mode = MODE_UDP_CLIENT;
		
		init_config->packets_to_send = atoi((char *)data->au8ParamsList[3]);
		
		if(init_config->packets_to_send == 0)
		{
			// Send indefinitely
			init_config->packets_to_send = -1; 
		}
			
		init_config->packet_len = atoi((char *)data->au8ParamsList[4]);

		if(init_config->packet_len > IPERF_TX_BUFFER_SIZE || init_config->packet_len <= 0)
		{
			init_config->packet_len = IPERF_TX_BUFFER_SIZE;
		}
		
		init_config->data_rate = atoi((char *)data->au8ParamsList[5]);
		
		if(init_config->data_rate >> 31)
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			M2M_INFO("ERROR: Data rate must be positive or 0");
			goto EXIT;
		}
	}	
EXIT:
	return s8Ret;
}

sint8 atCmd_PIperf_Start_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	tstrIperfInit init_config;
	sint8 s8Ret;

	PRINT("atCmd_PIperf_Start\r\n");

	if(data->u8NumOfParameters > 0)
	{
		s8Ret = PIperf_BuildInitConfig(data, &init_config);
	
		if(s8Ret != AT_ERR_NO_ERROR)
			return s8Ret;

		s8Ret = IperfCreate(&init_config, false);
	
		if(s8Ret != AT_ERR_NO_ERROR)
			return AT_ERR_INVALID_OPERATION;
	}
	else
	{
		s8Ret = IperfStart();

		if(s8Ret != AT_ERR_NO_ERROR)
			return AT_ERR_INVALID_OPERATION;
	}

	AT_SEND_OK(data->au8Cmd);

	return AT_ERR_NO_ERROR;
}

sint8 atCmd_PIperf_Create_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	tstrIperfInit init_config;
	sint8 s8Ret;

	PRINT("atCmd_PIperf_Create\r\n");

	if((data->u8NumOfParameters != 2) && (data->u8NumOfParameters != 5) && (data->u8NumOfParameters != 6))
	{
		return AT_ERR_NUM_OF_PARAMS;
	}

	s8Ret = PIperf_BuildInitConfig(data, &init_config);
	
	if(s8Ret != AT_ERR_NO_ERROR)
		return s8Ret;

	s8Ret = IperfCreate(&init_config, true);
	
	if(s8Ret != AT_ERR_NO_ERROR)
		return AT_ERR_INVALID_OPERATION;

	AT_SEND_OK(data->au8Cmd);

	return AT_ERR_NO_ERROR;
}

sint8 atCmd_PIperf_Stop_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = M2M_SUCCESS;
	SOCKET sock;
	
	PRINT("atCmd_PIperf_Stop\r\n");

	if(data->u8NumOfParameters == 0)
	{
		sock = -1;
	}
	else if(data->u8NumOfParameters == 2)
	{
		sock = (SOCKET) atoi((char *)data->au8ParamsList[1]);
	}
	else
	{
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	s8Ret = IperfStop(sock);
	
	if(s8Ret != M2M_SUCCESS)
	{
		s8Ret = AT_ERR_INVALID_OPERATION;
		goto EXIT;
	}
			
	AT_SEND_OK(data->au8Cmd);
	
	EXIT:
	return s8Ret;
}


/************************************************************************/
/* Default Connect Handler                                              */
/************************************************************************/
sint8 atCmd_DefaultConnect_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;
	
	PRINT("atCmd_DefaultConnect_Handler\r\n");
	
	if(gaAt_CMD_handler_fn[AT_INDEX_DEF_CONN].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	if(SERVICE_IS_RUNNING != status->u8Sta_status) {
		if(M2M_SUCCESS != (s8Ret=m2m_wifi_default_connect())) {
			s8Ret = AT_ERR_OPERATION_NOT_COMPLETED;
		}
	} else {
		s8Ret = AT_ERR_INVALID_OPERATION;
	}
	
EXIT:
	return s8Ret;
}

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Handlers in Access Point Mode                                        */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Access Point related Enums, Structs, and variables                    */
/************************************************************************/
enum{
	AP_INDEX_SSID = 0,
	AP_INDEX_SSID_VISIBLE_MODE,
	AP_INDEX_SEC_TYPE,
	AP_INDEX_SEC_KEY,
	AP_INDEX_CHANNEL,
	AP_INDEX_SERVER_IP,
	PROV_INDEX_SERVER_DOMAIN,
	PROV_INDEX_REDIRECT_STATE,
};
tstrM2MAPModeConfig gstrApConfig;
typedef struct{
	char	au8ServerDomain[252];
	uint8	bEnRedirect;
}tstrProv_Data;
tstrProv_Data	gtstrProv_Data;
/************************************************************************/
/* AP Disable Handler                                                   */
/************************************************************************/
sint8 atCmd_ApDisable_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;
	PRINT("atCmd_ApDisable_Handler\r\n");
	
	if(gaAt_CMD_handler_fn[AT_INDEX_AP_DISABLE].u8NoOfParameters != data->u8NumOfParameters) {
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	if(SERVICE_IS_STOPPED != status->u8Ap_status){
		if(M2M_SUCCESS == (s8Ret=m2m_wifi_disable_ap())) {
			if(status->u8Ap_status != SERVICE_DEV_IS_CONNECTED) {
				status->u8Ap_status = SERVICE_IS_STOPPED;
				AT_SEND_OK(data->au8Cmd);
			} else {
				status->u8Ap_status = SERVICE_IS_UNDEFINED;
			}
		} else {
			s8Ret = AT_ERR_OPERATION_NOT_COMPLETED;
		}
	} else {
		s8Ret = AT_ERR_INVALID_OPERATION;
	}
EXIT:
	return s8Ret;
}
/************************************************************************/
/* AP Enable Handler                                                    */
/************************************************************************/
sint8 atCmd_ApEnable_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	uint8 *pu8Dat = NULL;
	tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;
	uint8 u8LoopCntr = 0;
	
	PRINT("atCmd_ApEnable_Handler\r\n");
	
	if(1 == useStoredValue)
	{
		goto ENABLE_AP;
	}
	
	if(gaAt_CMD_handler_fn[AT_INDEX_AP_ENABLE].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	if(SERVICE_IS_RUNNING == status->u8Ap_status) { /*Check if AP mode is running*/
		s8Ret = AT_ERR_INVALID_OPERATION;
		goto EXIT;
	}
	
	if(SERVICE_IS_RUNNING == status->u8Sta_status)	/*Check if STA mode is running*/
	{
		s8Ret = AT_ERR_INVALID_OPERATION;
		goto EXIT;
	}	
	
	/*Get SSID*/
	memcpy(gstrApConfig.strApConfig.au8SSID,data->au8ParamsList[AP_INDEX_SSID], strnlen((const char *)data->au8ParamsList[AP_INDEX_SSID], M2M_MAX_SSID_LEN));
	gstrApConfig.strApConfig.au8SSID[strnlen((const char *)data->au8ParamsList[AP_INDEX_SSID], M2M_MAX_SSID_LEN-1)] = 0;
	
	/*Get SSID BroadCast type: Hidden or Visible*/
	gstrApConfig.strApConfig.u8SsidHide = atoi((const char *)data->au8ParamsList[AP_INDEX_SSID_VISIBLE_MODE]);
	/*Get Listen Channel*/
	gstrApConfig.strApConfig.u8ListenChannel = atoi((const char *)data->au8ParamsList[AP_INDEX_CHANNEL]);
	/*Get Security Type: OPEN or WEP*/
	gstrApConfig.strApConfig.u8SecType = atoi((const char *)data->au8ParamsList[AP_INDEX_SEC_TYPE]);

	if(CONN_WIFI_SEC_WPA2_PSK == gstrApConfig.strApConfig.u8SecType)
	{
		gstrApConfig.strApConfig.u8SecType = CONN_WIFI_SEC_WPA_PSK;
	}
	
	/*If security type is OPEN, so bypass the incoming key and enable AP mode directly*/
	if(CONN_WIFI_SEC_WEP == gstrApConfig.strApConfig.u8SecType) {
		/*Get WEP Key Index*/
		pu8Dat = (uint8 *)strtok((char *)data->au8ParamsList[AP_INDEX_SEC_KEY],"*");
		gstrApConfig.strApConfig.u8KeyIndx=atoi((const char *)pu8Dat);
		/*Get WEP Key Size*/
		pu8Dat = (uint8 *)strtok(NULL,"*");
		gstrApConfig.strApConfig.u8KeySz=strlen((const char *)pu8Dat);
		/*Get WEP Key Password*/
		memcpy(gstrApConfig.strApConfig.au8WepKey,pu8Dat,gstrApConfig.strApConfig.u8KeySz);
	}
	else if(CONN_WIFI_SEC_WPA_PSK == gstrApConfig.strApConfig.u8SecType)
	{
		uint8	*pu8WPAKey	= data->au8ParamsList[AP_INDEX_SEC_KEY];
		uint8	u8WPAKeySz	= strlen(pu8WPAKey);
		
		gstrApConfig.strApConfig.u8KeySz = u8WPAKeySz;
		memcpy(gstrApConfig.strApConfig.au8Key, pu8WPAKey, M2M_MAX_PSK_LEN);
	}
	
	pu8Dat = (uint8 *)strtok((char *)data->au8ParamsList[AP_INDEX_SERVER_IP],".");
	for(u8LoopCntr=0; u8LoopCntr<sizeof(gstrApConfig.strApConfig.au8DHCPServerIP); u8LoopCntr++) {
		uint8 octet = atoi((const char *)pu8Dat);
		gstrApConfig.strApConfig.au8DHCPServerIP[u8LoopCntr] = octet;
		gstrApConfig.strApConfigExt.au8DefRouterIP[u8LoopCntr] = octet;
		gstrApConfig.strApConfigExt.au8DNSServerIP[u8LoopCntr] = octet;
		pu8Dat = (uint8 *)strtok(NULL,".");
	}
	
	PRINT("\r>%s\r\n>%d\r\n>%d\r\n>%d\r\n>%s\r\n",
		gstrApConfig.au8SSID,
		gstrApConfig.u8SsidHide,
		gstrApConfig.u8ListenChannel,
		gstrApConfig.u8SecType,
		gstrApConfig.au8WepKey
	);
	if(CONN_WIFI_SEC_WEP == gstrApConfig.strApConfig.u8SecType)
	{
		PRINT("\r\n"
		"WEP Key Index: %d\r\n"
		"WEP Key      : %s\r\n"
		"WEP Key Sz   : %d\r\n",gstrApConfig.u8KeyIndx,gstrApConfig.au8WepKey,gstrApConfig.u8KeySz);
	}
	else if(CONN_WIFI_SEC_WPA_PSK == gstrApConfig.strApConfig.u8SecType)
	{
		PRINT("\r\n"
		"WPA Key      : %s\r\n"
		"WEP Key Sz   : %d\r\n",gstrApConfig.au8Key,gstrApConfig.u8KeySz);
	}
ENABLE_AP:
	//gstrApConfig.u8ListenChannel--;
	if(M2M_SUCCESS == (s8Ret=m2m_wifi_enable_ap_ext(&gstrApConfig)))
	{
		/*Because, there is no CB for Enabled AP Mode, the following 2 statements are executed here*/
		status->u8Ap_status = SERVICE_IS_RUNNING; 
		AT_SEND_OK(data->au8Cmd);				
	}
	else
	{
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
	}
EXIT:
	return s8Ret;
}
/************************************************************************/
/* Provisioning Mode Disable Handler                                    */
/************************************************************************/
sint8 atCmd_ProvisionDisable_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;
	PRINT("atCmd_ProvisionDisable_Handler\r\n");
	
	if(gaAt_CMD_handler_fn[AT_INDEX_PROV_DISABLE].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	if(SERVICE_IS_STOPPED != status->u8Prov_status){
		if(M2M_SUCCESS == (s8Ret=m2m_wifi_stop_provision_mode())) {
			if(status->u8Prov_status != SERVICE_DEV_IS_CONNECTED) {
				status->u8Prov_status = SERVICE_IS_STOPPED;
				AT_SEND_OK(data->au8Cmd);
			} else {
				status->u8Prov_status = SERVICE_IS_UNDEFINED;
			}
		} else {
			s8Ret = AT_ERR_OPERATION_NOT_COMPLETED;
		}
	} else {
		s8Ret = AT_ERR_INVALID_OPERATION;
	}
	
EXIT:
	return s8Ret;
}
/************************************************************************/
/* Provisioning Mode Enable Handler                                     */
/************************************************************************/
sint8 atCmd_ProvisionEnable_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	uint8 *pu8Dat = NULL;
	tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;
	uint8 u8LoopCntr, u8DomainLength;
	
	PRINT("atCmd_ProvisionEnable_Handler\r\n");
	
	M2M_DBG("gaAt_CMD_handler_fn[AT_INDEX_PROV_ENABLE][1]=%d\r\ndata.numOfParameters=%d",
		gaAt_CMD_handler_fn[AT_INDEX_PROV_ENABLE].u8NoOfParameters ,data->u8NumOfParameters);
	
	if(gaAt_CMD_handler_fn[AT_INDEX_PROV_ENABLE].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	/*Get SSID*/
	strncpy((char*)gstrApConfig.strApConfig.au8SSID, (char*)data->au8ParamsList[AP_INDEX_SSID], M2M_MAX_SSID_LEN);
	gstrApConfig.strApConfig.au8SSID[M2M_MAX_SSID_LEN - 1] = '\0';

	/*Get SSID BroadCast type: Hidden or Visible*/
	gstrApConfig.strApConfig.u8SsidHide = atoi((const char *)data->au8ParamsList[AP_INDEX_SSID_VISIBLE_MODE]);
	/*Get Listen Channel*/
	gstrApConfig.strApConfig.u8ListenChannel = atoi((const char *)data->au8ParamsList[AP_INDEX_CHANNEL]);
	/*Get Security Type: OPEN or WEP*/
	gstrApConfig.strApConfig.u8SecType = atoi((const char *)data->au8ParamsList[AP_INDEX_SEC_TYPE]);

	if(CONN_WIFI_SEC_WPA2_PSK == gstrApConfig.strApConfig.u8SecType)
	{
		gstrApConfig.strApConfig.u8SecType = CONN_WIFI_SEC_WPA_PSK;
	}

	/*If security type is OPEN, so bypass the incoming key and enable AP mode directly*/
	if(CONN_WIFI_SEC_WEP == gstrApConfig.strApConfig.u8SecType) {
		/*Get WEP Key Index*/
		pu8Dat = (uint8 *)strtok((char *)data->au8ParamsList[AP_INDEX_SEC_KEY],"*");
		gstrApConfig.strApConfig.u8KeyIndx=atoi((const char *)pu8Dat)-1;
		/*Get WEP Key Size*/
		pu8Dat = (uint8 *)strtok(NULL,"*");
		gstrApConfig.strApConfig.u8KeySz=strlen((const char *)pu8Dat);
		/*Get WEP Key Password*/
		memcpy(gstrApConfig.strApConfig.au8WepKey,pu8Dat,gstrApConfig.strApConfig.u8KeySz);
	}
	else if(CONN_WIFI_SEC_WPA_PSK == gstrApConfig.strApConfig.u8SecType){
		gstrApConfig.strApConfig.u8KeySz=strlen((const char *)data->au8ParamsList[AP_INDEX_SEC_KEY]);
		memcpy(gstrApConfig.strApConfig.au8Key,data->au8ParamsList[AP_INDEX_SEC_KEY],M2M_MAX_PSK_LEN);
	}
	
	pu8Dat = (uint8 *)strtok((char *)data->au8ParamsList[AP_INDEX_SERVER_IP],".");
	for(u8LoopCntr=0; u8LoopCntr<sizeof(gstrApConfig.strApConfig.au8DHCPServerIP); u8LoopCntr++) {
		uint8 octet = strtoul((const char *)pu8Dat, NULL, 10);
		gstrApConfig.strApConfig.au8DHCPServerIP[u8LoopCntr] = octet;
		gstrApConfig.strApConfigExt.au8DefRouterIP[u8LoopCntr] = octet;
		gstrApConfig.strApConfigExt.au8DNSServerIP[u8LoopCntr] = octet;
		pu8Dat = (uint8 *)strtok(NULL,".");
	}
	
	u8DomainLength = (sizeof(gtstrProv_Data.au8ServerDomain) <= sizeof(data->au8ParamsList[PROV_INDEX_SERVER_DOMAIN])?
					sizeof(gtstrProv_Data.au8ServerDomain) : sizeof(data->au8ParamsList[PROV_INDEX_SERVER_DOMAIN]));				
	memcpy(gtstrProv_Data.au8ServerDomain,data->au8ParamsList[PROV_INDEX_SERVER_DOMAIN],u8DomainLength);
	/* Get Redirect State*/
	gtstrProv_Data.bEnRedirect = atoi((const char *)data->au8ParamsList[PROV_INDEX_REDIRECT_STATE]);
	
	if(SERVICE_IS_RUNNING != status->u8Prov_status) {
		s8Ret = m2m_wifi_start_provision_mode_ext(&gstrApConfig, gtstrProv_Data.au8ServerDomain, gtstrProv_Data.bEnRedirect);
		if(M2M_SUCCESS == s8Ret) {
			status->u8Prov_status = SERVICE_IS_RUNNING;
			AT_SEND_OK(data->au8Cmd);
		} else {
			
			M2M_DBG("Provisioning parameters:\r\n");
			M2M_DBG("Domain name: %s\r\n",gtstrProv_Data.au8ServerDomain);
			M2M_DBG("Redirect enable: %d\r\n",gtstrProv_Data.bEnRedirect);
			M2M_DBG("DHCPIP: [%d.%d.%d.%d]\r\n",gstrApConfig.au8DHCPServerIP[0],gstrApConfig.au8DHCPServerIP[1],
				gstrApConfig.au8DHCPServerIP[2],gstrApConfig.au8DHCPServerIP[3]);
			M2M_DBG("Default router: [%d.%d.%d.%d]\r\n",gstrApConfig.au8DefRouterIP[0],gstrApConfig.au8DefRouterIP[1],
				gstrApConfig.au8DefRouterIP[2],gstrApConfig.au8DefRouterIP[3]);
			M2M_DBG("DNS Server: [%d.%d.%d.%d]\r\n",gstrApConfig.au8DNSServerIP[0],gstrApConfig.au8DNSServerIP[1],
				gstrApConfig.au8DNSServerIP[2],gstrApConfig.au8DNSServerIP[3]);
			M2M_DBG("SSID: [%s] hidden: [%d]\r\nCh: [%d]\r\n", gstrApConfig.au8SSID,gstrApConfig.u8SsidHide,
				gstrApConfig.u8ListenChannel);
			M2M_DBG("Sec:%d Key:[%s] Sz: [%d] Ind:[%d]\r\n",gstrApConfig.u8SecType,gstrApConfig.au8WepKey,
				gstrApConfig.u8KeySz,gstrApConfig.u8KeyIndx);
			
			AT_SEND_ERROR(s8Ret, data->au8Cmd);
		}
	} else {
		s8Ret = AT_ERR_INVALID_OPERATION;
	}
	
EXIT:
	return s8Ret;
}
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Misc Handlers                                                        */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
uint32 gu32ScanStartTime = 0;
uint32 gu32DownloadFileTime = 0;
extern uint32 get_systicks(void);
void SetScanTimeStart(void)
{
	gu32ScanStartTime = get_systicks();
}
uint32 GetScanTimeStart(void)
{
	return gu32ScanStartTime;
}
void SetDownloadStartTime(void)
{
	gu32DownloadFileTime =  get_systicks();
}
uint32 GetDownloadStartTime(void)
{
	return gu32DownloadFileTime;
}
/************************************************************************/
/* Scan Handler                                                         */
/************************************************************************/
sint8 atCmd_Scan_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	uint16 channel = atoi((const char *)data->au8ParamsList[0]);
	PRINT("atCmd_Scan_Handler\r\n");
	if(gaAt_CMD_handler_fn[AT_INDEX_SCAN].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	PRINT("Scanning on channel %d\r\n",channel);
	
	SetScanTimeStart();
	s8Ret=m2m_wifi_request_scan(channel);

EXIT:
	return s8Ret;
}
/************************************************************************/
/* Scan Region Handler                                                  */
/************************************************************************/
sint8 atCmd_ScanRegion_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
    sint8  s8Ret = AT_ERR_NO_ERROR;
    uint16 region = 0;

    region = atoi((char *)data->au8ParamsList[0]);

    s8Ret=m2m_wifi_set_scan_region(region);
EXIT:
    return s8Ret;
}
sint8 atCmd_Scan_Options_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
    sint8 s8Ret = AT_ERR_NO_ERROR;

    tstrM2MScanOption strM2MScanOption = {0};

    PRINT("atCmd_Scan_Options_Handler\r\n");

	if(data->u8NumOfParameters != 4){
		if(data->u8NumOfParameters != 5){
			s8Ret = AT_ERR_NUM_OF_PARAMS;
			goto EXIT;
		}else{
			s8Ret = m2m_wifi_set_stop_scan_on_first(atoi(data->au8ParamsList[4]));
		}
	}

	if(s8Ret == AT_ERR_NO_ERROR)
	{
		strM2MScanOption.u8NumOfSlot        = atoi(data->au8ParamsList[0]);
		strM2MScanOption.u8SlotTime         = atoi(data->au8ParamsList[1]);
		strM2MScanOption.u8ProbesPerSlot    = atoi(data->au8ParamsList[2]);
		strM2MScanOption.s8RssiThresh       = atoi(data->au8ParamsList[3]);

		s8Ret = m2m_wifi_set_scan_options(&strM2MScanOption);

		if(s8Ret == AT_ERR_NO_ERROR)
			AT_SEND_OK(data->au8Cmd);
	}

EXIT:
    return s8Ret;
}
sint8 atCmd_ScanSsidList_Handler(tstrAt_cmd_content *data,void* moreData,uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	uint8 u8Ch = M2M_WIFI_CH_6;
	uint8 u8NumSsid = 0;
	uint8* au8Ssid[4];
	uint8 au8SsidList[1+4*(1+32)] = {0};
	uint8 ind=0;
	int i=0;

	u8Ch = atoi((const char*) data->au8ParamsList[SCAN_LIST_CH]);
	u8NumSsid = atoi((const char*) data->au8ParamsList[SCAN_LIST_NUM_SSID]);
	au8Ssid[0] =  data->au8ParamsList[SCAN_LIST_SSID0];
	au8Ssid[1] =  data->au8ParamsList[SCAN_LIST_SSID1];
	au8Ssid[2] =  data->au8ParamsList[SCAN_LIST_SSID2];
	au8Ssid[3] =  data->au8ParamsList[SCAN_LIST_SSID3];

	au8SsidList[ind++] = u8NumSsid;
	for(i=0;i<u8NumSsid;i++)
	{
		uint8 SsidLen = strlen((const char*)au8Ssid[i]);
		au8SsidList[ind++] = SsidLen;
		memcpy(&au8SsidList[ind],au8Ssid[i],SsidLen);
		ind += SsidLen;
	}
	SetScanTimeStart();
	s8Ret = m2m_wifi_request_scan_ssid_list(u8Ch,au8SsidList);
	return s8Ret;
}
/************************************************************************/
/* RSSI Handler                                                         */
/************************************************************************/
sint8 atCmd_CurRSSI_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	PRINT("atCmd_CurRSSI_Handler\r\n");
	
	if(gaAt_CMD_handler_fn[AT_INDEX_RSSI].u8NoOfParameters != data->u8NumOfParameters) {
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	if(M2M_SUCCESS != (s8Ret=m2m_wifi_req_curr_rssi())) {
		s8Ret = AT_ERR_OPERATION_NOT_COMPLETED;
	}

EXIT:
	return s8Ret;
}

#ifdef LED_BTN_DEMO
/************************************************************************/
/* De-Initialization TCP Server Handler                                 */
/************************************************************************/
sint8 atCmd_WifiDirectDeinit_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;

	PRINT("atCmd_WifiDirectDeinit_Handler\r\n");
	if(gaAt_CMD_handler_fn[AT_INDEX_WDTEST_DEINIT].u8NoOfParameters != data->u8NumOfParameters) {
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	if(useStoredValue) {
		goto RUN;
	}
RUN:
	LedBtnDemo_AppStop();
	AT_SEND_OK(data->au8Cmd);
EXIT:
	return s8Ret;
}

/************************************************************************/
/* Initialization TCP Server Handler                                    */
/************************************************************************/
sint8 atCmd_WifiDirectInit_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	tpfLedBtnDemoCb tcp_LedBtn_CallBack = (tpfLedBtnDemoCb)moreData;
	
	PRINT("atCmd_WifiDirectInit_Handler\r\n");
	if(gaAt_CMD_handler_fn[AT_INDEX_WDTEST_INIT].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	if(useStoredValue) {
		goto RUN;
	}
RUN:
	LedBtnDemo_AppStart(tcp_LedBtn_CallBack);
	AT_SEND_OK(data->au8Cmd);
EXIT:
	return s8Ret;
}
#endif
/************************************************************************/
/* WPS Connect Handler                                                  */
/************************************************************************/
sint8 atCmd_WpsConnect_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue) {
	sint8 s8Ret = AT_ERR_NO_ERROR;

	PRINT("atCmd_WpsConnect_Handler\r\n");
	if(2 < data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	printf(">>Trigger Type %d",atoi((const char *)data->au8ParamsList[0]));
	printf(">>Pin Key      %s",data->au8ParamsList[1]);
	if(M2M_SUCCESS != (s8Ret=m2m_wifi_wps(atoi((const char *)data->au8ParamsList[0]),
		(const char *)(WPS_PIN_TRIGGER == atoi((const char *)data->au8ParamsList[0])?data->au8ParamsList[1]:NULL)))) {

		s8Ret = AT_ERR_INVALID_ARGUMENTS;		
	}
EXIT:
	return s8Ret;
}
/************************************************************************/
/* WPS Disable Handler                                                  */
/************************************************************************/
sint8 atCmd_WpsDisable_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue) {
	sint8 s8Ret = AT_ERR_NO_ERROR;
	
	PRINT("atCmd_WpsDisable_Handler\r\n");
	if(gaAt_CMD_handler_fn[AT_INDEX_WPS_DISABLE].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	if(M2M_SUCCESS != (s8Ret=m2m_wifi_wps_disable())) {
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
		AT_SEND_ERROR(AT_ERR_INVALID_ARGUMENTS, data->au8Cmd);
	} else {
		AT_SEND_OK(data->au8Cmd);
	}	
EXIT:
	return s8Ret;
}


void PrintSockConnections(SockConnInfo *conn)
{
    SockConnInfo* tmp;
    uint8 i=1;
    tmp=conn;

    if (conn==NULL){
	    M2M_INFO("The list is empty");
    }
    else{
	    while(tmp->next_SockConn!=NULL){
		    M2M_INFO("CID: %d, sock:%d, port:%u, ip: %lx", tmp->u8CID, tmp->sock, tmp->u16port, tmp->addr);
		    i++;
		    tmp=tmp->next_SockConn;
	    }
		    M2M_INFO("CID: %d, sock:%d, port:%u, ip: %lx", tmp->u8CID, tmp->sock, tmp->u16port, tmp->addr);
    }	
	
}

uint8 GetCID(void){
	uint8 itmp,cid= -1;
	
	for (itmp = 0; itmp<MAX_SOCKET; itmp++)
	{
		if (sCID[itmp]!= 0xFF)
		{
			cid=sCID[itmp];
			sCID[itmp] = 0xFF;
			return cid;
		}
	}
	return 0xFF;
}

void FreeCID(uint8 cid){
	if (sCID[cid] == 0xFF)
	{
		sCID[cid] = cid;
	}
}

uint8 UpdateSockConn(SockConnInfo *conn, SOCKET csock, uint16 port, uint32 saddr) {
	conn->u8CID = GetCID();
	conn->sock = csock;
	conn->addr = saddr;
	conn->u16port = port;
	conn->next_SockConn = NULL;
	return conn->u8CID;
}

uint8 AddSockConn(SockConnInfo **conn, SOCKET csock, uint16 port, uint32 saddr) {
	SockConnInfo* tmp;

	if ((*conn)==NULL)
	{
		(*conn)=(SockConnInfo *)malloc(sizeof(SockConnInfo));
		return (UpdateSockConn((*conn), csock, port, saddr));
	}
	else
	{
		tmp=(*conn);
		while(tmp->next_SockConn != NULL)
		{
			tmp = tmp->next_SockConn;
		}
		tmp->next_SockConn=(SockConnInfo *)malloc(sizeof(SockConnInfo));
		return(UpdateSockConn(tmp->next_SockConn, csock, port, saddr));
	}	
}

void DeleteSockConn(SockConnInfo **conn, SOCKET csock){
	SockConnInfo *aux = *conn;
	SockConnInfo *pre_conn = NULL;

	if (csock == gHTTPsock)
	{
		gHTTPsock = -1;
		memset(gHTTPport,0,AT_MAX_PARAM_LENGTH);
		gTLS = 0;
	}

	while((aux != NULL) && (aux->sock != csock))
	{
		pre_conn = aux;
		aux = aux->next_SockConn;
	}
	if(aux != NULL)
	{
		SockConnInfo *tmp = aux->next_SockConn;
		FreeCID(aux->u8CID);
		free(aux);
		if(pre_conn != NULL)
			pre_conn->next_SockConn = tmp;
		else
			*conn = tmp;
	}
}

SockConnInfo* RetrieveConnectionFromSocket(SockConnInfo **conn, uint8 sock){
	SockConnInfo* aux;

	if ((*conn)!=NULL)
	{
		if((*conn)->sock == sock)
			return (*conn);
		else {
			aux=(*conn)->next_SockConn;
			while(aux != NULL)
			{
				if(aux->sock != sock)
					aux = aux->next_SockConn;
				else
					return (aux);
			}

		}
	}
	return 0;
}

SockConnInfo* RetrieveConnectionFromCID(SockConnInfo **conn, uint8 cid){
	SockConnInfo* aux;

	if ((*conn)!=NULL)
	{
		if((*conn)->u8CID == cid)
			return (*conn);
		else {
			aux=(*conn)->next_SockConn;
			while(aux != NULL)
			{
				if(aux->u8CID != cid)
					aux = aux->next_SockConn;
				else
					return (aux);
			}

		}
	}
	return NULL;
}

sint8 atCmd_HTTPCon_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8	s8Ret = AT_ERR_NO_ERROR;
	uint8	Host_name[AT_MAX_HOST_NAME]="";
	
	PRINT("atCmd_HTTPCon_Handler\r\n");
	
	registerSocketCallback(m2m_wifi_socket_handler, m2m_wifi_resolve_handler);

	if(data->u8NumOfParameters != gaAt_CMD_handler_fn[AT_INDEX_HTTPCON].u8NoOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	memset(Host_name,0,AT_MAX_HOST_NAME);
	memset(gHTTPport,0,AT_MAX_PARAM_LENGTH);
	memcpy(Host_name,data->au8ParamsList[0],strnlen((const char *)data->au8ParamsList[0], AT_MAX_HOST_NAME) );
	memcpy(gHTTPport,data->au8ParamsList[1],strlen((const char *)data->au8ParamsList[1]));
	gTLS = atoi((const char *)data->au8ParamsList[2]);
	gHTTPChecksum = atoi((const char *)data->au8ParamsList[3]);

	gethostbyname(Host_name);

EXIT:
	return s8Ret;
}

sint8 atCmd_IsParameterNumericOnly(uint8* pData)
{
	uint8 *pu8tmp, u8itmp = 0;

	pu8tmp = pData;

	while(*(pu8tmp+u8itmp)!=NULL)
	{
		if(*(pu8tmp+u8itmp)<0x30 || *(pu8tmp+u8itmp)>0x39)
		{
			return M2M_ERR_INVALID_ARG;
		}
		u8itmp++;
	}
	return M2M_SUCCESS;
}

sint8 atCmd_IPCon_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue) 
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	SOCKET sock = -1;
	struct sockaddr_in myaddr;
	uint8 u8LoopCntr, sockCID=0xFF, *pu8Dat = NULL;
	uint16 portnum = 0;
	int ServerIP[4];

	PRINT("atCmd_IPCon_Handler\r\n");
		
	if((data->u8NumOfParameters > gaAt_CMD_handler_fn[AT_INDEX_IPCON].u8NoOfParameters) || (data->u8NumOfParameters < 2)){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	if (data->u8NumOfParameters == 2) {
		// Command is of the type: AT+IPCON=2[UDP|TCP,LOCAL_PORT]
		// Port number can't contain letters
		if(atCmd_IsParameterNumericOnly(&data->au8ParamsList[1])!= M2M_SUCCESS)
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			goto EXIT;
		}
		portnum = atoi((char *)data->au8ParamsList[1]);

	} else {
		// Command is of the type: AP+IPCON=3[UDP|TCP,@IP,REMOTE_PORT] or AP+IPCON=5[TCP,@IP,REMOTE_PORT,TLS,CHECKSUM]
		pu8Dat = (uint8 *)strtok((char *)data->au8ParamsList[1],".");

		for(u8LoopCntr=0; u8LoopCntr<4; u8LoopCntr++) {
			// IP address can't contain letters
			if(atCmd_IsParameterNumericOnly(pu8Dat)!=M2M_SUCCESS)
			{
				s8Ret = AT_ERR_INVALID_ARGUMENTS;
				goto EXIT;
			}
			if(((ServerIP[u8LoopCntr] = atoi((const char *)pu8Dat)) == 0) && (*pu8Dat != 0x30))
			{
				s8Ret = AT_ERR_INVALID_ARGUMENTS;
				goto EXIT;
			}
			if(ServerIP[u8LoopCntr]>255)
			{
				s8Ret = AT_ERR_INVALID_ARGUMENTS;
				goto EXIT;
			}
			pu8Dat = (uint8 *)strtok(NULL,".");
		}
		// Port number can't contain letters		
		if(atCmd_IsParameterNumericOnly(&data->au8ParamsList[2])!=M2M_SUCCESS)
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			goto EXIT;
		}
		portnum = atoi((char *)data->au8ParamsList[2]);
	}

	if(portnum == 0)
	{
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT;
	}
	
	registerSocketCallback(m2m_wifi_socket_handler, m2m_wifi_resolve_handler);
	memset(&myaddr,0,sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = 0;
	myaddr.sin_port = _htons(portnum);

	if(strcmp((const char *)data->au8ParamsList[0],"UDP") == 0)
	{			
		// Command is of the type: AT+IPCON=2[UDP,LOCAL_PORT] or AP+IPCON=3[UDP,@IP,REMOTE_PORT]
		if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			M2M_INFO("ERROR: Failed to create UDP socket!");
			goto EXIT;
		}
	}
	else if(strcmp((const char *)data->au8ParamsList[0],"TCP") == 0 && (data->u8NumOfParameters == 2 ||data->u8NumOfParameters == 3))
	{
		// Command is of the type: AT+IPCON=2[TCP,LOCAL_PORT] or AP+IPCON=3[TCP,@IP,REMOTE_PORT]
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			M2M_INFO("ERROR: Failed to create TCP socket!");
			goto EXIT;
		}
	}
	else if(strcmp((const char *)data->au8ParamsList[0],"TCP") == 0 && data->u8NumOfParameters == 5)
	{
			// Command is of the type: AP+IPCON=5[TCP,@IP,REMOTE_PORT,TLS,CHECKSUM]
			int sslOptionEnable = 1;
			gTLS = atoi((char *)data->au8ParamsList[3]);
			gHTTPChecksum = atoi((char *)data->au8ParamsList[4]);
			if ((gTLS > 2) || (gHTTPChecksum > 1))
			{
				s8Ret = AT_ERR_INVALID_ARGUMENTS;
				goto EXIT;
			}
					
		    if ((sock = socket(AF_INET, SOCK_STREAM, gTLS)) < 0) {
			    M2M_INFO("ERROR: Failed to create TCP socket!");
			    goto EXIT;
		    }
			
			if(gTLS)
			{
				// Set socket options if AT_SSL_OPTIONS command has been used to enable SNI, SessionCaching or BypassMode.
				set_alpn_list(sock, (const char *)gAlpnList);
				setsockopt(sock, SOL_SSL_SOCKET, SO_SSL_SNI, gHostNameSNI, strlen(gHostNameSNI) + 1);
				if(gstrSslOptions.bIsCertNameCheckEnabled)
				{
					setsockopt(sock, SOL_SSL_SOCKET, SO_SSL_ENABLE_SNI_VALIDATION, &sslOptionEnable, 4);
				}
			
				if(gstrSslOptions.bIsSessionCachingEnabled)
				{
					setsockopt(sock, SOL_SSL_SOCKET, SO_SSL_ENABLE_SESSION_CACHING, &sslOptionEnable, 4);
				}
			
				if(gstrSslOptions.bIsBypassModeEnabled)
				{
					setsockopt(sock, SOL_SSL_SOCKET, SO_SSL_BYPASS_X509_VERIF, &sslOptionEnable, 4);
				}
			}
	
	} else { 
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT; 
	}

	if (data->u8NumOfParameters == 2)
	{
		// Listen Socket
		if (bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
		{
			M2M_INFO("ERROR: Failed to bind socket");
			close(sock);
			goto EXIT;
		}
	} else {
		// Connection Socket
		myaddr.sin_addr.s_addr = _htonl((uint32_t) (ServerIP[0]<<24 | ServerIP[1]<<16 | ServerIP[2]<<8 | ServerIP[3]));
		
		if(strcmp((const char *)data->au8ParamsList[0],"TCP") == 0)
		{
			if(gTLS == 1)
			{
				gHTTPsock = sock;
			}
			if(connect(sock,(struct sockaddr *)&myaddr, sizeof(myaddr))<0) {
				close(sock);
				M2M_INFO("ERROR: Failed to connect to the TCP server!");
				goto EXIT;
			}
		}
	}

	sockCID = AddSockConn(&first_SockConn, sock, portnum, myaddr.sin_addr.s_addr);
			
	if(sockCID == 0xFF)
	{
		M2M_INFO("ERROR: Failed to add CID");
	}
	else if (strcmp((const char *)data->au8ParamsList[0],"UDP") == 0) {
		if (data->u8NumOfParameters == 3) {
			PrintSockConnections(first_SockConn); 
		}
	}
EXIT:
return s8Ret;	
}

static uint8 MapToControlCharacter(uint8 *to_convert, volatile uint8 c_from, volatile uint8 c_to, uint8 total_len)
{
	uint8 converted[AT_MAX_PARAM_LENGTH];
	uint8 data_index, first, data_len, char_delete=0;
	uint8 *pu8Dat = NULL;
	uint8 len_toconvert = strlen((char *)(to_convert));
	
	data_len = total_len;
	memset(converted,0,sizeof(converted));
	
	if (len_toconvert>1)
	{
		first = 1;
		data_index = 0;
		//pu8Dat points to the first character after the delimiter or the beginning of the string (first time)
		// e.g. "XXX\rYYYY\rZZZ"
		pu8Dat = (uint8 *)strtok((char *)to_convert,"\\");
		
		if ((len_toconvert !=strlen((char *)(pu8Dat))))
		{
			while (pu8Dat != NULL)
			{
				/* 
					If the string to be converted contains "XX\r\" and, when first output from strtok being "XX" you have 
					substituted \r by CR (data_index=1), the next output from strtok will be just "r". This one needs to be 
					skipped	as it has already been accounted for. 
				*/
				if(!((strlen((char *)(pu8Dat))==1) && (*(pu8Dat) == c_from) && data_index))
				{
					/* From the example shown in the comment above, if pu8Dat points to XXX" and the next
					   character after the end of the string is the character you are looking for eg. r, it means you have
					   found a \r and the string that pu8Dat points to should be copied to converted.

					   However, if pu8Dat points to "rYYYY" and r is the character that you are looking for, r needs to be
					   skipped when copying to converted (char *)(pu8Dat+1).
					*/
					if((first && (*(pu8Dat+strlen((char *)(pu8Dat))+1) == c_from)) || (!first && *(pu8Dat) == c_from))
					{
						if(first)
						{
							first = 0;
							strncat((char *)converted, (char *)(pu8Dat), Min(AT_MAX_PARAM_LENGTH - strlen((char *)converted), strlen((char *)(pu8Dat))) );
						}
						else
							strncat((char *)converted, (char *)(pu8Dat+1), Min(AT_MAX_PARAM_LENGTH - strlen((char *)converted), strlen((char *)(pu8Dat+1))) );

						data_index = 1;
						
						/* 
							If we haven't finished converting the \character, add the ControlCharacter and update
							the number of characters that we have deleted so far (one for each \character) and update
							the total size that converted will held at the end of the routine
						*/
						 
						if((strlen((char *)(converted))+char_delete)<total_len)
						{
							converted[strlen((char *)(converted))] = c_to;
							data_len -=1;
							char_delete++;
						}
					}
					else
					{
						/* 
							If the character after the backlash is not the one we are looking for, put back the '\'
							to converted 
						*/
						converted[strlen((char *)(converted))] = 0x5c;
						strcat((char *)converted,(char *)(pu8Dat));
						data_index=0;
					}
				}
				// Look for the next output of strtok 
				pu8Dat = (uint8 *)strtok(NULL,"\\");
			}
		}
		else
			memcpy(converted,to_convert,len_toconvert);
	}
	else
		memcpy(converted,to_convert,len_toconvert);
	
	// overwrite to_convert with converted
	memset(to_convert,0,len_toconvert);
	memcpy(to_convert,converted,AT_MAX_PARAM_LENGTH);
	
	return data_len;
}

sint8 atCmd_SocketDataStream_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue) {
	sint8 s8Ret = AT_ERR_NO_ERROR;
	uint8 len_tosend = 0, data_offset, data_len, u8LoopCntr;
	uint8 *pu8Dat = NULL;
	int	DestIP[4];
	uint8 data_tosend[AT_MAX_PARAM_LENGTH];
	uint16 portnum = 0;
	struct sockaddr_in destaddr;
	SockConnInfo *pDestSockConn = NULL;
	
	PRINT("atCmd_SocketDataStream_Handler\r\n");
	
	if(data->u8NumOfParameters !=3  && data->u8NumOfParameters != 5 ){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	if (data->u8NumOfParameters == 3) {
		// Command is of the type: <ESC>SDS=3[CID,LEN,DATA]
		data_offset = 1;
	} else {
		// Command is of the type: <ESC>SDS=5[CID,IP_ADDRESS,PORT,LEN,DATA]
		data_offset = 3;
		pu8Dat = (uint8 *)strtok((char *)data->au8ParamsList[1],".");
		// Parse IP address of the destination
		for(u8LoopCntr=0; u8LoopCntr<4; u8LoopCntr++) {
			if(((DestIP[u8LoopCntr] = atoi((const char *)pu8Dat)) == 0) && (*pu8Dat != 0x30))
			{
				s8Ret = AT_ERR_INVALID_ARGUMENTS;
				goto EXIT;
			}
			if(DestIP[u8LoopCntr]>255)
			{
				s8Ret = AT_ERR_INVALID_ARGUMENTS;
				goto EXIT;
			}
			pu8Dat = (uint8 *)strtok(NULL,".");
		}
		
		portnum = atoi((char *)data->au8ParamsList[2]);
		
		if(portnum == 0)
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			goto EXIT;		
		}
	}
	
	data_len = atoi((char *)data->au8ParamsList[data_offset]);
	
	memset(data_tosend,0,sizeof(data_tosend));	
	
	for(u8LoopCntr=0; u8LoopCntr<AT_MAX_PARAM_LENGTH; u8LoopCntr++) {
		data_tosend[u8LoopCntr] = data->au8ParamsList[data_offset+1][u8LoopCntr];
		if(data_tosend[u8LoopCntr] == 0)
			break;
		len_tosend++;
	}

	// Check: Length provided by the user has to be the same as the length of the string to be sent
	if (len_tosend != data_len)
	{
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT;
	}	

	// Map /r to carriage return
	len_tosend = MapToControlCharacter(data_tosend,(uint8)'r',(uint8)'\r',len_tosend);
	// Map /n to new line
	len_tosend = MapToControlCharacter(data_tosend,(uint8)'n',(uint8)'\n',len_tosend);		

	pDestSockConn = RetrieveConnectionFromCID(&first_SockConn, atoi((char *)data->au8ParamsList[0]));
	if(pDestSockConn==NULL)
	{
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT;
	}
	
	if(pDestSockConn->sock < TCP_SOCK_MAX)
	{
		// The socket is TCP
		if (gHTTPChecksum)
		{
			memset(&gHTTPInfo,0,sizeof(HTTP_RXInfo));
			SHA1_INIT(&gHTTPInfo.sha1_ctxt);
			gHTTPInfo.first_chunk = 1;
		}
		if(send(pDestSockConn->sock, data_tosend, len_tosend, 0) == M2M_SUCCESS)
			M2M_INFO("Message sent");
		else
			M2M_INFO("ERROR: Failed to send to socket %d", pDestSockConn->sock);
	}
	else
	{
		// The socket is UDP
		memset(&destaddr, 0, sizeof(destaddr));
		destaddr.sin_family = AF_INET;
		
		if (data->u8NumOfParameters == 3) {
			// Command is of the type: <ESC>SDS=3[CID,LEN,DATA]
			destaddr.sin_addr.s_addr = pDestSockConn->addr;
			destaddr.sin_port = _htons( pDestSockConn->u16port);
		} else {
			//Command is of the type: <ESC>SDS=5[CID,IP_ADDRESS,PORT,LEN,DATA]
			destaddr.sin_addr.s_addr = _htonl((uint32_t) (DestIP[0]<<24 | DestIP[1]<<16 | DestIP[2]<<8 | DestIP[3]));
			destaddr.sin_port = _htons(portnum);
		}

		if(sendto(pDestSockConn->sock, data_tosend, data_len, 0, (struct sockaddr *)&destaddr, sizeof(destaddr)) == M2M_SUCCESS)
		{
			M2M_INFO("Message sent");
			if (recvfrom(pDestSockConn->sock, gau8SocketTestBuffer, MAIN_WIFI_M2M_BUFFER_SIZE, 0) != M2M_SUCCESS)
			{
				M2M_INFO("ERROR: Failed to recv from socket %d", pDestSockConn->sock);
			}
		} else
			M2M_INFO("ERROR: Failed to send to socket %d", pDestSockConn->sock);
	}

EXIT:
return s8Ret;
}

sint8 atCmd_DeleteCID_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue) {
	sint8 s8Ret = AT_ERR_NO_ERROR;
	uint8 cid;
	SockConnInfo *pDestSockConn = NULL;
	
	PRINT("atCmd_DeleteCID_Handler\r\n");
	
	if(data->u8NumOfParameters != gaAt_CMD_handler_fn[AT_INDEX_DELCID].u8NoOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	cid = atoi((char *)data->au8ParamsList[0]);
	
	pDestSockConn = RetrieveConnectionFromCID(&first_SockConn, cid);
	
	if(pDestSockConn==NULL)
	{
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT;
	}	
	
	close(pDestSockConn->sock);
	DeleteSockConn(&first_SockConn,pDestSockConn->sock);
	
	printf("Available CIDs:\n");	
	PrintSockConnections(first_SockConn);	

EXIT:
return s8Ret;	
}

sint8 atCmd_TLS_SetCipherSuite_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8  s8Ret = AT_ERR_NO_ERROR;
	uint32 CSBitmap = 0;

	if(data->u8NumOfParameters != gaAt_CMD_handler_fn[AT_INDEX_TLS_SET_CS].u8NoOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	CSBitmap = atoi((char *)data->au8ParamsList[0]);

	// Active the ECC cipher suites in the WINC
	s8Ret = m2m_ssl_set_active_ciphersuites(CSBitmap);

	if (s8Ret != M2M_SUCCESS)
	{
		M2M_ERR("m2m_ssl_set_active_ciphersuites() failed with ret=%d", s8Ret);
		goto EXIT;
	}

	EXIT:
	return s8Ret;
}

sint8 atCmd_TLS_CertTransfer_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8  s8Ret = AT_ERR_NO_ERROR;

	if(data->u8NumOfParameters != gaAt_CMD_handler_fn[AT_INDEX_TLS_WCERT].u8NoOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	s8Ret = eccTransferCertificates();

	if (s8Ret != M2M_SUCCESS)
	{
		M2M_ERR("eccTransferCertificates() failed with ret=%d", s8Ret);
		goto EXIT;
	}

	EXIT:
	return s8Ret;
}

sint8 atCmd_OTA_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue) {
	sint8 s8Ret = AT_ERR_NO_ERROR;

	PRINT("atCmd_OTA_Handler\r\n");
	
	if(data->u8NumOfParameters != gaAt_CMD_handler_fn[AT_INDEX_OTA].u8NoOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	if(strcmp((const char *)data->au8ParamsList[0],"ROLLBACK") == 0){
		if (m2m_ota_rollback() != M2M_SUCCESS){
			M2M_INFO("ERROR: Failed to rollback to the previous image");
		}
	} else if(strcmp((const char *)data->au8ParamsList[0],"ABORT") == 0){
		if (m2m_ota_abort() != M2M_SUCCESS){
			M2M_INFO("ERROR: Failed to find any ota in progress");
		}
	} else {
		if (m2m_ota_start_update(data->au8ParamsList[0]) != M2M_SUCCESS){
			M2M_INFO("ERROR: Failed to start OTA update");
		} 
	}

	
	EXIT:
	return s8Ret;
}

sint8 atCmd_GetVer_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue) {
	sint8 s8Ret = AT_ERR_NO_ERROR;
	tstrM2mRev strM2mRev;
	PRINT("atCmd_GetVer_Handler\r\n");
	
	if(data->u8NumOfParameters != gaAt_CMD_handler_fn[AT_INDEX_GETVER].u8NoOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	M2M_INFO("Curr driver ver: %u.%u.%u\n", M2M_RELEASE_VERSION_MAJOR_NO, M2M_RELEASE_VERSION_MINOR_NO, M2M_RELEASE_VERSION_PATCH_NO);
	s8Ret = m2m_wifi_get_firmware_version(&strM2mRev);
	if(M2M_SUCCESS == s8Ret)
	{
		/* char acCmdRet[]="GET_VER"; */
		M2M_INFO("Firmware ver   : %u.%u.%u Svnrev %u\n", strM2mRev.u8FirmwareMajor, strM2mRev.u8FirmwareMinor, strM2mRev.u8FirmwarePatch,strM2mRev.u16FirmwareSvnNum);
		M2M_INFO("Min driver ver : %u.%u.%u\n", strM2mRev.u8DriverMajor, strM2mRev.u8DriverMinor, strM2mRev.u8DriverPatch);
		M2M_INFO("Curr driver ver: %u.%u.%u Svnrev %u \n", M2M_RELEASE_VERSION_MAJOR_NO, M2M_RELEASE_VERSION_MINOR_NO, M2M_RELEASE_VERSION_PATCH_NO,M2M_RELEASE_VERSION_SVN_VERSION);
		M2M_INFO("Firmware Build %s Time %s\n",strM2mRev.BuildDate,strM2mRev.BuildTime);
	}
	else
	{	
		AT_SEND_ERROR(s8Ret, "GETVER");
	}
	
	s8Ret = m2m_ota_get_firmware_version(&strM2mRev);
	if((s8Ret == M2M_SUCCESS)||(s8Ret == M2M_ERR_FW_VER_MISMATCH))
	{
		M2M_INFO("OTA Firmware ver   : %u.%u.%u Svnrev %u\n", strM2mRev.u8FirmwareMajor, strM2mRev.u8FirmwareMinor, strM2mRev.u8FirmwarePatch,strM2mRev.u16FirmwareSvnNum);
		M2M_INFO("OTA Min driver ver : %u.%u.%u\n", strM2mRev.u8DriverMajor, strM2mRev.u8DriverMinor, strM2mRev.u8DriverPatch);
		M2M_INFO("OTA Firmware Build %s Time %s\n",strM2mRev.BuildDate,strM2mRev.BuildTime);
	}
	
	if(s8Ret == M2M_ERR_INVALID)
	{
		M2M_INFO("OTA image NOT found\n");
		s8Ret = M2M_SUCCESS;
	}
		
	EXIT:
	return s8Ret;
}


sint8 atCmd_Reset_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue) {
	sint8 s8Ret = AT_ERR_NO_ERROR;

	PRINT("atCmd_Reset_Handler\r\n");
	
	if(data->u8NumOfParameters > gaAt_CMD_handler_fn[AT_INDEX_RESET].u8NoOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	if(
			(data->u8NumOfParameters == 0)
		||	(strcmp((const char *)data->au8ParamsList[0],"MCU") == 0)
	)		
	{
#if defined(__SAML21J18B__) || defined(__SAMD21J18A__)
		system_reset();
#elif defined(__SAM4SD32C__) || defined(__SAMG53N19__)
		NVIC_SystemReset();
#endif
	}
	else if(strcmp((const char *)data->au8ParamsList[0],"WINC") == 0)
	{
		s8Ret = winc_reset(1, 0, M2M_WIFI_MODE_NORMAL);
	}
#ifdef ETH_MODE
	else if (strcmp((const char *)data->au8ParamsList[0],"ETH") == 0)
	{
		s8Ret = winc_reset(1, 0, M2M_WIFI_MODE_ETHERNET);
	}
#endif
	if(M2M_SUCCESS == s8Ret)
		AT_SEND_OK("WINC Reset Done\r\n");
	else
		AT_SEND_ERROR(s8Ret, "WINC Reset Failed\r\n");
EXIT:
	return s8Ret;
}


/************************************************************************/
/* List Handler                                                         */
/************************************************************************/
sint8 atCmd_List_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue) {
	sint8 s8Ret = AT_ERR_NO_ERROR;
	uint8 u8LoopCntr = 0;
	
	printf("This is a list of all AT Commands supported even the configuration commands:\r\n");
	printf("%-2s\t%-10s\t%s\r\n","#","Command","#Parameters");
	
	printf("%02d\t%-10s\t%02d\r\n", (AT_INDEX_CONFIG+1), gaAt_CMD_handler_fn[AT_INDEX_CONFIG].au8AtCMD, gaAt_CMD_handler_fn[AT_INDEX_CONFIG].u8NoOfParameters);
	for(u8LoopCntr=0; u8LoopCntr<AT_MAX_CFG_CMD_COUNT; u8LoopCntr++) {	//Config. commands
		printf("\t%c)\t%-10s\r\n", (u8LoopCntr+'a'), gaAt_CFG_Values_arr[u8LoopCntr]);
	}
	printf("\r\n");
	for(u8LoopCntr=1; u8LoopCntr<AT_MAX_COMMANDS_COUNT; u8LoopCntr++) {	//main commands
		printf("%02d\t%-10s\t%02d\r\n", (u8LoopCntr+1), gaAt_CMD_handler_fn[u8LoopCntr].au8AtCMD, gaAt_CMD_handler_fn[u8LoopCntr].u8NoOfParameters);
	}
	printf("\r\n");
	return s8Ret;
}

/************************************************************************/
/* Help Handler                                                         */
/************************************************************************/
sint8 atCmd_Help_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue) {
	sint8 s8Ret = AT_ERR_NO_ERROR;
	
	PRINT("atCmd_Help_Handler\r\n");
	printf("%s  \r\n", "You can use AT Commands as the following forms:");
	printf("\t%s\r\n", "Without Parameters :AT+CMD<CR><LF>");
	printf("\t%s\r\n", "With Parameters    :AT+CMD=N[Param1,Param2,...,ParamN]<CR><LF>");
	
	printf("\r\n\tWhere <CR>\"Carriage Return\" -> (13)");
	printf("\r\n\t      <LF>\"Line Feed\"       -> (10)");
	printf("\r\n");
	
	printf("%s  \r\n", "Also, You can use \"AT+LIST\" to display all supported AT Commands");
	printf("%s  \r\n", "or type the AT Command without parameters followed by \"/?\" to view help about it.");
	printf("\t%s\r\n", "Examples : AT+CFG=2[ECHO,0]");
	printf("\t%s\r\n", "         : AT+LIST");
	printf("\t%s\r\n", "         : AT+CFG /?");
	printf("\t%s\r\n", "\r\n");
	return s8Ret;
}

/************************************************************************/
/* atCmd_Inquiries_Handler Handler                                      */
/************************************************************************/
sint8 atCmd_Inquiries_Handler(tstrAt_cmd_content *data, strAtCMD_Handler *pastrAtCMD_Handler)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	uint8 u8LoopCntr;
	uint8 u8CmdLength = strlen((const char *)data->au8Cmd);
	//tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;
	
	PRINT("atCmd_Inquiries_Handler\r\n");
	for(u8LoopCntr = 0; u8LoopCntr<AT_MAX_COMMANDS_COUNT; u8LoopCntr++) {
		if(!memcmp(data->au8Cmd, pastrAtCMD_Handler[u8LoopCntr].au8AtCMD, u8CmdLength)) {
			break;
		}
	}
	if(u8LoopCntr < AT_MAX_COMMANDS_COUNT) {
		printf("\r\n>>Description of [%s] command:\r\n", pastrAtCMD_Handler[u8LoopCntr].au8AtCMD);
	} else {
		printf("\r\n>>Invalid Command, You can use one of the following forms:\r\n");
	}
	switch(u8LoopCntr)
	{
		case AT_INDEX_CONFIG:
		{
			printf("%s  \r\n", "AT+CFG=2[CMD,VALUE]<CR><LF>");
			printf("\t%s\r\n", "This command used to set or reset some configuration for the host.");
			printf("\t%s\r\n", "where CMD   : is a command for configuration");
			printf("\t%s\r\n", "      VALUE : is the value for this command");
			printf("\t%s\r\n", "EX.: AT+CFG=2[ECHO,0], AT+CFG=2[ECHO,1] -> to Disable/Enable ECHO mode.");
			break;
		}
		case AT_INDEX_LIST:
		{
			printf("%s  \r\n", "AT+LIST<CR><LF>");
			printf("\t%s\r\n", "This command used to get a list of all AT Commands.");
			break;
		}
		case AT_INDEX_HELP:
		{
			printf("%s  \r\n", "AT+HELP<CR><LF>");
			printf("\t%s\r\n", "This command use to display how to write AT Command.");
			break;
		}
		case AT_INDEX_SCAN:
		{
			printf("%s  \r\n", "AT+SCAN=1[CHANNEL]<CR><LF>");
			printf("\t%s\r\n", "where CHANNEL : is channel number to scan on.");
			printf("\t%s\r\n", "                This value can be 1 -> 13.");
			printf("\t%s\r\n", "EX.: AT+SCAN=1[255] -> to scan over all channels at the same time.");
			break;
		}
		case AT_INDEX_SCAN_REGION:
		{
			printf("%s  \r\n\r\n", "AT+SCAN_REGION=1[MAP]<CR><LF>");
			printf("\t%s\r\n", "MAP : is a bitmap of channels to scan");
			printf("\t%s\r\n\r\n", "             This is a decimal value where bit 0 represents channel 1, etc.");
			printf("%s\r\n", "EXAMPLE: AT+SCAN_REGION=1[2047] -> Scan channels up to and including 11 (North American Region).");
			break;
		}
		case AT_INDEX_CONN:
		{
			printf("%s  \r\n", "AT+CONN=5[SSID,SSID_LENGTH,SEC_TYPE,SEC_KEY,CHANNEL]<CR><LF>");
			printf("\t%s\r\n", "where SSID        : is Access Point you want to connect with.");
			printf("\t%s\r\n", "      SSID_LENGTH : number of characters of SSID");
			printf("\t%s\r\n", "      SEC_TYPE    : Type of Security of AP and has one of the following:");
			printf("\t%s\r\n", "          1       : OPEN");
			printf("\t%s\r\n", "          2       : WPA/WPA2");
			printf("\t%s\r\n", "          3       : WEP");
			printf("\t%s\r\n", "          4       : Enterprise");
			printf("\t%s\r\n", "      SEC_KEY     : Security key and will be in case of:");
			printf("\t%s\r\n", "          OPEN      : at least any character");
			printf("\t%s\r\n", "          WPA/WPA2  : any valid sequence of characters");
			printf("\t%s\r\n", "          WEP       : KEY_INDEX*KEY_VALUE ,where they are concatenated with '*'");
			printf("\t%s\r\n", "          Enterprise: USRNAME*PASSWORD ,where they are concatenated with '*'");
			printf("\t%s\r\n", "      CHANNEL     : is channel number of the Access Point");
			
			printf("\t%s\r\n", "EX.: AT+CONN=5[DEMO_AP,7,1,NS,5]                  -> Connect to OPEN");
			printf("\t%s\r\n", "   : AT+CONN=5[DEMO_AP,7,2,1234567890,6]          -> Connect to WPA/WPA2");
			printf("\t%s\r\n", "   : AT+CONN=5[DEMO_AP,7,3,1*1234567890,7]        -> Connect to WEP");
			printf("\t%s\r\n", "   : AT+CONN=5[DEMO_AP,7,4,EntUser*EntPassword,8] -> Connect to Enterprise");
			break;
		}
		case AT_INDEX_AP_DISABLE:
		{
			printf("%s  \r\n", "AT+AP_DIS<CR><LF>");
			printf("\t%s\r\n", "This command use to disable Opened Access Point Mode.");
			break;
		}
		case AT_INDEX_AP_ENABLE:
		{
			printf("%s  \r\n", "AT+AP_EN=5[SSID,SSID_VISIBLE_MODE,SEC_TYPE,SEC_KEY,CHANNEL]<CR><LF>");
			printf("\t%s\r\n", "where SSID             : is Access Point you want to connect with.");
			printf("\t%s\r\n", "      SSID_VISIBLE_MODE: which means the SSI is visible or Hidden");
			printf("\t%s\r\n", "          0            : Visible SSID");
			printf("\t%s\r\n", "          1            : Hidden SSID");
			printf("\t%s\r\n", "      SEC_TYPE         : Type of Security of AP and has one of the following:");
			printf("\t%s\r\n", "          1            : OPEN");
			printf("\t%s\r\n", "          3            : WEP");
			printf("\t%s\r\n", "      SEC_KEY          : Security key and will be in case of:");
			printf("\t%s\r\n", "          OPEN         : at least any character");
			printf("\t%s\r\n", "          WEP          : KEY_INDEX*KEY_VALUE ,where they are concatenated with '*'");
			printf("\t%s\r\n", "      CHANNEL          : is channel number of the Access Point");
			
			printf("\t%s\r\n", "EX.: AT+AP_EN=5[DEMO_AP,0,1,NS,5]            -> OPEN with Hidden SSID");
			printf("\t%s\r\n", "   : AT+AP_EN=5[DEMO_AP,1,3,1*1234567890,7]  -> WEP with visible SSID");
			break;
		}
		case AT_INDEX_DEF_CONN:
		{
			printf("%s  \r\n", "AT+DEF_CONN<CR><LF>");
			printf("\t%s\r\n", "This command use to connect to default and saved access point.");
			break;
		}
		case AT_INDEX_DISCONN:
		{
			printf("%s  \r\n", "AT+DISCONN<CR><LF>");
			printf("\t%s\r\n", "This command use to disconnect your station from the connected access point");
			break;
		}
		case AT_INDEX_RSSI:
		{
			printf("%s  \r\n", "AT+RSSI<CR><LF>");
			printf("\t%s\r\n", "This command use to get RSSI of connected Access Point.");
			break;
		}
		case AT_INDEX_PROV_ENABLE:
		{
			printf("%s  \r\n", "AT+PROV_START=8[SSID,SSID_VISIBLE_MODE,SEC_TYPE,SEC_KEY,CHANNEL,SERVER_IP,SERVER_DOMAIN,REDIRECT_STATE]<CR><LF>");
			printf("\t%s\r\n", "where SSID             : is Access Point you want to connect with.");
			printf("\t%s\r\n", "      SSID_VISIBLE_MODE: which means the SSI is visible or Hidden");
			printf("\t%s\r\n", "          0            : Visible SSID");
			printf("\t%s\r\n", "          1            : Hidden SSID");
			printf("\t%s\r\n", "      SEC_TYPE         : Type of Security of AP and has one of the following:");
			printf("\t%s\r\n", "          1            : OPEN");
			printf("\t%s\r\n", "          3            : WEP");
			printf("\t%s\r\n", "      SEC_KEY          : Security key and will be in case of:");
			printf("\t%s\r\n", "          OPEN         : at least any character");
			printf("\t%s\r\n", "          WEP          : KEY_INDEX*KEY_VALUE ,where they are concatenated with '*'");
			printf("\t%s\r\n", "      CHANNEL          : is channel number of the Access Point");
			printf("\t%s\r\n", "      SERVER_IP        : is an IP which the station can access through it");
			printf("\t%s\r\n", "                         where they are concatenated with '*'");
			printf("\t%s\r\n", "      SERVER_DOMAIN    : is a domain which the station can access through it");
			printf("\t%s\r\n", "      REDIRECT_STATE   : to allow redirection or not while typing any other domain or IP:");
			printf("\t%s\r\n", "          0            : Disable redirection");
			printf("\t%s\r\n", "          1            : Enable redirection");
			
			printf("\t%s\r\n", "EX.: AT+PROV_START=5[DEMO_AP,0,1,NS,5,192*168*1*1,test_domain.com,1]           -> OPEN with Hidden SSID");
			printf("\t%s\r\n", "   : AT+PROV_START=5[DEMO_AP,1,3,1*1234567890,7,192*168*0*1,test_domain.com,0] -> WEP with visible SSID");
			break;
		}
		case AT_INDEX_PROV_DISABLE:
		{
			printf("%s  \r\n", "AT+PROV_DIS<CR><LF>");
			printf("\t%s\r\n", "This command use to disable provisioning mode.");
			break;
		}
#ifdef LED_BTN_DEMO
		case AT_INDEX_WDTEST_INIT:
		{
			printf("%s  \r\n", "AT+WDTEST_INIT<CR><LF>");
			printf("\t%s\r\n", "This command use to enable WIFI direct test application.");
			break;
		}
		case AT_INDEX_WDTEST_DEINIT:
		{
			printf("%s  \r\n", "AT+WDTEST_DEINIT<CR><LF>");
			printf("\t%s\r\n", "This command use to terminate WIFI direct test application.");
			break;
		}
#endif
		case AT_INDEX_WPS_START:
		{
			printf("%s  \r\n", "AT+WPS_START=2[TRIGGER_TYPE,PIN_CODE]<CR><LF>");
			printf("\t%s\r\n", "where TRIGGER_TYPE : type of how to connect withing WPS:");
			printf("\t%s\r\n", "          0        : Using PIN Code");
			printf("\t%s\r\n", "          4        : Using Push Button");
			printf("\t%s\r\n", "      PIN_CODE     : The key in case of using TRIGGER_TYPE as PIN Code");
			printf("\t%s\r\n", "EX.: AT+WPS_START=1[4]");
			printf("\t%s\r\n", "   : AT+WPS_START=2[0,12345679]");
			break;
		}
		case AT_INDEX_WPS_DISABLE:
		{
			printf("%s  \r\n", "AT+WPS_DIS<CR><LF>");
			printf("\t%s\r\n", "This command use to disable WPS Mode.");
			break;
		}
		case AT_INDEX_IPCON:
		{
			printf("%s  \r\n", "There are three modes for this command:");
			printf("\t%s\r\n", "  1.- AT+IPCON=2[<PROTOCOL>,<LOCAL_PORT>]<CR><LF>");
			printf("\t%s\r\n", "      where PROTOCOL : TCP or UDP");			
			printf("\t%s\r\n", "      where PORT : Port of the listen socket to be created");
			printf("\t%s\r\n", "      Returns the created connection ID (CID)");			
			printf("\t%s\r\n", "      EX. AT+IPCON=2[UDP,6666]");
			printf("\t%s\r\n", "  2.- AT+IPCON=3[<PROTOCOL>,<REMOTE_IP>,<REMOTE_PORT>]<CR><LF>");
			printf("\t%s\r\n", "      where REMOTE_IP : IP of the remote machine to connect to");
			printf("\t%s\r\n", "      where REMOTE_PORT : Port of the connection socket to connect to");
			printf("\t%s\r\n", "      Returns the created connection ID (CID)");						
			printf("\t%s\r\n", "      EX. AT+IPCON=3[UDP,192.168.1.3,4444]");
			printf("\t%s\r\n", "  3.- AT+IPCON=5[TCP,<REMOTE_IP>,<REMOTE_PORT>,<TLS>,<CHECKSUM>]<CR><LF>");
			printf("\t%s\r\n", "      where REMOTE_IP : IP of the remote machine to connect to");
			printf("\t%s\r\n", "      where REMOTE_PORT : Port of the connection socket to connect to");
			printf("\t%s\r\n", "      where TLS : 1 for TLS, 0 for non TLS, 2 for non TLS with capability to become TLS later");
			printf("\t%s\r\n", "      where CHECKSUM : 1 for checksum verification on the received data, 0 otherwise");							
			printf("\t%s\r\n", "      Returns the created connection ID (CID)");
			printf("\t%s\r\n", "      EX. AT+IPCON=3[UDP,192.168.1.3,4444]");												
			break;
		}
		case AT_INDEX_SDS:
		{
			printf("%s  \r\n", "There are two modes for this command:");			
			printf("%s  \r\n", "  1.- <Esc>SDS=3[<CID>,<LEN>,<DATA>]");
			printf("\t%s\r\n", "      where CID : CID to send data to");
			printf("\t%s\r\n", "      where LEN: Length of the data to be send");
			printf("\t%s\r\n", "      where DATA: Data to be send");					
			printf("\t%s\r\n", "      EX. [1B]SDS=3[1,5,Hello]");							
			printf("%s  \r\n", "  2.- <Esc>SDS=5[<CID>,<REMOTE_IP>,<REMOTE_PORT>,<LEN>,<DATA>]");
			printf("\t%s\r\n", "      where CID : CID to send data to");
			printf("\t%s\r\n", "      where REMOTE_IP : IP of the remote machine to send data to");
			printf("\t%s\r\n", "      where REMOTE_PORT : Port of the remote machine to send data to");			
			printf("\t%s\r\n", "      where LEN: Length of the data to be send");
			printf("\t%s\r\n", "      where DATA: Data to be send");
			printf("\t%s\r\n", "      EX. [1B]SDS=5[0,192.168.1.3,4444,5,Hello]");													
			break;			
		}
		case AT_INDEX_DELCID:
		{
			printf("%s  \r\n", "AT+DELCID=1[<CID>]");
			printf("\t%s\r\n", "where CID : CID to delete");						
			break;			
		}
		case AT_INDEX_OTA:
		{
			printf("%s  \r\n", "AT+OTA=1[<URL>| \"ROLLBACK\"| \"ABORT\"]");
			printf("\t%s\r\n", "where URL : Full URL path to the OTA image");
			printf("\t%s\r\n", "where \"ROLLBACK\" : Forces a OTA Rollback to the old image");
			printf("\t%s\r\n", "where \"ABORT\" : Forces abortion of an OTA");
			break;
		}
		case AT_INDEX_GETVER:
		{
			printf("%s  \r\n", "AT+GETVER");
			break;
		}
		case AT_INDEX_RESET:
		{
			printf("%s  \r\n", "AT+RESET=1[\"MCU\" | \"WINC\" | \"ETH\"]");
			printf("\t%s\r\n", "where MCU  : Triggers a reset of the MCU, which in turn resets the WINC (normal mode)");
			printf("\t%s\r\n", "where WINC : Triggers a reset of the WINC only, (normal mode)");
			printf("\t%s\r\n", "where ETH  : Triggers a reset of the WINC only, (bypass mode)");
			printf("\t%s\r\n", "AT+RESET (no parameters) is equivalent to AT+RESET=1[MCU]");
			break;
		}
		case AT_INDEX_PIPERF_START:
		{
			printf("%s  \r\n", "There are three modes for this command:");			
			printf("%s  \r\n", "  1.- AT+PIPERF+START=2[<PROTOCOL>,<PORT>]");
			printf("\t%s\r\n", "      where PROTOCOL : TCP or UDP");
			printf("\t%s\r\n", "      where PORT: the port the server will listen to. Use 5001 for IPERF");		
			printf("\t%s\r\n", "      EX. AT+PIPERF+START=2[UDP,5001]");							
			printf("%s  \r\n", "  2.- AT+PIPERF+START=4[TCP,<PORT>,<IP>,<STREAM_LEN>]");
			printf("\t%s\r\n", "      where PORT: the port the client will connect to. Use 5001 for IPERF");
			printf("\t%s\r\n", "      where IP : IP of the remote server to connect to");
			printf("\t%s\r\n", "      where STREAM_LEN : The total amount of data to be sent. If the user wants to send data indefinitely, <STREAM_LEN> should be set to 0");			
			printf("\t%s\r\n", "      EX. AT+PIPERF+START=4[TCP,5001,10.146.146.209,14000]");
			printf("%s  \r\n", "  3.- AT+PIPERF+START=6[UDP,<PORT>,<IP>,<NUM_PACKETS>,<PACKET_LEN>,<RATE>]");
			printf("\t%s\r\n", "      where PORT: the port the client will connect to. Use 5001 for IPERF");
			printf("\t%s\r\n", "      where IP : IP of the remote server to connect to");
			printf("\t%s\r\n", "      where NUM_PACKETS : The total  number of packets to be send. If the user wants to send data indefinitely, <NUM_PACKETS> should be set to 0");
			printf("\t%s\r\n", "      where PACKET_LEN : Length of each the packet");	
			printf("\t%s\r\n", "      where RATE : rate at the chip is going to try to send packets at. If the user wants to send at the fastest rate, <RATE> should be set to 0");										
			printf("\t%s\r\n", "      EX. AT+PIPERF_START=6[UDP,5001,10.146.146.209,20,1400,10000000]");																
			break;
		}
		case AT_INDEX_PIPERF_CREATE:
		{
			printf("%s  \r\n", "There are three modes for this command:");
			printf("%s  \r\n", "  1.- AT+PIPERF_CREATE=2[<PROTOCOL>,<PORT>]");
			printf("\t%s\r\n", "PROTOCOL : TCP or UDP");
			printf("\t%s\r\n\r\n", "PORT: the port the server will listen to. Use 5001 for IPERF");
			printf("%s\r\n\r\n", "EXAMPLE: AT+PIPERF_CREATE=2[UDP,5001]");
			printf("%s  \r\n", "  2.- AT+PIPERF_CREATE=5[TCP,<PORT>,<IP>,<STREAM_LEN>,<TLS>]");
			printf("\t%s\r\n", "PORT: the port the client will connect to. Use 5001 for IPERF");
			printf("\t%s\r\n", "IP : IP of the remote server to connect to");
			printf("\t%s\r\n", "STREAM_LEN : The total amount of data to be sent. If the user wants to send data indefinitely, <STREAM_LEN> should be set to 0");
			printf("\t%s\r\n\r\n", "TLS : Socket is connected via a TLS server. <TLS> should be set to 0 if no TLS connection required.");
			printf("%s\r\n\r\n", "EXAMPLE: AT+PIPERF_CREATE=5[TCP,5001,10.146.146.209,14000,0]");
			printf("%s  \r\n", "  3.- AT+PIPERF_CREATE=6[UDP,<PORT>,<IP>,<NUM_PACKETS>,<PACKET_LEN>,<RATE>]");
			printf("\t%s\r\n", "PORT: the port the client will connect to. Use 5001 for IPERF");
			printf("\t%s\r\n", "IP : IP of the remote server to connect to");
			printf("\t%s\r\n", "NUM_PACKETS : The total  number of packets to be send. If the user wants to send data indefinitely, <NUM_PACKETS> should be set to 0");
			printf("\t%s\r\n", "PACKET_LEN : Length of each the packet");
			printf("\t%s\r\n\r\n", "RATE : rate at the chip is going to try to send packets at. If the user wants to send at the fastest rate, <RATE> should be set to 0");
			printf("%s\r\n", "EXAMPLE: AT+PIPERF_CREATE=6[UDP,5001,10.146.146.209,20,1400,10000000]");
			break;
		}
		case AT_INDEX_PIPERF_STOP:
		{
			printf("%s  \r\n", "AT+PIPERF_STOP=2[<IS_SERVER><SOCKET>]");
			printf("\t%s\r\n", "      where IS_SERVER: 1 if server, 0 if client");
			printf("\t%s\r\n", "      where SOCKET   : The socket you want to stop transmitting/receiving");			
			break;
		}
		case AT_INDEX_HTTPCON:
		{
			printf("%s  \r\n", "AT+HTTPCON=4[<SERVERNAME>,<PORT>,<TLS>,<CHECKSUM>]");
			printf("\t%s\r\n", "      where SERVERNAME: The name of the server you want to stablish a connection to");
			printf("\t%s\r\n", "      where PORT      : The port to be used for the connection");
			printf("\t%s\r\n", "      where TLS : 1 for TLS, 2 for non TLS");
			printf("\t%s\r\n", "      where CHECKSUM : 1 for checksum verification on the received data, 0 otherwise");			
			break;
		}	
		case AT_TLS_CRL_RESET:
		{
			printf("%s  \r\n", "AT+TLS_CRL_RESET=1[<CRL_TYPE>]");
			printf("\t%s\r\n", "      where CRL_TYPE: 0 for none, 1 for CERT_HASH");	
			break;
		}
		case AT_TLS_CRL_ADD:
		{
			printf("%s  \r\n", "AT+TLS_CRL_ADD=1[<CERT_LEN>,<CERT_HASH>]");
			printf("\t%s\r\n", "      where CERT_LEN: Number of bytes for the hashed certificate");
			printf("\t%s\r\n", "      where CERT_HASH: Certificate hash in hexadecimal colon separated byte by byte");
			printf("\t%s\r\n", "      eg. e4:ae:c7:5e:43:4a:05:e7:08:5d:b9:1f:40:f6:91:1c:a2:91:eb:b4:2f:a2:8a:3e:e2:67:09:f8:e0:21:44:6b");						
			break;
		}	
		case AT_TLS_CRL_SEND:
		{
			printf("%s  \r\n", "AT+TLS_CRL_SEND");
			break;
		}
		case AT_INDEX_TLS_SET_CS:
		{
			M2M_PRINT("%s  \r\n", "AT+TLS_SET_CS=1[<CIPHER-SUITE>]");
			M2M_PRINT("\t%s\r\n", "where CIPHER-SUITE : is a 32-bit value indicating which cipher-suites should be enabled");
			break;
		}
		case AT_INDEX_TLS_WCERT:
		{
			M2M_PRINT("%s  \r\n", "AT+TLS_WCERT");
			break;
		}
		case AT_SSL_OPT:
		{
			M2M_PRINT("%s  \r\n\r\n", "AT+SSL_OPTIONS=4[<BYPASS_MODE>,<SESSION_CACHING>,<SERVER_NAME_CHECK>,<SNI_STRING>");
			M2M_PRINT("\t%s\r\n", "BYPASS_MODE: Set or Unset Bypass Mode");
			M2M_PRINT("\t%s\r\n", "\t0 : DISABLED");
			M2M_PRINT("\t%s\r\n", "\t1 : ENABLED");
			M2M_PRINT("\t%s\r\n", "SESSION_CACHING: Set or Unset Session Caching.");
			M2M_PRINT("\t%s\r\n", "\t0 : DISABLED");
			M2M_PRINT("\t%s\r\n", "\t1 : ENABLED");
			M2M_PRINT("\t%s\r\n", "SERVER_NAME_CHECK: Set or unset checking of server certificate name against SNI_STRING.");
			M2M_PRINT("\t%s\r\n", "\t0 : DISABLED");
			M2M_PRINT("\t%s\r\n", "\t1 : ENABLED");
			M2M_PRINT("\t%s\r\n", "SNI_STRING: Server name to be sent in the Client Hello SNI extension.");
			M2M_PRINT("\t\t%s\r\n\r\n", "Special case: If SNI_STRING begins with '0' then no SNI extension is sent.");
			M2M_PRINT("%s\r\n", "EXAMPLE: AT+SSL_OPTIONS=4[0,1,0,0]                                     -> Enable Session Caching");
			M2M_PRINT("%s\r\n", "\t     : AT+SSL_OPTIONS=4[0,0,0,hostname.com]            -> Server name sent in Client Hello SNI Extension");
			M2M_PRINT("%s\r\n", "\t     : AT+SSL_OPTIONS=4[0,0,1,hostname.com]            -> Enable checking of server name" );
			break;
		}
		case AT_INDEX_SET_TX_RATE:
		{
			M2M_PRINT("%s  \r\n\r\n", "AT+SET_TX_RATE=1[<TX_RATE]<CR><LF>");
			M2M_PRINT("\t%s\r\n", "TX_RATE: Set TX rate to the rate in which packets will be sent at.");
			M2M_PRINT("\t%s", "\t255  : Automatic Rate Selection");
			M2M_PRINT("\t%s\r\n", "\t254  : Force Lowest Possible Rate For Longest Range");
			M2M_PRINT("\t%s", "\t     0 : 1 Mbps");
			M2M_PRINT("\t%s", "\t     1 : 2 Mbps");
			M2M_PRINT("\t%s", "\t     2 : 5 Mbps");
			M2M_PRINT("\t%s", "\t     3 : 11 Mbps");
			M2M_PRINT("\t%s", "\t   11 : 6 Mbps");
			M2M_PRINT("\t%s", "\t   15 : 9 Mbps");
			M2M_PRINT("\t%s", "\t   10 : 12 Mbps");
			M2M_PRINT("\t%s", "\t   14 : 18 Mbps");
			M2M_PRINT("\t%s", "\t     9 :  24Mbps");
			M2M_PRINT("\t%s", "\t   13 : 36 Mbps");
			M2M_PRINT("\t%s", "\t     8 : 48 Mbps");
			M2M_PRINT("\t%s\r\n", "\t   12 : 54 Mbps");
			M2M_PRINT("\t%s", "\t 128 : MCS 0 :  6.5 Mbps");
			M2M_PRINT("\t%s", "\t 129 : MCS 1 :  13 Mbps");
			M2M_PRINT("\t%s", "\t 130 : MCS 2 : 19.5 Mbps");
			M2M_PRINT("\t%s", "\t 131 : MCS 3 :  26 Mbps");
			M2M_PRINT("\t%s", "\t 132 : MCS 4 :  39 Mbps");
			M2M_PRINT("\t%s", "\t 133 : MCS 5 :  52 Mbps");
			M2M_PRINT("\t%s", "\t 134 : MCS 6 : 58.2 Mbps");
			M2M_PRINT("\t%s\r\n", "\t 135 : MCS 7 :  65 Mbps");
			M2M_PRINT("%s\r\n", "EXAMPLE: AT+SET_TX_RATE=1[255]          -> Set Automatic Rate Selection");
			M2M_PRINT("%s\r\n", "\t     : AT+SET_TX_RATE=1[0]               -> Set TX Rate to 1 Mbps");
			M2M_PRINT("%s\r\n", "\t     : AT+SET_TX_RATE=1[128]          -> Set TX Rate to MCS0 : 6.5 Mbps" );
			break;
		}
        case AT_INDEX_HFD_GET:
        {
            M2M_PRINT("%s  \r\n\r\n", "AT+HFD_GET=1[<link to file>]<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "The <link to file> should be the link to the file stored in a remote server.<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "This will work for HTTP and HTTPS protocols only.<CR><LF>");
            break;
        }
        case AT_INDEX_HFD_READ:
        {
            M2M_PRINT("%s  \r\n", "There are two modes for this command:");
            M2M_PRINT("%s  \r\n\r\n", "1. Read using HIF.<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "AT+HFD_READ=4[HIF,<file handler ID>,<size>,<offset>]<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "2. Read using SPI.<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "AT+HFD_READ=4[SPI,<file handler ID>,<size>,<offset>]<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "The <file handler> will be an unsigned 8-bit integer returned from HFD_GET.<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "The <size> is the size (in bytes) of the file or section to be read.<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "The <offset> is an offset counting from the start of the file, which marks the start of the reading.<CR><LF>");
            break;
        }
        case AT_INDEX_HFD_ERASE:
        {
            M2M_PRINT("%s  \r\n\r\n", "AT+HFD_ERASE=1[<file handler>]<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "The <file handler> will be an unsigned 8-bit integer returned from HFD_GET.<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "Erase will delete the file corresponding to the given file handler, if the handler provided is valid.<CR><LF>");
            break;
        }
		case AT_INDEX_ROAMING:
		{
            M2M_PRINT("%s  \r\n", "There are two modes for this command:");
            M2M_PRINT("%s  \r\n\r\n", "1. Enabling Roaming<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "AT+ROAMING=2[<TRIGGER TYPE>,<DHCP ENABLE]<CR><LF>");
			M2M_PRINT("%s  \r\n\r\n", "The <trigger type> is 1 is to enable roaming.<CR><LF>");
			M2M_PRINT("%s  \r\n\r\n", "The <dhcp enable> is 1 or 0 and enables/disables dhcp client after roaming.<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "2. Disabling Roaming<CR><LF>");
            M2M_PRINT("%s  \r\n\r\n", "AT+ROAMING=1[<TRIGGER TYPE>]<CR><LF>");
			M2M_PRINT("%s  \r\n\r\n", "The <trigger type> is 0 is to disable roaming.<CR><LF>");
			break;
		}
        case AT_INDEX_NTP:
        {
	        M2M_PRINT("%s  \r\n\r\n", "AT+NTP=2[<NTP Server>,<Use DHCP NTP server>]<CR><LF>");
	        M2M_PRINT("%s  \r\n\r\n", "<NTP Server> is the NTP server to use, either a domain name or IP. If the first character is a * then it will be treated as a pool where the * is replaced with an incrementing value 0 to 3. '*.pool.ntp.org' will become '0-3.pool.ntp.org'.<CR><LF>");
	        M2M_PRINT("%s  \r\n\r\n", "If <Use DHCP NTP server> is 1 then use NTP server supplied by DHCP before trying any other servers.<CR><LF>");
	        break;
        }
        case AT_INDEX_1X_TLS_CRED:
        {
	        M2M_PRINT("%s\r\n", "AT+1X_TLS_CRED=2[<key URL>,<certificate URL>]<CR><LF>");
	        M2M_PRINT("%s\r\n", "<key URL> URL for downloading the enterprise private key file.");
	        M2M_PRINT("%s\r\n", "<certificate URL> URL for downloading the enterprise certificate file.");
	        break;
        }
        case AT_INDEX_1X_CONN:
        {
	        M2M_PRINT("%s\r\n", "AT+1X_CONN=5[<SSID>,<SSID_LENGTH>,<USERNAME@DOMAIN>,<PASSWORD>,<CHANNEL>]<CR><LF>");
			M2M_PRINT("\t%s\r\n", "where SSID            : Access Point you want to connect with.");
			M2M_PRINT("\t%s\r\n", "      SSID_LENGTH     : Number of characters of SSID.");
			M2M_PRINT("\t%s\r\n", "      USERNAME@DOMAIN : Username and domain formatted as Username@Domain or Domain\\Username. Domain is optional.");
			M2M_PRINT("\t%s\r\n", "      PASSWORD        : MSCHAPv2 password, or EAP-TLS (for EAP-TLS), or P-EAP-TLS (for tunneled EAP-TLS).");
			M2M_PRINT("\t%s\r\n", "      CHANNEL         : Channel number of the Access Point, or 255 (for any channel).");
			
			M2M_PRINT("\t%s\r\n", "EX.: AT+1X_CONN=5[DEMO_AP,7,BOB@CORP,P-EAP-TLS,255]");
			M2M_PRINT("\t%s\r\n", "   : AT+1X_CONN=5[DEMO_AP,7,CHARLIE,SUPERSECRET,6]");
	        break;
        }
		case AT_INDEX_1X_OPTIONS:
		{
			M2M_PRINT("%s\r\n", "AT+1X_OPTIONS=4[<BYPASS>, <ROOT_NAME_SHA1>, <TIME_OPTION>, <SESSION_CACHING>]");
			M2M_PRINT("\t%s\r\n", "where BYPASS          : 0 for doing server authentication");
			M2M_PRINT("\t%s\r\n", "                      : 1 for bypassing server authentication");
			M2M_PRINT("\t%s\r\n", "      ROOT_NAME_SHA1  : 0 for allowing server authentication via any root certificate in the WINC flash");
			M2M_PRINT("\t%s\r\n", "                      : 40-character hex string representing SHA1 digest of root certificate subject name");
			M2M_PRINT("\t%s\r\n", "      TIME_OPTION     : 0 for expiry-checking server certificate chain");
			M2M_PRINT("\t%s\r\n", "                      : 1 for expiry-checking if system time is known");
			M2M_PRINT("\t%s\r\n", "                      : 2 for bypassing expiry-checking");
			M2M_PRINT("\t%s\r\n", "      SESSION_CACHING : 0 for no TLS session caching");
			M2M_PRINT("\t%s\r\n", "                      : 1 for TLS session caching");
			break;
		}
		case AT_INDEX_TLS_START:
		{
			M2M_PRINT("%s\r\n", "AT+TLS_START=1[<SOCKET>]");
			M2M_PRINT("\t%s\r\n", "where SOCKET : The socket to be made secure");
			break;
		}
		case AT_INDEX_SCAN_OPTIONS:
		{
			M2M_PRINT("%s  \r\n\r\n", "AT+SCAN_OPTIONS=5[NUM_SLOTS,SLOT_TIME,PROBES_PER_SLOT,RSSI_THRESH,STOP_ON_FIRST]<CR><LF>");
			M2M_PRINT("\t%s\r\n", "NUM_SLOTS: number of slots for scanning. Each slot is based on a certain amount of time during which probes are sent and replies are expected.");
			M2M_PRINT("\t%s\r\n", "SLOT_TIME: time (in milliseconds) for the duration of each slot for scanning.");
			M2M_PRINT("\t%s\r\n", "PROBES_PER_SLOT: number of probes to be sent at the beginning of each slot for scanning.");
			M2M_PRINT("\t%s\r\n", "RSSI_THRESH: The Received Signal Strength Indicator threshold required for (fast) reconnection to an AP without scanning all channels first.");
			M2M_PRINT("\t%s\r\n", "STOP_ON_FIRST: Enables/Disables the option to stop the scan and return the results as soon as an SSID is found.");
			M2M_PRINT("\t\r\n\r\n");
			M2M_PRINT("%s\r\n", "EXAMPLE: AT+SCAN_OPTIONS=5[1,20,1,-45,0] -> Set the scan options to scan using 1 slot, each slot takes 20ms, send 1 probe per slot. Set the fast connect rssi threshold to -45 and don't return on first scan result.");
			M2M_PRINT("\t%s\r\n\r\n", "The 5th parameter is optional, meaning that SCAN_OPTIONS can be used with 4 parameters instead of 5, see below:");
			M2M_PRINT("%s\r\n", "EXAMPLE: AT+SCAN_OPTIONS=4[1,20,1,-45] -> Similarly to the above, set the scan options to scan using 1 slot, each slot takes 20ms, send 1 probe per slot. Set the fast connect rssi threshold to -45.");
			break;
		}
		default:
		{
			printf("\tWithout parameters             : AT+cmd<CR><LF>\r\n");
			printf("\tUsing parameters               : AT+cmd=N[param1,param2,....,paramN]<CR><LF>\r\n");
			printf("\tTo display help use            : AT+cmd/?<CR><LF>\r\n");
			printf("\tTo display List of Commands use: AT+LIST<CR><LF>\r\n");
			printf("\r\n");
			printf("\tTo display Help use            : HELP<CR><LF>\r\n");
			s8Ret = AT_ERR_NOT_REGISTERED_CMD;
			break;
		}
	}
	printf("\r\n\tWhere <CR>\"Carriage Return\" -> (13)");
	printf("\r\n\t      <LF>\"Line Feed\"       -> (10)");
	printf("\r\n");
//EXIT:
	return s8Ret;
}

static uint8 Ascii2Hex(uint8 u8AsciiChar)
{
	uint8	u8Hex = 0xFF;
	if((u8AsciiChar >= 'A') && (u8AsciiChar <= 'F'))
	{
		u8Hex = u8AsciiChar - 0x37;
	}
	else if((u8AsciiChar >= '0') && (u8AsciiChar <= '9'))
	{
		u8Hex = u8AsciiChar - 0x30;
	}
	else if((u8AsciiChar >= 'a') && (u8AsciiChar <= 'f'))
	{
		u8Hex = u8AsciiChar - 0x57;
	}
	return u8Hex;
}

sint32 StrMac2Bin(sint8* ps8MacStr,sint8* ps8MacBin)
{
	sint32 ret = M2M_SUCCESS;
	if(ps8MacStr == NULL || ps8MacBin == NULL)
	{
		printf("NULL pointer\n");
		return M2M_ERR_FAIL;
	}
	
	if(memcmp("any",ps8MacStr,4))
	{
		unsigned int i=0;
		unsigned int ind =0;
		while(i<strlen(ps8MacStr))
		{
			if(ps8MacStr[i] != ':')
			{
				ps8MacBin[ind] = Ascii2Hex(ps8MacStr[i])<<4;
				i++;
				ps8MacBin[ind] |= Ascii2Hex(ps8MacStr[i]);
				printf("%x-",ps8MacBin[ind]);
				ind++;
			}
			i++;
		}
	}
	else
	{
		memset(ps8MacBin,0,6);
	}



	return ret;
}
enum {AT_DATAACK_SUBTYPR,
	AT_DATAPOLL_SUBTYPR,
	AT_DATAPOLLACK_SUBTYPR,
	AT_NULLFRAME_SUBTYPR,
	AT_CFACK_SUBTYPR,
	AT_CFPOLL_SUBTYPR,
	AT_CPPOLLACK_SUBTYPR,
	AT_QOSDATA_SUBTYPR,
	AT_QOSDATAACK_SUBTYPR,
	AT_QOSDATAPOLL_SUBTYPR,
	AT_QOSDATAPOLLACK_SUBTYPR,
	AT_QOSNULLFRAME_SUBTYPR,
	AT_QOSCFPOLL_SUBTYPR,
	AT_QOSCFPOLLACK_SUBTYPR,
	AT_MAX_SUBTYPE=15,} AT_DATA_SUBTYPE;
enum {AT_CONTROL_PS_POLL_SUBTYPE,   
	AT_CONTROL_RTS_SUBTYPE,     
	AT_CONTROL_CTS_SUBTYPE,         
	AT_CONTROL_ACK_SUBTYPE,         
	AT_CONTROL_CFEND_SUBTYPE,       
	AT_CONTROL_CFEND_ACK_SUBTYPE,   
	AT_CONTROL_BLOCKACK_REQ_SUBTYPE,
	AT_CONTROL_BLOCKACK_SUBTYPE,
	AT_CONTROL_MAX_SUBTYPR=8} AT_CONTROL_SUBTYPR;
enum {AT_ASSOC_REQ_SUBTYPE,
	AT_ASSOC_RSP_SUBTYPE,
	AT_REASSOC_REQ_SUBTYPE,
	AT_REASSOC_RSP_SUBTYPE,
	AT_PROBE_REQ_SUBTYPE,
	AT_PROBE_RSP_SUBTYPE,
	AT_BEACON_SUBTYPE,
	AT_ATIM_SUBTYPE,
	AT_DISASOC_SUBTYPE,
	AT_AUTH_SUBTYPE,
	AT_DEAUTH_SUBTYPE,
	AT_ACTION_SUBTYPE,
	AT_MANAGEMENT_MAX_SUBTYPR=12};

uint8 u8StaConnect = 0;
uint8 u8IsFileDownloadStarted = 0;

uint8 GetFiledownloadStarted()
{
	return u8IsFileDownloadStarted;
}
sint8 atCmd_MonEn_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	return AT_ERR_INVALID_CMD;
}

sint8 atCmd_MonDis_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
    return AT_ERR_INVALID_CMD;
}

sint8 atCmd_PsMode_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 ret = AT_ERR_NO_ERROR;
	sint8 s8PsMode = 0;
	sint8 s8BcEn = 0;
	
	if(gaAt_CMD_handler_fn[AT_INDEX_PS_MODE].u8NoOfParameters != data->u8NumOfParameters)
	{
		ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	s8PsMode = atoi(data->au8ParamsList[0]);
	if(s8PsMode < M2M_NO_PS || s8PsMode > M2M_PS_MANUAL)
	{
		M2M_PRINT("Invalid PS type\n");
		ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT;
	}
	
	s8BcEn = atoi(data->au8ParamsList[1]);
	if(s8BcEn < 0 || s8BcEn > 1)
	{
		M2M_PRINT("Invalid BC value\n");
		ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT;
	}
	
	
	ret = m2m_wifi_set_sleep_mode(s8PsMode,s8BcEn);
	if(ret == AT_ERR_NO_ERROR)
	{
		sint8 s8Buf[20] = {0};
		sprintf(s8Buf,"PS_MODE:%d,%d",s8PsMode,s8BcEn);
		AT_SEND_OK(s8Buf);
	}
	else
	{
		AT_SEND_ERROR(ret,"PS_MODE");
	}
	

EXIT:
	return ret;
}
sint8 atCmd_GetConnInfo_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 ret = M2M_ERR_FAIL;
	ret = m2m_wifi_get_connection_info();
	return ret;
}
sint8 atCmd_SetPowerProfile_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = M2M_ERR_FAIL;
	uint8 u8PwrProfile = 0;
	u8PwrProfile = atoi((const char*)data->au8ParamsList[0]);
	s8Ret = m2m_wifi_set_power_profile(u8PwrProfile);
	return s8Ret;
}
extern void http_client_test_init(void);
extern void set_download_url(char*);
sint8 atCmd_FileDownload_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = M2M_ERR_FAIL;
	set_download_url(data->au8ParamsList[0]);
	u8IsFileDownloadStarted = 1;
	SetDownloadStartTime();
	http_client_test_init();
	s8Ret = 0;
	return s8Ret;
}
sint8 atCmd_ScanPassive_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = M2M_ERR_FAIL;
	uint8 u8ChNo = (uint8) atoi(data->au8ParamsList[0]);
	uint16 u16ScanTime = (uint16) atoi(data->au8ParamsList[1]);

	PRINT("atCmd_ScanPassive_Handler\r\n");

	SetScanTimeStart();
	s8Ret = m2m_wifi_request_scan_passive(u8ChNo, u16ScanTime);

	return s8Ret;
}

sint8 atCmd_SslEnableCertExpirationCheck(tstrAt_cmd_content *data, void* moreData, uint8 useStoreValue)
{
	sint8 s8Ret = M2M_ERR_FAIL;
	uint8 u8ExpChkFlag = (uint8) atoi(data->au8ParamsList[0]);
	
	if(u8ExpChkFlag > 1)
	{
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
		
	}
	else
	{
		s8Ret = sslEnableCertExpirationCheck(u8ExpChkFlag);
		if(AT_ERR_NO_ERROR == s8Ret)
		{
			AT_SEND_OK("EN_CERT_EXPCHK");
		}
		else
		{
			AT_SEND_ERROR(s8Ret,"EN_CERT_EXPCHK");
		}
	}
	
		
	return s8Ret;
}

sint8 atCmd_Ping(tstrAt_cmd_content *data, void* moreData, uint8 useStoreValue)
{
	sint8	s8Ret = M2M_ERR_INVALID_ARG;

	if(data->u8NumOfParameters != gaAt_CMD_handler_fn[AT_PING].u8NoOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	uint8 u8count = 0, u8count_dot = 0, u8AddrByte = 0;
	char *pcPingDest = data->au8ParamsList[0];

	/* Filter invalid/unsupported addresses */
	if((0 == strcmp(pcPingDest,"0.0.0.0")) || (0 == strcmp(pcPingDest,"255.255.255.255")) || (0 == strcmp(pcPingDest,"127.0.0.1")))
		goto EXIT;

	/* Validate the IP Address received */
	while (pcPingDest[u8count]!='\0')
	{
		if (pcPingDest[u8count]=='.')
		{
			u8count_dot++;
			u8AddrByte = 0;
		}
		else
		{
			u8AddrByte = u8AddrByte * 10 + (uint8)(pcPingDest[u8count] - '0');

			if((3 == u8count_dot) && (255 == u8AddrByte))
				goto EXIT;	
		}
		u8count++;
	}

	uint32 u32PingCount = (uint32) atoi(data->au8ParamsList[1]);
	if(gu32PingCount == 0)
	{
		registerSocketCallback(NULL, m2m_wifi_resolve_handler);
		M2M_INFO("Pinging %s\n",pcPingDest);
		gu32PingCount = u32PingCount;
		if((*pcPingDest >= '1') && (*pcPingDest <= '9'))
		{
			uint32	u32IP = nmi_inet_addr(pcPingDest);
			M2M_INFO("Pinging %lu\n",u32IP);
			s8Ret = m2m_ping_req(u32IP, 0, pingCb);
		}
		else
		{
			gPingRequired = 1;
			gethostbyname(pcPingDest);
		}
	}
EXIT:
	return s8Ret;
}

static tstrTlsCrlInfo gCRL = {0};
sint8 atCmd_TlsCrlReset_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	uint8 u8CertRevocType = atoi(data->au8ParamsList[0]);

	if(gaAt_CMD_handler_fn[AT_TLS_CRL_RESET].u8NoOfParameters != data->u8NumOfParameters)
	{
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	memset(&gCRL,0,sizeof(tstrTlsCrlInfo));
	gCRL.u8CrlType = u8CertRevocType;

	M2M_INFO("CRL has been reset");
	AT_SEND_OK(data->au8Cmd);

EXIT:
	return s8Ret;
}

/*
uint8 AsciiString2Hex(uint8 *pu8AsciiString)

Description:
	Converts colon-separated ascii string representing hex bytes into hex.
	Conversion is done in-place.

Params:
	pu8AsciiString	Zero-terminated ascii string. Must be colon-separated, with characters in the
						range [0-9,a-f,A-F]. 1 or 2 characters between each colon.

Return:
	Length of resulting hex buffer.
	0 if input string could not be processed.
*/
static uint8 AsciiString2Hex(uint8 *pu8AsciiString)
{
	uint8 *pu8Dat = (uint8 *)strtok((char *)pu8AsciiString,":");
	uint8 u8DataLen = 0;

	while (pu8Dat != NULL)
	{
		uint8 u8Hex = Ascii2Hex(*pu8Dat++);
		if(u8Hex == 0xff)
			return 0;
		if(*pu8Dat != 0)
		{
			uint8 u8Tmp = Ascii2Hex(*pu8Dat++);
			if(u8Tmp == 0xff)
				return 0;
			u8Hex = u8Hex * BASE_16 + u8Tmp;
		}
		if(*pu8Dat != 0)
			return 0;
		*(pu8AsciiString + u8DataLen) = u8Hex;
		u8DataLen++;
		// Look for the next output of strtok
		pu8Dat = (uint8 *)strtok(NULL,":");
	}

	return u8DataLen;
}

sint8 atCmd_TlsCrlAdd_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	uint8 u8Index;
	uint8 u8DataLen = atoi(data->au8ParamsList[0]);	/* Data length specified by user */

	tstrTlsCrlEntry *pCrlEntry = gCRL.astrTlsCrl;

	if(gaAt_CMD_handler_fn[AT_TLS_CRL_ADD].u8NoOfParameters != data->u8NumOfParameters)
	{
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	if(u8DataLen > TLS_CRL_DATA_MAX_LEN)
	{
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT;
	}

	if(AsciiString2Hex(data->au8ParamsList[1]) != u8DataLen)
	{
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT;
	}

	for(u8Index=0; u8Index<TLS_CRL_MAX_ENTRIES; u8Index++)
	{
		if(pCrlEntry->u8DataLen == 0)
			break;
		pCrlEntry++;
	}

	if(u8Index == TLS_CRL_MAX_ENTRIES)
	{
		M2M_INFO("ERROR: No space to allocate another entry");
		s8Ret = M2M_ERR_FAIL;
		goto EXIT;
	}

	pCrlEntry->u8DataLen = u8DataLen;
	memcpy(pCrlEntry->au8Data, data->au8ParamsList[1], u8DataLen);

	M2M_INFO("New entry added to CRL");
	AT_SEND_OK(data->au8Cmd);

EXIT:
	return s8Ret;
}

sint8 atCmd_TlsCrlSend_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = M2M_ERR_FAIL;

	if(gaAt_CMD_handler_fn[AT_TLS_CRL_SEND].u8NoOfParameters != data->u8NumOfParameters)
	{
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	s8Ret = m2m_wifi_send_crl(&gCRL);
	M2M_INFO("CRL sent");

EXIT:
	return s8Ret;
}

sint8 atCmd_SslGlobalOption_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = M2M_ERR_FAIL;

	if((data->u8NumOfParameters > gaAt_CMD_handler_fn[AT_SSL_OPT].u8NoOfParameters) || (data->u8NumOfParameters < 4))
	{
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	memset(gHostNameSNI,0,AT_MAX_HOST_NAME);
	if (data->au8ParamsList[3][0] != '0')
		memcpy(gHostNameSNI,data->au8ParamsList[3],AT_MAX_HOST_NAME);

	memset(gAlpnList, 0, ALPN_LIST_MAX_APP_LENGTH);
	if(data->u8NumOfParameters > 4)
	{
		if(data->au8ParamsList[4][0] != '0')
			memcpy(gAlpnList, data->au8ParamsList[4], ALPN_LIST_MAX_APP_LENGTH);
	}

	gstrSslOptions.bIsBypassModeEnabled = atoi(data->au8ParamsList[0]);
	gstrSslOptions.bIsSessionCachingEnabled = atoi(data->au8ParamsList[1]);
	gstrSslOptions.bIsCertNameCheckEnabled = atoi(data->au8ParamsList[2]);

	AT_SEND_OK("SSL_OPTIONS");
	s8Ret = M2M_SUCCESS;	
EXIT:
	return s8Ret;
}

sint8 atCmd_GetSysTime_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;

	if(data->u8NumOfParameters != gaAt_CMD_handler_fn[AT_INDEX_GETIME].u8NoOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	if(m2m_wifi_get_system_time() != M2M_SUCCESS)
	{
		M2M_INFO("ERROR: Failed to request SysTime to the WINC");
		s8Ret = M2M_ERR_FAIL;
		goto EXIT;
	}
EXIT:
	return s8Ret;
}

sint8 atCmd_TxPwr_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
    sint8 s8Ret = AT_ERR_NO_ERROR;
    uint8 pwr_val = 0;
    
    if(gaAt_CMD_handler_fn[AT_TX_PWR].u8NoOfParameters != data->u8NumOfParameters)
	{
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
    
    pwr_val = atoi((char *)data->au8ParamsList[0]);

    if ((pwr_val < TX_PWR_HIGH) || (pwr_val > TX_PWR_LOW))
    {
        M2M_INFO("ERROR: Parameter must be between 1 and 3 inclusive (1==TX_PWR_HIGH, 2==TX_PWR_MED, 3==TX_PWR_LOW)");
        s8Ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT;
    }

    m2m_wifi_set_tx_power(pwr_val);
    M2M_INFO("TX power set to %d", pwr_val);
         
EXIT:
    return s8Ret;
}

sint8 atCmd_SetTxRate_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;
	tstrConfAutoRate strConfAutoRate;

	if(data->u8NumOfParameters != gaAt_CMD_handler_fn[AT_INDEX_SET_TX_RATE].u8NoOfParameters)
	{
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}
	
	strConfAutoRate.enuWlanTxRate = atoi((char *)data->au8ParamsList[0]);
	
	if (m2m_wifi_conf_auto_rate(&strConfAutoRate) != M2M_SUCCESS)
	{
		M2M_INFO("ERROR: Failed to change TX rate.");
		s8Ret = M2M_ERR_FAIL;
		goto EXIT;
	}
	
	AT_SEND_OK(data->au8Cmd);
	s8Ret = M2M_SUCCESS;
	EXIT:
	return s8Ret;
}

/************************************************************************/
/* Host File Download Handler Example                                   */
/************************************************************************/
typedef struct {
    uint8  u8Handler;
    uint32 u32Size;
    uint32 u32Offset;
    uint32 u32SizeToRead;
}tstrFileDescriptor;

static tstrFileDescriptor gstrAppFile;

/**
*   @fn         FileGetCallback(uint8 u8Status, uint8 u8Handler, uint32 u32Size)
*   @brief      Callback to be executed when a given read operation completes.
*   @param[in]  u8Status
*                   The result of the Get File operation.
*   @param[in]  u8Handler
*                   A handler for the file just downloaded.
*   @param[in]  u32Size
*                   Size of the file (in bytes).
*/
static void FileGetCallback(uint8 u8Status, uint8 u8Handler, uint32 u32Size)
{
    if(OTA_STATUS_SUCCESS == u8Status)
    {
        gstrAppFile.u8Handler = u8Handler;
        gstrAppFile.u32Size   = u32Size;
        M2M_INFO("File Handler %d, Total Size: %d\n", gstrAppFile.u8Handler, gstrAppFile.u32Size);
        AT_SEND_OK("File Get Successful.\n");
    }
    else
        AT_SEND_ERROR(u8Status, "File Get Failed.\n");
}

/**
*   @fn         atCmd_HostFileGet_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
*   @brief      Start the Host File download from the URL provided in AT Command.
*   @return     Status of the operation.
*/
sint8 atCmd_HostFileGet_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
    (void)moreData;
    (void)useStoredValue;
    sint8 s8Ret = AT_ERR_NO_ERROR;

    if(gaAt_CMD_handler_fn[AT_INDEX_HFD_GET].u8NoOfParameters != data->u8NumOfParameters){
        s8Ret = AT_ERR_NUM_OF_PARAMS;
        goto EXIT;
    }

    gstrAppFile.u8Handler = HFD_INVALID_HANDLER;
    gstrAppFile.u32Size   = 0;
    s8Ret = m2m_ota_host_file_get((char *)data->au8ParamsList[0], FileGetCallback);

EXIT:
    if(s8Ret != M2M_SUCCESS)
        M2M_ERR("atCmd_HostFileGet_Handler failed with ret=%d\r\n", s8Ret);

    return s8Ret;
}

//#define         HFD_PRINT_FILE_CONTENTS

tstrHashContext HFD_MD5_Context;
uint8           HFD_MD5_digest[MD5_DIGEST_SIZE];

/**
*   @fn         FileReadCallback_hif(uint8 u8Status, void *pBuff, uint32 u32Size)
*   @brief      Callback to be executed when a read operation via HIF completes.
*   @param[in]  u8Status
*                   The result of the Erase File operation.
*   @param[in]  pBuff
*                   Pointer to the buffer containing the data read.
*   @param[in]  u32Size
*                   Amount of data read (in bytes).
*/
static void FileReadCallback_hif(uint8 u8Status, void *pBuff, uint32 u32Size)
{
    sint8 s8Ret;
    uint32 i;
    
    if(OTA_STATUS_SUCCESS == u8Status)
    {
#ifdef HFD_PRINT_FILE_CONTENTS
        for(i = 0; i < u32Size; i++)
            printf("%02X", ((uint8 *)pBuff)[i]);    // Do something with the data
#endif

        static uint32 u32sz;
        u32sz += u32Size;
        if(u32sz < gstrAppFile.u32SizeToRead){
            gstrAppFile.u32Offset += u32Size;
            s8Ret =  m2m_ota_host_file_read_hif(gstrAppFile.u8Handler, gstrAppFile.u32Offset, (gstrAppFile.u32SizeToRead - u32sz), FileReadCallback_hif);
            if(M2M_SUCCESS != s8Ret)
                AT_SEND_ERROR(s8Ret, "File Read Failed.\n");
            else
                MD5_UPDATE(&HFD_MD5_Context, pBuff, u32Size);
        }
        else{
            MD5_UPDATE(&HFD_MD5_Context, pBuff, u32Size);
            printf("\nMD5: ");
            MD5_FINISH(&HFD_MD5_Context, HFD_MD5_digest);
            for(i = 0; i < sizeof(HFD_MD5_digest); i++)
                printf("%02x", HFD_MD5_digest[i]);

            printf("\n");
            AT_SEND_OK("HIF File Read Successful.\n");
            u32sz = 0;
        }
    }
    else
        AT_SEND_ERROR(u8Status, "HIF File Read Failed.\n");
}

/* Up to the App developer to pick an appropriate value, this is just an example */
#define FILE_MAX_SPI_BUFF_SIZE   1024

/**
*   @fn         atCmd_HostFileRead_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
*   @brief      Read the Host File stored in WINC's Flash. Can be done via HIF or SPI (specified in the AT Command as argument).
*               The AT Command can also specify the the file handler, offset to read from and size. See AT+HFD_READ help.
*               If these are specified as zero, the values in gstrAppFile will be used (Except for the offset).
*   @return     Status of the operation.
*   @note       Reading the file via SPI will require the Application to reinitialize all HIF callbacks and reconnect to the network.
*/
sint8 atCmd_HostFileRead_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
    (void)moreData;
    (void)useStoredValue;
    uint8 au8Buff[FILE_MAX_SPI_BUFF_SIZE];
    uint32 u32BuffSize   = FILE_MAX_SPI_BUFF_SIZE;
    sint8 s8Ret          = M2M_ERR_FAIL;
    uint32 i             = 0;
    uint32 u32AmountRead = 0;
    uint32 u32offset     = 0;
    uint8 u8Handler      = gstrAppFile.u8Handler;

    if(gaAt_CMD_handler_fn[AT_INDEX_HFD_READ].u8NoOfParameters != data->u8NumOfParameters){
        s8Ret = AT_ERR_NUM_OF_PARAMS;
        goto EXIT;
    }

    memset(au8Buff, 0, FILE_MAX_SPI_BUFF_SIZE);

    u32offset = atoi((char *)data->au8ParamsList[3]);

    if(atoi((char *)data->au8ParamsList[1]) != 0)
        u8Handler = atoi((char *)data->au8ParamsList[1]);

    gstrAppFile.u32SizeToRead = gstrAppFile.u32Size;

    if(atoi((char *)data->au8ParamsList[2]) != 0)
        gstrAppFile.u32SizeToRead = atoi((char *)data->au8ParamsList[2]);

    memset(HFD_MD5_digest, 0, sizeof(HFD_MD5_digest));
    MD5_INIT(&HFD_MD5_Context);

    if(strcmp((char *)data->au8ParamsList[0],"HIF")==0) {
        gstrAppFile.u32Offset = u32offset;
        s8Ret = m2m_ota_host_file_read_hif(u8Handler, gstrAppFile.u32Offset, gstrAppFile.u32SizeToRead, FileReadCallback_hif);
        if(M2M_SUCCESS != s8Ret) goto EXIT;
    }
    else if(strcmp((char *)data->au8ParamsList[0],"SPI")==0) {
        /* Ensure that the file handler is valid before changing the WINC's mode of operation */
        if((HFD_INVALID_HANDLER == gstrAppFile.u8Handler) || (u8Handler != gstrAppFile.u8Handler))
        {
            AT_SEND_ERROR(gstrAppFile.u8Handler, "File Handler is invalid.\n");    
            goto EXIT;
        }

        if(WIFI_STATE_DEINIT != m2m_wifi_get_state())
            m2m_wifi_deinit(NULL);

        s8Ret = m2m_wifi_download_mode();

        if(M2M_SUCCESS != s8Ret) goto REINIT;

        while(u32AmountRead < gstrAppFile.u32SizeToRead)
        {
            u32BuffSize = gstrAppFile.u32SizeToRead - u32AmountRead;
            if(u32BuffSize > FILE_MAX_SPI_BUFF_SIZE)
                u32BuffSize = FILE_MAX_SPI_BUFF_SIZE;

            s8Ret = m2m_ota_host_file_read_spi(u8Handler, au8Buff, u32offset + u32AmountRead, u32BuffSize);
            if(M2M_SUCCESS != s8Ret) goto REINIT;   // Ensure that the WINC gets out of the download mode

            MD5_UPDATE(&HFD_MD5_Context, au8Buff, u32BuffSize);

#ifdef HFD_PRINT_FILE_CONTENTS
            /* Do something with the contents of au8buff, for example write it to the host flash.
               In here and as an example, we just print out the values. */
            for(i = 0; i < u32BuffSize; i++)
                printf("%02X", ((uint8 *)au8Buff)[i]);
#endif

            u32AmountRead += u32BuffSize;
        }

        printf("\nMD5: ");
        MD5_FINISH(&HFD_MD5_Context, HFD_MD5_digest);
        for(i = 0; i < sizeof(HFD_MD5_digest); i++)
            printf("%02x", HFD_MD5_digest[i]);

        printf("\n");
REINIT:
        /* Reset the WINC, reinit WIFI and reconnect to previously connected network */
        s8Ret = winc_reset(1, 1, M2M_WIFI_MODE_NORMAL);
    }
    else {
        s8Ret = AT_ERR_INVALID_ARGUMENTS;
        goto EXIT;
    }

EXIT:
    if(M2M_SUCCESS != s8Ret)
        AT_SEND_ERROR(s8Ret, "File Read Failed.\n");

    return s8Ret;
}

/**
*   @fn         FileEraseCallback(uint8 u8Status)
*   @brief      Callback to be executed when file erase is completed.
*   @param[in]  u8Status
*                   The result of the Erase File operation.
*/
static void FileEraseCallback(uint8 u8Status)
{
    if(OTA_STATUS_SUCCESS == u8Status)
    {
        AT_SEND_OK("File Erase Successful.\n");
    }
    else
    {
        AT_SEND_ERROR(u8Status, "File Erase Failed.\n");
    }

    gstrAppFile.u8Handler = HFD_INVALID_HANDLER;
    gstrAppFile.u32Size   = 0;
}

/**
*   @fn         atCmd_HostFileErase_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
*   @brief      Erase Host File via AT Command.
*               If File Handler is given as zero, handler in gstrAppFile will be used.
*   @return     Status of the operation.
*/
sint8 atCmd_HostFileErase_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
    (void)moreData;
    (void)useStoredValue;
    sint8 s8Ret = M2M_SUCCESS;
    uint8 u8Handler = gstrAppFile.u8Handler;

    if(gaAt_CMD_handler_fn[AT_INDEX_HFD_ERASE].u8NoOfParameters != data->u8NumOfParameters){
        s8Ret = AT_ERR_NUM_OF_PARAMS;
        goto EXIT;
    }

    if(atoi((char *)data->au8ParamsList[0]) != 0)
        u8Handler = atoi((char *)data->au8ParamsList[0]);

    s8Ret = m2m_ota_host_file_erase(u8Handler, FileEraseCallback);

    gstrAppFile.u8Handler = HFD_INVALID_HANDLER;
    gstrAppFile.u32Size   = 0;
EXIT:
    if(s8Ret != M2M_SUCCESS)
        M2M_ERR("atCmd_HostFileErase_Handler failed with ret=%d\r\n", s8Ret);

    return s8Ret;
}

sint8 atCmd_Roaming_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	(void)moreData;
	(void)useStoredValue;
	sint8 s8Ret = M2M_SUCCESS;

	if((data->u8NumOfParameters != 1) && (data->u8NumOfParameters != 2))
	{
		return AT_ERR_NUM_OF_PARAMS;
	}
	
	if(data->u8NumOfParameters == 2)
	{
		if ((atoi((char *)data->au8ParamsList[0]) == '0'))
		{
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			M2M_INFO("ERROR: Only 2 arguments used for enabling roaming.");
			goto EXIT;
		}
		s8Ret = m2m_wifi_enable_roaming(atoi((char *)data->au8ParamsList[1]));
	}
	else
	{
		s8Ret = m2m_wifi_disable_roaming();
	}

	if (s8Ret == M2M_SUCCESS)
	AT_SEND_OK(data->au8Cmd);
	else
		AT_SEND_ERROR(s8Ret, "Roaming operation failed\n");

EXIT:
	return s8Ret;
}

sint8 atCmd_NTP_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	(void)moreData;
	(void)useStoredValue;
	sint8 s8Ret = M2M_ERR_FAIL;

	if(data->u8NumOfParameters == 2)
	{
		s8Ret = m2m_wifi_configure_sntp(
			data->au8ParamsList[0],
			strlen(data->au8ParamsList[0]),
			((char*)data->au8ParamsList[1])[0] == '1'
		);
		
		if(s8Ret == M2M_SUCCESS)
			AT_SEND_OK(data->au8Cmd);
	}

	return s8Ret;
}

enum enum_enterpriseIndexes{
	ENTERPRISE_INDEX_URLKEY = 0,
	ENTERPRISE_INDEX_URLCERT
};

enum enum_enterpriseStates{
	ENTERPRISE_STATE_IDLE = 0,
	ENTERPRISE_STATE_DLKEY,
	ENTERPRISE_STATE_DLCERT,
	ENTERPRISE_STATE_ERROR
};

static tstrHTTPClient* gphHTTPClientHandle;

static const char b64_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

static uint32 base64decode(char* src, uint32 len)
{
	uint32 i = 0;
	uint32 j = 0;
	uint32 size = 0;
	uint8 buf[3];
	uint8 tmp[4];
	char* dst = src;
	
	// Skip line which contains '-' (-----BEGIN ...)
	for(;j<len;j++)
	{
		if(src[j] == '-')
			break;
	}
	for(;j<len;j++)
	{
		if(src[j] == '\r' || src[j] == '\n')
		{
			len -= j;
			src = &src[j];
			break;
		}
	}

	for(uint32 j=0;j<len;j++)
	{
		char c = src[j];

		if(c == '\r' || c == '\n') // Ignore new line characters
			continue;
		else if (c == '=' || (!isalnum(c) && c != '+' && c != '/')) // Stop if character is padding (=) or invalid
			break;

		tmp[i++] = c;

		if (i == 4)
		{
			// Translate values in 'tmp' from table
			for (i=0;i<4;i++)
			{
				// Find translation char in 'b64_table'
				for (uint8 x=0;x<64;x++)
				{
					if (tmp[i] == b64_table[x])
					{
						tmp[i] = x;
						break;
					}
				}
			}

			// Decode
			buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
			buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
			buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

			memcpy(&dst[size], buf, sizeof(buf));
			size += sizeof(buf);

			i = 0;
		}
	}

	// Remainder
	if (i > 0)
	{
		// Fill remaining chars with '\0'
		for (uint8 j=i;j<4;j++)
			tmp[j] = '\0';

		// Translate remainder
		for (uint8 j=0;j<4;j++)
		{
			// Find translation char in 'b64_table'
			for (uint8 x=0;x<64;x++)
			{
				if(tmp[j] == b64_table[x])
				{
					tmp[j] = x;
					break;
				}
			}
		}

		// Decode remainder
		buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
		buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
		buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

		memcpy(&dst[size], buf, i - 1);
		size += i - 1;
	}

	return size;
}

static uint32 decodeLength(uint8* buff, uint8* lenBytes)
{
	// The length byte can be in 2 forms:
	// Short form: 8th bit is 0, remaining bits is the length of the section, up to 127 bytes.
	// Long form: 8th bit is 1, remaining bits are how many subsequent bytes make up the length.
	// See ISO/IEC 8825-1 section 8.1.3
	// https://www.itu.int/ITU-T/studygroups/com17/languages/X.690-0207.pdf
	
	uint32 length = buff[0];
	*lenBytes = 0;

	if(length & 0x80) // Long form
	{
		*lenBytes = length & 0x7F; // Get how many bytes make up the length value
		length = 0;
		for(uint8 i=0;i<*lenBytes;i++)
			length += (uint32)(buff[i + 1] << ((*lenBytes - i - 1) * 8));
	}

	return length;
}

static sint8 httpEnterpiseClientCallback(uint8 *pu8Chunk, sint16 s16ChunkSize, uint32 u32Arg);

static void enterpriseStateProcess(tstrHTTPClient* phHTTPClientHandle)
{
	if(gstrEnterprise.state == ENTERPRISE_STATE_IDLE)
	{
		HTTP_ClientInit();
		gphHTTPClientHandle = HTTP_ClientConnect(gstrEnterprise.urlKey, HTTP_CLIENT_ACTION_DOWNLOAD, 0, httpEnterpiseClientCallback, (uint32)&gphHTTPClientHandle);
		gstrEnterprise.state = ENTERPRISE_STATE_DLKEY;
	}
	else if(gstrEnterprise.state == ENTERPRISE_STATE_DLKEY)
	{
		HTTP_ClientStop(phHTTPClientHandle);
		phHTTPClientHandle->bIsRunning = 0;

		gphHTTPClientHandle = HTTP_ClientConnect(gstrEnterprise.urlCert, HTTP_CLIENT_ACTION_DOWNLOAD, 0, httpEnterpiseClientCallback, (uint32)&gphHTTPClientHandle);
		gstrEnterprise.state = ENTERPRISE_STATE_DLCERT;
	}
	else if(gstrEnterprise.state == ENTERPRISE_STATE_ERROR)
	{
		HTTP_ClientStop(phHTTPClientHandle);
		phHTTPClientHandle->bIsRunning = 0;

		gstrEnterprise.state = ENTERPRISE_STATE_IDLE;
		AT_SEND_ERROR(1, "1X_TLS_CRED");
	}
	else // ENTERPRISE_STATE_DLCERT
	{
		HTTP_ClientStop(phHTTPClientHandle);
		phHTTPClientHandle->bIsRunning = 0;
		
		gstrEnterprise.state = ENTERPRISE_STATE_IDLE;
		
		// https://crypto.stackexchange.com/questions/21102/what-is-the-ssl-private-key-file-format
		// See ISO/IEC 8825-1 section 8.1.3

		if(gstrEnterprise.keyBuff[0] == 0x30 && gstrEnterprise.keyBuff[1] == 0x82)
		{
			uint32 offset = 8;

			uint8 modulusLenBytes;
			uint32 modulusLen = decodeLength(&gstrEnterprise.keyBuff[offset], &modulusLenBytes);
			offset += modulusLenBytes + 1;
			uint8* modulus = &gstrEnterprise.keyBuff[offset];

			offset += 2 + modulusLen;
			uint8 pubExponentLenBytes;
			uint32 pubExponentLen = decodeLength(&gstrEnterprise.keyBuff[offset], &pubExponentLenBytes);
			
			offset += 2 + pubExponentLenBytes + 1 + pubExponentLen;
			uint8 privExponentLenBytes;
			uint32 privExponentLen = decodeLength(&gstrEnterprise.keyBuff[offset], &privExponentLenBytes);
			offset += privExponentLenBytes + 1;
			uint8* privExponent = &gstrEnterprise.keyBuff[offset];
			
			// Modulus contains an extra 0 byte at the beginning to make sure it's always a positive
			// integer when stored as a signed variable, but we don't want that extra byte for our stuff.
			if(modulusLen & 1)
			{
				modulus++;
				modulusLen--;
			}
			if(privExponentLen & 1)
			{
				privExponent++;
				privExponentLen--;
			}

			if((modulusLen == 64 || modulusLen == 128 || modulusLen == 256) && modulusLen == privExponentLen)
			{
				gstrEnterprise.ptrModulus = modulus;
				gstrEnterprise.modulusLength = modulusLen;
				gstrEnterprise.ptrPrivateExponent = privExponent;
				gstrEnterprise.privateExponentLength = privExponentLen;
				AT_SEND_OK("1X_TLS_CRED");
			}
			else
			{
				M2M_INFO("Bad private key file format (unsupported modulus %u or private key %u length)!\r\n", modulusLen, privExponentLen);
				AT_SEND_ERROR(1, "1X_TLS_CRED");
			}
		}
		else
		{
			M2M_INFO("Bad private key file format!\r\n");
			AT_SEND_ERROR(1, "1X_TLS_CRED");
		}
		
	}
}

static sint8 httpEnterpiseClientCallback(uint8 *pu8Chunk, sint16 s16ChunkSize, uint32 u32Arg)
{
	tstrHTTPClient* phHTTPClientHandle = *((tstrHTTPClient**)u32Arg);
	uint16* len;
	uint8* buff;
	uint16 maxSize;

	if(gstrEnterprise.state == ENTERPRISE_STATE_DLKEY)
	{
		len = &gstrEnterprise.keyLen;
		buff = gstrEnterprise.keyBuff;
		maxSize = sizeof(someSharedMemory);
	}
	else if(gstrEnterprise.state == ENTERPRISE_STATE_DLCERT)
	{
		len = &gstrEnterprise.certLen;
		buff = gstrEnterprise.certBuff;
		maxSize = sizeof(gstrEnterprise.certBuff);
	}
	else
		return 0;
	
	if(pu8Chunk != NULL)
	{
		if(*len + s16ChunkSize <= maxSize)
		{
			memcpy(&buff[*len], pu8Chunk, s16ChunkSize);
			*len += s16ChunkSize;
		}
		else
		{
			M2M_INFO("\"%s\" Fail! Out of space! (Max %u)\n", phHTTPClientHandle->acFileName, maxSize);
			gstrEnterprise.state = ENTERPRISE_STATE_ERROR;
			enterpriseStateProcess(phHTTPClientHandle);
		}
	}
	else
	{
		if(s16ChunkSize == HTTP_CLIENT_CONNECTED)
		{

		}
		else if(s16ChunkSize == HTTP_CLIENT_DOWNLOADING)
		{
			uint32 u32FileSize = phHTTPClientHandle->strRxMsg.u32ContentLength;
			M2M_INFO("File size: %u B\n",u32FileSize);
			if(u32FileSize > maxSize)
			{
				M2M_INFO("\"%s\" Fail! File too large! (Max %u)\n", phHTTPClientHandle->acFileName, maxSize);
				gstrEnterprise.state = ENTERPRISE_STATE_ERROR;
				enterpriseStateProcess(phHTTPClientHandle);
			}
		}
		else if(s16ChunkSize == HTTP_CLIENT_FINISHED)
		{
			//printf("\r\nCODED\r\n");
			//for(uint32 i=0;i<*len;i++)
			//	printf("%c", buff[i]);
			//printf("\r\n\r\n");
			
			*len = base64decode(buff, *len);
			printf("\"%s\" Complete! Decoded len: %u\r\n", phHTTPClientHandle->acFileName, *len);
			
			//printf("\r\nDECODED\r\n");
			//for(uint32 i=0;i<*len;i++)
			//	printf("%02x", buff[i]);
			//printf("\r\n\r\n");
			
			enterpriseStateProcess(phHTTPClientHandle);
		}
		else if(s16ChunkSize == HTTP_CLIENT_CONNECTION_ERROR)
		{
			M2M_INFO("\"%s\" Fail!\n", phHTTPClientHandle->acFileName);
			gstrEnterprise.state = ENTERPRISE_STATE_ERROR;
			enterpriseStateProcess(phHTTPClientHandle);
		}
	}

	return 0;
}

sint8 atCmd_1X_TLS_Cred_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = M2M_ERR_FAIL;
	tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;
	
	PRINT("atCmd_1X_TLS_Cred_Handler\r\n");

	if(gaAt_CMD_handler_fn[AT_INDEX_1X_TLS_CRED].u8NoOfParameters != data->u8NumOfParameters)
	{
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	if(status->u8Sta_status != SERVICE_IS_RUNNING || gstrEnterprise.state != ENTERPRISE_STATE_IDLE)
	{
		// WiFi is not connected to AP or we've already ran the AT+1X_TLS_CRED command and it's still busy getting files
		s8Ret = AT_ERR_INVALID_OPERATION;
		goto EXIT;
	}

	memset(&gstrEnterprise, 0, sizeof(gstrEnterprise));
	
	gstrEnterprise.keyBuff = someSharedMemory;

	memcpy(gstrEnterprise.urlKey, data->au8ParamsList[ENTERPRISE_INDEX_URLKEY], strnlen((const char*)data->au8ParamsList[ENTERPRISE_INDEX_URLKEY], KEY_CERT_MAX_SIZE));
	memcpy(gstrEnterprise.urlCert, data->au8ParamsList[ENTERPRISE_INDEX_URLCERT], strnlen((const char*)data->au8ParamsList[ENTERPRISE_INDEX_URLCERT], KEY_CERT_MAX_SIZE));

	M2M_INFO("\r\n>Key: %s\r\n>Cert: %s\r\n",
		gstrEnterprise.urlKey,
		gstrEnterprise.urlCert
	);
	
	enterpriseStateProcess(NULL);
	
	s8Ret = M2M_SUCCESS;
EXIT:
	return s8Ret;
}

sint8 atCmd_1X_Connect_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = M2M_ERR_FAIL;
	tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;
	tstrNetworkId strNetworkId;
	
	PRINT("atCmd_1X_Connect_Handler\r\n");
	
	if(gaAt_CMD_handler_fn[AT_INDEX_1X_CONN].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	if(SERVICE_IS_RUNNING == status->u8Sta_status) { /*WiFi is already connected to AP*/
		s8Ret = AT_ERR_INVALID_OPERATION;
		goto EXIT;
	}
	
	if(SERVICE_IS_RUNNING == status->u8Ap_status) /*Check if AP mode is running*/
	{
		s8Ret = AT_ERR_INVALID_OPERATION;
		goto EXIT;
	}
	
	/*Clear gstrConn first*/
	memset((char *)&gstrConn,0,sizeof(gstrConn));
	
	/*Get SSID*/
	memcpy(gstrConn.ssid, data->au8ParamsList[CONN1X_INDEX_SSID], strnlen((const char *)data->au8ParamsList[CONN1X_INDEX_SSID], sizeof(gstrConn.ssid)));
	
	/*Get SSID Length*/
	gstrConn.ssidLen = Min( atoi((const char *)data->au8ParamsList[CONN1X_INDEX_SSID_LENGTH]), sizeof(gstrConn.ssid) );

	/*Get domain and username*/
	char* at = strchr((char*)data->au8ParamsList[CONN1X_INDEX_USERNAMEDOMAIN], '@');
	char* slash = strchr((char*)data->au8ParamsList[CONN1X_INDEX_USERNAMEDOMAIN], '\\');
	if(at != NULL)
	{
		gstrConn.domain = at;
		gstrConn.domainLen = strlen((const char *)gstrConn.domain);
		gstrConn.username = data->au8ParamsList[CONN1X_INDEX_USERNAMEDOMAIN];
		gstrConn.usernameLen = gstrConn.domain - gstrConn.username;
	}
	else if(slash != NULL)
	{
		gstrConn.username = slash + 1;
		gstrConn.usernameLen = strlen((const char *)gstrConn.username);
		gstrConn.domain = data->au8ParamsList[CONN1X_INDEX_USERNAMEDOMAIN];
		gstrConn.domainLen = gstrConn.username - gstrConn.domain;
	}
	else
	{
		gstrConn.domain = NULL;
		gstrConn.domainLen = 0;
		gstrConn.username = data->au8ParamsList[CONN1X_INDEX_USERNAMEDOMAIN];
		gstrConn.usernameLen = strlen((const char *)gstrConn.username);
	}

	/*Get password*/
	gstrConn.password = data->au8ParamsList[CONN1X_INDEX_PASSWORD];
	gstrConn.passwordLen = strlen((const char *)data->au8ParamsList[CONN1X_INDEX_PASSWORD]);
	
	/*Get CHANNEL*/
	gstrConn.channel = atoi((const char *)data->au8ParamsList[CONN1X_INDEX_CHANNEL]);
	
	strNetworkId.pu8Bssid = NULL;
	strNetworkId.pu8Ssid = (uint8*)gstrConn.ssid;
	strNetworkId.u8SsidLen = gstrConn.ssidLen;
	strNetworkId.enuChannel = (tenuM2mScanCh)gstrConn.channel;

	if(strcmp((char*)gstrConn.password, "EAP-TLS") == 0 || strcmp((char*)gstrConn.password, "P-EAP-TLS") == 0)
	{
		tstrAuth1xTls auth1xTls = {
				gstrConn.domain,
				gstrConn.username,
				gstrEnterprise.ptrModulus,
				gstrEnterprise.ptrPrivateExponent,
				gstrEnterprise.certBuff,
				gstrConn.domainLen,
				gstrConn.usernameLen,
				gstrEnterprise.privateExponentLength,
				gstrEnterprise.certLen,
				(strcmp((char*)gstrConn.password, "EAP-TLS") == 0),
				(at == NULL)
		};
		s8Ret = m2m_wifi_connect_1x_tls(WIFI_CRED_SAVE_ENCRYPTED, &strNetworkId, &auth1xTls);
	}
	else
	{
		tstrAuth1xMschap2 auth1xMschap2 = {
				gstrConn.domain,
				gstrConn.username,
				gstrConn.password,
				gstrConn.domainLen,
				gstrConn.usernameLen,
				gstrConn.passwordLen,
				false,
				(at == NULL)
		};
		s8Ret = m2m_wifi_connect_1x_mschap2(WIFI_CRED_SAVE_ENCRYPTED, &strNetworkId, &auth1xMschap2);
	}

EXIT:
	return s8Ret;
}

sint8 atCmd_1X_Options_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;

	tstr_test_conn_status *status = (tstr_test_conn_status *)moreData;

	PRINT("atCmd_1X_ServerAuth_Options_Handler\r\n");

	if(gaAt_CMD_handler_fn[AT_INDEX_1X_OPTIONS].u8NoOfParameters != data->u8NumOfParameters){
		s8Ret = AT_ERR_NUM_OF_PARAMS;
		goto EXIT;
	}

	/* Set Bypass option. */
	{
		int option_value;

		switch(atoi((const char *)data->au8ParamsList[0]))
		{
		case 0:
			option_value = 0;
			break;
		case 1:
			option_value = 1;
			break;
		default:
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			goto EXIT;
		}
		m2m_wifi_1x_set_option(WIFI_1X_BYPASS_SERVER_AUTH, &option_value, sizeof(option_value));
	}

	/* Set Specific RootCert option. */
	switch(strlen((const char *)data->au8ParamsList[1]))
	{
	case 1:
		m2m_wifi_1x_set_option(WIFI_1X_SPECIFIC_ROOTCERT, NULL, 0);
		break;
	case 40:
		{
			uint8 au8OptionValue[20];
			hexstr_2_bytes(au8OptionValue, data->au8ParamsList[1], 20);
			m2m_wifi_1x_set_option(WIFI_1X_SPECIFIC_ROOTCERT, au8OptionValue, 20);
		}
		break;
	default:
		s8Ret = AT_ERR_INVALID_ARGUMENTS;
		goto EXIT;
	}

	/* Set Time mode option. */
	{
		tenuSslCertExpSettings option_value;

		switch(atoi((const char *)data->au8ParamsList[2]))
		{
		case 0:
			option_value = SSL_CERT_EXP_CHECK_ENABLE;
			break;
		case 1:
			option_value = SSL_CERT_EXP_CHECK_EN_IF_SYS_TIME;
			break;
		case 2:
			option_value = SSL_CERT_EXP_CHECK_DISABLE;
			break;
		default:
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			goto EXIT;
		}
		m2m_wifi_1x_set_option(WIFI_1X_TIME_VERIF_MODE, &option_value, sizeof(option_value));
	}

	/* Set TLS caching option. */
	{
		int option_value;

		switch(atoi((const char *)data->au8ParamsList[3]))
		{
		case 0:
			option_value = 0;
			break;
		case 1:
			option_value = 1;
			break;
		default:
			s8Ret = AT_ERR_INVALID_ARGUMENTS;
			goto EXIT;
		}
		m2m_wifi_1x_set_option(WIFI_1X_SESSION_CACHING, &option_value, sizeof(option_value));
	}

	AT_SEND_OK(data->au8Cmd);

EXIT:
	return s8Ret;
}

sint8 atCmd_TlsStart_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
	sint8 s8Ret = AT_ERR_NO_ERROR;

	SOCKET sock;
	sint8 status;

	PRINT("atCmd_TlsStart_Handler\r\n");

	sock = (SOCKET) atoi((char *)data->au8ParamsList[0]);
	status = secure(sock);

	if (SOCK_ERR_NO_ERROR != status)
	{
		AT_SEND_ERROR(status, "");
	}
	else
	{
		AT_SEND_OK(data->au8Cmd);
	}

	return s8Ret;
}

static uint8 au8_random[4] = {0};
sint8 atCmd_APIConfig_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
    sint8 ret = AT_ERR_NO_ERROR;
    sint8 acPrint[20] = {0};
    uint8 au8_mac_fake[6] = {0,22,33,44,55,66};
    uint8 au8_mac_real[6] = {0};
    uint8 au8_mac_check[6] = {0};

    ret = m2m_wifi_set_power_profile(PWR_LOW1);
    if (ret == M2M_SUCCESS)
    {
        ret = m2m_wifi_set_battery_voltage(250);
        if (ret == M2M_SUCCESS)
        {
            ret = m2m_wifi_set_battery_voltage(300);
            if (ret == M2M_SUCCESS)
            {
                ret = m2m_wifi_set_tx_power(TX_PWR_LOW);
                if (ret == M2M_SUCCESS)
                {
                    ret = m2m_wifi_set_tx_power(TX_PWR_HIGH);
                    if (ret == M2M_SUCCESS)
                    {
                        ret = m2m_wifi_prng_get_random_bytes(au8_random, sizeof(au8_random));
                    }
                }
            }
        }
    }
    if (ret != M2M_SUCCESS)
        goto EXIT;
    sprintf(acPrint, "MiscAPI");
    AT_SEND_OK(acPrint);

    ret = m2m_wifi_get_mac_address(au8_mac_real);
    if (ret == M2M_SUCCESS)
    {
        ret = m2m_wifi_enable_firmware_logs(0);
        if (ret == M2M_SUCCESS)
        {
            ret = m2m_wifi_set_mac_address(au8_mac_fake);
            if (ret == M2M_SUCCESS)
            {
                ret = m2m_wifi_get_mac_address(au8_mac_check);
                if (ret == M2M_SUCCESS)
                {
                    if (m2m_memcmp(au8_mac_fake, au8_mac_check, 6))
                    {
                        ret = M2M_ERR_FAIL;
                    }
                }
            }
            m2m_wifi_enable_firmware_logs(1);
            m2m_wifi_set_mac_address(au8_mac_real);
        }
    }
    if (ret != M2M_SUCCESS)
        goto EXIT;
    sprintf(acPrint, "MACAPI");
    AT_SEND_OK(acPrint);

    ret = m2m_wifi_enable_sntp(0);
    if (ret == M2M_SUCCESS)
    {
        ret = m2m_wifi_set_system_time(0xFFFFFF00);
        if (ret == M2M_SUCCESS)
        {
            ret = m2m_wifi_get_system_time();
            if (ret == M2M_SUCCESS)
            {
                ret = m2m_wifi_enable_sntp(1);
            }
        }
    }
    if (ret != M2M_SUCCESS)
        goto EXIT;
    sprintf(acPrint, "TimeAPI");
    AT_SEND_OK(acPrint);

    EXIT:
    return ret;
}

sint8 atCmd_APIAPConn_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
    sint8 ret = AT_ERR_NO_ERROR;
    uint8 au8_ie[] = {	10,			0x55,	4,		0xFE,0xDC,0xBA,0x98,			0xAA,	1,		0x12};
    /*					#of all Bytes | IE1 ID | Length1 | Data1(Hex Coded) | ..... | IEx ID | Lengthx | Datax(Hex Coded)	*/
    
    ret = m2m_wifi_set_cust_InfoElement(au8_ie);
    if (ret == M2M_SUCCESS)
    {
        ret = m2m_wifi_get_connection_info();
        if (ret == M2M_SUCCESS)
        {
            ret = m2m_wifi_req_curr_rssi();
        }
    }
    return ret;
}

sint8 atCmd_XOEnable_Handler(tstrAt_cmd_content *data, void* moreData, uint8 useStoredValue)
{
    static uint8 XOen = 0;
    sint8 ret = AT_ERR_NO_ERROR;
    M2M_PRINT("XO is now ");
    if (XOen == 0)
    {
        M2M_PRINT("ON ");
        ret = m2m_wifi_enable_XO_during_sleep(1);
        XOen = 1;
    }
    else
    {
        M2M_PRINT("OFF ");
        ret = m2m_wifi_enable_XO_during_sleep(0);
        XOen = 0;
    }
    M2M_PRINT("during sleep\r\n");
    return ret;
}