/**
*  @file		nm_bsp_win32.c
*  @brief		This module contains NMC1000 Win32 bsp APIs implementation 
*  @author		M. Abdelmawla
*  @date		10 JULY 2012
*  @version		1.0	
*/
#ifdef WIN32
#include "bsp/include/nm_bsp.h"
#include "bsp/include/nm_bsp_internal.h"
#include "common/include/nm_common.h"
#include "windows.h"

tpfNmBspIsr gpfIsr = NULL;
/**
*	@fn		nm_bsp_init
*	@brief	Initialize BSP
*	@return	0 in case of success and -1 in case of failure
*	@author	M. Abdelmawla
*	@date	11 July 2012
*	@version	1.0
*/ 
sint8 nm_bsp_init(void)
{
	return M2M_SUCCESS;
}

/*
*	@fn		nm_bsp_sleep
*	@brief	Sleep in units of mSec
*	@param [in]	u32TimeMsec
*				Time in milliseconds
*	@author	M. Abdelmawla
*	@date	15 July 2012
*	@version	1.0
*/ 
void nm_bsp_sleep(uint32 u32TimeMsec)
{
	Sleep(u32TimeMsec);
}

/*
*	@fn		nm_bsp_buf_read
*	@brief	Read from permanent storage buffer
*	@param [in]	enuType
*				Buffer type 
*	@param [out]	pu8Buf
*				Pointer to buffer used to copy the read data form the permanent storage.
*				Must be allocated by the caller
*	@param [in]	u32Offset
*				Start read offset with in the permanent storage buffer
*	@param [in]	u32Sz
*				The requested read size
*	@return	0 in case of success and -1 in case of failure
*	@author	M. Abdelmawla
*	@date	15 July 2012
*	@sa		tenuNmBspBufType
*	@version	1.0
*/ 
sint8 nm_bsp_buf_read(tenuNmBspBufType enuType, uint8* pu8Buf, uint32 u32Offset, uint32 u32Sz)
{
	switch(enuType)
	{
	case NM_BSP_BUF_FIRMWARE:
		break;
	case NM_BSP_BUF_CONFIG:
	default:
		NM_BSP_PRINTF("invalid buffer type\n");
	}
	return 0;
}

/*
*	@fn		nm_bsp_buf_write
*	@brief	write permanent storage buffer
*	@param [in]	enuType
*				Buffer type 
*	@param [out]	pu8Buf
*				Pointer to buffer used to copy the read data form the permanent storage.
*				Must be allocated by the caller
*	@param [in]	u32Offset
*				Start read offset with in the permanent storage buffer
*	@param [in]	u32Sz
*				The requested read size
*	@return	0 in case of success and -1 in case of failure
*	@author	M. Abdelmawla
*	@date	15 July 2012
*	@sa		tenuNmBspBufType
*	@version	1.0
*/ 
sint8 nm_bsp_buf_write(tenuNmBspBufType enuType, uint8* pu8Buf, uint32 u32Offset, uint32 u32Sz)
{
	return 0;
}

/*
*	@fn		nm_bsp_buf_erase
*	@brief	erase storage buffer	(set to 0xff)
*	@param [in]	enuType
*				Buffer type
*	@return	0 in case of success and -1 in case of failure
*	@author	M. Abdelmawla
*	@date	15 July 2012
*	@sa		tenuNmBspBufType
*	@version	1.0
*/ 
sint8 nm_bsp_buf_erase(tenuNmBspBufType enuType)
{
	return 0;
}

/**
*	@fn		nm_bsp_register_isr
*	@brief	Register interrupt service routine
*	@param [in]	pfIsr
*				Pointer to ISR handler
*	@author	M. Abdelmawla
*	@date	29 July 2012
*	@sa		tpfNmBspIsr
*	@version	1.0
*/
void nm_bsp_register_isr(tpfNmBspIsr pfIsr)
{
	gpfIsr = pfIsr;
}

void nm_bsp_poll(void)
{
	if(gpfIsr)
	gpfIsr();
}
void nm_bsp_interrupt_ctrl(uint8 u8Enable)
{
}

uint32 nm_bsp_get_tick()
{
	return (uint32)GetTickCount();
}
#endif
