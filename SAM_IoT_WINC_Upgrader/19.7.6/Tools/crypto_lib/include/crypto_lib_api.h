
#ifndef __CRYPTO_LIB_API_H__
#define __CRYPTO_LIB_API_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "common/include/nm_common.h"
#include "driver/include/m2m_types.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#ifdef __GNUC__
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#endif // __GNUC__

#define CRYPTO_AES_BLOCK_SZ				(16)
#define CRYPTO_SHA1_DIGEST_SIZE			(20)
#define CRYPTO_SHA256_DIGEST_SIZE		(32)
#define ECC_POINT_MAX					(72)

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/*!
@enum\
	tenuCertPubKeyType
*/
typedef enum{
	X509_CERT_PUBKEY_RSA	= 1,
	X509_CERT_PUBKEY_ECDSA	= 2
}tenuCertPubKeyType;


/*!
@struct	\
	tstrRsaPrivateKey
*/
typedef struct{
	uint16	u16NSize;
	uint16	u16eSize;
	uint16	u16dSize;
	uint16	u16PSize;
	uint16	u16QSize;
	uint16	u16dPSize;
	uint16	u16dQSize;
	uint16	u16QInvSize;
	uint32	u32Version;
	uint8	*pu8N;
	uint8	*pu8e;
	uint8	*pu8d;
	uint8	*pu8p;
	uint8	*pu8q;
	uint8	*pu8dP;
	uint8	*pu8dQ;
	uint8	*pu8QInv;
}tstrRsaPrivateKey;


/*!
@struct	\
	tstrASN1RsaPrivateKey

@brief	RSA Private Key Elements

	The structure contains the elements of the RSA Private key as decoded from
	the supplied private key file (PEM Format).
*/
typedef struct{
	tstrRsaPrivateKey	strRsaPrivKey;
	uint8				*pu8AllocPtr;
	uint16				u16KeyBufSz;
}tstrASN1RSAPrivateKey;


/*!
@struct	\
	tstrRSAPubKey
*/
typedef struct{
	uint16	u16NSize;
	uint16	u16ESize;
	uint8	*pu8N;
	uint8	*pu8E;
}tstrRSAPubKey;


/*!
@struct	\
	tstrECDSAPubKey
*/
typedef struct{
	uint16	u16CurveID;
	uint16	u16EcPointSz;
	uint8	au8EcPoint[ECC_POINT_MAX * 2];
}tstrECDSAPubKey;


/*!
@struct	\
	tstrX509CertPublicKey
*/
typedef struct{
	tenuCertPubKeyType	enuCertKeyType;
	union{
		tstrRSAPubKey	strRsaPub;
		tstrECDSAPubKey	strEcdsaPub;
	};
}tstrX509CertPublicKey;


/*!
@struct	\
	tstrX520Name

@brief
*/
typedef struct{
	char	acCmnName[64];
	uint8	au8NameSHA1[20];
}tstrX509Name;


/*!
@struct	\
	txtrX509CertInfo
*/
typedef struct{
	uint8					u8SerialNumberLength;
	uint8					au8SerialNo[64];
	tstrX509Name			strIssuer;
	tstrSystemTime			strExpiryDate;
	tstrSystemTime			strStartDate;
	tstrX509Name			strSubject;
	tstrX509CertPublicKey	strPubKey;
	void					*pvPrivate;
}txtrX509CertInfo;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

sint8 CryptoRSASignVerify(uint8 *pu8RsaModulusN, uint16 u16RsaModulusLen, uint8 *pu8RsaExpE, uint16 u16RsaExpLen, 
						  uint8	*pu8Signature, uint16 u16SignLen, uint8	*pu8SignedMsgHash, uint16 u16HashLength);

sint8 CryptoRSASignGen(uint8 *pu8RsaModulusN, uint16 u16RsaModulusLen, uint8 *pu8RsaExpd, uint16 u16RsaExpLen, 
						  uint8	*pu8Signature, uint8 *pu8SignedMsgHash, uint16 u16HashLength);

sint8 CryptoX509CertDecode(uint8 *pu8X509CertBuf, uint16 u16X509CertBuffer, txtrX509CertInfo *pstrCert, uint8 bDumpCert);
void CryptoX509DeleteContext(txtrX509CertInfo *pstrCert);
sint8 CryptoDecodeRsaPrivKey(uint8 *pu8RsaKeyFile, uint32 u32KeySize, tstrASN1RSAPrivateKey *pstrRsaPrivKey);

void CryptoSha1Hash(uint8 *pu8Data, uint32 u32DataSize, uint8 *pu8Digest);

void CryptoSha256Hash(uint8 *pu8Data, uint32 u32DataSize, uint8 *pu8Digest);


void CryptoAESCbcEnc(uint8 *pu8IV, uint8 *pu8Data, uint32 u32DataSz, uint8 *pu8Key, uint8 u8KeySz);

void CryptoAESCbcDec(uint8 *pu8IV, uint8 *pu8Data, uint32 u32DataSz, uint8 *pu8Key, uint8 u8KeySz);

void CryptoAESCTREnc(uint8 *pu8IV, uint8 *pu8Data, uint32 u32DataSz, uint8 *pu8Key, uint8 u8KeySz);
void CryptoAESCTRDec(uint8 *pu8IV, uint8 *pu8Data, uint32 u32DataSz, uint8 *pu8Key, uint8 u8KeySz);

#endif