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
#include "udp_demo.h"

#ifdef UDP_DEMO

#include <at30tse75x.h>
#include <port.h>
#include <string.h>

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
#define TEST_BUFFER_SIZE			1400
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
GLOBALS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
/* Demo string identifiers
 */
static uint8 client_keepalive[]    = { CLIENT_KEEPALIVE };
static uint8 client_report[]       = { CLIENT_REPORT };
static const uint8 server_report[] = { SERVER_REPORT };

/* Receive buffers for test sockets. 
*/
static uint8	gau8SocketTestBuffer[TEST_BUFFER_SIZE];

/* Sockets.
*/
static SOCKET	udpBcastSocket      = -1; 
static SOCKET	gUdpTxSocket        = -1;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
NMI_API void TCP_SocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg);
NMI_API void UDP_SocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg);
NMI_API void TEST_APPSocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg);

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
	uint32 tempsensor;
	double temp;
	static struct sockaddr_in	addr,cliaddr;
	
	/*
	 * Create the socket for the first time.
	 */
	if(udpBcastSocket < 0)
	{
		udpBcastSocket = socket(AF_INET,SOCK_DGRAM,0);
		if(udpBcastSocket < 0)
		{
			M2M_ERR("UDP Client Socket Creation Failed\n");
			return;
		}
		cliaddr.sin_family = AF_INET;
		cliaddr.sin_addr.s_addr= 0xFFFFFFFF;
		cliaddr.sin_port=_htons(DEMO_SERVER_PORT);
		bind(udpBcastSocket, (struct sockaddr*)&cliaddr, sizeof(struct sockaddr_in));
	}
	
	if(gUdpTxSocket < 0)
	{
		gUdpTxSocket = socket(AF_INET,SOCK_DGRAM,0);
		if(gUdpTxSocket < 0)
		{
			M2M_ERR("UDP Client Socket Creation Failed\n");
			return;
		}
	}
	

	addr.sin_family			= AF_INET;
	addr.sin_port			= _htons(DEMO_SERVER_PORT);
	addr.sin_addr.s_addr	= 0xFFFFFFFF;
	
	temp = at30tse_read_temperature();
	tempsensor = (uint32)(temp * 10);
	client_report[CLIENT_RPOS_LED] = '0' + !port_pin_get_output_level(CONF_DEMO_LED_PIN);
	client_report[CLIENT_RPOS_TEMP] = ((tempsensor / 100) % 10) + '0';
	client_report[CLIENT_RPOS_TEMP + 1] = ((tempsensor / 10) % 10) + '0';
	client_report[CLIENT_RPOS_TEMP + 3] = (tempsensor % 10) + '0';

	sendto(gUdpTxSocket, client_keepalive, sizeof(client_keepalive), 0, (struct sockaddr*)&addr, sizeof(addr));
	sendto(gUdpTxSocket, client_report,    sizeof(client_report),    0, (struct sockaddr*)&addr, sizeof(addr));
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
	} else if(u8Msg == SOCKET_MSG_BIND)	{
		tstrSocketBindMsg	*pstrBind = (tstrSocketBindMsg*)pvMsg;
		if(pstrBind != NULL)
		{
			if(pstrBind->status == 0)
			{
				recvfrom(sock, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 0);
			}
			else
			{
				M2M_ERR("bind\n");
			}
		}
	}
	else if(u8Msg == SOCKET_MSG_RECVFROM)
	{
		tstrSocketRecvMsg	*pstrRx = (tstrSocketRecvMsg*)pvMsg;

		if(pstrRx->pu8Buffer != NULL)
		{
			if(pstrRx->s16BufferSize)
			{
				uint8 *result;
					/* Check for server report and update led status if necessary. */
				result = (uint8 *)strstr((const char *)pstrRx->pu8Buffer, (const char *)server_report);
				if (result) {
					//dbg_print_str("Received server message!\r\n");
					port_pin_set_output_level(CONF_DEMO_LED_PIN, result[SERVER_RPOS_LED] == '1' ? false : true);
					UDP_SendBcast();
				}
			}
			recvfrom(sock, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 0);
		}
		else
		{
			if(pstrRx->s16BufferSize == SOCK_ERR_TIMEOUT)
			{
				recvfrom(sock, gau8SocketTestBuffer, TEST_BUFFER_SIZE, 0);
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
NMI_API void TEST_APPSocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg)
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
	udpBcastSocket			= -1;
	gUdpTxSocket = -1;
	socketInit();
	registerSocketCallback(TEST_APPSocketEventHandler, NULL);
	UDP_SendBcast();
}
#endif
