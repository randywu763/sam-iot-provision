/**
*
* \file
*
* \brief This module contains NMC1000 bus wrapper APIs implementation.
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

#include <stdio.h>
#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "asf.h"
#include "conf_wifi_m2m.h"

#define xSPI_ASSERT_CS()					{gpio_set_pin_low(CONF_WIFI_NC_SPI_CS_GPIO);}
#define xSPI_DEASSERT_CS()				{gpio_set_pin_high(CONF_WIFI_NC_SPI_CS_GPIO);}
	
	
#define SPI_ASSERT_CS()					do {p_pio->PIO_CODR = 1 << (CONF_WIFI_NC_SPI_CS_GPIO & 0x1F);}while(0)
	
#define SPI_DEASSERT_CS()				do {p_pio->PIO_SODR = 1 << (CONF_WIFI_NC_SPI_CS_GPIO & 0x1F);}while(0)


	

/** Pointer to Pdc data structure. */
static Pdc *g_p_spim_pdc;

#define SLAVE_ADDRESS 0x60

/* Number of times to try to send packet if failed. */
#define I2C_TIMEOUT 100

#define NM_BUS_MAX_TRX_SZ	4096

tstrNmBusCapabilities egstrNmBusCapabilities =
{
	NM_BUS_MAX_TRX_SZ
};

#ifdef USE_I2C
static sint8 nm_i2c_write(uint8 *b, uint16 sz)
{
	sint8 result = M2M_ERR_BUS_FAIL;

	return result;

}

static sint8 nm_i2c_read(uint8 *rb, uint16 sz)
{
	sint8 result = M2M_ERR_BUS_FAIL;

	return result;
}

static sint8 nm_i2c_write_special(uint8 *wb1, uint16 sz1, uint8 *wb2, uint16 sz2)
{
	static uint8 tmp[NM_BUS_MAX_TRX_SZ];
	m2m_memcpy(tmp, wb1, sz1);
	m2m_memcpy(&tmp[sz1], wb2, sz2);
	return nm_i2c_write(tmp, sz1+sz2);
}
#endif

#ifdef CONF_WINC_USE_SPI
Pio *p_pio;
sint8 nm_spi_rw(uint8 *pu8Mosi, uint8 *pu8Miso, uint16 u16Sz)
{
#ifdef DMA_SPI
	pdc_packet_t pdc_spi_tx_packet,pdc_spi_rx_packet;

	p_pio = (Pio *)((uint32_t)PIOA + (PIO_DELTA * (CONF_WIFI_NC_SPI_CS_GPIO >> 5)));

	pdc_spi_tx_packet.ul_addr = NULL;
	pdc_spi_rx_packet.ul_addr = NULL;
	pdc_spi_tx_packet.ul_size = u16Sz;
	pdc_spi_rx_packet.ul_size = u16Sz;
		
	if (pu8Mosi) {
		pdc_spi_tx_packet.ul_addr = (uint32_t)pu8Mosi;
	}

	if(pu8Miso) {
		pdc_spi_rx_packet.ul_addr = (uint32_t)pu8Miso;
	}

	pdc_tx_init(g_p_spim_pdc, &pdc_spi_tx_packet, NULL);
	pdc_rx_init(g_p_spim_pdc, &pdc_spi_rx_packet, NULL);

	/*Assert CS*/
	SPI_ASSERT_CS();
	/* Enable the RX and TX PDC transfer requests */
	g_p_spim_pdc->PERIPH_PTCR = PERIPH_PTCR_RXTEN|PERIPH_PTCR_TXTEN;

	/* Waiting transfer done*/
	while((CONF_WIFI_NC_SPI->SPI_SR & SPI_SR_RXBUFF) == 0);

	/*DEASSERT CS*/
	SPI_DEASSERT_CS();
	
	/* Disable the RX and TX PDC transfer requests */
	g_p_spim_pdc->PERIPH_PTCR = PERIPH_PTCR_TXTDIS|PERIPH_PTCR_RXTDIS;
	
	
	return M2M_SUCCESS;
		
#elif (defined LOW_DELAY)


uint8 u8Dummy = 0;
uint8 u8SkipMosi = 0, u8SkipMiso = 0;
uint8_t uc_pcs = 0;

p_pio = (Pio *)((uint32_t)PIOA + (PIO_DELTA * (CONF_WIFI_NC_SPI_CS_GPIO >> 5)));

if (!pu8Mosi) {
	pu8Mosi = &u8Dummy;
	u8SkipMosi = 1;
}
else if(!pu8Miso) {
	/*RX must be zero*/
	pu8Miso = &u8Dummy;
	u8SkipMiso = 1;
}
else {
	return M2M_ERR_BUS_FAIL;
}

/*TX path*/
if(!u8SkipMosi)
{
	SPI_ASSERT_CS();
	while (u16Sz--)
	{
		CONF_WIFI_NC_SPI->SPI_TDR = SPI_TDR_TD((uint16)*pu8Mosi);
		while (!(CONF_WIFI_NC_SPI->SPI_SR & SPI_SR_TDRE));
		pu8Mosi++;
	}
	SPI_DEASSERT_CS();
}
/*RX path*/
if(!u8SkipMiso)
{
	uc_pcs = 0;
	SPI_ASSERT_CS();
	while (u16Sz--)
	{
		
		CONF_WIFI_NC_SPI->SPI_TDR = SPI_TDR_TD((uint16)*pu8Mosi);
		while (!(CONF_WIFI_NC_SPI->SPI_SR & SPI_SR_TDRE));
		while (!(CONF_WIFI_NC_SPI->SPI_SR & (SPI_SR_RDRF)));
		*pu8Miso = (uint16_t) ((CONF_WIFI_NC_SPI->SPI_RDR) & SPI_RDR_RD_Msk);
		pu8Miso++;
	}
	SPI_DEASSERT_CS();
}
return M2M_SUCCESS;

#else
	uint8 u8Dummy = 0;
	uint8 u8SkipMosi = 0, u8SkipMiso = 0;
	uint16_t txd_data = 0;
	uint16_t rxd_data = 0;
	uint8_t uc_pcs = 0;
	pdc_packet_t pdc;

	p_pio = (Pio *)((uint32_t)PIOA + (PIO_DELTA * (CONF_WIFI_NC_SPI_CS_GPIO >> 5)));

	if (!pu8Mosi) {
		pu8Mosi = &u8Dummy;
		u8SkipMosi = 1;
	}
	else if(!pu8Miso) {
		pu8Miso = &u8Dummy;
		u8SkipMiso = 1;
	}
	else {
		return M2M_ERR_BUS_FAIL;
	}
	SPI_ASSERT_CS();
	while (u16Sz) {
		txd_data = *pu8Mosi;

		spi_write(CONF_WIFI_NC_SPI, txd_data, 0, 0);
		spi_read(CONF_WIFI_NC_SPI, &rxd_data, &uc_pcs);

		*pu8Miso = rxd_data;
			
		u16Sz--;
		if (!u8SkipMiso)
			pu8Miso++;
		if (!u8SkipMosi)
			pu8Mosi++;
	}
	SPI_DEASSERT_CS();

	return M2M_SUCCESS;
	#endif

}
#endif

/*
*	@fn		nm_bus_init
*	@brief	Initialize the bus wrapper
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*/
sint8 nm_bus_init(void *pvinit)
{
	sint8 result	= M2M_SUCCESS;
	#ifdef USE_I2C


	#elif CONF_WINC_USE_SPI

	/* Configure SPI pins. */
	gpio_configure_pin(CONF_WIFI_NC_SPI_MISO_GPIO, CONF_WIFI_NC_SPI_MISO_FLAGS);
	gpio_configure_pin(CONF_WIFI_NC_SPI_MOSI_GPIO, CONF_WIFI_NC_SPI_MOSI_FLAGS);
	gpio_configure_pin(CONF_WIFI_NC_SPI_CLK_GPIO, CONF_WIFI_NC_SPI_CLK_FLAGS);
	gpio_configure_pin(CONF_WIFI_NC_SPI_CS_GPIO, CONF_WIFI_NC_SPI_CS_FLAGS);
	SPI_DEASSERT_CS();

	spi_enable_clock(CONF_WIFI_NC_SPI);
	spi_disable(CONF_WIFI_NC_SPI);
	spi_reset(CONF_WIFI_NC_SPI);
	spi_set_master_mode(CONF_WIFI_NC_SPI);
	spi_disable_mode_fault_detect(CONF_WIFI_NC_SPI);
#ifdef	__SAM4SD32C__
	spi_set_fixed_peripheral_select(CONF_WIFI_NC_SPI);
#else
	spi_set_peripheral_chip_select_value(CONF_WIFI_NC_SPI, CONF_WIFI_NC_SPI_NPCS);
#endif

	spi_set_clock_polarity(CONF_WIFI_NC_SPI,
	CONF_WIFI_NC_SPI_NPCS, CONF_WIFI_NC_SPI_POL);
	spi_set_clock_phase(CONF_WIFI_NC_SPI, CONF_WIFI_NC_SPI_NPCS, CONF_WIFI_NC_SPI_PHA);
	spi_set_bits_per_transfer(CONF_WIFI_NC_SPI,
	CONF_WIFI_NC_SPI_NPCS, SPI_CSR_BITS_8_BIT);
	printf("sys clk %d\r\n",sysclk_get_cpu_hz());
	spi_set_baudrate_div(CONF_WIFI_NC_SPI,
	CONF_WIFI_NC_SPI_NPCS,
	(sysclk_get_cpu_hz() / CONF_WIFI_NC_SPI_CLOCK));
	#if 1
	spi_set_transfer_delay(CONF_WIFI_NC_SPI,
	CONF_WIFI_NC_SPI_NPCS, CONF_WIFI_NC_SPI_DLYBS, CONF_WIFI_NC_SPI_DLYBCT);
	#endif
	spi_enable(CONF_WIFI_NC_SPI);

	/* Get pointer to SPI master PDC register base. */
	g_p_spim_pdc = spi_get_pdc_base(CONF_WIFI_NC_SPI);
	pdc_disable_transfer(g_p_spim_pdc, PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);

	nm_bsp_reset();
	SPI_DEASSERT_CS();
	#endif
	return result;
}

/*
*	@fn		nm_bus_ioctl
*	@brief	send/receive from the bus
*	@param[IN]	u8Cmd
*					IOCTL command for the operation
*	@param[IN]	pvParameter
*					Arbitrary parameter depending on IOCTL
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@note	For SPI only, it's important to be able to send/receive at the same time
*/
sint8 nm_bus_ioctl(uint8 u8Cmd, void* pvParameter)
{
	sint8 s8Ret = 0;
	switch(u8Cmd)
	{
		#ifdef USE_I2C
		case NM_BUS_IOCTL_R: {
			tstrNmI2cDefault *pstrParam = (tstrNmI2cDefault *)pvParameter;
			s8Ret = nm_i2c_read(pstrParam->pu8Buf, pstrParam->u16Sz);
		}
		break;
		case NM_BUS_IOCTL_W: {
			tstrNmI2cDefault *pstrParam = (tstrNmI2cDefault *)pvParameter;
			s8Ret = nm_i2c_write(pstrParam->pu8Buf, pstrParam->u16Sz);
		}
		break;
		case NM_BUS_IOCTL_W_SPECIAL: {
			tstrNmI2cSpecial *pstrParam = (tstrNmI2cSpecial *)pvParameter;
			s8Ret = nm_i2c_write_special(pstrParam->pu8Buf1, pstrParam->u16Sz1, pstrParam->pu8Buf2, pstrParam->u16Sz2);
		}
		break;
		#elif CONF_WINC_USE_SPI
		case NM_BUS_IOCTL_RW: {
			tstrNmSpiRw *pstrParam = (tstrNmSpiRw *)pvParameter;
        s8Ret = nm_spi_rw(pstrParam->pu8InBuf, pstrParam->pu8OutBuf, pstrParam->u16Sz);
		}
		break;
		#endif
		default:
		s8Ret = -1;
        M2M_ERR("invalide ioclt cmd\n");
		break;
	}

	return s8Ret;
}

/*
*	@fn		nm_bus_deinit
*	@brief	De-initialize the bus wrapper
*/
sint8 nm_bus_deinit(void)
{
	return M2M_SUCCESS;
}
