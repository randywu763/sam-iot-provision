/**
	Hacked version of sio2host.

	The hacking is pretty light, really it is so you get another instance and avoid 
	clashing with the EDB port. But it also takes out the tie to stdio, for stdio via
	EDBG port use "Standard Serial I/O (stdio)(driver)".

	Sorry there seems to be no constant naming scheme that I can set up
	as USART%%PORT ect., so you will have to edit for port all thru here
 */

#ifndef CONF_BUFFERED_UART_H_INCLUDED
#define CONF_BUFFERED_UART_H_INCLUDED
#define SERIAL_RX_BUF_SIZE_HOST    256

#warning \
	"Using a default value. Edit this file to modify that define value according to the current board."
/* ! \name Configuration for Xmega */
/* ! @{ */
#if (XMEGA)
#define USART_HOST                &USARTC0
#define USART_HOST_BAUDRATE       9600
#define USART_HOST_CHAR_LENGTH    USART_CHSIZE_8BIT_gc
#define USART_HOST_PARITY         USART_PMODE_DISABLED_gc
#define USART_HOST_STOP_BITS      false

#define USART_HOST_RX_ISR_ENABLE() usart_set_rx_interrupt_level(USART_HOST, \
		USART_INT_LVL_HI)
#define USART_HOST_ISR_VECT()      ISR(USARTC0_RXC_vect)
#endif /* XMEGA */
/* ! @} */

/* ! \name Configuration for MegaRF */
/* ! @{ */
#if (MEGA_RF)
#define USART_HOST                &USARTA1
#define USART_HOST_BAUDRATE       9600
#define USART_HOST_CHAR_LENGTH    USART_CHSIZE_8BIT_gc
#define USART_HOST_PARITY         USART_PMODE_DISABLED_gc
#define USART_HOST_STOP_BITS      false

#define USART_HOST_RX_ISR_ENABLE() usart_rx_complete_interrupt_enable(USART_HOST)
#define USART_HOST_ISR_VECT()      ISR(USART1_RX_vect)
#endif /* MEGA_RF */
/* ! @} */

/* ! \name Configuration for UC3 */
/* ! @{ */
#if (UC3)
#define USART_HOST                &AVR32_USART0
#define USART_HOST_BAUDRATE       9600
#define USART_HOST_CHAR_LENGTH    8
#define USART_HOST_PARITY         USART_NO_PARITY
#define USART_HOST_STOP_BITS      USART_1_STOPBIT

#define USART_HOST_RX_ISR_ENABLE()
#define USART_HOST_ISR_VECT()     ISR(host_uart_isr, 2, 1)
#endif /* UC3 */
/* ! @} */

/* ! \name Configuration for SAM4L */
/* ! @{ */
#if (SAM && !(SAMD || SAMR21 || SAML21))
#if SAM4S
	/* Configuration for console uart IRQ handler */
	#define USART_HOST_ISR_VECT()      ISR(UART1_Handler)
	/* UART Host IRQ Number */
	#define USART_HOST_IRQn            UART1_IRQn
	
	#define USART_HOST                 UART1

#elif SAMG55
	/* Configuration for console uart IRQ handler */
	#define USART_HOST_ISR_VECT()      ISR(FLEXCOM6_Handler)
	/* UART Host IRQ Number */
	#define USART_HOST_IRQn            FLEXCOM6_IRQn	

	#define USART_HOST                 USART6
#else
	/* Configuration for console uart IRQ handler */
	#define USART_HOST_ISR_VECT()      ISR(USART0_Handler)
	/* UART Host IRQ Number */
	#define USART_HOST_IRQn            USART0_IRQn

    #define USART_HOST                 USART6
#endif
    /* / ** Baudrate setting * / */
    #define USART_HOST_BAUDRATE        115200
    /* / ** Character length setting * / */
    #define USART_HOST_CHAR_LENGTH     US_MR_CHRL_8_BIT
    /* / ** Parity setting * / */
    #define USART_HOST_PARITY          US_MR_PAR_NO
    /* / ** Stop bits setting * / */
    #define USART_HOST_STOP_BITS       US_MR_NBSTOP_1_BIT
    #define USART_HOST_RX_ISR_ENABLE() \
	    do {\
		    usart_enable_interrupt(USART_HOST, US_IER_RXRDY); \
	        NVIC_EnableIRQ(USART_HOST_IRQn);\
		} while (0)
	#define USART_HOST_RX_ISR_DISABLE() \
		do {\
			usart_disable_interrupt(USART_HOST, US_IER_RXRDY); \
			NVIC_DisableIRQ(USART_HOST_IRQn);\
		} while (0)
#endif /* SAM */

/* ! \name Configuration for SAMD20 */
/* ! @{ */
#if (SAMD || SAMR21 || SAML21)
#define USART_HOST                 SERCOM0
#define HOST_SERCOM_MUX_SETTING    USART_RX_1_TX_0_XCK_1
#define HOST_SERCOM_PINMUX_PAD0    PINMUX_PA04D_SERCOM0_PAD0
#define HOST_SERCOM_PINMUX_PAD1    PINMUX_PA05D_SERCOM0_PAD1
#define HOST_SERCOM_PINMUX_PAD2    PINMUX_UNUSED
#define HOST_SERCOM_PINMUX_PAD3    PINMUX_UNUSED
/** Baudrate setting */
#define USART_HOST_BAUDRATE        9600

#define USART_HOST_RX_ISR_ENABLE()  _sercom_set_handler(0, USART_HOST_ISR_VECT); \
	USART_HOST->USART.INTENSET.reg = SERCOM_USART_INTFLAG_RXC; \
	system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_SERCOM0);
#endif /* SAMD || SAMR21 || SAML21 */

/* ! @} */
#endif /* CONF_BUFFERED_UART_H_INCLUDED */
