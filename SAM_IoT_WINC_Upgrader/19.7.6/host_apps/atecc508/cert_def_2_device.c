#include "atcacert/atcacert_def.h"

const uint8_t g_cert_template_2_device[] = {
    0x30, 0x82, 0x01, 0xAE, 0x30, 0x82, 0x01, 0x53,  0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x11, 0x40,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,  0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x02,
    0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE,  0x3D, 0x04, 0x03, 0x02, 0x30, 0x47, 0x31, 0x1D,
    0x30, 0x1B, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x0C,  0x14, 0x45, 0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65,
    0x20, 0x4F, 0x72, 0x67, 0x61, 0x6E, 0x69, 0x7A,  0x61, 0x74, 0x69, 0x6F, 0x6E, 0x31, 0x26, 0x30,
    0x24, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x1D,  0x45, 0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x20,
    0x41, 0x54, 0x45, 0x43, 0x43, 0x35, 0x30, 0x38,  0x41, 0x20, 0x53, 0x69, 0x67, 0x6E, 0x65, 0x72,
    0x20, 0x30, 0x30, 0x33, 0x34, 0x30, 0x20, 0x17,  0x0D, 0x31, 0x35, 0x31, 0x32, 0x31, 0x36, 0x30,
    0x32, 0x35, 0x31, 0x33, 0x37, 0x5A, 0x18, 0x0F,  0x39, 0x39, 0x39, 0x39, 0x31, 0x32, 0x33, 0x31,
    0x32, 0x33, 0x35, 0x39, 0x35, 0x39, 0x5A, 0x30,  0x42, 0x31, 0x1D, 0x30, 0x1B, 0x06, 0x03, 0x55,
    0x04, 0x0A, 0x0C, 0x14, 0x45, 0x78, 0x61, 0x6D,  0x70, 0x6C, 0x65, 0x20, 0x4F, 0x72, 0x67, 0x61,
    0x6E, 0x69, 0x7A, 0x61, 0x74, 0x69, 0x6F, 0x6E,  0x31, 0x21, 0x30, 0x1F, 0x06, 0x03, 0x55, 0x04,
    0x03, 0x0C, 0x18, 0x45, 0x78, 0x61, 0x6D, 0x70,  0x6C, 0x65, 0x20, 0x41, 0x54, 0x45, 0x43, 0x43,
    0x35, 0x30, 0x38, 0x41, 0x20, 0x44, 0x65, 0x76,  0x69, 0x63, 0x65, 0x30, 0x59, 0x30, 0x13, 0x06,
    0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01,  0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03,
    0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x81, 0x9A,  0x9B, 0x76, 0x23, 0xE3, 0x20, 0xE9, 0x85, 0x6D,
    0x74, 0xEE, 0xCC, 0x94, 0x60, 0x06, 0xDB, 0x7E,  0x64, 0xE3, 0x86, 0xB3, 0x3B, 0xF5, 0x08, 0x42,
    0x47, 0x39, 0x8E, 0x5D, 0xD3, 0x22, 0xAF, 0x5B,  0xDC, 0x57, 0xBB, 0xC6, 0xC0, 0xD2, 0x18, 0xF8,
    0x82, 0x01, 0x2F, 0x1E, 0x3B, 0xDA, 0xF7, 0xF2,  0xED, 0x22, 0xAA, 0xCA, 0x88, 0xF5, 0x7C, 0x3A,
    0xEE, 0x5A, 0x3C, 0x3E, 0x1D, 0xF5, 0xA3, 0x23,  0x30, 0x21, 0x30, 0x1F, 0x06, 0x03, 0x55, 0x1D,
    0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x04,  0xCD, 0xE7, 0x19, 0x2E, 0x83, 0x65, 0xA4, 0xC5,
    0x3B, 0xAE, 0xA9, 0x8C, 0xAC, 0xD2, 0x1C, 0xAF,  0xFB, 0xCF, 0x2C, 0x30, 0x0A, 0x06, 0x08, 0x2A,
    0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x03,  0x49, 0x00, 0x30, 0x46, 0x02, 0x21, 0x00, 0xC3,
    0x10, 0x54, 0xB6, 0x26, 0xE1, 0x02, 0x0B, 0x9E,  0xA8, 0x02, 0xE7, 0xA4, 0x58, 0x0F, 0xC3, 0x94,
    0x80, 0xA4, 0x64, 0xB2, 0xC8, 0x0A, 0x50, 0xE2,  0x69, 0x71, 0x87, 0x32, 0x4B, 0xC3, 0xAA, 0x02,
    0x21, 0x00, 0x84, 0xA2, 0x28, 0xED, 0xAB, 0x1D,  0xA9, 0x9A, 0x16, 0x36, 0x77, 0x2A, 0x1E, 0xE0,
    0xBB, 0x3B, 0x1F, 0x4B, 0xCF, 0xBC, 0x6B, 0xD2,  0x54, 0xBA, 0xB7, 0x5F, 0x79, 0xBB, 0x56, 0x44,
    0x30, 0xE4
};

const atcacert_def_t g_cert_def_2_device = {
    .type                   = CERTTYPE_X509,
    .template_id            = 2,
    .chain_id               = 0,
    .private_key_slot       = 0,
    .sn_source              = SNSRC_PUB_KEY_HASH,
    .cert_sn_dev_loc        = { 
        .zone      = DEVZONE_NONE,
        .slot      = 0,
        .is_genkey = 0,
        .offset    = 0,
        .count     = 0
    },
    .issue_date_format      = DATEFMT_RFC5280_UTC,
    .expire_date_format     = DATEFMT_RFC5280_GEN,
    .tbs_cert_loc           = {
        .offset = 4,
        .count  = 343	// 0x157
    },
    .expire_years           = 0,
    .public_key_dev_loc     = {
        .zone      = DEVZONE_DATA,
        .slot      = 0,
        .is_genkey = 1,
        .offset    = 0,
        .count     = 64
    },
    .comp_cert_dev_loc      = {
        .zone      = DEVZONE_DATA,
        .slot      = 10,
        .is_genkey = 0,
        .offset    = 0,
        .count     = 72
    },
    .std_cert_elements      = {
        { // STDCERT_PUBLIC_KEY
            .offset = 246,	// 0xF6
            .count  = 64
        },
        { // STDCERT_SIGNATURE
            .offset = 359,	// 0x167
            .count  = 74
        },
        { // STDCERT_ISSUE_DATE
            .offset = 121,	// 0x79
            .count  = 13
        },
        { // STDCERT_EXPIRE_DATE
            .offset = 136,
            .count  = 15
        },
        { // STDCERT_SIGNER_ID
            .offset = 113,
            .count  = 4
        },
        { // STDCERT_CERT_SN
            .offset = 15,
            .count  = 16
        },
        { // STDCERT_AUTH_KEY_ID
            .offset = 327,
            .count  = 20
        },
        { // STDCERT_SUBJ_KEY_ID
            .offset = 0,
            .count  = 0
        }
    },
    .cert_elements          = NULL,
    .cert_elements_count    = 0,
    .cert_template          = g_cert_template_2_device,
    .cert_template_size     = sizeof(g_cert_template_2_device),
};
