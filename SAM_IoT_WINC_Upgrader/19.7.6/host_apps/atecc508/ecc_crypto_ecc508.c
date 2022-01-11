/**
 *
 * \file
 *
 * \brief BSD compatible socket interface.
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

#include "ecc_types.h"
#include "driver/include/m2m_ssl.h"
#include "ecc_crypto_ecc508.h"
#include "cryptoauthlib.h"
#include "tls/atcatls.h"
#include "basic/atca_basic.h"
#include "cert_def_1_signer.h"
#include "cert_def_2_device.h"
#include "atcacert/atcacert_client.h"
#include "crc32.h"  // add crc32 in ASF to project

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

//#define __DBG__

#ifdef __DBG__
#define ECC_DBG								M2M_INFO
#define M2M_DUMP_BUF(name,Buffer,size)		\
do											\
{											\
	int k;									\
	uint8	*buf = Buffer;					\
	printf("%s(%u)",name, size);			\
	for (k = 0; k < size; k++)				\
	{										\
		if (!(k % 8))						\
		printf("\r\n\t");					\
		printf("0x%02X, ", buf[k]);			\
	}										\
	printf("\r\n");							\
}while(0)
#else
#define M2M_DUMP_BUF(name,Buffer,size)
#define ECC_DBG(...)
#endif


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
GLOBALS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

uint16 gau16EcdhKeyIds[] = {2}; //!< Array of private key slots to rotate through for ECDH calculations
uint32 gu32EcdhKeyIdx = 0; //!< Index into gau16EcdhKeyIds for the next key ID to use for ECDH
static ATCAIfaceCfg g_tls_crypto;

static const char* bin2hex(const void* data, size_t data_size)
{
	static char buf[256];
	static char hex[] = "0123456789abcdef";
	const uint8_t* data8 = data;
	
	if (data_size*2 > sizeof(buf)-1)
	return "[buf too small]";
	
	for (size_t i = 0; i < data_size; i++)
	{
		buf[i*2 + 0] = hex[(*data8) >> 4];
		buf[i*2 + 1] = hex[(*data8) & 0xF];
		data8++;
	}
	buf[data_size*2] = 0;
	
	return buf;
}

static int eccPrintInfo(void)
{
	ATCA_STATUS atca_status;
	uint8 sn[9];
	uint8 dev_rev[4];

	atca_status = atcab_read_serial_number(sn);
	if (atca_status)
	{
		printf("atcab_read_serial_number() failed with ret=0x%08X\r\n", atca_status);
		return atca_status;
	}
	
	atca_status = atcab_info(dev_rev);
	if (atca_status)
	{
		printf("atcab_info() failed with ret=0x%08X\r\n", atca_status);
		return atca_status;
	}

	printf("Crypto Device:\r\n");
	printf("  SN:          %s\r\n", bin2hex(sn, sizeof(sn)));
	printf("  DevRev:      %s\r\n", bin2hex(dev_rev, sizeof(dev_rev)));

	return 0;
}

int eccReadAWSData(aws_data_t* aws_data)
{
	ATCA_STATUS status;

	status = atcab_read_bytes_zone(ATCA_ZONE_DATA, 8, 0, (uint8_t*)aws_data, 224);
	if (status != ATCA_SUCCESS)
	return status;

	status = atcab_read_serial_number(aws_data->client_id);
	if (status != ATCA_SUCCESS)
	return status;

	return status;
}

static size_t winc_certs_get_total_files_size(const tstrTlsSrvSecHdr* header)
{
	uint8 *pBuffer = (uint8*) header;
	uint16 count = 0;

	while ((*pBuffer) == 0xFF)
	{
		
		if(count == INIT_CERT_BUFFER_LEN)
			break;
		count++;
		pBuffer++;
	}

	if(count == INIT_CERT_BUFFER_LEN)
		return sizeof(*header); // Buffer is empty, no files
	
	return header->u32NextWriteAddr;
}

static size_t winc_certs_get_files_buf_size(const uint32* buffer32)
{
	return winc_certs_get_total_files_size((tstrTlsSrvSecHdr*)buffer32);
}


static sint8 winc_certs_append_file_buf(uint32* buffer32, uint32 buffer_size, const char* file_name, uint32 file_size, const uint8* file_data)
{
    tstrTlsSrvSecHdr* header = (tstrTlsSrvSecHdr*)buffer32;
    tstrTlsSrvSecFileEntry* file_entry = NULL;
    uint16 str_size = m2m_strlen((uint8*)file_name) + 1;
	uint16 count = 0;
	uint8 *pBuffer = (uint8*)buffer32;

	while ((*pBuffer) == 0xFF)
	{
		
		if(count == INIT_CERT_BUFFER_LEN)
			break;
		count++;
		pBuffer++;
	}

	if(count == INIT_CERT_BUFFER_LEN)
	{
		// The WINC will need to add the reference start pattern to the header
	    header->u32nEntries = 0; // No certs
		// The WINC will need to add the offset of the flash were the certificates are stored to this address
        header->u32NextWriteAddr = sizeof(*header); // Next cert will be written after the header
	}
    
    if (header->u32nEntries >= sizeof(header->astrEntries)/sizeof(header->astrEntries[0]))
        return M2M_ERR_FAIL; // Already at max number of files
    
    if ((header->u32NextWriteAddr + file_size) > buffer_size)
        return M2M_ERR_FAIL; // Not enough space in buffer for new file
        
    file_entry = &header->astrEntries[header->u32nEntries];
    header->u32nEntries++;
    
    if (str_size > sizeof(file_entry->acFileName))
        return M2M_ERR_FAIL; // File name too long
    m2m_memcpy((uint8*)file_entry->acFileName, (uint8*)file_name, str_size);
    
    file_entry->u32FileSize = file_size;
    file_entry->u32FileAddr = header->u32NextWriteAddr;
    header->u32NextWriteAddr += file_size;
    
    // Use memmove to accommodate optimizations where the file data is temporarily stored
    // in buffer32
    memmove(((uint8*)buffer32) + (file_entry->u32FileAddr), (uint8*)file_data, file_size);

	// update header CRC - just extend crc for last written cert, unless 1st.
	if(header->u32nEntries == 1)
		crc32_calculate((uint8*)buffer32 + header->astrEntries[header->u32nEntries-1].u32FileAddr, 
			header->astrEntries[header->u32nEntries-1].u32FileSize,	&header->u32CRC);
	else
		crc32_recalculate((uint8*)buffer32 + header->astrEntries[header->u32nEntries-1].u32FileAddr,
			header->astrEntries[header->u32nEntries-1].u32FileSize,	&header->u32CRC);

    return M2M_SUCCESS;
}


static sint8 eccSendCertsToWINC(void)
{
	sint8 s8Ret = M2M_SUCCESS;
	int atca_ret = ATCACERT_E_SUCCESS;
	uint8_t* signer_cert = NULL;
	size_t signer_cert_size;
	uint8_t signer_public_key[SIGNER_PUBLIC_KEY_MAX_LEN];
	uint8_t* device_cert = NULL;
	size_t device_cert_size;
	uint8_t cert_sn[CERT_SN_MAX_LEN];
	size_t cert_sn_size;
	uint8_t* file_list = NULL;
    char* device_cert_filename = NULL;
    char* signer_cert_filename = NULL;
	uint32 sector_buffer[MAX_TLS_CERT_LENGTH];
	
	// Clear cert chain buffer
	memset(sector_buffer, 0xFF, sizeof(sector_buffer));

    // Use the end of the sector buffer to temporarily hold the data to save RAM
    file_list   = ((uint8_t*)sector_buffer) + (sizeof(sector_buffer) - TLS_FILE_NAME_MAX*2);
    signer_cert = file_list - SIGNER_CERT_MAX_LEN;
    device_cert = signer_cert - DEVICE_CERT_MAX_LEN;

    // Init the file list
    memset(file_list, 0, TLS_FILE_NAME_MAX*2);
    device_cert_filename = (char*)&file_list[0];
    signer_cert_filename = (char*)&file_list[TLS_FILE_NAME_MAX];


	// Uncompress the signer certificate from the ATECC508A device
	signer_cert_size = SIGNER_CERT_MAX_LEN;
	atca_ret = atcacert_read_cert(&g_cert_def_1_signer, g_signer_1_ca_public_key, signer_cert, &signer_cert_size);
	if (atca_ret != ATCACERT_E_SUCCESS)
		goto ATERR;
	
	// Get the signer's public key from its certificate
	atca_ret = atcacert_get_subj_public_key(&g_cert_def_1_signer, signer_cert, signer_cert_size, signer_public_key);
	if (atca_ret != ATCACERT_E_SUCCESS)
		goto ATERR;
	
	// Uncompress the device certificate from the ATECC508A device.
	device_cert_size = DEVICE_CERT_MAX_LEN;
	atca_ret = atcacert_read_cert(&g_cert_def_2_device, signer_public_key, device_cert, &device_cert_size);
	if (atca_ret != ATCACERT_E_SUCCESS)
		goto ATERR;
	
	// Get the device certificate SN for the filename
	cert_sn_size = sizeof(cert_sn);
	atca_ret = atcacert_get_cert_sn(&g_cert_def_2_device, device_cert, device_cert_size, cert_sn, &cert_sn_size);
	if (atca_ret != ATCACERT_E_SUCCESS)
		goto ATERR;
	
	// Build the device certificate filename
	strcpy(device_cert_filename, "CERT_");
	strcat(device_cert_filename, bin2hex(cert_sn, cert_sn_size));
	
	// Add the DER device certificate the TLS certs buffer
	s8Ret = winc_certs_append_file_buf(sector_buffer, sizeof(sector_buffer), device_cert_filename, device_cert_size, device_cert);
	if (s8Ret != M2M_SUCCESS)
		return s8Ret;
	
	device_cert = NULL; // Make sure we don't use this now that it has moved
	
	// Get the signer certificate SN for the filename
	cert_sn_size = sizeof(cert_sn);
	atca_ret = atcacert_get_cert_sn(&g_cert_def_1_signer, signer_cert, signer_cert_size, cert_sn, &cert_sn_size);
	if (atca_ret != ATCACERT_E_SUCCESS)
		goto ATERR;
	
	// Build the signer certificate filename
	strcpy(signer_cert_filename, "CERT_");
	strcat(signer_cert_filename, bin2hex(cert_sn, cert_sn_size));
	
	// Add the DER signer certificate the TLS certs buffer
	s8Ret = winc_certs_append_file_buf(sector_buffer, sizeof(sector_buffer), signer_cert_filename, signer_cert_size, signer_cert);
	if (s8Ret != M2M_SUCCESS)
		return s8Ret;
	
	// Add the cert chain list file to the TLS certs buffer
	s8Ret = winc_certs_append_file_buf(sector_buffer, sizeof(sector_buffer), TLS_SRV_ECDSA_CHAIN_FILE, TLS_FILE_NAME_MAX*2, file_list);
	if (s8Ret != M2M_SUCCESS)
		return s8Ret;

    file_list = NULL;
    signer_cert_filename = NULL;
    device_cert_filename = NULL;
	
	// Update the TLS cert chain on the WINC.

	s8Ret = m2m_ssl_send_certs_to_winc((uint8 *)sector_buffer,(uint32) winc_certs_get_files_buf_size(sector_buffer));
	if (s8Ret != M2M_SUCCESS)
		return s8Ret;

ATERR:
	if (atca_ret)
	{
		M2M_ERR("eccSendCertsToWINC() failed with ret=%d", atca_ret);
		s8Ret =  M2M_ERR_FAIL;
	}
	return s8Ret;
}

sint8 eccInit(ATCAIfaceCfg* cryptoconf)
{
	sint8 s8Ret = M2M_SUCCESS;
	ATCA_STATUS atca_status;
	
	memcpy(&g_tls_crypto,cryptoconf,sizeof(ATCAIfaceCfg));
	atca_status = atcab_init(&g_tls_crypto);
	if (atca_status == ATCACERT_E_SUCCESS)
	{
		atca_status = eccPrintInfo();
	}

	if (atca_status != ATCACERT_E_SUCCESS)
	{
		M2M_ERR("eccInit() failed with ret=%d", atca_status);
		s8Ret =  M2M_ERR_FAIL;
	}
	return s8Ret;
}

sint8 eccTransferCertificates(void)
{
	sint8 s8Ret = M2M_ERR_FAIL;

	// Rebuild certificates from ATECC508A and save them to the WINC TLS files section
	s8Ret = eccSendCertsToWINC();
	if (s8Ret != M2M_SUCCESS)
	{
		M2M_ERR("eccSendCertsToWINC() failed with ret=%d", s8Ret);
	}
	return s8Ret;
}

/************************************************************/
static sint8 ecdhDeriveClientSharedSecret
(
tstrECPoint			*pstrServerPubKey, 
uint8				*pu8ECDHSharedSecret, 
tstrECPoint			*pstrClientPubKey
)
{
	sint8 s8Ret = M2M_ERR_FAIL;
	
    if (gu32EcdhKeyIdx >= sizeof(gau16EcdhKeyIds)/sizeof(gau16EcdhKeyIds[0]))
        gu32EcdhKeyIdx = 0;
    ECC_DBG("ECDH = %u\n", gau16EcdhKeyIds[gu32EcdhKeyIdx]);
    
	if(atcatls_create_key(gau16EcdhKeyIds[gu32EcdhKeyIdx], pstrClientPubKey->XY) == ATCA_SUCCESS)
	{
		pstrClientPubKey->u16Size = 32;
		if(atcab_ecdh(gau16EcdhKeyIds[gu32EcdhKeyIdx], pstrServerPubKey->XY, pu8ECDHSharedSecret) == ATCA_SUCCESS)
		{
			s8Ret = M2M_SUCCESS;
			gu32EcdhKeyIdx++;
		}
	}
	return s8Ret;
}

/************************************************************/
static sint8 ecdhDeriveKeyPair
(
tstrECPoint		*pstrSrvPubKey
)
{
	sint8 ret = M2M_ERR_FAIL;
	
	if (gu32EcdhKeyIdx >= sizeof(gau16EcdhKeyIds)/sizeof(gau16EcdhKeyIds[0]))
	    gu32EcdhKeyIdx = 0;
	ECC_DBG("DRV_SLOT = %u\n", gau16EcdhKeyIds[gu32EcdhKeyIdx]);

	if(atcatls_create_key(gau16EcdhKeyIds[gu32EcdhKeyIdx], pstrSrvPubKey->XY) == ATCA_SUCCESS)
	{
		pstrSrvPubKey->u16Size		= 32;
		pstrSrvPubKey->u16PrivKeyID = gau16EcdhKeyIds[gu32EcdhKeyIdx];

		gu32EcdhKeyIdx ++;

		ret = M2M_SUCCESS;
	}

	return ret;
}

/************************************************************/
static sint8 ecdhDeriveServerSharedSecret
(
uint16			u16PrivKeyID,
tstrECPoint		*pstrClientPubKey,
uint8			*pu8ECDHSharedSecret
)
{
	uint16	u16KeySlot	= u16PrivKeyID;
	sint8	s8Ret		= M2M_ERR_FAIL;
	uint8	u8AtcaRet;

	ECC_DBG("__SLOT = %u\n", u16KeySlot);
	u8AtcaRet = atcab_ecdh(u16KeySlot, pstrClientPubKey->XY, pu8ECDHSharedSecret);
	if(u8AtcaRet == ATCA_SUCCESS)
	{
		s8Ret = M2M_SUCCESS;
	}
	else
	{
		M2M_INFO("__SLOT = %u, Err = %X\n", u16KeySlot, u8AtcaRet);
	}
	return s8Ret;
}

/************************************************************/
static sint8 ecdsaProcessSignVerifREQ(uint32 u32NumSig)
{
	sint8		ret			= M2M_ERR_FAIL;
	tstrECPoint	Key;
	uint32		u32Idx;
	uint8		au8Sig[80];
	uint8		au8Hash[80] = {0};
	uint16		u16CurveType;
	
	for(u32Idx = 0; u32Idx < u32NumSig; u32Idx ++)
	{
		ret = m2m_ssl_retrieve_cert(&u16CurveType, au8Hash, au8Sig, &Key);

		if (ret != M2M_SUCCESS)
		{
			M2M_ERR("m2m_ssl_retrieve_cert() failed with ret=%d", ret);
			return ret;
		}

		if(u16CurveType == EC_SECP256R1)
		{
			bool	bIsVerified = false;
			
			ret = atcatls_verify(au8Hash, au8Sig, Key.XY, &bIsVerified);
			if(ret == ATCA_SUCCESS)
			{
				ret = (bIsVerified == true) ? M2M_SUCCESS : M2M_ERR_FAIL;
				if(bIsVerified == false)
				{
					M2M_INFO("ECDSA SigVerif FAILED\n");
				}
			}
			else
			{
				ECC_DBG("atcatls_verify (%X)\n", ret);
				ret = M2M_ERR_FAIL;
			}
			
			if(ret != M2M_SUCCESS)
			{
				m2m_ssl_stop_processing_certs();
				break;
			}
		}
		else
		{
				
		}
	}
	return ret;
}

/************************************************************/
static sint8 ecdsaProcessSignGenREQ(tstrEcdsaSignReqInfo *pstrSignREQ, uint8 *pu8Sig, uint16 *pu16SigSz)
{
	sint8		ret		= M2M_ERR_FAIL;
	uint8		au8Hash[32];
	
	ret = m2m_ssl_retrieve_hash(au8Hash, pstrSignREQ->u16HashSz);

	if (ret != M2M_SUCCESS)
	{
		M2M_ERR("m2m_ssl_retrieve_hash() failed with ret=%d", ret);
		return ret;
	}

	if(pstrSignREQ->u16CurveType == EC_SECP256R1)
	{
		*pu16SigSz = 64;
		ret = atcatls_sign(0, au8Hash, pu8Sig);
		{
			uint8	key[64];
			atcatls_calc_pubkey(0, key);
			M2M_DUMP_BUF("KEY", key, 64);
		}
		M2M_DUMP_BUF("H", au8Hash, 32);
		M2M_DUMP_BUF("SIG", pu8Sig, 64);
	}
	else
	{
			
	}
	return ret;
}

/************************************************************/
void eccProcessREQ(tstrEccReqInfo* strEccREQ)
{
	tstrEccReqInfo	strECCResp;
	uint8			au8Sig[80];
	uint16			u16RspDataSz	= 0;
	uint8			*pu8RspDataBuff	= NULL;
	
	strECCResp.u16Status	= 1;

	switch(strEccREQ->u16REQ)
	{
	case ECC_REQ_CLIENT_ECDH:
		strECCResp.u16Status = ecdhDeriveClientSharedSecret(&(strEccREQ->strEcdhREQ.strPubKey), strECCResp.strEcdhREQ.au8Key, &strECCResp.strEcdhREQ.strPubKey);
		break;

	case ECC_REQ_GEN_KEY:
		strECCResp.u16Status = ecdhDeriveKeyPair(&strECCResp.strEcdhREQ.strPubKey);
		break;

	case ECC_REQ_SERVER_ECDH:
		strECCResp.u16Status = ecdhDeriveServerSharedSecret(strEccREQ->strEcdhREQ.strPubKey.u16PrivKeyID,&(strEccREQ->strEcdhREQ.strPubKey), strECCResp.strEcdhREQ.au8Key);
		break;
			
	case ECC_REQ_SIGN_VERIFY:
		strECCResp.u16Status = ecdsaProcessSignVerifREQ(strEccREQ->strEcdsaVerifyREQ.u32nSig);
		break;
			
	case ECC_REQ_SIGN_GEN:
		strECCResp.u16Status = ecdsaProcessSignGenREQ(&(strEccREQ->strEcdsaSignREQ), au8Sig, &u16RspDataSz);
		pu8RspDataBuff = au8Sig;
		break;
	}

	strECCResp.u16REQ		= strEccREQ->u16REQ;
	strECCResp.u32UserData	= strEccREQ->u32UserData;
	strECCResp.u32SeqNo		= strEccREQ->u32SeqNo;

	m2m_ssl_ecc_process_done();
	m2m_ssl_handshake_rsp(&strECCResp, pu8RspDataBuff, u16RspDataSz);
}

#if 0
/************************************************************/
static void eccSigTest(void)
{
	uint8		key[64] = {0}, Sig[64] = {0};
	uint8		msg[32];
	uint8		ret;
	bool		ver;
	
	atcatls_random(msg);
	
	ret = atcatls_calc_pubkey(0, key);
	if(ret == ATCA_SUCCESS)
	{
		printf("Key Gen\r\n");
		ret = atcatls_sign(0, msg, Sig);
		if(ret == ATCA_SUCCESS)
		{
			ret = atcatls_verify(msg, Sig, key, &ver);
			if(ret == ATCA_SUCCESS)
			{
				printf("Verification Status = %d\r\n", ver);
			}
			else
			{
				printf("VERIF ERROR %d\r\n", ret);
			}
		}
		else
		{
			printf("SIGN ERROR <%d>\r\n", ret);
		}
	}
	else
	{
		printf("Key Gen ERROR = %d\r\n", ret);
	}
}
#endif
