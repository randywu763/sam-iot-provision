/**
*  @file		nm_bus_wrapper_win_uart.c				
*  @brief		This module contains NMC1000 bus_wrapper APIs implementation for Windows Uart
*  @author		Dina El Sissy
*  @date		19 Sep 2012
*  @version		1.0	
*/
#ifdef WIN32
#ifdef CONF_WINC_USE_UART
#include "bus_wrapper\include\nm_bus_wrapper.h"
#include "bsp\include\nm_bsp.h"


#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>


#define NM_BUS_MAX_TRX_SZ	128
#define NM_UART_READ_RETRIES 10

typedef sint8(*tpfCheckPort)(void);

tstrNmBusCapabilities egstrNmBusCapabilities = 
{
	NM_BUS_MAX_TRX_SZ	
};

HANDLE *phUARTPortHandle = 0;
uint8 onchipuart = 0;
uint8 comports[255][3] = {0};
static DCB defaultDCB;

static sint8 nm_uart_reinit(void *pvConfig)
{
	DCB PortDCB;
	sint8 s8Status = M2M_SUCCESS;
	if(pvConfig != NULL)
	{
		if(!GetCommState (*phUARTPortHandle, &PortDCB))
		{
			M2M_ERR("Could not Set Port Settings\n");
			s8Status = M2M_ERR_BUS_FAIL;
		}
		else
		{
			DWORD *baud = (DWORD*)pvConfig;
			PortDCB.BaudRate = *baud; 
			// Configure the port according to the specifications of the DCB 
			// structure.
			if (!SetCommState (*phUARTPortHandle, &PortDCB))
			{
				M2M_ERR("Could not Set Port Settings\n");
				s8Status = M2M_ERR_BUS_FAIL;
			}	

		}
	}
	else
	{
		M2M_ERR("NULL pointer\n");
		s8Status = M2M_ERR_BUS_FAIL;
	}
	return s8Status;
}

#define DELM "(COM"
#define EDBG "EDBG"

int get_all_ports(uint8 * arr)
{
	char buf[300];		   
	//char friendlyName[300];
	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD i;
	int k =0;

	
	// Create a HDEVINFO with all present devices.
	hDevInfo = SetupDiGetClassDevs(NULL,
		0, // Enumerator
		0,
		DIGCF_PRESENT | DIGCF_ALLCLASSES );

	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		// Insert error handling here.
		return 1;
	}

	// Enumerate through all devices in Set.
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,
		&DeviceInfoData);i++)
	{
		DWORD buffersize = 0;



		if(!SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_FRIENDLYNAME, NULL, buf, sizeof(buf), &buffersize)) {	
			DWORD err = GetLastError();
			if (err == ERROR_INSUFFICIENT_BUFFER) {
			} else {
				// Unhandled er
				//friendlyName[0] = '\0';
				continue;
			}
		} else {
			//wcstombs((char*)&friendlyName[0], (wchar_t*)buf, sizeof(friendlyName));
			char * pc;
			char * pe;
			pc = strstr(buf,DELM);
			pe = strstr(buf,EDBG);
			if(pc != NULL)
			{
				//printf("%s,%d\n",pc,atoi(pc+sizeof(DELM)-1));
				arr[k*3] = atoi(pc+sizeof(DELM)-1);
				/* Due to some machines having comm loop back interface e.g. com0com above call to atoi returns 0.
				That causes the calling function to assume that it has reached the end of the array. */
				if (arr[k * 3] == 0)
					continue;
				if(pe != NULL)
				{
					printf("%s\n",pe);
					arr[k*3 + 2] = 1;
					/*flow control for the edbg*/
				}
				k++;
			}
		}

	}
	//  Cleanup
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return 0;
}

/*
*	@fn			nm_uart_get_com_port
*	@brief		get the all the available  uart port
*	@version	1.0
*/ 
static sint8 nm_uart_get_com_port(uint8 * arr)
{
	uint8 commports[255][3];
	uint8 i = 0, k = 0;
	sint8 s8Status = M2M_SUCCESS;
	sint8 portstr[] = "\\\\.\\COM255"; 

	memset(commports,0,sizeof(commports));
	get_all_ports((uint8 *)&commports);
	for(i = 0; (i<255)&&(commports[i][0]!= 0); i++)
	{
		sprintf(&portstr[7], "%d", commports[i][0]);

		/* allocate space for the handle structure */
		phUARTPortHandle = (HANDLE *)malloc(sizeof(HANDLE));

		/*Open Com port */
		*phUARTPortHandle = CreateFile (portstr, // Pointer to the name of the port
			GENERIC_READ | GENERIC_WRITE,
			0,            // Share mode
			NULL,         // Pointer to the security attribute
			OPEN_EXISTING,// How to open the serial port
			0,            // Port attributes
			NULL); 
		if (*phUARTPortHandle== INVALID_HANDLE_VALUE) {
			s8Status = M2M_ERR_BUS_FAIL;
		}
		else
		{
			arr[k*3] = commports[i][0];
			arr[k*3 + 2] = commports[i][2]; /*if edbg port that mean need flow control*/
			k++;
		}
		if(phUARTPortHandle)
		{
			CloseHandle(*phUARTPortHandle);
			free(phUARTPortHandle);
		}
	}
	return s8Status;
}

/*
*	@fn			nm_uart_init
*	@brief		init uart
*	@param [in]	uint8 comnum
*					COM port number
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/ 
static sint8 nm_uart_init(uint8 comnum,uint8 flow)
{
	DCB PortDCB;
	COMMTIMEOUTS timeouts;
	sint8 s8Status = M2M_SUCCESS;
	sint8 portstr[] = "\\\\.\\COM255"; 
	sprintf(&portstr[7], "%d", comnum);

	/* allocate space for the handle structure */
	phUARTPortHandle = (HANDLE *)malloc(sizeof(HANDLE));

	/*Open Com port */
	*phUARTPortHandle = CreateFile (portstr, // Pointer to the name of the port
		GENERIC_READ | GENERIC_WRITE,
		0,            // Share mode
		NULL,         // Pointer to the security attribute
		OPEN_EXISTING,// How to open the serial port
		0,            // Port attributes
		NULL); 
	if (*phUARTPortHandle== INVALID_HANDLE_VALUE) {
		M2M_ERR("Could not open port\n");	
		s8Status = M2M_ERR_BUS_FAIL;
	}
	else
	{
		/*Configure Com port */
		// Initialize the DCBlength member. 
		PortDCB.DCBlength = sizeof (DCB); 

		// Get the default port setting information.
		if(!GetCommState (*phUARTPortHandle, &PortDCB))
		{
			M2M_ERR("Could not Set Port Settings\n");
			s8Status = M2M_ERR_BUS_FAIL;
		}
		else
		{
			memcpy(&defaultDCB,&PortDCB,sizeof(DCB));
			PortDCB.BaudRate = CBR_115200; 
			PortDCB.ByteSize = 8;                 // Number of bits/byte, 4-8 
			PortDCB.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space 
			PortDCB.StopBits = ONESTOPBIT;
			PortDCB.fOutxCtsFlow = 0;
			PortDCB.fDtrControl = 0;
			PortDCB.fRtsControl = 0;
			PortDCB.ByteSize = 0x08;
			PortDCB.fBinary = TRUE;
			PortDCB.fDsrSensitivity = FALSE;                                                                                          
			PortDCB.fTXContinueOnXoff = FALSE;                                                                                        
			PortDCB.fErrorChar = FALSE;                                                                                               
			PortDCB.fNull = FALSE;                                                                                                                                                                                  
			PortDCB.fAbortOnError = FALSE;                                                                                            
			PortDCB.fOutxCtsFlow = FALSE;                                                                                             
			PortDCB.fOutxDsrFlow = FALSE;

			if(flow)
			{
				PortDCB.fInX = TRUE;
				PortDCB.fOutX = TRUE;                                                                                                  
				PortDCB.fDtrControl = DTR_CONTROL_ENABLE;                                                                                 
			}
			else
			{
				PortDCB.fInX = FALSE;
				PortDCB.fOutX = FALSE;                                                                           
				PortDCB.fDtrControl = DTR_CONTROL_DISABLE;                                                                                 
			}

			// Configure the port according to the specifications of the DCB 
			// structure.
			if (!SetCommState (*phUARTPortHandle, &PortDCB))
			{
				M2M_ERR("Could not Set Port Settings\n");
				s8Status = M2M_ERR_BUS_FAIL;
			}	
			else
			{
				if(!GetCommTimeouts(*phUARTPortHandle, &timeouts)) 
				{
					M2M_ERR("Could not Set Port Settings\n");
					s8Status = M2M_ERR_BUS_FAIL;
				}
				else
				{
					ZeroMemory(&timeouts, sizeof(timeouts));
					timeouts.ReadTotalTimeoutConstant = 0x64;
					timeouts.WriteTotalTimeoutConstant = 0x64;
					if(!SetCommTimeouts(*phUARTPortHandle, &timeouts))
					{
						M2M_ERR("Could not Set Port Settings\n");
						s8Status = M2M_ERR_BUS_FAIL;
					}
				}
			}
		}
	}
	return s8Status;
}

/*
*	@fn			nm_uart_read
*	@brief		read from the bus
*	@param [out]uint8 *pu8ReadBuffer
*					Buffer to data
*	@param [in]	sint32 s32ReadSize
*					Size of data
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/ 
static sint8 nm_uart_read(uint8 *pu8ReadBuffer, sint32 s32ReadSize)
{
	sint32 s32i=0;
	sint8 s8Ret = M2M_SUCCESS;
	sint32 s32Result = 0;
	uint32 u32BytesTransferred;
	uint8 u8ReadRetry = 0;

	for(s32i=0;s32i<s32ReadSize;s32i++)
	{
		if (ReadFile(*phUARTPortHandle,		// Port handle
			pu8ReadBuffer+s32i,				// Pointer to data to read
			1,								// Number of bytes to read
			&u32BytesTransferred,			// Pointer to number of bytes read
			NULL							// Must be NULL for Windows CE
			)==TRUE)
		{
			if(u32BytesTransferred > 0)
			{
				s32Result+= u32BytesTransferred;
			}
			else
			{
				if(u8ReadRetry++ > NM_UART_READ_RETRIES)
				{
					break;
				}
				else
				{
					s32i--;
				}
			}
		}
		else
		{
			if(s32Result==0)
			{
				s32Result= -1;
			}
			break;
		}
	}
	if(s32Result == s32ReadSize)
		s8Ret = M2M_SUCCESS;
	else
		s8Ret =  M2M_ERR_BUS_FAIL;

	return s8Ret;
}

/*
*	@fn			nm_uart_write
*	@brief		write to the bus
*	@param [in]	uint8 *pu8Buffer
*					Buffer to data
*	@param [in]	sint32 s32Size
*					Size of data
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/ 
static sint8 nm_uart_write(uint8 *pu8Buffer, sint32 s32Size)
{
	sint8 s8Ret = M2M_SUCCESS;
	sint32 s32Status = 0;
	uint32 u32NumBytesWritten;

	if(WriteFile (*phUARTPortHandle,// Port handle
		pu8Buffer,					// Pointer to the data to write 
		s32Size,					// Number of bytes to write
		&u32NumBytesWritten,		// Pointer to the number of bytes written
		NULL						// Must be NULL for Windows CE
		)!=TRUE)
	{

		M2M_ERR("Error writing to PORT\n");
		s8Ret = M2M_ERR_BUS_FAIL;
	}

	return s8Ret;
}

/*
*	@fn			nm_uart_deinit
*	@brief		deinit the bus
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/ 
static sint8 nm_uart_deinit(void)
{
	if(phUARTPortHandle)
	{
		SetCommState(*phUARTPortHandle,&defaultDCB);
		Sleep(100);
		if(CloseHandle(*phUARTPortHandle)==TRUE)
		{
			free(phUARTPortHandle);
			return M2M_SUCCESS;
		}
		else
		{
			free(phUARTPortHandle);
			return M2M_ERR_BUS_FAIL;
		}
	}
	return M2M_SUCCESS;
}
/*
*	@fn			nm_bus_get_chip_type
*	@brief		get chip type
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		M.S.M
*	@date		19 Sept 2012
*	@version	1.0
*/ 
uint8 nm_bus_get_chip_type(void)
{
	return onchipuart;

}
/*
*	@fn			nm_bus_break
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		M.S.M
*	@date		19 Sept 2012
*	@version	1.0
*/ 
sint8 nm_bus_break(void)
{
	SetCommBreak(*phUARTPortHandle);
	M2M_PRINT("Please reset the WINC1500 to detect the break command and then \n");
	system("pause");
	ClearCommBreak(*phUARTPortHandle);
	return 0;
}
/*
*	@fn			nm_bus_init
*	@brief		Initialize the bus wrapper
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/ 
sint8 nm_bus_init(void *pvPortNum)
{
	uint32 i;
	sint8 result = M2M_SUCCESS;

	if(0 == *((uint8*)pvPortNum))
	{
		M2M_PRINT("Enter port number: \n");
		scanf("%hhu", (uint8*)pvPortNum);
	}
	for(i=0;i<255,comports[i][0]!=0;i++)
	{
		if(*((uint8 *)pvPortNum) == comports[i][0])
		{
			onchipuart = comports[i][1];
			break;
		}
	}
	if(*((uint8*)pvPortNum)>0 && *((uint8*)pvPortNum) <=256)
	{
		result = nm_uart_init(*((uint8*)pvPortNum),comports[i][2]);
	}
	else
	{
		M2M_PRINT(">Invalid port number\n");
		result = M2M_ERR_BUS_FAIL;
	}
	return result;
}

/*
*	@fn			nm_bus_ioctl
*	@brief		send/receive from the bus
*	@param [in]	u8Cmd
*					IOCTL command for the operation
*	@param [in]	pvParameter
*					Arbitrary parameter depending on IOCTL
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/ 
sint8 nm_bus_ioctl(uint8 u8Cmd, void* pvParameter)
{
	sint8 s8Ret = 0;
	switch(u8Cmd)
	{
	case NM_BUS_IOCTL_R:
		{
			tstrNmUartDefault *pstrParam = (tstrNmUartDefault *)pvParameter;
			s8Ret = nm_uart_read(pstrParam->pu8Buf, pstrParam ->u16Sz);
		}
		break;
	case NM_BUS_IOCTL_W:
		{
			tstrNmUartDefault *pstrParam = (tstrNmUartDefault *)pvParameter;
			s8Ret = nm_uart_write(pstrParam->pu8Buf, pstrParam ->u16Sz);
		}
		break;
	default:
		s8Ret = M2M_ERR_BUS_FAIL;
		M2M_ERR("invalid ioclt cmd\n");
		break;
	}
	return s8Ret;
}

/*
*	@fn			nm_bus_deinit
*	@brief		De-initialize the bus wrapper
*	@author		Dina El Sissy
*	@date		20 Sept 2012
*	@version	1.0
*/ 
sint8 nm_bus_deinit(void)
{
	return nm_uart_deinit();
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
sint8 nm_bus_reinit(void* config)
{
	return nm_uart_reinit(config);
}

/*
*	@fn			port_detect
*	@brief		detecting COM port
*	@param [in]	uint8 *avail
*					pointer to available ports
*	@param [in]	tpfCheckPort pfChkPort
*					pointer to call back function (nm_uart_check_port) to check 
*	@return		port number in case of success and 0 in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/
uint8 nm_bus_port_detect(uint8 * avail, tpfCheckPort pfChkPort)
{
	sint8 uart_type = 0;
	sint8 ret = 0;
	int i;
	int k = 0;
	
	printf("Detecting ports...\n");
	nm_uart_get_com_port((uint8*)comports);
	for(i = 0; (i<255)&&(comports[i][0] != 0); i++)
	{	
		ret = nm_bus_init((uint8 *)&comports[i][0]);
		if(ret != M2M_SUCCESS)
		{
			M2M_PRINT(">>(ERR):Connect uart\n");
			continue;
		}
		uart_type =  pfChkPort();
		if (uart_type >= 0) {
			comports[i][1] = uart_type;
			avail[k] = comports[i][0];
			printf("Avail port COM%d\n",avail[k]);
			k++;
		} else {
		}
		nm_bus_deinit();
	}
	if(k == 0)
	{
		M2M_ERR("Failed to find any COM ports\n");
	}
	printf("%d of ports found\n",k);
	return k;
}


#endif /*CONF_WINC_USE_UART*/
#endif
