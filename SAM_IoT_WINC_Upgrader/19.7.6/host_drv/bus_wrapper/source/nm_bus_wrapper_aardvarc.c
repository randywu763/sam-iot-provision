/**
*  @file		nm_bus_wrapper_aardvarc.c				
*  @brief		This module contains NMC1000 bus_wrapper APIs implementation for Aardvarc
*  @author		M. Abdelmawla
*  @date		10 JULY 2012
*  @version		1.0	
*/
#ifdef WIN32
#ifndef CONF_WINC_USE_UART 

#include "bus_wrapper\include\nm_bus_wrapper.h"
#include "bus_wrapper\source\aardvarc\aardvark.h"

#include "windows.h"

#define I2C_ADDR	0x60

#define NM_BUS_MAX_TRX_SZ	40*1024UL


tstrNmBusCapabilities egstrNmBusCapabilities = 
{
	NM_BUS_MAX_TRX_SZ	
};

static Aardvark haa;
static int already_init = 0;
int num_aadevices;	

static void nmi_aardvark_deinit(void)
{
	if (haa > 0) {
		aa_close(haa);
		haa = -1;
	}
}
static int nmi_aardvark_init(void)
{
	
	aa_u16 device_lists[10];	
	int result = -10;
	int index;

	num_aadevices = aa_find_devices(10, device_lists);
	if (num_aadevices <= 0) 
		goto _fail_;


	haa = -1; 

	for(index=0; index<num_aadevices; index++)
	{
		Aardvark temp;	

		temp = aa_open(device_lists[index]);
		if (temp < 0)
			continue;

		result = aa_configure(temp, AA_CONFIG_SPI_I2C);
		if (result < 0) {
			goto _fail_;
		}

		result = aa_i2c_bitrate(temp, 600);
		if (result < 0) {
			goto _fail_;
		}

		result = aa_spi_master_ss_polarity(temp,AA_SPI_SS_ACTIVE_LOW);
		if (result < 0) {
			goto _fail_;
		}

		result = aa_spi_configure(temp, AA_SPI_POL_FALLING_RISING, AA_SPI_PHASE_SAMPLE_SETUP, AA_SPI_BITORDER_MSB);
		if (result < 0) {
			goto _fail_;
		}

		/* SPI clock change */
		result = aa_spi_bitrate(temp, 10000);
		if (result < 0) {
			goto _fail_;
		}
		if(temp < 0) 
		{
			goto _fail_;
		}	
		else
		{
			haa = temp;
			break;
		
		}
	}
	if(result == -10)
	{
		printf("Ardvark in use\n");
	}
	return result;
		
_fail_:
	result = -1;
	return result;
}

static sint8 nm_i2c_write(uint8 *b, uint16 sz)
{
	sint8 result = M2M_SUCCESS;

	if(aa_i2c_write(haa, I2C_ADDR, AA_I2C_NO_FLAGS, (aa_u16)sz, b) <= 0)
	{
		result = M2M_ERR_BUS_FAIL;
	}

	return result;

}

static sint8 nm_i2c_read(uint8 *rb, uint16 sz)
{
	sint8 result = M2M_SUCCESS;	
	
	if(aa_i2c_read(haa, I2C_ADDR, AA_I2C_NO_FLAGS, (aa_u16)sz, rb) <= 0 )
	{
		result = M2M_ERR_BUS_FAIL;
	}

	return result;
}

static sint8 nm_i2c_write_special(uint8 *wb1, uint16 sz1, uint8 *wb2, uint16 sz2)
{
	static uint8 tmp[NM_BUS_MAX_TRX_SZ];
	m2m_memcpy(tmp, wb1, sz1);
	m2m_memcpy(&tmp[sz1], wb2, sz2);
	return nm_i2c_write(tmp, sz1+sz2);
}

static sint8 nm_spi_rw(uint8 *wb, uint8 *rb, uint16 sz)
{
	static uint8 dummy[NM_BUS_MAX_TRX_SZ];
	sint8 result = M2M_SUCCESS;
	uint8 *pw, *pr;

	m2m_memset(dummy, 0, sizeof(dummy));

	if(!wb && !rb)
	{
		M2M_ERR("wb & rb both are null");
		result = M2M_ERR_BUS_FAIL;

	}
	if(wb) pw = wb;
	else pw = dummy;
	if(rb) pr = rb;
	else pr = dummy;

	if(aa_spi_write(haa, (aa_u16)sz, pw,(aa_u16)sz , pr) <= 0 )
	{
		result = M2M_ERR_BUS_FAIL;
	}	
	return result;
}

/*
*	@fn		nm_bus_init
*	@brief	Initialize the bus wrapper
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author	M. Abdelmawla
*	@date	10 July 2012
*	@version	1.0
*/ 
sint8 nm_bus_init(void *pvInitValue)
{
	sint8 result	= M2M_SUCCESS;

	if (!already_init) {
		if (nmi_aardvark_init() < 0) {
			result = M2M_ERR_BUS_FAIL;
		} else {
			already_init = 1;			
		}
	} 

	return result;
}

/*
*	@fn		nm_bus_ioctl
*	@brief	send/receive from the bus
*	@param [in]	u8Cmd
*					IOCTL command for the operation
*	@param [in]	pvParameter
*					Arbitrary parameter depending on IOCTL
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author	M. Abdelmawla
*	@note	For SPI only, it's important to be able to send/receive at the same time
*	@date	10 July 2012
*	@version	1.0
*/ 
sint8 nm_bus_ioctl(uint8 u8Cmd, void* pvParameter)
{
	sint8 s8Ret = 0;
	switch(u8Cmd)
	{
	case NM_BUS_IOCTL_R:
		{
			tstrNmI2cDefault *pstrParam = (tstrNmI2cDefault *)pvParameter;
			s8Ret = nm_i2c_read(pstrParam->pu8Buf, pstrParam->u16Sz);
		}
		break;
	case NM_BUS_IOCTL_W:
		{
			tstrNmI2cDefault *pstrParam = (tstrNmI2cDefault *)pvParameter;
			s8Ret = nm_i2c_write(pstrParam->pu8Buf, pstrParam->u16Sz);
		}
		break;
	case NM_BUS_IOCTL_W_SPECIAL:
		{
			tstrNmI2cSpecial *pstrParam = (tstrNmI2cSpecial *)pvParameter;
			s8Ret = nm_i2c_write_special(pstrParam->pu8Buf1, pstrParam->u16Sz1, pstrParam->pu8Buf2, pstrParam->u16Sz2);
		}
		break;
	case NM_BUS_IOCTL_RW:
		{
			tstrNmSpiRw *pstrParam = (tstrNmSpiRw *)pvParameter;
			s8Ret = nm_spi_rw(pstrParam->pu8InBuf, pstrParam->pu8OutBuf, pstrParam->u16Sz);
		}
		break;
	default:
		s8Ret = -1;
		M2M_ERR("invalid ioclt cmd\n");
		break;
	}
	return s8Ret;
}

/*
*	@fn		nm_bus_deinit
*	@brief	De-initialize the bus wrapper
*	@author	M. Abdelmawla
*	@date	10 July 2012
*	@version	1.0
*/ 
sint8 nm_bus_deinit(void)
{
	if (haa > 0) {
		aa_close(haa);
		haa = -1;
	}
	already_init = 0;
	return 0;
}

/*
*	@fn			nm_bus_reinit
*	@brief		re-initialize the bus wrapper
*	@param [in]	void *config
*					re-init configuration data
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/
sint8 nm_bus_reinit(void * config)
{
	return M2M_SUCCESS;
}

#endif
#endif
