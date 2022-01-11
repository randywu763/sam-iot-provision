/**
	Hacked version of sio2host.

	The hacking is pretty light, really it is so you get another instance and avoid
	clashing with the EDB port. But it also takes out the tie to stdio, for stdio via
	EDBG port use "Standard Serial I/O (stdio)(driver)".
 */

/* === INCLUDES ============================================================ */

#include "asf.h"
#include <stdio.h>
#include <string.h>
#include "buffered_uart.h"
#include "conf_buffered_uart.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === PROTOTYPES ========================================================== */

/* === GLOBALS ========================================================== */
#if SAMD || SAMR21 || SAML21
static struct usart_module host_uart_module;
#else
static usart_serial_options_t usart_serial_options = {
	.baudrate     = USART_HOST_BAUDRATE,
	.charlength   = USART_HOST_CHAR_LENGTH,
	.paritytype   = USART_HOST_PARITY,
	.stopbits     = USART_HOST_STOP_BITS
};
#endif

/**
 * Receive buffer
 * The buffer size is defined in buffered_uart.h
 */
static uint8_t serial_rx_buf[SERIAL_RX_BUF_SIZE_HOST];

/**
 * Receive buffer head
 */
static uint8_t serial_rx_buf_head;

/**
 * Receive buffer tail
 */
static uint8_t serial_rx_buf_tail;


/* === IMPLEMENTATION ====================================================== */
void buffered_uart_init(uint32_t baudrate)
{
	serial_rx_buf_head = 0;
	serial_rx_buf_tail = 0;


#if SAMD || SAMR21 || SAML21
	struct usart_config host_uart_config;
	/* Configure USART for unit test output */
	usart_get_config_defaults(&host_uart_config);
	host_uart_config.mux_setting = HOST_SERCOM_MUX_SETTING;

	host_uart_config.pinmux_pad0 = HOST_SERCOM_PINMUX_PAD0;
	host_uart_config.pinmux_pad1 = HOST_SERCOM_PINMUX_PAD1;
	host_uart_config.pinmux_pad2 = HOST_SERCOM_PINMUX_PAD2;
	host_uart_config.pinmux_pad3 = HOST_SERCOM_PINMUX_PAD3;
	host_uart_config.baudrate    = baudrate;
	//usart_serial_init(&host_uart_module, USART_HOST, &host_uart_config);
	stdio_serial_init(&host_uart_module, USART_HOST, &host_uart_config);
	usart_enable(&host_uart_module);
	/* Enable transceivers */
	//usart_enable_transceiver(&host_uart_module, USART_TRANSCEIVER_TX);
	//usart_enable_transceiver(&host_uart_module, USART_TRANSCEIVER_RX);
#else
	usart_serial_options.baudrate = baudrate;

	# if (XMEGA || MEGA_RF)
	usart_serial_init(USART_HOST,&usart_serial_options);
	# elif UC3
	usart_serial_init(USART_HOST,(usart_serial_options_t *)&usart_serial_options);
	# elif SAM
	//usart_serial_init((Usart *)USART_HOST,(usart_serial_options_t *)&usart_serial_options);
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(USART_HOST, &usart_serial_options);
	# else
	#  error Unsupported chip type
	# endif
#endif
	USART_HOST_RX_ISR_ENABLE();
}

void buffered_uart_term(void)
{
#if SAMD21
	usart_disable(&host_uart_module);
#elif SAM
	USART_HOST_RX_ISR_DISABLE();
#endif
}


void buffered_uart_putchar(uint8_t ch)
{
#if SAMD21
	//while (!(USART_HOST->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_TXC));
	USART_HOST->USART.DATA.reg = ch;
	while (!(USART_HOST->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_TXC));
#elif SAMD || SAMR21 || SAML21
	usart_serial_putchar(&host_uart_module, ch);
#else
	usart_serial_putchar(USART_HOST, ch);
	while(!uart_is_tx_buf_empty(USART_HOST));
#endif
}



uint8_t buffered_uart_tx(uint8_t *data, uint32_t length)
{
#if SAMD21
	uint8_t status = 0;
	uint32_t written = 0;
#elif SAMD || SAMR21 || SAML21
	status_code_genare_t status;
#else
	status_code_t status;
#endif /*SAMD || SAMR21 || SAML21 */

	do {
#if SAMD21
		while (length != written) {
			buffered_uart_putchar(*data);
			data++;
			written++;	
		}

#elif SAMD || SAMR21 || SAML21
		status
			= usart_serial_write_packet(&host_uart_module,
			(const uint8_t *)data, length);
#else
		status = usart_serial_write_packet(USART_HOST,
			(const uint8_t *)data,
			length);
#endif
	} while (status != STATUS_OK);
	return length;
}

uint8_t buffered_uart_rx(uint8_t *data, uint8_t max_len)
{
	uint8_t copy_len = 0;
	uint8_t data_len = 0;

	if (serial_rx_buf_tail != serial_rx_buf_head) {
		cpu_irq_disable();
		
		if (serial_rx_buf_tail >= serial_rx_buf_head) {
			copy_len = serial_rx_buf_tail - serial_rx_buf_head;
		} else {
			copy_len = serial_rx_buf_tail + (SERIAL_RX_BUF_SIZE_HOST - serial_rx_buf_head);
		}
		
		if (copy_len > max_len) {
			copy_len = max_len;
		}
		
		if (copy_len) {
			if (SERIAL_RX_BUF_SIZE_HOST < (copy_len + serial_rx_buf_head)) {
				data_len = SERIAL_RX_BUF_SIZE_HOST - serial_rx_buf_head;
				memcpy(data, &serial_rx_buf[serial_rx_buf_head], data_len);
				data += data_len;
				copy_len -= data_len; 
				serial_rx_buf_head = 0;
			}
			memcpy(data, &serial_rx_buf[serial_rx_buf_head], copy_len);
			data_len += copy_len;
			serial_rx_buf_head += copy_len;
		}
		cpu_irq_enable();
	}
	return data_len;
}

int buffered_uart_getchar_nowait(void)
{
	int c = -1;
	cpu_irq_disable();
	if (serial_rx_buf_tail != serial_rx_buf_head) {
		c = serial_rx_buf[serial_rx_buf_head];
		serial_rx_buf_head++;
		if (serial_rx_buf_head == SERIAL_RX_BUF_SIZE_HOST) {
			serial_rx_buf_head = 0;
		}
	}
	cpu_irq_enable();
	return c;
}

uint8_t buffered_uart_getchar(void)
{
	int c; 
	do {
		c = buffered_uart_getchar_nowait();
	} while (c < 0);
	return (uint8_t)c;
}

#define UNSAFE_HACK
int buffered_uart_is_empty(void)
{
#ifdef UNSAFE_HACK
	return serial_rx_buf_tail == serial_rx_buf_head;
#else
	int r;
	cpu_irq_disable();
	r = serial_rx_buf_tail == serial_rx_buf_head;
	cpu_irq_enable();
	return r;
#endif
}

#if SAMD || SAMR21 || SAML21
//void USART_HOST_ISR_VECT(uint8_t instance)
void USART_HOST_ISR_VECT(void)
#else
USART_HOST_ISR_VECT()
#endif
{
#if SAMD || SAML21
	while (USART_HOST->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_RXC) {
		serial_rx_buf[serial_rx_buf_tail] = USART_HOST->USART.DATA.reg;
#elif SAMR21 
	while (usart_serial_is_rx_ready(&host_uart_module)) {
		usart_serial_getchar(&host_uart_module, &serial_rx_buf[serial_rx_buf_tail]);
#else
	while (usart_serial_is_rx_ready(USART_HOST)) {
		usart_serial_getchar(USART_HOST, &serial_rx_buf[serial_rx_buf_tail]);
#endif
		if ((SERIAL_RX_BUF_SIZE_HOST - 1) == serial_rx_buf_tail) {
			/* Reached the end of buffer, revert back to beginning of
			 * buffer. */
			serial_rx_buf_tail = 0x00;
		} else {
			serial_rx_buf_tail++;
		}
	}
}

/* EOF */
