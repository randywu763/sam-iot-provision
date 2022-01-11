/*!
@file       		
	asn1.c

@brief	
	Implementation for ASN.1 Decoder for Distinguished Encoding Rules DER.
*/

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "asn1.h"

/*********************************************************************
Function
	ASN1_GetNextElement

Description
	

Return
	None
*********************************************************************/
uint16 ASN1_GetNextElement
(
tstrAsn1Context 	*pstrAsn1Ctxt,
tstrAsn1Element 	*pstrElement
)
{
	uint16	u16ElemLength = 0;
	
	if((pstrElement != NULL) && (pstrAsn1Ctxt != NULL))
	{
		uint8	u8NLenBytes;

		do
		{
			u8NLenBytes = 0;
			
			/* Get the ASN.1 Element Tag. 
			*/
			TLS_BufferRead(pstrAsn1Ctxt->pstrTlsBuffer, 1, &pstrElement->u8Tag);

			/* Get the ASN.1 element length. 
			*/
			TLS_BufferRead(pstrAsn1Ctxt->pstrTlsBuffer, 1, (uint8*)&pstrElement->u32Length);
			pstrElement->u32Length &= 0xFF;
			if(pstrElement->u32Length & BIT7)
			{
				uint8	u8Idx;
				uint8	au8Tmp[4];

				/* Multiple Length octets. 
				*/
				u8NLenBytes = pstrElement->u32Length & 0x03;
				pstrElement->u32Length = 0;
				TLS_BufferRead(pstrAsn1Ctxt->pstrTlsBuffer, u8NLenBytes, au8Tmp);
				
				for(u8Idx = 0 ; u8Idx < u8NLenBytes ; u8Idx ++)
					pstrElement->u32Length  += 
						(uint32)(au8Tmp[u8Idx] << ((u8NLenBytes - u8Idx - 1) * 8));
			}
			u16ElemLength += (uint16)(u8NLenBytes + 2 + pstrElement->u32Length);
		}while(pstrElement->u8Tag == ASN1_NULL);
	}
	return u16ElemLength;
}
/*********************************************************************
Function
	ASN1_Read

Description
	

Return
	None
*********************************************************************/
uint16 ASN1_Read
(
tstrAsn1Context	*pstrAsn1Cxt,
uint32			u32ReadLength,
uint8			*pu8ReadBuffer
)
{
	uint16	u16Read = 0;

	if((pstrAsn1Cxt != NULL) && (u32ReadLength > 0))
	{
		u16Read = TLS_BufferRead(pstrAsn1Cxt->pstrTlsBuffer, (uint16)u32ReadLength, pu8ReadBuffer);
	}
	return u16Read;
}
