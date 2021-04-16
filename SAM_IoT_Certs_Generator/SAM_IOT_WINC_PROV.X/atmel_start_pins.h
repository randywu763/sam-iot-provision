/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMD21 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define SW_0 GPIO(GPIO_PORTA, 0)
#define SW_1 GPIO(GPIO_PORTA, 1)
#define PA04 GPIO(GPIO_PORTA, 4)
#define PA05 GPIO(GPIO_PORTA, 5)
#define LED_YELLOW GPIO(GPIO_PORTA, 11)
#define PA12 GPIO(GPIO_PORTA, 12)
#define PA13 GPIO(GPIO_PORTA, 13)
#define CS_PIN GPIO(GPIO_PORTA, 14)
#define PA15 GPIO(GPIO_PORTA, 15)
#define CONF_WINC_EXT_INT_PIN GPIO(GPIO_PORTA, 18)
#define LED_GREEN GPIO(GPIO_PORTA, 20)
#define LED_BLUE GPIO(GPIO_PORTA, 21)
#define PA22 GPIO(GPIO_PORTA, 22)
#define PA23 GPIO(GPIO_PORTA, 23)
#define LED_RED GPIO(GPIO_PORTA, 25)
#define PB02 GPIO(GPIO_PORTB, 2)
#define PB03 GPIO(GPIO_PORTB, 3)
#define RESET_PIN GPIO(GPIO_PORTB, 10)
#define CE_PIN GPIO(GPIO_PORTB, 11)
#define WINC_WAKE GPIO(GPIO_PORTA, 19)

#endif // ATMEL_START_PINS_H_INCLUDED
