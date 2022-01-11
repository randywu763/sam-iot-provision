/**
 *
 * \file
 *
 * \brief WINC1500 Chip Information Example.
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

/** \mainpage
 * \section intro Introduction
 * This example demonstrates the use of the WINC1500 with the SAMD21 Xplained Pro
 * board to retrieve the chip information of the Wi-Fi module.<br>
 * It uses the following hardware:
 * - the SAMD21 Xplained Pro.
 * - the WINC1500 on EXT1.
 *
 * \section files Main Files
 * - main.c : Initialize the WINC1500 and retrieve information.
 *
 * \section usage Usage
 * -# Build the program and download it into the board.
 * -# On the computer, open and configure a terminal application as the follows.
 * \code
 *    Baud Rate : 115200
 *    Data : 8bit
 *    Parity bit : none
 *    Stop bit : 1bit
 *    Flow control : none
 * \endcode
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 * \code
 *    -- WINC1500 chip information example --
 *    -- SAMD21_XPLAINED_PRO --
 *    -- Compiled: xxx xx xxxx xx:xx:xx --
 *    Chip ID :             xxxxxx
 *    RF Revision ID :      x
 *    Done.
 * \endcode
 *
 * \section compinfo Compilation Information
 * This software was written for the GNU GCC compiler using Atmel Studio 6.2
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com">Atmel</A>.\n
 */

#include "asf.h"
#include <string.h>
#include "driver/include/m2m_wifi.h"
#include "driver/source/nmasic.h"
#include "driver/include/m2m_ate_mode.h"

#define STRING_EOL    "\r\n"
#define STRING_HEADER "-- Wifi ATE Firmware Demo --"STRING_EOL \
	"-- "BOARD_NAME " --"STRING_EOL	\
	"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

/** UART module for debug. */
static struct usart_module cdc_uart_module;

#define M2M_ATE_TEST_DURATION_IN_SEC	60	/*Seconds*/

#define M2M_ATE_RUN_RX_TEST_CASE		ENABLE
#define M2M_ATE_RUN_TX_TEST_CASE		ENABLE

/**
 * \brief Configure UART console.
 */
static void configure_console(void)
{
	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	usart_conf.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	usart_conf.baudrate    = 115200;

	stdio_serial_init(&cdc_uart_module, EDBG_CDC_MODULE, &usart_conf);
	usart_enable(&cdc_uart_module);
}

#if (M2M_ATE_RUN_TX_TEST_CASE == ENABLE)
static void start_tx_test(uint8_t tx_rate)
{	
	tstrM2mAteTx tx_struct;
	
	/*Initialize parameter structure*/
	m2m_memset((uint8 *)&tx_struct, 0 , sizeof(tx_struct));
	
	/*Set TX Configuration parameters, 
	 *refer to tstrM2mAteTx for more information about parameters*/
	tx_struct.channel_num	= M2M_ATE_CHANNEL_11;
	tx_struct.data_rate		= m2m_ate_get_tx_rate(tx_rate);
	tx_struct.dpd_ctrl		= M2M_ATE_TX_DPD_DYNAMIC;
	tx_struct.duty_cycle	= M2M_ATE_TX_DUTY_1;
	tx_struct.frame_len		= 1024;
	tx_struct.num_frames	= 0;
	tx_struct.phy_burst_tx	= M2M_ATE_TX_SRC_MAC;
	tx_struct.tx_gain_sel	= M2M_ATE_TX_GAIN_DYNAMIC;
	tx_struct.use_pmu		= M2M_ATE_PMU_DISBLE;
	tx_struct.cw_tx			= M2M_ATE_TX_MODE_CW;
	tx_struct.xo_offset_x1000 =  0;
	
	/*Start TX Case*/
	if(M2M_ATE_SUCCESS == m2m_ate_start_tx(&tx_struct))
	{
		uint32 u32TxTimeout = M2M_ATE_TEST_DURATION_IN_SEC;
		
		M2M_INFO(">>Running TX Test case on CH<%02u>.\r\n", tx_struct.channel_num);
		do
		{
			nm_bsp_sleep(1000);
			printf("%02u\r", (unsigned int)u32TxTimeout);
		}while(--u32TxTimeout);
		
		if(M2M_ATE_SUCCESS == m2m_ate_stop_tx())
		{
			M2M_INFO("Completed TX Test successfully.\r\n");
		}
	}
	else
	{
		M2M_INFO("Failed to start TX Test case.\r\n");
	}
}
#endif

#if (M2M_ATE_RUN_RX_TEST_CASE == ENABLE)
static void start_rx_test(void)
{
	tstrM2mAteRx rx_struct;
	
	/*Initialize parameter structure*/
	m2m_memset((uint8 *)&rx_struct, 0, sizeof(rx_struct));
	
	/*Set RX Configuration parameters*/
	rx_struct.channel_num	= M2M_ATE_CHANNEL_6;
	rx_struct.use_pmu		= M2M_ATE_PMU_DISBLE;
	rx_struct.xo_offset_x1000 =  0;
	
	/*Start RX Case*/
	if(M2M_ATE_SUCCESS == m2m_ate_start_rx(&rx_struct))
	{
		tstrM2mAteRxStatus rx_data;
		uint32 u32RxTimeout = M2M_ATE_TEST_DURATION_IN_SEC;
		
		M2M_INFO(">>Running RX Test case on CH<%02u>.\r\n", rx_struct.channel_num);
		do
		{
			m2m_ate_read_rx_status(&rx_data);
			M2M_INFO("Num Rx PKTs: %d, Num ERR PKTs: %d, PER: %1.3f", (int)rx_data.num_rx_pkts, (int)rx_data.num_err_pkts,
				(rx_data.num_rx_pkts>0)?((double)rx_data.num_err_pkts/(double)rx_data.num_rx_pkts):(0));
			nm_bsp_sleep(1000);
		}while(--u32RxTimeout);
		printf("\r\n");
		if(M2M_ATE_SUCCESS == m2m_ate_stop_rx())
		{
			M2M_INFO("Compeleted RX Test successfully.\r\n");
		}
	}
	else
	{
		M2M_INFO("Failed to start RX Test case.\r\n");
	}
}
#endif

/**
 * \brief Main application function.
 *
 * Application entry point.
 * Initialize board and WINC1500 Wi-Fi module.
 * Display WINC1500 chip id and rf revision id.
 *
 * \return program return value.
 */
int main(void)
{
	/* Initialize the board. */
	system_init();

	/* Initialize the UART console. */
	configure_console();
	printf(STRING_HEADER);

	/* Initialize the BSP. */
	nm_bsp_init();

	/*Check if initialization of ATE firmware is succeeded or not*/
	if(M2M_SUCCESS == m2m_ate_init())
	{
		/*Run TX test case if defined*/
		#if (M2M_ATE_RUN_TX_TEST_CASE == ENABLE)
		start_tx_test(M2M_ATE_TX_RATE_1_Mbps_INDEX);
		#endif
		/*Run RX test case if defined*/
		#if (M2M_ATE_RUN_RX_TEST_CASE == ENABLE)
		start_rx_test();
		#endif

		/*De-Initialization of ATE firmware test mode*/
		m2m_ate_deinit();
	}
	else
	{
		M2M_ERR("Failed to initialize ATE firmware.\r\n");
		while(1);
	}

	#if ((M2M_ATE_RUN_RX_TEST_CASE == ENABLE) && (M2M_ATE_RUN_TX_TEST_CASE == ENABLE))
	M2M_INFO("Test cases have been finished.\r\n");
	#else
	M2M_INFO("Test case has been finished.\r\n");
	#endif

	while(1);
}
