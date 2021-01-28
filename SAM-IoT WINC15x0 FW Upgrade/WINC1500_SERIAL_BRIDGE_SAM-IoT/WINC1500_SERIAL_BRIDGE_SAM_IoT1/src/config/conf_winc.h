/**
 *
 * \file
 *
 * \brief WINC1500 configuration.
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
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

#define CONF_WINC_PIN_RESET				PIN_PB10 //PIN_PB06
#define CONF_WINC_PIN_CHIP_ENABLE		PIN_PB11 //PIN_PB05
#define CONF_WINC_PIN_WAKE				PIN_PA19 //PIN_PB07

/*
   ---------------------------------
   ---------- SPI settings ---------
   ---------------------------------
*/

#define CONF_WINC_USE_SPI				(1)

/** SPI pin and instance settings. */
#define CONF_WINC_SPI_MODULE			SERCOM4 //EXT1_SPI_MODULE
#define CONF_WINC_SPI_SERCOM_MUX		SPI_SIGNAL_MUX_SETTING_D //EXT1_SPI_SERCOM_MUX_SETTING
#define CONF_WINC_SPI_PINMUX_PAD0		PINMUX_PA12D_SERCOM4_PAD0// EXT1_SPI_SERCOM_PINMUX_PAD0 /* in */
#define CONF_WINC_SPI_PINMUX_PAD1		PINMUX_PA13D_SERCOM4_PAD1 //PINMUX_UNUSED /* cs driven from software */
#define CONF_WINC_SPI_PINMUX_PAD2		PINMUX_UNUSED //EXT1_SPI_SERCOM_PINMUX_PAD2 /* out */
#define CONF_WINC_SPI_PINMUX_PAD3		PINMUX_PA15D_SERCOM4_PAD3 //EXT1_SPI_SERCOM_PINMUX_PAD3 /* sck */
#define CONF_WINC_SPI_CS_PIN			PIN_PA14 //EXT1_PIN_SPI_SS_0

#define CONF_WINC_SPI_MISO				PIN_PA15 //EXT1_PIN_SPI_MISO
#define CONF_WINC_SPI_MOSI				PIN_PA12 //EXT1_PIN_SPI_MOSI
#define CONF_WINC_SPI_SCK				PIN_PA13 //EXT1_PIN_SPI_SCK
#define CONF_WINC_SPI_SS				PIN_PA14 //EXT1_PIN_SPI_SS_0

/** SPI interrupt pin. */
#define CONF_WINC_SPI_INT_PIN			PIN_PA18A_EIC_EXTINT2 //EXT1_IRQ_PIN
#define CONF_WINC_SPI_INT_MUX			PINMUX_PA18A_EIC_EXTINT2 //EXT1_IRQ_MUX
#define CONF_WINC_SPI_INT_EIC			(2) //(4)

/** SPI clock. */
#define CONF_WINC_SPI_CLOCK				(12000000)

/*
   ---------------------------------
   --------- Debug Options ---------
   ---------------------------------
*/

#define CONF_WINC_DEBUG					(1)
#define CONF_WINC_PRINTF				printf

#ifdef __cplusplus
}
#endif

#endif /* CONF_WINC_H_INCLUDED */
