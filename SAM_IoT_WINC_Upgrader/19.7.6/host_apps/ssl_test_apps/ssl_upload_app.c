
#include "asf.h"
#include "ssl_test_conf.h"
#include "socket/include/socket.h"
#include <string.h>

#if TEST_APP == TEST_APP_SSL_UPLOAD

#if 0
#define PRINT(...) printf(__VA_ARGS__);
#else
#define PRINT(...)
#endif


/*
	SSL UPLOAD TEST CONFIGURATIONS
*/
#define LOCAL_SERVER_IP_ADDR				"192.168.0.101"
#define MAIN_WIFI_M2M_BUFFER_SIZE			1460
#define MAIN_WIFI_M2M_SERVER_PORT			(8080)
#define MAIN_WIFI_M2M_SERVER_PORT_SSL		(4402)




typedef enum{
	PUT = 0,
	GET
}request_type_t;


typedef enum
{
	APP_IDLE = 0,
	APP_WIFI_CONNECT,
	APP_TCP_CONNECT,
	APP_UPLOAD,
	APP_TCP_DISCONNECT,
	APP_WIFI_DISCONNECT
}app_states_t;


static SOCKET			tcp_client_socket = -1;
static uint8_t			gau8SocketTestBuffer[MAIN_WIFI_M2M_BUFFER_SIZE];
char					rest_request[1024];
char					upload_data[1024]; //= "Welcome to Amazon S3.";
static app_states_t		app_state = APP_WIFI_CONNECT;
static bool				request_sent = false;
static uint32_t			send_bytes = 0,sent_bytes = 0,req_length = 0,received_bytes = 0,download_time = 0;

/***********************************************************/
inline static app_states_t get_app_state(void)
{
	return app_state;
}

/***********************************************************/
inline static void set_app_state(app_states_t state)
{
	app_state = state;
}

/***********************************************************/
static int construct_request(request_type_t req,char* buffer)
{
	int request_length = 0;
	if(req == PUT)
	{
		request_length += sprintf(buffer,HTTP_POST_REQ,LOCAL_SERVER_IP_ADDR);
		PRINT("PUT ");
	}
	else if(req == GET)
	{
		request_length += sprintf(buffer,HTTP_GET_REQ,LOCAL_SERVER_IP_ADDR);
		PRINT("GET ");
	}
	PRINT("REQ:");
	PRINT("%s\r\n",buffer);
	return request_length;
}

/***********************************************************/
static void socket_cb(SOCKET sock, uint8_t u8Msg, void *pvMsg)
{
	switch (u8Msg)
	{
	case SOCKET_MSG_CONNECT:
	{
		tstrSocketConnectMsg *pstrConnect = (tstrSocketConnectMsg *)pvMsg;
		if (pstrConnect && pstrConnect->s8Error >= 0)
		{
			//printf("socket_cb: connect success!\r\n");
			//Be ready to receive data on this socket
			m2m_memset(gau8SocketTestBuffer, 0, sizeof(gau8SocketTestBuffer));
			recv(tcp_client_socket, gau8SocketTestBuffer, sizeof(gau8SocketTestBuffer), 0);
			set_app_state(APP_UPLOAD);	
		}
		else
		{
			printf("socket_cb: connect error -%d!\r\n",pstrConnect->s8Error);
			set_app_state(APP_TCP_DISCONNECT);
		}
	}
	break;

	/* Message send */
	case SOCKET_MSG_SEND:
	{
		sint16 s16SentBytes = *((sint16*)pvMsg);
		if(s16SentBytes < 0)
		{
			printf("Socket (%d) Send Error: %d\r\n",sock , s16SentBytes);
			set_app_state(APP_TCP_DISCONNECT);
			break;
		}
		else
		{
			sent_bytes += s16SentBytes;
			//printf("Uploaded %lu bytes\r\n",sent_bytes);
			if(sent_bytes == (1024*1024) + req_length )
			{
				printf("(%lu)%lu\r\n",sent_bytes, ms_ticks-download_time);
				//printf("Uploaded 1MB of data\r\n");
				set_app_state(APP_TCP_DISCONNECT);	
			}	
			//printf("(%u)(%u)(%u)\r\n",sent_bytes, s16SentBytes, req_length);
		}
	}
	break;

	/* Message receive */
	case SOCKET_MSG_RECV:
	{
		tstrSocketRecvMsg *pstrRecv = (tstrSocketRecvMsg *)pvMsg;
		if (pstrRecv && pstrRecv->s16BufferSize > 0) {
			if(strstr((char*)pstrRecv->pu8Buffer,"HTTP/1.1 200 OK"))
			{
				set_app_state(APP_TCP_DISCONNECT);	
			}
			else
			{
				recv(tcp_client_socket, gau8SocketTestBuffer, sizeof(gau8SocketTestBuffer), 0);	
			}
		} else {
			printf("socket_cb: recv error!\r\n");
			set_app_state(APP_TCP_DISCONNECT);
		}
	}

	break;

	default:
		break;
	}
}

/***********************************************************/
void WifiCallback(uint8 u8MsgType, void * pvMsg)
{
	switch (u8MsgType) 
	{
	case M2M_WIFI_RESP_CON_STATE_CHANGED:
		{
			tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
			if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) 
			{
				printf("Wi-Fi connected\r\n");
			} 
			else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) 
			{
				printf("Wi-Fi disconnected\r\n");
				set_app_state(APP_WIFI_CONNECT);
			}
		}
		break;

	case M2M_WIFI_REQ_DHCP_CONF:
		{
			uint8_t *pu8IPAddress = (uint8_t *)pvMsg;
			printf("Wi-Fi IP is %u.%u.%u.%u\r\n", pu8IPAddress[0], pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
			set_app_state(APP_TCP_CONNECT);
		}
		break;
	
	default:
		break;
	}
}

/***********************************************************/
void SslUploadAppRun(void)
{
	int8_t				ret;
	struct sockaddr_in	addr;

	memset(upload_data,0x61,sizeof(upload_data));

	/* Initialize socket module */
	socketInit();
	registerSocketCallback(socket_cb, NULL);

	/* Infinite loop to handle a event from the WINC1500. */
	while (1) 
	{
		m2m_wifi_handle_events(NULL);
		
		switch(app_state)
		{
		case APP_IDLE:
			break;

		case APP_WIFI_CONNECT:
			send_bytes = 0;
			sent_bytes = 0;
			req_length = 0;
			received_bytes = 0;
			request_sent = false;
			if(tcp_client_socket >= 0)
			{
				close(tcp_client_socket);
				tcp_client_socket = -1;
			}
			AppWifiConnect(APP_WIFI_MODE_WPA);
			set_app_state(APP_IDLE);
			break;
			
		case APP_TCP_CONNECT:
			if (tcp_client_socket < 0) 
			{
				uint8	u8SslConfig = SOCKET_CONFIG_SSL_ON;
								
				if ((tcp_client_socket = socket(AF_INET, SOCK_STREAM, u8SslConfig)) < 0) 
				{
					printf("main: failed to create TCP client socket error!\r\n");
					set_app_state(APP_TCP_CONNECT);
					break;
				}
				
				/*
					SELECT AES_128_CBC_SHA256 Ciphers for faster operation.
				*/
				m2m_ssl_set_active_ciphersuites(SSL_CIPHER_DHE_RSA_WITH_AES_128_CBC_SHA256 | SSL_CIPHER_RSA_WITH_AES_128_CBC_SHA256);
					
				addr.sin_family			= AF_INET;
				addr.sin_port			= _htons( MAIN_WIFI_M2M_SERVER_PORT_SSL);
				addr.sin_addr.s_addr	= nmi_inet_addr((char*)LOCAL_SERVER_IP_ADDR);				
				ret = connect(tcp_client_socket, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
				if (ret < 0)
				{
					close(tcp_client_socket);
					tcp_client_socket = -1;
					set_app_state(APP_TCP_CONNECT);
					break;
				}
			}
			set_app_state(APP_IDLE);
			break;
			
		case APP_UPLOAD:
			if(!request_sent)
			{
				request_sent = true;
				req_length = construct_request(PUT,(char*)&rest_request);
				download_time = ms_ticks;
				ret = send( tcp_client_socket,rest_request,req_length, 0 );
				if( 0 != ret )
				{
					request_sent = false;
				}						
			}
			else
			{
				if(send_bytes < 1024*1024)
				{
					ret = send( tcp_client_socket,upload_data,sizeof(upload_data), 0 );
					if(ret == 0)
					{
						send_bytes += 1024;	
					}
				}
				else
				{
					printf("No ack sent time..%lu\r\n",ms_ticks-download_time);
					set_app_state(APP_IDLE);
				}
			}
			break;
			
		case APP_TCP_DISCONNECT:
			close(tcp_client_socket);
			tcp_client_socket = -1;
			send_bytes = 0;
			sent_bytes = 0;
			req_length = 0;
			received_bytes = 0;
			request_sent = false;
			set_app_state(APP_TCP_CONNECT);
			break;
			
		case APP_WIFI_DISCONNECT:
		delay_ms(100);
		m2m_wifi_disconnect();
			break;
		}
	}
}
#endif