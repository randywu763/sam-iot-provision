/*!
@file		
	m2m_test_config.h

@brief				
	Configurations and definitions of different test applications.

@author
	NMI
*/

#ifndef __M2M_TEST_CONFIG_H__
#define __M2M_TEST_CONFIG_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
#include "common/include/nm_common.h"
#include "driver/include/m2m_wifi.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define M2M_SEC_PARAM_MAX				M2M_MAX(M2M_PSK_MAX_LENGTH, sizeof(tstr1xAuthCredentials))

#ifdef ETH_MODE
#define LIST_CONNECT
#define CONF_STATIC_IP_ADDRESS
#endif

typedef struct{
	uint8	au8Ssid[M2M_MAX_SSID_LEN];
	uint8	*pu8AuthCred;	
	uint8	u8AuthType;		
} tstrM2mAp;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
#define OTA_URL			"https://s3.amazonaws.com/aezzat/m2m_ota_3a0.bin"

/*************************
*    AP CONFIGURATIONS   *
**************************/

#define NMI_M2M_AP						"WINC1500_HOTSPOT_00:00"
#define NMI_M2M_AP_SEC					M2M_WIFI_SEC_WPA_PSK
#define NMI_M2M_AP_WEP_KEY_INDEX		M2M_WIFI_WEP_KEY_INDEX_1
#define NMI_M2M_AP_WEP_KEY				"1234567890"
#define NMI_M2M_AP_WPA_KEY				"12345678"
#define NMI_M2M_AP_SSID_MODE			SSID_MODE_VISIBLE
#define NMI_M2M_AP_CHANNEL				M2M_WIFI_CH_11

#define HTTPS_PROV_SERVER_DN			"https://prov.winc.atmel.com"
#define HTTP_PROV_SERVER_DN				"http://prov.winc.atmel.com"
#define HTTP_PROV_SERVER_IP_ADDRESS		{192, 168, 1, 1}
#define HTTP_REDIRECT_FLAG				1

/*************************
* STATION CONFIGURATIONS *
**************************/

#define M2M_DEVICE_NAME				"WINC1500_00:00"
#define MAC_ADDRESS					{0xf8, 0xf0, 0x05, 0x45, 0xD4, 0x84}

/*********** Wi-Fi Enterprise Settings ***********
# RADIUS Server 
	RADIUS IP Address    : 
	RADIUS Port			 : 
	RADIUS Shared Secret : 

# Access Credentials
	User-Name : WINC1500
	Password  : WINC1500
**************************************************/

#define M2M_802_1X_USR_NAME			"WINC1500"
#define M2M_802_1X_PWD				"WINC1500"

#define DEFAULT_SSID				"DEMO_AP"
#define DEFAULT_AUTH				M2M_WIFI_SEC_WPA_PSK
#define	DEFAULT_KEY					"12345678"

#define WEP_KEY_INDEX				M2M_WIFI_WEP_KEY_INDEX_1
#define WEP_KEY						"1234567890"
#define WEP_KEY_SIZE				sizeof(WEP_KEY)


#define WEP_CONN_PARAM				{ WEP_KEY_INDEX, WEP_KEY_SIZE, WEP_KEY}

#define AUTH_CREDENTIALS			{ M2M_802_1X_USR_NAME, M2M_802_1X_PWD }

#define WINC_HOTSPOT_CONF	\
	{NMI_M2M_AP, NMI_M2M_AP_CHANNEL, M2M_WIFI_WEP_KEY_INDEX_1, WEP_40_KEY_STRING_SIZE, \
	NMI_M2M_AP_WEP_KEY, (uint8) NMI_M2M_AP_SEC, NMI_M2M_AP_SSID_MODE, HTTP_PROV_SERVER_IP_ADDRESS, NMI_M2M_AP_WPA_KEY};



#define AP_LIST													\
{																\
	{DEFAULT_SSID, (uint8*)DEFAULT_KEY,	 DEFAULT_AUTH },			\
	{"DEMO_AP1", (uint8*)"",             M2M_WIFI_SEC_OPEN},	\
	{"DEMO_AP2", (uint8*)&gstrWepParams, M2M_WIFI_SEC_WEP},		\
	{"DEMO_AP3", (uint8*)"12345678",     M2M_WIFI_SEC_WPA_PSK},	\
	{"DEMO_AP4", (uint8*)&gstrCred1x,    M2M_WIFI_SEC_802_1X},	\
	/* Add more access points to the scan list here... */		\
}	

	


#ifdef LIST_CONNECT
#define M2M_AP_LIST_SZ (sizeof(gastrPreferredAPList)/sizeof(tstrM2mAp))
#endif

/*******************************
* PS_SERVER APP CONFIGURATIONS *
********************************/
#define M2M_SERVER_CHANNEL		M2M_WIFI_CH_6

#ifdef _PS_SERVER_
#ifdef _PS_CLIENT_
#error "You can't define _PS_SERVER_ & _PS_CLIENT_ together"
#endif /* _PS_CLIENT_ */
#endif /* _PS_SERVER_ */

/******************************
* UDP TEST APP CONFIGURATIONS *
*******************************/

#ifdef UDP_TEST
extern uint8 gbUdpTestActive;
#ifdef LED_BTN_DEMO
#undef LED_BTN_DEMO
#endif
#ifdef _STATIC_PS_
#error "please undef _STATIC_PS_"
#endif
#ifdef _DYNAMIC_PS_
#error "please undef _DYNAMIC_PS_"
#endif
#define UDP_SERVER_PORT				9000
#define UDP_CLIENT_PORT				9002
#define TCP_SERVER_PORT				20000

#ifdef WIN32
#define TEST_PACKET_COUNT			250
#else
#define TEST_PACKET_COUNT			1000
#endif
#endif /* UDP_TEST */


/**********************************
* UDP LED-TEMP APP CONFIGURATIONS *
**********************************/
#ifdef UDP_DEMO

#ifdef LED_BTN_DEMO
#undef LED_BTN_DEMO
#endif
#endif /* UDP_DEMO */

#define DEMO_PRODUCT_NAME					"TempSensor"
#define BROADCAST							"255.255.255.255"
#define DEMO_SERVER_PORT					(6666)
#define DEMO_REPORT_INTERVAL				(1000)
#define CLIENT_KEEPALIVE					"0001,"DEMO_PRODUCT_NAME","
#define CLIENT_REPORT						"0002,"DEMO_PRODUCT_NAME",00.0,0,"
#define SERVER_REPORT						"0002,"DEMO_PRODUCT_NAME","


#if defined SAMW25_XPLAINED_PRO
#define CONF_DEMO_LED_PIN                   PIN_PA10
#elif defined SAMD21_XPLAINED_PRO
#define CONF_DEMO_LED_PIN                   PIN_PB12
#else
//#error Please define CONF_DEMO_LED_PIN
#endif


#define CLIENT_RPOS_TEMP					(sizeof(CLIENT_REPORT) - 8)
#define CLIENT_RPOS_LED						(sizeof(CLIENT_REPORT) - 3)
#define SERVER_RPOS_LED						(sizeof(SERVER_REPORT) - 1)


/***************************
* STATIC IP CONFIGURATIONS *
****************************/
#ifdef CONF_STATIC_IP_ADDRESS
#define STATIC_IP_ADDRESS				"192.168.1.4"
#define DNS_ADDRESS						"192.168.1.1"
#define DEFAULT_GATEWAY_ADDRESS			"192.168.1.1"
#define SUBNET_MASK						"255.255.255.0"
#endif /* CONF_STATIC_IP_ADDRESS */


#define WPS_DISABLED				0
#define WPS_ENABLED					1

#define PROV_DISABLED				0
#define PROV_ENABLED				1


#define PS_WAKE       				0
#define PS_SLEEP      				1
#define PS_REQ_SLEEP				3
#define PS_REQ_CHECK				4
#define PS_SLEEP_TIME_MS			30000 /*30sec*/

#define TEST_BUS_BUF_SZ				100U
#define SHARED_PKT_MEM_BASE			0xd0000UL
#define DEFAULT_PREFIX				"NMI"

#define M2M_CLIENT_CMD_WAKE_FIRMWARE    ((uint8)10)
#define M2M_CLIENT_CMD_LED_ON 			((uint8)12)
#define M2M_CLIENT_CMD_LED_OFF 			((uint8)11)

/**/
#define M2M_CLIENT_RESP_MOVEMENT 		((uint8)20)
#define M2M_CLIENT_RESP_BTN1_PRESS 		((uint8)21)
#define M2M_CLIENT_RESP_BTN2_PRESS 		((uint8)22)
#define M2M_CLIENT_CHECK_STATE			((uint8)0)

#ifdef _STATIC_PS_
#ifdef _DYNAMIC_PS_
#error "you can't define both power save modes together"
#endif
#endif

#endif
