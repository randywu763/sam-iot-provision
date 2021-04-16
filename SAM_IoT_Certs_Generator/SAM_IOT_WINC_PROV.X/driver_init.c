/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include "config/hpl_sercom_config.h"
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

struct spi_m_sync_descriptor SPI_INSTANCE;

struct usart_sync_descriptor TARGET_IO;

struct i2c_m_sync_desc Interface;

struct usart_sync_descriptor USART_0;

void EXTERNAL_IRQ_0_init(void)
{
	_gclk_enable_channel(EIC_GCLK_ID, CONF_GCLK_EIC_SRC);

	// Set pin direction to input
	gpio_set_pin_direction(SW_0, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SW_0,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SW_0, PINMUX_PA00A_EIC_EXTINT0);

	// Set pin direction to input
	gpio_set_pin_direction(SW_1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SW_1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SW_1, PINMUX_PA01A_EIC_EXTINT1);

	// Set pin direction to input
	gpio_set_pin_direction(CONF_WINC_EXT_INT_PIN, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(CONF_WINC_EXT_INT_PIN,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(CONF_WINC_EXT_INT_PIN, PINMUX_PA18A_EIC_EXTINT2);

	ext_irq_init();
}

void TARGET_IO_PORT_init(void)
{

	gpio_set_pin_function(PA04, PINMUX_PA04D_SERCOM0_PAD0);

	gpio_set_pin_function(PA05, PINMUX_PA05D_SERCOM0_PAD1);
}

void TARGET_IO_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
}

void TARGET_IO_init(void)
{
	TARGET_IO_CLOCK_init();
	usart_sync_init(&TARGET_IO, SERCOM0, (void *)NULL);
	TARGET_IO_PORT_init();
}

void SPI_INSTANCE_PORT_init(void)
{

	gpio_set_pin_level(PA12,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PA12, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA12, PINMUX_PA12C_SERCOM2_PAD0);

	gpio_set_pin_level(PA13,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PA13, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA13, PINMUX_PA13C_SERCOM2_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(PA15, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PA15,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA15, PINMUX_PA15C_SERCOM2_PAD3);
}

void SPI_INSTANCE_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM2);
	_gclk_enable_channel(SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC);
}

void SPI_INSTANCE_init(void)
{
	SPI_INSTANCE_CLOCK_init();
	spi_m_sync_init(&SPI_INSTANCE, SERCOM2);
    spi_m_sync_set_baudrate(&SPI_INSTANCE,5);
	SPI_INSTANCE_PORT_init();
}

void Interface_PORT_init(void)
{

	gpio_set_pin_pull_mode(PA22,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA22, PINMUX_PA22C_SERCOM3_PAD0);

	gpio_set_pin_pull_mode(PA23,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA23, PINMUX_PA23C_SERCOM3_PAD1);
}

void Interface_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM3);
	_gclk_enable_channel(SERCOM3_GCLK_ID_CORE, CONF_GCLK_SERCOM3_CORE_SRC);
	_gclk_enable_channel(SERCOM3_GCLK_ID_SLOW, CONF_GCLK_SERCOM3_SLOW_SRC);
}

void Interface_init(void)
{
	Interface_CLOCK_init();
	i2c_m_sync_init(&Interface, SERCOM3);
	Interface_PORT_init();
}

void USART_0_PORT_init(void)
{

	gpio_set_pin_function(PB02, PINMUX_PB02D_SERCOM5_PAD0);

	gpio_set_pin_function(PB03, PINMUX_PB03D_SERCOM5_PAD1);
}

void USART_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM5);
	_gclk_enable_channel(SERCOM5_GCLK_ID_CORE, CONF_GCLK_SERCOM5_CORE_SRC);
}

void USART_0_init(void)
{
	USART_0_CLOCK_init();
	usart_sync_init(&USART_0, SERCOM5, (void *)NULL);
	USART_0_PORT_init();
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA11

	gpio_set_pin_level(LED_YELLOW,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(LED_YELLOW, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED_YELLOW, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA14

	gpio_set_pin_level(CS_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(CS_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(CS_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA20

	gpio_set_pin_level(LED_GREEN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(LED_GREEN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED_GREEN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA21

	gpio_set_pin_level(LED_BLUE,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(LED_BLUE, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED_BLUE, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA25

	gpio_set_pin_level(LED_RED,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(LED_RED, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED_RED, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB10

	gpio_set_pin_level(RESET_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(RESET_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RESET_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB11


	gpio_set_pin_level(WINC_WAKE,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(WINC_WAKE, GPIO_DIRECTION_OUT);

gpio_set_pin_level(CE_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(CE_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(CE_PIN, GPIO_PIN_FUNCTION_OFF);

	EXTERNAL_IRQ_0_init();

	TARGET_IO_init();

	SPI_INSTANCE_init();

	Interface_init();

	USART_0_init();

	delay_driver_init();
}
