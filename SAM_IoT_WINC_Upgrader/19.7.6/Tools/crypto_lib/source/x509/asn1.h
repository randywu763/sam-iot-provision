/*!
@file       		
	asn1.h

@brief			

*/

#ifndef __ASN1_H__
#define __ASN1_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "m2m_types.h"
#include "tls_buffer_manager.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
		ASN.1 TAG DEFINITIONS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define ASN1_INVALID					0x00
#define ASN1_BOOLEAN					0x01
#define ASN1_INTEGER					0x02
#define ASN1_BIT_STRING					0x03
#define ASN1_OCTET_STRING				0x04
#define ASN1_NULL						0x05
#define ASN1_OBJECT_IDENTIFIER			0x06
#define ASN1_UTF8_DTRING				0x0C
#define ASN1_PRINTABLE_STRING			0x13
#define ASN1_TELETEX_STRING				0x14
#define ASN1_UTC_TIME					0x17
#define ASN1_GENERALIZED_TIME			0x18
#define ASN1_SEQUENCE					0x30
#define ASN1_SET						0x31

#define ASN1_SUCCESS					0
#define ASN1_FAIL						-1

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

typedef struct{
	tstrTlsBuffer		*pstrTlsBuffer;
}tstrAsn1Context;

typedef struct{
	uint32	u32Length;
	uint8	u8Tag;
}tstrAsn1Element;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/



uint16 ASN1_GetNextElement(tstrAsn1Context *pstrAsn1Ctxt, tstrAsn1Element *pstrElement);

uint16 ASN1_Read
(
tstrAsn1Context	*pstrAsn1Cxt,
uint32			u32ReadLength,
uint8			*pu8ReadBuffer
);

#endif /* __ASN1_H__ */
