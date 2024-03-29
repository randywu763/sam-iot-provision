/*!
@file \
    root_setup.c

@brief  ROOT CERT DOWNLOADER

    This is the main file for the root_certificate_downloader tool which installs
    digital X.509 root certificates on WINC1500 for proper operation of TLS.

*/


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "crypto_lib_api.h"
#include "root_setup.h"
#include "programmer.h"
#include "pem.h"

//#define ENABLE_VERIFICATION

#define WORD_ALIGN(val)                 (((val) & 0x03) ? ((val) + 4 - ((val) & 0x03)) : (val))

#define ROOT_CERT_FLASH_START_PATTERN_LENGTH        16


#define ROOT_CERT_FLASH_EMPTY_PATTERN \
{\
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  \
}

/* tstrRootCertEntryHeader Format used in 19.4.x */
#define ROOT_CERT_FLASH_START_PATTERN_V0    \
{\
    0x01, 0xF1, 0x02, 0xF2, 0x03, 0xF3, 0x04, 0xF4, \
    0x05, 0xF5, 0x06, 0xF6, 0x07, 0xF7, 0x08, 0xF8  \
}

/* tstrRootCertEntryHeader Format used in 19.5.x */
#define ROOT_CERT_FLASH_START_PATTERN   \
{\
    0x11, 0xF1, 0x12, 0xF2, 0x13, 0xF3, 0x14, 0xF4, \
    0x15, 0xF5, 0x16, 0xF6, 0x17, 0xF7, 0x18, 0xF8  \
}
/*!< A Pattern is stored at the start of the root certificate flash area
    in order to identify if the flash is written before or not and the
    format of the content.
*/


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


typedef struct{
    uint8  au8StartPattern[ROOT_CERT_FLASH_START_PATTERN_LENGTH];
    uint32 u32nCerts;
}tstrRootCertFlashHeader;


typedef enum{
    ROOT_CERT_PUBKEY_RSA        = 1,
    ROOT_CERT_PUBKEY_ECDSA      = 2
}tenuRootCertPubKeyType;


typedef struct{
    uint16 u16NSz;
    uint16 u16ESz;
}tstrRootCertRsaKeyInfo;


typedef struct{
    uint16 u16CurveID;
    uint16 u16KeySz;
}tstrRootCertEcdsaKeyInfo;

typedef struct{
    uint32 u32PubKeyType;
    union{
        tstrRootCertRsaKeyInfo   strRsaKeyInfo;
        tstrRootCertEcdsaKeyInfo strEcsdaKeyInfo;
    };
}tstrRootCertPubKeyInfo;


/*!
@struct
    tstrRootCertEntryHeader

@brief
    Header of a root certificate entry in flash.
*/
typedef struct{
    uint8                   au8SHA1NameHash[CRYPTO_SHA1_DIGEST_SIZE];
    tstrSystemTime          strStartDate;
    tstrSystemTime          strExpDate;
    tstrRootCertPubKeyInfo  strPubKey;
}tstrRootCertEntryHeader;


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
GLOBALS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

uint8 gau8RootCertMem[M2M_TLS_ROOTCER_FLASH_SIZE * 2];

#ifdef ENABLE_VERIFICATION
static uint8 gau8Verify[M2M_TLS_ROOTCER_FLASH_SIZE];
#endif


/************************************************/
static int GetRootCertificate(uint8 *pu8RootCert, uint32 u32RootCertSz, txtrX509CertInfo *pstrX509)
{
    int        ret;
    uint32    u32FileSize;
    uint8    *pcRootCertDER;

    /* Decode the certificate.
    */
    ret = DecodeX509Certificate(pu8RootCert, u32RootCertSz, &pcRootCertDER, &u32FileSize);
    if(ret == M2M_SUCCESS)
    {
        ret = CryptoX509CertDecode(pcRootCertDER, (uint16)u32FileSize, pstrX509, 0);
        if(ret == M2M_SUCCESS)
        {
            printf("\r\n>>>Found Certificate:\n");
            printf(">>>\t%s\n", pstrX509->strSubject.acCmnName);
        }
    }
    else
    {
        printf("\r\n>>>Invalid certificate!\n");
    }

    return ret;
}

/************************************************/
static uint16 writeRootCertEntry(uint8 *pu8WriteBuff, txtrX509CertInfo *pstrRootCert)
{
    uint16 u16WriteSz = 0;

    if((pu8WriteBuff != NULL) && (pstrRootCert != NULL))
    {
        tstrRootCertEntryHeader *pstrEntryHdr = (tstrRootCertEntryHeader*)pu8WriteBuff;
        uint8                   *pu8KeyMem    = &pu8WriteBuff[sizeof(tstrRootCertEntryHeader)];

        u16WriteSz = sizeof(tstrRootCertEntryHeader);

        m2m_memset((uint8*)pstrEntryHdr, 0, sizeof(tstrRootCertEntryHeader));

        /*
            Write Root Certificate Entry Header
        */
        m2m_memcpy(pstrEntryHdr->au8SHA1NameHash, pstrRootCert->strSubject.au8NameSHA1, CRYPTO_SHA1_DIGEST_SIZE);       // Subject Name SHA1
        m2m_memcpy((uint8*)&pstrEntryHdr->strStartDate, (uint8*)&pstrRootCert->strStartDate, sizeof(tstrSystemTime));   // Cert. Start Date.
        m2m_memcpy((uint8*)&pstrEntryHdr->strExpDate, (uint8*)&pstrRootCert->strExpiryDate, sizeof(tstrSystemTime));    // Cert. Expiration Date.

        /*
            Write the certificate public key
        */
        if(pstrRootCert->strPubKey.enuCertKeyType == X509_CERT_PUBKEY_RSA)
        {
            /*
                RSA Public Key
            */
            tstrRSAPubKey *pstrKey = &pstrRootCert->strPubKey.strRsaPub;

            pstrEntryHdr->strPubKey.u32PubKeyType        = ROOT_CERT_PUBKEY_RSA;
            pstrEntryHdr->strPubKey.strRsaKeyInfo.u16NSz = pstrKey->u16NSize;
            pstrEntryHdr->strPubKey.strRsaKeyInfo.u16ESz = pstrKey->u16ESize;

            /* N */
            m2m_memcpy(pu8KeyMem, pstrKey->pu8N, pstrKey->u16NSize);
            pu8KeyMem += pstrKey->u16NSize;

            /* E */
            m2m_memcpy(pu8KeyMem, pstrKey->pu8E, pstrKey->u16ESize);
            u16WriteSz += WORD_ALIGN(pstrKey->u16ESize) + WORD_ALIGN(pstrKey->u16NSize);
        }
        else if(pstrRootCert->strPubKey.enuCertKeyType == X509_CERT_PUBKEY_ECDSA)
        {
            tstrECDSAPubKey *pstrKey = &pstrRootCert->strPubKey.strEcdsaPub;

            pstrEntryHdr->strPubKey.u32PubKeyType              = ROOT_CERT_PUBKEY_ECDSA;
            pstrEntryHdr->strPubKey.strEcsdaKeyInfo.u16CurveID = pstrKey->u16CurveID;
            pstrEntryHdr->strPubKey.strEcsdaKeyInfo.u16KeySz   = pstrKey->u16EcPointSz;
            m2m_memcpy(pu8KeyMem, pstrKey->au8EcPoint, pstrKey->u16EcPointSz * 2);
            u16WriteSz += pstrKey->u16EcPointSz * 2;
        }
    }
    return u16WriteSz;
}

/************************************************/
static sint8 UpdateRootList(txtrX509CertInfo *pstrRootCert)
{
    uint32                  u32Idx;
    uint8                   bIncrement        = 0;
    uint32                  u32nStoredCerts   = 0;
    uint8                   au8StartPattern[] = ROOT_CERT_FLASH_START_PATTERN;
    uint8                   au8EmptyPattern[] = ROOT_CERT_FLASH_EMPTY_PATTERN;
    tstrRootCertFlashHeader *pstrRootFlashHdr;
    tstrRootCertEntryHeader *pstrEntryHdr;
    uint16                  u16Offset;
    uint16                  u16WriteSize;
    tstrRootCertPubKeyInfo  *pstrKey;

    pstrRootFlashHdr = (tstrRootCertFlashHeader*)((void *)gau8RootCertMem);
    u16Offset        = sizeof(tstrRootCertFlashHeader);

    /* Check if the flash has been written before.
    */
    if(m2m_memcmp(au8EmptyPattern, pstrRootFlashHdr->au8StartPattern, ROOT_CERT_FLASH_START_PATTERN_LENGTH) != 0)
    {
        u32nStoredCerts = pstrRootFlashHdr->u32nCerts;
        bIncrement = 1;

        for(u32Idx = 0 ; u32Idx < u32nStoredCerts ; u32Idx ++)
        {
            pstrEntryHdr = (tstrRootCertEntryHeader*)((void *)&gau8RootCertMem[u16Offset]);
            pstrKey      = &pstrEntryHdr->strPubKey;

            /* Check for match (equivalent NameSHA1).
            */
            if(!m2m_memcmp(pstrRootCert->strSubject.au8NameSHA1, pstrEntryHdr->au8SHA1NameHash, 20))
            {
                /* The current entry will be overwritten.
                */
                bIncrement = 0;
                break;
            }
            u16Offset += sizeof(tstrRootCertEntryHeader);
            u16Offset += (pstrKey->u32PubKeyType == ROOT_CERT_PUBKEY_RSA) ?
                (WORD_ALIGN(pstrKey->strRsaKeyInfo.u16NSz) + WORD_ALIGN(pstrKey->strRsaKeyInfo.u16ESz)) : (WORD_ALIGN(pstrKey->strEcsdaKeyInfo.u16KeySz) * 2);
        }
    }
    else
    {
        pstrRootFlashHdr->u32nCerts = 0;
        m2m_memcpy(pstrRootFlashHdr->au8StartPattern, au8StartPattern, ROOT_CERT_FLASH_START_PATTERN_LENGTH);
        bIncrement = 1;
    }

    u16WriteSize = writeRootCertEntry(&gau8RootCertMem[u16Offset], pstrRootCert);

    if(bIncrement)
    {
        /* A new certificate is to be inserted into the flash.
        Increment the number of stored Certificates.
        */
        if(u16Offset + u16WriteSize > M2M_TLS_ROOTCER_FLASH_SIZE)
        {
            printf("(ERROR) Root Certificate Flash is Full\n");
            return -1;
        }
        pstrRootFlashHdr->u32nCerts ++;
    }
    return 0;
}

/************************************************/
int WriteRootCertificate(uint8 *pu8RootCert, uint32 u32RootCertSz, uint8* vflash)
{
    txtrX509CertInfo strX509Root;
    int              ret = -1;

    /* Read Certificate File.
    */
    if(GetRootCertificate(pu8RootCert, u32RootCertSz, &strX509Root) == 0)
    {
        programmer_read_root_cert(gau8RootCertMem);

        if(UpdateRootList(&strX509Root) == 0)
        {
            /* Erase memory.
            */
            ret = programmer_erase_root_cert(vflash);
            if(M2M_SUCCESS != ret) goto END;
            /* Write.
            */
            printf(">Writing the certificate to SPI flash...\r\n");
            ret = programmer_write_root_cert(gau8RootCertMem, vflash);
            if(M2M_SUCCESS != ret) goto END;
            printf("Done\r\n");
            //nm_bsp_sleep(50);

#ifdef ENABLE_VERIFICATION //Enable verification or print array

            {
                uint32            u32Idx;

                memset(gau8Verify, 0, M2M_TLS_ROOTCER_FLASH_SIZE);
                programmer_read_cert_image(gau8Verify);
                //nm_bsp_sleep(32);

                for(u32Idx = 0; u32Idx < M2M_TLS_ROOTCER_FLASH_SIZE; u32Idx ++)
                {
                    if(gau8RootCertMem[u32Idx] != gau8Verify[u32Idx])
                    {
                        printf("ERROR verification failed at %u\n", u32Idx);
                        ret = -1;
                        break;
                    }
                }
            }

#endif //Enable verification or print array
        }
        CryptoX509DeleteContext(&strX509Root);
    }
END:
    return ret;
}
