/*!
@file       		
	tls_buffer_manager.h

@brief			
	APIs and types for TLS  buffer manager.
*/

#ifndef __TLS_BUFFER_MANAGER_H__
#define __TLS_BUFFER_MANAGER_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "tls_port.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


#define TLS_BUFFER_SEEK_SET						(1)
/*!<
	Move the current read cursor of the TLS Buffer from start.
*/


#define TLS_BUFFER_SEEK_END						(2)
/*!<
	Move the current read cursor of the TLS Buffer from end.
*/


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

typedef struct{
	uint8		*pu8Buff;
	uint16		u16BuffSz;
	uint16		u16ReadOffset;
}tstrTlsBuffer;

/*!
@struct	\
	tstrTLSBufferPos
*/
typedef struct{
	uint16	u16BuffOffset;
}tstrTLSBufferPos;


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

uint16 TLS_BufferReadCurrEntry
(
tstrTlsBuffer 	*pstrTlsBuff, 
uint16		u16ReadByteCount,
uint8		**ppu8Buffer,
BOOL_T		bUpdateCursor
);

uint16 TLS_BufferRead
(
tstrTlsBuffer	*pstrTlsBuff,
uint16		u16ReadByteCount,
uint8		*pu8ReadBuffer
);

void TLS_BufferSetPos
(
tstrTlsBuffer 		*pstrTlsBuff, 
tstrTLSBufferPos 	*pstrNewPos
);


void TLS_BufferGetPos
(
tstrTlsBuffer 		*pstrTlsBuff, 
tstrTLSBufferPos 	*pstrCurrPos
);



void TLS_BufferSeek
(
tstrTlsBuffer	*pstrTlsBuff,
uint16		u16Offset,
uint8		u8Origin
);


void TLS_BufferTrim
(
tstrTlsBuffer	*pstrTlsBuff,
uint16		u16Offset,
uint8		u8Origin
);



#endif /* __TLS_BUFFER_MANAGER_H__ */
