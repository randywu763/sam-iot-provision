/**
 *
 * \file
 *
 * \brief This module contains SAMD21 BSP APIs implementation.
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

#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "asf.h"
#include "conf_wifi_m2m.h"


static tpfNmBspIsr gpfIsr;


static void chip_isr(uint32_t id, uint32_t mask)
{
	if ((id == CONF_WIFI_NC_SPI_INT_PIO_ID) && (mask == CONF_WIFI_NC_SPI_INT_MASK)) {
		if (gpfIsr) {
			gpfIsr();
		}
	}
}
void nm_bsp_reset(void)
{
	pio_set_pin_low(CHIP_ENABLE_PIN);
	pio_set_pin_low(RESET_PIN);
	nm_bsp_sleep(100);
	pio_set_pin_high(CHIP_ENABLE_PIN);
	nm_bsp_sleep(100);
	pio_set_pin_high(RESET_PIN);
	nm_bsp_sleep(100);
}

static void init_chip_pins(void)
{
#ifdef __SAM4SD32C__
	pio_configure_pin(RESET_PIN, PIO_TYPE_PIO_OUTPUT_0|PIO_PULLUP);
	pio_configure_pin(CHIP_ENABLE_PIN, PIO_TYPE_PIO_OUTPUT_0|PIO_PULLUP);
	pio_configure_pin(WAKE_PIN, PIO_TYPE_PIO_OUTPUT_0|PIO_PULLUP);
	pio_configure_pin(CONF_WIFI_NC_SPI_CS_GPIO, PIO_DEFAULT|PIO_PULLUP);
	pio_set_pin_high(CONF_WIFI_NC_SPI_CS_GPIO);
#else
	pio_configure_pin(RESET_PIN, PIO_TYPE_PIO_OUTPUT_0);
	pio_configure_pin(CHIP_ENABLE_PIN, PIO_TYPE_PIO_OUTPUT_0);
	pio_configure_pin(WAKE_PIN, PIO_TYPE_PIO_OUTPUT_0);
#endif
}

/*
*	@fn		nm_bsp_init
*	@brief	Initialize BSP
*	@return	0 in case of success and -1 in case of failure
*	@author	M.S.M
*	@date	11 July 2012
*	@version	1.0
*/
sint8 nm_bsp_init(void)
{
	gpfIsr = NULL;

	init_chip_pins();
	nm_bsp_reset();

	return 0;
}
/*
*	@fn		nm_bsp_sleep
*	@brief	Sleep in units of mSec
*	@param[IN]	u32TimeMsec
*				Time in milliseconds
*	@author	M.S.M
*	@date	28 OCT 2013
*	@version	1.0
*/
void nm_bsp_sleep(uint32 u32TimeMsec)
{
	while(u32TimeMsec--) {
		delay_ms(1);
	}
}

/*
*	@fn		nm_bsp_register_isr
*	@brief	Register interrupt service routine
*	@param[IN]	pfIsr
*				Pointer to ISR handler
*	@author	M.S.M
*	@date	28 OCT 2013
*	@sa		tpfNmBspIsr
*	@version	1.0
*/
void nm_bsp_register_isr(tpfNmBspIsr pfIsr)
{
	gpfIsr = pfIsr;
	
	/* Configure PGIO pin for interrupt from SPI slave, used when slave has data to send. */
	sysclk_enable_peripheral_clock(CONF_WIFI_NC_SPI_INT_PIO_ID);
	pio_configure_pin(CONF_WIFI_NC_SPI_INT_PIN, PIO_TYPE_PIO_INPUT);
	pio_pull_up(CONF_WIFI_NC_SPI_INT_PIO, CONF_WIFI_NC_SPI_INT_MASK, PIO_PULLUP);
//	pio_set_debounce_filter(CONF_WIFI_NC_SPI_INT_PIO, CONF_WIFI_NC_SPI_INT_MASK, 10);
	pio_handler_set_pin(CONF_WIFI_NC_SPI_INT_PIN, PIO_IT_LOW_LEVEL, chip_isr);
	pio_enable_interrupt(CONF_WIFI_NC_SPI_INT_PIO, CONF_WIFI_NC_SPI_INT_MASK);
	pio_handler_set_priority(CONF_WIFI_NC_SPI_INT_PIO, (IRQn_Type)CONF_WIFI_NC_SPI_INT_PIO_ID,
			CONF_WIFI_NC_SPI_INT_PRIORITY);
}
/*
*	@fn		nm_bsp_interrupt_ctrl
*	@brief	Enable/Disable interrupts
*	@param[IN]	u8Enable
*				'0' disable interrupts. '1' enable interrupts
*	@author	M.S.M
*	@date	28 OCT 2013
*	@version	1.0
*/
void nm_bsp_interrupt_ctrl(uint8 u8Enable)
{
	if (u8Enable) {
		pio_enable_interrupt(CONF_WIFI_NC_SPI_INT_PIO, CONF_WIFI_NC_SPI_INT_MASK);		
	}
	else {
		pio_disable_interrupt(CONF_WIFI_NC_SPI_INT_PIO, CONF_WIFI_NC_SPI_INT_MASK);
	}
}

