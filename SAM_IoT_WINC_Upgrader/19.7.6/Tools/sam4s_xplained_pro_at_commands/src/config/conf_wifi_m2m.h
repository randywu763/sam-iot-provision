/**
 *
 * \file
 *
 * \brief Wifi M2M configuration
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
#ifndef CONF_WIFI_M2M_H_INCLUDED
#define CONF_WIFI_M2M_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"

#define NM_LEVEL_INTERRUPT

/*
   ---------------------------------
   ---------- PIN settings ---------
   ---------------------------------
*/

#define RESET_PIN						IOPORT_CREATE_PIN(PIOA, 24)
#define CHIP_ENABLE_PIN					IOPORT_CREATE_PIN(PIOA, 6) // unused
#define WAKE_PIN						IOPORT_CREATE_PIN(PIOA, 25)
#define DEBUG1_PIN						IOPORT_CREATE_PIN(PIOC, 26)
#define DEBUG2_PIN						IOPORT_CREATE_PIN(PIOC, 27)
#define DEBUG3_PIN						IOPORT_CREATE_PIN(PIOC, 19)
#define DEBUG4_PIN						IOPORT_CREATE_PIN(PIOC, 22)

#define CONF_WIFI_M2M_DEVICE_NAME		"NMC1500"

#define CONF_WINC_USE_SPI				(1)

#define USE_OLD_SPI_SW
//#define USE_I2C							(1)
//#define LOW_DELAY
#define DMA_SPI
/*
   ---------------------------------
   ---------- SPI settings ---------
   ---------------------------------
*/

/** SPI pin and instance settings. */
#define CONF_WIFI_NC_SPI				SPI
#define CONF_WIFI_NC_SPI_ID				ID_SPI
#define CONF_WIFI_NC_SPI_MISO_GPIO		SPI_MISO_GPIO
#define CONF_WIFI_NC_SPI_MISO_FLAGS		SPI_MISO_FLAGS
#define CONF_WIFI_NC_SPI_MOSI_GPIO		SPI_MOSI_GPIO
#define CONF_WIFI_NC_SPI_MOSI_FLAGS		SPI_MOSI_FLAGS
#define CONF_WIFI_NC_SPI_CLK_GPIO		SPI_SPCK_GPIO
#define CONF_WIFI_NC_SPI_CLK_FLAGS		SPI_SPCK_FLAGS
#define CONF_WIFI_NC_SPI_CS_GPIO		SPI_NPCS0_GPIO
#define CONF_WIFI_NC_SPI_CS_FLAGS		PIO_OUTPUT_1|PIO_PULLUP
#define CONF_WIFI_NC_SPI_NPCS			(0)

/** SPI delay before SPCK and between consecutive transfer. */
#define CONF_WIFI_NC_SPI_DLYBS			(1)
#define CONF_WIFI_NC_SPI_DLYBCT 		(0)

/** SPI interrupt pin. */
#define CONF_WIFI_NC_SPI_INT_PIN		IOPORT_CREATE_PIN(PIOA, 1)
#define CONF_WIFI_NC_SPI_INT_PIO		PIOA
#define CONF_WIFI_NC_SPI_INT_PIO_ID		ID_PIOA
#define CONF_WIFI_NC_SPI_INT_MASK		(1 << 1)
#define CONF_WIFI_NC_SPI_INT_PRIORITY	(0)

/** Clock polarity & phase. */
#define CONF_WIFI_NC_SPI_POL			(0)
#define CONF_WIFI_NC_SPI_PHA			(1)

/** SPI clock. */
#define CONF_WIFI_NC_SPI_CLOCK			(40000000)


/*
   ---------------------------------
   --------- Debug options ---------
   ---------------------------------
*/

#define CONF_PERIPH						(1)

#define CONF_WINC_DEBUG					(1)
//#define CONF_WINC_PRINTF				printf
#define CONF_WINC_PRINTF				at_sb_printf

#ifdef __cplusplus
}
#endif

#endif /* CONF_WIFI_M2M_H_INCLUDED */
