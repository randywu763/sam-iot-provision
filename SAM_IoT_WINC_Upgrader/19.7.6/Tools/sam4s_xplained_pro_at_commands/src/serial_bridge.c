/**
 *
 * \file
 *
 * \brief Serial Bridge.
 *
 * Copyright (c) 2016 Atmel Corporation. All rights reserved.
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
 
#include <asf.h>
#include "common/include/nm_common.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "driver/source/nmbus.h"
#include "driver/source/nmasic.h"
#include "driver/include/m2m_wifi.h"
#include "buffered_uart.h"
#include <string.h>

struct nm_command {
	uint16_t cmd;
	uint16_t size;
	uint32_t addr;
	uint32_t val;
} __attribute__ ((packed));

struct serial_bridge_frame {
	uint8_t op_code;
	uint8_t cmd[sizeof(struct nm_command)];
} __attribute__ ((packed));


#define PACKET_QUEUE_BUFFER_SIZE 2048
struct serial_bridge_frame_buffer {
	uint8_t buffer[PACKET_QUEUE_BUFFER_SIZE];
	uint32_t size;
};

static struct serial_bridge_frame_buffer frame_buffer;

static void serial_bridge_frame_buffer_pop(struct serial_bridge_frame_buffer *frame, int size)
{
	if (size < 0 || size > frame->size) {
		size = frame->size;
	}
	frame->size -= size;
	if (frame->size) {
		memmove(frame->buffer, &frame->buffer[size], frame->size);
	}
}

static int serial_bridge_frame_receive(struct serial_bridge_frame_buffer *frame, uint16_t len)
{
	while (frame->size < len) {
		frame->size += buffered_uart_rx(&frame->buffer[frame->size], 255);
	}
	return 0;
}

/**
 * \brief Raad out the first 13bytes and copy into cmd buffer after checking the frame.
 *  frame buffer pointer is pop'ed by this function.
 */
static int serial_bridge_frame_get_nm_command(struct serial_bridge_frame *frame, struct nm_command *cmd)
{
	uint8 checksum = 0;
	int i;
	

	for (i = 0; i < sizeof(struct nm_command); i++) {
		checksum ^= frame->cmd[i];
	}
	if (checksum == 0) {
		union {
			uint32_t i;
			char c[4];
		}
		bint = {0x01020304};

		if (bint.c[0] == 1) {
			/* Big endian. */
			cmd->cmd  = ((uint32)((frame->cmd[0]) << 8)  |\
			             (uint32)((frame->cmd[1]) << 0)  );
			cmd->size = ((uint32)((frame->cmd[2]) << 8)  |\
						 (uint32)((frame->cmd[3]) << 0)  );
			cmd->addr = ((uint32)((frame->cmd[4]) << 24) |\
			             (uint32)((frame->cmd[5]) << 16) |\
						 (uint32)((frame->cmd[6]) << 8)  |\
						 (uint32)((frame->cmd[7]) << 0)  );
			cmd->val  = ((uint32)((frame->cmd[8]) << 24) |\
			             (uint32)((frame->cmd[9]) << 16) |\
						 (uint32)((frame->cmd[10])<< 8)  |\
						 (uint32)((frame->cmd[11])<< 0)  );
		} else {
			/* Little endian. */
			memcpy(cmd, frame->cmd, sizeof(struct nm_command));
		}
	}
	serial_bridge_frame_buffer_pop(frame, sizeof(struct serial_bridge_frame));
	return checksum;
}

/**
 * \brief Handles the UART frame from nm_sync_cmd() in nmuart.c
 */
static int serial_bridge_process_sync_cmd(struct serial_bridge_frame *frame)
{
	serial_bridge_frame_buffer_pop(frame, 1);
	buffered_uart_putchar(0x5B); // WINCxx00 Serial Bridge + AT CMD app - W3400-404 report as plain serial bridge
	return 0;
}

static int serial_bridge_process_reboot_cmd(struct serial_bridge_frame *frame)
{
	NVIC_SystemReset();
	return 0;
}

/**
 * \brief Handles the UART frame from nm_read_reg_with_ret() in nmuart.c
 */
 static int serial_bridge_process_read_reg_with_ret(struct serial_bridge_frame *frame)
{
	struct nm_command cmd;
	uint32_t val;

	if (serial_bridge_frame_get_nm_command(frame, &cmd) != 0) {
		buffered_uart_putchar(0x5A); // NACK
		return -1;
	}
	buffered_uart_putchar(0xAC); // ACK

	val = nm_read_reg(cmd.addr);

	buffered_uart_putchar((uint8_t)(val >> 24));
	buffered_uart_putchar((uint8_t)(val >> 16));
	buffered_uart_putchar((uint8_t)(val >> 8));
	buffered_uart_putchar((uint8_t)(val >> 0));
	return 0;
}

/**
 * \brief Handles the UART frame from nm_write_reg() in nmuart.c
 */
static int serial_bridge_process_write_reg(struct serial_bridge_frame *frame)
{
	struct nm_command cmd;

	if (serial_bridge_frame_get_nm_command(frame, &cmd) != 0) {
		buffered_uart_putchar(0x5A); // NACK
		return -1;
	}
	buffered_uart_putchar(0xAC); // ACK

	nm_write_reg(cmd.addr, cmd.val); 
	return 0;
}

/**
 * \brief Handles the UART frame from nm_read_block() in nmuart.c
 */
static int serial_bridge_process_read_block(struct serial_bridge_frame *frame)
{
	struct nm_command cmd;
	uint8_t reg_buffer[PACKET_QUEUE_BUFFER_SIZE];

	if (serial_bridge_frame_get_nm_command(frame, &cmd) != 0) {
		buffered_uart_putchar(0x5A); // NACK
		return -1;
	}
	buffered_uart_putchar(0xAC); // ACK

	nm_read_block(cmd.addr, reg_buffer, cmd.size);
	buffered_uart_tx(reg_buffer, cmd.size);
	return 0;
}

/**
 * \brief Handles frame from nm_uart_write_block() in nmuart.c
 */
static int serial_bridge_process_write_block(struct serial_bridge_frame *frame)
{
	struct nm_command cmd;
	sint8 r;

	if (serial_bridge_frame_get_nm_command(frame, &cmd) != 0) {
		buffered_uart_putchar(0x5A); // NACK
		return -1;
	}
	buffered_uart_putchar(0xAC); // ACK

	serial_bridge_frame_receive(frame, cmd.size);
	r = nm_write_block(cmd.addr, (uint8_t*)frame, cmd.size);
	serial_bridge_frame_buffer_pop(frame, cmd.size);
	buffered_uart_putchar(r ? 0x5A : 0xAC); // NACK or ACK

	return 0;
}

/**
 * \brief Handles the UART frame from nm_reconfigure() in nmuart.c
 */
static int serial_bridge_process_reconfigure(struct serial_bridge_frame *frame)
{
	struct nm_command cmd;

	if (serial_bridge_frame_get_nm_command(frame, &cmd) != 0) {
		buffered_uart_putchar(0x5A); // NACK
		return -1;
	}
	// ACK char must be sent before re-configuring the UART.
	buffered_uart_putchar(0xAC); // ACK

	buffered_uart_term();
	buffered_uart_init(cmd.val);
	return 0;
}

struct serial_bridge_frame_lookup {
	uint8_t header[4];
	uint32_t header_size;
	uint32_t min_size;
	int (*handler)(struct serial_bridge_frame *);
};

struct serial_bridge_frame_lookup serial_bridge_lookup_table[] = {
	{{0x12      }, 1, 1,                                  serial_bridge_process_sync_cmd         },
	{{0x13      }, 1, 1,                                  serial_bridge_process_reboot_cmd       },
	{{0xa5, 0x00}, 2, sizeof(struct serial_bridge_frame), serial_bridge_process_read_reg_with_ret},
	{{0xa5, 0x01}, 2, sizeof(struct serial_bridge_frame), serial_bridge_process_write_reg        },
	{{0xa5, 0x02}, 2, sizeof(struct serial_bridge_frame), serial_bridge_process_read_block       },
	{{0xa5, 0x03}, 2, sizeof(struct serial_bridge_frame), serial_bridge_process_write_block      },
	{{0xa5, 0x05}, 2, sizeof(struct serial_bridge_frame), serial_bridge_process_reconfigure      },
};

#define SERIAL_BRIDGE_LOOKUP_TABLE_ENTRIES sizeof(serial_bridge_lookup_table) / sizeof(struct serial_bridge_frame_lookup)


static void serial_bridge_process_frame(void)
{
	uint32_t i;

	for (i = 0; i < SERIAL_BRIDGE_LOOKUP_TABLE_ENTRIES; i++) {
		if (frame_buffer.size >= serial_bridge_lookup_table[i].min_size
		&&  !memcmp(serial_bridge_lookup_table[i].header, frame_buffer.buffer, serial_bridge_lookup_table[i].header_size)) {
			if (serial_bridge_lookup_table[i].handler((struct serial_bridge_frame*)(frame_buffer.buffer)) == 0) {
				return;
			} else {
				break;
			}
		}
	}

	if (frame_buffer.buffer[0] == 0xFF) {
		serial_bridge_frame_buffer_pop(&frame_buffer, -1);
	} else if (frame_buffer.buffer[0] != 0x12 && frame_buffer.buffer[0] != 0xA5) {
		buffered_uart_putchar(0xEA); //??
		serial_bridge_frame_buffer_pop(&frame_buffer, -1);
	}
}

sint8 enter_wifi_firmware_download(void)
{
	int n;

	if (cpu_start() != M2M_SUCCESS) {
		puts("Failed to put the WiFi Chip in download mode!\n");
		return M2M_ERR_INIT;
	}

	frame_buffer.size = 0;
	memset(frame_buffer.buffer, 0, sizeof(frame_buffer.buffer));
	
	// 0x12 triggers AT command to call this function.
	serial_bridge_process_sync_cmd(&frame_buffer);

	while (1) {
		n = buffered_uart_rx(&frame_buffer.buffer[frame_buffer.size], 255);
		if (n) {
		    frame_buffer.size += n;
			serial_bridge_process_frame();
		}
	}
	return 0;
}
