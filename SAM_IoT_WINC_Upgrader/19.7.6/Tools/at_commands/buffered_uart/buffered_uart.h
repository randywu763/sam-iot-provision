/**
	Hacked version of sio2host.

	The hacking is pretty light, really it is so you get another instance and avoid
	clashing with the EDB port. But it also takes out the tie to stdio, for stdio via
	EDBG port use "Standard Serial I/O (stdio)(driver)".
 */

#ifndef BUFFERED_UART_H_INCLUDED
#define BUFFERED_UART_H_INCLUDED

/**
 * \defgroup group_sio2host_uart SIO2HOST - UART
 * This module performs serial input/output functionalities via UART
 * @{
 */

/* === INCLUDES ============================================================ */

#include "compiler.h"
#include "status_codes.h"

/* === PROTOTYPES ============================================================
**/
/**
 * \brief check if the uart buffer is empty or not.
 * \return non zero if empy.
 * not initialized
 */
int buffered_uart_is_empty(void);

/**
 * \brief Initializes the Buffered Serial IO Module
 * \return STATUS_OK for successful initialization and FAILURE in case the IO is
 * not initialized
 */
void buffered_uart_init(uint32_t baudrate);

/**
 * \brief Terminate the Buffered Serial IO Module
 * \return STATUS_OK for successful initialization and FAILURE in case the IO is
 * not initialized
 */
void buffered_uart_term(void);

/**
 * \brief Transmits data via UART
 * \param data Pointer to the buffer where the data to be transmitted is present
 * \param length Number of bytes to be transmitted
 *
 * \return Number of bytes actually transmitted
 */
uint8_t buffered_uart_tx(uint8_t *data, uint32_t length);

/**
 * \brief Receives data from UART
 *
 * \param data pointer to the buffer where the received data is to be stored
 * \param max_length maximum length of data to be received
 *
 * \return actual number of bytes received
 */
uint8_t buffered_uart_rx(uint8_t *data, uint8_t max_length);

/**
 * \brief This function performs a blocking character receive functionality
 * \return returns the data which is received
 */
uint8_t buffered_uart_getchar(void);

/**
 * \brief This function prints the character to the host
 * \return void
 */

void buffered_uart_putchar(uint8_t);

/**
 * \brief This function performs a non-blocking character receive functionality
 * \return '-1' if no data is received or returns the data if a character is
 * received
 */
int buffered_uart_getchar_nowait(void);

#if SAMD || SAMR21 || SAML21
////void USART_HOST_ISR_VECT(uint8_t instance);
void USART_HOST_ISR_VECT(void);

#endif

#endif /* BUFFERED_UART_H_INCLUDED */
