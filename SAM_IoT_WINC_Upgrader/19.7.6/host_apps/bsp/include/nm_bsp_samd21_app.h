/*
* nm_bsp_samd21.h
*
*  Created on: oct 28, 2013
*      Author: MSM
*/

#ifndef _NM_BSP_SAMD21_APP_H_
#define _NM_BSP_SAMD21_APP_H_

#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include <asf.h>


#define M2M_PRINTX(x)  nm_bsp_uart_send((const uint8_t *)x,sizeof(x))
/**
*Extern global variables
*
*/
extern uint32 gu32Jiffies;
extern uint32 gu32Jiffies1ms;
#ifdef __cplusplus
extern "C"{
#endif

/*
*
*/
#define SW1		NBIT0
#define SW2		NBIT1
/**/

#define NM_BSP_PERM_FIRMWARE_SIZE	(1024UL*256)	/* Permanent storage size available for the firmware */

#define TICK_RES							20		/*!< Tick resolution in milliseconds */
#define TICK_RES_SLEEP						20		/*it must be equal tick or higher */
#define NM_BSP_TIME_MSEC			(gu32Jiffies * TICK_RES)
/**
*
*Callback functions
*/
typedef void (*tpfNmBspBtnPress)(uint8 u8Btn, uint8 u8Type);
typedef void (*tpfNmBspTimerCb)(void);

/**
*	@fn		nm_bsp_init
*	@brief	Initialize BSP
*	@return	0 in case of success and -1 in case of failure
*	@author	M. Abdelmawla
*	@date	11 July 2012
*	@version	1.0
*/ 
sint8 nm_bsp_app_init(void);

/*
*	@fn			nm_bsp_app_configurable_timer_init
*	@brief		Initialize the Configurable Timer
*	@date		08 October 2015
*	@version	1.0
*/
void nm_bsp_app_configurable_timer_init(uint32_t u32Period);

/**
*	@fn		nm_bsp_deinit
*	@brief	De-iInitialize BSP
*	@return	0 in case of success and -1 in case of failure
*	@author	M. Abdelmawla
*	@date	11 July 2012
*	@version	1.0
*/ 
sint8 nm_bsp_app_deinit(void);

/**
*	@fn		nm_bsp_btn_init
*	@brief	Initialize buttons driver
*	@author	M.S.M
*	@date	28 oct 2013
*	@version	1.0
*/
void nm_bsp_uart_send(const uint8 *pu8Buf, uint16 u16Sz);
/**
*	@fn		nm_bsp_btn_init
*	@brief	Initialize buttons driver
*	@author	M.S.M
*	@date	28 oct 2013
*	@version	1.0
*/
void nm_bsp_btn_init(tpfNmBspBtnPress pfBtnCb);
#ifdef _STATIC_PS_
/**
*	@fn		nm_bsp_register_wake_isr
*	@brief	REGISTER wake up timer 
*	@author	M.S.M
*	@date	28 oct 2013
*	@version	1.0
*/
void nm_bsp_register_wake_isr(tpfNmBspIsr pfIsr,uint32 u32MsPeriod);
/**
*	@fn		nm_bsp_wake_ctrl
*	@brief	control wake up timer
*	@author	M.S.M
*	@date	28 oct 2013
*	@version	1.0
*/
void nm_bsp_wake_ctrl(uint8 en);

#endif
#if (defined _STATIC_PS_)||(defined _DYNAMIC_PS_)
/**
*	@fn		nm_bsp_enable_mcu_ps
*	@brief	Start POWER SAVE FOR MCU 
*	@author	M.S.M
*	@date	28 oct 2013
*	@version	1.0
*/
void nm_bsp_enable_mcu_ps(void);
#endif

/**
*	@fn		nm_bsp_start_timer
*	@brief	Start periodic timer
*	@author	M.S.M
*	@date	28 oct 2013
*	@version	1.0
*/
void nm_bsp_start_timer(tpfNmBspTimerCb pfCb, uint32 u32Period);

/*
*	@fn			nm_bsp_start_1ms_timer
*	@brief		Start 1ms timer
*	@date		08 October 2015
*	@version	1.0
*/
void nm_bsp_start_1ms_timer(tpfNmBspTimerCb pfCb);

/*
*	@fn			nm_bsp_start_configurable_timer
*	@brief		Start configurable timer
*	@date		08 October 2015
*	@version	1.0
*/
void nm_bsp_start_configurable_timer(tpfNmBspTimerCb pfCb);

/**
*	@fn		nm_bsp_stop_timer
*	@brief	Start periodic timer
*	@author	M.S.M
*	@date	28 oct 2013
*	@version	1.0
*/
void nm_bsp_stop_timer(void);

/*
*	@fn			nm_bsp_stop_1ms_timer
*	@brief		Stop 1ms timer
*	@date		08 October 2015
*	@version	1.0
*/
void nm_bsp_stop_1ms_timer(void);

/*
*	@fn			nm_bsp_stop_configurable_timer
*	@brief		Stop configurable timer
*	@date		08 October 2015
*	@version	1.0
*/
void nm_bsp_stop_configurable_timer(void);


#ifdef __cplusplus
}
#endif
#endif /* _NM_BSP_SAMD21_H_ */
