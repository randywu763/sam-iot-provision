/*
 * image_downloader.h
 *
 * Created: 12/4/2014 3:04:24 PM
 *  Author: aali
 */ 

#ifndef __PROGRAMMER_H__
#define __PROGRAMMER_H__

/**
* Include
*/
#include "spi_flash_map.h"
#include "spi_flash/include/spi_flash.h"


#define ROOT_CERT_SIZE M2M_TLS_ROOTCER_FLASH_SIZE

#define	programmer_get_flash_size()						(((spi_flash_get_size()*1024)/8)*1024)
#define	programmer_write(pu8Buf, u32Offset, u32Sz)		spi_flash_write(pu8Buf, u32Offset, u32Sz)
#define	programmer_erase(u32Offset, u32Sz)				spi_flash_erase(u32Offset, u32Sz)
#define	programmer_eraseall()							programmer_erase(0, programmer_get_flash_size())
#define	programmer_read(pu8Buf, u32Offset, u32Sz)		spi_flash_read(pu8Buf, u32Offset, u32Sz)	


#define programmer_write_root_cert(buff)				programmer_write((uint8*)buff, M2M_TLS_ROOTCER_FLASH_OFFSET, M2M_TLS_ROOTCER_FLASH_SIZE)
#define programmer_read_root_cert(buff)					programmer_read((uint8*)buff, M2M_TLS_ROOTCER_FLASH_OFFSET, M2M_TLS_ROOTCER_FLASH_SIZE)
#define programmer_erase_root_cert()					programmer_erase(M2M_TLS_ROOTCER_FLASH_OFFSET, M2M_TLS_ROOTCER_FLASH_SIZE)

#define programmer_write_tls_cert_store(buff)			programmer_write((uint8*)buff, M2M_TLS_SERVER_FLASH_OFFSET, M2M_TLS_SERVER_FLASH_SIZE)
#define programmer_read_tls_cert_store(buff)			programmer_read((uint8*)buff, M2M_TLS_SERVER_FLASH_OFFSET, M2M_TLS_SERVER_FLASH_SIZE)
#define programmer_erase_tls_cert_store()				programmer_erase(M2M_TLS_SERVER_FLASH_OFFSET, M2M_TLS_SERVER_FLASH_SIZE)

#endif /* __PROGRAMMER_H__ */