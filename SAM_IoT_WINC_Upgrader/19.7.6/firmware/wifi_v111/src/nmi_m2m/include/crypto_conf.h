/*!
@file
	tls_conf.h

@brief	Crypto Configuration File

@author
	Ahmed Ezzat

@date
	16 Nov 2015
*/

#ifndef __TLS_CONF_H__
#define __TLS_CONF_H__

/****************************************
 *	Crypto hardware available			*
 ****************************************/
#define __HW_BIGINT_ENGINE__
/*!<
Enable the HW accelerator for the BIGINT module of the 1003A0 ASIC.
*/

#define BI_HW_MAX_SZ					(256)

#define __AES_HW_ENGINE__
/*!<
	Enable the HW accelerator for AES128
*/

#define __SHA_HW_ENGINE__
/*!<
	Enable the HW accelerator for SHA1 and SHA256 (for use by TLS, Wi-Fi PSK calculation, and host via HIF).
	Note that the TLS code:
	- does not actually use HW for SHA1
	- only uses HW for SHA256 if __HW_SHA256_ENGINE__ is also defined.
*/

#ifdef __SHA_HW_ENGINE__
	//#define __HW_SHA256_ENGINE__
	/*!<
		Enable TLS usage of the HW accelerator for Sha256
	*/
#endif

#define __CRYPTO_AES_GCM_SUPPORT__
/*!<
*/

#define TLS_MAX_SUPPORTED_DH_GROUP				(BI_HW_MAX_SZ)
/*!<
*/

#define __CRYPTO_SHA224_SUPPORT__
#define __CRYPTO_SHA384_SUPPORT__
#define __CRYPTO_SHA512_SUPPORT__
#if (defined __CRYPTO_SHA224_SUPPORT__) || (defined __CRYPTO_SHA384_SUPPORT__) \
		|| (defined __CRYPTO_SHA512_SUPPORT__) || (__ROM_VER__ < ROM_VER_1)
#	define USE_HASH512CONTEXT
#endif


/****************************************
 *	TLS ciphersuite support				*
 ****************************************/
#define __TLS_ECC_SUPPORT__
#define __TLS_ECDSA_SUPPORT__
#define __TLS_ECC_LIB_HOST__

#define __TLS_RSA_CS_SUPPORT__
#ifdef __TLS_RSA_CS_SUPPORT__
#	define __TLS_DHE_SUPPORT__			// DHE requires RSA
#endif

#if (!defined __TLS_RSA_CS_SUPPORT__) && (!defined __TLS_ECC_SUPPORT__)
#error "At least one of RSA_CS or ECC must be supported"
#endif

#ifdef __CRYPTO_AES_GCM_SUPPORT__
#	define __TLS_GCM_SUPPORT__
#endif

#define __TLS_AES256_SUPPORT__
/****************************************
 *	TLS mode support					*
 ****************************************/
#define __TLS_SRV_SUPPORT__
#define __TLS_CLIENT_AUTH_SUPPORT__


/****************************************
 *	Debug options						*
 ****************************************/
//#define __TLS_CIPHERSUITE_DBG__
//#define X509_DUMP_ENABLE

#endif /* __TLS_CONF_H__ */
