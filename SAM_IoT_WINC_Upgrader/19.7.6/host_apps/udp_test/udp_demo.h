/*!
 @file


 @brief


 @author		NMI
 @date
 @version	1.0
 */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 INCLUDES
 *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#ifndef _UDP_DEMO_H_
#define _UDP_DEMO_H_

#include "m2m_test_config.h"


#ifdef __cplusplus
extern "C"{
#endif

#ifdef UDP_DEMO
extern void UDP_SendBcast(void);
extern void SocketTest(void);
#endif

#ifdef __cplusplus
}
#endif

#endif 
