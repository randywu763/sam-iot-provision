/**
 * \file
 *
 * \brief AT30TSE75X driver.
 *
 * Copyright (c) 2013-2014 Atmel Corporation. All rights reserved.
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
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
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

#include "asf.h"
#include "at30tse75x.h"

/* AT30TSE75x Device Type ID for Temperature Sensor: 0b1001xxx */
#define AT30TSE75X_DEVICE_TYPE_ID_TEMP    0x48
/* AT30TSE75x Device Type ID for EEPROM: 0b1010xxx */
#define AT30TSE75X_DEVICE_TYPE_ID_EEPROM  0x50

#ifndef BOARD_AT30TSE_DEVICE_ADDR
/* Using the Default Device Address */
#define BOARD_AT30TSE_DEVICE_ADDR  0x07 /* 0b111 */
#endif

#ifndef BOARD_AT30TSE_TWI
/* Using the Default TWI Instance */
#define BOARD_AT30TSE_TWI  TWI0
#endif

#ifndef BOARD_AT30TSE_TWI_ID
/* Using the Default TWI Instance ID */
#define BOARD_AT30TSE_TWI_ID  ID_TWI0
#endif

#ifndef BOARD_TWI_SPEED
/* Using the Default TWI Bus Speed */
#define BOARD_TWI_SPEED  10000
#endif

#if (BOARD_USING_AT30TSE != AT30TSE752) && \
		(BOARD_USING_AT30TSE != AT30TSE754) && \
		(BOARD_USING_AT30TSE != AT30TSE758) && \
		(BOARD_USING_AT30TSE != AT30TS75)
/* Using AT30TSE758 in Default */
#define BOARD_USING_AT30TSE  AT30TSE758
#endif

/* Setting the AT30TSE75x Temperature Sensor Address */
#define AT30TSE_TEMPERATURE_TWI_ADDR  (AT30TSE75X_DEVICE_TYPE_ID_TEMP | \
		(BOARD_AT30TSE_DEVICE_ADDR & 0x07))

#if BOARD_USING_AT30TSE == AT30TSE752
#define AT30TSE_EEPROM_TWI_ADDR  (AT30TSE75X_DEVICE_TYPE_ID_EEPROM | \
		(BOARD_AT30TSE_DEVICE_ADDR & 0x07))
#elif BOARD_USING_AT30TSE == AT30TSE754
#define AT30TSE_EEPROM_TWI_ADDR  (AT30TSE75X_DEVICE_TYPE_ID_EEPROM | \
		(BOARD_AT30TSE_DEVICE_ADDR & 0x06))
#elif BOARD_USING_AT30TSE == AT30TSE758
#define AT30TSE_EEPROM_TWI_ADDR  (AT30TSE75X_DEVICE_TYPE_ID_EEPROM | \
		(BOARD_AT30TSE_DEVICE_ADDR & 0x04))
#elif BOARD_USING_AT30TSE != AT30TS75
/* Using AT30TSE758 in Default */
#define AT30TSE_EEPROM_TWI_ADDR  (AT30TSE75X_DEVICE_TYPE_ID_EEPROM | \
		(BOARD_AT30TSE_DEVICE_ADDR & 0x04))
#endif


volatile uint8_t resolution = AT30TSE_CONFIG_RES_9_bit;

/**
 * \brief Initialize the TWI instance used for AT30TSE75x.
 */
void at30tse_init(void)
{
	twi_options_t opts = {
		.master_clk = sysclk_get_cpu_hz(),
		.speed = BOARD_TWI_SPEED,
		.smbus = 0
	};

	sysclk_enable_peripheral_clock(BOARD_AT30TSE_TWI_ID);
	twi_master_init(BOARD_AT30TSE_TWI, &opts);
}

#if BOARD_USING_AT30TSE != AT30TS75
/**
 * \brief Write EEPROM in AT30TSE75x
 *
 * \param data   Pointer to the data to be written
 * \param length  Length of the data to be written
 * \param word_addr  Start EEPROM address to be written
 * \param page  Specify the page to be written
 *
 * \return TWI_SUCCESS if success, otherwise false
 */
uint8_t at30tse_eeprom_write(uint8_t *data, uint8_t length,
		uint8_t word_addr, uint8_t page)
{
	twi_packet_t packet = {
		/*
		 * Internal chip addr
		 * Byte addr in page (0-15) + 4 lower bits of page addr in EEPROM
		 */
		.addr[0] = (word_addr & 0x0F) | ((0x0F & page) << 4),
		.addr_length = 1,
		/* Data buffer */
		.buffer = data,
		.length = length,
		/*
		 * Chip addr
		 * TWI addr + 2 upper bytes of page addr.
		 */
		.chip = AT30TSE_EEPROM_TWI_ADDR | ((0x30 & page) >> 4),
	};

	return twi_master_write(BOARD_AT30TSE_TWI, &packet);
}

/**
 * \brief Read EEPROM in AT30TSE75x
 *
 * \param data   Pointer to the data to be read
 * \param length  Length of the data to be read
 * \param word_addr  Start EEPROM address to be read
 * \param page  Specify the page to be read
 *
 * \return TWI_SUCCESS if success, otherwise false
 */
uint8_t at30tse_eeprom_read(uint8_t *data, uint8_t length,
		uint8_t word_addr, uint8_t page)
{
	twi_packet_t packet = {
		/*
		 * Internal chip addr
		 * Byte addr in page (0-15) + 4 lower bits of page addr in EEPROM
		 */
		.addr[0] = (word_addr & 0x0F) | ((0x0F & page) << 4),
		.addr_length = 1,
		/* Data buffer */
		.buffer = data,
		.length = length,
		/*
		 * Chip addr
		 * TWI addr + 2 upper bytes of page addr.
		 */
		.chip = AT30TSE_EEPROM_TWI_ADDR | ((0x30 & page) >> 4),
	};

	return twi_master_read(BOARD_AT30TSE_TWI, &packet);
}
#endif

/**
 * \brief Read register in AT30TSE75x
 *
 * \param reg   Register in AT30TSE75x to be read
 * \param reg_type  Specify the type of the register, nonvolatile or not
 * \param reg_size  Read lenght
 * \param buffer  Pointer to the buffer where the read data will be stored
 *
 * \return TWI_SUCCESS if success, otherwise false
 */
uint8_t at30tse_read_register(uint8_t reg, uint8_t reg_type,
		uint8_t reg_size, uint8_t* buffer)
{
	twi_packet_t packet = {
		/* Internal chip addr */
		.addr[0] = reg | reg_type,
		.addr_length = 1,
		/* Data buffer */
		.buffer = buffer,
		.length = reg_size,
		/* Chip addr */
		.chip = AT30TSE_TEMPERATURE_TWI_ADDR
	};

	return twi_master_read(BOARD_AT30TSE_TWI, &packet);
}

/**
 * \brief Write register in AT30TSE75x
 *
 * \param reg   Register in AT30TSE75x to be read
 * \param reg_type  Specify the type of the register, nonvolatile or not
 * \param reg_size  Write lenght
 * \param reg_value  Value to be written
 *
 * \return TWI_SUCCESS if success, otherwise false
 */
uint8_t at30tse_write_register(uint8_t reg, uint8_t reg_type,
		uint8_t reg_size, uint16_t reg_value)
{
	uint8_t data[2];
	twi_packet_t packet = {
		/* Internal chip addr */
		.addr[0] = reg | reg_type,
		.addr_length = 1,
		/* Data buffer */
		.buffer = data,
		.length = reg_size,
		/* Chip addr */
		.chip = AT30TSE_TEMPERATURE_TWI_ADDR
	};

	/* cppcheck-suppress unreadVariable */
	data[0] = 0x00FF & (reg_value >> 8);
	/* cppcheck-suppress unreadVariable */
	data[1] = 0x00FF & reg_value;

	return twi_master_write(BOARD_AT30TSE_TWI, &packet);
}

/**
 * \brief Write nonvolatile configuration register in AT30TSE75x
 *
 * \param value  Value to be written
 *
 * \return TWI_SUCCESS if success, otherwise false
 */
uint8_t at30tse_write_config_register(uint16_t value)
{
	uint8_t error_code = at30tse_write_register(AT30TSE_CONFIG_REG,
			AT30TSE_NON_VOLATILE_REG, AT30TSE_CONFIG_REG_SIZE - 1, value);

	if (error_code == TWI_SUCCESS) {
		resolution = (value >> AT30TSE_CONFIG_RES_Pos) &
				(AT30TSE_CONFIG_RES_Msk >> AT30TSE_CONFIG_RES_Pos);
	}

	return error_code;
}

/**
 * \brief Read temperature
 *
 * \param temperature  Pointer to the buffer where the temperature will be
 * stored
 *
 * \return TWI_SUCCESS if success, otherwise false
 */
uint8_t at30tse_read_temperature(double *temperature)
{
	/* Placeholder buffer to put temperature data in. */
	uint8_t buffer[2];
	uint8_t error_code = 0;
	buffer[0] = 0;
	buffer[1] = 0;

	/* Read the 16-bit temperature register. */
	error_code = at30tse_read_register(AT30TSE_TEMPERATURE_REG,
			AT30TSE_NON_VOLATILE_REG, AT30TSE_TEMPERATURE_REG_SIZE, buffer);

	/* Only convert temperature data if read success. */
	if (error_code == TWI_SUCCESS) {
		uint16_t data = (buffer[0] << 8) | buffer[1];
		int8_t sign = 1;

		/* Check if negative and clear sign bit. */
		if (data & (1 << 15)) {
			sign *= -1;
			data &= ~(1 << 15);
		}

		/* Convert to temperature. */
		switch (resolution) {
		case AT30TSE_CONFIG_RES_9_bit:
			data = (data >> 7);
			*(temperature) = data * sign * 0.5;
			break;

		case AT30TSE_CONFIG_RES_10_bit:
			data = (data >> 6);
			*(temperature) = data * sign * 0.25;
			break;

		case AT30TSE_CONFIG_RES_11_bit:
			data = (data >> 5);
			*(temperature) = data * sign * 0.125;
			break;

		case AT30TSE_CONFIG_RES_12_bit:
			data = (data >> 4);
			*(temperature) = data * sign * 0.0625;
			break;

		default:
			break;
		}
	}

	return error_code;
}
