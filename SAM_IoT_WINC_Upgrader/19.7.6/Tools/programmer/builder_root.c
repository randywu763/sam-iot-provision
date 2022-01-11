/**
*  @file		programmer.c
*  @brief		This module contains SPI FLASH APIs implementation
*  @author		M.S.M
*  @date		17 SEPT 2013
*  @version		1.0
*/

#ifdef BUILD_CERT
#include "programmer.h"

/**
* STATE REGISTERS ADDRESSES MACROS
*/
static FILE *pFile = NULL;
uint32 gu32ImageSize;

/*********************************************/
/* GLOBAL FUNCTIONS							 */
/*********************************************/
/**
*	@fn			sint8 programmer_init(u32Offset, u32Sz)
*	@brief		programmer intialization
*	@return		erasing status
*	@author		M.S.M
*	@version	1.0
*/
uint8 buff[6*1024UL];

/**
*	@fn			sint8 programmer_erase(u32Offset, u32Sz, uint8*)
*	@brief		programmer erase memory size
*	@param[IN]	u32Offset
*					Address to write to at the SPI flash
*	@param[IN]	u32Offset
*					Size of memory to erase
*	@param[IN]	uint8*
*					vflash pointer
*	@return		erasing status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_erase(uint32 u32Offset, uint32 u32Sz, uint8* vflash)
{
	return M2M_SUCCESS;
}

/**
*	@fn			sint8 programmer_write(*pu8Buf, u32Offset, u32Sz, uint8*)
*	@brief		programmer initialization
*	@param[IN]	* pu8Buf
*					pointer to data buffer
*	@param[IN]	u32Offset
*					Address to write to at the SPI flash
*	@param[IN]	u32Sz
*					Size of memory to write
*	@return		writing status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_write(uint8* pu8Buf, uint32 u32Offset, uint32 u32Sz, uint8* vflash)
{
	sint8 ret = M2M_SUCCESS;
	/* Write.
	*/
	fseek(pFile, 0L, SEEK_SET);
	fwrite(pu8Buf, u32Sz, 1, pFile);
	fclose(pFile);
	return ret;
}

/**
*	@fn			programmer_read
*	@brief		Read from data from SPI flash
*	@param[OUT]	pu8Buf
*					Pointer to data buffer
*	@param[IN]	u32Addr
*					Address to read from at the SPI flash
*	@param[IN]	u32Sz
*					Data size
*	@note		Data size is limited by the SPI flash size only
*	@author		M. Abdelmawla
*	@version	1.0
*/
sint8 programmer_read(uint8 *pu8Buf, uint32 u32offset, uint32 u32Sz)
{
	sint8 ret = M2M_SUCCESS;

	if(pFile == NULL) //if file does not exist, create it
	{
		pFile = fopen("root_cert.bin", "wb");
		memset(pu8Buf, 0xFF, M2M_TLS_ROOTCER_FLASH_SIZE);
	} else {
		pFile = fopen("root_cert.bin","rb+");
	}
	fseek(pFile, 0L, SEEK_END);
	gu32ImageSize = ftell(pFile);
	fread(pu8Buf, gu32ImageSize, 1, pFile);


	return ret;
}
#endif