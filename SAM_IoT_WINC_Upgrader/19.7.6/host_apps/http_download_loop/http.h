/*!
@file       		
	http.h

@brief

			
@author   		
	Ahmed Ezzat

@date      		
	18 Sep 2014
*/
#ifndef __HTTP_H__
#define __HTTP_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "driver/include/m2m_types.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define HTTP_CLIENT_FINISHED								(0)
#define HTTP_CLIENT_CONNECTION_ERROR						(-100)

#define HTTP_PORT									(80)
#define HTTPS_PORT									(443)
#define HTTP_CLIENT_FILE_NAME_MAX					(256)
#define HTTP_CLIENT_HOST_NAME_MAX					(128)
#define HTTP_CLIENT_RX_BUFFER_MAX					(1024)
#define HTTP_CLIENT_TX_BUFFER_MAX					(1024)

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


typedef enum{
	HTTP_CLIENT_ACTION_DOWNLOAD,
	HTTP_CLIENT_ACTION_UPLOAD
}tenuHttpClientAction;


typedef struct{
	uint8		pu8Hdr[1024];
	uint16		u16HdrSize;
	uint32		bIsComplete;
}tstrHTTPRespHeader;

typedef struct{
	uint8				bIsChunked;
	uint8				bIsWaitingChunkHdr;
	uint8				au8ChunkHdr[14];
	uint32				u32ContentLength;
	uint32				u32RxCount;
	tstrHTTPRespHeader	strHttpHdr;
}tstrRxMsgStatus;

typedef sint8 (*tpfHttpClientNotificationCallback)(uint8 *pu8Chunk, sint16 s16ChunkSize, uint32 u32UsrData);

/*!
@
*/
typedef struct{
	tenuHttpClientAction				enuAction;
	uint32								u32UploadSize;
	uint32								u32TxSize;
	uint32								u32RemoteIP;
	uint16								u16RemotePort;
	sint8								sock;
	uint8								bIsRunning;
	uint8								u8ConnRetryCount;								
	uint8								bIsHTTPS;
	uint8								bIsTxPending;
	uint8								au8RxBuffer[HTTP_CLIENT_RX_BUFFER_MAX];
	uint8								au8TxBuffer[HTTP_CLIENT_RX_BUFFER_MAX];
	char								acHostName[HTTP_CLIENT_HOST_NAME_MAX];
	char								acFileName[HTTP_CLIENT_FILE_NAME_MAX];
	tstrRxMsgStatus						strRxMsg;
	tpfHttpClientNotificationCallback	fpCb;
	uint32								u32CbArg;
}tstrHTTPClient;

typedef struct{
	uint8	bIsBypassModeEnabled;
	uint8	bIsSessionCachingEnabled;
}tstrSslGlobalOptions;

extern tstrSslGlobalOptions	gstrSslOptions;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void HTTP_ClientInit(void);

tstrHTTPClient* HTTP_ClientConnect(char *pcTargetURL, tenuHttpClientAction enuAction, uint32 u32UploadSize, 
		tpfHttpClientNotificationCallback fpCb, uint32 u32UsrData);

void HTTP_ClientStop(tstrHTTPClient* pstrHTTPClient);


#endif /* __M2M_OTA_HTTP_CLIENT_H__ */
