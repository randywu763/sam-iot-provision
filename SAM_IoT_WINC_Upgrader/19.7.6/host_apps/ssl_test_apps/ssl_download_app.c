
/*******************************************************************************************
INCLUDES
*******************************************************************************************/

#include "asf.h"
#include "http.h"
#include "ssl_test_conf.h"

#if ((TEST_APP == TEST_APP_SSL_DOWNLOAD) || (TEST_APP == TEST_APP_WPA_ENT_CONN))

/*******************************************************************************************
MACROS
*******************************************************************************************/

#define __SNI_VALIDATION_TEST__
//#define __REPEAT_INFINITELY__
//#define __STOP_ON_FAILURE__


/*
	HTTP CLIENT TEST FILE LIST
*/
#define HTTPS_LOCAL_SERVER_NAME					"EGYLT0004.Corp.Atmel.Com"//"192.168.0.101"//
#define N_HTTPS_FILES							(sizeof(gpacHTTPSFileList)/ 256)

#define HTTP_FILE_LIST		\
{\
	/*
		RSA Authentication
	*/\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4400/",	\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4401/",	\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4402/",	\
	/*
		RSA Authentication (WITH CLIENT_AUTH)
	*/\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4420/",	\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4421/",	\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4422/",	\
	/*
		ECDSA Authentication
	*/\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4482/",	\
	/*
		ECDSA Authentication (WITH CLIENT_AUTH)
	*/\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4492/",	\
	/*
		RSA Certificates with different hash algorithms
	*/\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4224/",	\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4384/",	\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4512/",	\
	/*
		RSA-4096 certificate chain
	*/\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4096/",	\
	/*
		SNI Validation TEST
	*/\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4430/",	\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4431/",	\
	"https://" HTTPS_LOCAL_SERVER_NAME ":4432/"		\
}


/*******************************************************************************************
DATA TYPES
*******************************************************************************************/

typedef struct{
	tstrHTTPClient	*hHTTPClientHandle;
	uint32			u32RxCount;
	uint32			u32DownloadTime;
	char			acFileName[256];
}tstrHTTPSession;


/*******************************************************************************************
GLOBALS
*******************************************************************************************/

static volatile uint8			gu8Event;
tenuAppWifiConnMode				genuConnMode;
static volatile	uint8			gbIsTestFinished			= 0;
static tstrHTTPSession			gstrHTTPSSession;
static volatile uint8			gbHttpClientTestActive		= 0;
static volatile uint8			gbIsHTTPSSessionRunning		= 0;
static char						gpacHTTPSFileList[][256]	= HTTP_FILE_LIST;
static uint8					gau8Status[N_HTTPS_FILES];

/***********************************************************/
static void http_client_test_init(void)
{
	memset(&gstrHTTPSSession, 0, sizeof(tstrHTTPSession));
	gbIsHTTPSSessionRunning	= 0;
	gbIsTestFinished		= 0;
	gbHttpClientTestActive	= 1;
	sslEnableCertExpirationCheck(0);
	HTTP_ClientInit();
}

/***********************************************************/
static sint8 httpsClientCallback(uint8 *pu8Chunk, sint16 s16ChunkSize, uint32 u32Arg)
{
	if(pu8Chunk != NULL)
	{
	}
	else
	{
		if(s16ChunkSize == HTTP_CLIENT_CONNECTED)
		{
		}
		else if(s16ChunkSize == HTTP_CLIENT_FINISHED)
		{
			M2M_INFO("\"%s\" Success\n", gstrHTTPSSession.acFileName);
			HTTP_ClientStop(gstrHTTPSSession.hHTTPClientHandle);
			gbIsHTTPSSessionRunning = 0;
			gau8Status[u32Arg]		= 0;
		}
		else if(s16ChunkSize == HTTP_CLIENT_CONNECTION_ERROR)
		{
			M2M_INFO("\"%s\" Fail!\n", gstrHTTPSSession.acFileName);
			HTTP_ClientStop(gstrHTTPSSession.hHTTPClientHandle);
#ifndef __STOP_ON_FAILURE__
			gbIsHTTPSSessionRunning = 0;
#endif
			gau8Status[u32Arg]		= 1;
		}
	}
	return 0;
}

/***********************************************************/
static void http_test_task(void)
{
#define REPEAT_TEST			(0)
	
	static uint32	u32ServerIdx	= 0;
	static uint32	u32Repeat		= REPEAT_TEST;
	
	if(!gu8IsWiFiConnected)
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
				HTTP_ClientConnect(gpacHTTPSFileList[u32ServerIdx], HTTP_CLIENT_ACTION_DOWNLOAD, 0, httpsClientCallback, u32ServerIdx);
				if(gstrHTTPSSession.hHTTPClientHandle != NULL)
				{

				}
				if(u32Repeat)
				{
					u32Repeat --;
				}
				else
				{
					u32Repeat = REPEAT_TEST;
					u32ServerIdx ++;
				}
			}
			else
			{
				M2M_INFO("Test Finished\n");
				M2M_INFO("*******************************\n");
				M2M_INFO("\tDownload Status\n");
				for(u32ServerIdx = 0; u32ServerIdx < N_HTTPS_FILES; u32ServerIdx ++)
				{
					M2M_INFO("%02lu .................... %s\n", u32ServerIdx, gau8Status[u32ServerIdx] == 0 ? "SUCCESS" : "FAIL");
				}
				M2M_INFO("*******************************\n");

				gbIsTestFinished		= 1;
				gbHttpClientTestActive	= 0;
				u32ServerIdx			= 0;
				
#ifdef __SNI_VALIDATION_TEST__
				if(!gstrSslOptions.bIsSNIEnabled)
				{
					/* Run another test for SNI.
					*/
					M2M_INFO(">>>>>>>>>>>> Start SNI TEST......\n");
					http_client_test_init();
					gstrSslOptions.bIsSNIEnabled = 1;
				}
#endif
#ifdef __REPEAT_INFINITELY__
				http_client_test_init();
#endif
			}
		}
	}
}

/***********************************************************/
static void AppHttpStart(void)
{
	http_client_test_init();
}

/***********************************************************/
static void AppHttpPoll(void)
{
	http_test_task();
}

/***********************************************************/
void WifiCallback(uint8 u8MsgType, void * pvMsg)
{
	if(u8MsgType == M2M_WIFI_RESP_CON_STATE_CHANGED)
	{
		tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged*) pvMsg;
		M2M_INFO("Wifi State :: %s ::\n", pstrWifiState->u8CurrState ? "CONNECTED" : "DISCONNECTED");
		if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED)
		{

		}
		else if(pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED)
		{
			gu8IsWiFiConnected = M2M_WIFI_DISCONNECTED;
			AppWifiConnect(genuConnMode);
		}
	}
	else if(u8MsgType == M2M_WIFI_REQ_DHCP_CONF)
	{
		gu8IsWiFiConnected = M2M_WIFI_CONNECTED;
		tstrM2MIPConfig* pstrM2MIpConfig = (tstrM2MIPConfig*) pvMsg;
		uint8 *pu8IPAddress = (uint8*) &pstrM2MIpConfig->u32StaticIP;
		
		M2M_INFO("DHCP IP Address :: %u.%u.%u.%u ::\n", pu8IPAddress[0], pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
		
		AppHttpStart();
	}
	else if(u8MsgType == M2M_WIFI_RESP_GET_SYS_TIME)
	{
		tstrSystemTime *pstrTime = (tstrSystemTime*)pvMsg;
		M2M_INFO("Time Of Day\n\t%d/%02d/%d %02d:%02d:%02d GMT\n",
		pstrTime->u8Month, pstrTime->u8Day, pstrTime->u16Year,
		pstrTime->u8Hour, pstrTime->u8Minute, pstrTime->u8Second);
	}
}

/***********************************************************/
void SslDownloadAppRun(tenuAppWifiConnMode enuConnMode)
{
	genuConnMode = enuConnMode;
	AppWifiConnect(enuConnMode);
	
	for (;;)
	{
		while(m2m_wifi_handle_events(NULL) != M2M_SUCCESS)
		{
		}
		AppHttpPoll();
	}
}
#endif