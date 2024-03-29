/**
 *
 * \file
 *
 * \brief WINC1500 configuration.
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

#ifndef CONF_WINC_H_INCLUDED
#define CONF_WINC_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"

/*
   ---------------------------------
   ---------- PIN settings ---------
   ---------------------------------
*/

#define CONF_WINC_PIN_RESET				PIN_PB06
#define CONF_WINC_PIN_CHIP_ENABLE		PIN_PB05
#define CONF_WINC_PIN_WAKE				PIN_PB07

/*
   ---------------------------------
   ---------- SPI settings ---------
   ---------------------------------
*/

#define CONF_WINC_USE_SPI				(1)
#define CONF_WINC_USE_EDGE_INTERRUPT	(1)

/** SPI pin and instance settings. */
#define CONF_WINC_SPI_MODULE			EXT1_SPI_MODULE
#define CONF_WINC_SPI_SERCOM_MUX		EXT1_SPI_SERCOM_MUX_SETTING
#define CONF_WINC_SPI_PINMUX_PAD0		EXT1_SPI_SERCOM_PINMUX_PAD0 /* in */
#define CONF_WINC_SPI_PINMUX_PAD1		PINMUX_UNUSED /* cs driven from software */
#define CONF_WINC_SPI_PINMUX_PAD2		EXT1_SPI_SERCOM_PINMUX_PAD2 /* out */
#define CONF_WINC_SPI_PINMUX_PAD3		EXT1_SPI_SERCOM_PINMUX_PAD3 /* sck */
#define CONF_WINC_SPI_CS_PIN			EXT1_PIN_SPI_SS_0

#define CONF_WINC_SPI_MISO				EXT1_PIN_SPI_MISO
#define CONF_WINC_SPI_MOSI				EXT1_PIN_SPI_MOSI
#define CONF_WINC_SPI_SCK				EXT1_PIN_SPI_SCK
#define CONF_WINC_SPI_SS				EXT1_PIN_SPI_SS_0

/* SPI DMA Configuration */
#define CONF_WINC_SPI_DMA
#define CONF_WINC_SPI_DMA_PERIPHERAL_TRIGGER_TX		EXT1_SPI_SERCOM_DMAC_ID_TX
#define CONF_WINC_SPI_DMA_PERIPHERAL_TRIGGER_RX		EXT1_SPI_SERCOM_DMAC_ID_RX
/*
   ---------------------------------
   ---------- I2C settings ---------
   ---------------------------------
*/
#define CONF_WIFI_M2M_I2C               (0) 

#define CONF_WINC_I2C_SCL               EXT1_PIN_I2C_SCL
#define CONF_WINC_I2C_SDA               EXT1_PIN_I2C_SDA

/** WINC interrupt pin. */
#define CONF_WINC_SPI_INT_PIN			EXT1_IRQ_PIN
#define CONF_WINC_SPI_INT_MUX			EXT1_IRQ_MUX
#define CONF_WINC_SPI_INT_EIC			(4)

/** SPI clock. */
#define CONF_WINC_SPI_CLOCK				(10000000)

/*
   ---------------------------------
   --------- Debug Options ---------
   ---------------------------------
*/

#define CONF_WINC_DEBUG					(1)
#define CONF_WINC_PRINTF				printf
//#define CONF_WINC_PRINTF				at_sb_printf

#ifdef __cplusplus
}
#endif

#endif /* CONF_WINC_H_INCLUDED */
