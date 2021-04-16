/* Auto-generated config file cryptoauthlib_config.h */
#ifndef CRYPTOAUTHLIB_CONFIG_H
#define CRYPTOAUTHLIB_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Basic configuration

// <hidden>

// <e>enable Polling
// <i> enables polling code
// <id> cal_polling
#ifndef CONF_CRYPTO_AUTHENTICATION_0_EN_POLL
#define CONF_CRYPTO_AUTHENTICATION_0_EN_POLL 0
#endif

// <o> Poll - Initial Wait time (ms)<0-13000>
// <i> Configure wait time before first response check
// <id> cal_init_time
#ifndef CONF_CRYPTO_AUTHENTICATION_0_INIT_TIME
#define CONF_CRYPTO_AUTHENTICATION_0_INIT_TIME 1
#endif

// <o> Poll - Frequency (ms)<0-13000>
// <i> Configure period between two consecutive responses
// <id> cal_freq
#ifndef CONF_CRYPTO_AUTHENTICATION_0_FREQ
#define CONF_CRYPTO_AUTHENTICATION_0_FREQ 2
#endif

// <o> Poll - Maximum Time (ms)<0-13000>
// <i> Configure maximum poll time before declaring no response
// <id> cal_max_time
#ifndef CONF_CRYPTO_AUTHENTICATION_0_MAX_TIME
#define CONF_CRYPTO_AUTHENTICATION_0_MAX_TIME 2500
#endif

// </e>
// </hidden>

// <q>enable heap
// <i> Select to use dynamic memory allocations
// <id> cal_heap
#ifndef CONF_CRYPTO_AUTHENTICATION_0_EN_HEAP
#define CONF_CRYPTO_AUTHENTICATION_0_EN_HEAP 1
#endif

// <q>enable debug print
// <i> Enable Debug Print messages
// <id> cal_printf
#ifndef CONF_CRYPTO_AUTHENTICATION_0_EN_PRINTF
#define CONF_CRYPTO_AUTHENTICATION_0_EN_PRINTF 1
#endif

// </h>

#if CONF_CRYPTO_AUTHENTICATION_0_EN_POLL == 0
#ifndef ATCA_NO_POLL
#define ATCA_NO_POLL
#endif
#else
#define ATCA_POLLING_INIT_TIME_MSEC CONF_CRYPTO_AUTHENTICATION_0_INIT_TIME
#define ATCA_POLLING_FREQUENCY_TIME_MSEC CONF_CRYPTO_AUTHENTICATION_0_FREQ
#define ATCA_POLLING_MAX_TIME_MSEC CONF_CRYPTO_AUTHENTICATION_0_MAX_TIME
#endif

#if CONF_CRYPTO_AUTHENTICATION_0_EN_PRINTF == 1
#ifndef ATCAPRINTF
#define ATCAPRINTF
#endif
#endif

#if CONF_CRYPTO_AUTHENTICATION_0_EN_HEAP == 0
#ifndef ATCA_NO_HEAP
#define ATCA_NO_HEAP
#endif
#endif

/* Enable HAL I2C */
#ifndef ATCA_HAL_I2C
#define ATCA_HAL_I2C
#endif

// <<< end of configuration section >>>

#endif // CRYPTOAUTHLIB_CONFIG_H
