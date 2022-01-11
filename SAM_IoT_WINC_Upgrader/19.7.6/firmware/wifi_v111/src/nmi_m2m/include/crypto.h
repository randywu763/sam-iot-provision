/*!
@file       		
	crypto.h

@brief			
	APIs and data types for cryptographic algorithms.

@author   		
	Ahmed Ezzat

@date      		
	27 Feb 2013
*/
#ifndef __CRYPTO_H__
#define __CRYPTO_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "crypto_conf.h"
#include "m2m_types.h"
#include "ecc_types.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/*======*======*======*======*
HASH RELATED DEFINITIONS
*======*======*======*======*/

#define SHA512_BLOCK_SIZE					(128)
#define SHA_BLOCK_SIZE						(64)

#define MD4_DIGEST_SIZE						(16)
#define MD5_DIGEST_SIZE						(16)
#define SHA1_DIGEST_SIZE					(20)
#define SHA224_DIGEST_SIZE					(28)
#define SHA256_DIGEST_SIZE 					(32)
#define SHA384_DIGEST_SIZE					(48)
#define SHA512_DIGEST_SIZE					(64)

#if (defined __CRYPTO_SHA512_SUPPORT__) || (defined __CRYPTO_SHA384_SUPPORT__)
#define SHA_MAX_DIGEST_SIZE					SHA512_DIGEST_SIZE
#else
#	define SHA_MAX_DIGEST_SIZE				SHA256_DIGEST_SIZE
#endif

#define SHA_FLAGS_INIT						0x01
/*!<
	A flag to tell the hash function to start hash from 
	the initial state.
*/


#define SHA_FLAGS_UPDATE					0x02
/*!< 
	Update the current hash with the given data.
*/


#define SHA_FLAGS_FINISH					0x04
/*!< 
	Finalize the hashing and calculate the final result.
*/


#define SHA_FLAGS_FULL_HASH					(SHA_FLAGS_INIT | SHA_FLAGS_UPDATE | SHA_FLAGS_FINISH)
/*!< 
	Perform Full hash operation with the DIGEST
	returned to the caller.
*/



#define SHA1_INIT(ctxt)						SHA1_Hash((ctxt), SHA_FLAGS_INIT, NULL, 0, NULL)
#define SHA1_UPDATE(ctxt,data,dataLen)		SHA1_Hash((ctxt), SHA_FLAGS_UPDATE, (data), (dataLen), NULL)
#define SHA1_FINISH(ctxt,digest)			SHA1_Hash((ctxt), SHA_FLAGS_FINISH, NULL, 0, digest)

#define MD5_INIT(ctxt)						MD5_Hash((ctxt), SHA_FLAGS_INIT, NULL, 0, NULL)
#define MD5_UPDATE(ctxt,data,dataLen)		MD5_Hash((ctxt), SHA_FLAGS_UPDATE, (data), (dataLen), NULL)
#define MD5_FINISH(ctxt,digest)				MD5_Hash((ctxt), SHA_FLAGS_FINISH, NULL, 0, digest)

#define SHA256_INIT(ctxt)					SHA256_Hash((ctxt), SHA_FLAGS_INIT, NULL, 0, NULL)
#define SHA256_UPDATE(ctxt,data,dataLen)	SHA256_Hash((ctxt), SHA_FLAGS_UPDATE, (data), (dataLen), NULL)
#define SHA256_FINISH(ctxt,digest)			SHA256_Hash((ctxt), SHA_FLAGS_FINISH, NULL, 0, digest)


/*======*======*======*======*
ENCRYPTION RELATED DEFINITIONS
*======*======*======*======*/

#define AES_BLOCK_SIZE 							(16UL)

#ifdef __AES_HW_ENGINE__
#define AES_CBC_HW(kSz)							((kSz) == SIZE_128_BITS)
#define AES_CBC_ENC_HW(cxt,iv,data,datasz)		AES_CbcEncHW(cxt,iv,data,datasz)
#define AES_CBC_DEC_HW(cxt,iv,data,datasz)		AES_CbcDecHW(cxt,iv,data,datasz)

#else
#define AES_CBC_HW(kSz)							(0)
#define AES_CBC_ENC_HW(cxt,iv,data,datasz)		
#define AES_CBC_DEC_HW(cxt,iv,data,datasz)		
#endif

/*======*======*======*======*
BIGINT RELATED DEFINITIONS
*======*======*======*======*/

#define BI_SUCCESS							(0)
#define BI_FAIL								(-10)
#define BITS_PER_DIGIT						((uint32)32)





#define SHA_SHR(x,n) 						((x & 0xFFFFFFFF) >> n)
#define SHA_SHL(x,n) 						((x & 0xFFFFFFFF) << n)

#define SHA_SHR_64(high,low,n,high_o,low_o)	(high_o = SHA_SHR(high,n));\
											(low_o  =  SHA_SHR(low,n) | SHA_SHL(high , (32 - n)))
#define SHA_SHL_64(high,low,n,high_o,low_o) (high_o = SHA_SHL(high,n) | SHA_SHR(low , (32 - n)));\
											(low_o  =  SHA_SHL(low,n))
											
#define SHA_ROTR(x,n) 						(SHA_SHR(x,n) | SHA_SHL(x , (32 - n)))
#define SHA_ROTL(x,n) 						(SHA_SHL(x,n) | SHA_SHR(x , (32 - n)))

#define SHA_ROTR_64(high,low,n,high_o,low_o) (high_o = SHA_SHR(high,n) | SHA_SHL(low , (32 - n)));\
											(low_o  =  SHA_SHR(low,n) | SHA_SHL(high , (32 - n)))
#define SHA_ROTL_64(high,low,n,high_o,low_o) (high_o = SHA_SHL(high,n) | SHA_SHR(low , (32 - n)));\
											(low_o  =  SHA_SHL(low,n) | SHA_SHR(high , (32 - n)))


#define XOR									^

#define BLOCK_XOR(X,Y,Z)	\
	do\
	{\
		(Z)[0]	= (X)[0] ^ (Y)[0];\
		(Z)[1]	= (X)[1] ^ (Y)[1];\
		(Z)[2]	= (X)[2] ^ (Y)[2];\
		(Z)[3]	= (X)[3] ^ (Y)[3];\
		(Z)[4]	= (X)[4] ^ (Y)[4];\
		(Z)[5]	= (X)[5] ^ (Y)[5];\
		(Z)[6]	= (X)[6] ^ (Y)[6];\
		(Z)[7]	= (X)[7] ^ (Y)[7];\
		(Z)[8]	= (X)[8] ^ (Y)[8];\
		(Z)[9]	= (X)[9] ^ (Y)[9];\
		(Z)[10]	= (X)[10] ^ (Y)[10];\
		(Z)[11]	= (X)[11] ^ (Y)[11];\
		(Z)[12]	= (X)[12] ^ (Y)[12];\
		(Z)[13]	= (X)[13] ^ (Y)[13];\
		(Z)[14]	= (X)[14] ^ (Y)[14];\
		(Z)[15]	= (X)[15] ^ (Y)[15];\
	}while(0)

/*!< 
	Bitwise XOR.
*/


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


/*======*======*======*======*
PUBLIC KEY SPECIFIC DATA TYPES
*======*======*======*======*/


typedef enum{
	PUBKEY_ALG_NONE		= 0,
	PUBKEY_ALG_RSA		= 1,
#ifdef __TLS_DHE_SUPPORT__
	PUBKEY_ALG_DH		= 2,
#endif
#ifdef __TLS_ECC_SUPPORT__
	PUBKEY_ALG_ECC		= 3
#endif
}tenuPubKeyAlg;


typedef enum{
	SIGN_VERIFY,
	SIGN_GEN
}tenuSignMode;


/*!
@enum	\
	tenuTlsSignAlg

@brief	TLS Defined Signature Algorithms

	Public Key Signature algorithms mapped to the IDs defined by the TLS protocol specifications. 
	It is used in the signature_algorithms Hello Extension that must be included in the
	TLS.Handshake.ClientHello message starting from TLS 1.2.
*/
typedef enum{
	TLS_SIG_ANON		 = 0,
	TLS_SIG_ALG_RSA		 = 1,
	TLS_SIG_ALG_DSA		 = 2,
#ifdef __TLS_ECC_SUPPORT__
	TLS_SIG_ALG_ECDSA	 = 3
#endif
}tenuTlsSignAlg;

#ifdef __TLS_ECC_SUPPORT__
/*!
@struct	\
	tstrECPublicKey
	
@brief	Elliptic Curve Public Key Definition
*/
typedef struct{
	tstrEllipticCurve	*pstrCurve;
	tstrECPoint			strQ;
}tstrECPublicKey;
#endif

#ifdef __TLS_DHE_SUPPORT__
/*!
@struct	\
	tstrDHPublicKey
	
@brief	Diffie-Hellman Public Key Definition
*/
typedef struct{
	uint8	*pu8P;
	uint8	*pu8G;
	uint8	*pu8Key;
	uint8	*pu8Priv;
	uint16	u16PSize;
	uint16	u16GSize;
	uint16	u16KeySize;
	uint16	u16PrivSize;
}tstrDHPublicKey;
#endif

/*!
@struct	\
	tstrRSAPublicKey
	
@brief	RSA Public Key Definition
*/
typedef struct{
	uint16	u16NSize;
	uint16	u16ESize;
	uint8	*pu8N;
	uint8	*pu8E;
}tstrRSAPublicKey;


/*!
@struct	\
	tstrPublicKey
	
@brief	Generic Public Key Structure
*/
typedef struct{
	tenuPubKeyAlg	enuType;
	/*!<
		Type of the underlying public key algorithm.
	*/
	union{
		tstrRSAPublicKey	strRSAKey;
#ifdef __TLS_DHE_SUPPORT__
		tstrDHPublicKey		strDHKey;
#endif
#ifdef __TLS_ECC_SUPPORT__
		tstrECPublicKey		strEccKey;
#endif
	};
}tstrPublicKey;


/*!
@struct	\
	tstrSignREQ
	
@brief	Signature Verification Parameters
*/
typedef struct{
	uint8			*pu8Hash;
	uint8			*pu8Sig;
	tstrPublicKey	*pstrPubKey;
	uint16			u16HashSz;
	uint16			u16SigSz;
}tstrSignREQ;

#ifdef __TLS_RSA_CS_SUPPORT__
typedef tstrRSAPublicKey	tstrRSAPrivateKey;
#endif


/*======*======*======*======*
HASH SPECIFIC DATA TYPES
*======*======*======*======*/

/*!
@typedef	\
	tpfHashProcessBlock
	
@brief
*/
typedef void (*tpfHashProcessBlock)(uint32* pu32HashState, const uint8* pu8MessageBlock);


/*!
@struct	\
	tstrHashContext
	
@brief
*/
typedef struct{
	uint32				au32HashState[SHA256_DIGEST_SIZE/4];
	uint8				au8CurrentBlock[SHA_BLOCK_SIZE];
	uint32				u32TotalLength;
	tpfHashProcessBlock	fpHash;
	uint8				u8BlockSize;
	uint8				u8DigestSize;
}tstrHashContext;

#ifdef USE_HASH512CONTEXT
/*!
@struct	\
	tstrHash512Context
	
@brief
*/
typedef struct{
	uint32				au32HashState[SHA_MAX_DIGEST_SIZE/4];
	uint8				au8CurrentBlock[SHA512_BLOCK_SIZE];
	uint32				u32TotalLength;
	tpfHashProcessBlock	fpHash;
	uint8				u8BlockSize;
	uint8				u8DigestSize;
}tstrHash512Context;
#endif


/*!
*/
typedef struct{
	union{
		tstrHashContext		strSha;
#ifdef USE_HASH512CONTEXT
		tstrHash512Context	strSha512;
#endif
	};
}tstrShaHashContext;


/*!
@typedef	\
	tstrMd5Context
	
@brief
*/
typedef tstrShaHashContext	tstrMd5Context;


/*!
@typedef	\
	tstrSha1Context
	
@brief
*/
typedef tstrShaHashContext	tstrSha1Context;


/*!
@typedef	\
	tstrSha256Context
	
@brief
*/
typedef tstrShaHashContext	tstrSha256Context;


/*!
@typedef	\
	tpfHash
	
@brief
*/
typedef void (*tpfHash)
(
tstrShaHashContext	*pstrHashContext,		
uint8				u8Flags,
uint8				*pu8Data, 
uint32 				u32DataLength, 
uint8				*pu8Digest
);


/*!
@enum	\
	tenuHashAlg

@brief	TLS Defined Hash Algorithms

	Hash algorithms mapped to the IDs defined by the TLS protocol specifications. It is 
	used in the signature_algorithms Hello Extension that must be included in the
	TLS.Handshake.ClientHello message starting from TLS 1.2.
	Although defined for TLS, these hash definitions get used for non-TLS purposes also.
*/
typedef enum{
	HASH_ALG_NONE	= 0,
	HASH_ALG_MD5	= 1,
	HASH_ALG_SHA1	= 2,
#ifdef __CRYPTO_SHA224_SUPPORT__
	HASH_ALG_SHA224	= 3,
#endif
	HASH_ALG_SHA256	= 4,
#ifdef __CRYPTO_SHA384_SUPPORT__
	HASH_ALG_SHA384	= 5,
#endif
#ifdef __CRYPTO_SHA512_SUPPORT__
	HASH_ALG_SHA512	= 6,
#endif
	HASH_ALG_MAX
}tenuHashAlg;


typedef struct{
	tpfHash	fpHash;
	uint16	u16HashSz;
	uint16	u16BlockSz;
}tstrHashInfo;


/*======*======*======*======*
AES SPECIFIC DATA TYPES
*======*======*======*======*/

/*!
@struct	\
	aes_context_t
	
@brief
*/
typedef struct{
	uint32 	keys[60];
	uint32 	ikeys[60];
	uint32 	nrounds;
} aes_context_t;


typedef struct{
	uint8			au8Key[SIZE_256_BITS];
	uint8			u8KeySize;
	uint8			bIsHwAes;
}tstrAesCbcContext;

#ifdef __CRYPTO_AES_GCM_SUPPORT__
typedef struct{
	uint8			H[AES_BLOCK_SIZE];
	uint8			au8Key[SIZE_256_BITS];
	uint8			u8KeySize;
	uint8			au8TH[16][16];
	uint8			au8TL[16][16];
}tstrAesGCMContext;
#endif

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/*======*
RNG APIs
*======*/

void	aes_prng_init(void);
uint8	aes_prng_get_random_byte(void);
sint8	aes_prng_get_random_bytes(uint8 * pu8Buff, uint32 u32Size);


/*======*======*======*======*
HASH ALGORITHMS
*======*======*======*======*/

/*
	SW HASH FUNCTIONS
*/
#if __ROM_VER__ < ROM_VER_1
void 	MD5_HashSW(tstrHash512Context * pstrMD5Cxt, uint8 u8Flags, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);
void 	SHA1_HashSW(tstrHash512Context * pstrSha1Cxt, uint8 u8Flags, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);
void 	SHA256_HashSW(tstrHash512Context * pstrSha256Cxt, uint8 u8Flags, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);
#endif
#ifdef __CRYPTO_SHA224_SUPPORT__
void 	SHA224_HashSW(tstrHash512Context * pstrSha224Cxt, uint8 u8Flags, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);
#endif
#if (defined __CRYPTO_SHA384_SUPPORT__) || (defined __CRYPTO_SHA512_SUPPORT__)
void 	SHA512_HashSW(tstrHash512Context * pstrSha512Cxt, uint8 u8Flags, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest, uint8 u8IsSHA384);
#endif
void 	MD4_Hash(uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);

/*
	HASH INTERFACE APIs (A unified interface is used for simplicity)
*/
void 	MD5_Hash(tstrShaHashContext * pstrSha1Context, uint8 u8Flags, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);
void 	SHA1_Hash(tstrShaHashContext * pstrSha1Context, uint8 u8Flags, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);
void	SHA256_Hash(tstrShaHashContext * pstrSha1Context, uint8 u8Flags, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);
#ifdef __CRYPTO_SHA224_SUPPORT__
void 	SHA224_Hash(tstrShaHashContext * pstrSha1Context, uint8 u8Flags, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);
#endif
#ifdef __CRYPTO_SHA384_SUPPORT__
void 	SHA384_Hash(tstrShaHashContext * pstrSha1Context, uint8 u8Flags, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);
#endif
#ifdef __CRYPTO_SHA512_SUPPORT__
void 	SHA512_Hash(tstrShaHashContext * pstrSha1Context, uint8 u8Flags, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);
#endif

/*
	HMAC-Hash APIs
*/
void 	hmac_md5(uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Key, uint32 u32KeyLength, uint8 * pu8Digest);
void 	hmac_sha1(uint8 * pu8Key, uint32 u32KeyLength, uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Digest);
void 	HMAC_SHA256(uint8 * pu8Data, uint32 u32DataLength, uint8 * pu8Key, uint32 u32KeyLength, uint8 * pu8Digest);
void 	HMAC_Vector(tenuHashAlg enuHashAlg, uint8 * pu8Key, uint32 u32KeyLength, tstrBuffer * pstrInData, uint8 u8NumInputs, uint8 * pu8Out);


/*======*======*======*======*
RSA CRYPTOGRAPHIC APIs
*======*======*======*======*/

sint8 	RSA_SignVerify(tstrRSAPublicKey * pstrKey, uint8 * pu8Signature, uint16 u16SignLen, uint8 * pu8Hash, uint16 u16HashLength);
#ifdef __TLS_RSA_CS_SUPPORT__
uint16	RSA_Encrypt(tstrRSAPublicKey * pstrKey, uint8 * pu8Plain, uint16 u16PlainSize, uint8 * pu8Cipher);
#ifdef __TLS_SRV_SUPPORT__
uint16 	RSA_Decrypt(tstrRSAPrivateKey * pstrKey, uint8 * pu8Cipher, uint16 u16CipherSize, uint8 * pu8Plain);
#endif
sint8 	RSA_SignGen(tstrRSAPublicKey * pstrKey, uint8 * pu8Signature, uint8 * pu8Hash, uint16 u16HashLength);
#endif

/*======*======*======*======*
ENCRYPTION/DECRYPTION
*======*======*======*======*/

/*
	DES Encryption.
*/
void DES_EncryptBlock(uint8 * pu8PlainTxt, uint8 * pu8CipherKey, uint8 * pu8CipherTxt);

/*
	AES Basic Block Cipher APIs.
*/
void aes_setup(aes_context_t *ctx, uint8 keysize, uint8 *key);
void aes_encrypt(aes_context_t *ctxt, uint8 *ptext, uint8 *ctext);
void aes_decrypt(aes_context_t *ctxt, uint8 *ctext, uint8 *ptext);

/*
	AES CBC Mode APIs.
*/
void AES_CbcContextInit(tstrAesCbcContext * pstrAesCbcCxt, uint8 * pu8Key, uint8 u8KeySize);
void AES_CbcEncrypt(tstrAesCbcContext * pstrAesCbcCxt, uint8 * pu8IV, uint8 * pu8Data, uint16 u16DataSize);
void AES_CbcDecrypt(tstrAesCbcContext * pstrAesCbcCxt, uint8 * pu8IV, uint8 * pu8Data, uint16 u16DataSize);
void AES_CbcDecLegacy(uint8 * pu8IV, uint8 * pu8Data, uint16 u16DataSize, uint8 * pu8Key, uint8 u8KeySize);
void AES_CbcEncLegacy(uint8 * pu8IV, uint8 * pu8Data, uint16 u16DataSize, uint8 * pu8Key, uint8 u8KeySize);

#ifdef __CRYPTO_AES_GCM_SUPPORT__
/*
	AES GCM Mode APIs.
*/
void AES_GCMContextInit(tstrAesGCMContext * pstrAesGCMCxt, uint8 * pu8Key, uint8 u8KeySize);
sint8 AES_GCMEncrypt(tstrAesGCMContext	*pstrAesGCMCxt, uint8 * pu8IV, uint8 u8IVLen, uint8 * pu8AAD, uint8 u8AADLen, uint8 * pu8Data, uint16 u16DataLen, uint8 * pu8T);
sint8 AES_GCMDecrypt(tstrAesGCMContext	*pstrAesGCMCxt, uint8 * pu8IV, uint8 u8IVLen, uint8 * pu8AAD, uint8 u8AADLen, uint8 * pu8Data, uint16 u16DataLen, uint8 * pu8T);
#endif

/*======*======*======*======*
BIGINT LIBRARY APIs
*======*======*======*======*/


sint32 BI_ModExp(uint8 * pu8X, uint32 u32XSize, uint8 * pu8E, uint32 u32ESize, uint8 * pu8N, uint32 u32NSize, uint8 * pu8Y, uint32 u32YSize);


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
EXTERNALS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

extern tstrHashInfo		eastrTlsHashes[HASH_ALG_MAX];


#endif /* __CRYPTO_H__ */
