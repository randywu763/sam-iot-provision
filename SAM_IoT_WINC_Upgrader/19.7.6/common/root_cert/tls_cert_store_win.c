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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "driver\include\m2m_types.h"
#include "crypto_lib_api.h"
#include "programmer.h"
#include "tls_srv_sec.h"


/**************************************************************/
int ReadFileToBuffer(char *pcFileName, uint8 **ppu8FileData, uint32 *pu32FileSize)
{
    FILE *fp;
    int  ret = M2M_ERR_FAIL;

    fp = fopen(pcFileName, "rb");
    if(fp)
    {
        uint32    u32FileSize;
        uint8    *pu8Buf;

        fseek(fp, 0, SEEK_END);
        u32FileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        pu8Buf = (uint8*)malloc(u32FileSize);
        if(pu8Buf != NULL)
        {
            fread(pu8Buf, 1, u32FileSize, fp);
            *pu32FileSize = u32FileSize;
            *ppu8FileData = pu8Buf;
            ret = M2M_SUCCESS;
        }
        fclose(fp);
    }
    return ret;
}

/**************************************************************/
void ListDirectoryContents(const char *pcDir, char *pcExt, char ***ppacFileList, uint32 *pu32ListSize)
{
    WIN32_FIND_DATA fdFile;
    HANDLE          hFind                    = NULL;
    uint32          u32ListSize              = 0;
    char            **pacFileList            = NULL;
    uint32          u32CertNamesListMaxSz    = 0;
    char            acPath[2048];

    sprintf(acPath, "%s\\*.%s", pcDir, pcExt);
    if((hFind = FindFirstFile(acPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        *pu32ListSize = u32ListSize;
        return;
    }

    do
    {
        if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0)
        {
            sprintf(acPath, "%s\\%s", pcDir, fdFile.cFileName);

            {
                size_t u32PathLen;

                if(u32CertNamesListMaxSz <= u32ListSize)
                {
                    if(u32CertNamesListMaxSz > 0 && u32CertNamesListMaxSz < (1ul << 31))
                    {
                        u32CertNamesListMaxSz <<= 1;
                    }
                    else
                    {
                        u32CertNamesListMaxSz = 1;
                    }
                    pacFileList = (char**)realloc(pacFileList, sizeof(char*)*u32CertNamesListMaxSz);
                    if(!pacFileList)
                    {
                        goto _EXIT;
                    }
                }
                u32PathLen = strlen(acPath);
                pacFileList[u32ListSize] = (char*)malloc(u32PathLen+1);
                if(!pacFileList[u32ListSize]) goto _EXIT;
                strncpy(pacFileList[u32ListSize], acPath, u32PathLen+1);
                u32ListSize ++;
            }
        }
    }
    while(FindNextFile(hFind, &fdFile));
    *pu32ListSize = u32ListSize;
    *ppacFileList = pacFileList;

_EXIT:
    FindClose(hFind);
    return;
}

/**************************************************************/
sint8 TlsCertStoreWriteCertChain(char *pcPrivKeyFile, char *pcSrvCertFile, char *pcCADirPath, uint8 *pu8TlsSrvSecBuff, uint32 *pu32SecSz, tenuWriteMode enuMode)
{
    sint8           ret                 = M2M_ERR_FAIL;
    uint32          u32Idx;
    uint8           u8nCerts            = 0;
    uint32          u32nCaCerts         = 0;
    uint8           *pu8PrivKey         = NULL;
    uint32          u32PrivKeySz        = 0;
    char            **ppCertNames;
    tstrFileInfo    astrCertList[10];

    if(ReadFileToBuffer(pcSrvCertFile, &astrCertList[0].pu8FileData, &astrCertList[0].u32FileSz) == M2M_SUCCESS)
    {
        u8nCerts = 1;

        if(pcPrivKeyFile != NULL)
        {
            ReadFileToBuffer(pcPrivKeyFile, &pu8PrivKey, &u32PrivKeySz);
        }

        /*
            Get all CA certificates from the given CA Path (if any).
        */
        if(pcCADirPath != NULL)
        {
            ListDirectoryContents(pcCADirPath, "cer", &ppCertNames, &u32nCaCerts);
            if(u32nCaCerts > 5)
            {
                printf("CA Directory contains too much certificates\n");
                free(ppCertNames);
                return M2M_ERR_FAIL;
            }

            for(u32Idx = 0; u32Idx < u32nCaCerts; u32Idx ++)
            {
                ReadFileToBuffer(ppCertNames[u32Idx], &astrCertList[u32Idx + 1].pu8FileData, &astrCertList[u32Idx + 1].u32FileSz);
            }
            u8nCerts += (uint8)u32nCaCerts;
        }

        /*
            Perform Writing
        */
        ret = TlsSrvSecWriteCertChain(pu8PrivKey, u32PrivKeySz, astrCertList, u8nCerts, pu8TlsSrvSecBuff, pu32SecSz, enuMode);
    }
    return ret;
}
