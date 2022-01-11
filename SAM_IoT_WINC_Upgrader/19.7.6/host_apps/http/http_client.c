/*!
@file			
	http_client.c

@brief

@author		
	Ahmed Ezzat
*/

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "http.h"
#include "http_utils.h"
#include "socket/include/socket.h"
#include "driver/include/m2m_wifi.h"



/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


#define HTTP_GET_REQ	\
	"GET %s HTTP/1.1\r\n"\
	"Host: %s\r\n"\
	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"\
	"User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.116 Safari/537.36\r\n"\
	"\r\n"

#define HTTP_POST_REQ	\
	"PUT %s HTTP/1.1\r\n"\
	"Host: %s\r\n"\
	"Content-Type: application/pdf\r\n"\
	"Content-Length: %lu\r\n"\
	"\r\n"


#define HTTP_CLIENT_MAX_SESSIONS						(1)
/*!<
*/


#define HTTP_CLIENT_CONN_RETRY_MAX						(0)
/*!<
*/
	
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
GLOBALS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

tstrHTTPClient			gastrHTTPClientList[HTTP_CLIENT_MAX_SESSIONS];
tstrSslGlobalOptions	gstrSslOptions;

/*********************************************************************
HttpClientParseHeader
*/
static uint32 HttpClientParseHeader(char *pcHeader, uint16 u16MsgSize, uint32 *pu32ContentLength)
{
	uint16	u16Offset			= 0;
	uint32	u32StatusCode;
	char	*pcStatusLine, *pcLine;
	char	*pcTmp;
	uint16	u16Tmp;

	/* Get Status Line.
	*/
	HttpstrTOK(pcHeader, (char*)"");
	pcStatusLine = HttpstrTOK(NULL, (char*)"\r");
	u16Tmp = (uint16)strlen(pcStatusLine);
	pcStatusLine[u16Tmp] = '\r'; 

	u16Offset = u16Tmp + 2;

	/* Parse Status Line.
	*/
	HttpstrTOK(pcStatusLine, (char*)"");
	pcTmp = HttpstrTOK(NULL, (char*)" ");
	pcTmp = HttpstrTOK(NULL, (char*)" ");
	u32StatusCode = Ascii2Dec(pcTmp);
	if(u32StatusCode == 200)
	{
		/* Parse Response Headers.
		*/
		pcTmp = pcHeader + u16Offset;
		HttpstrTOK(pcTmp, (char*)"");

		while(u16Offset < u16MsgSize)
		{
			pcLine = HttpstrTOK(NULL, (char*)"\r");
			if(pcLine != NULL)
			{
				u16Tmp = (uint16)strlen(pcLine);
				pcTmp = pcLine;
				while((*pcTmp == ' ') || (*pcTmp == '\n') || (*pcTmp == '\r'))
					pcTmp ++;

				if(!HttpstrCmpIgnoreCase(pcTmp, (char*)"Content-Length", (uint32)strlen("Content-Length")))
				{
					pcTmp += strlen("Content-Length");
					*pu32ContentLength = Ascii2Dec(pcTmp);
				}

				pcLine[u16Tmp] = '\r';
				if(!memcmp(&pcLine[u16Tmp], "\r\n\r\n", 4))
				{
					u16Offset = (uint16)(pcLine - pcHeader + u16Tmp + 4);
					break;
				}
				u16Offset = (uint16)(u16Tmp + (pcLine - pcHeader) + 1);
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		M2M_INFO("HTTP Status Error <%lu>\n", u32StatusCode);
	}
	return u32StatusCode;
}
/*********************************************************************
HttpClientRecvCb
*/
static void httpClientRecvCallback
	(
	tstrHTTPClient	*pstrHTTPClient,
	uint8 			*pu8RxBuffer,
	sint16 			s16DataSize
	)
{
	if((pstrHTTPClient != NULL) && (pu8RxBuffer != NULL) && (s16DataSize > 0))
	{
		uint8				*pu8HttpResp	= pu8RxBuffer;
		uint32				u32DataSize		= (uint32)s16DataSize;
		uint32				bIsRxDone		= 0;
		tstrRxMsgStatus		*pstrRxMsg		= &pstrHTTPClient->strRxMsg;
		tstrHTTPRespHeader	*pstrHttpHdr	= &pstrRxMsg->strHttpHdr;
		
		/* If the header is not completed, search for the header trailer "\r\n\r\n".
		*/
		if(!pstrHttpHdr->bIsComplete)
		{
			sint16	s16Idx;
			uint16	u16HdrLength	= (uint16)s16DataSize;
			char	*pcHeader		= (char*)pu8RxBuffer;

			/* Searching.
			*/
			pu8RxBuffer[u32DataSize] = 0;
			for(s16Idx = 0; s16Idx < s16DataSize; )
			{
				if(pcHeader[s16Idx ++] == '\r')
					if(pcHeader[s16Idx ++] == '\n')
						if(pcHeader[s16Idx ++] == '\r')
							if(pcHeader[s16Idx ++] == '\n')
							{
								pstrHttpHdr->bIsComplete = 1;
								pu8HttpResp = &pu8RxBuffer[s16Idx];
								u16HdrLength = (uint16)s16Idx;
								break;
							}
			}

			/* Append to the saved header segment.
			*/
			if(pstrHttpHdr->u16HdrSize == 0)
			{
				memcpy(pstrHttpHdr->pu8Hdr, pu8RxBuffer, u16HdrLength);
				pstrHttpHdr->u16HdrSize += u16HdrLength;
			}
			else
			{
				memcpy(&pstrHttpHdr->pu8Hdr[pstrHttpHdr->u16HdrSize], pu8RxBuffer, u16HdrLength);
				pstrHttpHdr->u16HdrSize += u16HdrLength;
			}

			if(pstrHttpHdr->bIsComplete)
			{
				uint32	u32HttpRspStatus;

				M2M_DBG("Socket <%d> Start Downloading\r\n",pstrHTTPClient->sock);

				/* Parse header and jump to the start of data.
				*/
				pstrRxMsg->u32ContentLength = 0;
				pstrRxMsg->u32RxCount		= 0;
				u32HttpRspStatus = HttpClientParseHeader((char*)pstrHttpHdr->pu8Hdr, pstrHttpHdr->u16HdrSize, 
					&pstrRxMsg->u32ContentLength);

				if(u32HttpRspStatus == 200)
				{
					if(pstrRxMsg->u32ContentLength != 0)
					{
						pstrRxMsg->bIsChunked = 0;
					}
					else
					{
						pstrRxMsg->bIsChunked			= 1;
						pstrRxMsg->bIsWaitingChunkHdr	= 1;
						pstrRxMsg->u32RxCount			= 0;
						pstrRxMsg->u32ContentLength		= 0;
					}

					if(pu8HttpResp != NULL)
						u32DataSize = s16DataSize - (sint16)(pu8HttpResp - pu8RxBuffer);				
				}
				else
				{
					M2M_INFO("HTTP Download Error %lu\n", u32HttpRspStatus);
					if(pstrHTTPClient->fpCb != NULL)
					{
						pstrHTTPClient->fpCb(NULL, HTTP_CLIENT_CONNECTION_ERROR, pstrHTTPClient->u32CbArg);
					}					
				}
			}
		}

		if((pu8HttpResp != NULL) && (u32DataSize != 0) && (pstrHttpHdr->bIsComplete))
		{
			uint32	u32Rem = 0;
			pu8HttpResp[u32DataSize] = 0;

			if(!pstrRxMsg->bIsChunked)
			{
				u32Rem = pstrRxMsg->u32ContentLength - pstrRxMsg->u32RxCount;
				pstrRxMsg->u32RxCount += u32DataSize;
				M2M_DBG("SOCK <%d><%d> RX<%lu/%lu>\n", pstrHTTPClient->sock, u32DataSize, 
					pstrRxMsg->u32RxCount, pstrRxMsg->u32ContentLength);

				if(u32DataSize >= u32Rem)
				{
					bIsRxDone = 1;
					u32DataSize = u32Rem;
					pstrRxMsg->u32ContentLength 	= 0;
					pstrRxMsg->u32RxCount			= 0;
				}

				/* Deliver the received Chunk to the caller.
				*/
				if(pstrHTTPClient->fpCb)
				{
					pstrHTTPClient->fpCb(pu8HttpResp, (sint16)u32DataSize, pstrHTTPClient->u32CbArg);
				}
				else
				{
					HTTP_DBG("NULL CB\n");
				}
			}
			else
			{
				for(;;)
				{
					uint8	*pu8ChunkLen = pstrRxMsg->au8ChunkHdr;					
					if(pstrRxMsg->bIsWaitingChunkHdr)
					{
						pu8ChunkLen = &pstrRxMsg->au8ChunkHdr[pstrRxMsg->u32RxCount];
						while(((*pu8HttpResp == ' ') || (*pu8HttpResp == '\r') || (*pu8HttpResp == '\n')) && (u32DataSize))
						{
							pu8HttpResp ++;
							u32DataSize --;
						}

						while((*pu8HttpResp != '\r') && (u32DataSize))
						{
							*pu8ChunkLen = *pu8HttpResp;
							pu8HttpResp ++;
							pu8ChunkLen ++;
							u32DataSize --;
						}

						if(*pu8HttpResp == '\r')
						{
							/*
								CHUNK Header Complete
							*/
							*pu8ChunkLen					= 0;
							pstrRxMsg->u32RxCount			= 0;
							pstrRxMsg->u32ContentLength		= strtol((char*)pstrRxMsg->au8ChunkHdr, NULL, 16);

							while(((*pu8HttpResp == '\r') || (*pu8HttpResp == '\n')) && (u32DataSize))
							{
								pu8HttpResp ++;
								u32DataSize --;
							}

							if(pstrRxMsg->u32ContentLength == 0)
							{
								bIsRxDone = 1;
								break;
							}

							pstrRxMsg->bIsWaitingChunkHdr	= 0;
							if(u32DataSize == 0)
							{
								goto __RECV__;
							}
						}
						else
						{
							pstrRxMsg->u32RxCount			= (uint32)(pu8ChunkLen - pstrRxMsg->au8ChunkHdr);
							goto __RECV__;
						}
					}
					else
					{
						u32Rem = pstrRxMsg->u32ContentLength - pstrRxMsg->u32RxCount;
						if(u32DataSize >= u32Rem)
						{
							/* Deliver the received Chunk to the caller.
							*/
							if(pstrHTTPClient->fpCb)
							{
								pstrHTTPClient->fpCb(pu8HttpResp, (sint16)u32Rem, pstrHTTPClient->u32CbArg);
							}
							pu8HttpResp += u32Rem;
							u32DataSize	-= u32Rem;
							pstrRxMsg->u32RxCount			= 0;
							pstrRxMsg->u32ContentLength		= 0;
							pstrRxMsg->bIsWaitingChunkHdr	= 1;
							while(((*pu8HttpResp == '\r') || (*pu8HttpResp == '\n') || (*pu8HttpResp == ' ') || (*pu8HttpResp == '\0')) && (u32DataSize))
							{
								pu8HttpResp ++;
								u32DataSize --;
							}
							if(u32DataSize == 0)
							{
								goto __RECV__;
							}
						}
						else
						{
							/* Deliver the received Chunk to the caller.
							*/
							if(pstrHTTPClient->fpCb)
							{
								pstrHTTPClient->fpCb(pu8HttpResp, (sint16)u32DataSize, pstrHTTPClient->u32CbArg);
							}
							pstrRxMsg->u32RxCount += u32DataSize;
							goto __RECV__;
						}
					}
				}
			}

			if(bIsRxDone)
			{
				/* Notify the user.
				*/
				if(pstrHTTPClient->fpCb != NULL)
				{
					pstrHTTPClient->fpCb(NULL, HTTP_CLIENT_FINISHED, pstrHTTPClient->u32CbArg);
				}
			}
		}

__RECV__:
		if(!bIsRxDone)
		{
			memset(pstrHTTPClient->au8RxBuffer, 0, sizeof(pstrHTTPClient->au8RxBuffer));
			recv(pstrHTTPClient->sock, pstrHTTPClient->au8RxBuffer, sizeof(pstrHTTPClient->au8RxBuffer), 0);
		}
	}
	else
	{
		if(pstrHTTPClient->fpCb != NULL)
			pstrHTTPClient->fpCb(NULL, HTTP_CLIENT_CONNECTION_ERROR, pstrHTTPClient->u32CbArg);		
	}
}
/*********************************************************************
*/
static void httpClientConnectCb
(
tstrHTTPClient	*pstrHTTPClient,
sint8			err
)
{
	if(err == SOCK_ERR_NO_ERROR)
	{
		uint32	u32HttpReqSize;
		char	acHttpREQ[512];
		
		if(pstrHTTPClient->fpCb != NULL)
			pstrHTTPClient->fpCb(NULL, HTTP_CLIENT_CONNECTED, pstrHTTPClient->u32CbArg);

		if(pstrHTTPClient->enuAction == HTTP_CLIENT_ACTION_DOWNLOAD)
		{
			u32HttpReqSize = sprintf(acHttpREQ, HTTP_GET_REQ, pstrHTTPClient->acFileName, pstrHTTPClient->acHostName);
		}
		else
		{
			u32HttpReqSize = sprintf(acHttpREQ, HTTP_POST_REQ, pstrHTTPClient->acFileName, pstrHTTPClient->acHostName, pstrHTTPClient->u32UploadSize);
		}
		send(pstrHTTPClient->sock, (uint8*)acHttpREQ, u32HttpReqSize, 0);
		recv(pstrHTTPClient->sock, pstrHTTPClient->au8RxBuffer, sizeof(pstrHTTPClient->au8RxBuffer), 0);
	}
	else
	{
		if(pstrHTTPClient->u8ConnRetryCount == HTTP_CLIENT_CONN_RETRY_MAX)
		{
			pstrHTTPClient->u8ConnRetryCount = 0;
			if(pstrHTTPClient->fpCb != NULL)
				pstrHTTPClient->fpCb(NULL, HTTP_CLIENT_CONNECTION_ERROR, pstrHTTPClient->u32CbArg);
		}
		else
		{
			struct sockaddr_in	strHostAddr;
			strHostAddr.sin_family		= AF_INET;
			strHostAddr.sin_port		= _htons(pstrHTTPClient->u16RemotePort);
			strHostAddr.sin_addr.s_addr	= pstrHTTPClient->u32RemoteIP;
			M2M_INFO("Sock <%d> Connection Retry\n",pstrHTTPClient->sock);
			pstrHTTPClient->u8ConnRetryCount ++;
			connect(pstrHTTPClient->sock, (struct sockaddr*)&strHostAddr, sizeof(struct sockaddr_in));
		}
	}
}
/*********************************************************************
*/
static void httpClientDnsCb
	(
	tstrHTTPClient	*pstrHTTPClient,
	char			*pcName,
	uint32			u32IPAddr
	)
{
	if((pcName != NULL) && (u32IPAddr != 0))
	{
		struct sockaddr_in	strHostAddr;

		pstrHTTPClient->u32RemoteIP	= u32IPAddr;

		strHostAddr.sin_family		= AF_INET;
		strHostAddr.sin_port		= _htons(pstrHTTPClient->u16RemotePort);
		strHostAddr.sin_addr.s_addr	= pstrHTTPClient->u32RemoteIP;

		HTTP_DBG("IP for \"%s\" is \"%u.%u.%u.%u\"\n",pcName,
			BYTE_0(u32IPAddr), BYTE_1(u32IPAddr), BYTE_2(u32IPAddr), BYTE_3(u32IPAddr));

		connect(pstrHTTPClient->sock, (struct sockaddr*)&strHostAddr, sizeof(struct sockaddr_in));
	}
	else
	{
		M2M_ERR("DNS Resolution Failed<%s>\n",pcName);
		if(pstrHTTPClient->u8ConnRetryCount == HTTP_CLIENT_CONN_RETRY_MAX)
		{
			pstrHTTPClient->u8ConnRetryCount = 0;
			if(pstrHTTPClient->fpCb != NULL)
				pstrHTTPClient->fpCb(NULL, HTTP_CLIENT_CONNECTION_ERROR, pstrHTTPClient->u32CbArg);
		}
		else
		{
			pstrHTTPClient->u8ConnRetryCount ++;
			gethostbyname((uint8*)pcName);
		}
	}
}
/*********************************************************************
httpClientSendCb
*/
static void httpClientSendCb
(
tstrHTTPClient	*pstrHTTPClient
)
{
	if(pstrHTTPClient->enuAction != HTTP_CLIENT_ACTION_UPLOAD)
		return;

	if(pstrHTTPClient->fpCb != NULL)
	{
		uint16	u16TxSize;
		do 
		{
			if(pstrHTTPClient->bIsTxPending)
			{
				u16TxSize = pstrHTTPClient->u32TxSize;
			}
			else
			{
				u16TxSize = sizeof(pstrHTTPClient->au8RxBuffer);
				if(u16TxSize > pstrHTTPClient->u32UploadSize)
					u16TxSize = pstrHTTPClient->u32UploadSize;
					
				pstrHTTPClient->fpCb(pstrHTTPClient->au8RxBuffer, u16TxSize, pstrHTTPClient->u32CbArg);
			}
			
			if(send(pstrHTTPClient->sock, pstrHTTPClient->au8RxBuffer, u16TxSize, 0) != 0)
			{
				pstrHTTPClient->bIsTxPending	= 1;
				pstrHTTPClient->u32TxSize		= u16TxSize;
				break;
			}
			else
			{
				pstrHTTPClient->u32UploadSize -= u16TxSize;
				if(pstrHTTPClient->u32UploadSize == 0)
				{
					pstrHTTPClient->fpCb(NULL, HTTP_CLIENT_FINISHED, pstrHTTPClient->u32CbArg);
					break;
				}
			}
		}while(1);
	}
}
/*********************************************************************
HttpClientParseURL
*/
static sint8 HttpClientParseURL(tstrHTTPClient *pstrHTTPClient, char *pcURL)
{
	char		*pcTmp;
	uint16		u16URLSize = (uint16)strlen(pcURL);
	sint8		s8Error = 0;
	HttpstrTOK(pcURL, (char*)"");
	pcTmp = HttpstrTOK(NULL, (char*)":");

	if(!HttpstrCmpIgnoreCase(pcTmp, (char*)"https", 5))
	{
		pstrHTTPClient->u16RemotePort	= HTTPS_PORT;
		pstrHTTPClient->bIsHTTPS		= 1;
	}
	else if(!HttpstrCmpIgnoreCase(pcTmp, (char*)"http", (uint32)strlen(pcTmp)))
	{
		pstrHTTPClient->u16RemotePort	= HTTP_PORT;
		pstrHTTPClient->bIsHTTPS		= 0;
	}
	else
	{
		s8Error = -1;
	}

	if(s8Error == 0)
	{
		char	*pcHostName;

		pcTmp += strlen(pcTmp) + 1;
		while((*pcTmp == '/') || (*pcTmp == ' ') || (*pcTmp == ':'))
			pcTmp ++;

		if(pcTmp < (pcURL + u16URLSize))
		{
			char	*pcPort;

			HttpstrTOK(pcTmp, (char*)"");
			pcHostName = HttpstrTOK(NULL, (char*)"/");
			strcpy(pstrHTTPClient->acFileName, (char*)"/");

			pcTmp += strlen(pcHostName) + 1;
			if(pcTmp < (pcURL + u16URLSize))
			{
				pcTmp = HttpstrTOK(NULL, (char*)")");
				strcpy(&pstrHTTPClient->acFileName[1], pcTmp);
			}

			/* Check if the port number is appended in the URL.
			*/
			HttpstrTOK(pcHostName, (char*)"");
			pcPort = HttpstrTOK(NULL, (char*)":");
			strcpy(pstrHTTPClient->acHostName, pcPort);
			pcPort = HttpstrTOK(NULL, (char*)"/");
			if(pcPort != NULL)
			{
				if(*pcPort != '\0')
				{
					uint16	u16Port = (uint16)Ascii2Dec(pcPort); 
					pstrHTTPClient->u16RemotePort = u16Port;
				}
			}
		}
		else
		{
			s8Error = -1;
		}
	}
	return s8Error;
}
/*********************************************************************
*/
static uint32 inet_addr(char *pcIpAddr)
{
	uint8	tmp;
	uint32	u32IP = 0;
	uint8	au8IP[4];
	uint8 	c;
	uint8	i, j;

	tmp = 0;

	for(i = 0; i < 4; ++i) 
	{
		j = 0;
		do 
		{
			c = *pcIpAddr;
			++j;
			if(j > 4) 
			{
				return 0;
			}
			if(c == '.' || c == 0) 
			{
				au8IP[i] = tmp;
				tmp = 0;
			} 
			else if(c >= '0' && c <= '9') 
			{
				tmp = (tmp * 10) + (c - '0');
			} 
			else 
			{
				if(i == 3)
				{
					au8IP[i] = tmp;
					break;
				}
				return 0;
			}
			++pcIpAddr;
		} while(c != '.' && c != 0);
	}
	memcpy((uint8*)&u32IP, au8IP, 4);
	return u32IP;
}
/*********************************************************************
*/
tstrHTTPClient* HTTP_ClientConnect
(
char								*pcTargetURL,
tenuHttpClientAction				enuAction,
uint32								u32UploadSize,
tpfHttpClientNotificationCallback	fpCb,
uint32								u32UsrData
)
{
	tstrHTTPClient	*pstrHTTPClient = NULL;

	if(pcTargetURL != NULL)
	{
		uint8			u8Idx;

		for(u8Idx = 0; u8Idx < HTTP_CLIENT_MAX_SESSIONS; u8Idx ++)
		{
			if(!gastrHTTPClientList[u8Idx].bIsRunning)
			{
				pstrHTTPClient = &gastrHTTPClientList[u8Idx];
				break;
			}
		}

		if(pstrHTTPClient != NULL)
		{
			char	acTmpURL[256];

			/* Initialization.
			*/
			m2m_memset((uint8*)pstrHTTPClient, 0, sizeof(tstrHTTPClient));
			pstrHTTPClient->bIsRunning 		= 1;
			pstrHTTPClient->fpCb 			= fpCb;
			pstrHTTPClient->u32CbArg		= u32UsrData;
			pstrHTTPClient->sock			= -1;
			pstrHTTPClient->enuAction		= enuAction;
			pstrHTTPClient->u32UploadSize	= u32UploadSize;
			
			m2m_memcpy((uint8*)acTmpURL, (uint8*)pcTargetURL, 256);

			/* URL Analysis.
			*/
			if(HttpClientParseURL(pstrHTTPClient, acTmpURL) == 0)
			{
				pstrHTTPClient->sock = socket(AF_INET, SOCK_STREAM, pstrHTTPClient->bIsHTTPS);
				if(pstrHTTPClient->sock >= 0)
				{
					if(pstrHTTPClient->bIsHTTPS)
					{
						int sslOptionEnable = 1;
						
						if(gstrSslOptions.bIsSNIEnabled)
						{
							setsockopt(pstrHTTPClient->sock, SOL_SSL_SOCKET, SO_SSL_SNI, pstrHTTPClient->acHostName, strlen(pstrHTTPClient->acHostName) + 1);
							setsockopt(pstrHTTPClient->sock, SOL_SSL_SOCKET, SO_SSL_ENABLE_SNI_VALIDATION, &sslOptionEnable, 4);
						}
						
						if(gstrSslOptions.bIsSessionCachingEnabled)
						{
							setsockopt(pstrHTTPClient->sock, SOL_SSL_SOCKET, SO_SSL_ENABLE_SESSION_CACHING, &sslOptionEnable, 4);
						}
						
						if(gstrSslOptions.bIsBypassModeEnabled)
						{
							setsockopt(pstrHTTPClient->sock, SOL_SSL_SOCKET, SO_SSL_BYPASS_X509_VERIF, &sslOptionEnable, 4);
						}
					}

					if((pstrHTTPClient->acHostName[0] >= '1') && (pstrHTTPClient->acHostName[0] <= '9'))
					{
						/* URL already has the IP Address.
						*/
						struct sockaddr_in	strHostAddr;

						pstrHTTPClient->u32RemoteIP = inet_addr(pstrHTTPClient->acHostName);

						strHostAddr.sin_family		= AF_INET;
						strHostAddr.sin_port		= _htons(pstrHTTPClient->u16RemotePort);
						strHostAddr.sin_addr.s_addr	= pstrHTTPClient->u32RemoteIP;
						HTTP_DBG("Socket <%d> Connecting......\n",pstrHTTPClient->sock);
						connect(pstrHTTPClient->sock, (struct sockaddr*)&strHostAddr, sizeof(struct sockaddr_in));
					}
					else
					{
						sint8	s8DnsRsp;
						
						if(pstrHTTPClient->bIsHTTPS)
						{
							/* 
								Set the SNI socket option.
							*/
							setsockopt(pstrHTTPClient->sock, SOL_SSL_SOCKET, SO_SSL_SNI, pstrHTTPClient->acHostName,
								m2m_strlen((uint8*)pstrHTTPClient->acHostName));
						}
						
						s8DnsRsp = gethostbyname((uint8*)pstrHTTPClient->acHostName);
						HTTP_DBG("Resolving <%s>\n",pstrHTTPClient->acHostName);
						if(s8DnsRsp)
						{
							M2M_INFO("gethostbyname = %d\n", s8DnsRsp);
							pstrHTTPClient->bIsRunning = 0;
							HTTP_ClientStop(pstrHTTPClient);
							pstrHTTPClient = NULL;							
						}
					}
				}
			}
			else
			{
				M2M_ERR("URL Parsing Error\n");
				pstrHTTPClient->bIsRunning = 0;
				HTTP_ClientStop(pstrHTTPClient);
				pstrHTTPClient = NULL;
			}
		}
		else
		{
			M2M_ERR("HTTP Client MAX\n");
		}
	}
	return pstrHTTPClient;
}
/*********************************************************************
HTTP_ClientStop
*/
void HTTP_ClientStop(tstrHTTPClient *pstrHTTPClient)
{
	if(pstrHTTPClient != NULL)
	{
		if(pstrHTTPClient->bIsRunning)
		{
			if(pstrHTTPClient->sock >= 0)
				close(pstrHTTPClient->sock);

			memset(pstrHTTPClient, 0, sizeof(tstrHTTPClient));		
			pstrHTTPClient->sock		= -1;
		}
	}
}
/*********************************************************************
httpClientSocketCallback
*/
static void httpClientSocketCallback(SOCKET sock, uint8 u8Msg, void *pvMsg)
{
	switch(u8Msg)
	{
	case SOCKET_MSG_CONNECT:
		{
			tstrSocketConnectMsg	*pstrConnect = (tstrSocketConnectMsg *)pvMsg;
			uint8					u8Idx;
			tstrHTTPClient			*pstrHTTPClient;

			for(u8Idx = 0; u8Idx < HTTP_CLIENT_MAX_SESSIONS; u8Idx ++)
			{
				pstrHTTPClient = &gastrHTTPClientList[u8Idx];
				if(pstrHTTPClient->sock == sock)
				{
					httpClientConnectCb(pstrHTTPClient, pstrConnect->s8Error);
					break;
				}
			}
		}
		break;

	case SOCKET_MSG_RECV:
		{
			tstrSocketRecvMsg	*pstrRecv = (tstrSocketRecvMsg *)pvMsg;
			uint8				u8Idx;
			tstrHTTPClient		*pstrHTTPClient;

			for(u8Idx = 0; u8Idx < HTTP_CLIENT_MAX_SESSIONS; u8Idx ++)
			{
				pstrHTTPClient = &gastrHTTPClientList[u8Idx];
				if(pstrHTTPClient->sock == sock)
				{
					httpClientRecvCallback(pstrHTTPClient, pstrRecv->pu8Buffer, pstrRecv->s16BufferSize);
					break;
				}
			}
		}
		break;
	case SOCKET_MSG_SEND:
		{
			uint8				u8Idx;
			tstrHTTPClient		*pstrHTTPClient;
			for(u8Idx = 0; u8Idx < HTTP_CLIENT_MAX_SESSIONS; u8Idx ++)
			{
				pstrHTTPClient = &gastrHTTPClientList[u8Idx];
				if(pstrHTTPClient->sock == sock)
				{
					httpClientSendCb(pstrHTTPClient);
					break;
				}
			}
		}
		break;
	}
}
/*********************************************************************
httpClientDnsCallback
*/
static void httpClientDnsCallback(uint8* pu8DomainName, uint32 u32ServerIP)
{
	uint8				u8Idx;
	tstrHTTPClient		*pstrHTTPClient;

	for(u8Idx = 0; u8Idx < HTTP_CLIENT_MAX_SESSIONS; u8Idx ++)
	{
		pstrHTTPClient = &gastrHTTPClientList[u8Idx];
		if(!strcmp((char*)pu8DomainName, pstrHTTPClient->acHostName))
		{
			httpClientDnsCb(pstrHTTPClient, (char*)pu8DomainName, u32ServerIP);
			break;
		}
	}
}
/*********************************************************************
HTTP_ClientInit
*/
void HTTP_ClientInit(void)
{
	socketInit();
	registerSocketCallback(httpClientSocketCallback, httpClientDnsCallback);

	m2m_memset((uint8*)gastrHTTPClientList, 0, HTTP_CLIENT_MAX_SESSIONS * sizeof(tstrHTTPClient));
	m2m_memset((uint8*)&gstrSslOptions, 0, sizeof(tstrSslGlobalOptions));
}