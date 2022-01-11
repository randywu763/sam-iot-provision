/**
 * \file
 *
 * \brief gcc starttup file for SAMD21
 *
 * Copyright (c) 2013-2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "samd21.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void __libc_init_array(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M0+ core handlers */
void NMI_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
//void HardFault_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
 void HardFault_Handler( void ) __attribute__( ( naked ) );


void SVC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler          ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void PM_Handler              ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void NVMCTRL_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DMAC_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USB_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM4_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM5_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC0_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC1_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC2_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC6_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC7_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AC_Handler              ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PTC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void I2S_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

        /* Configure Initial Stack Pointer, using linker-generated symbols */
        (void*) (&_estack),

        (void*) Reset_Handler,
        (void*) NMI_Handler,
        (void*) HardFault_Handler,
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) SVC_Handler,
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) PendSV_Handler,
        (void*) SysTick_Handler,

        /* Configurable interrupts */
        (void*) PM_Handler,             /*  0 Power Manager */
        (void*) SYSCTRL_Handler,        /*  1 System Control */
        (void*) WDT_Handler,            /*  2 Watchdog Timer */
        (void*) RTC_Handler,            /*  3 Real-Time Counter */
        (void*) EIC_Handler,            /*  4 External Interrupt Controller */
        (void*) NVMCTRL_Handler,        /*  5 Non-Volatile Memory Controller */
        (void*) DMAC_Handler,           /*  6 Direct Memory Access Controller */
        (void*) USB_Handler,            /*  7 Universal Serial Bus */
        (void*) EVSYS_Handler,          /*  8 Event System Interface */
        (void*) SERCOM0_Handler,        /*  9 Serial Communication Interface 0 */
        (void*) SERCOM1_Handler,        /* 10 Serial Communication Interface 1 */
        (void*) SERCOM2_Handler,        /* 11 Serial Communication Interface 2 */
        (void*) SERCOM3_Handler,        /* 12 Serial Communication Interface 3 */
        (void*) SERCOM4_Handler,        /* 13 Serial Communication Interface 4 */
        (void*) SERCOM5_Handler,        /* 14 Serial Communication Interface 5 */
        (void*) TCC0_Handler,           /* 15 Timer Counter Control 0 */
        (void*) TCC1_Handler,           /* 16 Timer Counter Control 1 */
        (void*) TCC2_Handler,           /* 17 Timer Counter Control 2 */
        (void*) TC3_Handler,            /* 18 Basic Timer Counter 0 */
        (void*) TC4_Handler,            /* 19 Basic Timer Counter 1 */
        (void*) TC5_Handler,            /* 20 Basic Timer Counter 2 */
        (void*) TC6_Handler,            /* 21 Basic Timer Counter 3 */
        (void*) TC7_Handler,            /* 22 Basic Timer Counter 4 */
        (void*) ADC_Handler,            /* 23 Analog Digital Converter */
        (void*) AC_Handler,             /* 24 Analog Comparators */
        (void*) DAC_Handler,            /* 25 Digital Analog Converter */
        (void*) PTC_Handler,            /* 26 Peripheral Touch Controller */
        (void*) I2S_Handler             /* 27 Inter-IC Sound Interface */
};

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
        uint32_t *pSrc, *pDest;

        /* Initialize the relocate segment */
        pSrc = &_etext;
        pDest = &_srelocate;

        if (pSrc != pDest) {
                for (; pDest < &_erelocate;) {
                        *pDest++ = *pSrc++;
                }
        }

        /* Clear the zero segment */
        for (pDest = &_szero; pDest < &_ezero;) {
                *pDest++ = 0;
        }

        /* Set the vector table base address */
        pSrc = (uint32_t *) & _sfixed;
        SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

        /* Initialize the C library */
        __libc_init_array();

        /* Branch to main function */
        main();

        /* Infinite loop */
        while (1);
}

/**
 * HardFault_HandlerAsm:
 * Alternative Hard Fault handler to help debug the reason for a fault.
 * To use, edit the vector table to reference this function in the HardFault vector
 * This code is suitable for Cortex-M3 and Cortex-M0 cores
 */

void HardFault_Handler(void){
	/*
	 * Get the appropriate stack pointer, depending on our mode,
	 * and use it as the parameter to the C handler. This function
	 * will never return
	 */

	__asm(	".syntax unified\n"
			"MOVS	R0, #4	\n"
			"MOV	R1, LR	\n"
			"TST	R0, R1	\n"
			"BEQ	_MSP	\n"
			"MRS 	R0, PSP \n"
			"B 	HardFault_HandlerC	\n"
		"_MSP:	\n"
			"MRS 	R0, MSP	\n"
			"B 	HardFault_HandlerC	\n"
		".syntax divided\n") ;
}
void HardFault_HandlerC(unsigned long *hardfault_args);
/**
 * HardFaultHandler_C:
 * This is called from the HardFault_HandlerAsm with a pointer the Fault stack
 * as the parameter. We can then read the values from the stack and place them
 * into local variables for ease of reading.
 * We then read the various Fault Status and Address Registers to help decode
 * cause of the fault.
 * The function ends with a BKPT instruction to force control back into the debugger
 */
void HardFault_HandlerC(unsigned long *hardfault_args){
	volatile unsigned long stacked_r0 ;
	volatile unsigned long stacked_r1 ;
	volatile unsigned long stacked_r2 ;
	volatile unsigned long stacked_r3 ;
	volatile unsigned long stacked_r12 ;
	volatile unsigned long stacked_lr ;
	volatile unsigned long stacked_pc ;
	volatile unsigned long stacked_psr ;
	volatile unsigned long _CFSR ;
	volatile unsigned long _HFSR ;
	volatile unsigned long _DFSR ;
	volatile unsigned long _AFSR ;
	volatile unsigned long _BFAR ;
	volatile unsigned long _MMAR ;

	stacked_r0 = ((unsigned long)hardfault_args[0]) ;
	stacked_r1 = ((unsigned long)hardfault_args[1]) ;
	stacked_r2 = ((unsigned long)hardfault_args[2]) ;
	stacked_r3 = ((unsigned long)hardfault_args[3]) ;
	stacked_r12 = ((unsigned long)hardfault_args[4]) ;
	stacked_lr = ((unsigned long)hardfault_args[5]) ;
	stacked_pc = ((unsigned long)hardfault_args[6]) ;
	stacked_psr = ((unsigned long)hardfault_args[7]) ;

        // Configurable Fault Status Register
        // Consists of MMSR, BFSR and UFSR
	_CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;	
											
	// Hard Fault Status Register
	_HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;

	// Debug Fault Status Register
	_DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;

	// Auxiliary Fault Status Register
	_AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;

	// Read the Fault Address Registers. These may not contain valid values.
	// Check BFARVALID/MMARVALID to see if they are valid values
	// MemManage Fault Address Register
	_MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
	// Bus Fault Address Register
	_BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;

	__asm("BKPT #0\n") ; // Break into the debugger

}

void Dummy_Handler(void)
{
        while (1) {
        }
}
