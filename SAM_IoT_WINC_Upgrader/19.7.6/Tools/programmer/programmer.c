/**
*  @file		programmer.c
*  @brief		This module contains SPI FLASH APIs implementation
*  @author		M.S.M
*  @date		17 SEPT 2013
*  @version		1.0
*/

#include "../efuse/efuse.h"
#include "programmer.h"
#include "spi_flash/include/spi_flash.h"
#include "firmware_addresses.h"
#include "driver/source/nmuart.h"

#define UART_HIGH_RATE  (500000UL)
#define UART_LOW_RATE	(115200UL)
#define TIMEOUT  10000

#ifdef CONF_WINC_USE_UART
#define RETRY_UART_WRITE_WORKAROUND
#endif

#ifdef RETRY_UART_WRITE_WORKAROUND
#define UART_RETRY_MAX (50)
#endif /* RETRY_UART_WRITE_WORKAROUND */

typedef sint8(*tpfCheckPort)(void);

#define M2M_SHARED_OFFSET				0x3000UL
#define NMI_AHB_SHARE_MEM_BASE			0xd0000UL
#define NMI_CORTUS_SHARE_MEM_BASE		0x60000000UL


/*
SPI FLASH COMMANDS
*/
#define PROG_CMD_W						((uint8)6)		/*!< write from chip memory memory to SPI flash */
#define PROG_CMD_R						((uint8)7)		/*!< Read from SPI flash to chip memory */
#define PROG_CMD_E						((uint8)8)		/*!< Erase SPI flash memory of size */

/**
SPI FLASH STATES
*/
#define PROG_STATE_IDLE 				((uint8)9)
#define	PROG_STATE_WAITING				((uint8)10)
#define	PROG_STATE_PROCESSING			((uint8)11)
#define	PROG_STATE_ERROR				((uint8)12)

#define PROG_HDR_SZ					(20)

extern uint8 nm_bus_port_detect(uint8 * avail,tpfCheckPort pfChkPort);
extern uint32 spi_flash_get_size(void);
extern uint8 nm_bus_get_chip_type(void);

#ifdef RETRY_UART_WRITE_WORKAROUND
static uint8 temp_buf[64*1024UL];
#endif /* RETRY_UART_WRITE_WORKAROUND */

/*********************************************/
/* STATIC FUNCTIONS							 */
/*********************************************/

#ifndef OTA_GEN
/**
*	@fn			sint8 download_and_run_programmer_fw(void)
*	@brief		Download programmer_firmware
*	@author		M.S.M
*	@version	1.0
*/
static sint8 download_and_run_programmer_fw(void)
{
	uint32 reg = 0,val = 0;
	uint32 cnt = 0;
	sint8 ret = M2M_SUCCESS;
	FILE *fp = NULL;

	fp = fopen(M2M_PROGRAMMER_FILE, "rb");

	if(fp)
	{
		uint8 *fw;
		int sz;

		fseek(fp, 0L, SEEK_END);
		sz = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		fw = malloc(sz);
		if(!fw)
		{
			ret = M2M_ERR_FAIL;
			M2M_PRINT("Out of memory download_and_run_programmer_fw\n");
			return ret;
		}
		fread(fw,sz,1,fp);

		nm_write_block(NMI_AHB_SHARE_MEM_BASE + 0x1000,&fw[0],sz);
		nm_write_block(0x60000,&fw[0],4);

#ifdef CONF_WINC_USE_UART
		if(!nm_bus_get_chip_type())
#endif
		{
			/* set boot reg to trap #0 in case of serial bridge only and I2C*/
			nm_write_reg(0xc0000, 0x71);
			cpu_start();
		}
		nm_write_reg(BOOTROM_REG,M2M_START_FIRMWARE);
		nm_bsp_sleep(100);
	}
	else
	{
		ret = M2M_ERR_FAIL;
		M2M_PRINT("Failed to open programmer fw file at \n%s\n",M2M_PROGRAMMER_FILE);
	}
	return ret;
}

/**
*	@fn			sint8 programmer_load_to_cortus_mem(u32MemAdr, u32FlashAdr, u32Sz)
*	@brief		Load program to cortus memory
*	@author		M.S.M
*	@version	1.0
*/
static sint8 programmer_load_to_cortus_mem(uint32 u32MemAdr, uint32 u32FlashAdr, uint32 u32Sz)
{
	uint8 i = 6;
	uint32 reg = 0,cnt = 0;
	uint32 cmd = 0;
	uint32 ret = M2M_SUCCESS;
	uint8 b[PROG_HDR_SZ];

	/*write reg*/
	b[0] = PROG_CMD_R;
	i = 8;
	b[0+i] = (uint8)(u32Sz & 0x000000ff);
	b[1+i] = (uint8)((u32Sz & 0x0000ff00)>>8);
	b[2+i] = (uint8)((u32Sz & 0x00ff0000)>>16);
	b[3+i] = (uint8)((u32Sz & 0xff000000)>>24);
	b[4+i] = (uint8)(u32MemAdr & 0x000000ff);
	b[5+i] = (uint8)((u32MemAdr & 0x0000ff00)>>8);
	b[6+i] = (uint8)((u32MemAdr & 0x00ff0000)>>16);
	b[7+i] = (uint8)((u32MemAdr & 0xff000000)>>24);
	b[8+i] = (uint8)(u32FlashAdr & 0x000000ff);
	b[9+i] = (uint8)((u32FlashAdr & 0x0000ff00)>>8);
	b[10+i] = (uint8)((u32FlashAdr & 0x00ff0000)>>16);
	b[11+i] = (uint8)((u32FlashAdr & 0xff000000)>>24);
	cmd = b[0];
	nm_write_block(NMI_AHB_SHARE_MEM_BASE + M2M_SHARED_OFFSET, (uint8*)&b, sizeof(b));
	nm_write_reg(NMI_STATE_REG,cmd);
	while(1)
	{
		ret = nm_read_reg_with_ret(NMI_STATE_REG, (uint32 *)&reg);
		if(M2M_SUCCESS != ret) break;
		if(reg != cmd){
			break;
		}
		if(++cnt > TIMEOUT){
			reg = PROG_STATE_ERROR;
			break;
		}
		nm_bsp_sleep(1);
	}

	return ((reg==PROG_STATE_IDLE)?(M2M_SUCCESS):(M2M_ERR_FAIL));
}
/**
*	@fn			sint8 programmer_read_internal(*pu8Buf, u32Addr, u32Sz)
*	@brief		read block of memory
*	@author		M.S.M
*	@version	1.0
*/
static sint8 programmer_read_internal(uint8 *pu8Buf, uint32 u32Addr, uint32 u32Sz)
{
	sint8 ret = M2M_SUCCESS;
	/* read size must be < 64KB */
	uint32 u32Ram = NMI_AHB_SHARE_MEM_BASE + M2M_SHARED_OFFSET + PROG_HDR_SZ;
	programmer_load_to_cortus_mem(u32Ram, u32Addr, u32Sz);
	nm_read_block(u32Ram, pu8Buf, u32Sz);
	return ret;
}

/**
*	@fn			sint8 programmer_pp(u32Offset, *pu8Buf, u32Sz)
*	@brief		Program Page Programming
*	@author		M.S.M
*	@version	1.0
*/
static sint8 programmer_pp(uint32 u32Offset, uint8 *pu8Buf, uint32 u32Sz)
{
	uint32 u32RamAddr = NMI_AHB_SHARE_MEM_BASE + M2M_SHARED_OFFSET + PROG_HDR_SZ;
	uint32 reg = 0,cnt = 0;
	uint32 cmd = 0;
	uint8 cs = 0;
	uint32 ret = M2M_SUCCESS;
	uint8 b[PROG_HDR_SZ];
#ifdef RETRY_UART_WRITE_WORKAROUND
	int retry;
#endif /* RETRY_UART_WRITE_WORKAROUND */
	cs = m2m_checksum(pu8Buf,u32Sz);
	/*write reg*/
	b[0] = PROG_CMD_W;
	b[1] = 0;
	b[2] = 0;
	b[3] = 0;

	b[4] = cs;
	b[5] = 0;
	b[6] = 0;
	b[7] = 0;
	b[8] = (uint8)(u32Sz & 0x000000ff);
	b[9] = (uint8)((u32Sz & 0x0000ff00)>>8);
	b[10] = (uint8)((u32Sz & 0x00ff0000)>>16);
	b[11] = (uint8)((u32Sz & 0xff000000)>>24);
	b[12] = (uint8)(u32RamAddr & 0x000000ff);
	b[13] = (uint8)((u32RamAddr & 0x0000ff00)>>8);
	b[14] = (uint8)((u32RamAddr & 0x00ff0000)>>16);
	b[15] = (uint8)((u32RamAddr & 0xff000000)>>24);
	b[16] = (uint8)(u32Offset & 0x000000ff);
	b[17] = (uint8)((u32Offset & 0x0000ff00)>>8);
	b[18] = (uint8)((u32Offset & 0x00ff0000)>>16);
	b[19] = (uint8)((u32Offset & 0xff000000)>>24);
	cmd = b[0];

#ifndef RETRY_UART_WRITE_WORKAROUND
	ret = nm_write_block(NMI_AHB_SHARE_MEM_BASE + M2M_SHARED_OFFSET, (uint8*)&b, sizeof(b));
#else /* RETRY_UART_WRITE_WORKAROUND */
	retry = UART_RETRY_MAX;
	do {
		ret = nm_write_block(NMI_AHB_SHARE_MEM_BASE + M2M_SHARED_OFFSET, (uint8*)&b, sizeof(b));
		if(M2M_SUCCESS == ret)  {

			ret = nm_read_block(NMI_AHB_SHARE_MEM_BASE + M2M_SHARED_OFFSET, temp_buf, (sizeof(b)));
			if(M2M_SUCCESS == ret)  {
				if(memcmp(b, temp_buf, sizeof(b)) != 0) {
					ret = M2M_ERR_FAIL;
				}
			}
		}
	} while((retry-- > 0) && (M2M_SUCCESS != ret));
#endif /* RETRY_UART_WRITE_WORKAROUND */
	if(M2M_SUCCESS != ret)  {
		/* return error */ goto ERR1;
	}
#ifndef RETRY_UART_WRITE_WORKAROUND
	ret = nm_write_block(NMI_AHB_SHARE_MEM_BASE + PROG_HDR_SZ+ M2M_SHARED_OFFSET, pu8Buf, u32Sz);
#else /* RETRY_UART_WRITE_WORKAROUND */
	retry = UART_RETRY_MAX;
	do {

		ret = nm_write_block(NMI_AHB_SHARE_MEM_BASE + PROG_HDR_SZ+ M2M_SHARED_OFFSET, pu8Buf, u32Sz);
		if(M2M_SUCCESS == ret)  {
			ret = nm_read_block(NMI_AHB_SHARE_MEM_BASE + PROG_HDR_SZ + M2M_SHARED_OFFSET, temp_buf, u32Sz);
			if(M2M_SUCCESS == ret)  {
				if(memcmp(pu8Buf, temp_buf, u32Sz) != 0) {
					ret = M2M_ERR_FAIL;
				}
			}
		}
	} while((retry-- > 0) && (M2M_SUCCESS != ret));
#endif /* RETRY_UART_WRITE_WORKAROUND */
	if(M2M_SUCCESS != ret)  {
		/* return error */ goto ERR1;
	}
	/* send command to programmer firmware. */
	nm_write_reg(NMI_STATE_REG,cmd);
	while(1)
	{
		ret = nm_read_reg_with_ret(NMI_STATE_REG, (uint32 *)&reg);
		if(M2M_SUCCESS != ret) break;
		if(reg != cmd){
			break;
		}
		if(++cnt > TIMEOUT){
			reg = PROG_STATE_ERROR;
			break;
		}
		nm_bsp_sleep(1);
	}

	return ((reg==PROG_STATE_IDLE)?(M2M_SUCCESS):(M2M_ERR_FAIL));
ERR1:
	return M2M_ERR_FAIL;
}


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
extern uint8 comports[255][3];
sint8 programmer_init(void *pvInitValue,uint8 u8Br)
{
	sint8 ret = M2M_SUCCESS;
	uint32 chipid = 0, reg = 0;
	uint32 cnt = 0;
	uint8 uart_type = 0;

	uint8 port = *((uint8*)pvInitValue);

#ifdef CONF_WINC_USE_UART
	if(u8Br)
	{
		if(port != 0)
		{
			ret = nm_bus_iface_init(&port);
			if(ret != M2M_SUCCESS)
			{
				M2M_PRINT(">>(ERR):Connect Fail\n");
				goto ERR;
			}
			M2M_PRINT("Send break on %d\n", port);
			nm_bus_break();
			nm_bus_iface_deinit();
		}
		else
		{
			M2M_PRINT("please set the port number in arguments to send the break command\n");
		}
	}

    if(port == 0) // Auto-detect
    {
        uint8 avail[255] = {0};
        int navail = nm_bus_port_detect(avail, nm_uart_sync_cmd);
        if(navail < 1) // No ports available.
        {
            ret = -1;
            goto ERR;
        }
        else if(navail < 2)
            port = avail[0]; // Only one port available. Use it
        else
            port = 0; // Let user choose COM port
    }
    else
    {
        // comports is normally populated when nm_bus_port_detect() is called, but we don't want to do all
        // of the auto-detect stuff if we've manually supplied a port to use.
        comports[0][0] = port;
        comports[0][1] = 0;
        comports[0][2] = 1; // Flow control
    }
#endif

	ret = nm_bus_iface_init((uint8 *)&port);
	if(ret != M2M_SUCCESS)
	{
		M2M_PRINT(">>(ERR):Connect Fail\n");
		goto ERR;
	}

#ifdef CONF_WINC_USE_UART
    uart_type = nm_uart_sync_cmd();
    if (uart_type >= 0)
        comports[0][1] = uart_type;
    else
        goto ERR;
#endif

#ifndef CONF_WINC_USE_UART
	chip_reset_and_cpu_halt();
#endif

	chipid =  GET_CHIPID();
	if (!ISNMC1500(chipid) && !ISNMC1000(chipid)) {
		M2M_PRINT(">>(ERR):Invalid chip ID = 0x%08lx\n", chipid);
		M2M_PRINT(">>(ERR):Connect NMC1500 Dx Fail\n");
		ret = M2M_ERR_FAIL;
		goto ERR;
	}

	M2M_PRINT("Chip id %lx\n", chipid);
	ret = download_and_run_programmer_fw();
	if(ret != M2M_SUCCESS)
	{
		goto ERR;
	}
	M2M_PRINT(">Waiting for chip permission...\n");
	{
#ifdef CONF_WINC_USE_UART
		uint32 baudrate;
		baudrate = UART_HIGH_RATE;
		if(!nm_bus_get_chip_type())
		{
			ret = nm_bus_iface_reconfigure(&baudrate);
			if(ret != M2M_SUCCESS)
			{
				goto ERR;
			}
		}
#else
		uint32 baudrate =0;
#endif
		nm_bus_reinit((void *)&baudrate);
	}
	do
	{
		ret = nm_read_reg_with_ret(NMI_STATE_REG, (uint32 *)&reg);
		if(M2M_SUCCESS != ret)
		{
			goto ERR;
		}
		if(++cnt > 2000)
		{
			M2M_INFO("Time out for wait firmware Run\n");
			ret = M2M_ERR_INIT;
			goto ERR;
		}
		if(reg == PROG_STATE_IDLE)
		{
			M2M_PRINT("OK.\n\r");
			break;
		}
		nm_bsp_sleep(1);
	}while(1);
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
uint32 programmer_get_flash_size(void)
{
	return (((spi_flash_get_size()*1024)/8)*1024);
}
/**
*	@fn			sint8 programmer_eraseall(uint8*)
*	@brief		programmer erase all
*	@param[IN]	uint8*
*					pointer to virtual flash to update
*	@return		erasing status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_eraseall(uint8* vflash)
{
	return programmer_erase(0,programmer_get_flash_size(), vflash);
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
	uint8 i = 6;
	uint32 reg = 0,cnt = 0;
	uint32 cmd = 0;
	uint32 ret = M2M_SUCCESS;
	uint8 b[PROG_HDR_SZ];
#ifdef PROFILING
	uint32 t;
	t = GetTickCount();
	M2M_PRINT(">Start erasing...\n");
#endif
	//M2M_INFO("(OTA)Erase FO =  %d SZ =  %d  I = %d \n",u32Offset/1024,u32Sz/1024,0);
	/*write reg*/
	b[0] = PROG_CMD_E;
	i = 6;
	b[2+i] = (uint8)(u32Sz & 0x000000ff);
	b[3+i] = (uint8)((u32Sz & 0x0000ff00)>>8);
	b[4+i] = (uint8)((u32Sz & 0x00ff0000)>>16);
	b[5+i] = (uint8)((u32Sz & 0xff000000)>>24);
	b[10+i] = (uint8)(u32Offset & 0x000000ff);
	b[11+i] = (uint8)((u32Offset & 0x0000ff00)>>8);
	b[12+i] = (uint8)((u32Offset & 0x00ff0000)>>16);
	b[13+i] = (uint8)((u32Offset & 0xff000000)>>24);
	cmd = b[0];
	nm_write_block(NMI_AHB_SHARE_MEM_BASE + M2M_SHARED_OFFSET, (uint8*)&b, sizeof(b));
	nm_write_reg(NMI_STATE_REG,cmd);
	while(1)
	{
		ret = nm_read_reg_with_ret(NMI_STATE_REG, (uint32 *)&reg);
		if(M2M_SUCCESS != ret) break;
		if(reg != cmd){
			break;
		}
		if(++cnt > TIMEOUT){
			reg = PROG_STATE_ERROR;
			break;
		}
		nm_bsp_sleep(1);
	}

    if(vflash)
        memset(&vflash[u32Offset], 0xFF, u32Sz);

#ifdef PROFILING
	if(M2M_SUCCESS == ret)
	{
		M2M_PRINT("Done\n");
		M2M_PRINT("#Erase time = %f sec\n", (GetTickCount()-t)/1000.0);
	}
#endif
	return ((reg==PROG_STATE_IDLE)?(M2M_SUCCESS):(M2M_ERR_FAIL));
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
*					pointer to virtual flash
*	@return		writing status
*	@author		M.S.M
*	@version	1.0
*/
sint8 programmer_write(uint8* pu8Buf, uint32 u32Offset, uint32 u32Sz, uint8* vflash)
{
#ifdef PROFILING
	uint32 t1 = 0;
	uint32 percent =0;
	uint32 tpercent =0;
#endif
	sint8 ret = M2M_SUCCESS;
	uint32 u32wsz;
	uint32 u32off;
	uint32 u32Blksz;
	u32Blksz = FLASH_BLOCK_SIZE;
	u32off = u32Offset % u32Blksz;
#ifdef PROFILING
	tpercent = (u32Sz/u32Blksz)+((u32Sz%u32Blksz)>0);
	t1 = GetTickCount();
	M2M_PRINT(">Start programming...\n");
#endif
	//M2M_INFO("(OTA)Write FO =  %x SZ =  %d  I = %x \n",u32Offset,u32Sz,0);
	if(u32Sz<=0)
	{
		M2M_PRINT("Data size = %d",u32Sz);
		ret =  M2M_ERR_FAIL;
		goto ERR;
	}

	if (u32off)/*first part of data in the address page*/
	{
		u32wsz = u32Blksz - u32off;
		if(programmer_pp(u32Offset, pu8Buf, (uint32)BSP_MIN(u32Sz, u32wsz))!=M2M_SUCCESS)
		{
			ret =  M2M_ERR_FAIL;
			goto ERR;
		}
		if(vflash)
            memcpy(&vflash[u32Offset], pu8Buf, (uint32)BSP_MIN(u32Sz, u32wsz));
		if (u32Sz < u32wsz) goto EXIT;
		/*return data less than the remaining   bytes in page*/
		pu8Buf += u32wsz;
		u32Offset += u32wsz;
		u32Sz -= u32wsz;
	}
	while (u32Sz > 0)
	{
		u32wsz = BSP_MIN(u32Sz, u32Blksz);
		/*write complete page or the remaining data*/
		if(programmer_pp(u32Offset, pu8Buf, (uint32)u32wsz)!=M2M_SUCCESS)
		{
			ret = M2M_ERR_FAIL;
			goto ERR;
		}
		if(vflash)
            memcpy(&vflash[u32Offset], pu8Buf, (uint32)u32wsz);
		pu8Buf += u32wsz;
		u32Offset += u32wsz;
		u32Sz -= u32wsz;
#ifdef PROFILING
		percent++;
		M2M_PRINT("\r>Complete Percentage = %d%%.\r",((percent*100)/tpercent));
#endif
	}
EXIT:
#ifdef PROFILING
	M2M_PRINT("\rDone\t\t\t\t\t\t");
	M2M_PRINT("\n#Programming time = %f sec\r\n\r\n",(GetTickCount() - t1)/1000.0);
#endif
ERR:
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
	uint32 u32BlkSz = FLASH_BLOCK_SIZE;
	sint8 ret = M2M_SUCCESS;
	//M2M_INFO("(OTA)Read FO =  %d SZ =  %d  I = %x \n",u32offset/1024,u32Sz/1024,0);
	if(u32Sz > u32BlkSz)
	{
		do
		{
			ret = programmer_read_internal(pu8Buf, u32offset, u32BlkSz);
			if(M2M_SUCCESS != ret) goto ERR;
			u32Sz -= u32BlkSz;
			u32offset += u32BlkSz;
			pu8Buf += u32BlkSz;
		} while(u32Sz > u32BlkSz);
	}

	ret = programmer_read_internal(pu8Buf, u32offset, u32Sz);

ERR:
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
	sint8 ret = M2M_SUCCESS;
#ifdef CONF_WINC_USE_UART
	if(nm_bus_get_chip_type())
	{
		ret = chip_reset();
	}
	else
	{
		DWORD baudrate;
		baudrate = UART_LOW_RATE;
		ret = nm_bus_iface_reconfigure(&baudrate);
	}
#else
	ret = chip_reset();
#endif
	ret = nm_bus_deinit();
	return ret;
}
#endif
