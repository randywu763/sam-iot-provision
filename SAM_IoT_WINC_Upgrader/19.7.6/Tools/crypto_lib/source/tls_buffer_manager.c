/*!
@file       		
	tls_buffer_manager.c

@brief			
	Implementation for TLS  buffer manager.
*/

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "tls_buffer_manager.h"


/**************************************************************/
uint16 TLS_BufferRead(tstrTlsBuffer	*pstrTlsBuff, uint16 u16ReadByteCount, uint8 *pu8ReadBuffer)
{
	uint16	u16Read = 0;
	
	if(pstrTlsBuff != NULL)
	{
		if((u16ReadByteCount + pstrTlsBuff->u16ReadOffset) < pstrTlsBuff->u16BuffSz)
		{
			u16Read = u16ReadByteCount;
		}
		else
		{
			u16Read = pstrTlsBuff->u16BuffSz - pstrTlsBuff->u16ReadOffset;
		}

		if(pu8ReadBuffer != NULL)
			M2M_MEMCPY(pu8ReadBuffer, &pstrTlsBuff->pu8Buff[pstrTlsBuff->u16ReadOffset], u16Read);
		
		pstrTlsBuff->u16ReadOffset += u16Read;
	}
	return u16Read;
}

/**************************************************************/
uint16 TLS_BufferReadCurrEntry(tstrTlsBuffer *pstrTlsBuff, uint16 u16ReadByteCount, uint8 **ppu8Buffer, BOOL_T bUpdateCursor)
{
	uint16	u16Read = 0;
	
	if(pstrTlsBuff != NULL)
	{
		uint16	u16nBytes = pstrTlsBuff->u16BuffSz - pstrTlsBuff->u16ReadOffset;
		*ppu8Buffer = &pstrTlsBuff->pu8Buff[pstrTlsBuff->u16ReadOffset];
		if(u16nBytes > u16ReadByteCount)
		{				
			u16Read = u16ReadByteCount;
		}
		else
		{
			u16Read = u16nBytes;
		}
		if(bUpdateCursor == BTRUE)
			pstrTlsBuff->u16ReadOffset += u16Read;
	}
	return u16Read;
}

/**************************************************************/
void TLS_BufferSetPos(tstrTlsBuffer *pstrTlsBuff, tstrTLSBufferPos *pstrNewPos)
{
	if(pstrTlsBuff != NULL)
	{
		pstrTlsBuff->u16ReadOffset = pstrNewPos->u16BuffOffset;
	}
}

/**************************************************************/
void TLS_BufferGetPos(tstrTlsBuffer *pstrTlsBuff, tstrTLSBufferPos *pstrCurrPos)
{
	if(pstrTlsBuff != NULL)
	{
		pstrCurrPos->u16BuffOffset = pstrTlsBuff->u16ReadOffset;
	}	
}
