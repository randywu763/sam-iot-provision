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
#ifndef AT_COMMAND_SERIAL_INPUT
#define AT_COMMAND_SERIAL_INPUT


#define SERIAL_INPUT_NEW_LINE           -2
#define SERIAL_INPUT_INVALID            -1
#define SERIAL_INPUT_WAITING_MORE_DATA   0
#define SERIAL_INPUT_VALID               1
#define SERIAL_INPUT_SBMODE              2
#define SERIAL_INPUT_BACKSPACE           3

/**
 * \brief Initializes serial Input parser
 * \return SERIAL_INPUT_VALID if the parser found the valid AT command input.
 */
int serial_input_parse(char c);

/**
 * \brief Reset serial input parser.
 */
void serial_input_reset(void);

/**
 * \brief 
 * \return a data buffer
 */
const uint8_t * serial_input_get_buffer(void);


#endif //AT_COMMAND_SERIAL_INPUT
