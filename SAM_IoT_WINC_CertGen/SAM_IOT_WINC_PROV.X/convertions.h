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

#ifndef CONVERTIONS_H_
#define CONVERTIONS_H_

#include <stdint.h>
#include <stdbool.h>

// Target buffer must be at least length/2
uint16_t convert_hex2bin(uint16_t length, const uint8_t *source, uint8_t *target);
// Target buffer size must be at least 2*length + 1 
uint16_t convert_bin2hex(uint16_t length, const uint8_t *source, uint8_t *target);
uint8_t convert_itohex(uint16_t num, uint8_t *buf);
// Missing library function in avr-libc
char *stpcpy(char *dest, const char *src);
// Convert a hex digit ('0'..'F') to binary
uint8_t hex2nibble(uint8_t hex);
uint8_t nibble2hex(uint8_t nibble);

// Should use ctype function isxdigit() here but uncertain about its memory usage.
bool is_hexdigit(const char c);

#endif /* CONVERTIONS_H_ */
