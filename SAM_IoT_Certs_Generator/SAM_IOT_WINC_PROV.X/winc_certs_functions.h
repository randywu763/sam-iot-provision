/* 
    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#ifndef WINC_CERTS_FUNCTIONS_H_
#define WINC_CERTS_FUNCTIONS_H_

#include <stdint.h>
#include <stddef.h>
#include "winc1500/bsp/include/nm_bsp.h"
#include "winc1500/driver/include/m2m_types.h"
#include "cert_def_1_signer.h"
#include "cert_def_4_signer.h"
#include "cert_def_2_device.h"
#include "cryptoauthlib/lib/cryptoauthlib.h"

#define SIGNER_PUBLIC_KEY_MAX_LEN     64
#define MAX_TLS_CERT_LENGTH           1200
#define CERT_SN_MAX_LEN               32

#define SIGNER_CERT_MAX_LEN           (g_cert_def_1_signer.cert_template_size + 120) // +8)
#define DEVICE_CERT_MAX_LEN           (g_cert_def_2_device.cert_template_size + 120) // +8)
#define INIT_CERT_BUFFER_LEN          (MAX_TLS_CERT_LENGTH*sizeof(uint32) - TLS_FILE_NAME_MAX*2 - SIGNER_CERT_MAX_LEN - DEVICE_CERT_MAX_LEN)
#define TLS_SRV_ECDSA_CHAIN_FILE      "ECDSA.lst"

sint8 WINC_CERTS_appendFileBuf(uint32* buffer32, uint32 buffer_size, const char* file_name, uint32 file_size, const uint8* file_data);
size_t WINC_CERTS_getTotalFilesSize(const tstrTlsSrvSecHdr* header);


sint8 WINC_CERTS_transfer(const atcacert_def_t *sign_cert_template,
                          const atcacert_def_t *dev_cert_template);

void WINC_CERTS_initBuffer();

extern uint8_t *signer_cert;
extern size_t signer_cert_size;
extern uint8_t *device_cert;
extern size_t device_cert_size;
extern uint32_t signer_ca_public_key_size;
extern uint32 sector_buffer[];

#endif /* WINC_CERTS_FUNCTIONS_H_ */
