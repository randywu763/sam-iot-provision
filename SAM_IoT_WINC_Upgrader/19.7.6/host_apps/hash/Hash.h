/*!
@file       		hash.h

@brief			APIs and data types for hash algorithms.

@author   		Ahmed Ezzat

@date      		11 April 2013

@version  		1.0
*/
#ifndef __HASH_H__
#define __HASH_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "common.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define SHA_BLOCK_SIZE					64

#define MD5_DIGEST_SIZE					(16)
#define SHA1_DIGEST_SIZE				(20)
#define SHA256_DIGEST_SIZE 				32

#define SHA_MAX_DIGEST_SIZE				SHA256_DIGEST_SIZE

#define SHA_FLAGS_INIT					0x01
/*!< A flag to tell the hash function to 
	start hash from the initial state.
*/

#define SHA_FLAGS_UPDATE				0x02
/*!< Update the current hash with the 
	given data.
*/


#define SHA_FLAGS_FINISH				0x04
/*!< Finalize the hashing and calculate
	the final result.
*/


#define SHA_FLAGS_FULL_HASH			(SHA_FLAGS_INIT | SHA_FLAGS_UPDATE | SHA_FLAGS_FINISH)


#define SHA1_INIT(ctxt)					SHA1_Hash((ctxt), SHA_FLAGS_INIT, NULL, 0, NULL)
#define SHA1_UPDATE(ctxt,data,dataLen)	SHA1_Hash((ctxt), SHA_FLAGS_UPDATE, (data), (dataLen), NULL)
#define SHA1_FINISH(ctxt,digest)			SHA1_Hash((ctxt), SHA_FLAGS_FINISH, NULL, 0, digest)

#define MD5_INIT(ctxt)					MD5_Hash((ctxt), SHA_FLAGS_INIT, NULL, 0, NULL)
#define MD5_UPDATE(ctxt,data,dataLen)	MD5_Hash((ctxt), SHA_FLAGS_UPDATE, (data), (dataLen), NULL)
#define MD5_FINISH(ctxt,digest)			MD5_Hash((ctxt), SHA_FLAGS_FINISH, NULL, 0, digest)

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

typedef void (*tpfHashProcessBlock)(uint32* pu32HashState, const uint8* pu8MessageBlock);

typedef struct{
	uint32				au32HashState[SHA_MAX_DIGEST_SIZE/4];
	uint8				au8CurrentBlock[SHA_BLOCK_SIZE];
	uint32				u32TotalLength;
	tpfHashProcessBlock	fpHash;
	uint8				u8BlockSize;
	uint8				u8DigestSize;
}tstrHashContext;

typedef tstrHashContext	tstrMd5Context;
typedef tstrHashContext	tstrSha1Context;
typedef tstrHashContext	tstrSha256Context;

typedef void (*tpfHash)
(
	tstrHashContext	*pstrHashContext,
	uint8			u8Flags,
	uint8			*pu8Data, 
	uint32 		u32DataLength, 
	uint8			*pu8Digest
);

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


void MD5_Hash(tstrHashContext *pstrMD5Context, uint8 u8Flags, uint8 *pu8Data, uint32 u32DataLength,uint8 *pu8Digest);

/*!
@fn			BOOL_T HMAC_SHA256(uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Key, uint32 u32KeyLength, UWORD8 *pu8Out)

@brief		
			Performs keyed-Hash Message Authentication Code (HMAC) to a block of data

@author		Adham Abozaeid

@param [in]	pu8Data			
				Data to be HMACed. Can't be NULL
				
@param [in] 	u32DataLength	
				Length of the data to be HMACed
				
@param [in] 	pu8Key			
				The key used for HMAC. Can't be NULL
				
@param [in] 	u32KeyLength	
				Length of the supplied key
				
@param [out]	pu8Out			
				Buffer that will hold the HMAC value of the supplied data and key. It's size should be a minimum of 32 byte
				
@return		Boolean indicating the success state of the operation

@date		07 June 2009

@sa			RFC 2104
*/
void HMAC_Vector
(
	tpfHash			fpHash,
	uint8			*pu8Key,
	uint32 		u32KeyLength, 
	tstrBuffer			*pstrInData,
	uint8			u8NumInputs,
	uint8			*pu8Out
);


void SHA1_Hash
(
	tstrHashContext	*pstrSha1Context,
	uint8			u8Flags,
	uint8 		*pu8Data, 
	uint32 		u32DataLength, 
	uint8 		*pu8Digest
);

void SHA256_Hash
(
	tstrHashContext		*pstrSha256Context,
	uint8				u8Flags,
	uint8 			*pu8Data, 
	uint32 			u32DataLength, 
	uint8 			*pu8Digest
);


#endif /* __HASH_H__ */
