/**
 *
 * \file
 *
 * \brief WINC SSL Interface.
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

/** @defgroup SSLAPI SSL
    @brief
        Provides a description of the SSL Layer.
    @{
        @defgroup SSLCallbacks Callbacks
        @brief
            Provides detail on the available callbacks for the SSL Layer.

        @defgroup SSLEnums Enumerations and Typedefs
        @brief
            Specifies the enums and Data Structures used by the SSL APIs.

        @defgroup SSLFUNCTIONS Functions
        @brief
            Provides detail on the available APIs for the SSL Layer.
    @}
*/

#ifndef __M2M_SSL_H__
#define __M2M_SSL_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "common/include/nm_common.h"
#include "driver/include/m2m_types.h"
#include "driver/source/nmdrv.h"
#include "ecc_types.h"
#include "socket/include/socket.h"

/*!
@ingroup    SSLCallbacks
@typedef    void (*tpfAppSSLCb)(uint8 u8MsgType, void* pvMsg);
@brief      A callback to get SSL notifications.
@param[in]  u8MsgType
                The type of the message received.
@param[in]  pvMsg
                A structure to provide notification payload.
*/
typedef void (*tpfAppSSLCb)(uint8 u8MsgType, void *pvMsg);

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/*!
@ingroup    SSLFUNCTIONS
@fn         NMI_API sint8 m2m_ssl_init(tpfAppSSLCb pfAppSSLCb);
@brief      Initializes the SSL layer.
@param[in]  pfAppSSLCb
                Application SSL callback function.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.
*/
NMI_API sint8 m2m_ssl_init(tpfAppSSLCb pfAppSSLCb);

/*!
@ingroup    SSLFUNCTIONS
@fn         NMI_API sint8 m2m_ssl_handshake_rsp(tstrEccReqInfo* strECCResp, uint8* pu8RspDataBuff, uint16 u16RspDataSz);
@brief      Sends ECC responses to the WINC.
@param[in]  strECCResp
                ECC Response struct.
@param[in]  pu8RspDataBuff
                Pointer of the response data to be sent.
@param[in]  u16RspDataSz
                Response data size.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.
*/
NMI_API sint8 m2m_ssl_handshake_rsp(tstrEccReqInfo *strECCResp, uint8 *pu8RspDataBuff, uint16 u16RspDataSz);

/*!
@ingroup    SSLFUNCTIONS
@fn         NMI_API sint8 m2m_ssl_send_certs_to_winc(uint8* pu8Buffer, uint32 u32BufferSz);
@brief      Sends certificates to the WINC.
@param[in]  pu8Buffer
                Pointer to the certificates. The buffer format must match the format of @ref tstrTlsSrvSecHdr.
@param[in]  u32BufferSz
                Size of the certificates.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.
*/
NMI_API sint8 m2m_ssl_send_certs_to_winc(uint8 *pu8Buffer, uint32 u32BufferSz);

/*!
@ingroup    SSLFUNCTIONS
@fn         NMI_API sint8 m2m_ssl_retrieve_next_for_verifying(tenuEcNamedCurve *penuCurve, uint8 *pu8Value, uint16 *pu16ValueSz, uint8 *pu8Sig, uint16 *pu16SigSz, tstrECPoint *pstrKey);
@brief      Retrieve the next set of information from the WINC for ECDSA verification.
@param[out] penuCurve
                The named curve.
@param[out] pu8Value
                Value retrieved for verification. This is the digest of the message, truncated/prepended to the appropriate size.
@param[inout] pu16ValueSz
                in: Size of value buffer provided by caller.
                out: Size of value retrieved (provided for convenience; the value size is in fact determined by the curve).
@param[out] pu8Sig
                Signature retrieved for verification.
@param[inout] pu16SigSz
                in: Size of signature buffer provided by caller.
                out: Size of signature retrieved (provided for convenience; the signature size is in fact determined by the curve).
@param[out] pstrKey
                Public key retrieved for verification.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.

@pre        This function should only be called after the application has been notified that
            verification information is ready via @ref ECC_REQ_SIGN_VERIFY.

@warning    If this function returns @ref M2M_ERR_FAIL, then any remaining verification info from
            the WINC is lost.
*/
NMI_API sint8 m2m_ssl_retrieve_next_for_verifying(tenuEcNamedCurve *penuCurve, uint8 *pu8Value, uint16 *pu16ValueSz, uint8 *pu8Sig, uint16 *pu16SigSz, tstrECPoint *pstrKey);

/*!
@ingroup    SSLFUNCTIONS
@fn         NMI_API sint8 m2m_ssl_retrieve_cert(uint16* pu16Curve, uint8* pu8Value, uint8* pu8Sig, tstrECPoint* pstrKey);
@brief      Retrieve the next set of information from the WINC for ECDSA verification.
@param[out] pu16Curve
                The named curve, to be cast to type @ref tenuEcNamedCurve.
@param[out] pu8Value
                Value retrieved for verification. This is the digest of the message, truncated/prepended to the appropriate size.
                The size of the value is equal to the field size of the curve, hence is determined by pu16Curve.
@param[out] pu8Sig
                Signature retrieved for verification.
                The size of the signature is equal to twice the field size of the curve, hence is determined by pu16Curve.
@param[out] pstrKey
                Public key retrieved for verification.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.

@pre        This function should only be called after the application has been notified that
            verification information is ready via @ref ECC_REQ_SIGN_VERIFY.

@warning    If this function returns @ref M2M_ERR_FAIL, then any remaining verification info from
            the WINC is lost.

@warning    This API has been deprecated and is kept for legacy purposes only. It is recommended
            that @ref m2m_ssl_retrieve_next_for_verifying is used instead.
*/
NMI_API sint8 m2m_ssl_retrieve_cert(uint16 *pu16Curve, uint8 *pu8Value, uint8 *pu8Sig, tstrECPoint *pstrKey);

/*!
@ingroup    SSLFUNCTIONS
@fn         NMI_API sint8 m2m_ssl_retrieve_hash(uint8* pu8Value, uint16 u16ValueSz)
@brief      Retrieve the value from the WINC for ECDSA signing.
@param[out] pu8Value
                Value retrieved for signing. This is the digest of the message, truncated/prepended to the appropriate size.
@param[in]  u16ValueSz
                Size of value to be retrieved. (The application should obtain this information,
                along with the curve, from the associated @ref ECC_REQ_SIGN_GEN notification.)
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.

@pre        This function should only be called after the application has been notified that
            signing information is ready via @ref ECC_REQ_SIGN_GEN.

@warning    If this function returns @ref M2M_ERR_FAIL, then the value for signing is lost.
*/
NMI_API sint8 m2m_ssl_retrieve_hash(uint8 *pu8Value, uint16 u16ValueSz);

/*!
@ingroup    SSLFUNCTIONS
@fn         NMI_API void m2m_ssl_stop_retrieving(void);
@brief      Allow SSL driver to tidy up when the application chooses not to retrieve all available
            information.

@return     None.

@warning    The application must call this function if it has been notified (via
            @ref ECC_REQ_SIGN_GEN or @ref ECC_REQ_SIGN_VERIFY) that information is available for
            retrieving from the WINC, but chooses not to retrieve it all.
            The application must not call this function if it has retrieved all the available
            information, or if a retrieve function returned @ref M2M_ERR_FAIL indicating that any
            remaining information has been lost.

@see        m2m_ssl_retrieve_next_for_verifying\n
            m2m_ssl_retrieve_cert\n
            m2m_ssl_retrieve_hash
*/
NMI_API void m2m_ssl_stop_retrieving(void);

/*!
@ingroup    SSLFUNCTIONS
@fn         NMI_API void m2m_ssl_stop_processing_certs(void);
@brief      Allow SSL driver to tidy up in case application does not read all available certificates.
@return     None.

@warning    This API has been deprecated and is kept for legacy purposes only. It is recommended
            that @ref m2m_ssl_stop_retrieving is used instead.
*/
NMI_API void m2m_ssl_stop_processing_certs(void);

/*!
@ingroup    SSLFUNCTIONS
@fn         NMI_API void m2m_ssl_ecc_process_done(void);
@brief      Allow SSL driver to tidy up after application has finished processing ECC message.

@return     None.

@warning    The application should call this function after receiving an SSL callback with message
            type @ref M2M_SSL_REQ_ECC, after retrieving any related information, and before
            calling @ref m2m_ssl_handshake_rsp.
*/
NMI_API void m2m_ssl_ecc_process_done(void);

/*!
@ingroup    SSLFUNCTIONS
@fn         NMI_API sint8 m2m_ssl_set_active_ciphersuites(uint32 u32SslCsBMP);
@brief      Sets the active ciphersuites.
@details    Override the default Active SSL ciphers in the SSL module with a certain combination selected by
            the caller in the form of a bitmap containing the required ciphers to be on.\n
            There is no need to call this function if the application will not change the default ciphersuites.
@param[in]  u32SslCsBMP
                Bitmap containing the desired ciphers to be enabled for the SSL module. The ciphersuites are defined in
                @ref SSLCipherSuiteID.
                The default ciphersuites are all ciphersuites supported by the firmware with the exception of ECC ciphersuites.
                The caller can override the default with any desired combination.
                If u32SslCsBMP does not contain any ciphersuites supported by firmware, then the current active list will not
                change.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.
*/
NMI_API sint8 m2m_ssl_set_active_ciphersuites(uint32 u32SslCsBMP);

#endif /* __M2M_SSL_H__ */