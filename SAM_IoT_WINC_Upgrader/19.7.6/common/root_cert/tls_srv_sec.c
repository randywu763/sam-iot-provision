/**
 *
 * \file
 *
 * \brief This module implements TLS Server Certificate Installation.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "crypto_lib_api.h"
#include "pem.h"
#include "tls_srv_sec.h"
#include <stdlib.h>

#define K_DUMP(name,Buffer,size)                        \
do                                                      \
{                                                       \
    int k;                                              \
    uint8    *buf = Buffer;                             \
    printf("%s",name);    \
    for (k = 0; k < size; k++)                          \
    {                                                   \
        if (!(k % 16))                                  \
            printf("\r\n\t");                           \
        printf("%02X ", buf[k]);                        \
    }                                                   \
    printf("\r\n");                                     \
}while(0)

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

typedef struct __tstrTlsSrvSecReadEntry{
    struct __tstrTlsSrvSecReadEntry *pstrNext;
    uint8                           au8FileName[TLS_SRV_SEC_FILE_NAME_MAX];
    uint8                           *pu8FileContent;
    uint8                           *pu8CmnName;
    uint32                          u32FileSz;
    uint8                           bIsCert;
    union{
        tstrRsaPrivateKey           strRSAPrivKey;
        txtrX509CertInfo            strX509;
    };
}tstrTlsSrvSecReadEntry;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
GLOBALS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

static tstrTlsSrvSecReadEntry *gpstrRsaChain;
static tstrTlsSrvSecReadEntry *gpstrECDSAChain;
static uint8                  gpau8HexTable[]    = "0123456789abcdef";
static tstrTlsSrvSecHdr       *gpstrTlsSrvSecHdr = NULL;

/**************************************************************/
static void Hex2String(uint8 * pu8HexArray, uint32 u32Length, char * pcHexString)
{
    uint8 u8Temp;
    uint32 u32Idx;

    for(u32Idx = 0; u32Idx < u32Length; u32Idx ++)
    {
        u8Temp = (pu8HexArray[u32Idx] & 0xF0) >> 4;
        pcHexString[2*u32Idx] = gpau8HexTable[u8Temp];
        u8Temp = pu8HexArray[u32Idx] & 0x0F;
        pcHexString[2 * u32Idx+ 1] = gpau8HexTable[u8Temp];
    }
    pcHexString[2 * u32Length] = '\0';
}

/**************************************************************/
static tstrTlsSrvSecFileHandle* TlsSrvSecFopen(char *pcFileName, tenuTlsSrvSecFileMode enuMode)
{
    tstrTlsSrvSecFileHandle *pstrHandle = NULL;
    tstrTlsSrvSecFileEntry  *pstrEntry  = NULL;

    if(enuMode == FILE_MODE_WRITE)
    {
        uint32    u32Idx;
        for(u32Idx = 0; u32Idx < gpstrTlsSrvSecHdr->u32nEntries; u32Idx ++)
        {
            pstrEntry = &gpstrTlsSrvSecHdr->astrEntries[u32Idx];
            if(!memcmp(pstrEntry->acFileName, pcFileName, strlen(pcFileName)))
            {
                goto __END;
            }
        }

        if(gpstrTlsSrvSecHdr->u32nEntries < TLS_SRV_SEC_MAX_FILES)
        {
            pstrEntry = &gpstrTlsSrvSecHdr->astrEntries[gpstrTlsSrvSecHdr->u32nEntries ++];
            memcpy((uint8*)pstrEntry->acFileName, (uint8*)pcFileName, TLS_SRV_SEC_FILE_NAME_MAX);
            pstrEntry->u32FileAddr = gpstrTlsSrvSecHdr->u32NextWriteAddr;
        }
    }
    else if(enuMode == FILE_MODE_READ)
    {
        uint32    u32FileIdx;

        for(u32FileIdx = 0; u32FileIdx < gpstrTlsSrvSecHdr->u32nEntries; u32FileIdx ++)
        {
            if(!m2m_strncmp((uint8*)pcFileName, (uint8*)gpstrTlsSrvSecHdr->astrEntries[u32FileIdx].acFileName,
                (uint16)M2M_MAX(strlen(pcFileName), strlen(gpstrTlsSrvSecHdr->astrEntries[u32FileIdx].acFileName))))
            {
                pstrEntry = &gpstrTlsSrvSecHdr->astrEntries[u32FileIdx];
                break;
            }
        }
    }

    if(pstrEntry != NULL)
    {
        pstrHandle = (tstrTlsSrvSecFileHandle*)malloc(sizeof(tstrTlsSrvSecFileHandle));
        if(pstrHandle != NULL)
        {
            pstrHandle->u32Offset = 0;
            pstrHandle->pstrFile  = pstrEntry;
        }
    }

__END:
    return pstrHandle;
}

/**************************************************************/
static sint8 TlsSrvSecFwrite(uint8 *pu8WriteBuff, uint16 u16BuffSize, tstrTlsSrvSecFileHandle *pstrHandle)
{
    sint8    ret = M2M_ERR_FAIL;
    if((pu8WriteBuff != NULL) && (pstrHandle != NULL))
    {
        uint32 u32WriteOffset = (pstrHandle->pstrFile->u32FileAddr - M2M_TLS_SERVER_FLASH_OFFSET) + pstrHandle->u32Offset;
        uint8  *pu8TlsSrvSec = (uint8*)gpstrTlsSrvSecHdr;

        memcpy(&pu8TlsSrvSec[u32WriteOffset], pu8WriteBuff, u16BuffSize);
        pstrHandle->u32Offset += u16BuffSize;
        ret = M2M_SUCCESS;
    }
    return ret;
}

/**************************************************************/
static sint8 TlsSrvSecFread(uint8 *pu8ReadBuff, uint32 u32BuffSize, tstrTlsSrvSecFileHandle *pstrHandle)
{
    sint8 ret = M2M_ERR_FAIL;
    if((pu8ReadBuff != NULL) && (pstrHandle != NULL))
    {
        uint32 u32ReadOffset = (pstrHandle->pstrFile->u32FileAddr - M2M_TLS_SERVER_FLASH_OFFSET) + pstrHandle->u32Offset;
        uint8  *pu8TlsSrvSec = (uint8*)gpstrTlsSrvSecHdr;

        memcpy(pu8ReadBuff, &pu8TlsSrvSec[u32ReadOffset], u32BuffSize);
        pstrHandle->u32Offset += u32BuffSize;
        ret = M2M_SUCCESS;
    }
    return ret;
}

/**************************************************************/
static void TlsSrvSecFclose(tstrTlsSrvSecFileHandle *pstrHandle)
{
    if(pstrHandle != NULL)
    {
        pstrHandle->pstrFile->u32FileSize = (uint16)pstrHandle->u32Offset;
        gpstrTlsSrvSecHdr->u32NextWriteAddr += WORD_ALIGN(pstrHandle->u32Offset);
        free(pstrHandle);
    }
}

/**************************************************************/
static sint8 ReadServerX509Chain(tstrBuff *pstrX509Chain, uint8 u8nCerts, tstrX509Entry **ppstrChainHead)
{
    sint8               ret = M2M_SUCCESS;
    uint8               u8Idx;
    txtrX509CertInfo    strX509;
    tstrX509Entry       *pstrHead = NULL, *pstrTail = NULL;
    tstrX509Entry       *pstrTarget, *pstrCurr, *pstrPrev;
    uint8               bIsIssuerFound;

    for(u8Idx = 0; u8Idx < u8nCerts; u8Idx ++)
    {
        if(CryptoX509CertDecode(pstrX509Chain[u8Idx].pu8Data, pstrX509Chain[u8Idx].u16BufferSize, &strX509, 0) == M2M_SUCCESS)
        {
            tstrX509Entry *pstrNewX509 = (tstrX509Entry*)malloc(sizeof(tstrX509Entry));
            if(pstrNewX509 != NULL)
            {
                memcpy((uint8*)&pstrNewX509->strX509, (uint8*)&strX509, sizeof(txtrX509CertInfo));
                memcpy((uint8*)&pstrNewX509->strX509ASN1Buff, (uint8*)&pstrX509Chain[u8Idx], sizeof(tstrBuff));
                pstrNewX509->pstrNext = NULL;

                if(pstrHead == NULL)
                {
                    pstrHead = pstrNewX509;
                    pstrTail = pstrNewX509;
                }
                else
                {
                    pstrTail->pstrNext = pstrNewX509;
                    pstrTail           = pstrNewX509;
                }
            }
        }
        else
        {
        }
    }

    /* Sort the list.
    */
    for(pstrTarget = pstrHead; pstrTarget != NULL; pstrTarget = pstrTarget->pstrNext)
    {
        if(pstrTarget->pstrNext != NULL)
        {
            pstrCurr = pstrTarget->pstrNext;
            if(m2m_memcmp(pstrTarget->strX509.strIssuer.au8NameSHA1, pstrCurr->strX509.strSubject.au8NameSHA1, CRYPTO_SHA1_DIGEST_SIZE))
            {
                pstrPrev       = pstrTarget;
                pstrCurr       = pstrCurr->pstrNext;
                bIsIssuerFound = 0;

                for(; pstrCurr != NULL; pstrCurr = pstrCurr->pstrNext)
                {
                    if(!m2m_memcmp(pstrTarget->strX509.strIssuer.au8NameSHA1, pstrCurr->strX509.strSubject.au8NameSHA1, CRYPTO_SHA1_DIGEST_SIZE))
                    {
                        if(pstrCurr == pstrTail)
                        {
                            pstrTail = pstrPrev;
                        }
                        pstrPrev->pstrNext   = pstrCurr->pstrNext;
                        pstrCurr->pstrNext   = pstrTarget->pstrNext;
                        pstrTarget->pstrNext = pstrCurr;

                        pstrPrev       = pstrCurr;
                        bIsIssuerFound = 1;
                        break;
                    }
                    pstrPrev = pstrCurr;
                }

                if(!bIsIssuerFound)
                {
                    pstrTail           = pstrTarget;
                    pstrTail->pstrNext = NULL;
                    break;
                }
            }
        }
    }

    /*
        Remove Unrelated certificates.
    */
    for(pstrCurr = pstrHead; pstrCurr != NULL; )
    {
        if(pstrCurr->pstrNext != NULL)
        {
            if(m2m_memcmp(pstrCurr->strX509.strIssuer.au8NameSHA1, pstrCurr->pstrNext->strX509.strSubject.au8NameSHA1, CRYPTO_SHA1_DIGEST_SIZE))
            {
                pstrTarget = pstrCurr->pstrNext;
                pstrCurr->pstrNext = pstrCurr->pstrNext->pstrNext;
                CryptoX509DeleteContext(&pstrTarget->strX509);
                free(pstrTarget);
            }
        }
        pstrCurr = pstrCurr->pstrNext;
    }

    *ppstrChainHead = pstrHead;
    return ret;
}

/**************************************************************/
static sint8 ValidateKeyMaterial(tstrRsaPrivateKey *pstrPrivKey, txtrX509CertInfo *pstrServerCert)
{
    sint8 ret = M2M_SUCCESS;

    /*
        Validate the private key matches the server certificate.
    */
    if(pstrPrivKey != NULL)
    {
        if(memcmp(pstrServerCert->strPubKey.strRsaPub.pu8N, pstrPrivKey->pu8N, pstrPrivKey->u16NSize))
        {
            M2M_ERR("Private key does not match the certificate\n");
            ret = M2M_ERR_FAIL;
        }
    }

    if(ret == M2M_SUCCESS)
    {

        /* Add a new chain to the chain tracking file.
        */

        /* Write the certificate individual files.
        */

        /* Write the private key file.
        */
    }
    return ret;
}

/**************************************************************/
static sint8 RsaBuildServerX509CertChain(tstrX509Entry *pstrHead, tstrASN1RSAPrivateKey *pstrRsaPrivKey)
{
    sint8 ret = M2M_SUCCESS;

    if(pstrHead != NULL)
    {
        if(ValidateKeyMaterial(&pstrRsaPrivKey->strRsaPrivKey, &pstrHead->strX509) == M2M_SUCCESS)
        {
            char                    acPrivFileName[TLS_SRV_SEC_FILE_NAME_MAX] = "PRIV_";
            tstrX509Entry           *pstrCurr = pstrHead;
            tstrTlsSrvSecFileHandle *pstrHandle;

            /* Write the private key file.
            */
            strcpy(&acPrivFileName[5], &pstrHead->acFileName[5]);
            pstrHandle = TlsSrvSecFopen(acPrivFileName, FILE_MODE_WRITE);
            if(pstrHandle != NULL)
            {
                TlsSrvSecFwrite(pstrRsaPrivKey->pu8AllocPtr, pstrRsaPrivKey->u16KeyBufSz, pstrHandle);
                TlsSrvSecFclose(pstrHandle);
            }

            /* Write the chain holder file.
            */
            pstrCurr = pstrHead;
            pstrHandle = TlsSrvSecFopen((char*)"RSA.lst", FILE_MODE_WRITE);
            if(pstrHandle != NULL)
            {
                TlsSrvSecFwrite((uint8*)acPrivFileName, TLS_SRV_SEC_FILE_NAME_MAX, pstrHandle);
                while(pstrCurr != NULL)
                {
                    TlsSrvSecFwrite((uint8*)pstrCurr->acFileName, TLS_SRV_SEC_FILE_NAME_MAX, pstrHandle);
                    pstrCurr = pstrCurr->pstrNext;
                }
                TlsSrvSecFclose(pstrHandle);
            }
        }
        else
        {
            M2M_ERR("Key material mismatch\n");
            ret = M2M_ERR_FAIL;
        }
    }
    else
    {
        ret = M2M_ERR_FAIL;
    }
    return ret;
}

/**************************************************************/
static sint8 EcdsaBuildServerX509CertChain(tstrX509Entry *pstrHead)
{
    sint8 ret = M2M_SUCCESS;

    if(pstrHead != NULL)
    {
        if(ValidateKeyMaterial(NULL, &pstrHead->strX509) == M2M_SUCCESS)
        {
            tstrX509Entry           *pstrCurr = pstrHead;
            tstrTlsSrvSecFileHandle *pstrHandle;

            /* Write the chain holder file.
            */
            pstrCurr = pstrHead;
            pstrHandle = TlsSrvSecFopen((char*)"ECDSA.lst", FILE_MODE_WRITE);
            if(pstrHandle != NULL)
            {
                while(pstrCurr != NULL)
                {
                    TlsSrvSecFwrite((uint8*)pstrCurr->acFileName, TLS_SRV_SEC_FILE_NAME_MAX, pstrHandle);
                    pstrCurr = pstrCurr->pstrNext;
                }
                TlsSrvSecFclose(pstrHandle);
            }
        }
        else
        {
            M2M_ERR("Key material mismatch\n");
            ret = M2M_ERR_FAIL;
        }
    }
    else
    {
        ret = M2M_ERR_FAIL;
    }
    return ret;
}

/**************************************************************/
static sint8 BuildServerX509CertChain(tenuCertPubKeyType enuChainType, tstrX509Entry *pstrHead, tstrASN1RSAPrivateKey *pstrRsaPrivKey)
{
    sint8 ret = M2M_SUCCESS;

    tstrX509Entry           *pstrCurr = pstrHead;
    tstrTlsSrvSecFileHandle *pstrHandle;
    char                    acCertFileName[TLS_SRV_SEC_FILE_NAME_MAX] = "CERT_";

    /* Write the server certificate chain files.
    */
    while(pstrCurr != NULL)
    {
        Hex2String(pstrCurr->strX509.au8SerialNo, pstrCurr->strX509.u8SerialNumberLength, &acCertFileName[5]);
        pstrHandle = TlsSrvSecFopen(acCertFileName, FILE_MODE_WRITE);
        if(pstrHandle != NULL)
        {
            TlsSrvSecFwrite(pstrCurr->strX509ASN1Buff.pu8Data, pstrCurr->strX509ASN1Buff.u16BufferSize, pstrHandle);
            TlsSrvSecFclose(pstrHandle);
        }
        memcpy((uint8*)pstrCurr->acFileName, (uint8*)acCertFileName, TLS_SRV_SEC_FILE_NAME_MAX);
        pstrCurr = pstrCurr->pstrNext;
    }

    if(enuChainType == X509_CERT_PUBKEY_RSA)
    {
        ret = RsaBuildServerX509CertChain(pstrHead, pstrRsaPrivKey);
    }
    else if(enuChainType == X509_CERT_PUBKEY_ECDSA)
    {
        ret = EcdsaBuildServerX509CertChain(pstrHead);
    }
    else
    {
        ret = M2M_ERR_FAIL;
    }
    return ret;
}

/**************************************************************/
static sint8 WriteTlsServerKeyMaterial(uint8 *pu8PrivKey, uint32 u32PrivKeySz, tstrFileInfo *pstrCertChain, uint8 u8nCerts, uint32 *pu32SecSz)
{
    sint8                 ret             = M2M_ERR_FAIL;
    uint8                 u8Idx;
    uint32                u32Tmp;
    tstrASN1RSAPrivateKey strRSAPrivKey;
    tstrX509Entry         *pstrServerCert = NULL;
    tstrBuff              astrCertBuffs[20];

    for(u8Idx = 0; u8Idx < u8nCerts; u8Idx ++)
    {
        if(DecodeX509Certificate(pstrCertChain[u8Idx].pu8FileData, pstrCertChain[u8Idx].u32FileSz, &astrCertBuffs[u8Idx].pu8Data, &u32Tmp) == M2M_SUCCESS)
        {
            astrCertBuffs[u8Idx].u16BufferSize = (uint16)u32Tmp;
        }
        else
        {
            ret = M2M_ERR_FAIL;
            goto __ERR;
        }
    }

    ret = ReadServerX509Chain(astrCertBuffs, u8nCerts, &pstrServerCert);
    if(ret == M2M_SUCCESS)
    {
        tenuCertPubKeyType enuCertType = pstrServerCert->strX509.strPubKey.enuCertKeyType;

        if(enuCertType == X509_CERT_PUBKEY_RSA)
        {
#ifdef __RSA_CERTIFICATE__
            memset(&strRSAPrivKey, 0, sizeof(tstrASN1RSAPrivateKey));
            ret = CryptoDecodeRsaPrivKey(pu8PrivKey, u32PrivKeySz, &strRSAPrivKey);
            if(ret != M2M_SUCCESS)
                goto __ERR;
#endif
        }
        else if(enuCertType == X509_CERT_PUBKEY_ECDSA)
        {
            ret = M2M_SUCCESS;
        }

        ret = BuildServerX509CertChain(enuCertType, pstrServerCert, &strRSAPrivKey);
        if(ret == M2M_SUCCESS)
        {
            if(pu32SecSz != NULL)
            {
                uint32 u32SecSz = sizeof(tstrTlsSrvSecHdr);
                for(u8Idx = 0; u8Idx < (uint8)gpstrTlsSrvSecHdr->u32nEntries; u8Idx ++)
                {
                    u32SecSz += WORD_ALIGN(gpstrTlsSrvSecHdr->astrEntries[u8Idx].u32FileSize);
                }
                *pu32SecSz = u32SecSz;
            }
        }
    }
__ERR:
    {
        tstrX509Entry *pstrCur = pstrServerCert;
        tstrX509Entry *pstrDel;

        while(pstrCur != NULL)
        {
            pstrDel = pstrCur;
            pstrCur = pstrCur->pstrNext;
            CryptoX509DeleteContext(&pstrDel->strX509);
            free(pstrDel);
        }
    }
    return ret;
}

/**************************************************************/
sint8 TlsSrvSecWriteCertChain(uint8 *pu8PrivKey, uint32 u32PrivKeySz, tstrFileInfo *pstrCertChain, uint8 u8ChainDepth, uint8 *pu8TlsSrvSecBuff, uint32 *pu32SecSz, tenuWriteMode enuMode)
{
    sint8 ret          = M2M_ERR_FAIL;
    uint8 au8Pattern[] = TLS_SRV_SEC_START_PATTERN;

    if((pu8TlsSrvSecBuff != NULL) && (pstrCertChain != NULL))
    {
        /*
            Initialize the write operation
        */
        if(enuMode == TLS_SRV_SEC_MODE_WRITE)
        {
            memset(pu8TlsSrvSecBuff, 0xFF, M2M_TLS_SERVER_FLASH_SIZE);
            gpstrTlsSrvSecHdr = (tstrTlsSrvSecHdr*)pu8TlsSrvSecBuff;
            memcpy(gpstrTlsSrvSecHdr->au8SecStartPattern, au8Pattern, TLS_SRV_SEC_START_PATTERN_LEN);
            gpstrTlsSrvSecHdr->u32nEntries      = 0;
            gpstrTlsSrvSecHdr->u32NextWriteAddr = sizeof(tstrTlsSrvSecHdr) + M2M_TLS_SERVER_FLASH_OFFSET;
        }
        else if(enuMode == TLS_SRV_SEC_MODE_APPEND)
        {
            if(!memcmp(pu8TlsSrvSecBuff, au8Pattern, TLS_SRV_SEC_START_PATTERN_LEN))
            {
                gpstrTlsSrvSecHdr = (tstrTlsSrvSecHdr*)pu8TlsSrvSecBuff;
            }
            else
            {
                goto __ERR;
            }
        }
        else
        {
                goto __ERR;
        }

        /*
            Perform Writing
        */
        ret = WriteTlsServerKeyMaterial(pu8PrivKey, u32PrivKeySz, pstrCertChain, u8ChainDepth, pu32SecSz);
    }
__ERR:
    return ret;
}

/**************************************************************/
static sint8 WriteBufferToFile(uint8 *pu8Buff, uint32 u32WriteSz, char *pcFwFile)
{
    sint8 s8Ret = M2M_ERR_FAIL;
#ifdef WIN32
    FILE    *fp;

    fp = fopen(pcFwFile, "wb");
    if(fp)
    {
        fwrite(pu8Buff, 1, u32WriteSz, fp);
        fclose(fp);
        s8Ret = M2M_SUCCESS;
    }
    else
    {
        printf("(ERR)Cannot Open File <%s> For Writing\n", pcFwFile);
    }
#endif
    return s8Ret;
}

/**************************************************************/
static sint8 TlsSrvSecAddReadEntry(uint8 *pu8FileName, tstrTlsSrvSecReadEntry **ppstrEntry)
{
    sint8                   ret           = M2M_ERR_FAIL;
    tstrTlsSrvSecFileHandle *pstrHdl      = NULL;
    tstrTlsSrvSecReadEntry  *pstrNewEntry = NULL;

    if(pu8FileName != NULL)
    {
        pstrHdl = TlsSrvSecFopen((char*)pu8FileName, FILE_MODE_READ);
        if(pstrHdl != NULL)
        {
            pstrNewEntry = (tstrTlsSrvSecReadEntry*)malloc(sizeof(tstrTlsSrvSecReadEntry));
            if(pstrNewEntry != NULL)
            {
                pstrNewEntry->u32FileSz      = pstrHdl->pstrFile->u32FileSize;
                pstrNewEntry->pu8FileContent = (uint8*)malloc(pstrNewEntry->u32FileSz);
                if(pstrNewEntry->pu8FileContent != NULL)
                {
                    TlsSrvSecFread(pstrNewEntry->pu8FileContent, pstrHdl->pstrFile->u32FileSize, pstrHdl);
                    ret = M2M_SUCCESS;
                }
                else
                {
                    free(pstrNewEntry);
                }
            }
            TlsSrvSecFclose(pstrHdl);
        }

        if(ret == M2M_SUCCESS)
        {
            memcpy(pstrNewEntry->au8FileName, pu8FileName, TLS_SRV_SEC_FILE_NAME_MAX);
            if(!memcmp(pu8FileName, "CERT", 4))
            {
                CryptoX509CertDecode(pstrNewEntry->pu8FileContent, (uint16)pstrNewEntry->u32FileSz, &pstrNewEntry->strX509, 0);
                pstrNewEntry->bIsCert    = 1;
                pstrNewEntry->pu8CmnName = pstrNewEntry->strX509.strSubject.acCmnName;
            }
            else
            {
                uint8             *pu8PRIV = pstrNewEntry->pu8FileContent;
                tstrRsaPrivateKey *pstrKey = &pstrNewEntry->strRSAPrivKey;

                memcpy(pstrKey, pu8PRIV, sizeof(tstrRsaPrivateKey));
                pu8PRIV                  += sizeof(tstrRsaPrivateKey);

                pstrKey->pu8N            = pu8PRIV;
                pu8PRIV                  += WORD_ALIGN(pstrKey->u16NSize);

                pstrKey->pu8e            = pu8PRIV;
                pu8PRIV                  += WORD_ALIGN(pstrKey->u16eSize);

                pstrKey->pu8d            = pu8PRIV;
                pu8PRIV                  += WORD_ALIGN(pstrKey->u16dSize);

                pstrKey->pu8p            = pu8PRIV;
                pu8PRIV                  += WORD_ALIGN(pstrKey->u16PSize);

                pstrKey->pu8q            = pu8PRIV;
                pu8PRIV                  += WORD_ALIGN(pstrKey->u16QSize);

                pstrKey->pu8dP           = pu8PRIV;
                pu8PRIV                  += WORD_ALIGN(pstrKey->u16dPSize);

                pstrKey->pu8dQ           = pu8PRIV;
                pu8PRIV                  += WORD_ALIGN(pstrKey->u16dQSize);

                pstrKey->pu8QInv         = pu8PRIV;
                pu8PRIV                  += WORD_ALIGN(pstrKey->u16QInvSize);

                pstrNewEntry->bIsCert    = 0;
                pstrNewEntry->pu8CmnName = NULL;
            }
            *ppstrEntry = pstrNewEntry;
        }
    }
    return ret;
}

/**************************************************************/
static tstrTlsSrvSecReadEntry* TlsLoadCertChain(char *pcTlsChainFile)
{
    tstrTlsSrvSecFileHandle    *pstrHdl;
    tstrTlsSrvSecReadEntry *pstrNew  = NULL;
    tstrTlsSrvSecReadEntry *pstrHead = NULL;
    tstrTlsSrvSecReadEntry *pstrTail = NULL;
    uint8                  au8Fname[TLS_SRV_SEC_FILE_NAME_MAX];

    pstrHdl = TlsSrvSecFopen(pcTlsChainFile, FILE_MODE_READ);
    if(pstrHdl != NULL)
    {
        while(pstrHdl->u32Offset < pstrHdl->pstrFile->u32FileSize)
        {
            TlsSrvSecFread(au8Fname, TLS_SRV_SEC_FILE_NAME_MAX, pstrHdl);
            if(TlsSrvSecAddReadEntry(au8Fname, &pstrNew) != M2M_SUCCESS)
            {
                break;
            }

            /*
                INSERTION.
            */
            pstrNew->pstrNext = NULL;
            if(pstrHead == NULL)
            {
                pstrHead = pstrNew;
                pstrTail = pstrNew;
            }
            else
            {
                pstrTail->pstrNext = pstrNew;
                pstrTail = pstrNew;
            }
        }
        TlsSrvSecFclose(pstrHdl);
    }
    return pstrHead;
}

/**************************************************************/
static sint8 TlsSrvDumpChain(tstrTlsSrvSecReadEntry *pstrChain, uint8 bPrintPrivKey, uint8 bWriteToFile, uint8 bDumpWholeChain, char *pcKeyWord, char *pcOutPath)
{
    sint8 ret = M2M_ERR_FAIL;
    char  acFileName[324];

    if(pstrChain != NULL)
    {
        tstrTlsSrvSecReadEntry *pstrCur       = pstrChain;
        uint8                  bWaitingEuCert = 1;

        ret = M2M_SUCCESS;
        while(pstrCur != NULL)
        {
            if(pstrCur->bIsCert)
            {
                if(bWriteToFile)
                {
                    strcpy(acFileName, pcOutPath);
                    if((acFileName[strlen(pcOutPath)] != '/') && (acFileName[strlen(pcOutPath)] != '\\'))
                    {
                        strcat(acFileName, "/");
                    }

                    if(bWaitingEuCert)
                    {
                        bWaitingEuCert = 0;
                        strcat(acFileName, pcKeyWord);
                    }

                    if(pstrCur->pu8CmnName[0] == '*')
                    {
                        strcat(acFileName, (char*)&pstrCur->pu8CmnName[2]);
                    }
                    else
                    {
                        strcat(acFileName, (char*)pstrCur->pu8CmnName);
                    }
                    strcat(acFileName, ".cer");

                    if(WriteBufferToFile(pstrCur->pu8FileContent, pstrCur->u32FileSz, acFileName) != M2M_SUCCESS)
                    {
                        ret = M2M_ERR_FAIL;
                        break;
                    }
                }
                else
                {
                    if(CryptoX509CertDecode(pstrCur->pu8FileContent, (uint16)pstrCur->u32FileSz, &pstrCur->strX509, 1) != M2M_SUCCESS)
                    {
                        ret = M2M_ERR_FAIL;
                        break;
                    }
                    M2M_PRINT("\n\n");
                }

                if(!bDumpWholeChain)
                {
                    break;
                }
            }
            else
            {
                if(bPrintPrivKey)
                {
                    tstrRsaPrivateKey *pstrKey = &pstrCur->strRSAPrivKey;
                    M2M_PRINT("Private-Key: (%u bit)\n", pstrKey->u16NSize * 8);
                    K_DUMP("modulus (N):", pstrKey->pu8N, pstrKey->u16NSize);
                    K_DUMP("publicExponent (e):", pstrKey->pu8e, pstrKey->u16eSize);
                    K_DUMP("privateExponent (d):", pstrKey->pu8d, pstrKey->u16dSize);
                    K_DUMP("prim1 (p):", pstrKey->pu8p, pstrKey->u16PSize);
                    K_DUMP("prime2 (q):", pstrKey->pu8q, pstrKey->u16QSize);
                    K_DUMP("exponent1 (dP):", pstrKey->pu8dP, pstrKey->u16dPSize);
                    K_DUMP("exponent2 (dQ):", pstrKey->pu8dQ, pstrKey->u16dQSize);
                    K_DUMP("coefficient(QInv):", pstrKey->pu8QInv, pstrKey->u16QInvSize);
                    M2M_PRINT("\n\n");
                }
            }
            pstrCur = pstrCur->pstrNext;
        }
    }
    return ret;
}

/**************************************************************/
sint8 TlsSrvSecReadInit(uint8 *pu8TlsSrvSec)
{
    sint8 ret = M2M_ERR_FAIL;

    if(pu8TlsSrvSec != NULL)
    {
        uint8 au8Pattern[] = TLS_SRV_SEC_START_PATTERN;

        gpstrTlsSrvSecHdr = (tstrTlsSrvSecHdr*)pu8TlsSrvSec;
        if(!memcmp(pu8TlsSrvSec, au8Pattern, TLS_SRV_SEC_START_PATTERN_LEN))
        {
            gpstrRsaChain   = TlsLoadCertChain((char*)TLS_SRV_RSA_CHAIN_FILE);
            gpstrECDSAChain = TlsLoadCertChain((char*)TLS_SRV_ECDSA_CHAIN_FILE);
            ret = M2M_SUCCESS;
        }
    }
    return ret;
}

/**************************************************************/
sint8 TlsSrvSecReadDeinit(void)
{
    sint8 ret = M2M_ERR_FAIL;

    if(gpstrTlsSrvSecHdr != NULL)
    {
        uint8 au8Pattern[] = TLS_SRV_SEC_START_PATTERN;

        if(!memcmp((uint8*)gpstrTlsSrvSecHdr, au8Pattern, TLS_SRV_SEC_START_PATTERN_LEN))
        {
            tstrTlsSrvSecReadEntry *pstrCur, *pstrDel;
            if(gpstrRsaChain != NULL)
            {
                pstrCur = gpstrRsaChain;
                while(pstrCur != NULL)
                {
                    pstrDel = pstrCur;
                    pstrCur = pstrDel->pstrNext;
                    free(pstrDel->pu8FileContent);
                    free(pstrDel);
                }
            }

            if(gpstrECDSAChain != NULL)
            {
                pstrCur = gpstrECDSAChain;
                while(pstrCur != NULL)
                {
                    pstrDel = pstrCur;
                    pstrCur = pstrDel->pstrNext;
                    free(pstrDel->pu8FileContent);
                    free(pstrDel);
                }
            }

            ret = M2M_SUCCESS;
        }
    }
    return ret;
}

/**************************************************************/
sint8 TlsSrvSecDumpContents(uint8 bDumpRsa, uint8 bDumpEcdsa, uint8 bPrintPrivKey, uint8 bDumpWholeChain, uint8 bListFiles, uint8 bWriteToFile, char *pcOutPath)
{
    sint8 ret          = M2M_ERR_FAIL;
    uint8 au8Pattern[] = TLS_SRV_SEC_START_PATTERN;

    if(gpstrTlsSrvSecHdr != NULL)
    {
        tstrTlsSrvSecReadEntry    *pstrCur;

        if(!memcmp(gpstrTlsSrvSecHdr->au8SecStartPattern, au8Pattern, TLS_SRV_SEC_START_PATTERN_LEN))
        {
            /*
                DIR Command
            */
            if(bListFiles)
            {
                if((gpstrRsaChain != NULL) && (bDumpRsa))
                {
                    M2M_PRINT("<RSA CERTIFICATE CHAIN FILE LIST>\n\n");
                    pstrCur = gpstrRsaChain;
                    while(pstrCur != NULL)
                    {
                        printf("%-*s %*lu %-12s %s\n",
                            TLS_SRV_SEC_FILE_NAME_MAX,
                            pstrCur->au8FileName, 4, pstrCur->u32FileSz,
                            pstrCur->bIsCert ? "CERTIFICATE" : "PRIVATE KEY",
                            ((pstrCur->pu8CmnName != NULL) ? (char*)pstrCur->pu8CmnName : (char*)" "));
                        pstrCur = pstrCur->pstrNext;
                    }
                    M2M_PRINT("\n\n");
                }

                if((gpstrECDSAChain != NULL) && (bDumpEcdsa))
                {
                    M2M_PRINT("<ECDSA CERTIFICATE CHAIN FILE LIST>\n\n");
                    pstrCur = gpstrECDSAChain;
                    while(pstrCur != NULL)
                    {
                        M2M_PRINT("%-*s %*lu %-12s %s\n",
                            TLS_SRV_SEC_FILE_NAME_MAX,
                            pstrCur->au8FileName, 4, pstrCur->u32FileSz,
                            pstrCur->bIsCert ? "CERTIFICATE" : "PRIVATE KEY",
                            (pstrCur->pu8CmnName != NULL) ? (char*)pstrCur->pu8CmnName : (char*)" ");
                        pstrCur = pstrCur->pstrNext;
                    }
                    M2M_PRINT("\n\n");
                }
            }

            if(bDumpRsa)
            {
                ret = TlsSrvDumpChain(gpstrRsaChain, bPrintPrivKey, bWriteToFile, bDumpWholeChain, (char*)"RSA_", pcOutPath);
            }

            if(bDumpEcdsa)
            {
                ret = TlsSrvDumpChain(gpstrECDSAChain, 0, bWriteToFile, bDumpWholeChain, (char*)"ECDSA_", pcOutPath);
            }
        }
    }

    return ret;
}
