/*!
@file
*/

#ifndef __TLS_SRV_SEC_H__
#define __TLS_SRV_SEC_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "crypto_lib_api.h"
#include "programmer.h"
#include "pem.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define TLS_SRV_SEC_MAX_FILES           8
#define TLS_SRV_SEC_START_PATTERN_LEN   8
#define TLS_SRV_SEC_START_PATTERN       {0xAB, 0xFE, 0x18, 0x5B, 0x70, 0xC3, 0x46, 0x92}
#define TLS_SRV_SEC_FILE_NAME_MAX       (48)

#define TLS_SRV_ECDSA_CHAIN_FILE        "ECDSA.lst"
#define TLS_SRV_RSA_CHAIN_FILE          "RSA.lst"

#ifdef WIN32
#define __RSA_CERTIFICATE__
#endif

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

typedef enum{
    TLS_SRV_SEC_MODE_WRITE = 0,
    TLS_SRV_SEC_MODE_APPEND
}tenuWriteMode;


typedef struct{
    uint16    u16BufferSize;
    uint8    *pu8Data;
}tstrBuff;


typedef struct certEntry{
    txtrX509CertInfo    strX509;
    tstrBuff            strX509ASN1Buff;
    char                acFileName[60];
    struct certEntry    *pstrNext;
}tstrX509Entry;


typedef enum{
    FILE_MODE_READ,
    FILE_MODE_WRITE
}tenuTlsSrvSecFileMode;


typedef struct{
    tstrTlsSrvSecFileEntry        *pstrFile;
    uint32                        u32Offset;
}tstrTlsSrvSecFileHandle;


typedef struct{
    uint32    u32FileSz;
    uint8    *pu8FileData;
}tstrFileInfo;



/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

sint8 TlsSrvSecWriteCertChain(uint8 *pu8PrivKey, uint32 u32PrivKeySz, tstrFileInfo *pstrCertChain, uint8 u8ChainDepth, uint8 *pu8TlsSrvSecBuff, uint32 *pu32SecSz, tenuWriteMode enuMode);
sint8 TlsSrvSecReadInit(uint8 *pu8TlsSrvSec);
sint8 TlsSrvSecReadDeinit(void);
sint8 TlsSrvSecDumpContents(uint8 bDumpRsa, uint8 bDumpEcdsa, uint8 bPrintPrivKey, uint8 bDumpWholeChain, uint8 bListFiles, uint8 bWriteToFile, char *pcOutPath);

#endif
