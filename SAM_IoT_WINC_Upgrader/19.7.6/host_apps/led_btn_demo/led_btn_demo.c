#include "m2m_test_config.h"
#include "socket/include/socket.h"
#include "led_btn_demo.h"

#ifdef LED_BTN_DEMO

#define UDP_ACTION_RECV				1
#define UDP_ACTION_SEND				2
#define TEST_RECV_TIMEOUT			0
#define TEST_BUFFER_SIZE			4
#define TEST_PACKET_COUNT			50
#define TIMEOUT_MAX					10
#define TCP_SERVER_PORT				20000
#define UDP_SERVER_PORT				25000

tpfLedBtnDemoCb gpfLedBtnDemoCb;

/* Receive buffers for test sockets. 
*/
static uint8	gau8RxBuffer[TEST_BUFFER_SIZE];

/* Sockets.
*/
static SOCKET	TcpServerListenSocket	= -1;
static SOCKET	udpServerSocket			= -1;
static SOCKET	TcpNotificationSocket	= -1;

/*********************************************************************
Function: 

Description:

Return: None.

Author: Ahmed Ezzat

Version: 1.0

Date: 
*********************************************************************/
static void TCP_StartServer(void)
{
	struct sockaddr_in	addr;

	/* TCP Server. */
	if(TcpServerListenSocket == -1)
	{
		TcpServerListenSocket = socket(AF_INET,SOCK_STREAM,0);
		if(TcpServerListenSocket >= 0)
		{
			addr.sin_family			= AF_INET;
			addr.sin_port			= _htons(TCP_SERVER_PORT);
			addr.sin_addr.s_addr	= 0;
			bind(TcpServerListenSocket,(struct sockaddr*)&addr,sizeof(addr));
		}
		else
		{
			M2M_ERR("TCP Server Socket Creation Failed\n");
			return;
		}
	}
	else
	{
		accept(TcpServerListenSocket,NULL,0);
	}
}
/*********************************************************************
Function: 

Description:

Return: None.

Author: Ahmed Ezzat

Version: 1.0

Date: 
*********************************************************************/
static void UDP_StartServer(void)
{
	struct sockaddr_in	addr;
	if(udpServerSocket == -1)
	{
		udpServerSocket = socket(AF_INET,SOCK_DGRAM,0);
		if(udpServerSocket >= 0)
		{
			addr.sin_family			= AF_INET;
			addr.sin_port			= _htons(UDP_SERVER_PORT);
			addr.sin_addr.s_addr	= 0;
			bind(udpServerSocket,(struct sockaddr*)&addr,sizeof(addr));
		}
		else
		{
			M2M_ERR("UDP Server Socket Creation Failed\n");
			return;
		}
	}
	else
	{
		recvfrom(udpServerSocket, gau8RxBuffer, sizeof(udpServerSocket),0);
	}
}
/*********************************************************************
Function: 

Description:

Return: None.

Author: Ahmed Ezzat

Version: 1.0

Date: 
*********************************************************************/
static void UDP_SocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg)
{
	switch(u8Msg)
	{	
	case SOCKET_MSG_BIND:
		{
			tstrSocketBindMsg	*pstrBind = (tstrSocketBindMsg*)pvMsg;
			if(pstrBind != NULL)
			{
				if(pstrBind->status == 0)
				{
					recvfrom(udpServerSocket,gau8RxBuffer, sizeof(gau8RxBuffer),0);
				}
				else
				{
					M2M_ERR("bind\n");
				}
			}
		}
		break;
	case SOCKET_MSG_RECVFROM:
		{
			tstrSocketRecvMsg	*pstrRx = (tstrSocketRecvMsg*)pvMsg;
			if(pstrRx->pu8Buffer != NULL)
			{
				sendto(udpServerSocket, (uint8*)"m2m", 3, 0, (struct sockaddr*)&pstrRx->strRemoteAddr, sizeof(struct sockaddr_in));
				recvfrom(udpServerSocket,gau8RxBuffer, sizeof(gau8RxBuffer),0);
			}
		}
		break;
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
static void TCP_SocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg)
{	
	uint8 u8counter;
	
	switch(u8Msg)
	{	
	case SOCKET_MSG_BIND:
		{
			tstrSocketBindMsg	*pstrBind = (tstrSocketBindMsg*)pvMsg;
			if(pstrBind != NULL)
			{
				if(pstrBind->status == 0)
				{
					listen(sock, 2);
				}
				else
				{
					M2M_ERR("bind\n");
				}
			}
		}
		break;

	case SOCKET_MSG_LISTEN:
		{

			tstrSocketListenMsg	*pstrListen = (tstrSocketListenMsg*)pvMsg;
			if(pstrListen != NULL)
			{
				if(pstrListen->status == 0)
				{
					accept(sock,NULL,0);
				}
				else
				{
					M2M_ERR("listen\n");
				}
			}
		}
		break;

	case SOCKET_MSG_ACCEPT:
		{
			tstrSocketAcceptMsg	*pstrAccept = (tstrSocketAcceptMsg*)pvMsg;
			if(TcpServerListenSocket!=-1)
			{
				accept(TcpServerListenSocket,NULL,0);
			}
			if(pstrAccept->sock >= 0)
			{
				TcpNotificationSocket = pstrAccept->sock;
				recv(pstrAccept->sock,gau8RxBuffer,sizeof(gau8RxBuffer),TEST_RECV_TIMEOUT);
			}
			else
			{
				M2M_ERR("accept\n");
			}
		}
		break;

	case SOCKET_MSG_SEND:
		{
		}
		break;

	case SOCKET_MSG_RECV:
		{
			tstrSocketRecvMsg	*pstrRx = (tstrSocketRecvMsg*)pvMsg;
			if(pstrRx->pu8Buffer != NULL)
			{
				if(gpfLedBtnDemoCb)
				{
					/* consume all the data in the tcp packet (if Repacketization happened) */
					for (u8counter=0; u8counter<pstrRx->s16BufferSize; u8counter++)
					{
						gpfLedBtnDemoCb(pstrRx->pu8Buffer[u8counter]);
					}
				}
				pstrRx->pu8Buffer[pstrRx->s16BufferSize] = '\0';
				recv(sock,gau8RxBuffer,sizeof(gau8RxBuffer),TEST_RECV_TIMEOUT);			
			}
			else
			{
				if(pstrRx->s16BufferSize == SOCK_ERR_TIMEOUT)
				{
					recv(sock,gau8RxBuffer,sizeof(gau8RxBuffer),TEST_RECV_TIMEOUT);	
				}
				else
				{
					close(sock);
					TcpNotificationSocket = -1;
				}
			}
		}
		break;
	
	default:
		break;
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
static void P2P_APPSocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg)
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
void LedBtnDemo_AppSendNotification(char *pcMsg)
{
	if((pcMsg != NULL)&&(TcpNotificationSocket >= 0))
	{
		send(TcpNotificationSocket,pcMsg,m2m_strlen((uint8*)pcMsg),0);
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
void LedBtnDemo_AppStart(tpfLedBtnDemoCb pfLedBtnDemoCb)
{
	gpfLedBtnDemoCb = pfLedBtnDemoCb;
	socketInit();
	registerSocketCallback(P2P_APPSocketEventHandler, NULL);
	if(TcpServerListenSocket != -1)
	{
		close(TcpServerListenSocket);
		TcpServerListenSocket = -1;
	}
	TCP_StartServer();
	if(udpServerSocket != -1)
	{
		close(udpServerSocket);
		udpServerSocket = -1;
	}
	UDP_StartServer();
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
void LedBtnDemo_AppStop(void)
{
	if(TcpServerListenSocket!=-1)
	{
		close(TcpServerListenSocket);
	}
	if(udpServerSocket != -1)
	{
		close(udpServerSocket);
	}
	TcpServerListenSocket = -1;
	udpServerSocket = -1;
}

#endif

/**/
