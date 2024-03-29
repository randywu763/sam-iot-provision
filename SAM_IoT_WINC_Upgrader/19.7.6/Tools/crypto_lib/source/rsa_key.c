
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "nmi_m2m.h"
#include "x509_cert.h"
#include "pem.h"
#include "asn1.h"
#include "crypto_lib_api.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


/*!
@brief	
	A state machine for parsing the RSA Private key. It is ordered with
    the same order of the RSA key elements occurrence in the ASN.1 form.
*/
enum RSAKeyParsing{
	RSA_N,
	RSA_E,
	RSA_D,
	RSA_P,
	RSA_Q,
	RSA_DP,
	RSA_DQ,
	RSA_QINV,
	RSA_DONE
};



/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
GLOBALS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


/**************************************************************/
static sint8 ParseRsaPrivKey(uint8 *pu8PrivKey, uint16 u16KeySize, tstrASN1RSAPrivateKey *pstrAsn1Key)
{
	sint8				ret = M2M_ERR_FAIL;
	tstrAsn1Context		strASN1Cxt;
	tstrAsn1Element		strElement;
	tstrTlsBuffer		strTlsBuffer;
	tstrRsaPrivateKey	*pstrKey = &pstrAsn1Key->strRsaPrivKey;

	strTlsBuffer.pu8Buff		= pu8PrivKey;
	strTlsBuffer.u16BuffSz		= u16KeySize;
	strTlsBuffer.u16ReadOffset	= 0;

	/* Initialize the certificate structure. 
	*/
	memset((uint8*)&strASN1Cxt, 0, sizeof(tstrAsn1Context));

	/* Initialize the ASN1 Decoding operation. 
	*/
	strASN1Cxt.pstrTlsBuffer	= &strTlsBuffer;
			
	ASN1_GetNextElement(&strASN1Cxt, &strElement);
	if(strElement.u8Tag == ASN1_SEQUENCE)
	{
		uint16		u16TotalSize = 0;
		uint8		u8State = RSA_N;
		uint8		*pu8ElemVal;
		uint8		*pu8KeyBuff;
		uint8		u8Tmp;
		uint8		u8Offset;

		/* Version
		*/
		ASN1_GetNextElement(&strASN1Cxt, &strElement);			
		ASN1_Read(&strASN1Cxt, strElement.u32Length, (uint8*)&pstrKey->u32Version);

		while(u8State < RSA_DONE)
		{
			ASN1_GetNextElement(&strASN1Cxt, &strElement);
			if(u8State == RSA_N)
			{
				pstrAsn1Key->u16KeyBufSz	= (uint16)(sizeof(tstrRsaPrivateKey) + (WORD_ALIGN(strElement.u32Length) * 5));
				pstrAsn1Key->pu8AllocPtr	= (uint8*)malloc(pstrAsn1Key->u16KeyBufSz);
				if(pstrAsn1Key->pu8AllocPtr == NULL)
				{
					M2M_ERR("Out Of Memory\n");
					break;
				}
				pu8KeyBuff = pstrAsn1Key->pu8AllocPtr + sizeof(tstrRsaPrivateKey);
				u16TotalSize = sizeof(tstrRsaPrivateKey);
			}
			pu8ElemVal = pu8KeyBuff;

			ASN1_Read(&strASN1Cxt, 1, &u8Tmp);
			if(u8Tmp == 0)
			{
				u8Offset = 0;
				strElement.u32Length --;
			}
			else
			{
				u8Offset		= 1;
				pu8ElemVal[0]	= u8Tmp;
			}

			ASN1_Read(&strASN1Cxt, (strElement.u32Length - u8Offset), &pu8ElemVal[u8Offset]);
			pu8KeyBuff += WORD_ALIGN(strElement.u32Length);
			u16TotalSize += (uint16)WORD_ALIGN(strElement.u32Length);
			switch(u8State)
			{
			case RSA_N:
				pstrKey->pu8N		= pu8ElemVal;
				pstrKey->u16NSize	= (uint16)strElement.u32Length;
				break;

			case RSA_E:
				pstrKey->pu8e		= pu8ElemVal;
				pstrKey->u16eSize	= (uint16)strElement.u32Length;
				break;

			case RSA_D:
				pstrKey->pu8d		= pu8ElemVal;
				pstrKey->u16dSize	= (uint16)strElement.u32Length;
				break;

			case RSA_P:
				pstrKey->pu8p		= pu8ElemVal;
				pstrKey->u16PSize	= (uint16)strElement.u32Length;
				break;

			case RSA_Q:
				pstrKey->pu8q		= pu8ElemVal;
				pstrKey->u16QSize	= (uint16)strElement.u32Length;
				break;

			case RSA_DP:
				pstrKey->pu8dP		= pu8ElemVal;
				pstrKey->u16dPSize	= (uint16)strElement.u32Length;
				break;

			case RSA_DQ:
				pstrKey->pu8dQ		= pu8ElemVal;
				pstrKey->u16dQSize	= (uint16)strElement.u32Length;
				break;

			case RSA_QINV:
				pstrKey->pu8QInv		= pu8ElemVal;
				pstrKey->u16QInvSize	= (uint16)strElement.u32Length;
				break;
			}
			u8State ++;
		}
		if(u8State == RSA_DONE)
		{
			pstrAsn1Key->u16KeyBufSz = u16TotalSize;
			memcpy(pstrAsn1Key->pu8AllocPtr, (uint8*)pstrKey, sizeof(tstrRsaPrivateKey));
			ret = M2M_SUCCESS;
		}
		else
		{
			ret = M2M_ERR_FAIL;
		}
	}
	return ret;
}

/**************************************************************/
static sint8 ParsePrivKey(uint8 *pu8PrivKey, uint16 u16KeySize, tstrASN1RSAPrivateKey *pstrKey)
{
	sint8			ret = M2M_ERR_FAIL;
	tstrAsn1Context	strASN1Cxt;
	tstrAsn1Element	strElement;
	tstrTlsBuffer	strTlsBuffer;

	strTlsBuffer.pu8Buff		= pu8PrivKey;
	strTlsBuffer.u16BuffSz		= u16KeySize;
	strTlsBuffer.u16ReadOffset	= 0;

	/* Initialize the certificate structure. 
	*/
	memset((uint8*)&strASN1Cxt, 0, sizeof(tstrAsn1Context));

	/* Initialize the ASN1 Decoding operation. 
	*/
	strASN1Cxt.pstrTlsBuffer	= &strTlsBuffer;

/*
	PrivateKeyInfo ::= SEQUENCE {
		version         Version,
		algorithm       AlgorithmIdentifier,
		PrivateKey      BIT STRING
	}
*/
	ASN1_GetNextElement(&strASN1Cxt, &strElement);
	if(strElement.u8Tag == ASN1_SEQUENCE)
	{
		/* Version
		*/
		ASN1_GetNextElement(&strASN1Cxt, &strElement);			
		ASN1_Read(&strASN1Cxt, strElement.u32Length, NULL);
		
		/*
            Private Key Algorithm ID.
		*/
		ASN1_GetNextElement(&strASN1Cxt, &strElement); // SEQUENCE
		if(strElement.u8Tag == ASN1_SEQUENCE)
		{
			ASN1_GetNextElement(&strASN1Cxt, &strElement); // OID
			if(strElement.u8Tag == ASN1_OBJECT_IDENTIFIER)
			{
				uint8	rsaOID[] = {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01};
				uint8	OID[32];

				ASN1_Read(&strASN1Cxt, strElement.u32Length, OID);
				if(!memcmp(OID, rsaOID, strElement.u32Length))
				{
					ASN1_GetNextElement(&strASN1Cxt, &strElement);
					ret = ParseRsaPrivKey(&strTlsBuffer.pu8Buff[strTlsBuffer.u16ReadOffset], (uint16)strElement.u32Length, pstrKey);
				}
			}
		}
	}
	return ret;
}

/**************************************************************/
sint8 CryptoDecodeRsaPrivKey(uint8 *pu8RsaKeyFile, uint32 u32KeySize, tstrASN1RSAPrivateKey *pstrRsaPrivKey)
{
	sint8	ret = M2M_ERR_FAIL;

	if((pu8RsaKeyFile != NULL) && (pstrRsaPrivKey != NULL))
	{
		uint8			*pu8PrivKey = NULL;
		tenuPEMEncType	enuPemType;

		if(DecodeBase64File(pu8RsaKeyFile, u32KeySize, &pu8PrivKey, &u32KeySize, &enuPemType) == M2M_SUCCESS)
		{
			if(enuPemType == PEM_PRIV_KEY)
			{
				ret = ParsePrivKey(pu8PrivKey, (uint16)u32KeySize, pstrRsaPrivKey);
			}
			else if(enuPemType == PEM_RSA_PRIV_KEY)
			{
				ret = ParseRsaPrivKey(pu8PrivKey, (uint16)u32KeySize, pstrRsaPrivKey);
			}
		}
		/* The pu8PrivKey will contain a pointer to space that was allocated by Base64_Decode. It needs to be freed here */
		if (pu8PrivKey != NULL)
			free(pu8PrivKey);
	}
	return ret;
}
