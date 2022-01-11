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
#include "spi_flash/include/spi_flash.h"
#include "firmware_addresses.h"
#include "driver/source/nmasic.h"
#include "ota_hdr.h"


#define M2M_OTA_TMP_FILE	"ota_tmp.bin"
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
uint8 buff[512*1024UL];
sint8 programmer_init(void *pvInitValue, uint8 u8Br)
{
	sint8 ret = M2M_SUCCESS;
	uint32 chipid = 0 ,reg = 0;
	uint32 cnt = 0;
	uint8 avail[255] = {0};
	gu32ImageOffset = 0;
	gu32ImageSize = 0;
#ifdef USE_UART
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
#ifndef USE_UART
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
	fpOtaFile = fopen(M2M_OTA_FOLDER,"rb");
	if(fpOtaFile)
	{
		uint32 fileSize = 0;
		fseek(fpOtaFile,0,SEEK_END);
		fileSize = ftell(fpOtaFile);
		fseek(fpOtaFile,0,SEEK_SET);
		fread(buff,fileSize,1,fpOtaFile);
		gu32ImageSize = fileSize;
		//gu32ImageOffset = /*sizeof(tstrOtaShahdr) +*/ sizeof(tstrOtaInitHdr);
		fclose(fpOtaFile);
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
*	@return		erasing status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_erase(uint32 u32Offset, uint32 u32Sz, uint8* vflash)
{
	sint8 ret = M2M_SUCCESS;

	M2M_INFO("(OTA)Erase FO %x sz = %x I = %x\n",u32Offset,u32Sz,gu32ImageOffset);
#if 0
	m2m_memset(&buff[u32Offset],0xff,u32Sz);
	//gu32ImageSize+= u32Sz;
#endif
	return ret;
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


	M2M_INFO("(OTA)Write FO %x sz = %x I = %x\n",u32Offset,u32Sz,gu32ImageOffset);

	m2m_memcpy(&buff[u32Offset],pu8Buf,u32Sz);




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
	M2M_INFO("(OTA)Read FO %x sz = %x I = %x\n",u32offset,u32Sz,gu32ImageOffset);
	m2m_memcpy(pu8Buf,&buff[u32offset],u32Sz);

	return ret;
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
	FILE *fp =NULL;
	sint8 ret = M2M_SUCCESS;
	uint32 u32Sz = 0,val = 0;
	//gu32ImageSize += M2M_TLS_FLASH_ROOTCERT_CACHE_SIZE;
	fp = fopen(M2M_OTA_FOLDER, "wb");
	if(fp)
	{

		fwrite(&buff[0], 1, gu32ImageSize, fp);
		fclose(fp);

	}
	else
	{
		M2M_ERR("cann't create ota file\n");
		ret = M2M_ERR_FAIL;
	}

	return ret;
}

#endif