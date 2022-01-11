/*******************************************************************************
© 2017 Microchip Technology Inc. and its subsidiaries.  You may use this
software and any derivatives exclusively with Microchip products.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS,
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES
OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE, OR
ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR
USE IN ANY APPLICATION.

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER
RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF
THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED
BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY
TO MICROCHIP FOR THIS SOFTWARE.

MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
TERMS.
****************************************************************************/
#include <asf.h>
#include <string.h>
#include "at_cmd_handler/serial_input.h"
#include "at_cmd_handler/at_cmd_Handler.h"

#define AT_COMMAND_PARSER_FLAG_ESCAPE 1

struct serial_input_parser {
	uint8_t flags;
	uint8_t rsvd;
	uint16_t data_len;
	uint8_t  buffer[AT_MAX_RX_BUFF_SIZE];
};
static struct serial_input_parser serial_input;

//------------------------------------------------------------------------------
int serial_input_parse(char c)
{
    serial_input.buffer[serial_input.data_len++] = c;

    if (serial_input.data_len == 1) {
        switch(serial_input.buffer[0]) {
        case 0x1B: 
		    // ESC
			// Not sure how to use ESC command.
            serial_input.flags |= AT_COMMAND_PARSER_FLAG_ESCAPE;
            break;
        case 0x12:
            /* We have received the identity request command from the programmer. We are about to flip to Serial Bridge Mode.
               The UART will no longer be used for debugging purposes. Instead, it will be used to communicate with the
               programmer */ 
            return SERIAL_INPUT_SBMODE;
        case '\r':
        case '\n':
            serial_input.data_len = 0;
            return SERIAL_INPUT_NEW_LINE;
	    case 'A':
		case 'H':
			break;
		default:
	        return SERIAL_INPUT_INVALID;

        }
    } else {
        if ((serial_input.flags & AT_COMMAND_PARSER_FLAG_ESCAPE) == 0) {
			if (serial_input.buffer[0] == 'A' && serial_input.data_len <= 3) {
				if (memcmp(serial_input.buffer, "AT+", serial_input.data_len) != 0) {
						return SERIAL_INPUT_INVALID;
				}
			}
			if (serial_input.buffer[0] == 'H' && serial_input.data_len <= 4) {
				if (memcmp(serial_input.buffer, "HELP", serial_input.data_len) != 0) {
						return SERIAL_INPUT_INVALID;
				}
			}
		}       	
		
		if ( c == '\r' || c == '\n') {
			serial_input.buffer[serial_input.data_len-1] = 0;
			if (serial_input.buffer[serial_input.data_len-2] == '\r') {
				serial_input.buffer[serial_input.data_len-2] = 0;
			}
			return SERIAL_INPUT_VALID;
		}
		
		if(c == 0x08) // Backspace
		{
			if(serial_input.data_len > 1)
			{
				// Remove the backspace and the char before it from the buffer
				serial_input.buffer[serial_input.data_len - 1] = 0;
				serial_input.buffer[serial_input.data_len - 2] = 0;
				serial_input.data_len -= 2;
				return SERIAL_INPUT_BACKSPACE;
			}
			else if(serial_input.data_len == 1)
			{
				// Only remove the backspace since thats the only thing in the buffer
				serial_input.buffer[serial_input.data_len - 1] = 0;
				serial_input.data_len--;
				return SERIAL_INPUT_BACKSPACE;
			}
		}

		if (serial_input.data_len == AT_MAX_RX_BUFF_SIZE) {
			return -SERIAL_INPUT_INVALID;
		}
	}

    return SERIAL_INPUT_WAITING_MORE_DATA;
}
//------------------------------------------------------------------------------
void serial_input_reset(void)
{
    memset(&serial_input, 0, sizeof(serial_input));;
}
//------------------------------------------------------------------------------
const uint8_t * serial_input_get_buffer(void)
{
	return serial_input.buffer;
}
