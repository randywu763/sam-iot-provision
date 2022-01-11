/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */


/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include "common/include/nm_common.h"
#include "bsp/include/nm_bsp_samd21.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "driver\source\nmbus.h"
#include "driver/include/m2m_wifi.h"
#include "stdio_serial.h"
#include "conf_winc.h"
#include "driver/source/nmspi.h"

#define STRING_EOL    "\r\n"
#define STRING_HEADER "-- Wifi Firmware Downloader --"STRING_EOL \
"-- "BOARD_NAME" --"STRING_EOL \
"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL


/** UART COMMAND */
enum nm_usart_event_types {
	USART_PKT_RECEIVED = 0,
	USART_PKT_TRANSMITTED,
	USART_ERROR_ON_RECEPTION,	
};
enum nm_usart_cmd_process_states {
	INIT = 0,
	WAIT_SYNC,
	WAITING,
	COLLECTING_HDR,
	COLLECTING_PAYLOAD,
	PROCESSING,
};
typedef struct uart_cmd_hdr_t {
	unsigned long cmd;
	unsigned long addr;
	unsigned long val;
}uart_cmd_hdr;

#define READ_REG	0
#define WRITE_REG	1
#define READ_BUFF	2
#define WRITE_BUFF	3
#define RESET		4
#define RECONFIGURE_UART	5

/** UART module for debug. */
extern struct usart_module cdc_uart_module;

#define CONF_STDIO_USART_MODULE  EDBG_CDC_MODULE
#define CONF_STDIO_MUX_SETTING   EDBG_CDC_SERCOM_MUX_SETTING
#define CONF_STDIO_PINMUX_PAD0   EDBG_CDC_SERCOM_PINMUX_PAD0
#define CONF_STDIO_PINMUX_PAD1   EDBG_CDC_SERCOM_PINMUX_PAD1
#define CONF_STDIO_PINMUX_PAD2   EDBG_CDC_SERCOM_PINMUX_PAD2
#define CONF_STDIO_PINMUX_PAD3   EDBG_CDC_SERCOM_PINMUX_PAD3
#define CONF_STDIO_BAUDRATE      115200


static uint16_t usart_cmd_recv_buffer[8];
//static uint8_t nm_usart_cmd[16];
static uint16_t usart_payload_buffer[512];
static uint8_t usart_tx_buffer[16];
static uint8_t serial_command_pending = 0;
static uint8_t usart_pkt_received = 0;
static uint8_t usart_err_on_reception = 0;
static uint8 *uart_cmd_buf;
static uart_cmd_hdr uart_cmd;
static uint16_t schedule_rx_length = 0;
static uint8_t schedule_rx = 0;
static uint16_t *schedule_rx_buffer = NULL;
static uint8_t *usart_pkt = NULL;
static uint8_t usart_prot_handler_status = INIT;
static uint8_t new_state = INIT;
static uint8_t change_state = 0;
static uint8_t uart_reconfigure = 0;
#define USART_CMD_HDR_LENGTH	sizeof(uart_cmd_hdr)
#define SPI_TRANSFER_SIZE	512
static void usart_tx_complete_handler(const struct usart_module *const module)
{
	if(schedule_rx) {
		if(schedule_rx_length == 1) {
			usart_read_job((struct usart_module *)module, schedule_rx_buffer);
		}
		else {
			usart_read_buffer_job((struct usart_module *)module, (uint8_t *)schedule_rx_buffer, schedule_rx_length);
		}
		schedule_rx = 0;
		//schedule_rx_length = 0;
		//schedule_rx_buffer = NULL;
	}
	if(change_state) {
		usart_prot_handler_status = new_state;
		change_state = 0;
	}
}

static void usart_rx_complete_handler(const struct usart_module *const module)
{
	usart_pkt_received = 1;
	usart_pkt = (uint8_t *)(module->rx_buffer_ptr - schedule_rx_length);
	if(change_state) {
		usart_prot_handler_status = new_state;
		change_state = 0;
	}
}

//static void usart_err_handler(struct usart_module *module)
//{
//	usart_err_on_reception = 1;
//	usart_abort_job(module,USART_TRANSCEIVER_RX);
//}

static void nm_usart_send_regval(struct usart_module *module,uint8_t *tx_data,uint16_t length)
{
	uint8_t temp,i,j;
	for(i=0,j=(length-1);i<j;i++,j--) {
		temp = tx_data[i];
		tx_data[i] = tx_data[j];
		tx_data[j] = temp;
	}
	usart_write_buffer_job(module,tx_data,length);
}

static void nm_usart_protocol_handler(struct usart_module *module,enum nm_usart_event_types event_name)
{
	//uint16_t response;
	static uint16_t payload_length = 0;
	//uint16_t expected_length;
	//static uint32_t target_address = 0;
	uint8 checksum = 0;
	uint8 i;
#if 1
switch(usart_prot_handler_status) {
	case INIT:
		if((event_name == USART_PKT_RECEIVED) && (schedule_rx_length == 1)) {
			if((usart_pkt[0] == 0x12)) {
				usart_prot_handler_status = WAIT_SYNC;
				usart_cmd_recv_buffer[0] = 0xFF;
				schedule_rx_buffer = &usart_cmd_recv_buffer[0];
				schedule_rx_length = 1;
				schedule_rx = 1;
				usart_tx_buffer[0] = 0x5B;  // WINCxx00 Serial Bridge + AT CMD app - W3400-404 report as plain serial bridge
				usart_write_job(module,usart_tx_buffer[0]);
			}
			else if((usart_pkt[0] == 0x13)) {
				NVIC_SystemReset();
				while(1);
			}
			else {
				schedule_rx_buffer = &usart_cmd_recv_buffer[0];
				schedule_rx_length = 1;
				schedule_rx = 1;
				usart_tx_buffer[0] = usart_pkt[0];
				usart_write_job(module,usart_tx_buffer[0]);
				//usart_read_job(module,&usart_cmd_recv_buffer[0]);
			}
		}
		else {
			usart_cmd_recv_buffer[0] = 0xFF;
			schedule_rx_buffer = &usart_cmd_recv_buffer[0];
			schedule_rx_length = 1;
			schedule_rx = 1;
			usart_tx_buffer[0] = 0xEA;
			usart_write_job(module,usart_tx_buffer[0]);
		}
		break;
	case WAIT_SYNC:
		if(event_name == USART_PKT_RECEIVED) {
			if(usart_pkt[0] == 0xA5) {
				uint8 * usart_cmd_recv_buffer_u8 = (uint8*)&usart_cmd_recv_buffer[0];
				//module->rx_buffer_ptr = &usart_cmd_recv_buffer_u8[4];
				//module->remaining_rx_buffer_length = 1;
				usart_prot_handler_status = WAITING;
				usart_cmd_recv_buffer_u8[4] = 0xFF;
				schedule_rx_length = 1;
				usart_read_job(module, &usart_cmd_recv_buffer[2]);
			}
			else if(usart_pkt[0] == 0x12) {	//uart identification command
				usart_tx_buffer[0] = 0x5B; // WINCxx00 Serial Bridge + AT CMD app - W3400-404 report as plain serial bridge
				usart_cmd_recv_buffer[0] = 0xFF;
				schedule_rx_buffer = &usart_cmd_recv_buffer[0];
				schedule_rx_length = 1;
				schedule_rx = 1;
				usart_write_job(module,usart_tx_buffer[0]);
			}
			else if((usart_pkt[0] == 0x13)) {
				NVIC_SystemReset();
				while(1);
			}
			else {
				if(!uart_reconfigure) {
					usart_tx_buffer[0] = 0x5A;
					usart_cmd_recv_buffer[0] = 0xFF;
					schedule_rx_buffer = &usart_cmd_recv_buffer[0];
					schedule_rx_length = 1;
					schedule_rx = 1;
					usart_write_job(module,usart_tx_buffer[0]);
				}
				else {
					//uart_reconfigure = 0;
					schedule_rx_length = 1;
					usart_read_job(module, &usart_cmd_recv_buffer[0]);
				}
			}
		}
			break;
	case WAITING:
		if(event_name == USART_PKT_RECEIVED) {
			usart_prot_handler_status = COLLECTING_HDR;
			uart_cmd_buf = usart_pkt;
			//module->remaining_rx_buffer_length = USART_CMD_HDR_LENGTH - 1;
			schedule_rx_length = (USART_CMD_HDR_LENGTH - 1);
			usart_read_buffer_job(module, (uint8_t *)module->rx_buffer_ptr, (USART_CMD_HDR_LENGTH - 1));
		}
		else {
			usart_prot_handler_status = WAIT_SYNC;
			schedule_rx_buffer = &usart_cmd_recv_buffer[0];
			schedule_rx = 1;
			schedule_rx_length = 1;
			usart_tx_buffer[0] = 0xEA;
			usart_write_job(module,usart_tx_buffer[0]);
		}
		break;
	case COLLECTING_HDR:
		if(event_name == USART_PKT_RECEIVED) {
			//Verify check sum
			for(i=0;i<(USART_CMD_HDR_LENGTH);i++) {
				checksum ^= *(((uint8_t *)uart_cmd_buf)+i);
			}
			if(checksum != 0) {
				usart_prot_handler_status = WAIT_SYNC;
				//module->remaining_rx_buffer_length = 1;
				usart_cmd_recv_buffer[0] = 0xFF;
				schedule_rx_buffer = &usart_cmd_recv_buffer[0];
				schedule_rx_length = 1;
				schedule_rx = 1;
				usart_tx_buffer[0] = 0x5A;
				//ptr_put(module,&response);
				usart_write_job(module,usart_tx_buffer[0]);
				//usart_read_job(module,&usart_cmd_recv_buffer[0]);
			}
			else {
				memcpy(&uart_cmd, uart_cmd_buf, sizeof(uart_cmd_hdr));
				//Process the Command.
				//uart_cmd = &module->rx_buffer_ptr[4];
				if((uart_cmd.cmd & 0xFF) == WRITE_BUFF) {
					usart_prot_handler_status = COLLECTING_PAYLOAD;
					payload_length = (uart_cmd.cmd >> 16) & 0xFFFF;
					//target_address = uart_cmd.addr;
					schedule_rx = 1;
					schedule_rx_buffer = &usart_payload_buffer[0];
					schedule_rx_length = payload_length;
					usart_tx_buffer[0] = 0xAC;
					usart_write_job(module,usart_tx_buffer[0]);
				} 
				else if((uart_cmd.cmd & 0xFF) == WRITE_REG) {
					serial_command_pending = 1;
					usart_prot_handler_status = PROCESSING;
				}
				else {
					serial_command_pending = 1;
					//usart_prot_handler_status = PROCESSING;
					change_state = 1;
					new_state = PROCESSING;
					usart_tx_buffer[0] = 0xAC;
					usart_write_job(module,usart_tx_buffer[0]);
				}
			}
		}
		else if(event_name == USART_ERROR_ON_RECEPTION) {
			usart_prot_handler_status = WAIT_SYNC;
			schedule_rx_buffer = &usart_cmd_recv_buffer[0];
			schedule_rx = 1;
			schedule_rx_length = 1;
			usart_tx_buffer[0] = 0xEA;
			usart_write_job(module,usart_tx_buffer[0]);
		}
		break;
	case COLLECTING_PAYLOAD:
		if((event_name == USART_PKT_RECEIVED) && (schedule_rx_length == payload_length)) {
			serial_command_pending = 1;
			usart_prot_handler_status = PROCESSING;
		}
		else if(event_name == USART_ERROR_ON_RECEPTION) {
			usart_prot_handler_status = WAIT_SYNC;
			usart_tx_buffer[0] = 0xEA;
			usart_cmd_recv_buffer[0] = 0xFF;
			schedule_rx_length = 1;
			schedule_rx_buffer = &usart_cmd_recv_buffer[0];
			schedule_rx = 1;
			usart_write_job(module,usart_tx_buffer[0]);
		}
		else {
			usart_prot_handler_status = WAIT_SYNC;
			usart_tx_buffer[0] = 0x5A;
			usart_cmd_recv_buffer[0] = 0xFF;
			schedule_rx_length = 1;
			schedule_rx_buffer = &usart_cmd_recv_buffer[0];
			schedule_rx = 1;
			usart_write_job(module,usart_tx_buffer[0]);
		}
		break;
	default:
		usart_prot_handler_status = WAIT_SYNC;		
		break;
	}
#else
#endif
}
/**
 *  Configure UART console.
 */
static void configure_console(uint32_t baud)
{
	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;
	usart_conf.pinmux_pad0 = CONF_STDIO_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = CONF_STDIO_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = CONF_STDIO_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = CONF_STDIO_PINMUX_PAD3;
	usart_conf.baudrate    = baud;
	//usart_conf.sample_rate = USART_SAMPLE_RATE_16X_FRACTIONAL;
	stdio_serial_init(&cdc_uart_module, CONF_STDIO_USART_MODULE, &usart_conf);
	usart_register_callback(&cdc_uart_module,usart_rx_complete_handler,USART_CALLBACK_BUFFER_RECEIVED);
	usart_register_callback(&cdc_uart_module,usart_tx_complete_handler,USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&cdc_uart_module,USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&cdc_uart_module,USART_CALLBACK_BUFFER_TRANSMITTED);
	//usart_register_callback(&cdc_uart_module,usart_err_handler,USART_CALLBACK_ERROR);
	//usart_enable_callback(&cdc_uart_module,USART_CALLBACK_ERROR);
	usart_enable(&cdc_uart_module);
	schedule_rx_length = 1;
	usart_read_job(&cdc_uart_module,&usart_cmd_recv_buffer[0]);
}

static sint8 enter_wifi_firmware_download(void)
{
	sint8 ret;
	uint8 u8WifiMode = M2M_WIFI_MODE_NORMAL;
	//uint32 reg_val;
	static uint8_t done_programming = 0;
	//nm_bsp_start_timer(NULL, 500);
	
	// Should use m2m_wifi_download_mode(), but it prints out a Chip ID message which messes with the downloader
	//ret = m2m_wifi_download_mode();
	ret = nm_bus_iface_init(NULL);
#ifdef CONF_WINC_USE_SPI
	nm_spi_init();
#endif

	if(ret != M2M_SUCCESS)
	{
		M2M_ERR("Failed to put the WiFi Chip in download mode\n");
		return M2M_ERR_INIT;
	}
	//Program the WiFi chip here
	while(!done_programming) {
		
		if(port_pin_get_input_level(BUTTON_0_PIN) == false)
		{
			ret = nm_drv_init(&u8WifiMode);
		}
		
		
		if(usart_pkt_received) {
			usart_pkt_received = 0;
			nm_usart_protocol_handler(&cdc_uart_module,USART_PKT_RECEIVED);
			//usart_write_job(&cdc_uart_module,0xAC);
		}
		if(usart_err_on_reception) {
			usart_err_on_reception = 0;
			nm_usart_protocol_handler(&cdc_uart_module,USART_ERROR_ON_RECEPTION);
		}
		if(serial_command_pending && (usart_prot_handler_status == PROCESSING)) {
			uint32_t temp;
			switch((uart_cmd.cmd) & 0xFF) {
				//Forward it to SERCOM0 SPI
				case READ_REG:
					//Transalate it to SPI Read register
					temp = nm_read_reg(uart_cmd.addr);
					usart_tx_buffer[0] = (uint8)(temp >> 0);
					usart_tx_buffer[1] = (uint8)(temp >> 8);
					usart_tx_buffer[2] = (uint8)(temp >> 16);
					usart_tx_buffer[3] = (uint8)(temp >> 24);
					//usart_write_buffer_wait(&cdc_uart_module,&reg_val,sizeof(reg_val));
					schedule_rx_buffer = &usart_cmd_recv_buffer[0];
					schedule_rx_length = 1;
					schedule_rx = 1;
					//change_state = 1;
					//new_state = WAIT_SYNC;
					usart_prot_handler_status = WAIT_SYNC;
					nm_usart_send_regval(&cdc_uart_module,&usart_tx_buffer[0],sizeof(uint32_t));
					break;
				case WRITE_REG:
					//Transalate it to SPI Write register
					nm_write_reg(uart_cmd.addr,uart_cmd.val);
					schedule_rx_buffer = &usart_cmd_recv_buffer[0];
					schedule_rx_length = 1;
					schedule_rx = 1;
					usart_tx_buffer[0] = 0xAC;
					usart_prot_handler_status = WAIT_SYNC;
					usart_write_job(&cdc_uart_module,usart_tx_buffer[0]);
					break;
				case READ_BUFF:
				//Transalate it to SPI Read buffer
					nm_read_block(uart_cmd.addr, (uint8 *)&usart_payload_buffer[0],((uart_cmd.cmd >> 16) & 0xFFFF));
					schedule_rx_buffer = &usart_cmd_recv_buffer[0];
					schedule_rx_length = 1;
					schedule_rx = 1;
					usart_prot_handler_status = WAIT_SYNC;
					usart_write_buffer_job(&cdc_uart_module, (uint8 *)&usart_payload_buffer[0],((uart_cmd.cmd >> 16) & 0xFFFF));
					break;
				case WRITE_BUFF:
					//Transalate it to SPI Write buffer
					nm_write_block(uart_cmd.addr, (uint8 *)&usart_payload_buffer[0],((uart_cmd.cmd >> 16) & 0xFFFF));
					schedule_rx_buffer = &usart_cmd_recv_buffer[0];
					schedule_rx_length = 1;
					schedule_rx = 1;
					usart_tx_buffer[0] = 0xAC;
					usart_prot_handler_status = WAIT_SYNC;
					usart_write_job(&cdc_uart_module,usart_tx_buffer[0]);
					break;
				case RECONFIGURE_UART:
					// Send the ack back
					usart_prot_handler_status = WAIT_SYNC;
					uart_reconfigure = 1;
					usart_disable(&cdc_uart_module);
					configure_console(uart_cmd.val);
					break;
				case 10:
				//GPIO read command
					usart_tx_buffer[0] = port_pin_get_input_level(PIN_PB09);
					//usart_tx_buffer[0]= port_pin_get_output_level(PIN_PB09);
					schedule_rx_buffer = &usart_cmd_recv_buffer[0];
					schedule_rx_length = 1;
					schedule_rx = 1;
					usart_prot_handler_status = WAIT_SYNC;
					usart_write_job(&cdc_uart_module,usart_tx_buffer[0]);
					break;
				default:
					break;
			}
			serial_command_pending = 0;
		}
	}
	return ret;
}

void enter_sb_from_at()
{
	configure_console(CONF_STDIO_BAUDRATE);

	// Manual reply to the 0x12 command
	usart_prot_handler_status = WAIT_SYNC;
	usart_cmd_recv_buffer[0] = 0xFF;
	schedule_rx_buffer = &usart_cmd_recv_buffer[0];
	schedule_rx_length = 1;
	schedule_rx = 1;
	usart_tx_buffer[0] = 0x5B; // WINCxx00 Serial Bridge + AT CMD app - W3400-404 report as plain serial bridge
	usart_write_job(&cdc_uart_module,usart_tx_buffer[0]);
	
	enter_wifi_firmware_download();
}

int main2 (void)
{

	sint8 ret = 0;
	system_init();
	/* Initialize the UART console. */
	configure_console(CONF_STDIO_BAUDRATE);
	/* Output example information */

	
	while(1)
	{
		nm_bsp_init();
		{
			
			struct extint_chan_conf config_extint_chan;

			//gpfIsr = pfIsr;

			extint_chan_get_config_defaults(&config_extint_chan);
			config_extint_chan.gpio_pin = CONF_WINC_SPI_INT_PIN;
			config_extint_chan.gpio_pin_mux = CONF_WINC_SPI_INT_MUX;
			config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
			config_extint_chan.detection_criteria = EXTINT_DETECT_FALLING;

			extint_chan_set_config(CONF_WINC_SPI_INT_EIC, &config_extint_chan);
		}
		//nm_bsp_sleep(500);
		//system_interrupt_enable_global();
		ret = enter_wifi_firmware_download();
	}
	return ret;	
}