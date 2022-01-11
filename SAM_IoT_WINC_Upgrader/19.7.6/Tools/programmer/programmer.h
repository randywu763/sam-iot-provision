/**
*  @file		spi_flash.h
*  @brief		This module contains spi flash APIs definations
*  @author		M.S.M
*  @date		17 SEPT 2013
*  @version		1.0
*/

#ifndef __FIRMWARE_PROGRAMMER__
#define __FIRMWARE_PROGRAMMER__
//#define _FLASH_TEST_

/**
* Include
*/
#include "common/include/nm_common.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "driver/source/nmbus.h"
#include "spi_flash_map.h"
#include "spi_flash_map_v0.h"
#include "firmware_addresses.h"
#include "driver/source/nmasic.h"
#include "driver/source/nmdrv.h"

/**
* Windows specific for threads
*/
#ifdef WIN32
#include "windows.h"
#include "conio.h"
#include "process.h"
#endif

/**
* REVISION MACROS
*/
#if 0
extern  uint32 nmi_get_chipid(void);

#define REV_B0			(0x2B0)	/*!< Rev. B0 boards */
#define REV_3A0         (0x3A0) /*!< Rev. A0 boards */

#define #REV(id)			(((id) & 0x00000fff ) )						/*!< To get revision ID */
#define ISNMC1000(id)	(((id & 0xfffff000) == 0x100000) ? 1 : 0)	/*!< To check if revision is 0x100000 */
#define ISNMC1500(id)   (((id & 0xfffff000) == 0x150000) ? 1 : 0)	/*!< To check if revision is 0x150000 */
#endif

#define programmer_write_root_cert(buff, vflash)				programmer_write((uint8*)buff, M2M_TLS_ROOTCER_FLASH_OFFSET, M2M_TLS_ROOTCER_FLASH_SIZE, vflash)
#define programmer_read_root_cert(buff)		   			        programmer_read((uint8*)buff, M2M_TLS_ROOTCER_FLASH_OFFSET, M2M_TLS_ROOTCER_FLASH_SIZE)
#define programmer_erase_root_cert(vflash)						programmer_erase(M2M_TLS_ROOTCER_FLASH_OFFSET, M2M_TLS_ROOTCER_FLASH_SIZE, vflash)

#define programmer_write_firmware_image(buff,offSet,sz,vflash)	programmer_write((uint8*)buff, offSet, sz, vflash)
#define programmer_read_firmware_image(buff,offSet,sz)	        programmer_read((uint8*)buff, offSet, sz)
#define programmer_erase_firmware_image(vflash)					programmer_erase(0,programmer_get_flash_size(), vflash)

#define programmer_erase_http_files(vflash)						programmer_erase(M2M_HTTP_MEM_FLASH_OFFSET, M2M_HTTP_MEM_FLASH_SZ, vflash)
#define programmer_erase_first_partition(vflash)				programmer_erase(M2M_FIRMWARE_FLASH_OFFSET, M2M_FIRMWARE_FLASH_SZ, vflash)

/**
PROGRAMMER APIS
*/
/**
*	@fn			sint8 programmer_init(u32Offset, u32Sz)
*	@brief		programmer intialization
*	@param[IN]	uint8*
*					pointer to virtual flash to update
*	@return		erasing status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_init(void *,uint8);
/**
*	@fn			sint8 programmer_erase(u32Offset, u32Sz, uint8*)
*	@brief		programmer erase memory size
*	@param[IN]	u32Offset
*					Address to write to at the SPI flash
*	@param[IN]	u32Offset
*					Size of memory to erase
*	@param[IN]	uint8*
*					pointer to virtual flash to update
*	@return		erasing status
*	@author		M.S.M
*	@version	1.0
*/
uint32 programmer_get_flash_size(void);
/**
*	@fn			sint8 programmer_erase(u32Offset, u32Sz, uint8*)
*	@brief		programmer erase memory size
*	@param[IN]	u32Offset
*					Address to write to at the SPI flash
*	@param[IN]	u32Offset
*					Size of memory to erase
*	@param[IN]	uint8*
*					pointer to virtual flash to update
*	@return		erasing status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_eraseall(uint8* vflash);
/**
*	@fn			sint8 programmer_erase(u32Offset, u32Sz, uint8*)
*	@brief		programmer erase memory size
*	@param[IN]	u32Offset
*					Address to write to at the SPI flash
*	@param[IN]	u32Offset
*					Size of memory to erase
*	@param[IN]	uint8*
*					pointer to virtual flash to update
*	@return		erasing status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_erase(uint32 u32Offset, uint32 u32Sz, uint8* vflash);
/**
*	@fn			sint8 programmer_write(*pu8Buf, u32Offset, u32Sz, uint8*)
*	@brief		programmer initialization
*	@param[IN]	* pu8Buf
*					pointer to data buffer
*	@param[IN]	u32Offset
*					Address to write to at the SPI flash
*	@param[IN]	u32Sz
*					Size of memory to write
*	@param[IN]	uint8*
*					pointer to virtual flash to update
*	@return		writing status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_write(uint8* pu8Buf, uint32 u32Offset, uint32 u32Sz, uint8* vflash);
/**
*	@fn			programmer_read
*	@brief		Read from data from SPI flash
*	@param[OUT]	pu8Buf
*					Pointer to data buffer
*	@param[IN]	u32Addr
*					Address to read from at the SPI flash
*	@param[IN]	u32Sz
*					Data size
*	@param[IN]	uint8*
*					pointer to virtual flash to update
*	@note		Data size is limited by the SPI flash size only
*	@author		M. Abdelmawla
*	@version	1.0
*/
sint8 programmer_read(uint8 *pu8Buf, uint32 u32Offset, uint32 u32Sz);
/**
*	@fn			sint8 programmer_deinit(void)
*	@brief		programmer de-initialization
*	@return		de-initialization status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_deinit(void);

#endif /* __FIRMWARE_PROGRAMMER__ */
