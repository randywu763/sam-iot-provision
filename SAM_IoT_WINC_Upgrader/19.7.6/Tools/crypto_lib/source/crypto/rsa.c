/**
 * \file        rsa.c
 *
 * \brief   RSA Cryptography
 *
 *    Implementation of the RSA cryptographic operations. Supports the implementation of:
 *    - RSA Encryption Algorithm
 *    - RSA Signature Algorithm (Verification only)
 *    See RFC3447.
 *
 * Copyright (C) 2013-2014 Atmel Corporation. All rights reserved.
 * 
 * \asf_license_start
 *
 *  \page License
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 *  3. The name of Atmel may not be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *  4. This software may only be redistributed and used in connection with an
 *      Atmel microcontroller product.
 *
 *    THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 *    WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 *    EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 *    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *     POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
*/

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


#include "crypto.h"
#include "nmi_m2m.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
GLOBAL VARIABLES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

static const uint8 gau8TEncodingMD5[] = 
{
	0x30, 0x20, 0x30, 0x0C, 0x06, 0x08, 0x2A, 0x86,
	0x48, 0x86, 0xF7, 0x0D, 0x02, 0x05, 0x05, 0x00,
	0x04
};
/*!< Fixed part of the Encoding T for the MD5 hash algorithm.
*/


static const uint8 gau8TEncodingSHA1[] = 
{
	0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2B, 0x0E,
	0x03, 0x02, 0x1A, 0x05, 0x00, 0x04 
};
/*!< Fixed part of the Encoding T for the SHA-1 hash algorithm.
*/

static const uint8 gau8TEncodingSHA224[] = 
{
	0x30, 0x2D, 0x30, 0x0D, 0x06, 0x09, 0x60, 0x86,
	0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x04, 0x05,
	0x00, 0x04
};
/*!< Fixed part of the Encoding T for the SHA-224 hash algorithm.
*/

static const uint8 gau8TEncodingSHA256[] = 
{
	0x30, 0x31, 0x30, 0x0D, 0x06, 0x09, 0x60, 0x86,
	0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05,
	0x00, 0x04
};
/*!< Fixed part of the Encoding T for the SHA-256 hash algorithm.
*/

static const uint8 gau8TEncodingSHA384[] = 
{
	0x30, 0x41, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
	0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02, 0x05,
	0x00, 0x04
};
/*!< Fixed part of the Encoding T for the SHA-384 hash algorithm.
*/

static const uint8 gau8TEncodingSHA512[] = 
{
	0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
	0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03, 0x05,
	0x00, 0x04
};
/*!< Fixed part of the Encoding T for the SHA-512 hash algorithm.
*/



/*********************************************************************
Function
	GetTConst

Description

Return
*********************************************************************/
static void GetTConst(uint16 u16HashSz, uint8 **ppu8T, uint16 *pu16TEncSz)
{
	if(u16HashSz == MD5_DIGEST_SIZE)
	{
		*ppu8T			= (uint8*)gau8TEncodingMD5;
		*pu16TEncSz		= sizeof(gau8TEncodingMD5);
	}
	else if(u16HashSz == SHA1_DIGEST_SIZE)
	{
		*ppu8T			= (uint8*)gau8TEncodingSHA1;
		*pu16TEncSz		= sizeof(gau8TEncodingSHA1);
	}
	else if(u16HashSz == SHA224_DIGEST_SIZE)
	{
		*ppu8T			= (uint8*)gau8TEncodingSHA224;
		*pu16TEncSz		= sizeof(gau8TEncodingSHA224);
	}
	else if(u16HashSz == SHA256_DIGEST_SIZE)
	{
		*ppu8T			= (uint8*)gau8TEncodingSHA256;
		*pu16TEncSz		= sizeof(gau8TEncodingSHA256);
	}
	else if(u16HashSz == SHA384_DIGEST_SIZE)
	{
		*ppu8T			= (uint8*)gau8TEncodingSHA384;
		*pu16TEncSz		= sizeof(gau8TEncodingSHA384);
	}
	else if(u16HashSz == SHA512_DIGEST_SIZE)
	{
		*ppu8T			= (uint8*)gau8TEncodingSHA512;
		*pu16TEncSz		= sizeof(gau8TEncodingSHA512);
	}
	else
	{
		*ppu8T			= NULL;
		*pu16TEncSz		= 0;
	}
	
}
/*********************************************************************
Function
	RSA_Encrypt

Description
	- 	The function performs RSA encryption algorithm using the algorithm 
		RSAES-PKCS1-v1_5 defined in RFC3447.
	-	The CipherText buffer must be allocated by the caller. 

Return
	-	Size of the cipher text for a successful operation.
	-	ZERO for encryption failure.
*********************************************************************/
uint16 RSA_Encrypt
(
tstrRSAPublicKey 	*pstrKey,
uint8				*pu8Plain,
uint16				u16PlainSize,
uint8				*pu8Cipher
)
{
	uint16	u16CipherSize = 0;
	uint32	u32ScratchMemIdx;
	uint16	u16NByteLength, u16PsLength;
	uint8	*pu8EM;

	if((pstrKey != NULL) && (pu8Plain != NULL) && (pu8Cipher != NULL))
	{
		if((pstrKey->pu8N != NULL) && (pstrKey->pu8E != NULL))
		{
			u16NByteLength 		= pstrKey->u16NSize;
			u32ScratchMemIdx	= GET_SCRATCH_MEM_IDX();
			pu8EM = (uint8*)SCRATCH_MEM_ALLOC(u16NByteLength);
			if(pu8EM != NULL)
			{
				/*
				STEP 1
				Check -> mLen <= (k - 11)
				*/
				if(u16PlainSize <= (u16NByteLength - 11))
				{
					/*
					STEP 2
					EME-PKCS1-v1_5 encoding
					*/
					
					/* 2.a Generate PS
					Random sequence of length k - mLen - 3 (non-zero bytes).
					*/
					u16PsLength = u16NByteLength - u16PlainSize - 3;
                    M2M_GET_RAND_BYTES(&pu8EM[2], u16PsLength);

					/* 2.b Produce EM
					EM = 0x00 || 0x02 || PS || 0x00 || M.
					*/
					pu8EM[0] = 0x00;
					pu8EM[1] = 0x02;
					pu8EM[u16PsLength + 2] = 0x00;
					M2M_MEMCPY(&pu8EM[u16PsLength + 3], pu8Plain , u16PlainSize);

					/* 
					STEP 3 RSA Encryption
					c = m ^ e mod n
					*/
					if(BI_MOD_EXP(pu8EM, u16NByteLength, pstrKey->pu8E, pstrKey->u16ESize, 	
						pstrKey->pu8N, pstrKey->u16NSize, pu8Cipher, u16NByteLength) == BI_SUCCESS)
					{
						u16CipherSize = u16NByteLength;
					}
				}
			}
			RESTORE_SCRATCH_MEM_IDX(u32ScratchMemIdx);
		}
	}	
	return u16CipherSize;
}
/*********************************************************************
Function
	RSA_Decrypt

Description
	- 	The function performs RSA decryption algorithm using the algorithm 
		RSAES-PKCS1-v1_5 defined in RFC3447.
	-	The PlianText buffer must be allocated by the caller. 

Return
	-	Size of the cipher text for a successful operation.
	-	ZERO for decryption failure.
*********************************************************************/
uint16 RSA_Decrypt
(
tstrRSAPrivateKey 	*pstrKey,
uint8				*pu8Cipher,
uint16				u16CipherSize, 
uint8				*pu8Plain
)
{
	uint16	u16PlainSize = 0;

	if((pstrKey != NULL) && (pu8Plain != NULL) && (pu8Cipher != NULL))
	{
		if((pstrKey->pu8N != NULL) && (pstrKey->pu8E != NULL))
		{
			if(u16CipherSize == pstrKey->u16NSize)
			{
				uint32	u32ScratchMemIdx;
				uint16	u16NByteLength;
				uint8	*pu8EM;
				
				u16NByteLength 		= pstrKey->u16NSize;
				u32ScratchMemIdx	= GET_SCRATCH_MEM_IDX();
				pu8EM = (uint8*)SCRATCH_MEM_ALLOC(u16NByteLength);
				if(pu8EM != NULL)
				{
					if(BI_MOD_EXP(pu8Cipher, u16CipherSize, pstrKey->pu8E, pstrKey->u16ESize,	
						pstrKey->pu8N, pstrKey->u16NSize, pu8EM, u16NByteLength) == BI_SUCCESS)
					{
						uint32	u32Idx = 0;
						/* 
                        The calculated EM must match the following pattern.
						*======*======*======*======*
							0x00 || 0x02 || PS || 0x00 || M 		
						*======*======*======*======*
						*/
						if((pu8EM[0] == 0x00) && (pu8EM[1] == 0x02))
						{
							for(u32Idx = 2; pu8EM[u32Idx] != 0; u32Idx ++);
							if(u32Idx < u16NByteLength)
							{
								u32Idx ++;
                                u16PlainSize = u16NByteLength - (uint16)u32Idx;
								M2M_MEMCPY(pu8Plain, &pu8EM[u32Idx], u16PlainSize);
							}						
						}
					}
				}
				RESTORE_SCRATCH_MEM_IDX(u32ScratchMemIdx);
			}
		}
	}	
	return u16PlainSize;
}
/*********************************************************************
Function
	RSA_SignVerify

Description
	- 	The function performs RSA encryption algorithm using the algorithm 
		RSAES-PKCS1-v1_5 defined in RFC3447.
	-	The CipherText buffer must be allocated by the caller. 

Return
	-	ZERO for successful operation.
	-	Any integer value otherwise.

Author
	Mohammed Nour

Version
	1.0
	2.0 12 Aug 2014   Add support for MD5 and SHA-2 hash algorithms beside SHA-1

Date
	22 April 2013
*********************************************************************/
sint8 RSA_SignVerify
(
tstrRSAPublicKey	*pstrKey,
uint8				*pu8Signature,
uint16				u16SignLen,
uint8				*pu8Hash,
uint16				u16HashLength
)
{
	sint8		s8Ret = -1;

	if((pstrKey != NULL) && (pu8Signature != NULL))
	{
		if((pstrKey->pu8N != NULL) && (pstrKey->pu8E != NULL))
		{
			uint16	u16NByteLength;
			uint32	u32ScratchMemIdx;
			uint16	u16TLength, u16TEncodingLength;
			uint8	*pu8T;
			uint8	*pu8EM;

			u16NByteLength		= pstrKey->u16NSize;
			u32ScratchMemIdx	= GET_SCRATCH_MEM_IDX();
			pu8EM = (uint8*)SCRATCH_MEM_ALLOC(u16SignLen);
			if(pu8EM != NULL)
			{
				/*
				Check -> signature length == key length
				*/
				if(u16SignLen == u16NByteLength)
				{
					GetTConst(u16HashLength, &pu8T, &u16TEncodingLength);
					if(u16TEncodingLength != 0)
					{
						u16TLength = u16TEncodingLength + 1 + u16HashLength;
					}
					else
					{
						u16TLength = u16HashLength;
					}
					
					/* If emLen < tLen + 11.
					*/
					if(u16NByteLength >= (u16TLength + 11))
					{
						/*
						RSA verification
						*/
						if(BI_MOD_EXP(pu8Signature, u16NByteLength, pstrKey->pu8E, pstrKey->u16ESize,
							pstrKey->pu8N, pstrKey->u16NSize, pu8EM, u16NByteLength) == BI_SUCCESS)
						{
							uint32 	u32PSLength,u32Idx = 0;

							u32PSLength = u16NByteLength - u16TLength - 3;

							/* 
                            The calculated EM must match the following pattern.
							*======*======*======*======*
								0x00 || 0x01 || PS || 0x00 || T 		
							*======*======*======*======*
							Where PS is all 0xFF 
							T is defined based on the hash algorithm.
							*/
							if((pu8EM[0] == 0x00) && (pu8EM[1] == 0x01))
							{
								for(u32Idx = 2; pu8EM[u32Idx] == 0xFF; u32Idx ++);
								if(u32Idx == (u32PSLength + 2))
								{
									if(pu8EM[u32Idx ++] == 0x00)
									{
										if(u16TEncodingLength > 0)
										{
											if(!M2M_MEMCMP(&pu8EM[u32Idx], pu8T, u16TEncodingLength))
											{
												u32Idx += u16TEncodingLength;
												if(pu8EM[u32Idx ++] == u16HashLength)
												{
													s8Ret = M2M_MEMCMP(&pu8EM[u32Idx], pu8Hash, u16HashLength);
												}
											}
										}
										else
										{
											s8Ret = M2M_MEMCMP(&pu8EM[u32Idx], pu8Hash, u16HashLength);	
										}
									}
								}
							}
						}
					}
				}
			}
			RESTORE_SCRATCH_MEM_IDX(u32ScratchMemIdx);
		}
	}
	return s8Ret;
}
/*********************************************************************
Function
	RSA_SignGen

Description
	- 	The function performs RSA encryption algorithm using the algorithm
		RSAES-PKCS1-v1_5 defined in RFC3447.
	-	The CipherText buffer must be allocated by the caller.

Return
	-	ZERO for successful operation.
	-	Any integer value otherwise.

Author
	Mohammed Nour

Version
	1.0
	2.0 12 Aug 2014   Add support for MD5 and SHA-2 hash algorithms beside SHA-1

Date
	22 April 2013
*********************************************************************/
sint8 RSA_SignGen
(
tstrRSAPublicKey	*pstrKey,
uint8				*pu8Signature,
uint8				*pu8Hash,
uint16				u16HashLength
)
{
	sint8		s8Ret = -5;

	if((pstrKey != NULL) && (pu8Signature != NULL) && (pu8Hash != NULL))
	{
		if((pstrKey->pu8N != NULL) && (pstrKey->pu8E != NULL))
		{

			uint16	u16NByteLength;
			uint32	u32ScratchMemIdx;
			uint16	u16TLength, u16TEncodingLength;
			uint8	*pu8T;
			uint8	*pu8EM;
			s8Ret = -4;

			u16NByteLength		= pstrKey->u16NSize;
			u32ScratchMemIdx	= GET_SCRATCH_MEM_IDX();
			pu8EM = (uint8*)SCRATCH_MEM_ALLOC(u16NByteLength);
			if(pu8EM != NULL)
			{
				uint32	u32PSLength,u32Idx = 0;

				GetTConst(u16HashLength, &pu8T, &u16TEncodingLength);
				if(u16TEncodingLength != 0)
				{
					u16TLength = u16TEncodingLength + 1 + u16HashLength;
				}
				else
				{
					u16TLength = u16HashLength;
				}
				
				u32PSLength = u16NByteLength - u16TLength - 3;
				
				/* 
                The calculated EM must match the following pattern.
				*======*======*======*======*
					0x00 || 0x01 || PS || 0x00 || T 		
				*======*======*======*======*
				Where PS is all 0xFF 
				T is defined based on the hash algorithm.
				*/
				pu8EM[u32Idx ++]	= 0;
				pu8EM[u32Idx ++]	= 1;
				M2M_MEMSET(&pu8EM[u32Idx], 0xFF, u32PSLength);
				u32Idx += u32PSLength;
				pu8EM[u32Idx ++] = 0;
				if(u16TEncodingLength > 0)
				{
					M2M_MEMCPY(&pu8EM[u32Idx], pu8T, u16TEncodingLength);
					u32Idx += u16TEncodingLength;
					pu8EM[u32Idx ++] = (uint8)u16HashLength;
				}
				M2M_MEMCPY(&pu8EM[u32Idx], pu8Hash, u16HashLength);
				
				/*
				RSA generation
                Note BI_MOD_EXP returns sint32, not sint8, but only possible return values are:
                    -10 (BI_FAIL)
                    0   (BI_SUCCESS)
                so no info lost by casting to sint8.
				*/
                s8Ret = (sint8)(BI_MOD_EXP(pu8EM, u16NByteLength, pstrKey->pu8E, pstrKey->u16ESize,
                                           pstrKey->pu8N, pstrKey->u16NSize, pu8Signature, u16NByteLength));
			}
			else
			{
				s8Ret = -3;
			}
			RESTORE_SCRATCH_MEM_IDX(u32ScratchMemIdx);
		}
		else
		{
			s8Ret = -4;
		}
	}
	return s8Ret;
}
