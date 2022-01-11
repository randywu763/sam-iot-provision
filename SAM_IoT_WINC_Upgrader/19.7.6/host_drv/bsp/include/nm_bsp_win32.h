/**
*  @file		nm_bsp_win32.h				
*  @brief		This module contains NMC1000 Win32 bsp APIs declarations 
*  @author		M. Abdelmawla
*  @date		10 JULY 2012
*  @version		1.0	
*/
#ifndef _NM_BSP_WIN32_H_
#define _NM_BSP_WIN32_H_
#include <stdio.h>
#include <math.h>

#define BSP_MAX_BUF_CPY_SZ						256	
/*!< This depends on available RAM for copying.
*/
	
/*!< Permanent storage size available for
	the firmware. 8 KB of the total size are
	left for internal usage by firmware.
*/

#define CONF_WINC_PRINTF printf
#ifdef _DEBUG
#define CONF_WINC_DEBUG	1
#else
#define CONF_WINC_DEBUG	0
#endif

#define NM_DEBUG				CONF_WINC_DEBUG
#define NM_BSP_PRINTF			CONF_WINC_PRINTF
/**
*	@enum		tenuNmBspBufType
*	@brief		Enumeration to convey permanent buffer type
*	@author		M. Abdelmawla
*	@date		15 July 2012
*	@version	1.0
*/ 
typedef enum
{
	NM_BSP_BUF_FIRMWARE,		/*!< Firmware buffer */
	NM_BSP_BUF_CONFIG			/*!< Configuration buffer */
} tenuNmBspBufType;
/**
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
sint8 nm_bsp_buf_read(tenuNmBspBufType enuType, uint8* pu8Buf, uint32 u32Offset, uint32 u32Sz);

/**
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
sint8 nm_bsp_buf_write(tenuNmBspBufType enuType, uint8* pu8Buf, uint32 u32Offset, uint32 u32Sz);

/**
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
sint8 nm_bsp_buf_erase(tenuNmBspBufType enuType);

uint32 nm_bsp_get_tick();
void nm_bsp_poll(void);

#endif	/*_NM_BSP_WIN32_H_*/
