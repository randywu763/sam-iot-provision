/*!
@file	\
	ssl_main_app.c
	
@brief	MAIN SSL TEST FILE
*/

/*******************************************************************************************
INCLUDES
*******************************************************************************************/

#include <string.h>
#include "common/include/nm_common.h"
#include "driver/include/m2m_wifi.h"
#include "driver/include/m2m_ssl.h"
#include "driver/source/m2m_hif.h"
#include "ssl_test_conf.h"

/*******************************************************************************************
MACROS
*******************************************************************************************/

tstr1xAuthCredentials	gstrCred1x		= SSL_APP_1X_CRED;
tstrM2MAPConfig			gstrApConf		= SSL_APP_WINC_HOTSPOT_CONF;
uint8					gau8MacAddr[]	= {0};
char					gacHttpProvDN[]	= SSL_APP_PROV_HTTP_URL;
char					gacHttpsProvDN[]= SSL_APP_PROV_HTTPS_URL;
volatile uint8			gu8IsWiFiConnected;


/***********************************************************/
void AppWifiConnect(tenuAppWifiConnMode enuConnMode)
{
	switch(enuConnMode)
	{
		case APP_WIFI_MODE_HTTPS_PROV:
			m2m_wifi_start_provision_mode(&gstrApConf, gacHttpsProvDN, SSL_APP_PROV_HTTP_REDIRECT);
		break;
		
		case APP_WIFI_MODE_HTTP_PROV:
			m2m_wifi_start_provision_mode(&gstrApConf, gacHttpProvDN, SSL_APP_PROV_HTTP_REDIRECT);
		break;
		
		case APP_WIFI_MODE_WPA:
			m2m_wifi_connect((char*)WLAN_WPA_AP_SSID, sizeof(WLAN_WPA_AP_SSID), WLAN_WPA_AP_SEC, WLAN_WPA_AP_PWD, M2M_WIFI_CH_ALL);
		break;
		
		case APP_WIFI_MODE_WPA_ENT:
			m2m_wifi_connect((char*)WLAN_ENT_AP_SSID, sizeof(WLAN_ENT_AP_SSID), WLAN_ENT_AP_SEC, WLAN_ENT_AP_PWD, M2M_WIFI_CH_ALL);
		break;
		
		case APP_WIFI_MODE_HOTSPOT:
			m2m_wifi_enable_ap(&gstrApConf);
		break;
	}
}

/***********************************************************/
int ssl_main_app(void)
{
	sint8				ret;
	tstrWifiInitParam	param;
	ATCAIfaceCfg tls_atecc508a;
	aws_data_t cryptodata;

	/* Initialize the BSP. */
	nm_bsp_init();
	
	/*
		WINC Driver Initialization
	*/
	gu8IsWiFiConnected = M2M_WIFI_DISCONNECTED;	
	m2m_memset((uint8*)&param, 0, sizeof(param));
	param.pfAppWifiCb = WifiCallback;
	ret = m2m_wifi_init(&param);
	if (M2M_SUCCESS != ret)
	{
		M2M_ERR("Driver Init Failed <%d>\n",ret);
		for (;;)
		{
		}
	}

	m2m_ssl_init(NULL);
#ifdef __ATECC508__
	// Initialize the ATECC508A
	tls_atecc508a = cfg_ateccx08a_i2c_default;
	if(eccInit(&tls_atecc508a) == M2M_SUCCESS) {
		eccReadAWSData(&cryptodata);
		m2m_ssl_set_active_ciphersuites(SSL_NON_ECC_CIPHERS_AES_128 | SSL_ECC_ALL_CIPHERS);
	}
#endif
	
#if  TEST_APP == TEST_APP_SSL_UPLOAD
	SslUploadAppRun();
#elif TEST_APP == TEST_APP_SSL_DOWNLOAD
	SslDownloadAppRun(APP_WIFI_MODE_WPA);
#elif TEST_APP == TEST_APP_HTTPS_PROV
	httpProvAppRun(APP_WIFI_MODE_HTTPS_PROV);
#elif TEST_APP == TEST_APP_WPA_ENT_CONN
	SslDownloadAppRun(APP_WIFI_MODE_WPA_ENT);
#else
	httpServerAppRun();
#endif

	return 0;
}