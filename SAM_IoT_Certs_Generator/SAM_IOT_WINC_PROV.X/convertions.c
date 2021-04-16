/* 
    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#include <stddef.h>
#include <string.h>
#include "convertions.h"

bool is_hexdigit(const char c)
{
    return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}


uint8_t hex2nibble(uint8_t hex)
{
    if ((hex <= '9') && (hex >= '0'))
    {
        hex -= '0';
    }
    else if ((hex <= 'F' ) && (hex >= 'A'))
    {
        hex -= ('A' - 10);
    }
    else if ((hex <= 'f') && (hex >= 'a'))
    {
        hex -= ('a' - 10);
    }
    else
    {
        hex = 0;
    }

    return hex;
}

/**
 * \brief Converts an nibble to a ASCII hex character.
 *
 * \param[in] nibble       The nibble value to be converted
 *
 * \return    The ASCII hex value
 */
uint8_t nibble2hex(uint8_t nibble)
{
    nibble &= 0x0F;

    if (nibble <= 0x09)
    {
        nibble += '0';
    }
    else
    {
        nibble += ('A' - 10);
    }

    return nibble;
}

/**
 * \brief Converts an ASCII hex buffer to a separate binary buffer.
 *
 * \param[in]     length   The length of the ASCII buffer
 * \param[in]     source   Source buffer
 * \param[out]    target  Target buffer
 *
 * \return    The length of the binary buffer
 */
uint16_t convert_hex2bin(uint16_t length, const uint8_t *source, uint8_t *target)
{
    uint16_t i;
    const uint8_t *p;
    uint8_t byte;

    if ((source == NULL) || (length < 2))
        return 0;
    length = (length >> 1) + (length & 1);  // string length -> binary length
    for (i = 0, p = source; i < length; i++)
    {
        byte = hex2nibble(*p++) << 4;
        byte |= hex2nibble(*p++);
        target[i] = byte;
    }
    return i;
}



/**
 * \brief Converts an binary buffer to a separate ASCII null-terminated hex string.
 *
 * \note  The target buffer must have the allocated space needed to store a
 *        nul-terminated C ASCII string representation of the binary data,
 *        ie 2*length + 1
 *
 * \param[in]     length   The length of the binary buffer
 * \param[in]     source   The source binary buffer
 * \param[out]    target   The ASCII nul-terminated hex string
 *
 * \return    The length of the ASCII null-terminated hex buffer excluding 0 terminator
 */
uint16_t convert_bin2hex(uint16_t length, const uint8_t *source, uint8_t *target)
{
    uint16_t hex_length = 0;

    if ((source == NULL) || (length == 0))
    {
        return 0;
    }
    for (uint16_t index = 0; index < length; index++)
    {
        target[hex_length++] = (char)nibble2hex((source[index] >> 4));
        target[hex_length++] = (char)nibble2hex((source[index] & 0x0F));
    }
    target[hex_length] = '\0';
    return hex_length;
}


/**
 * \brief Converts an binary integer to ASCII null-terminated hex
 *
 * \note  this is different from using convert_bin2hex above on little-
 *        endian architecture, this implementation is endianness-independent.
 *
 * \param[in]     num       The 16-bit number to convert
 * \param[out]    buf       The ASCII nul-terminated hex string
 *
 * \return    The length of the ASCII null-terminated hex buffer excluding
 *            0 termination (ie 4)
 */
uint8_t convert_itohex(uint16_t num, uint8_t *buf)
{
    uint8_t i;
    for (i = 0; i < 2*sizeof(num); i++)
       *buf++ = nibble2hex((num >> (4*((2*sizeof(num) - 1) - i))) & 0xf);
    *buf = '\0';
    return i; 
}


/**
 * \brief Copy a string, like strcpy(), but return pointer to the end of 
 *        destination string rather than the beginning.
 *        This standard lib function should have been in avr-libc,
 *        but seems to be missing.
 *        Eliminates the need to use strcat() and hex conversion with
 *        static intermediate buffer.
 *
 * \param[in]     dest      Pointer to destination
 * \param[in]     src       Source string
 *
 * \return        Pointer to end of destination string (the \0 terminator)
 */
char *stpcpy(char *dest, const char *src) {
	const size_t length = strlen(src);
    memcpy(dest, src, length + 1);
    return dest + length;
}
