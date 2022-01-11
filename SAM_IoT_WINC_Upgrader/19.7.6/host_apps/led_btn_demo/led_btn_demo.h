#include "m2m_test_config.h"

#ifndef _LED_BTN_DEMO_H_
#define _LED_BTN_DEMO_H_

#ifdef LED_BTN_DEMO

typedef void (*tpfLedBtnDemoCb)(uint8 cmd);

#ifdef __cplusplus
extern "C"{
#endif

extern void LedBtnDemo_AppStart(tpfLedBtnDemoCb pfLedBtnDemoCb);
extern void LedBtnDemo_AppStop(void);
extern void LedBtnDemo_AppSendNotification(char *pcMsg);

#ifdef __cplusplus
}
#endif

#endif

#endif 
