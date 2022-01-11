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
#include <string.h>
#include "delay.h"

/*******************************************************************************************
MACROS
*******************************************************************************************/

/** security information for Wi-Fi connection */
#define DEFAULT_SSID				"DEMO_AP"
#define DEFAULT_AUTH				M2M_WIFI_SEC_WPA_PSK
#define	DEFAULT_KEY					"12345678"


#define UIP_ETHTYPE_ARP 0x0806
#define ARP_REQ_OPCODE   0x01
#define ARP_REPLY_OPCODE 0x02

#define BUFLEN 1500

/*******************************************************************************************
GLOBALS
*******************************************************************************************/
extern volatile uint32_t ms_ticks;

#define ARP_TIMEOUT 1000
#define ARP_RETRY_COUNT 5
static uint32_t arp_send_time;
static uint32_t arp_retry_count;

static uint8_t eth_rx_pkt_buff[BUFLEN];

typedef enum
{
	APP_IDLE = 0,
	APP_WIFI_CONNECTING,
	APP_WIFI_CONNECTED,
	APP_ARP_REQUESTED,
	APP_ARP_RECEIVED,
	APP_WIFI_DISCONNECTED,
	APP_WAIT
} app_states_t;

app_states_t app_state;

static uint8_t my_ip_addr[] = { 192, 168, 1, 100 };
static uint8_t my_mac_addr[6];
static uint8_t remote_peer_ip_addr[] = { 192, 168, 1, 1 };
static uint8_t remote_peer_mac_addr[6];

static uint8_t bcast_ip_addr[] = { 255, 255, 255, 255 };
static uint8_t bcast_mac_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

/***********************************************************/

static void send_arp_req(void)
{
	uint8_t eth_tx_pkt_buff[42];
	printf("TX ARP request: Who has %u.%u.%u.%u \r\n",
	remote_peer_ip_addr[0], remote_peer_ip_addr[1], remote_peer_ip_addr[2], remote_peer_ip_addr[3]);
	
	memset(&eth_tx_pkt_buff[0], 0xff, 6); //BROADCAST message.
	memcpy(&eth_tx_pkt_buff[6], my_mac_addr, 6);
	eth_tx_pkt_buff[12] = (UIP_ETHTYPE_ARP >> 8) & 0xff;
	eth_tx_pkt_buff[13] = (UIP_ETHTYPE_ARP >> 0) & 0xff;
	eth_tx_pkt_buff[14] = 0x00; eth_tx_pkt_buff[15]	=  0x01; //Hardware Type Ethernet
	eth_tx_pkt_buff[16] = 0x08; eth_tx_pkt_buff[17]	=  0x00; //Protocol Type : IP
	eth_tx_pkt_buff[18] = 0x06; //Hardware Size
	eth_tx_pkt_buff[19] = 0x04; //Protocol Size
	eth_tx_pkt_buff[20] = 0x00;
	eth_tx_pkt_buff[21] = ARP_REQ_OPCODE;//OpCode :request
	memcpy(&eth_tx_pkt_buff[22], my_mac_addr, 6);  //Sender Mac address
	memcpy(&eth_tx_pkt_buff[28], my_ip_addr, 4); //Sender IP Address
	memset(&eth_tx_pkt_buff[32], 0, 6); // Target Mac address
	memcpy(&eth_tx_pkt_buff[38], remote_peer_ip_addr, 4); //Target IP address
	m2m_wifi_send_ethernet_pkt(eth_tx_pkt_buff, 42);
}

static void ethernet_demo_cb(uint8 u8MsgType,void * pvMsg,void * pvCtrlBf)
{
	uint8_t *eth_rx_pkt_ptr = (uint8*)pvMsg;
	uint8_t * eth_dst_mac, *eth_src_mac;
	uint16_t  eth_type;
	tstrM2mIpCtrlBuf     *ip_ctrl_buf =(tstrM2mIpCtrlBuf *)pvCtrlBf;
	
	if(u8MsgType != M2M_WIFI_RESP_ETHERNET_RX_PACKET) goto EXIT;
	
	eth_dst_mac = &eth_rx_pkt_ptr[0];
	eth_src_mac = &eth_rx_pkt_ptr[6];
	eth_type = (eth_rx_pkt_ptr[12] << 8) | eth_rx_pkt_ptr[13];
	
	if(memcmp(eth_rx_pkt_ptr, my_mac_addr, 6) != 0) {
		if(memcmp(eth_rx_pkt_ptr, bcast_mac_addr, 6) != 0) {
			goto EXIT;
		}
	}
	
	/************************* ARP ****************************/
	if(eth_type == UIP_ETHTYPE_ARP)
	{
		uint8_t arp_opcode = eth_rx_pkt_ptr[21];
		uint8 * arp_sender_mac_addr = &eth_rx_pkt_ptr[22];//Sender Mac address
		uint8 * arp_sender_ip_addr = &eth_rx_pkt_ptr[28];//Sender IP address
		uint8 * arp_target_mac_addr = &eth_rx_pkt_ptr[32];//Target Mac address
		uint8 * arp_target_ip_addr = &eth_rx_pkt_ptr[38];//Target IP address
		/********************ARP REQUEST***************************/
		if(arp_opcode == ARP_REQ_OPCODE)
		{
			printf("RX ARP request: Who has %u.%u.%u.%u?\r\n",
			arp_target_ip_addr[0], arp_target_ip_addr[1], arp_target_ip_addr[2], arp_target_ip_addr[3]);
			
			if(memcmp(arp_target_ip_addr, my_ip_addr, 4) == 0) {
				uint8_t eth_tx_pkt_buff[42];
				
				memcpy(&eth_tx_pkt_buff[0], arp_sender_mac_addr, 6); //Destination Mac address
				memcpy(&eth_tx_pkt_buff[6], my_mac_addr, 6); // Source Mac address
				eth_tx_pkt_buff[12] = (UIP_ETHTYPE_ARP >> 8) & 0xff;
				eth_tx_pkt_buff[13] = (UIP_ETHTYPE_ARP >> 0) & 0xff;
				eth_tx_pkt_buff[14] = 0x00; eth_tx_pkt_buff[15]	=  0x01; //Hardware Type Ethernet
				eth_tx_pkt_buff[16] = 0x08; eth_tx_pkt_buff[17]	=  0x00; //Protocol Type : IP
				eth_tx_pkt_buff[18] = 0x06; //Hardware Size
				eth_tx_pkt_buff[19] = 0x04; //Protocol Size
				eth_tx_pkt_buff[20] = 0x00;
				eth_tx_pkt_buff[21] = ARP_REPLY_OPCODE; //OpCode :request
				memcpy(&eth_tx_pkt_buff[22], my_mac_addr,6);  //Sender Mac address
				memcpy(&eth_tx_pkt_buff[28], my_ip_addr, 4); //Sender IP Address
				memcpy(&eth_tx_pkt_buff[32], arp_sender_mac_addr, 6); // Target Mac address
				memcpy(&eth_tx_pkt_buff[38], arp_sender_ip_addr, 4); //Target IP address
				m2m_wifi_send_ethernet_pkt(eth_tx_pkt_buff, 42);
				
				printf("TX ARP reply:%u.%u.%u.%u is at %02x:%02x:%02x:%02x:%02x:%02x\r\n",
				my_ip_addr[0], my_ip_addr[1], my_ip_addr[2], my_ip_addr[3],
				my_mac_addr[0], my_mac_addr[1], my_mac_addr[2],
				my_mac_addr[3], my_mac_addr[4], my_mac_addr[5]);
			}
		}
		/********************ARP REPLY****************************/
		if(arp_opcode == ARP_REPLY_OPCODE)
		{
			if(memcmp(arp_sender_ip_addr, remote_peer_ip_addr, 4) == 0) {
				memcpy(remote_peer_mac_addr, arp_sender_mac_addr, 6); //Extract mac address of the targeted station to be pinged
				printf("RX ARP reply:%u.%u.%u.%u is at %02x:%02x:%02x:%02x:%02x:%02x\r\n",
				remote_peer_ip_addr[0], remote_peer_ip_addr[1], remote_peer_ip_addr[2], remote_peer_ip_addr[3],
				remote_peer_mac_addr[0], remote_peer_mac_addr[1], remote_peer_mac_addr[2],
				remote_peer_mac_addr[3], remote_peer_mac_addr[4], remote_peer_mac_addr[5]);
				app_state = APP_ARP_RECEIVED;
			}
		}
	}
	EXIT:
	return;
}

/***********************************************************/
static void wifi_cb(uint8 u8MsgType, void * pvMsg)
{
	if(u8MsgType == M2M_WIFI_RESP_CON_STATE_CHANGED)
	{
		tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged*) pvMsg;
		printf("Wifi State :: %s ::\r\n", pstrWifiState->u8CurrState ? "CONNECTED" : "DISCONNECTED");
		if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
			app_state = APP_WIFI_CONNECTED;
			} else if(pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
			app_state = APP_WIFI_DISCONNECTED;
		}
	}
}
/***********************************************************/
void app_run_sm(void)
{
	switch(app_state)
	{
		case APP_IDLE:
		{
			app_state = APP_WIFI_CONNECTING;
			m2m_wifi_connect((char*)DEFAULT_SSID, sizeof(DEFAULT_SSID), DEFAULT_AUTH, DEFAULT_KEY, M2M_WIFI_CH_ALL);
		}
		break;
		case APP_WIFI_DISCONNECTED:
		{
			app_state = APP_WAIT;
		}
		break;
		case APP_WAIT:
		{
			delay_s(3);
			app_state = APP_IDLE;
		}
		break;
		case APP_WIFI_CONNECTED:
		{
			app_state = APP_ARP_REQUESTED;
			arp_send_time = ms_ticks;
			arp_retry_count = 0;
			send_arp_req();
		}
		break;
		case APP_ARP_REQUESTED:
		{
			if(ms_ticks < (arp_send_time+ARP_TIMEOUT)) {
				/* do nothing an wait... */
				} else {
				arp_retry_count++;
				if(arp_retry_count < ARP_RETRY_COUNT) {
					arp_send_time = ms_ticks;
					send_arp_req();
					} else {
					/* No response... disconnect Wi-Fi */
					m2m_wifi_disconnect();
				}
			}
		}
		break;
		case APP_ARP_RECEIVED:
		{
			/* do nothing */
		}
		break;
		
		default:
		break;
	}
}
/***********************************************************/
void demo_start(void)
{
	sint8				ret;
	tstrWifiInitParam	param;
	
	/* Initialize the BSP. */
	nm_bsp_init();
	
	memset((uint8*)&param, 0, sizeof(param));
	param.pfAppWifiCb = wifi_cb;
	param.strEthInitParam.u8EthernetEnable = M2M_WIFI_MODE_ETHERNET;
	param.strEthInitParam.pfAppWifiCb = wifi_cb;
	param.strEthInitParam.pfAppEthCb = ethernet_demo_cb;
	param.strEthInitParam.au8ethRcvBuf = eth_rx_pkt_buff;
	param.strEthInitParam.u16ethRcvBufSize = sizeof(eth_rx_pkt_buff);

	ret = m2m_wifi_init(&param);
	if (M2M_SUCCESS != ret)
	{
		M2M_ERR("Driver Init Failed <%d>\n",ret);
		for (;;)
		{
		}
	}
	
	m2m_wifi_get_mac_address(my_mac_addr);
	
	app_state = APP_IDLE;

	for (;;)
	{
		while(m2m_wifi_handle_events(NULL) != M2M_SUCCESS)
		{
		}
		
		app_run_sm();
	}
}
