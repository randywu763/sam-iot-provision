/**
*  @file		nm_bsp_win32.h				
*  @brief		This module contains NMC1000 Win32 bsp APIs declarations 
*  @author		M. Abdelmawla
*  @date		10 JULY 2012
*  @version		1.0	
*/
#ifndef __NM_BSP_APS3_CORTUS_H__
#define __NM_BSP_APS3_CORTUS_H__

#ifdef __APP_APS3_CORTUS__

extern int (*volatile app_printf)(const char* format, ...);

#define M2M_USR_APP (noinline,externally_visible,section(".sec_app"))


#ifdef NMI_API
#undef NMI_API
#endif
#define NMI_API			__attribute__(M2M_USR_APP)



#define CONF_WINC_PRINTF 	app_printf
#define CONF_WINC_DEBUG		1

#define NM_DEBUG				CONF_WINC_DEBUG
#define NM_BSP_PRINTF			CONF_WINC_PRINTF

#endif

#endif	/*__NM_BSP_APS3_CORTUS_H__ */
