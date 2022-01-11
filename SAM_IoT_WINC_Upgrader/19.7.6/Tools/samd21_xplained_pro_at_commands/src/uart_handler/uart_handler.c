/*
 * uart_handler.c
 *
 * Created: 10/1/2014 5:43:47 PM
 *  Author: aali
 */ 

#include "uart_handler.h"

void configure_uart(struct usart_module *usart_module, uint8 usart_mode, uint32 usart_baudRate)
{
	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	
	usart_conf.baudrate			= usart_baudRate;
	usart_conf.transfer_mode	= usart_mode;
	usart_conf.mux_setting		= CONF_STDIO_MUX_SETTING;
	usart_conf.pinmux_pad0		= CONF_STDIO_PINMUX_PAD0;
	usart_conf.pinmux_pad1		= CONF_STDIO_PINMUX_PAD1;
	usart_conf.pinmux_pad2		= CONF_STDIO_PINMUX_PAD2;
	usart_conf.pinmux_pad3		= CONF_STDIO_PINMUX_PAD3;
	
	stdio_serial_init(usart_module, CONF_STDIO_USART_MODULE, &usart_conf);
	//usart_init(usart_module, CONF_STDIO_USART_MODULE, &usart_conf);
	usart_enable(usart_module);
}

void configure_uart_callbacks
(
	struct usart_module *cdc_uart_module,
	usart_callback_t callback_func_tx, 
	usart_callback_t callback_func_rx
)
{
	usart_register_callback(cdc_uart_module, callback_func_tx, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(cdc_uart_module, callback_func_rx, USART_CALLBACK_BUFFER_RECEIVED);
	
	usart_enable_callback(cdc_uart_module, USART_CALLBACK_BUFFER_TRANSMITTED);
	//usart_disable_callback(cdc_uart_module, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(cdc_uart_module, USART_CALLBACK_BUFFER_RECEIVED);
}