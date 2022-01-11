/**
 *
 * \file
 *
 * \brief Wifi NMI temperature sensor demo.
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
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

#ifndef IPERF_H_
#define IPERF_H_

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "bsp/include/nm_bsp.h"
#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"
#include "conf_winc.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define TEST_STATE_UDP_TX			1
#define TEST_STATE_UDP_RX			2
#define TEST_STATE_TCP_TX			3
#define TEST_STATE_TCP_RX			4

#define IPERF_TX_BUFFER_SIZE		1400
#define IPERF_RX_BUFFER_SIZE		1600
#define IPERF_BUFFER_SIZE			((IPERF_RX_BUFFER_SIZE > IPERF_TX_BUFFER_SIZE) ? IPERF_RX_BUFFER_SIZE : IPERF_TX_BUFFER_SIZE)

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

// used to reference the 4 byte ID number Iperf place in UDP datagrams
typedef struct UDP_datagram
{
    uint32_t id;
    uint32_t tv_sec;
    uint32_t tv_usec;
} UDP_datagram;

/*
 * The client_hdr structure is sent from clients
 * to servers to alert them of things that need
 * to happen. Order must be preserved in all
 * future releases for backward compatibility.
 * 1.7 has flags, numThreads, mPort, and bufferlen
 */
typedef struct client_hdr {

    /*
     * flags is a bitmap for different options
     * the most significant bits are for determining
     * which information is available. So 1.7 uses
     * 0x80000000 and the next time information is added
     * the 1.7 bit will be set and 0x40000000 will be
     * set signifying additional information. If no
     * information bits are set then the header is ignored.
     * The lowest order differentiates between dualtest and
     * tradeoff modes, whether the speaker needs to start
     * immediately or after the audience finishes.
     */
    int32_t flags;
    int32_t numThreads;
    int32_t mPort;
    int32_t bufferlen;
    int32_t mWinBand;
    int32_t mAmount;
} client_hdr;

/*
 * The server_hdr structure facilitates the server
 * report of jitter and loss on the client side.
 * It piggy_backs on the existing clear to close
 * packet.
 */
typedef struct server_hdr {
    /*
     * flags is a bitmap for different options
     * the most significant bits are for determining
     * which information is available. So 1.7 uses
     * 0x80000000 and the next time information is added
     * the 1.7 bit will be set and 0x40000000 will be
     * set signifying additional information. If no
     * information bits are set then the header is ignored.
     */
    int32_t flags;
    int32_t total_len1;
    int32_t total_len2;
    int32_t stop_sec;
    int32_t stop_usec;
    int32_t error_cnt;
    int32_t outorder_cnt;
    int32_t datagrams;
    int32_t jitter1;
    int32_t jitter2;

} server_hdr;

typedef enum{
	MODE_TCP_CLIENT,
	MODE_TCP_SERVER,
	MODE_UDP_CLIENT,
	MODE_UDP_SERVER,
	MODE_TLS_CLIENT,
}tenuNMI_IperfMode;

typedef struct{
	tenuNMI_IperfMode	operating_mode;
	uint32_t			packets_to_send;
	uint32_t			data_rate;
	uint32_t			packet_len;
	uint8_t				ip[4];
	uint16_t			port;
	uint8_t				tls;
}tstrIperfInit;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

NMI_API void IperfInit(void);

NMI_API sint8 IperfSocketClose(SOCKET sock);

NMI_API sint8 IperfRemoteSocketClose(void);

NMI_API void IperfPrintStats(SOCKET sock);

NMI_API void IperfUpdate(void);

NMI_API sint8 IperfCreate(tstrIperfInit* pstrIperfInit, bool bIsPaused);

NMI_API sint8 IperfStart(void);

NMI_API sint8 IperfStop(SOCKET sock);

#endif /* IPERF_H_ */
