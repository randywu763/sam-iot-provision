#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>

typedef unsigned long uint32;
typedef unsigned char uint8;
typedef unsigned short uint16;

#define BYTE_0(word)   					((uint8)(((word) >> 0 	) & 0x000000FFUL))
#define BYTE_1(word)  	 				((uint8)(((word) >> 8 	) & 0x000000FFUL))
#define BYTE_2(word)   					((uint8)(((word) >> 16) & 0x000000FFUL))
#define BYTE_3(word)   					((uint8)(((word) >> 24) & 0x000000FFUL))


typedef struct{
	uint16	u16BufferSize;
	uint8	*pu8Data;
}tstrBuffer;



#define SIZE_128_BITS					(16)
#define SIZE_256_BITS					(32)
#define SIZE_512_BITS					(64)
#define SIZE_1024_BITS					(128)
#define SIZE_1536_BITS					(192)
#define SIZE_2048_BITS					(256)


#define GETU16(BUF,OFFSET)				((((uint16)((BUF)[OFFSET]) << 8)) |(((uint16)(BUF)[OFFSET + 1])))
/*!< Retrieve 2 bytes from the given buffer at the given 
	offset as 16-bit unsigned integer in the Network byte order.
*/


#define GETU32(BUF,OFFSET)				((((uint32)((BUF)[OFFSET]) << 24)) | (((uint32)((BUF)[OFFSET + 1]) << 16)) | \
										(((uint32)((BUF)[OFFSET + 2]) << 8)) | ((uint32)((BUF)[OFFSET + 3])))
/*!< Retrieve 4 bytes from the given buffer at the given 
	offset as 32-bit unsigned integer in the Network byte order.
*/



#define PUTU32(VAL32,BUF,OFFSET)	\
do	\
{	\
	(BUF)[OFFSET	] = BYTE_3((VAL32));	\
	(BUF)[OFFSET +1	] = BYTE_2((VAL32));	\
	(BUF)[OFFSET +2	] = BYTE_1((VAL32));	\
	(BUF)[OFFSET +3	] = BYTE_0((VAL32));	\
}while(0)


#define PUTU16(VAL16,BUF,OFFSET)	\
do	\
{	\
	(BUF)[OFFSET	] = BYTE_1((VAL16));		\
	(BUF)[OFFSET +1	] = BYTE_0((VAL16));		\
}while(0)
	

#endif /* _COMMON_H_ */