/* Auto-generated config file cryptoauthlib_interface_config.h */
#ifndef CRYPTOAUTHLIB_INTERFACE_CONFIG_H
#define CRYPTOAUTHLIB_INTERFACE_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Basic configuration

// <o> I2C Address <0x0-0xFE>
// <i> The I2C Address of ATECC108A
// <id> atecc108a_i2c_address
#ifndef CONF_CRYPTO_AUTHENTICATION_INTERFACE_0_I2C_ADDRESS
#define CONF_CRYPTO_AUTHENTICATION_INTERFACE_0_I2C_ADDRESS 0xb0
#endif

// <o> I2C Clock Speed (Hz) <1000-1000000>
// <i> The I2C SCK Clock Frequency
// <id> atecc108a_i2c_frequency
#ifndef CONF_CRYPTO_AUTHENTICATION_INTERFACE_0_I2C_FREQUENCY
#define CONF_CRYPTO_AUTHENTICATION_INTERFACE_0_I2C_FREQUENCY 100000
#endif

// <o> Wakeup Delay (us)<1-10000>
// <i> Microseconds of Wake High Delay(tWHI) to Data Communication
// <id> atecc108a_i2c_wakeup_delay
#ifndef CONF_CRYPTO_AUTHENTICATION_INTERFACE_0_I2C_WAKEUP_DELAY
#define CONF_CRYPTO_AUTHENTICATION_INTERFACE_0_I2C_WAKEUP_DELAY 800
#endif

// <o> Receive Retry <1-100>
// <i> The number of retries to attempt for receiving bytes
// <id> atecc108a_i2c_rxretry
#ifndef CONF_CRYPTO_AUTHENTICATION_INTERFACE_0_I2C_RXRETRY
#define CONF_CRYPTO_AUTHENTICATION_INTERFACE_0_I2C_RXRETRY 20
#endif

// </h>

// <<< end of configuration section >>>

#endif // CRYPTOAUTHLIB_INTERFACE_CONFIG_H
