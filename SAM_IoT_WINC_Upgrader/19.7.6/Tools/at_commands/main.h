/*
 * main.h
 *
 * Created: 10/23/2014 11:33:38 AM
 *  Author: aali
 */ 


#ifndef MAIN_H_
#define MAIN_H_
/*
 * Include header files for all drivers that have been used from C Standard Libs
 */
#include <string.h>

#include "m2m_test_config.h"
#include "common/include/nm_common.h"
#include "bsp/include/nm_bsp_samd21_app.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "driver/include/m2m_wifi.h"
#include "driver/include/m2m_periph.h"
#include "driver/include/m2m_ota.h"
#include "driver/include/m2m_ssl.h"
#include "led_btn_demo/led_btn_demo.h"
#include "udp_demo.h"
#include "socket/include/socket.h"
#include "at_cmd_handler/at_cmd_Handler.h"
#include "http_client/http.h"
#include "http_client/http_client.h"
#include "ecc_crypto_ecc508.h"

/*!< used to display received command
 * after parsing and before it.
 */
#define DEBUG_CMD

#define ENABLED                 1
#define DISABLED                0

#define SET_VAR_TO_1(var)       (var = 1)
#define RESET_VAR_TO_0(var)     (var = 0)
#define IS_VAR_IS_L1(var)       (var > 0)
#define IS_VAR_IS_L0(var)       (var == 0)

#define ESCAPE_CHAR             27
#define MQTT_PORT               8883

enum{
    SERVICE_IS_STOPPED = 0,
    SERVICE_IS_RUNNING,
    SERVICE_DEV_IS_CONNECTED,
    SERVICE_IS_UNDEFINED = 0xFF,
};

typedef struct connect_status{
    uint8 u8Sta_status;
    uint8 u8Ap_status;
    uint8 u8Prov_status;
    uint8 u8Wps_status;
}tstr_test_conn_status;

typedef struct {
    uint8 u8Echo_status;
    /*!< Must be disabled if automated transfer is used
     */
    uint8 u8Restart_app;
    /*!< Has the value of the application to restart or not
     * when this variable will auto reset after execution
     */
    uint8 u8Display_hint;
    /*!< If this variable has been set, this means if something wrong happened
     * This will display a hint about the command
     */
}tstr_app_config;

#define MAIN_WIFI_M2M_BUFFER_SIZE   1460

SockConnInfo    *first_SockConn;
uint8           sCID[MAX_SOCKET];
uint8_t         gau8SocketTestBuffer[MAIN_WIFI_M2M_BUFFER_SIZE];

tstrM2MIPConfig StaticIPcfg;
uint8           use_static_IP;

void m2m_wifi_socket_handler(SOCKET sock, uint8_t u8Msg, void *pvMsg);
void m2m_wifi_resolve_handler(uint8* pu8DomainName, uint32 u32ServerIP);
void pingCb(uint32, uint32, uint8);
sint8 winc_reset(uint8 reinit, uint8 bConnect, uint8 mode);
#endif /* MAIN_H_ */