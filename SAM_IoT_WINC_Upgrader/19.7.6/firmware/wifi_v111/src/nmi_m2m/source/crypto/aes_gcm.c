/*!
@file 		
	aes_gcm.c

@brief
	AES GCM mode implementation.

@author
	Ahmed Ezzat

@date
	26 July 2015
	
@sa
	NIST Special Publication 800-38D
*/


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "crypto.h"
#include "nmi_m2m.h"

#ifdef __CRYPTO_AES_GCM_SUPPORT__

#define __FAST_GCM__

M2M_SHARED_DATA_SEC
volatile uint32 gau32AesGcmSharedMemBuffer[AES_BLOCK_SIZE/4];

#ifndef __FAST_GCM__
#define GCM_BLK_MUL(CXT ,X, XH)		GcmBlockMul((CXT), (X), (XH))
#else
#define GCM_BLK_MUL(CXT, X, XH)		GcmBlockMulFast((CXT), (X), (XH))
#endif

const uint8	gau8GcmRTable[512] = {
    0x00, 0x00, 0x01, 0xc2, 0x03, 0x84, 0x02, 0x46, 0x07, 0x08, 0x06, 0xca, 0x04, 0x8c, 0x05, 0x4e,
    0x0e, 0x10, 0x0f, 0xd2, 0x0d, 0x94, 0x0c, 0x56, 0x09, 0x18, 0x08, 0xda, 0x0a, 0x9c, 0x0b, 0x5e,
    0x1c, 0x20, 0x1d, 0xe2, 0x1f, 0xa4, 0x1e, 0x66, 0x1b, 0x28, 0x1a, 0xea, 0x18, 0xac, 0x19, 0x6e,
    0x12, 0x30, 0x13, 0xf2, 0x11, 0xb4, 0x10, 0x76, 0x15, 0x38, 0x14, 0xfa, 0x16, 0xbc, 0x17, 0x7e,
    0x38, 0x40, 0x39, 0x82, 0x3b, 0xc4, 0x3a, 0x06, 0x3f, 0x48, 0x3e, 0x8a, 0x3c, 0xcc, 0x3d, 0x0e,
    0x36, 0x50, 0x37, 0x92, 0x35, 0xd4, 0x34, 0x16, 0x31, 0x58, 0x30, 0x9a, 0x32, 0xdc, 0x33, 0x1e,
    0x24, 0x60, 0x25, 0xa2, 0x27, 0xe4, 0x26, 0x26, 0x23, 0x68, 0x22, 0xaa, 0x20, 0xec, 0x21, 0x2e,
    0x2a, 0x70, 0x2b, 0xb2, 0x29, 0xf4, 0x28, 0x36, 0x2d, 0x78, 0x2c, 0xba, 0x2e, 0xfc, 0x2f, 0x3e,
    0x70, 0x80, 0x71, 0x42, 0x73, 0x04, 0x72, 0xc6, 0x77, 0x88, 0x76, 0x4a, 0x74, 0x0c, 0x75, 0xce,
    0x7e, 0x90, 0x7f, 0x52, 0x7d, 0x14, 0x7c, 0xd6, 0x79, 0x98, 0x78, 0x5a, 0x7a, 0x1c, 0x7b, 0xde,
    0x6c, 0xa0, 0x6d, 0x62, 0x6f, 0x24, 0x6e, 0xe6, 0x6b, 0xa8, 0x6a, 0x6a, 0x68, 0x2c, 0x69, 0xee,
    0x62, 0xb0, 0x63, 0x72, 0x61, 0x34, 0x60, 0xf6, 0x65, 0xb8, 0x64, 0x7a, 0x66, 0x3c, 0x67, 0xfe,
    0x48, 0xc0, 0x49, 0x02, 0x4b, 0x44, 0x4a, 0x86, 0x4f, 0xc8, 0x4e, 0x0a, 0x4c, 0x4c, 0x4d, 0x8e,
    0x46, 0xd0, 0x47, 0x12, 0x45, 0x54, 0x44, 0x96, 0x41, 0xd8, 0x40, 0x1a, 0x42, 0x5c, 0x43, 0x9e,
    0x54, 0xe0, 0x55, 0x22, 0x57, 0x64, 0x56, 0xa6, 0x53, 0xe8, 0x52, 0x2a, 0x50, 0x6c, 0x51, 0xae,
    0x5a, 0xf0, 0x5b, 0x32, 0x59, 0x74, 0x58, 0xb6, 0x5d, 0xf8, 0x5c, 0x3a, 0x5e, 0x7c, 0x5f, 0xbe,
    0xe1, 0x00, 0xe0, 0xc2, 0xe2, 0x84, 0xe3, 0x46, 0xe6, 0x08, 0xe7, 0xca, 0xe5, 0x8c, 0xe4, 0x4e,
    0xef, 0x10, 0xee, 0xd2, 0xec, 0x94, 0xed, 0x56, 0xe8, 0x18, 0xe9, 0xda, 0xeb, 0x9c, 0xea, 0x5e,
    0xfd, 0x20, 0xfc, 0xe2, 0xfe, 0xa4, 0xff, 0x66, 0xfa, 0x28, 0xfb, 0xea, 0xf9, 0xac, 0xf8, 0x6e,
    0xf3, 0x30, 0xf2, 0xf2, 0xf0, 0xb4, 0xf1, 0x76, 0xf4, 0x38, 0xf5, 0xfa, 0xf7, 0xbc, 0xf6, 0x7e,
    0xd9, 0x40, 0xd8, 0x82, 0xda, 0xc4, 0xdb, 0x06, 0xde, 0x48, 0xdf, 0x8a, 0xdd, 0xcc, 0xdc, 0x0e,
    0xd7, 0x50, 0xd6, 0x92, 0xd4, 0xd4, 0xd5, 0x16, 0xd0, 0x58, 0xd1, 0x9a, 0xd3, 0xdc, 0xd2, 0x1e,
    0xc5, 0x60, 0xc4, 0xa2, 0xc6, 0xe4, 0xc7, 0x26, 0xc2, 0x68, 0xc3, 0xaa, 0xc1, 0xec, 0xc0, 0x2e,
    0xcb, 0x70, 0xca, 0xb2, 0xc8, 0xf4, 0xc9, 0x36, 0xcc, 0x78, 0xcd, 0xba, 0xcf, 0xfc, 0xce, 0x3e,
    0x91, 0x80, 0x90, 0x42, 0x92, 0x04, 0x93, 0xc6, 0x96, 0x88, 0x97, 0x4a, 0x95, 0x0c, 0x94, 0xce,
    0x9f, 0x90, 0x9e, 0x52, 0x9c, 0x14, 0x9d, 0xd6, 0x98, 0x98, 0x99, 0x5a, 0x9b, 0x1c, 0x9a, 0xde,
    0x8d, 0xa0, 0x8c, 0x62, 0x8e, 0x24, 0x8f, 0xe6, 0x8a, 0xa8, 0x8b, 0x6a, 0x89, 0x2c, 0x88, 0xee,
    0x83, 0xb0, 0x82, 0x72, 0x80, 0x34, 0x81, 0xf6, 0x84, 0xb8, 0x85, 0x7a, 0x87, 0x3c, 0x86, 0xfe,
    0xa9, 0xc0, 0xa8, 0x02, 0xaa, 0x44, 0xab, 0x86, 0xae, 0xc8, 0xaf, 0x0a, 0xad, 0x4c, 0xac, 0x8e,
    0xa7, 0xd0, 0xa6, 0x12, 0xa4, 0x54, 0xa5, 0x96, 0xa0, 0xd8, 0xa1, 0x1a, 0xa3, 0x5c, 0xa2, 0x9e,
    0xb5, 0xe0, 0xb4, 0x22, 0xb6, 0x64, 0xb7, 0xa6, 0xb2, 0xe8, 0xb3, 0x2a, 0xb1, 0x6c, 0xb0, 0xae,
    0xbb, 0xf0, 0xba, 0x32, 0xb8, 0x74, 0xb9, 0xb6, 0xbc, 0xf8, 0xbd, 0x3a, 0xbf, 0x7c, 0xbe, 0xbe 
};

aes_context_t	*gpstrAesCxt;

/*********************************************************************
Function
	BlockShiftRight

Description
	Perorms a shift right operation on an AES Block.
	
Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	26 July 2015
*********************************************************************/
static void BlockShiftRight(uint8 *X)
{
	X[15] = (X[15] >> 1) | (X[14] << 7);	
	X[14] = (X[14] >> 1) | (X[13] << 7);	
	X[13] = (X[13] >> 1) | (X[12] << 7);	
	X[12] = (X[12] >> 1) | (X[11] << 7);	
	X[11] = (X[11] >> 1) | (X[10] << 7);	
	X[10] = (X[10] >> 1) | (X[9] << 7);	
	X[9] = (X[9] >> 1) | (X[8] << 7);	
	X[8] = (X[8] >> 1) | (X[7] << 7);	
	X[7] = (X[7] >> 1) | (X[6] << 7);	
	X[6] = (X[6] >> 1) | (X[5] << 7);	
	X[5] = (X[5] >> 1) | (X[4] << 7);	
	X[4] = (X[4] >> 1) | (X[3] << 7);	
	X[3] = (X[3] >> 1) | (X[2] << 7);	
	X[2] = (X[2] >> 1) | (X[1] << 7);	
	X[1] = (X[1] >> 1) | (X[0] << 7);	
	X[0] >>= 1;
}
/*********************************************************************
Function
	BlockXOR

Description
	Compute the XOR of two AES blocks Z = X XOR Y.
	
Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	26 July 2015
*********************************************************************/
static void BlockXOR(uint8 *X, uint8 *Y, uint8 *Z)
{
	uint8	u8Idx;
	for(u8Idx = 0; u8Idx < 16; u8Idx ++)
	{
		Z[u8Idx] = X[u8Idx] ^ Y[u8Idx];
	}
}
/*********************************************************************
Function
	GcmBlockMul

Description
	Compute the Galois Multiplication of of two AES blocks XY = X mul Y.
	
Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	26 July 2015
*********************************************************************/
static void GcmBlockMul(tstrAesGCMContext *pstrAesGCMCxt, uint8 *pu8X, uint8 *pu8XH)
{
	uint8	u8ByteIdx, u8BitIdx;
	uint8	V[AES_BLOCK_SIZE], Z[AES_BLOCK_SIZE];
	uint8	u8CurrByte;
	uint8	u8VBit;

	M2M_MEMSET(Z, 0, 16);
	M2M_MEMCPY(V, pu8X, 16);

	for(u8ByteIdx = 0; u8ByteIdx < AES_BLOCK_SIZE; u8ByteIdx ++)
	{
		u8CurrByte = pstrAesGCMCxt->H[u8ByteIdx];
		for(u8BitIdx = 0; u8BitIdx < 8; u8BitIdx ++)
		{
			if(u8CurrByte & BIT7)
			{
				BlockXOR(Z, V, Z);
			}
			u8VBit = V[15] & BIT0;
			BlockShiftRight(V);
			if(u8VBit)
			{
				V[0] = V[0] ^ 0xE1;
			}
			u8CurrByte <<= 1;
		}
	}
	M2M_MEMCPY(pu8XH, Z, AES_BLOCK_SIZE);
}
/*********************************************************************
Function
	Gen4bitLookupTable

Description
	Compute H lookup table for 4-bits multiplication.
	
Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	25 Oct 2015
*********************************************************************/
static void Gen4bitLookupTable(tstrAesGCMContext *pstrAesGCMCxt)
{
    uint32	u32Idx;
    uint8	block[16] = {0};
	
	M2M_MEMSET(pstrAesGCMCxt->au8TL, 0, 16 * 16);
    M2M_MEMSET(pstrAesGCMCxt->au8TH, 0, 16 * 16);
    
    for(u32Idx = 1; u32Idx < 16; u32Idx ++)
    {
        block[0] = u32Idx;
        GcmBlockMul(pstrAesGCMCxt, block, pstrAesGCMCxt->au8TL[u32Idx]);
        block[0] = u32Idx << 4;
        GcmBlockMul(pstrAesGCMCxt, block, pstrAesGCMCxt->au8TH[u32Idx]);
    }
}
/*********************************************************************
Function
	GcmBlockMulFast

Description
	Compute the Galois Multiplication of of two AES blocks XY = X mul Y using
	pre-computed lookup tables for optimizing computation time.
	
Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	25 Oct 2015
*********************************************************************/
static void GcmBlockMulFast(tstrAesGCMContext *pstrAesGCMCxt, uint8 *pu8X, uint8 *pu8XH)
{
    uint8	u8Idx;
    uint8	Z[32] = {0};
    uint16  u16R;

    BLOCK_XOR(pstrAesGCMCxt->au8TL[((pu8X[0]) & 0x0F)], pstrAesGCMCxt->au8TH[((pu8X[0]) >> 4)], Z);

    for(u8Idx = 1; u8Idx < 16; u8Idx ++)
    {
        BLOCK_XOR(&Z[u8Idx], pstrAesGCMCxt->au8TL[((pu8X[u8Idx]) & 0x0F)], &Z[u8Idx]);
        BLOCK_XOR(&Z[u8Idx], pstrAesGCMCxt->au8TH[((pu8X[u8Idx]) >> 4)], &Z[u8Idx]);
    }
    for(u8Idx = 31; u8Idx >= 16; u8Idx --)
    {
        u16R = 2 * Z[u8Idx];
        Z[u8Idx - 16] ^= gau8GcmRTable[u16R];
        Z[u8Idx - 15] ^= gau8GcmRTable[u16R + 1];
    }
    M2M_MEMCPY(pu8XH, Z, 16);
}
/*********************************************************************
Function
	GcmIncr

Description
	Increment an AES block.
	
Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	26 July 2015
*********************************************************************/
static void GcmIncr(uint8 *pu8CTR)
{
	uint32 u32Tmp = GETU32(pu8CTR, 12);
	u32Tmp ++;
	PUTU32(u32Tmp, pu8CTR, 12);
}
/*********************************************************************
Function
	GcmGHASH

Description

	
Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	26 July 2015
*********************************************************************/
static void GcmGHASH(tstrAesGCMContext *pstrAesGCMCxt, uint8 *X, uint8 *Y, uint16 u16XLen)
{
	uint16	u16Idx;
	uint16	u16Blocks;
	uint8	u8RemainBytes;

	u16Blocks = u16XLen / AES_BLOCK_SIZE;
	u8RemainBytes = (uint8)(u16XLen % AES_BLOCK_SIZE);

	for(u16Idx = 0; u16Idx < (u16Blocks * AES_BLOCK_SIZE); u16Idx += AES_BLOCK_SIZE)
	{
		BlockXOR(&X[u16Idx], Y, Y);
		GCM_BLK_MUL(pstrAesGCMCxt, Y, Y);
	}

	if(u8RemainBytes)
	{
		uint8	au8LastBlock[AES_BLOCK_SIZE] = {0};
		M2M_MEMCPY(au8LastBlock, &X[u16XLen - u8RemainBytes], u8RemainBytes);
		BlockXOR(au8LastBlock, Y, Y);
		GCM_BLK_MUL(pstrAesGCMCxt, Y, Y);
	}
}
/*********************************************************************
Function
	GcmGCTR

Description

	
Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	26 July 2015
*********************************************************************/
static void GcmGCTR(tstrAesGCMContext *pstrAesGCMCxt, uint8 *J0, uint8 *X, uint16 u16XLen)
{
	if(pstrAesGCMCxt != NULL)
	{
		static uint8	CB[AES_BLOCK_SIZE];

		if(J0 != NULL)
		{
			M2M_MEMCPY(CB, J0, AES_BLOCK_SIZE);
			GcmIncr(CB);
		}

		if((X != NULL) && (u16XLen != 0))
		{
			uint8	encCB[AES_BLOCK_SIZE];
			uint16	u16Blocks;
			uint8	u8RemainBytes;

			u16Blocks = u16XLen / AES_BLOCK_SIZE;
			u8RemainBytes = (uint8)(u16XLen % AES_BLOCK_SIZE);

#ifdef __AES_HW_ENGINE__
			{
				uint32 u32Tmp = GETU32(CB, 12);
				AES128_CTR_Enc_HW(CB, X, (u16Blocks * AES_BLOCK_SIZE), pstrAesGCMCxt->au8Key);
				u32Tmp += u16Blocks;
				PUTU32(u32Tmp, CB, 12);
			}
#else
			{
				uint16	u16Idx;
				for(u16Idx = 0; u16Idx < (u16Blocks * AES_BLOCK_SIZE); u16Idx += AES_BLOCK_SIZE)
				{
					AES_ENCRYPT(gpstrAesCxt, CB, encCB);
					BlockXOR(&X[u16Idx], encCB, &X[u16Idx]);
					GcmIncr(CB);
				}
			}
#endif			
			if(u8RemainBytes)
			{
				uint8			au8LastBlock[AES_BLOCK_SIZE] = {0};
				
				M2M_MEMCPY(au8LastBlock, &X[u16XLen - u8RemainBytes], u8RemainBytes);
				AES_ENCRYPT(gpstrAesCxt, CB, encCB);
				BlockXOR(au8LastBlock, encCB, au8LastBlock);
				M2M_MEMCPY(&X[u16XLen - u8RemainBytes], au8LastBlock, u8RemainBytes);
			}
		}
	}
}
/*********************************************************************
Function
	GcmDeriveCTR0

Description
	Derive the first CTR block from the IV.
	
Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	26 July 2015
*********************************************************************/
static void GcmDeriveCTR0(tstrAesGCMContext *pstrAesGCMCxt, uint8 *pu8IV, uint8 u8IVLen, uint8 *pu8CTR0)
{
	/* CTR0 is caculated as follows:
		- IV || 0x00000001					if (IV_LEN == 96 bits).
		- GHASH(H, IV||0 ^ 64||[len(IV)]64)	Otherwise.
	*/
	M2M_MEMSET(pu8CTR0, 0, AES_BLOCK_SIZE);

	if(u8IVLen == 12)
	{
		M2M_MEMCPY(pu8CTR0, pu8IV, u8IVLen);
		pu8CTR0[AES_BLOCK_SIZE - 1] = 1;
	}
	else
	{
		uint8	au8LenBlock[AES_BLOCK_SIZE] = {0};

		PUTU32((uint32)(u8IVLen * 8), au8LenBlock, 12);
		GcmGHASH(pstrAesGCMCxt, pu8IV, pu8CTR0, u8IVLen);
		GcmGHASH(pstrAesGCMCxt, au8LenBlock, pu8CTR0, AES_BLOCK_SIZE);
	}
}
/*********************************************************************
Function
	AES_GCMContextInit

Description


Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	2 August 2015
*********************************************************************/
void AES_GCMContextInit
(
tstrAesGCMContext	*pstrAesGCMCxt,
uint8				*pu8Key,
uint8				u8KeySize
)
{
	if((pstrAesGCMCxt != NULL) && (pu8Key != NULL))
	{
		uint8			au8ZeroBlock[AES_BLOCK_SIZE] = {0};
		aes_context_t	strAesCxt;

		gpstrAesCxt	= &strAesCxt;
		
		M2M_MEMCPY(pstrAesGCMCxt->au8Key, pu8Key, u8KeySize);
		pstrAesGCMCxt->u8KeySize	= u8KeySize;
		AES_SETUP(&strAesCxt, u8KeySize, pu8Key);
		AES_ENCRYPT(&strAesCxt, au8ZeroBlock, pstrAesGCMCxt->H);
		Gen4bitLookupTable(pstrAesGCMCxt);
	}
}
/*********************************************************************
Function
	AES_GCMEncrypt

Description
	- Encrypt an input data buffer using the AES GCM mode. 
	- Output both the encrypted data (encryption in place) and the authentication TAG.

Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	26 July 2015
*********************************************************************/
sint8 AES_GCMEncrypt
(
tstrAesGCMContext	*pstrAesGCMCxt,
uint8 				*pu8IV,
uint8 				u8IVLen,
uint8 				*pu8AAD,
uint8 				u8AADLen,
uint8 				*pu8Data,
uint16 				u16DataLen,
uint8 				*pu8T
)
{
	uint8			CTR0[AES_BLOCK_SIZE]			= {0};
	uint8			S[AES_BLOCK_SIZE] 				= {0};
	uint8			au8TmpBlock[AES_BLOCK_SIZE]		= {0};
	aes_context_t	strAesCxt;
	
	AES_SETUP(&strAesCxt, pstrAesGCMCxt->u8KeySize, pstrAesGCMCxt->au8Key);
	gpstrAesCxt	= &strAesCxt;
	
	/* 2. Compute CTR0.
	*/
	GcmDeriveCTR0(pstrAesGCMCxt, pu8IV, u8IVLen, CTR0);

	/* 3. Compute the Ciphertext C.
	*/
	GcmGCTR(pstrAesGCMCxt, CTR0, pu8Data, u16DataLen);

	/* 4. Format the length block such with the form. 
		[len(A)]64 || [len(C)]64
	*/	
	PUTU32((uint32)(u8AADLen * 8), au8TmpBlock, 4);
	PUTU32((uint32)(u16DataLen * 8), au8TmpBlock, 12);

	/* 5. Compute the block S from:
		S = GHASH(H, A || C || [len(A)]64 || [len(C)]64)
	*/
	GcmGHASH(pstrAesGCMCxt, pu8AAD, S, u8AADLen);
	GcmGHASH(pstrAesGCMCxt, pu8Data, S, u16DataLen);
	GcmGHASH(pstrAesGCMCxt, au8TmpBlock, S, AES_BLOCK_SIZE);

	/* 6. Compute the authentication tag T
		T = E(K, CTR0) XOR S. 
	*/
	AES_ENCRYPT(&strAesCxt, CTR0, CTR0);
	BlockXOR(S, CTR0, pu8T);
	return 0;
}
/*********************************************************************
Function
	AES_GCMDecrypt

Description
	- Decrypt an input data buffer using the AES GCM mode. 
	- Output both the decrypted data (decryption in place) and the authentication TAG.

Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	26 July 2015
*********************************************************************/
sint8 AES_GCMDecrypt
(
tstrAesGCMContext	*pstrAesGCMCxt,
uint8				*pu8IV,
uint8				u8IVLen,
uint8				*pu8AAD,
uint8				u8AADLen,
uint8				*pu8Data,
uint16				u16DataSize,
uint8				*pu8T
)
{
	sint8	s8Ret = -1;

	if((pu8IV != NULL) && (pu8Data != NULL) && (pstrAesGCMCxt != NULL) && (pu8T != NULL))
	{
		uint8			*pu8TmpBlock;
		uint8			CTR0[AES_BLOCK_SIZE]	= {0};
		uint8			S[AES_BLOCK_SIZE]		= {0};
		aes_context_t	strAesCxt;
		
		AES_SETUP(&strAesCxt, pstrAesGCMCxt->u8KeySize, pstrAesGCMCxt->au8Key);
		gpstrAesCxt	= &strAesCxt;
		
		pu8TmpBlock = (uint8*)&gau32AesGcmSharedMemBuffer[0]; //M2M_MALLOC_S(AES_BLOCK_SIZE);
		if(pu8TmpBlock != NULL)
		{
			/* 2. Compute CTR0.
			*/
			GcmDeriveCTR0(pstrAesGCMCxt, pu8IV, u8IVLen, CTR0);
			
			/* 4. Compute the block S from:
				S = GHASH(H, A || C || [len(A)]64 || [len(C)]64)
			*/
			GcmGHASH(pstrAesGCMCxt, pu8AAD, S, u8AADLen);
			
			/* If BIT15(u16DataSize) is set, then the Encrypted Message is given as a list of buffers. The parameter
			pu8Data should be casted to tstrBuffer*. The number of buffers is given in the LSB(u16DataSize).
			*/
			if(u16DataSize & BIT15)
			{
				/* Multi-Buffer message.
				*/
				tstrBuffer	*pstrAESBuff	= (tstrBuffer*)pu8Data;
				uint8		u8nBuffers		= (uint8)u16DataSize;
				uint8		u8BuffIdx;
				uint8		*pu8CurrentBuffer;
				uint16		u16CurrBuffSize;
				uint16		u16Remain	= 0;
				uint16		u16Offset	= 0;
				uint16		u16Blocks;
								
				GcmGCTR(pstrAesGCMCxt, CTR0, NULL, 0);
				u16DataSize = 0;
				for(u8BuffIdx = 0; u8BuffIdx < u8nBuffers; u8BuffIdx ++)
				{
					if(pstrAESBuff[u8BuffIdx].pu8Data != NULL)
					{
						u16DataSize += pstrAESBuff[u8BuffIdx].u16BufferSize;
		
						pu8CurrentBuffer	= pstrAESBuff[u8BuffIdx].pu8Data + u16Offset;
						u16CurrBuffSize		= pstrAESBuff[u8BuffIdx].u16BufferSize - u16Offset;
						u16Blocks			= u16CurrBuffSize / AES_BLOCK_SIZE;
						u16Remain			= u16CurrBuffSize % AES_BLOCK_SIZE;
						u16Offset			= 0;

						if(u16Blocks != 0)
						{
							GcmGHASH(pstrAesGCMCxt, pu8CurrentBuffer, S, (u16Blocks * AES_BLOCK_SIZE));
							GcmGCTR(pstrAesGCMCxt, NULL, pu8CurrentBuffer, (u16Blocks * AES_BLOCK_SIZE));
							pu8CurrentBuffer += (u16Blocks * AES_BLOCK_SIZE);
						}

						/* Concatenate the residual bytes with the first bytes of the next buffer to
						get one complete AES_BLOCK.
						*/
						if(u16Remain != 0)
						{
							if(u8BuffIdx < (u8nBuffers - 1))
							{
								uint8	*pu8NextBuffer = pstrAESBuff[u8BuffIdx + 1].pu8Data;
								
								M2M_MEMCPY(pu8TmpBlock, pu8CurrentBuffer, u16Remain);
								M2M_MEMCPY(&pu8TmpBlock[u16Remain], pu8NextBuffer, (AES_BLOCK_SIZE - u16Remain));

								GcmGHASH(pstrAesGCMCxt, pu8TmpBlock, S, AES_BLOCK_SIZE);
								GcmGCTR(pstrAesGCMCxt, NULL, pu8TmpBlock, AES_BLOCK_SIZE);

								M2M_MEMCPY(pu8CurrentBuffer, pu8TmpBlock, u16Remain);
								M2M_MEMCPY(pu8NextBuffer, &pu8TmpBlock[u16Remain], (AES_BLOCK_SIZE - u16Remain));

								u16Offset = (AES_BLOCK_SIZE - u16Remain);
							}
							else
							{
								GcmGHASH(pstrAesGCMCxt, pu8CurrentBuffer, S, u16Remain);
								GcmGCTR(pstrAesGCMCxt, NULL, pu8CurrentBuffer, u16Remain);
							}
						}
					}
				}
			}
			else
			{
				GcmGHASH(pstrAesGCMCxt, pu8Data, S, u16DataSize);
				GcmGCTR(pstrAesGCMCxt, CTR0, pu8Data, u16DataSize);				
			}
			
			M2M_MEMSET(pu8TmpBlock, 0, 4);
			PUTU32((uint32)(u8AADLen * 8), pu8TmpBlock, 4);
			M2M_MEMSET(&pu8TmpBlock[8], 0, 4);
			PUTU32((uint32)(u16DataSize * 8), pu8TmpBlock, 12);
			GcmGHASH(pstrAesGCMCxt, pu8TmpBlock, S, AES_BLOCK_SIZE);

			/* 6. Compute the authentication tag T
				T = E(K, CTR0) XOR S. 
			*/
			AES_ENCRYPT(&strAesCxt, CTR0, CTR0);
			BlockXOR(S, CTR0, pu8TmpBlock);
			s8Ret = M2M_MEMCMP(pu8TmpBlock, pu8T, AES_BLOCK_SIZE);

			//M2M_FREE(pu8TmpBlock);
		}
	}
	return s8Ret;
}
#endif /* __CRYPTO_AES_GCM_SUPPORT__ */
