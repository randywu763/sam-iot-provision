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
#include "socket/include/socket.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define HTTP_CLIENT_CONNECTED								(0)
#define HTTP_CLIENT_FINISHED								(1)
#define HTTP_CLIENT_CONNECTION_ERROR						(-100)

#define HTTP_PORT									(80)
#define HTTPS_PORT									(443)
#define HTTP_CLIENT_FILE_NAME_MAX					(256)
#define HTTP_CLIENT_HOST_NAME_MAX					(128)
#define HTTP_CLIENT_RX_BUFFER_MAX					(1024)


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

typedef enum{
	HTTP_REQ_GET,
	HTTP_REQ_POST,
	HTTP_REQ_UNKNOWN
}tenuHTTPReqMethod;


typedef enum{
	HTTP_STATUS_OK						= 200,
	/*!<
	Request processing is OK.
	*/
	HTTP_STATUS_REDIRECT					= 303,
	/*!<
	The requested URL does not exist and a new URL should be used instead.
	*/
	HTTP_STATUS_BAD_REQ					= 400,
	/*!<
	The request format is wrong.
	*/
	HTTP_STATUS_NOT_FOUND				= 404,
	/*!<
	The requested file is not found on the server.
	*/
	HTTP_STATUS_METHOD_NOT_ALLOWED	= 405,
	/*!<
	The HTTP Request method is not supported by the server.
	*/
	HTTP_STATUS_SERVER_ERROR			= 500
	/*!<
	Something goes wrong at the server side.
	*/
}tenuHTTPStatusCode;


typedef enum{
	HTTP_ACTION_UPLOAD_FILE,
	HTTP_ACTION_DOWNLOAD_FILE,
	HTTP_ACTION_SEND_ERROR
}tenuHTTPAction;


/*!
@struct	\
tstrHttpReqInfo
*/
typedef struct{
	tenuHTTPAction		enuAction;
	tenuHTTPStatusCode	enuRspErrCode;
	uint32				u32PostFileSize;
	char				acFileName[256];
	char				*pcMsgBody;
	uint32				u32MsgBodyLen;
}tstrHttpReqInfo;

typedef struct{
	tenuHTTPReqMethod	enuMethod;
	/*!<
	HTTP Request method (GET, POST,...).
	*/
	char				*pcURL;
	/*!<
	The URL from the HTTP Request line.
	*/
	char				*pcHost;
	/*!<
	The host domain name retrieved from the HTTP Header Line "Host: ....".
	*/
	char				*pcMsgBody;
	/*!<
	Pointer to the start of the HTTP request entity body (if there is a body for the request).
	*/
	uint32				u32ContentLength;
	/*!<
	HTTP Request message body length.
	*/
}tstrHTTPReqHeader;



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


typedef struct{
	SOCKET					sslSock;
	uint32					u32DownloadSize;
	uint32					u32RxSize;
	uint8					bIsDownloadInProgress;
	uint8					au8ContentType[64];
}tstrHttpsConn;
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
	char								acHostName[HTTP_CLIENT_HOST_NAME_MAX];
	char								acFileName[HTTP_CLIENT_FILE_NAME_MAX];
	tstrRxMsgStatus						strRxMsg;
	tpfHttpClientNotificationCallback	fpCb;
	uint32								u32CbArg;
}tstrHTTPClient;

typedef struct{
	uint8	bIsBypassModeEnabled;
	uint8	bIsSessionCachingEnabled;
	uint8	bIsSNIEnabled;
}tstrSslGlobalOptions;

extern tstrSslGlobalOptions	gstrSslOptions;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void HTTP_ClientInit(void);

tstrHTTPClient* HTTP_ClientConnect(char *pcTargetURL, tenuHttpClientAction enuAction, uint32 u32UploadSize, 
		tpfHttpClientNotificationCallback fpCb, uint32 u32UsrData);

void HTTP_ClientStop(tstrHTTPClient* pstrHTTPClient);

void httpServerStart(void);

#endif /* __M2M_OTA_HTTP_CLIENT_H__ */
