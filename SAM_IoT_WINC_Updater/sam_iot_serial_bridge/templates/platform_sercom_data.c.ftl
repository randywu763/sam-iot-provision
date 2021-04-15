#include "definitions.h"
#include "platform_sercom_data.h"

const PLATFORM_USART_PLIB_INTERFACE platformUsartPlibAPI = {
    .initialize             = ${.vars["${APP_USART_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_Initialize,
    .readCallbackRegister   = ${.vars["${APP_USART_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_ReadCallbackRegister,
    .read                   = ${.vars["${APP_USART_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_Read,
    .readIsBusy             = ${.vars["${APP_USART_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_ReadIsBusy,
    .readCountGet           = ${.vars["${APP_USART_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_ReadCountGet,
    .writeCallbackRegister  = ${.vars["${APP_USART_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_WriteCallbackRegister,
    .write                  = ${.vars["${APP_USART_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_Write,
    .writeIsBusy            = ${.vars["${APP_USART_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_WriteIsBusy,
    .writeCountGet          = ${.vars["${APP_USART_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_WriteCountGet,
    .errorGet               = ${.vars["${APP_USART_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_ErrorGet,
    .serialSetup            = ${.vars["${APP_USART_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_SerialSetup
};

<#assign USART_PLIB = "APP_USART_PLIB">
<#assign APP_USART_PLIB_MULTI_IRQn = "core." + USART_PLIB?eval + "_MULTI_IRQn">
<#assign APP_USART_PLIB_SINGLE_IRQn = "core." + USART_PLIB?eval + "_SINGLE_IRQn">
<#if APP_USART_PLIB_MULTI_IRQn?eval??>
        <#assign APP_USART_PLIB_TX_COMPLETE_INDEX = "core." + USART_PLIB?eval + "_USART_TX_COMPLETE_INT_SRC">
        <#assign APP_USART_PLIB_TX_READY_INDEX = "core." + USART_PLIB?eval + "_USART_TX_READY_INT_SRC">
        <#assign APP_USART_PLIB_RX_INDEX = "core." + USART_PLIB?eval + "_USART_RX_INT_SRC">
        <#assign APP_USART_PLIB_ERROR_INDEX = "core." + USART_PLIB?eval + "_USART_ERROR_INT_SRC">
</#if>
	
const DRV_USART_INTERRUPT_SOURCES platformInterruptSources =
{	
<#if APP_USART_PLIB_MULTI_IRQn?eval??>
        <#lt>    /* Peripheral has more than one interrupt vector */
        <#lt>    .isSingleIntSrc                        = false,
 
        <#lt>    /* Peripheral interrupt lines */
        <#if APP_USART_PLIB_TX_COMPLETE_INDEX?eval??>
            <#lt>    .intSources.multi.usartTxCompleteInt   = ${APP_USART_PLIB_TX_COMPLETE_INDEX?eval},
        <#else>
            <#lt>    .intSources.multi.usartTxCompleteInt   = -1,
        </#if>
        <#if APP_USART_PLIB_TX_READY_INDEX?eval??>
            <#lt>    .intSources.multi.usartTxReadyInt      = ${APP_USART_PLIB_TX_READY_INDEX?eval},
        <#else>
            <#lt>    .intSources.multi.usartTxReadyInt      = -1,
        </#if>
        <#if APP_USART_PLIB_RX_INDEX?eval??>
            <#lt>    .intSources.multi.usartRxCompleteInt   = ${APP_USART_PLIB_RX_INDEX?eval},
        <#else>
            <#lt>    .intSources.multi.usartTxReadyInt      = -1,
        </#if>
        <#if APP_USART_PLIB_ERROR_INDEX?eval??>
            <#lt>    .intSources.multi.usartErrorInt        = ${APP_USART_PLIB_ERROR_INDEX?eval},
        <#else>
            <#lt>    .intSources.multi.usartErrorInt        = -1,
        </#if>
    <#else>
        <#lt>    /* Peripheral has single interrupt vector */
        <#lt>    .isSingleIntSrc                        = true,
 
        <#lt>    /* Peripheral interrupt line */
        <#if APP_USART_PLIB_SINGLE_IRQn?eval??>
            <#lt>    .intSources.usartInterrupt             = ${APP_USART_PLIB_SINGLE_IRQn?eval},
        <#else>
            <#lt>    .intSources.usartInterrupt             = ${APP_USART_PLIB}_IRQn,
        </#if>
    </#if>
};