#include "http.h"
#include <string.h>
#include "http_utils.h"

char	gpacHttpErr[][64] = {
	"400 Bad Request",
	"404 Not Found",
	"405 Method Not Allowed"
};

/*********************************************************************
*/
static void HttpServerParseMsgHeader
	(
	char 				*pcHttpReq, 
	uint32 				u32ReqSize,
	tstrHTTPReqHeader	*pstrHttpHdr
	)
{
	if((pcHttpReq != NULL) && (u32ReqSize > 0) && (pstrHttpHdr != NULL))
	{
		char	*pcCurr, *pcMethod;
		char	*pcREQLine, *pcHeaderLine;
		char	*pcHeaders;

		pstrHttpHdr->enuMethod 	= HTTP_REQ_UNKNOWN;
		pstrHttpHdr->pcMsgBody	= pcHttpReq;
		pstrHttpHdr->pcHost		= NULL;

		/* Get the Request Line
		*/
		HttpstrTOK(pcHttpReq, "");
		pcREQLine = HttpstrTOK(NULL, "\r");

		if(pcREQLine != NULL)
		{
			if(*pcREQLine != 0)
			{
				pcHeaders = pcHttpReq + strlen(pcREQLine) + 1;

				/* Parse the HTTP Request Line.
				*/
				HttpstrTOK(pcREQLine, "");
				pcMethod	= HttpstrTOK(NULL, " ");
				pstrHttpHdr->pcURL = HttpstrTOK(NULL, " ");

				if(!memcmp("GET", pcMethod, 3))
					pstrHttpHdr->enuMethod = HTTP_REQ_GET;
				else if(!memcmp("POST", pcMethod, 4))
					pstrHttpHdr->enuMethod = HTTP_REQ_POST;

				if(*(pstrHttpHdr->pcURL) == '/')
					pstrHttpHdr->pcURL ++;

				if(pstrHttpHdr->enuMethod != HTTP_REQ_UNKNOWN)
				{
					pstrHttpHdr->pcMsgBody	= NULL;

					/* Parse the request header fields.
					*/
					HttpstrTOK(pcHeaders, "");
					do
					{
						pcHeaderLine = HttpstrTOK(NULL, "\r");
						if(pcHeaderLine != NULL)
						{
							if(*pcHeaderLine != 0)
							{
								while((*pcHeaderLine == ' ') || (*pcHeaderLine == '\r') || (*pcHeaderLine == '\n'))
									pcHeaderLine ++;

								/* Get the "Host" Header field.
								*/
								if(!strncmp("Host", pcHeaderLine, 4))
								{
									pcCurr = pcHeaderLine + 4;
									while((*pcCurr == ' ') || (*pcCurr == ':'))
										pcCurr ++;

									pstrHttpHdr->pcHost = pcCurr;
								}
								else if(!strncmp("Content-Length", pcHeaderLine, 4))
								{
									pcCurr = pcHeaderLine + strlen("Content-Length");
									while((*pcCurr == ' ') || (*pcCurr == ':'))
										pcCurr ++;

									pstrHttpHdr->u32ContentLength = Ascii2Dec(pcCurr);
								}
								pcHeaderLine += strlen(pcHeaderLine) + 1;
								if(*pcHeaderLine ++ == '\n')
									if(*pcHeaderLine ++ == '\r')
										if(*pcHeaderLine ++ == '\n')
										{
											if(strlen(pcHeaderLine))
												pstrHttpHdr->pcMsgBody = pcHeaderLine;
											break;
										}
							}
							else
								break;
						}
						else
							break;
					}while((uint32)(pcHeaderLine - pcHttpReq) < u32ReqSize);
				}
			}
		}
	}
}
/*********************************************************************
*/
static int httpSrvParseRequest
	(
	char 			*pcHttpReq, 
	uint32 			u32ReqSize,
	tstrHttpReqInfo	*pstrReqInfo
	)
{
	int	ret = 50;

	if((pcHttpReq != NULL) && (pstrReqInfo != NULL))
	{
		tstrHTTPReqHeader		strREQHdr	= {0};
		uint8					bIsFile		= 0;

		pcHttpReq[u32ReqSize] = 0;

		pstrReqInfo->enuAction 		= HTTP_ACTION_SEND_ERROR;
		pstrReqInfo->enuRspErrCode	= HTTP_STATUS_BAD_REQ;
		ret = 0;
		HttpServerParseMsgHeader(pcHttpReq, u32ReqSize, &strREQHdr);
		HTTP_DBG("URL    <%s>\n", strREQHdr.pcURL);
		HTTP_DBG("Host   <%s>\n", strREQHdr.pcHost);

		if(strREQHdr.enuMethod != HTTP_REQ_UNKNOWN)
		{
			if(*(strREQHdr.pcURL) != 0)
			{
				memcpy(pstrReqInfo->acFileName, strREQHdr.pcURL, 256);
				pstrReqInfo->acFileName[255] = 0;
				bIsFile = 1;
				ret = 50;
			}
			if(strREQHdr.enuMethod == HTTP_REQ_GET)
			{
				pstrReqInfo->enuAction = HTTP_ACTION_UPLOAD_FILE;
				if(!bIsFile)
					strcpy(pstrReqInfo->acFileName, "index.html");
				ret = 50;
			}
			else if(strREQHdr.enuMethod == HTTP_REQ_POST)
			{
				if(bIsFile)
				{
					pstrReqInfo->enuAction			= HTTP_ACTION_DOWNLOAD_FILE;
					pstrReqInfo->u32PostFileSize	= strREQHdr.u32ContentLength;
					pstrReqInfo->u32MsgBodyLen		= 0;
					if(strREQHdr.pcMsgBody != NULL)
					{
						pstrReqInfo->pcMsgBody		= strREQHdr.pcMsgBody;
						pstrReqInfo->u32MsgBodyLen	= u32ReqSize - (strREQHdr.pcMsgBody - pcHttpReq);
					}
				}
				ret = 50;
			}
			else
			{
				ret = 2;
			}
		}
		else
		{
			ret = 2;
		}
	}
	return ret;
}
/*********************************************************************
*/
sint32 httpSrvProcessREQ(SOCKET sslSock,char *pcHttpREQ, uint32 u32ReqSize)
{
	sint32	ret = 1;

	if(pcHttpREQ != NULL)
	{
		tstrHttpReqInfo	strHttpREQ;

		httpSrvParseRequest(pcHttpREQ, u32ReqSize, &strHttpREQ);

		if(strHttpREQ.enuAction == HTTP_ACTION_UPLOAD_FILE)
		{
			static uint8	txBuf[256];
			uint32			u32BufSize;
			char			httpResponseHdr[] = "HTTP/1.0 200 OK\r\n"\
				"Content-type: text/html\r\n"\
				"Content-length: %d\r\n"\
				"\r\n%s";

			/* 
			Write HTTP Response header.
			*/
			u32BufSize = sprintf((char*)txBuf, httpResponseHdr, strlen("<html>Atmel SAMW25 SSL Application</html>"), "<html>Atmel SAMW25 SSL Application</html>");
			while(1)
			{
				ret = send(sslSock, txBuf, u32BufSize, 0);
				if(ret == M2M_SUCCESS)
					break;
			}
			close(sslSock);
		}
	}
	return ret;
}
/*********************************************************************
*/
static uint8 rxBuf[1400];
static void tcpSRVSockCB(SOCKET sock, uint8 u8Msg, void *pvMsg)
{
	if(u8Msg == SOCKET_MSG_BIND)
	{
		tstrSocketBindMsg	*pstrBind = (tstrSocketBindMsg*)pvMsg;
		M2M_INFO("Bind %d\n", pstrBind->status);
	}
	else if(u8Msg == SOCKET_MSG_LISTEN)
	{
		tstrSocketListenMsg	*pstrListen = (tstrSocketListenMsg*)pvMsg;
		M2M_INFO("Listen %d\n", pstrListen->status);
	}
	else if(u8Msg == SOCKET_MSG_ACCEPT)
	{
		tstrSocketAcceptMsg	*pstrAccept = (tstrSocketAcceptMsg*)pvMsg;
		if(pstrAccept->sock >= 0)
		{
			M2M_INFO("Accepted %d\n", pstrAccept->sock);
			recv(pstrAccept->sock, rxBuf, sizeof(rxBuf), 0);
		}
	}
	else if(u8Msg == SOCKET_MSG_RECV)
	{
		tstrSocketRecvMsg	*pstrRx = (tstrSocketRecvMsg*)pvMsg;
		if((pstrRx->pu8Buffer != NULL) && (pstrRx->s16BufferSize > 0))
		{
			httpSrvProcessREQ(sock, pstrRx->pu8Buffer, pstrRx->s16BufferSize);
		}
		else
		{
			close(sock);
		}
	}
}
/*********************************************************************
*/
void httpServerStart(void)
{
	static SOCKET	tcpSRVSock = -1;
	
	socketInit();
	registerSocketCallback(tcpSRVSockCB, NULL);
	
	tcpSRVSock = socket(AF_INET, SOCK_STREAM, SOCKET_CONFIG_SSL_ON);
	if(tcpSRVSock >= 0)
	{
		struct sockaddr_in	addr;
		
		addr.sin_family			= AF_INET;
		addr.sin_port			= _htons(443);
		addr.sin_addr.s_addr	= 0;
		bind(tcpSRVSock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
		listen(tcpSRVSock, 1);
	}
}