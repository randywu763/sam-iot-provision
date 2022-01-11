
/*******************************************************************************************
INCLUDES
*******************************************************************************************/

#include "asf.h"
#include "http.h"
#include "ssl_test_conf.h"

#if TEST_APP == TEST_APP_HTTPS_PROV


/***********************************************************/
void WifiCallback(uint8 u8MsgType, void * pvMsg)
{
	if(u8MsgType == M2M_WIFI_RESP_CON_STATE_CHANGED)
	{
		tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged*) pvMsg;
		M2M_INFO("Wifi State :: %s ::\n", pstrWifiState->u8CurrState ? "CONNECTED" : "DISCONNECTED");
		if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED)
		{

		}
		else if(pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED)
		{
			gu8IsWiFiConnected = M2M_WIFI_DISCONNECTED;
		}
	}
	else if(u8MsgType == M2M_WIFI_REQ_DHCP_CONF)
	{
		gu8IsWiFiConnected = M2M_WIFI_CONNECTED;
		tstrM2MIPConfig* pstrM2MIpConfig = (tstrM2MIPConfig*) pvMsg;
		uint8 *pu8IPAddress = (uint8*) &pstrM2MIpConfig->u32StaticIP;
		
		M2M_INFO("DHCP IP Address :: %u.%u.%u.%u ::\n", pu8IPAddress[0], pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
	}
	else if(u8MsgType == M2M_WIFI_RESP_GET_SYS_TIME)
	{
		tstrSystemTime *pstrTime = (tstrSystemTime*)pvMsg;
		M2M_INFO("Time Of Day\n\t%d/%02d/%d %02d:%02d:%02d GMT\n",
		pstrTime->u8Month, pstrTime->u8Day, pstrTime->u16Year,
		pstrTime->u8Hour, pstrTime->u8Minute, pstrTime->u8Second);

	}
	else if(u8MsgType == M2M_WIFI_RESP_PROVISION_INFO)
	{
		tstrM2MProvisionInfo	*pstrProvInfo = (tstrM2MProvisionInfo*)pvMsg;
		if(pstrProvInfo->u8Status == M2M_SUCCESS)
		{
			m2m_wifi_connect((char*)pstrProvInfo->au8SSID, (uint8)m2m_strlen(pstrProvInfo->au8SSID), pstrProvInfo->u8SecType, pstrProvInfo->au8Password, M2M_WIFI_CH_ALL);

			M2M_INFO("PROV SSID : %s\n",pstrProvInfo->au8SSID);
			M2M_INFO("PROV PSK  : %s\n",pstrProvInfo->au8Password);
		}
		else
		{
			M2M_ERR("Provisioning Failed\n");
			AppWifiConnect(APP_WIFI_MODE_HTTPS_PROV);
		}
	}
}

/***********************************************************/
void httpProvAppRun(tenuAppWifiConnMode enuMode)
{
	AppWifiConnect(enuMode);
	
	for (;;)
	{
		while(m2m_wifi_handle_events(NULL) != M2M_SUCCESS)
		{
		}
	}
}
#endif