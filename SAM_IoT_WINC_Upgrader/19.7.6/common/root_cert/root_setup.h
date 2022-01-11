#ifndef __ROOT_CERT_SETUP_H__
#define __ROOT_CERT_SETUP_H__

#include "programmer.h"

/*********************************************/

/**
*    @fn         int WriteRootCertificate(char *pcRootCertFileName)
*    @brief      Write a given Root Certificate on the WINC1500 FLASH Root certificate Store.
*    @param[in]  pu8RootCert
*                    Buffer holding the root certificate (Both DER and PEM formats are valid).
*
*   @param [in]  u32RootCertSz
*                    Size of the root certificate buffer
*
*    @return     writing status
*    @author     M.S.M
*    @version    1.0
*/
int WriteRootCertificate(uint8 *pu8RootCert, uint32 u32RootCertSz, uint8* vflash);

#endif  //__ROOT_CERT_SETUP_H__
