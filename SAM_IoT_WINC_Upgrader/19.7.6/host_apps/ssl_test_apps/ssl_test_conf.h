#ifndef __MAIN_H__
#define __MAIN_H__

/*******************************************************************************************
INCLUDES
*******************************************************************************************/

#include <string.h>
#include "common/include/nm_common.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "driver/include/m2m_wifi.h"

#ifdef __ATECC508__
#include "ecc_crypto_ecc508.h"
#endif

/*******************************************************************************************
MACROS
*******************************************************************************************/

/*
	TEST APPLICATIONS
*/
#define TEST_APP_SSL_UPLOAD				1		// Normal SSL Upload
#define TEST_APP_SSL_DOWNLOAD			2		// HTTPS File Download
#define TEST_APP_HTTPS_PROV				3
#define TEST_APP_HTTP_SERVER			4
#define TEST_APP_WPA_ENT_CONN			5

#define TEST_APP						TEST_APP_SSL_DOWNLOAD


/*
	WPA-PERSONAL AP CONFIGURATIONS
*/
#define WLAN_WPA_AP_SSID				"IoT_WIFI"
#define WLAN_WPA_AP_SEC					M2M_WIFI_SEC_WPA_PSK
#define WLAN_WPA_AP_PWD					"12345678"

/*
	WPA-ENTERPRISE AP CONFIGURATIONS
*/
#define SSL_APP_802_1X_USR				"WINC1500"
#define SSL_APP_802_1X_PWD				"WINC1500"
#define SSL_APP_1X_CRED					{SSL_APP_802_1X_USR, SSL_APP_802_1X_PWD}
#define WLAN_ENT_AP_SSID				"IoT_8021X"
#define WLAN_ENT_AP_SEC					M2M_WIFI_SEC_802_1X
#define WLAN_ENT_AP_PWD					(uint8*)&gstrCred1x

/*
	WINC AP Configurations
*/
#define SSL_APP_AP_SSID					"WINC1500_HOTSPOT"
#define SSL_APP_AP_PWD					"ozmodevices"
#define SSL_APP_AP_KEY_SZ				sizeof(SSL_APP_AP_PWD) - 1
#define SSL_APP_AP_CHANNEL				M2M_WIFI_CH_11
#define SSL_APP_AP_SEC					M2M_WIFI_SEC_WPA_PSK
#define SSL_APP_AP_SSID_MODE			SSID_MODE_VISIBLE

/*
	WINC PROVISIONING HTTP SERVER 
	CONFIGURATIONS
*/
#define SSL_APP_HTTP_DNS_NAME			"prov.winc.atmel.com"
#define SSL_APP_PROV_HTTPS_URL			"https://" SSL_APP_HTTP_DNS_NAME
#define SSL_APP_PROV_HTTP_URL			"http://" SSL_APP_HTTP_DNS_NAME
#define SSL_APP_PROV_HTTPS_IP			{192, 168, 1, 1}
#define SSL_APP_PROV_HTTP_REDIRECT		1


#define SSL_APP_WINC_HOTSPOT_CONF	\
{\
	SSL_APP_AP_SSID,			\
	SSL_APP_AP_CHANNEL,			\
	0, SSL_APP_AP_KEY_SZ, {0},	\
	(uint8)SSL_APP_AP_SEC,		\
	SSL_APP_AP_SSID_MODE,		\
	SSL_APP_PROV_HTTPS_IP,		\
	SSL_APP_AP_PWD				\
}


/*
	HTTP CLIENT DOWNLOAD REQUEST
*/
#define HTTP_GET_REQ	\
	"GET /upload.txt HTTP/1.1\r\n"\
	"Host: %s\r\n"\
	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"\
	"User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.116 Safari/537.36\r\n"\
	"\r\n"


/*
	HTTP CLIENT UPLOAD REQUEST
*/
#define HTTP_POST_REQ	\
"PUT /upload.txt HTTP/1.1\r\n"\
"Host: %s\r\n"\
"Content-Type: application/pdf\r\n"\
"Content-Length: 1048576\r\n"\
"\r\n"


/*******************************************************************************************
DTAT TYPES
*******************************************************************************************/

/*!
*/
typedef enum{
	APP_WIFI_MODE_HTTP_PROV,
	APP_WIFI_MODE_HTTPS_PROV,
	APP_WIFI_MODE_WPA,
	APP_WIFI_MODE_WPA_ENT,
	APP_WIFI_MODE_HOTSPOT
}tenuAppWifiConnMode;

/*******************************************************************************************
FUNCTION PROTOTYPES
*******************************************************************************************/

void	WifiCallback(uint8 u8MsgType, void * pvMsg);
void	SslUploadAppRun(void);
void	SslDownloadAppRun(tenuAppWifiConnMode enuConnMode);
void	httpProvAppRun(tenuAppWifiConnMode enuConnMode);
void	AppWifiConnect(tenuAppWifiConnMode enuConnMode);
void	httpServerAppRun(void);

int		ssl_main_app(void);

/*******************************************************************************************
EXTERNALS
*******************************************************************************************/

extern volatile uint32			ms_ticks;
extern tstr1xAuthCredentials	gstrCred1x;
extern uint8					gau8MacAddr[];
extern volatile uint8			gu8IsWiFiConnected;

#endif /* __MAIN_H__ */
