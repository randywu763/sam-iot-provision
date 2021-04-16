#include "definitions.h"
#include "platform_sercom_data.h"

const PLATFORM_USART_PLIB_INTERFACE platformUsartPlibAPI = {
    .initialize             = SERCOM5_USART_Initialize,
    .readCallbackRegister   = SERCOM5_USART_ReadCallbackRegister,
    .read                   = SERCOM5_USART_Read,
    .readIsBusy             = SERCOM5_USART_ReadIsBusy,
    .readCountGet           = SERCOM5_USART_ReadCountGet,
    .writeCallbackRegister  = SERCOM5_USART_WriteCallbackRegister,
    .write                  = SERCOM5_USART_Write,
    .writeIsBusy            = SERCOM5_USART_WriteIsBusy,
    .writeCountGet          = SERCOM5_USART_WriteCountGet,
    .errorGet               = SERCOM5_USART_ErrorGet,
    .serialSetup            = SERCOM5_USART_SerialSetup
};

	
const DRV_USART_INTERRUPT_SOURCES platformInterruptSources =
{	
    /* Peripheral has single interrupt vector */
    .isSingleIntSrc                        = true,
 
    /* Peripheral interrupt line */
    .intSources.usartInterrupt             = SERCOM5_IRQn,
};