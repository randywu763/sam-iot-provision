/**
 * \file
 *
 * \brief MAIN configuration.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define SSL_ENABLE 1
#define SKIP_TIME  1

/** security information for Wi-Fi connection */
#define MAIN_WLAN_DEVICE_NAME           "DEMO_AP" /**< Destination SSID */
#define MAIN_WLAN_PSK                   "12345678" /**< Password for Destination SSID */

#define MAIN_WIFI_M2M_BUFFER_SIZE		  1460
#define MAIN_WIFI_M2M_SERVER_NAME         "s3.amazonaws.com"
#define MAIN_WIFI_M2M_SERVER_PORT         (80)
#define MAIN_WIFI_M2M_SERVER_PORT_SSL     (443)
#define MAIN_WIFI_M2M_REPORT_INTERVAL     (1000)

typedef enum
{
	PUT = 0,
	GET	
}request_type_t;

typedef enum
{
	APP_IDLE = 0,
	APP_SCAN,
	APP_WIFI_CONNECT,
	APP_GET_TIME,
	APP_DNS_QUERY,
	APP_TCP_CONNECT,
	APP_UPLOAD,
	APP_DOWNLOAD,
	APP_TCP_DISCONNECT,
	APP_WIFI_DISCONNECT
}app_states_t;


void demo_start(void);

#ifdef __cplusplus
}
#endif

#endif /* DEMO_H_INCLUDED */
