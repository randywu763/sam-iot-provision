/**
 *
 * \file
 *
 * \brief This module implements Base64 Decoder.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common/include/nm_common.h"
#include "pem.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


typedef struct{
    tenuPEMEncType  enuType;
    char            acBeginPhrase[256];
    char            acEndPhrase[256];
}tstrPemDecInfo;


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
GLOBALS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

static uint8 gpau8Ascii2B64[128] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
    0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
    0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
/*!<
    Lookup table that maps the ascii characters to their base-64 equivalent.
    The unused characters takes a value of 0xFF.\
*/


static uint8            gpau8B64Table[65]    = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

tstrPemDecInfo      gastrPemTypes[] = {
    {PEM_X509_CERT      , "-----BEGIN CERTIFICATE-----"     , "-----END CERTIFICATE-----"},
    {PEM_PRIV_KEY       , "-----BEGIN PRIVATE KEY-----"     , "-----END PRIVATE KEY-----"},
    {PEM_RSA_PRIV_KEY   , "-----BEGIN RSA PRIVATE KEY-----" , "-----END RSA PRIVATE KEY-----"}
};

/**********************************************************************
Function
    Base64_EncodeBlock

Description

Return
    None

Author
    Ahmed Ezzat

Version
    1.0

Date
    15 Aug 2010
***********************************************************************/
static void Base64_EncodeBlock(uint8 * pu8BinaryData, uint8 * pu8B64EncodedData)
{
    uint8 u8Idx0,u8Idx1,u8Idx2,u8Idx3;

    /* Divide the 3 byte input array into four 6-bit groups */
    u8Idx0 = (pu8BinaryData[0] & 0xFC) >> 2;
    u8Idx1 = ((pu8BinaryData[0] & 0x03) << 4) + ((pu8BinaryData[1]  &  0xF0 ) >> 4);
    u8Idx2 = ((pu8BinaryData[1] & 0x0F) << 2) + (pu8BinaryData[2] >> 6);
    u8Idx3 = pu8BinaryData[2] & 0x3F;

    /* Find the corresponding Base64 character in the table */
    pu8B64EncodedData[0] = gpau8B64Table[u8Idx0];
    pu8B64EncodedData[1] = gpau8B64Table[u8Idx1];
    pu8B64EncodedData[2] = gpau8B64Table[u8Idx2];
    pu8B64EncodedData[3] = gpau8B64Table[u8Idx3];
}
/**********************************************************************
Function
    Base64_Encode

Description

Return
    None

Author
    Ahmed Ezzat

Version
    1.0

Date
    15 Aug 2010
***********************************************************************/
uint32 Base64_Encode(uint8 *pu8BinData, uint32 u32BinSz, uint8 *pu8Out)
{
    uint32  u32OutSz = 0;

    if((pu8BinData != NULL) && (pu8Out != NULL))
    {
        uint32  u32BinCounter = 0,u32B64Counter = 0;
        uint32  u32Rem;

        u32Rem      = u32BinSz % 3;
        u32OutSz    = ((u32BinSz + 2 - ((u32BinSz + 2) % 3)) / 3) * 4; /* Output size */

        /* Start encoding */
        for(u32BinCounter = 0; u32BinCounter < (u32BinSz - u32Rem); u32BinCounter += 3)
        {
            if((u32BinCounter != 0) && (u32BinCounter % 48) == 0)
            {
                pu8Out[u32B64Counter ++] = '\r';
                pu8Out[u32B64Counter ++] = '\n';
            }

            /* Encode a 3-byte binary block into 4 base64 characters */
            Base64_EncodeBlock(&pu8BinData[u32BinCounter],&pu8Out[u32B64Counter]);
            u32B64Counter += 4;
        }

        /*===== Check if padding is required =====*/
        if(u32Rem == 1)
        {
            uint8 u8Index0,u8Index1;
            u8Index0 = (pu8BinData[u32BinCounter] & 0xFC) >> 2;
            u8Index1 = (pu8BinData[u32BinCounter] & 0x03) << 4;
            pu8Out[u32B64Counter ++] = gpau8B64Table[u8Index0];
            pu8Out[u32B64Counter ++] = gpau8B64Table[u8Index1];
            pu8Out[u32B64Counter ++] = '=';
            pu8Out[u32B64Counter ++] = '=';
        }
        else if(u32Rem == 2)
        {
            uint8 u8Index0,u8Index1,u8Index2;
            u8Index0 = (pu8BinData[u32BinCounter] & 0xFC) >> 2;
            u8Index1 = ((pu8BinData[u32BinCounter] & 0x03) << 4) + ((pu8BinData[u32BinCounter + 1]  &  0xF0 ) >> 4);
            u8Index2 = (pu8BinData[u32BinCounter + 1] &0x0F) << 2;
            pu8Out[u32B64Counter ++] = gpau8B64Table[u8Index0];
            pu8Out[u32B64Counter ++] = gpau8B64Table[u8Index1];
            pu8Out[u32B64Counter ++] = gpau8B64Table[u8Index2];
            pu8Out[u32B64Counter ++] = '=';
        }
        u32OutSz = u32B64Counter;
    }
    return u32OutSz;
}

/**********************************************************************
Function
    Base64_DecodeBlock

Description
    Convert a 4 Base-64 characters (Base64_Block) into their 3 Bytes equivalent.

Return
    None

Author
    Ahmed Ezzat

Version
    1.0

Date
    15 Aug 2010
***********************************************************************/
static void Base64_DecodeBlock(uint8 *pu8B64Data, uint8 *pu8BinData)
{
    uint8 u8B0,u8B1,u8B2,u8B3;

    /* Obtain the binary value of the 4 base-64 characters */
    u8B0 = gpau8Ascii2B64[pu8B64Data[0]];
    u8B1 = gpau8Ascii2B64[pu8B64Data[1]];
    u8B2 = gpau8Ascii2B64[pu8B64Data[2]];
    u8B3 = gpau8Ascii2B64[pu8B64Data[3]];

    /* Convert to binary */
    pu8BinData[0] = (u8B0 << 2) + (u8B1 >> 4);
    pu8BinData[1] = (u8B1 << 4) + (u8B2 >> 2);
    pu8BinData[2] = (u8B2 << 6) + (u8B3);
}
/**********************************************************************
Function
    Base64_Decode

Description
    Decode a message encoded in Base-64 to the binary equivalent.

Return
    0 for success and -1 otherwise

Author
    Ahmed Ezzat

Version
    1.0

Date
    15 Aug 2010
***********************************************************************/
sint8 Base64_Decode
(
uint8   *pu8EncData,
uint32  u32EncLength,
uint8   **ppu8DecData,
uint32  *pu32DecLen
)
{
    sint8 s8RetVal = 0;

    if((pu8EncData != NULL) && (ppu8DecData != NULL))
    {
        uint32  u32DecLen, u32B64Counter,u32BinCounter, u32InLength;
        uint8   *pu8DecData, *pu8Base64Data;

        pu8Base64Data = (uint8*)malloc(u32EncLength);
        if(pu8Base64Data != NULL)
        {
            uint32    u32Idx;

            u32InLength = 0;
            for(u32Idx = 0; u32Idx < u32EncLength; u32Idx ++)
            {
                if((pu8EncData[u32Idx] != '\n') && (pu8EncData[u32Idx] != '\r') && (pu8EncData[u32Idx] != ' '))
                {
                    pu8Base64Data[u32InLength ++] = pu8EncData[u32Idx];
                }
            }

            u32DecLen = (u32InLength * 3)/4;

            /* Allocate memory for the output */
            pu8DecData = (uint8*)malloc(u32DecLen);
            if(pu8DecData != NULL)
            {
                uint32 u32NumPaddedBytes = 0;
                u32B64Counter = 0;
                u32BinCounter = 0;

                /* Check if there is any padding(i.e. the last byte must be '=' */
                if(pu8Base64Data[u32InLength -1] == '=')
                {
                    /* Replaces the padding bytes with A (will be decoded to 0) for easing the decoding.
                    This will corrupt the input data, so they will be restored after decoding is finished. */
                    pu8Base64Data[u32InLength - 1] = 'A';
                    u32NumPaddedBytes = 1;
                    if(pu8Base64Data[u32InLength -2] == '=')
                    {
                        /* The last two characters are "==", so there are two padded bytes */
                        u32DecLen -= 2;
                        pu8Base64Data[u32InLength - 2] = 'A';
                        u32NumPaddedBytes = 2;
                    }
                    else
                    {
                        /* The last character is "=", so there is one padded byte */
                        u32DecLen--;
                    }
                }

                /* Decoding ==> Loop on the input in 4-characters blocks */
                while(u32B64Counter < u32InLength)
                {
                    /* Decode block of data 4-base64 characters into 3 bytes */
                    Base64_DecodeBlock(&pu8Base64Data[u32B64Counter], &pu8DecData[u32BinCounter]);
                    u32BinCounter += 3;
                    u32B64Counter += 4;
                }

                /* Restore the overwritten bytes (in case of padding) */
                if(u32NumPaddedBytes != 0)
                {
                    pu8Base64Data[u32InLength - 1] = '=';
                    if(u32NumPaddedBytes == 2)
                    {
                        pu8Base64Data[u32InLength - 2] = '=';
                    }
                }
                s8RetVal = 0;
                *pu32DecLen     = u32DecLen;
                *ppu8DecData    = pu8DecData;
            }
            else
            {
                /* Memory Allocation failure */
                s8RetVal = -1;
            }
            free(pu8Base64Data);
        }
        else
        {
            s8RetVal = -1;
        }
    }
    else
    {
        /* Invalid inputs */
        s8RetVal = -1;
    }
    return s8RetVal;
}
/*********************************************************************
Function
    DecodeBase64File

Description


Return


Author
    Ahmed Ezzat

Version
    1.0

Date
    29 June 2015
*********************************************************************/
sint8 DecodeBase64File(uint8 *pu8B64File, uint32 u32FileSize, uint8 **ppu8Key, uint32 *pu32KeySize, tenuPEMEncType *penuPemType)
{
    sint8 ret = M2M_SUCCESS;

    *penuPemType = PEM_TYPE_NONE;

    if(pu8B64File != NULL)
    {
        uint8  u8Idx;
        uint8  *pu8B64Key;
        uint32 u32Tmp;

        pu8B64Key = pu8B64File;

        for(u8Idx = 0; u8Idx < sizeof(gastrPemTypes) / sizeof(tstrPemDecInfo); u8Idx ++)
        {
            u32Tmp = (uint32)strlen(gastrPemTypes[u8Idx].acBeginPhrase);
            if(!strncmp((char*)pu8B64Key, gastrPemTypes[u8Idx].acBeginPhrase, u32Tmp))
            {
                ret = M2M_ERR_FAIL;
                pu8B64Key += u32Tmp;
                u32FileSize -= u32Tmp;
                while((*pu8B64Key == '\n') || (*pu8B64Key == '\r') || (*pu8B64Key == ' '))
                {
                    pu8B64Key ++;
                    u32FileSize --;
                }

                while((pu8B64Key[u32FileSize - 1] == '\n') || (pu8B64Key[u32FileSize - 1] == '\r') ||
                    (pu8B64Key[u32FileSize - 1] == ' '))
                {
                    u32FileSize --;
                }
                u32Tmp = (uint32)strlen(gastrPemTypes[u8Idx].acEndPhrase);
                if(!strncmp((char*)&pu8B64Key[u32FileSize - u32Tmp], gastrPemTypes[u8Idx].acEndPhrase, u32Tmp))
                {
                    u32FileSize -= u32Tmp;
                    if(Base64_Decode(pu8B64Key, u32FileSize, ppu8Key, pu32KeySize) == M2M_SUCCESS)
                    {
                        *penuPemType = gastrPemTypes[u8Idx].enuType;
                        ret = M2M_SUCCESS;
                    }
                }
                break;
            }
        }

        if(*penuPemType == PEM_TYPE_NONE)
        {
            *ppu8Key     = pu8B64Key;
            *pu32KeySize = u32FileSize;
        }
    }
    return ret;
}
/*********************************************************************
Function
    DecodeX509Certificate

Description


Return


Author
    Ahmed Ezzat

Version
    1.0

Date
    29 June 2015
*********************************************************************/
sint8 DecodeX509Certificate(uint8 *pu8X509File, uint32 u32X509FileSz, uint8 **ppu8X509Der, uint32 *pu32X509Size)
{
    tenuPEMEncType    enuPemType;
    return DecodeBase64File(pu8X509File, u32X509FileSz, ppu8X509Der, pu32X509Size, &enuPemType);
}
