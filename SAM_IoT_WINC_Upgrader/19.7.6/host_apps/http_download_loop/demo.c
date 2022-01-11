/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \main page User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */

/*******************************************************************************************
INCLUDES
*******************************************************************************************/

#include "demo.h"
#include "common/include/nm_common.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "driver/include/m2m_wifi.h"
#include "driver/include/m2m_periph.h"
#include "driver/include/m2m_ota.h"
#include "stdio_serial.h"
#include "http.h"
#include "Hash.h"
#include "random_upload.h"
#include "sequence_upload.h"
#include <delay.h>
#include <string.h>

/*******************************************************************************************
MACROS
*******************************************************************************************/

#define STRING_EOL    "\r\n"
#define STRING_HEADER "-- Wifi NMI demo --"STRING_EOL \
"-- "BOARD_NAME" --"STRING_EOL \
"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

#define ENABLED   1
#define DISABLED  0

#define EVENT_BTN1_LONG_PRESS				NBIT0
#define EVENT_BTN1_SHORT_PRESS				NBIT1
#define EVENT_BTN2_LONG_PRESS				NBIT2
#define EVENT_BTN2_SHORT_PRESS				NBIT3
#define EVENT_MOVEMENT						NBIT4
#define EVENT_TIME_OUT						NBIT5

#define M2M_MOVE_MSG  "Movement Detected"
#define M2M_BTN_MSG	   "Btn_press"

#define N_HTTPS_FILES									(sizeof(gpacHTTPSFileList)/ 256)

//#define _DYNAMIC_PS_

/*******************************************************************************************
MACROS
*******************************************************************************************/

typedef struct{
	tstrHTTPClient	*hHTTPClientHandle;
	uint32			u32RxCount;
	char			acFileName[256];
	uint8           bSkipDigestCheck;
	uint32          u32DownloadStartTime;
	uint8           au8ExpectedDigest[20];
}tstrHTTPDownloadSession;

typedef struct{
	tstrHTTPClient	*hHTTPClientHandle;
	uint32			u32TxOffset;
	uint32			u32UploadStartTime;
	char			acFileName[256];	
	uint8 *         pu8FileBuffer;
	uint32          u32FileSize;
	uint32          u32UploadSize;
}tstrHTTPUploadSession;

typedef union 
{

	tstrHTTPDownloadSession dl;
	tstrHTTPUploadSession ul;
} tuniHttpSessions;

typedef struct 
{
	tenuHttpClientAction dir;
	tuniHttpSessions u;	
} tstrHTTPSession;

typedef enum{
	APP_STATE_IDLE,
	APP_STATE_PROV,
	APP_STATE_PSK_CONN,
	APP_STATE_802_1X_CONN,
	APP_STATE_DONE
}tenuAppState;

typedef struct {
	char     gacFileURL[256];
	uint8    bSkipDigestCheck;
	uint8    gau8ExpectedDigest[20];
} tstrDownloadFileInfo;

typedef struct {
	char     gacFileURL[256];
	uint8 * pu8FileBuffer;
	uint32 u32FileSize;
	uint32 u32UploadSize;
} tstrUploadFileInfo;

typedef union {
	tstrDownloadFileInfo dl;
	tstrUploadFileInfo ul;
} tuniFileInfo;
typedef struct {
	tenuHttpClientAction dir;
	tuniFileInfo u;
} tstrFileInfo;

/*******************************************************************************************
GLOBALS
*******************************************************************************************/
extern volatile uint32_t ms_ticks;
static volatile uint8			gu8IsWiFiConnected;
/*******************************************************************************************
PROTOTYPES
*******************************************************************************************/
static sint8 httpsUploadClientCallback(uint8 *pu8Chunk, sint16 s16ChunkSize, tstrHTTPUploadSession * pstrSession);
static sint8 httpsDownloadClientCallback(uint8 *pu8Chunk, sint16 s16ChunkSize, tstrHTTPDownloadSession * pstrSession);

static volatile uint8	gbHttpClientTestActive	= 0;
static volatile uint8	gbIsHTTPSSessionRunning	= 0;
static volatile	uint8	gbIsTestFinished		= 0;
static volatile uint32  gu32RecvCnt             = 0;
static tstrHTTPSession	gstrHTTPSSession;
static tstrHashContext sha1_ctxt;

char gacFileBuffer[] = "This is a file\nLine1\nLine2\n";

static tstrFileInfo gpacHTTPSFileList[]=
{
#if 0
	{ 
		"http://192.168.43.240:8000/", 0,
		{
			0x70, 0x35, 0xe7, 0x73, 0x02, 
			0x16, 0xd5, 0xc0, 0x0e, 0x0b, 
			0xf9, 0x52, 0x00, 0xb2, 0x12, 
			0x70, 0x6a, 0x72, 0x7b, 0x64
		},
	},
#endif
#if 1
	{
		.dir = HTTP_CLIENT_ACTION_DOWNLOAD,
		.u = {
			.dl = {
				"https://s3-us-west-2.amazonaws.com/winc1500/samer/dummy.pdf", 0,
				{
					0x70, 0x35, 0xe7, 0x73, 0x02,
					0x16, 0xd5, 0xc0, 0x0e, 0x0b,
					0xf9, 0x52, 0x00, 0xb2, 0x12,
					0x70, 0x6a, 0x72, 0x7b, 0x64
				},
			},
		},		
	},
#endif
#if 0
	{
		.dir = HTTP_CLIENT_ACTION_UPLOAD,
		.u = {
			.ul = {
				"http://192.168.43.240:8000/test4.txt",
				sequence_data,
				sizeof(sequence_data),
			},
		},
	},
#endif

#if 1
	{
		.dir = HTTP_CLIENT_ACTION_UPLOAD,
		.u = {
			.ul = {
				"http://192.168.4.75:8000/test4.txt",
				random_data,
				sizeof(random_data),
				10*1024*1024ul,
			},
		},
	},
#endif
#if 0
	{
		.dir = HTTP_CLIENT_ACTION_UPLOAD,
		.u = {
			.ul = {
				"http://192.168.43.240:8000/test4.txt",
				random_data,
				sizeof(random_data),
				10*1024*1024ul,
			},
		},
	},
#endif
#if 0
	{
		.dir = HTTP_CLIENT_ACTION_UPLOAD,
		.u = {
			.ul = {
				"http://192.168.43.240:8000/test3.txt",
				&gacFileBuffer,
				sizeof(gacFileBuffer)-1,
			},
		},		
	},
#endif
#if 0
	{ 
		"https://s3.amazonaws.com/aezzat/dummy.pdf", 0,
		{
			0x70, 0x35, 0xe7, 0x73, 0x02, 
			0x16, 0xd5, 0xc0, 0x0e, 0x0b, 
			0xf9, 0x52, 0x00, 0xb2, 0x12, 
			0x70, 0x6a, 0x72, 0x7b, 0x64
		},
	},
	{ 
		"http://s3.amazonaws.com/aezzat/dummy.pdf", 0,
		{
			0x70, 0x35, 0xe7, 0x73, 0x02, 
			0x16, 0xd5, 0xc0, 0x0e, 0x0b, 
			0xf9, 0x52, 0x00, 0xb2, 0x12, 
			0x70, 0x6a, 0x72, 0x7b, 0x64
		},
	},
#endif
#if 0
	{ 
		"http://www.atmel.com//Images/Atmel-42420-WINC1500-Software-Design-Guide_UserGuide.pdf", 1,
		{
			0, 
		}
	},
#endif
};



/***********************************************************/
static void http_client_test_init(void)
{
	m2m_memset((uint8*)&gstrHTTPSSession, 0, sizeof(tstrHTTPSession));
	gbIsHTTPSSessionRunning	= 0;
	gbIsTestFinished		= 0;
	gbHttpClientTestActive	= 1;
	gu32RecvCnt = 0;
	HTTP_ClientInit();
	gstrSslOptions.bIsSessionCachingEnabled = 1;
}

/***********************************************************/
static sint8 httpsUploadClientCallback(uint8 *pu8Chunk, sint16 s16ChunkSize, tstrHTTPUploadSession * pstrSession)
{
	sint8 ret = -1;
	
	if(pu8Chunk != NULL)
	{		
		uint32 len;
		len = ((uint32)s16ChunkSize);
		if(pstrSession->u32UploadSize > pstrSession->u32TxOffset) {
			uint32 rem, off;
			off = pstrSession->u32TxOffset%pstrSession->u32FileSize;
			rem = pstrSession->u32FileSize-off;
			if(len > rem) len = rem;
			memcpy(pu8Chunk, &pstrSession->pu8FileBuffer[off], len);
			pstrSession->u32TxOffset += len;
			printf("uploaded %d bytes\r", pstrSession->u32TxOffset);			
		}
		ret = 0;
	}
	else
	{
		printf("\r\n");
		if(s16ChunkSize == HTTP_CLIENT_FINISHED)
		{
			uint32 download_time = ms_ticks-pstrSession->u32UploadStartTime;
			printf("Upload Time:%lu, Speed:%lu KBps\r\n", download_time,
				(pstrSession->u32UploadSize*8)/download_time);
				
			HTTP_ClientStop(pstrSession->hHTTPClientHandle);
			gbIsHTTPSSessionRunning = 0;
			//delay_ms(500);
		}
		else if(s16ChunkSize == HTTP_CLIENT_CONNECTION_ERROR)
		{
			M2M_INFO("\"%s\" Fail!\n", pstrSession->acFileName);
			HTTP_ClientStop(pstrSession->hHTTPClientHandle);
			gbIsHTTPSSessionRunning = 0;
		}
		ret = 0;
	}
	return ret;
}
static sint8 httpsDownloadClientCallback(uint8 *pu8Chunk, sint16 s16ChunkSize, tstrHTTPDownloadSession * pstrSession)
{	
	if(pu8Chunk != NULL)
	{
		pstrSession->u32RxCount += s16ChunkSize;
		printf("received %d bytes\r", pstrSession->u32RxCount);
		if(pstrSession->bSkipDigestCheck == 0)	{
			SHA1_UPDATE(&sha1_ctxt, pu8Chunk, (uint32)s16ChunkSize);
		}
	}
	else
	{
		printf("\r\n");
		if(s16ChunkSize == HTTP_CLIENT_FINISHED)
		{			
			uint32 download_time = ms_ticks-pstrSession->u32DownloadStartTime;
			printf("Download Time:%lu, Speed:%lu KBps\r\n", download_time,
				(pstrSession->u32RxCount*8)/download_time);;
			if(pstrSession->bSkipDigestCheck) {
				printf("\r\n\"%s\" Success\r\n", pstrSession->acFileName);
			} else  {
				static uint8 digest[20];
			
				SHA1_FINISH(&sha1_ctxt, digest);		
			
				if(memcmp(digest, pstrSession->au8ExpectedDigest, sizeof(digest)) == 0) {
					printf("*** digest match ***\r\n");
					printf("\"%s\" Success\r\n\r\n", pstrSession->acFileName);
				} else {
					int i;
					printf("**** ERROR: DIGEST MISMATCH *****\r\n");
					printf("Received = ");
					for(i=0; i<sizeof(digest); i++) {
						printf("%02x ", digest[i]);
					}
					printf("\r\n");
				
					printf("Expected = ");
					for(i=0; i<sizeof(pstrSession->au8ExpectedDigest); i++) {
						printf("%02x ", pstrSession->au8ExpectedDigest[i]);
					}
					printf("\r\n");
					while(1);
				}
			}
			
			HTTP_ClientStop(pstrSession->hHTTPClientHandle);
			gbIsHTTPSSessionRunning = 0;
		}
		else if(s16ChunkSize == HTTP_CLIENT_CONNECTION_ERROR)
		{
			M2M_INFO("\"%s\" Fail!\n", pstrSession->acFileName);
			HTTP_ClientStop(pstrSession->hHTTPClientHandle);
			gbIsHTTPSSessionRunning = 0;
		}
	}
	return 0;
}

/***********************************************************/
static void http_test_task(void)
{
	static volatile uint32	u32ServerIdx = 0;
	
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
				
				if(gpacHTTPSFileList[u32ServerIdx].dir == HTTP_CLIENT_ACTION_DOWNLOAD)
				{
					tstrDownloadFileInfo * pstrDlFileInfo = &gpacHTTPSFileList[u32ServerIdx].u.dl;
					tstrHTTPDownloadSession * pstrHttpDlSession = &gstrHTTPSSession.u.dl;
					
					gstrHTTPSSession.dir = HTTP_CLIENT_ACTION_DOWNLOAD;
					
					pstrHttpDlSession->u32RxCount = 0;
					pstrHttpDlSession->u32DownloadStartTime = ms_ticks;
					strcpy(pstrHttpDlSession->acFileName, pstrDlFileInfo->gacFileURL);
					pstrHttpDlSession->bSkipDigestCheck = pstrDlFileInfo->bSkipDigestCheck;
					memcpy(pstrHttpDlSession->au8ExpectedDigest, pstrDlFileInfo->gau8ExpectedDigest, 20);
					pstrHttpDlSession->hHTTPClientHandle =
					HTTP_ClientConnect(pstrDlFileInfo->gacFileURL, HTTP_CLIENT_ACTION_DOWNLOAD, 0, httpsDownloadClientCallback, 
					(uint32)pstrHttpDlSession);
					if(pstrHttpDlSession->hHTTPClientHandle != NULL)
					{

					}
					u32ServerIdx ++;
					SHA1_INIT(&sha1_ctxt);
				} else {
					tstrUploadFileInfo * pstrUlFileInfo = &gpacHTTPSFileList[u32ServerIdx].u.ul;
					tstrHTTPUploadSession * pstrHttpUlSession = &gstrHTTPSSession.u.ul;					
					
					gstrHTTPSSession.dir = HTTP_CLIENT_ACTION_UPLOAD;
					
					pstrHttpUlSession->u32TxOffset = 0;
					pstrHttpUlSession->u32UploadStartTime = ms_ticks;
					pstrHttpUlSession->pu8FileBuffer = pstrUlFileInfo->pu8FileBuffer;
					pstrHttpUlSession->u32FileSize = pstrUlFileInfo->u32FileSize;
					pstrHttpUlSession->u32UploadSize = pstrUlFileInfo->u32UploadSize;
					strcpy(pstrHttpUlSession->acFileName, pstrUlFileInfo->gacFileURL);					
					pstrHttpUlSession->hHTTPClientHandle =
					HTTP_ClientConnect(pstrUlFileInfo->gacFileURL, HTTP_CLIENT_ACTION_UPLOAD, pstrUlFileInfo->u32UploadSize, httpsUploadClientCallback, 
					(uint32)pstrHttpUlSession);
					if(pstrHttpUlSession->hHTTPClientHandle != NULL)
					{

					}
					u32ServerIdx ++;
				}
			}
			else
			{
				M2M_INFO("Test Finished\n");
				gbIsTestFinished = 1;
				gbHttpClientTestActive = 0;
				u32ServerIdx = 0;
				http_client_test_init();
			}
		}
	}
}

/***********************************************************/
static void wifi_cb(uint8 u8MsgType, void * pvMsg)
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
			
		}
	}
	else if(u8MsgType == M2M_WIFI_REQ_DHCP_CONF)
	{
		gu8IsWiFiConnected = M2M_WIFI_CONNECTED;
		tstrM2MIPConfig* pstrM2MIpConfig = (tstrM2MIPConfig*) pvMsg;
		uint8 *pu8IPAddress = (uint8*) &pstrM2MIpConfig->u32StaticIP;
		
		M2M_INFO("DHCP IP Address :: %u.%u.%u.%u ::\n", pu8IPAddress[0], pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
		http_client_test_init();
	}
	else if(u8MsgType == M2M_WIFI_RESP_GET_SYS_TIME)
	{
		tstrSystemTime *pstrTime = (tstrSystemTime*)pvMsg;
		M2M_INFO("Time Of Day\n\t%d/%02d/%d %02d:%02d:%02d GMT\n",
		pstrTime->u8Month, pstrTime->u8Day, pstrTime->u16Year,
		pstrTime->u8Hour, pstrTime->u8Minute, pstrTime->u8Second);

	}	
	
	else if(u8MsgType == M2M_WIFI_RESP_SCAN_DONE)
	{
		tstrM2mScanDone* rslt = (tstrM2mScanDone*)pvMsg;
		printf( "number of scanned channels = %d\r\n", rslt->u8NumofCh );
		m2m_wifi_request_scan(M2M_WIFI_CH_ALL);
	}
}

/***********************************************************/
void demo_start(void)
{
	sint8				ret;
	tstrWifiInitParam	param;

	gu8IsWiFiConnected	= M2M_WIFI_DISCONNECTED;
	
	/* Initialize the BSP. */
	nm_bsp_init();
	
	m2m_memset((uint8*)&param, 0, sizeof(param));
	param.pfAppWifiCb = wifi_cb;	
	ret = m2m_wifi_init(&param);
	if (M2M_SUCCESS != ret)
	{
		M2M_ERR("Driver Init Failed <%d>\n",ret);
		for (;;)
		{
		}
	}

#ifdef _DYNAMIC_PS_
	{
		tstrM2mLsnInt strM2mLsnInt;
		M2M_INFO("M2M_PS_DEEP_AUTOMATIC\r\n");
		m2m_wifi_set_sleep_mode(M2M_PS_DEEP_AUTOMATIC, 1);
		strM2mLsnInt.u16LsnInt = M2M_LISTEN_INTERVAL;
		m2m_wifi_set_lsn_int(&strM2mLsnInt);
	}
#elif (defined _STATIC_PS_)
	M2M_INFO("M2M_PS_MANUAL\r\n");
	m2m_wifi_set_sleep_mode(M2M_PS_MANUAL, 1);
#else
	M2M_INFO("M2M_NO_PS\r\n");
	m2m_wifi_set_sleep_mode(M2M_NO_PS, 1);
#endif
	
	http_client_test_init();
	//m2m_wifi_request_scan_passive(M2M_WIFI_CH_ALL,1000);
	m2m_wifi_connect((char*)MAIN_WLAN_DEVICE_NAME, sizeof(MAIN_WLAN_DEVICE_NAME), M2M_WIFI_SEC_WPA_PSK, MAIN_WLAN_PSK, M2M_WIFI_CH_ALL);
	//m2m_wifi_default_connect();

	for (;;)
	{
		while(m2m_wifi_handle_events(NULL) != M2M_SUCCESS)
		{
		}
		http_test_task();
	}
}


