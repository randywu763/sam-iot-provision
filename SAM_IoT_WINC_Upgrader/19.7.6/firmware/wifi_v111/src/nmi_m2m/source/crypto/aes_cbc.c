/*!
@file       		
	aes_cbc.c

@brief
	AES encryption algorithm CBC mode implementation.

@author   		
	Ahmed Ezzat

@date      		
	21 OCT 2012
@sa			
*/

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "crypto.h"
#include "nmi_m2m.h"

#define AES_API	M2M_SHARED_SEC_API

M2M_SHARED_DATA_SEC
volatile uint32 gau32AesCbcSharedMemBuffer[AES_BLOCK_SIZE/4];

/*********************************************************************
Function
	AES_CbcContextInit

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
void AES_CbcContextInit
(
tstrAesCbcContext	*pstrAesCbcCxt,
uint8				*pu8Key,
uint8				u8KeySize
)
{
	if((pstrAesCbcCxt != NULL) && (pu8Key != NULL))
	{
		/* Initialize the AES context.
		*/
		M2M_MEMCPY(pstrAesCbcCxt->au8Key, pu8Key, u8KeySize);
		pstrAesCbcCxt->u8KeySize	= u8KeySize;
		pstrAesCbcCxt->bIsHwAes		= AES_CBC_HW(u8KeySize);
	}
}
#ifdef __AES_HW_ENGINE__
/*********************************************************************
Function
	AES_CbcEnc

Description

Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
*********************************************************************/
void AES_CbcEncHW
(
tstrAesCbcContext 	*pstrAesCbcCxt, 
uint8 				*pu8IV,
uint8 				*pu8Data, 
uint16 				u16DataSize
)
{
	if((pu8IV != NULL) && (pu8Data != NULL) && (pstrAesCbcCxt != NULL))
	{
		AES128_CBC_Enc_HW(pu8IV, pu8Data, u16DataSize, pstrAesCbcCxt->au8Key);
	}
}
/*********************************************************************
Function
	AES_CbcEncrypt

Description
	Encrypt an input data buffer using the AES in CBC mode. 
	Encryption in place is performed.

Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	2 August 2015
*********************************************************************/
void AES_CbcDecHW
(
tstrAesCbcContext	*pstrAesCbcCxt,
uint8 				*pu8IV,
uint8 				*pu8Data,
uint16				u16DataSize
)
{
	if((pu8IV != NULL) && (pu8Data != NULL) && (pstrAesCbcCxt != NULL))
	{
		uint32 			u32Idx,u32Count;
		uint8			au8NextIV[AES_BLOCK_SIZE];
		uint8			au8IV[AES_BLOCK_SIZE];
		uint16			u16Blocks;

		M2M_MEMCPY(au8IV, pu8IV, AES_BLOCK_SIZE);

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
			
			for(u8BuffIdx = 0; u8BuffIdx < u8nBuffers; u8BuffIdx ++)
			{
				if(pstrAESBuff[u8BuffIdx].pu8Data != NULL)
				{
					pu8CurrentBuffer	= pstrAESBuff[u8BuffIdx].pu8Data + u16Offset;
					u16CurrBuffSize		= pstrAESBuff[u8BuffIdx].u16BufferSize - u16Offset;
					u16Blocks			= u16CurrBuffSize / AES_BLOCK_SIZE;
					u16Remain			= u16CurrBuffSize % AES_BLOCK_SIZE;
					u16Offset			= 0;
					
					if(u16Blocks > 0)
					{
						M2M_MEMCPY(au8NextIV, &pu8CurrentBuffer[((u16Blocks - 1) * AES_BLOCK_SIZE)], AES_BLOCK_SIZE);
						AES128_CBC_Dec_HW(au8IV, pu8CurrentBuffer, (u16Blocks * AES_BLOCK_SIZE), pstrAesCbcCxt->au8Key);
						M2M_MEMCPY(au8IV, au8NextIV, AES_BLOCK_SIZE);
					}
					u32Idx = (u16Blocks * AES_BLOCK_SIZE);

					/* Concatenate the residual bytes with the first bytes of the next buffer to
					get one complete AES_BLOCK.
					*/
					if((u16Remain != 0) && (u8BuffIdx < (u8nBuffers - 1)))
					{
						uint8	*pu8NextBuffer = pstrAESBuff[u8BuffIdx + 1].pu8Data;
						static uint8	*pu8Tmp = NULL;
						uint8		au8TmpBlock[AES_BLOCK_SIZE];
						
						M2M_MEMCPY(au8TmpBlock, &pu8CurrentBuffer[u32Idx], u16Remain);
						M2M_MEMCPY(&au8TmpBlock[u16Remain], pu8NextBuffer, (AES_BLOCK_SIZE - u16Remain));

						/* Decryption.
						*/
						M2M_MEMCPY(au8NextIV, au8TmpBlock, AES_BLOCK_SIZE);

						pu8Tmp = (uint8*)&gau32AesCbcSharedMemBuffer[0]; //M2M_MALLOC_S(AES_BLOCK_SIZE);
						if(pu8Tmp != NULL)
						{
							M2M_MEMCPY(pu8Tmp, au8TmpBlock, AES_BLOCK_SIZE);
							AES128_CBC_Dec_HW(au8IV, pu8Tmp, AES_BLOCK_SIZE, pstrAesCbcCxt->au8Key);						
							for(u32Count = 0; u32Count < u16Remain; u32Count ++)
							{
								pu8CurrentBuffer[u32Count + u32Idx] = pu8Tmp[u32Count];
							}
							for(u32Count = u16Remain; u32Count < AES_BLOCK_SIZE; u32Count ++)
							{
								pu8NextBuffer[u32Count - u16Remain] = pu8Tmp[u32Count];
							}
							//M2M_FREE(pu8Tmp);
						}
						M2M_MEMCPY(au8IV, au8NextIV, AES_BLOCK_SIZE);
						u16Offset		= (AES_BLOCK_SIZE - u16Remain);
					}
				}
			}
		}
		else
		{
			AES128_CBC_Dec_HW(pu8IV, pu8Data, u16DataSize, pstrAesCbcCxt->au8Key);
		}
	}
}
#endif /* __AES_HW_ENGINE__ */
/*********************************************************************
Function
	AES_CbcEnc

Description

Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
*********************************************************************/
void AES_CbcEncSW
(
tstrAesCbcContext 	*pstrAesCbcCxt, 
uint8 				*pu8IV,
uint8 				*pu8Data, 
uint16 				u16DataSize
)
{
	if((pu8IV != NULL) && (pu8Data != NULL) && (pstrAesCbcCxt != NULL))
	{
		uint32			u32Idx,u32Count;
		uint8			au8Block[AES_BLOCK_SIZE];
		uint16			u16Blocks = u16DataSize/AES_BLOCK_SIZE;
		aes_context_t	strAesCxt;
		
		AES_SETUP(&strAesCxt, pstrAesCbcCxt->u8KeySize, pstrAesCbcCxt->au8Key);

		/* Set the first block to the IV.
		*/
		M2M_MEMCPY(au8Block, pu8IV, AES_BLOCK_SIZE);
		for(u32Idx = 0; u32Idx < (u16Blocks * AES_BLOCK_SIZE); u32Idx += AES_BLOCK_SIZE)
		{
			/* XOR */
			for(u32Count = 0; u32Count < AES_BLOCK_SIZE; u32Count ++)
			{
				au8Block[u32Count] = au8Block[u32Count] XOR pu8Data[u32Count + u32Idx];
			}
		
			/* Encrypt */
			AES_ENCRYPT(&strAesCxt, au8Block, &pu8Data[u32Idx]);
			M2M_MEMCPY(au8Block,&pu8Data[u32Idx], AES_BLOCK_SIZE);
		}
	}
}
/*********************************************************************
Function
	AES_CbcEncrypt

Description
	Encrypt an input data buffer using the AES in CBC mode. 
	Encryption in place is performed.

Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
	2 August 2015
*********************************************************************/
AES_API void AES_CbcDecSW
(
tstrAesCbcContext	*pstrAesCbcCxt,
uint8 				*pu8IV,
uint8 				*pu8Data,
uint16				u16DataSize
)
{
	if((pu8IV != NULL) && (pu8Data != NULL) && (pstrAesCbcCxt != NULL))
	{
		uint32 			u32Idx,u32Count;
		uint8			au8NextIV[AES_BLOCK_SIZE];
		uint8			au8IV[AES_BLOCK_SIZE];
		uint16			u16Blocks;
		aes_context_t	strAesCxt;
		
		AES_SETUP(&strAesCxt, pstrAesCbcCxt->u8KeySize, pstrAesCbcCxt->au8Key);

		M2M_MEMCPY(au8IV, pu8IV, AES_BLOCK_SIZE);

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
			uint8		au8TmpBlock[AES_BLOCK_SIZE];
			
			for(u8BuffIdx = 0; u8BuffIdx < u8nBuffers; u8BuffIdx ++)
			{
				if(pstrAESBuff[u8BuffIdx].pu8Data != NULL)
				{
					pu8CurrentBuffer	= pstrAESBuff[u8BuffIdx].pu8Data + u16Offset;
					u16CurrBuffSize		= pstrAESBuff[u8BuffIdx].u16BufferSize - u16Offset;
					u16Blocks			= u16CurrBuffSize / AES_BLOCK_SIZE;
					u16Remain			= u16CurrBuffSize % AES_BLOCK_SIZE;
					u16Offset			= 0;
					
					/* Handle the complete AES blocks in the current buffer.
					*/
					for(u32Idx = 0; u32Idx < (u16Blocks * AES_BLOCK_SIZE); u32Idx += AES_BLOCK_SIZE)
					{
						M2M_MEMCPY(au8NextIV, &pu8CurrentBuffer[u32Idx], AES_BLOCK_SIZE);
						AES_DECRYPT(&strAesCxt, &pu8CurrentBuffer[u32Idx], &pu8CurrentBuffer[u32Idx]);
						
						/* XOR */
						for(u32Count = 0; u32Count < AES_BLOCK_SIZE; u32Count ++)
						{
							pu8CurrentBuffer[u32Count + u32Idx] = pu8CurrentBuffer[u32Count + u32Idx] XOR au8IV[u32Count];
						}
						
						M2M_MEMCPY(au8IV, au8NextIV, AES_BLOCK_SIZE);
					}

					/* Concatenate the residual bytes with the first bytes of the next buffer to
					get one complete AES_BLOCK.
					*/
					if((u16Remain != 0) && (u8BuffIdx < (u8nBuffers - 1)))
					{
						uint8	*pu8NextBuffer = pstrAESBuff[u8BuffIdx + 1].pu8Data;
						
						M2M_MEMCPY(au8TmpBlock, &pu8CurrentBuffer[u32Idx], u16Remain);
						M2M_MEMCPY(&au8TmpBlock[u16Remain], pu8NextBuffer, (AES_BLOCK_SIZE - u16Remain));

						/* Decryption.
						*/
						M2M_MEMCPY(au8NextIV, au8TmpBlock, AES_BLOCK_SIZE);

						AES_DECRYPT(&strAesCxt, au8TmpBlock, au8TmpBlock);
					
						for(u32Count = 0; u32Count < u16Remain; u32Count ++)
						{
							pu8CurrentBuffer[u32Count + u32Idx] = au8TmpBlock[u32Count] XOR au8IV[u32Count];
						}
						for(u32Count = u16Remain; u32Count < AES_BLOCK_SIZE; u32Count ++)
						{
							pu8NextBuffer[u32Count - u16Remain] = au8TmpBlock[u32Count] XOR au8IV[u32Count];
						}
						M2M_MEMCPY(au8IV, au8NextIV, AES_BLOCK_SIZE);
						u16Offset		= (AES_BLOCK_SIZE - u16Remain);
					}
				}
			}
		}
		else
		{
			u16Blocks = u16DataSize/AES_BLOCK_SIZE;
			M2M_MEMCPY(au8IV, pu8IV, AES_BLOCK_SIZE);
			for(u32Idx = 0; u32Idx < (u16Blocks * AES_BLOCK_SIZE);u32Idx += AES_BLOCK_SIZE)
			{
				M2M_MEMCPY(au8NextIV,&pu8Data[u32Idx],AES_BLOCK_SIZE);
				AES_DECRYPT(&strAesCxt, &pu8Data[u32Idx], &pu8Data[u32Idx]);

				/* XOR */
				for(u32Count = 0; u32Count < AES_BLOCK_SIZE; u32Count ++)
				{
					pu8Data[u32Count + u32Idx] = pu8Data[u32Count + u32Idx] XOR au8IV[u32Count];
				}
				M2M_MEMCPY(au8IV,au8NextIV,AES_BLOCK_SIZE);
			}
		}
	}
}
/*********************************************************************
Function
	AES_CbcDec

Description
	Perform AES decryption in CBC mode. The decryption is performed in place.

Return

Author
	Ahmed Ezzat

Version
	1.0

Date
	11 Nov 2012
*********************************************************************/
void AES_CbcEncrypt
(
tstrAesCbcContext	*pstrAesCbcCxt,
uint8 				*pu8IV,
uint8 				*pu8Data,
uint16				u16DataSize
)
{
	if(pstrAesCbcCxt->bIsHwAes)
	{
		AES_CBC_ENC_HW(pstrAesCbcCxt, pu8IV, pu8Data, u16DataSize);
	}
	else
	{
		AES_CbcEncSW(pstrAesCbcCxt, pu8IV, pu8Data, u16DataSize);
	}
}
/*********************************************************************
Function
	AES_CbcDec

Description
	Perform AES decryption in CBC mode. The decryption is performed in place.

Return

Author
	Ahmed Ezzat

Version
	1.0

Date
	11 Nov 2012
*********************************************************************/
void AES_CbcDecrypt
(
tstrAesCbcContext	*pstrAesCbcCxt,
uint8 				*pu8IV,
uint8 				*pu8Data,
uint16				u16DataSize
)
{
	if(pstrAesCbcCxt->bIsHwAes)
	{
		AES_CBC_DEC_HW(pstrAesCbcCxt, pu8IV, pu8Data, u16DataSize);
	}
	else
	{
		AES_CbcDecSW(pstrAesCbcCxt, pu8IV, pu8Data, u16DataSize);
	}
}
/*********************************************************************
Function
	AES_CbcDec

Description
	Perform AES decryption in CBC mode. The decryption is performed in place.

Return

Author
	Ahmed Ezzat

Version
	1.0

Date
	11 Nov 2012
*********************************************************************/
void AES_CbcDecLegacy
(
uint8 	*pu8IV,
uint8 	*pu8Data,
uint16	u16DataSize,
uint8	*pu8Key,
uint8	u8KeySize
)
{
	tstrAesCbcContext	strAesCxt;
	AES_CbcContextInit(&strAesCxt, pu8Key, u8KeySize);
	AES_CbcDecrypt(&strAesCxt, pu8IV, pu8Data, u16DataSize);
}
/*********************************************************************
Function
	AES_CbcEnc

Description

Return
	None.

Author
	Ahmed Ezzat

Version
	1.0

Date
*********************************************************************/
void AES_CbcEncLegacy
(
uint8 	*pu8IV,
uint8 	*pu8Data,
uint16	u16DataSize,
uint8	*pu8Key,
uint8	u8KeySize
)
{
	tstrAesCbcContext	strAesCxt;
	AES_CbcContextInit(&strAesCxt, pu8Key, u8KeySize);
	AES_CbcEncrypt(&strAesCxt, pu8IV, pu8Data, u16DataSize);
}
