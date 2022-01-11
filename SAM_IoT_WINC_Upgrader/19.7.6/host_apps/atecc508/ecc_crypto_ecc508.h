
#ifndef __ECC_CRYPTO_ATECC508_H__
#define __ECC_CRYPTO_ATECC508_H__

#include "ecc_types.h"
#include "tls/atcatls.h"

#define MAX_TLS_CERT_LENGTH			1024
#define SIGNER_CERT_MAX_LEN 		(g_cert_def_1_signer.cert_template_size + 8) // Need some space in case the cert changes size by a few bytes
#define SIGNER_PUBLIC_KEY_MAX_LEN 	64
#define DEVICE_CERT_MAX_LEN			(g_cert_def_2_device.cert_template_size + 8) // Need some space in case the cert changes size by a few bytes
#define CERT_SN_MAX_LEN				32
#define TLS_SRV_ECDSA_CHAIN_FILE	"ECDSA.lst"
#define INIT_CERT_BUFFER_LEN		(MAX_TLS_CERT_LENGTH*sizeof(uint32) - TLS_FILE_NAME_MAX*2 - SIGNER_CERT_MAX_LEN - DEVICE_CERT_MAX_LEN)

typedef struct {
	uint32_t wifi_ssid_size;
	char     wifi_ssid[32];
	uint32_t wifi_password_size;
	char     wifi_password[32];
	uint32_t aws_hostname_size;
	char     aws_hostname[64];
	uint32_t thing_name_size;
	char     thing_name[16];
	uint8_t  root_public_key[64];
	uint8_t  client_id[9];
} aws_data_t;

void eccProcessREQ(tstrEccReqInfo* strEccREQ);
sint8 eccInit(ATCAIfaceCfg* cryptoconf);
sint8 eccTransferCertificates(void);
int eccReadAWSData(aws_data_t* aws_data);

#endif /* __ECC_CRYPTO_ATECC508_H__ */

