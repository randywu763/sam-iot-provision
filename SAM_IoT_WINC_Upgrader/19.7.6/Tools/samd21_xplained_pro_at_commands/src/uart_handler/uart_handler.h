/*
 * uart_handler.h
 *
 * Created: 10/1/2014 5:43:36 PM
 *  Author: aali
 */ 


#ifndef UART_HANDLER_H_
#define UART_HANDLER_H_


#include <asf.h>
#include "usart.h"
//#include "stdio_serial.h"
#include "common/include/nm_common.h"

#define STRING_EOL		"\r\n"

#define MAX_RX_BUFFER_LENGTH		1

#define CONF_STDIO_USART_MODULE		EDBG_CDC_MODULE
#define CONF_STDIO_MUX_SETTING		EDBG_CDC_SERCOM_MUX_SETTING
#define CONF_STDIO_PINMUX_PAD0		EDBG_CDC_SERCOM_PINMUX_PAD0
#define CONF_STDIO_PINMUX_PAD1		EDBG_CDC_SERCOM_PINMUX_PAD1
#define CONF_STDIO_PINMUX_PAD2		EDBG_CDC_SERCOM_PINMUX_PAD2
#define CONF_STDIO_PINMUX_PAD3		EDBG_CDC_SERCOM_PINMUX_PAD3

#define uart_readBuff(uart_module,buffer)		usart_read_buffer_job(&uart_module, (uint8_t *)buffer, MAX_RX_BUFFER_LENGTH)

void configure_uart(struct usart_module *, uint8 usart_mode, uint32_t);
void configure_uart_callbacks(struct usart_module *, usart_callback_t, usart_callback_t);

#endif /* UART_HANDLER_H_ */