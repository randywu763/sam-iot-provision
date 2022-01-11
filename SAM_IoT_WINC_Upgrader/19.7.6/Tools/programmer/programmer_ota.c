/**
*  @file		programmer.c
*  @brief		This module contains SPI FLASH APIs implementation
*  @author		M.S.M
*  @date		17 SEPT 2013
*  @version		1.0
*/

#ifdef OTA_GEN
#include "../efuse/efuse.h"
#include "programmer.h"
#include "spi_flash.h"
#include "firmware_addresses.h"
#include "driver/source/nmasic.h"
#include "ota_hdr.h"


/**
* STATE REGISTERS ADDRESSES MACROS
*/

FILE *	fpOtaFile		= NULL;
uint32	gu32ImageOffset = 0;
uint32	gu32ImageSize = 0;

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
sint8 programmer_init(void *pvInitValue,uint8 u8Br)
{
	sint8 ret = M2M_SUCCESS;
	uint32 chipid = 0 ,reg = 0;
	uint32 cnt = 0;
	uint8 avail[255] = {0};
	gu32ImageOffset = 0;
	gu32ImageSize = 0;
#ifdef CONF_WINC_USE_UART
	if(*((uint8 *)pvInitValue) == 0) {
		int navail;
		navail = nm_bus_port_detect(avail,nm_uart_sync_cmd);
		if(navail < 1) {
			/* No ports avaiable. */
			ret = -1;
			goto ERR;
		} else if(navail < 2) {
			/* Only one port avaiable. Use it */
			cnt = avail[0];
		} else {
			/* Let user choose COM port */
			cnt = 0;
		}
	}
	else{
		cnt = *((uint8 *)pvInitValue);
	}
#endif
#if 0
	ret = nm_bus_iface_init((uint8 *)&cnt);
	if(ret != M2M_SUCCESS)
	{
		M2M_PRINT(">>(ERR):Connect Fail\n");
		goto ERR;
	}
	cnt = 0;
#ifndef CONF_WINC_USE_UART
	chip_reset_and_cpu_halt();
#endif

	chipid =  nmi_get_chipid();
	if (!ISNMC1500(chipid) && !ISNMC1000(chipid)) {
		M2M_PRINT(">>(ERR):Invalid chip ID = 0x%08lx\n", chipid);
		M2M_PRINT(">>(ERR):Connect NMC1500 Dx Fail\n");
		ret = M2M_ERR_FAIL;
		goto ERR;
	}

	M2M_PRINT("Chip id %lx\n", chipid);
#endif
	fpOtaFile = fopen(M2M_OTA_FILE,"wb");
	if(fpOtaFile)
	{
		//gu32ImageOffset = /*sizeof(tstrOtaShahdr) +*/ sizeof(tstrOtaInitHdr);
		goto ERR;
	}
	else
	{
		M2M_ERR("Can't open ota tmp file\n");
		ret = M2M_ERR_FAIL;
	}
ERR:
	return ret;

}

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
sint8 programmer_erase(uint32 u32Offset, uint32 u32Sz, uint8* vflash)
{

	return M2M_SUCCESS;
}
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
uint32 programmer_get_flash_size(void)
{
	return FLASH_TOTAL_SZ;
}
/**
*	@fn			sint8 programmer_erase(u32Offset, u32Sz)
*	@brief		programmer erase memory size
*	@param[IN]	u32Offset
*					Address to write to at the SPI flash
*	@param[IN]	u32Offset
*					Size of memory to erase
*	@return		erasing status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_eraseall(void)
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
*	@param[IN]	uint8*
*					pointer to virtual flash to update
*	@return		writing status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_write(uint8* pu8Buf, uint32 u32Offset, uint32 u32Sz, uint8* vflash)
{
	sint8 ret = M2M_SUCCESS;


	M2M_INFO("(OTA)Write FO %x sz = %x I = %x\n",u32Offset,u32Sz,gu32ImageOffset);
	if(fpOtaFile)
	{
		fseek(fpOtaFile, gu32ImageOffset  + u32Offset, SEEK_SET);
		fwrite(pu8Buf,u32Sz,1,fpOtaFile);
		gu32ImageSize+= u32Sz;
		if(vflash)
            memcpy(&vflash[gu32ImageOffset  + u32Offset],pu8Buf, u32Sz);
	}
	else
	{
		M2M_ERR("cann't create ota file\n");
		ret = M2M_ERR_FAIL;
	}



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
sint8 programmer_read(uint8 *pu8Buf, uint32 u32offset, uint32 u32Sz, uint8* vflash)
{

	return M2M_SUCCESS;
}

/**
*	@fn			sint8 programmer_deinit(void)
*	@brief		programmer de-initialization
*	@return		de-initialization status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_deinit(void)
{
	sint8 ret = M2M_SUCCESS;
	uint32 u32Sz = 0,val = 0;
	if(fpOtaFile)
	{
#if 0
		tstrOtaInitHdr strOtaInitHdr = {0};
		//tstrOtaShahdr strOtaShahdr = {0};
		m2m_memcpy((uint8*)&strOtaInitHdr.u32OtaMagicValue, (uint8*)"NMIO", 4);

		/**
			Copy sha vector
		*/
		fseek(fpOtaFile, 0L, SEEK_END);
		u32Sz = ftell(fpOtaFile);
		fseek(fpOtaFile, 0L, SEEK_SET);
		M2M_INFO("SIZE %d %d\n",u32Sz,gu32ImageSize);
		strOtaInitHdr.u32OtaPayloadSize = u32Sz;// - sizeof(tstrOtaShahdr); 

		//fwrite(&strOtaShahdr,sizeof(tstrOtaShahdr),1,fpOtaFile);

		fwrite(&strOtaInitHdr,sizeof(tstrOtaInitHdr),1,fpOtaFile);
#endif
		fclose(fpOtaFile);
	}
	else
	{
		M2M_ERR("cann't create ota file\n");
		ret = M2M_ERR_FAIL;
	}

	return ret;
}

#endif
