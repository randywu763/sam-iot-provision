#include "crypto.h"
#include <string.h>
#include <stdlib.h>
#include "x509_cert.h"
#include "crypto_lib_api.h"

typedef struct __tstrCryptoMemHdr{
	struct __tstrCryptoMemHdr	*pstrNext;
}tstrCryptoMemHdr;

/**************************************************************/
void* CryptoInternalMalloc(void *hdl, uint32 u32AllocSz)
{
	txtrX509CertInfo	*pstrCert	= (txtrX509CertInfo*)hdl;
	tstrCryptoMemHdr	*pstrNew;
	uint8				*pu8Alloc;

	pu8Alloc = (uint8*)malloc(WORD_ALIGN(u32AllocSz) + sizeof(tstrCryptoMemHdr));
	if(pu8Alloc != NULL)
	{
		pstrNew = (tstrCryptoMemHdr*)pu8Alloc;

		if(pstrCert->pvPrivate == NULL)
		{
			pstrNew->pstrNext	= NULL;
		}
		else
		{
			pstrNew->pstrNext	= (tstrCryptoMemHdr*)pstrCert->pvPrivate;
		}
		pstrCert->pvPrivate	= (void*)pstrNew;

		pu8Alloc += sizeof(tstrCryptoMemHdr);
	}
	return (uint8*)pu8Alloc;
}

/**************************************************************/
sint8 CryptoRSASignVerify(uint8 *pu8RsaModulusN, uint16 u16RsaModulusLen, uint8 *pu8RsaExpE, uint16 u16RsaExpLen, 
						  uint8	*pu8Signature, uint16 u16SignLen, uint8	*pu8SignedMsgHash, uint16 u16HashLength)
{
	tstrRSAPublicKey	strRsaPub;

	strRsaPub.pu8N		= pu8RsaModulusN;
	strRsaPub.pu8E		= pu8RsaExpE;
	strRsaPub.u16NSize	= u16RsaModulusLen;
	strRsaPub.u16ESize	= u16RsaExpLen;

	return RSA_SignVerify(&strRsaPub, pu8Signature, u16SignLen, pu8SignedMsgHash, u16HashLength);
}

/**************************************************************/
sint8 CryptoRSASignGen(uint8 *pu8RsaModulusN, uint16 u16RsaModulusLen, uint8 *pu8RsaExpd, uint16 u16RsaExpLen, 
						  uint8	*pu8Signature, uint8 *pu8SignedMsgHash, uint16 u16HashLength)
{
	tstrRSAPublicKey	strRsaPub;

	strRsaPub.pu8N		= pu8RsaModulusN;
	strRsaPub.pu8E		= pu8RsaExpd;
	strRsaPub.u16NSize	= u16RsaModulusLen;
	strRsaPub.u16ESize	= u16RsaExpLen;

	return RSA_SignGen(&strRsaPub, pu8Signature, pu8SignedMsgHash, u16HashLength);
}

/**************************************************************/
void CryptoSha1Hash(uint8 *pu8Data, uint32 u32DataSize, uint8 *pu8Digest)
{
	tstrSha256Context	sha;

	SHA1_Hash(&sha, SHA_FLAGS_FULL_HASH, pu8Data, u32DataSize, pu8Digest);
}

/**************************************************************/
void CryptoSha256Hash(uint8 *pu8Data, uint32 u32DataSize, uint8 *pu8Digest)
{
	tstrSha256Context	sha256;

	SHA256_Hash(&sha256, SHA_FLAGS_FULL_HASH, pu8Data, u32DataSize, pu8Digest);
}

/**************************************************************/
sint8 CryptoX509CertDecode(uint8 *pu8X509CertBuf, uint16 u16X509CertBuffer, txtrX509CertInfo *pstrCert, uint8 bDumpCert)
{
	sint8			ret			= M2M_ERR_FAIL;
	tstrMemPool		strPool;
	tstrX509Cert	strX509Cer;
	tstrTlsBuffer	strX509Buff;

	strX509Buff.pu8Buff			= pu8X509CertBuf;
	strX509Buff.u16BuffSz		= u16X509CertBuffer;
	strX509Buff.u16ReadOffset	= 0;

	strPool.fpAlloc			= CryptoInternalMalloc;
	strPool.pvPoolHandle	= pstrCert;
	pstrCert->pvPrivate		= NULL;
	if(X509Cert_Decode(&strX509Buff, u16X509CertBuffer, &strPool, &strX509Cer , bDumpCert) == X509_SUCCESS)
	{
		pstrCert->u8SerialNumberLength	= strX509Cer.u8SerialNumberLength;
#pragma warning(suppress: 6386)
		memcpy(pstrCert->au8SerialNo, strX509Cer.au8SerialNo, min(sizeof(pstrCert->au8SerialNo), sizeof(strX509Cer.au8SerialNo)));
		memcpy(&pstrCert->strExpiryDate, &strX509Cer.strExpiryDate, sizeof(tstrSystemTime));
		memcpy(&pstrCert->strStartDate, &strX509Cer.strStartDate, sizeof(tstrSystemTime));
		memcpy(&pstrCert->strIssuer, &strX509Cer.strIssuer, sizeof(tstrX520Name));
		memcpy(&pstrCert->strSubject, &strX509Cer.strSubject, sizeof(tstrX520Name));
		if(strX509Cer.strPubKey.enuType == PUBKEY_ALG_RSA)
		{
			pstrCert->strPubKey.enuCertKeyType = X509_CERT_PUBKEY_RSA;
			memcpy(&pstrCert->strPubKey.strRsaPub, &strX509Cer.strPubKey.strRSAKey, sizeof(tstrRSAPubKey));
		}
		else if(strX509Cer.strPubKey.enuType == PUBKEY_ALG_ECC)
		{
			pstrCert->strPubKey.enuCertKeyType				= X509_CERT_PUBKEY_ECDSA;
			pstrCert->strPubKey.strEcdsaPub.u16CurveID		= strX509Cer.strPubKey.strEccKey.pstrCurve->enuType;
			pstrCert->strPubKey.strEcdsaPub.u16EcPointSz	= strX509Cer.strPubKey.strEccKey.strQ.u16Size;
			memcpy(&pstrCert->strPubKey.strEcdsaPub.au8EcPoint, &strX509Cer.strPubKey.strEccKey.strQ.X, pstrCert->strPubKey.strEcdsaPub.u16EcPointSz * 2);
		}
		ret = M2M_SUCCESS;
	}
	return ret;
}

/**************************************************************/
void CryptoX509DeleteContext(txtrX509CertInfo *pstrCert)
{
	tstrCryptoMemHdr	*pstrCur = (tstrCryptoMemHdr*)pstrCert->pvPrivate;
	tstrCryptoMemHdr	*pstrDel;

	while(pstrCur != NULL)
	{
		pstrDel = pstrCur;
		pstrCur = pstrCur->pstrNext;
		free(pstrDel);
	}
}