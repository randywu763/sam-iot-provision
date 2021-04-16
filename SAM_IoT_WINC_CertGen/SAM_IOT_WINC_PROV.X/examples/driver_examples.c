/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static void button_on_PA00_pressed(void)
{
}

static void button_on_PA01_pressed(void)
{
}

static void button_on_PA18_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_example(void)
{

	ext_irq_register(PIN_PA00, button_on_PA00_pressed);
	ext_irq_register(PIN_PA01, button_on_PA01_pressed);
	ext_irq_register(PIN_PA18, button_on_PA18_pressed);
}

/**
 * Example of using TARGET_IO to write "Hello World" using the IO abstraction.
 */
void TARGET_IO_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&TARGET_IO, &io);
	usart_sync_enable(&TARGET_IO);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using SPI_INSTANCE to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_INSTANCE[12] = "Hello World!";

void SPI_INSTANCE_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_INSTANCE, &io);

	spi_m_sync_enable(&SPI_INSTANCE);
	io_write(io, example_SPI_INSTANCE, 12);
}

void Interface_example(void)
{
	struct io_descriptor *Interface_io;

	i2c_m_sync_get_io_descriptor(&Interface, &Interface_io);
	i2c_m_sync_enable(&Interface);
	i2c_m_sync_set_slaveaddr(&Interface, 0x12, I2C_M_SEVEN);
	io_write(Interface_io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using USART_0 to write "Hello World" using the IO abstraction.
 */
void USART_0_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&USART_0, &io);
	usart_sync_enable(&USART_0);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

void delay_example(void)
{
	delay_ms(5000);
}
