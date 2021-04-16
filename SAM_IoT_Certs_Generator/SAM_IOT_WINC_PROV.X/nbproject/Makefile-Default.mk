#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Default.mk)" "nbproject/Makefile-local-Default.mk"
include nbproject/Makefile-local-Default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SAM_IOT_WINC_PROV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SAM_IOT_WINC_PROV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=cryptoauthlib/lib/atcacert/atcacert_client.c cryptoauthlib/lib/atcacert/atcacert_date.c cryptoauthlib/lib/atcacert/atcacert_def.c cryptoauthlib/lib/atcacert/atcacert_der.c cryptoauthlib/lib/atcacert/atcacert_host_hw.c cryptoauthlib/lib/atcacert/atcacert_host_sw.c cryptoauthlib/lib/basic/atca_basic.c cryptoauthlib/lib/basic/atca_basic_aes.c cryptoauthlib/lib/basic/atca_basic_aes_cbc.c cryptoauthlib/lib/basic/atca_basic_aes_cmac.c cryptoauthlib/lib/basic/atca_basic_aes_ctr.c cryptoauthlib/lib/basic/atca_basic_aes_gcm.c cryptoauthlib/lib/basic/atca_basic_checkmac.c cryptoauthlib/lib/basic/atca_basic_counter.c cryptoauthlib/lib/basic/atca_basic_derivekey.c cryptoauthlib/lib/basic/atca_basic_ecdh.c cryptoauthlib/lib/basic/atca_basic_gendig.c cryptoauthlib/lib/basic/atca_basic_genkey.c cryptoauthlib/lib/basic/atca_basic_hmac.c cryptoauthlib/lib/basic/atca_basic_info.c cryptoauthlib/lib/basic/atca_basic_kdf.c cryptoauthlib/lib/basic/atca_basic_lock.c cryptoauthlib/lib/basic/atca_basic_mac.c cryptoauthlib/lib/basic/atca_basic_nonce.c cryptoauthlib/lib/basic/atca_basic_privwrite.c cryptoauthlib/lib/basic/atca_basic_random.c cryptoauthlib/lib/basic/atca_basic_read.c cryptoauthlib/lib/basic/atca_basic_secureboot.c cryptoauthlib/lib/basic/atca_basic_selftest.c cryptoauthlib/lib/basic/atca_basic_sha.c cryptoauthlib/lib/basic/atca_basic_sign.c cryptoauthlib/lib/basic/atca_basic_updateextra.c cryptoauthlib/lib/basic/atca_basic_verify.c cryptoauthlib/lib/basic/atca_basic_write.c cryptoauthlib/lib/basic/atca_helpers.c cryptoauthlib/lib/crypto/hashes/sha1_routines.c cryptoauthlib/lib/crypto/hashes/sha2_routines.c cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c cryptoauthlib/lib/hal/atca_hal.c cryptoauthlib/lib/hal/hal_sam_i2c_start.c cryptoauthlib/lib/hal/hal_sam_timer_start.c cryptoauthlib/lib/host/atca_host.c cryptoauthlib/lib/jwt/atca_jwt.c cryptoauthlib/lib/atca_cfgs.c cryptoauthlib/lib/atca_command.c cryptoauthlib/lib/atca_device.c cryptoauthlib/lib/atca_execution.c cryptoauthlib/lib/atca_iface.c examples/driver_examples.c hal/src/hal_atomic.c hal/src/hal_delay.c hal/src/hal_gpio.c hal/src/hal_i2c_m_sync.c hal/src/hal_init.c hal/src/hal_io.c hal/src/hal_sleep.c hal/src/hal_ext_irq.c hal/src/hal_spi_m_sync.c hal/src/hal_usart_sync.c hal/utils/src/utils_assert.c hal/utils/src/utils_event.c hal/utils/src/utils_list.c hal/utils/src/utils_syscalls.c hpl/core/hpl_core_m0plus_base.c hpl/core/hpl_init.c hpl/dmac/hpl_dmac.c hpl/eic/hpl_eic.c hpl/gclk/hpl_gclk.c hpl/pm/hpl_pm.c hpl/sercom/hpl_sercom.c hpl/sysctrl/hpl_sysctrl.c hpl/systick/hpl_systick.c samd21a/gcc/gcc/startup_samd21.c samd21a/gcc/system_samd21.c stdio_redirect/gcc/read.c stdio_redirect/gcc/write.c stdio_redirect/stdio_io.c winc1500/bsp/source/nm_bsp.c winc1500/bus_wrapper/source/nm_bus_wrapper.c winc1500/common/source/nm_common.c winc1500/driver/source/m2m_ate_mode.c winc1500/driver/source/m2m_crypto.c winc1500/driver/source/m2m_hif.c winc1500/driver/source/m2m_ota.c winc1500/driver/source/m2m_periph.c winc1500/driver/source/m2m_ssl.c winc1500/driver/source/m2m_wifi.c winc1500/driver/source/nmasic.c winc1500/driver/source/nmbus.c winc1500/driver/source/nmdrv.c winc1500/driver/source/nmi2c.c winc1500/driver/source/nmspi.c winc1500/driver/source/nmuart.c winc1500/socket/source/socket.c winc1500/spi_flash/source/spi_flash.c main.c driver_init.c stdio_start.c cryptoauthlib_main.c winc_init.c atmel_start.c cert_def_1_signer.c cert_def_2_device.c cert_def_3_device_csr.c cert_def_4_device.c cert_def_4_signer.c convertions.c winc_certs_functions.c samd21a/gcc/gcc/newfile.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o ${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o ${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o ${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o ${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o ${OBJECTDIR}/cryptoauthlib/lib/atca_command.o ${OBJECTDIR}/cryptoauthlib/lib/atca_device.o ${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o ${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o ${OBJECTDIR}/examples/driver_examples.o ${OBJECTDIR}/hal/src/hal_atomic.o ${OBJECTDIR}/hal/src/hal_delay.o ${OBJECTDIR}/hal/src/hal_gpio.o ${OBJECTDIR}/hal/src/hal_i2c_m_sync.o ${OBJECTDIR}/hal/src/hal_init.o ${OBJECTDIR}/hal/src/hal_io.o ${OBJECTDIR}/hal/src/hal_sleep.o ${OBJECTDIR}/hal/src/hal_ext_irq.o ${OBJECTDIR}/hal/src/hal_spi_m_sync.o ${OBJECTDIR}/hal/src/hal_usart_sync.o ${OBJECTDIR}/hal/utils/src/utils_assert.o ${OBJECTDIR}/hal/utils/src/utils_event.o ${OBJECTDIR}/hal/utils/src/utils_list.o ${OBJECTDIR}/hal/utils/src/utils_syscalls.o ${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o ${OBJECTDIR}/hpl/core/hpl_init.o ${OBJECTDIR}/hpl/dmac/hpl_dmac.o ${OBJECTDIR}/hpl/eic/hpl_eic.o ${OBJECTDIR}/hpl/gclk/hpl_gclk.o ${OBJECTDIR}/hpl/pm/hpl_pm.o ${OBJECTDIR}/hpl/sercom/hpl_sercom.o ${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o ${OBJECTDIR}/hpl/systick/hpl_systick.o ${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o ${OBJECTDIR}/samd21a/gcc/system_samd21.o ${OBJECTDIR}/stdio_redirect/gcc/read.o ${OBJECTDIR}/stdio_redirect/gcc/write.o ${OBJECTDIR}/stdio_redirect/stdio_io.o ${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o ${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o ${OBJECTDIR}/winc1500/common/source/nm_common.o ${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o ${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o ${OBJECTDIR}/winc1500/driver/source/m2m_hif.o ${OBJECTDIR}/winc1500/driver/source/m2m_ota.o ${OBJECTDIR}/winc1500/driver/source/m2m_periph.o ${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o ${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o ${OBJECTDIR}/winc1500/driver/source/nmasic.o ${OBJECTDIR}/winc1500/driver/source/nmbus.o ${OBJECTDIR}/winc1500/driver/source/nmdrv.o ${OBJECTDIR}/winc1500/driver/source/nmi2c.o ${OBJECTDIR}/winc1500/driver/source/nmspi.o ${OBJECTDIR}/winc1500/driver/source/nmuart.o ${OBJECTDIR}/winc1500/socket/source/socket.o ${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o ${OBJECTDIR}/main.o ${OBJECTDIR}/driver_init.o ${OBJECTDIR}/stdio_start.o ${OBJECTDIR}/cryptoauthlib_main.o ${OBJECTDIR}/winc_init.o ${OBJECTDIR}/atmel_start.o ${OBJECTDIR}/cert_def_1_signer.o ${OBJECTDIR}/cert_def_2_device.o ${OBJECTDIR}/cert_def_3_device_csr.o ${OBJECTDIR}/cert_def_4_device.o ${OBJECTDIR}/cert_def_4_signer.o ${OBJECTDIR}/convertions.o ${OBJECTDIR}/winc_certs_functions.o ${OBJECTDIR}/samd21a/gcc/gcc/newfile.o
POSSIBLE_DEPFILES=${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o.d ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o.d ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o.d ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o.d ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o.d ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o.d ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d ${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o.d ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o.d ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o.d ${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o.d ${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o.d ${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o.d ${OBJECTDIR}/cryptoauthlib/lib/atca_command.o.d ${OBJECTDIR}/cryptoauthlib/lib/atca_device.o.d ${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o.d ${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o.d ${OBJECTDIR}/examples/driver_examples.o.d ${OBJECTDIR}/hal/src/hal_atomic.o.d ${OBJECTDIR}/hal/src/hal_delay.o.d ${OBJECTDIR}/hal/src/hal_gpio.o.d ${OBJECTDIR}/hal/src/hal_i2c_m_sync.o.d ${OBJECTDIR}/hal/src/hal_init.o.d ${OBJECTDIR}/hal/src/hal_io.o.d ${OBJECTDIR}/hal/src/hal_sleep.o.d ${OBJECTDIR}/hal/src/hal_ext_irq.o.d ${OBJECTDIR}/hal/src/hal_spi_m_sync.o.d ${OBJECTDIR}/hal/src/hal_usart_sync.o.d ${OBJECTDIR}/hal/utils/src/utils_assert.o.d ${OBJECTDIR}/hal/utils/src/utils_event.o.d ${OBJECTDIR}/hal/utils/src/utils_list.o.d ${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d ${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o.d ${OBJECTDIR}/hpl/core/hpl_init.o.d ${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d ${OBJECTDIR}/hpl/eic/hpl_eic.o.d ${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d ${OBJECTDIR}/hpl/pm/hpl_pm.o.d ${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d ${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o.d ${OBJECTDIR}/hpl/systick/hpl_systick.o.d ${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o.d ${OBJECTDIR}/samd21a/gcc/system_samd21.o.d ${OBJECTDIR}/stdio_redirect/gcc/read.o.d ${OBJECTDIR}/stdio_redirect/gcc/write.o.d ${OBJECTDIR}/stdio_redirect/stdio_io.o.d ${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o.d ${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o.d ${OBJECTDIR}/winc1500/common/source/nm_common.o.d ${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o.d ${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o.d ${OBJECTDIR}/winc1500/driver/source/m2m_hif.o.d ${OBJECTDIR}/winc1500/driver/source/m2m_ota.o.d ${OBJECTDIR}/winc1500/driver/source/m2m_periph.o.d ${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o.d ${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o.d ${OBJECTDIR}/winc1500/driver/source/nmasic.o.d ${OBJECTDIR}/winc1500/driver/source/nmbus.o.d ${OBJECTDIR}/winc1500/driver/source/nmdrv.o.d ${OBJECTDIR}/winc1500/driver/source/nmi2c.o.d ${OBJECTDIR}/winc1500/driver/source/nmspi.o.d ${OBJECTDIR}/winc1500/driver/source/nmuart.o.d ${OBJECTDIR}/winc1500/socket/source/socket.o.d ${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/driver_init.o.d ${OBJECTDIR}/stdio_start.o.d ${OBJECTDIR}/cryptoauthlib_main.o.d ${OBJECTDIR}/winc_init.o.d ${OBJECTDIR}/atmel_start.o.d ${OBJECTDIR}/cert_def_1_signer.o.d ${OBJECTDIR}/cert_def_2_device.o.d ${OBJECTDIR}/cert_def_3_device_csr.o.d ${OBJECTDIR}/cert_def_4_device.o.d ${OBJECTDIR}/cert_def_4_signer.o.d ${OBJECTDIR}/convertions.o.d ${OBJECTDIR}/winc_certs_functions.o.d ${OBJECTDIR}/samd21a/gcc/gcc/newfile.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o ${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o ${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o ${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o ${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o ${OBJECTDIR}/cryptoauthlib/lib/atca_command.o ${OBJECTDIR}/cryptoauthlib/lib/atca_device.o ${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o ${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o ${OBJECTDIR}/examples/driver_examples.o ${OBJECTDIR}/hal/src/hal_atomic.o ${OBJECTDIR}/hal/src/hal_delay.o ${OBJECTDIR}/hal/src/hal_gpio.o ${OBJECTDIR}/hal/src/hal_i2c_m_sync.o ${OBJECTDIR}/hal/src/hal_init.o ${OBJECTDIR}/hal/src/hal_io.o ${OBJECTDIR}/hal/src/hal_sleep.o ${OBJECTDIR}/hal/src/hal_ext_irq.o ${OBJECTDIR}/hal/src/hal_spi_m_sync.o ${OBJECTDIR}/hal/src/hal_usart_sync.o ${OBJECTDIR}/hal/utils/src/utils_assert.o ${OBJECTDIR}/hal/utils/src/utils_event.o ${OBJECTDIR}/hal/utils/src/utils_list.o ${OBJECTDIR}/hal/utils/src/utils_syscalls.o ${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o ${OBJECTDIR}/hpl/core/hpl_init.o ${OBJECTDIR}/hpl/dmac/hpl_dmac.o ${OBJECTDIR}/hpl/eic/hpl_eic.o ${OBJECTDIR}/hpl/gclk/hpl_gclk.o ${OBJECTDIR}/hpl/pm/hpl_pm.o ${OBJECTDIR}/hpl/sercom/hpl_sercom.o ${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o ${OBJECTDIR}/hpl/systick/hpl_systick.o ${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o ${OBJECTDIR}/samd21a/gcc/system_samd21.o ${OBJECTDIR}/stdio_redirect/gcc/read.o ${OBJECTDIR}/stdio_redirect/gcc/write.o ${OBJECTDIR}/stdio_redirect/stdio_io.o ${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o ${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o ${OBJECTDIR}/winc1500/common/source/nm_common.o ${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o ${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o ${OBJECTDIR}/winc1500/driver/source/m2m_hif.o ${OBJECTDIR}/winc1500/driver/source/m2m_ota.o ${OBJECTDIR}/winc1500/driver/source/m2m_periph.o ${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o ${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o ${OBJECTDIR}/winc1500/driver/source/nmasic.o ${OBJECTDIR}/winc1500/driver/source/nmbus.o ${OBJECTDIR}/winc1500/driver/source/nmdrv.o ${OBJECTDIR}/winc1500/driver/source/nmi2c.o ${OBJECTDIR}/winc1500/driver/source/nmspi.o ${OBJECTDIR}/winc1500/driver/source/nmuart.o ${OBJECTDIR}/winc1500/socket/source/socket.o ${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o ${OBJECTDIR}/main.o ${OBJECTDIR}/driver_init.o ${OBJECTDIR}/stdio_start.o ${OBJECTDIR}/cryptoauthlib_main.o ${OBJECTDIR}/winc_init.o ${OBJECTDIR}/atmel_start.o ${OBJECTDIR}/cert_def_1_signer.o ${OBJECTDIR}/cert_def_2_device.o ${OBJECTDIR}/cert_def_3_device_csr.o ${OBJECTDIR}/cert_def_4_device.o ${OBJECTDIR}/cert_def_4_signer.o ${OBJECTDIR}/convertions.o ${OBJECTDIR}/winc_certs_functions.o ${OBJECTDIR}/samd21a/gcc/gcc/newfile.o

# Source Files
SOURCEFILES=cryptoauthlib/lib/atcacert/atcacert_client.c cryptoauthlib/lib/atcacert/atcacert_date.c cryptoauthlib/lib/atcacert/atcacert_def.c cryptoauthlib/lib/atcacert/atcacert_der.c cryptoauthlib/lib/atcacert/atcacert_host_hw.c cryptoauthlib/lib/atcacert/atcacert_host_sw.c cryptoauthlib/lib/basic/atca_basic.c cryptoauthlib/lib/basic/atca_basic_aes.c cryptoauthlib/lib/basic/atca_basic_aes_cbc.c cryptoauthlib/lib/basic/atca_basic_aes_cmac.c cryptoauthlib/lib/basic/atca_basic_aes_ctr.c cryptoauthlib/lib/basic/atca_basic_aes_gcm.c cryptoauthlib/lib/basic/atca_basic_checkmac.c cryptoauthlib/lib/basic/atca_basic_counter.c cryptoauthlib/lib/basic/atca_basic_derivekey.c cryptoauthlib/lib/basic/atca_basic_ecdh.c cryptoauthlib/lib/basic/atca_basic_gendig.c cryptoauthlib/lib/basic/atca_basic_genkey.c cryptoauthlib/lib/basic/atca_basic_hmac.c cryptoauthlib/lib/basic/atca_basic_info.c cryptoauthlib/lib/basic/atca_basic_kdf.c cryptoauthlib/lib/basic/atca_basic_lock.c cryptoauthlib/lib/basic/atca_basic_mac.c cryptoauthlib/lib/basic/atca_basic_nonce.c cryptoauthlib/lib/basic/atca_basic_privwrite.c cryptoauthlib/lib/basic/atca_basic_random.c cryptoauthlib/lib/basic/atca_basic_read.c cryptoauthlib/lib/basic/atca_basic_secureboot.c cryptoauthlib/lib/basic/atca_basic_selftest.c cryptoauthlib/lib/basic/atca_basic_sha.c cryptoauthlib/lib/basic/atca_basic_sign.c cryptoauthlib/lib/basic/atca_basic_updateextra.c cryptoauthlib/lib/basic/atca_basic_verify.c cryptoauthlib/lib/basic/atca_basic_write.c cryptoauthlib/lib/basic/atca_helpers.c cryptoauthlib/lib/crypto/hashes/sha1_routines.c cryptoauthlib/lib/crypto/hashes/sha2_routines.c cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c cryptoauthlib/lib/hal/atca_hal.c cryptoauthlib/lib/hal/hal_sam_i2c_start.c cryptoauthlib/lib/hal/hal_sam_timer_start.c cryptoauthlib/lib/host/atca_host.c cryptoauthlib/lib/jwt/atca_jwt.c cryptoauthlib/lib/atca_cfgs.c cryptoauthlib/lib/atca_command.c cryptoauthlib/lib/atca_device.c cryptoauthlib/lib/atca_execution.c cryptoauthlib/lib/atca_iface.c examples/driver_examples.c hal/src/hal_atomic.c hal/src/hal_delay.c hal/src/hal_gpio.c hal/src/hal_i2c_m_sync.c hal/src/hal_init.c hal/src/hal_io.c hal/src/hal_sleep.c hal/src/hal_ext_irq.c hal/src/hal_spi_m_sync.c hal/src/hal_usart_sync.c hal/utils/src/utils_assert.c hal/utils/src/utils_event.c hal/utils/src/utils_list.c hal/utils/src/utils_syscalls.c hpl/core/hpl_core_m0plus_base.c hpl/core/hpl_init.c hpl/dmac/hpl_dmac.c hpl/eic/hpl_eic.c hpl/gclk/hpl_gclk.c hpl/pm/hpl_pm.c hpl/sercom/hpl_sercom.c hpl/sysctrl/hpl_sysctrl.c hpl/systick/hpl_systick.c samd21a/gcc/gcc/startup_samd21.c samd21a/gcc/system_samd21.c stdio_redirect/gcc/read.c stdio_redirect/gcc/write.c stdio_redirect/stdio_io.c winc1500/bsp/source/nm_bsp.c winc1500/bus_wrapper/source/nm_bus_wrapper.c winc1500/common/source/nm_common.c winc1500/driver/source/m2m_ate_mode.c winc1500/driver/source/m2m_crypto.c winc1500/driver/source/m2m_hif.c winc1500/driver/source/m2m_ota.c winc1500/driver/source/m2m_periph.c winc1500/driver/source/m2m_ssl.c winc1500/driver/source/m2m_wifi.c winc1500/driver/source/nmasic.c winc1500/driver/source/nmbus.c winc1500/driver/source/nmdrv.c winc1500/driver/source/nmi2c.c winc1500/driver/source/nmspi.c winc1500/driver/source/nmuart.c winc1500/socket/source/socket.c winc1500/spi_flash/source/spi_flash.c main.c driver_init.c stdio_start.c cryptoauthlib_main.c winc_init.c atmel_start.c cert_def_1_signer.c cert_def_2_device.c cert_def_3_device_csr.c cert_def_4_device.c cert_def_4_signer.c convertions.c winc_certs_functions.c samd21a/gcc/gcc/newfile.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${DFP_DIR}/samd21a/include"  -I "${CMSIS_DIR}/CMSIS/Core/Include"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Default.mk dist/${CND_CONF}/${IMAGE_TYPE}/SAM_IOT_WINC_PROV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=SAMD21G18A
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o: cryptoauthlib/lib/atcacert/atcacert_client.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o cryptoauthlib/lib/atcacert/atcacert_client.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o: cryptoauthlib/lib/atcacert/atcacert_date.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o cryptoauthlib/lib/atcacert/atcacert_date.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o: cryptoauthlib/lib/atcacert/atcacert_def.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o cryptoauthlib/lib/atcacert/atcacert_def.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o: cryptoauthlib/lib/atcacert/atcacert_der.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o cryptoauthlib/lib/atcacert/atcacert_der.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o: cryptoauthlib/lib/atcacert/atcacert_host_hw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o cryptoauthlib/lib/atcacert/atcacert_host_hw.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o: cryptoauthlib/lib/atcacert/atcacert_host_sw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o cryptoauthlib/lib/atcacert/atcacert_host_sw.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o: cryptoauthlib/lib/basic/atca_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o cryptoauthlib/lib/basic/atca_basic.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o: cryptoauthlib/lib/basic/atca_basic_aes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o cryptoauthlib/lib/basic/atca_basic_aes.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o: cryptoauthlib/lib/basic/atca_basic_aes_cbc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o cryptoauthlib/lib/basic/atca_basic_aes_cbc.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o: cryptoauthlib/lib/basic/atca_basic_aes_cmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o cryptoauthlib/lib/basic/atca_basic_aes_cmac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o: cryptoauthlib/lib/basic/atca_basic_aes_ctr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o cryptoauthlib/lib/basic/atca_basic_aes_ctr.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o: cryptoauthlib/lib/basic/atca_basic_aes_gcm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o cryptoauthlib/lib/basic/atca_basic_aes_gcm.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o: cryptoauthlib/lib/basic/atca_basic_checkmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o cryptoauthlib/lib/basic/atca_basic_checkmac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o: cryptoauthlib/lib/basic/atca_basic_counter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o cryptoauthlib/lib/basic/atca_basic_counter.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o: cryptoauthlib/lib/basic/atca_basic_derivekey.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o cryptoauthlib/lib/basic/atca_basic_derivekey.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o: cryptoauthlib/lib/basic/atca_basic_ecdh.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o cryptoauthlib/lib/basic/atca_basic_ecdh.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o: cryptoauthlib/lib/basic/atca_basic_gendig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o cryptoauthlib/lib/basic/atca_basic_gendig.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o: cryptoauthlib/lib/basic/atca_basic_genkey.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o cryptoauthlib/lib/basic/atca_basic_genkey.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o: cryptoauthlib/lib/basic/atca_basic_hmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o cryptoauthlib/lib/basic/atca_basic_hmac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o: cryptoauthlib/lib/basic/atca_basic_info.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o cryptoauthlib/lib/basic/atca_basic_info.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o: cryptoauthlib/lib/basic/atca_basic_kdf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o cryptoauthlib/lib/basic/atca_basic_kdf.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o: cryptoauthlib/lib/basic/atca_basic_lock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o cryptoauthlib/lib/basic/atca_basic_lock.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o: cryptoauthlib/lib/basic/atca_basic_mac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o cryptoauthlib/lib/basic/atca_basic_mac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o: cryptoauthlib/lib/basic/atca_basic_nonce.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o cryptoauthlib/lib/basic/atca_basic_nonce.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o: cryptoauthlib/lib/basic/atca_basic_privwrite.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o cryptoauthlib/lib/basic/atca_basic_privwrite.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o: cryptoauthlib/lib/basic/atca_basic_random.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o cryptoauthlib/lib/basic/atca_basic_random.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o: cryptoauthlib/lib/basic/atca_basic_read.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o cryptoauthlib/lib/basic/atca_basic_read.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o: cryptoauthlib/lib/basic/atca_basic_secureboot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o cryptoauthlib/lib/basic/atca_basic_secureboot.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o: cryptoauthlib/lib/basic/atca_basic_selftest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o cryptoauthlib/lib/basic/atca_basic_selftest.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o: cryptoauthlib/lib/basic/atca_basic_sha.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o cryptoauthlib/lib/basic/atca_basic_sha.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o: cryptoauthlib/lib/basic/atca_basic_sign.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o cryptoauthlib/lib/basic/atca_basic_sign.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o: cryptoauthlib/lib/basic/atca_basic_updateextra.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o cryptoauthlib/lib/basic/atca_basic_updateextra.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o: cryptoauthlib/lib/basic/atca_basic_verify.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o cryptoauthlib/lib/basic/atca_basic_verify.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o: cryptoauthlib/lib/basic/atca_basic_write.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o cryptoauthlib/lib/basic/atca_basic_write.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o: cryptoauthlib/lib/basic/atca_helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o cryptoauthlib/lib/basic/atca_helpers.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o: cryptoauthlib/lib/crypto/hashes/sha1_routines.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o cryptoauthlib/lib/crypto/hashes/sha1_routines.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o: cryptoauthlib/lib/crypto/hashes/sha2_routines.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o cryptoauthlib/lib/crypto/hashes/sha2_routines.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o: cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o: cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o: cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o: cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o: cryptoauthlib/lib/hal/atca_hal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o cryptoauthlib/lib/hal/atca_hal.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o: cryptoauthlib/lib/hal/hal_sam_i2c_start.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o cryptoauthlib/lib/hal/hal_sam_i2c_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o: cryptoauthlib/lib/hal/hal_sam_timer_start.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o cryptoauthlib/lib/hal/hal_sam_timer_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o: cryptoauthlib/lib/host/atca_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/host" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o cryptoauthlib/lib/host/atca_host.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o: cryptoauthlib/lib/jwt/atca_jwt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/jwt" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o cryptoauthlib/lib/jwt/atca_jwt.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o: cryptoauthlib/lib/atca_cfgs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o cryptoauthlib/lib/atca_cfgs.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atca_command.o: cryptoauthlib/lib/atca_command.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_command.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_command.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atca_command.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atca_command.o cryptoauthlib/lib/atca_command.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atca_command.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atca_device.o: cryptoauthlib/lib/atca_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_device.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atca_device.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atca_device.o cryptoauthlib/lib/atca_device.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atca_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o: cryptoauthlib/lib/atca_execution.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o cryptoauthlib/lib/atca_execution.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o: cryptoauthlib/lib/atca_iface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o cryptoauthlib/lib/atca_iface.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/examples/driver_examples.o: examples/driver_examples.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/examples" 
	@${RM} ${OBJECTDIR}/examples/driver_examples.o.d 
	@${RM} ${OBJECTDIR}/examples/driver_examples.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/examples/driver_examples.o.d" -o ${OBJECTDIR}/examples/driver_examples.o examples/driver_examples.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/examples/driver_examples.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_atomic.o: hal/src/hal_atomic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_atomic.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_atomic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_atomic.o.d" -o ${OBJECTDIR}/hal/src/hal_atomic.o hal/src/hal_atomic.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_atomic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_delay.o: hal/src/hal_delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_delay.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_delay.o.d" -o ${OBJECTDIR}/hal/src/hal_delay.o hal/src/hal_delay.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_gpio.o: hal/src/hal_gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_gpio.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_gpio.o.d" -o ${OBJECTDIR}/hal/src/hal_gpio.o hal/src/hal_gpio.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_gpio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_i2c_m_sync.o: hal/src/hal_i2c_m_sync.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_i2c_m_sync.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_i2c_m_sync.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_i2c_m_sync.o.d" -o ${OBJECTDIR}/hal/src/hal_i2c_m_sync.o hal/src/hal_i2c_m_sync.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_i2c_m_sync.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_init.o: hal/src/hal_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_init.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_init.o.d" -o ${OBJECTDIR}/hal/src/hal_init.o hal/src/hal_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_io.o: hal/src/hal_io.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_io.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_io.o.d" -o ${OBJECTDIR}/hal/src/hal_io.o hal/src/hal_io.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_io.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_sleep.o: hal/src/hal_sleep.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_sleep.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_sleep.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_sleep.o.d" -o ${OBJECTDIR}/hal/src/hal_sleep.o hal/src/hal_sleep.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_sleep.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_ext_irq.o: hal/src/hal_ext_irq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_ext_irq.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_ext_irq.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_ext_irq.o.d" -o ${OBJECTDIR}/hal/src/hal_ext_irq.o hal/src/hal_ext_irq.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_ext_irq.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_spi_m_sync.o: hal/src/hal_spi_m_sync.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_spi_m_sync.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_spi_m_sync.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_spi_m_sync.o.d" -o ${OBJECTDIR}/hal/src/hal_spi_m_sync.o hal/src/hal_spi_m_sync.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_spi_m_sync.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_usart_sync.o: hal/src/hal_usart_sync.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_usart_sync.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_usart_sync.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_usart_sync.o.d" -o ${OBJECTDIR}/hal/src/hal_usart_sync.o hal/src/hal_usart_sync.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_usart_sync.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/utils/src/utils_assert.o: hal/utils/src/utils_assert.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_assert.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_assert.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_assert.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_assert.o hal/utils/src/utils_assert.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/utils/src/utils_assert.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/utils/src/utils_event.o: hal/utils/src/utils_event.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_event.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_event.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_event.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_event.o hal/utils/src/utils_event.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/utils/src/utils_event.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/utils/src/utils_list.o: hal/utils/src/utils_list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_list.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_list.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_list.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_list.o hal/utils/src/utils_list.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/utils/src/utils_list.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/utils/src/utils_syscalls.o: hal/utils/src/utils_syscalls.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_syscalls.o hal/utils/src/utils_syscalls.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o: hpl/core/hpl_core_m0plus_base.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/core" 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o.d 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o.d" -o ${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o hpl/core/hpl_core_m0plus_base.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/core/hpl_init.o: hpl/core/hpl_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/core" 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_init.o.d 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/core/hpl_init.o.d" -o ${OBJECTDIR}/hpl/core/hpl_init.o hpl/core/hpl_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/core/hpl_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/dmac/hpl_dmac.o: hpl/dmac/hpl_dmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/dmac" 
	@${RM} ${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d 
	@${RM} ${OBJECTDIR}/hpl/dmac/hpl_dmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d" -o ${OBJECTDIR}/hpl/dmac/hpl_dmac.o hpl/dmac/hpl_dmac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/eic/hpl_eic.o: hpl/eic/hpl_eic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/eic" 
	@${RM} ${OBJECTDIR}/hpl/eic/hpl_eic.o.d 
	@${RM} ${OBJECTDIR}/hpl/eic/hpl_eic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/eic/hpl_eic.o.d" -o ${OBJECTDIR}/hpl/eic/hpl_eic.o hpl/eic/hpl_eic.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/eic/hpl_eic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/gclk/hpl_gclk.o: hpl/gclk/hpl_gclk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/gclk" 
	@${RM} ${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d 
	@${RM} ${OBJECTDIR}/hpl/gclk/hpl_gclk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d" -o ${OBJECTDIR}/hpl/gclk/hpl_gclk.o hpl/gclk/hpl_gclk.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/pm/hpl_pm.o: hpl/pm/hpl_pm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/pm" 
	@${RM} ${OBJECTDIR}/hpl/pm/hpl_pm.o.d 
	@${RM} ${OBJECTDIR}/hpl/pm/hpl_pm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/pm/hpl_pm.o.d" -o ${OBJECTDIR}/hpl/pm/hpl_pm.o hpl/pm/hpl_pm.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/pm/hpl_pm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/sercom/hpl_sercom.o: hpl/sercom/hpl_sercom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/sercom" 
	@${RM} ${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d 
	@${RM} ${OBJECTDIR}/hpl/sercom/hpl_sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d" -o ${OBJECTDIR}/hpl/sercom/hpl_sercom.o hpl/sercom/hpl_sercom.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o: hpl/sysctrl/hpl_sysctrl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/sysctrl" 
	@${RM} ${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o.d 
	@${RM} ${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o.d" -o ${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o hpl/sysctrl/hpl_sysctrl.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/systick/hpl_systick.o: hpl/systick/hpl_systick.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/systick" 
	@${RM} ${OBJECTDIR}/hpl/systick/hpl_systick.o.d 
	@${RM} ${OBJECTDIR}/hpl/systick/hpl_systick.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/systick/hpl_systick.o.d" -o ${OBJECTDIR}/hpl/systick/hpl_systick.o hpl/systick/hpl_systick.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/systick/hpl_systick.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o: samd21a/gcc/gcc/startup_samd21.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/samd21a/gcc/gcc" 
	@${RM} ${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o.d 
	@${RM} ${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o.d" -o ${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o samd21a/gcc/gcc/startup_samd21.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/samd21a/gcc/system_samd21.o: samd21a/gcc/system_samd21.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/samd21a/gcc" 
	@${RM} ${OBJECTDIR}/samd21a/gcc/system_samd21.o.d 
	@${RM} ${OBJECTDIR}/samd21a/gcc/system_samd21.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/samd21a/gcc/system_samd21.o.d" -o ${OBJECTDIR}/samd21a/gcc/system_samd21.o samd21a/gcc/system_samd21.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/samd21a/gcc/system_samd21.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/stdio_redirect/gcc/read.o: stdio_redirect/gcc/read.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/stdio_redirect/gcc" 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/read.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/read.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/stdio_redirect/gcc/read.o.d" -o ${OBJECTDIR}/stdio_redirect/gcc/read.o stdio_redirect/gcc/read.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/stdio_redirect/gcc/read.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/stdio_redirect/gcc/write.o: stdio_redirect/gcc/write.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/stdio_redirect/gcc" 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/write.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/write.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/stdio_redirect/gcc/write.o.d" -o ${OBJECTDIR}/stdio_redirect/gcc/write.o stdio_redirect/gcc/write.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/stdio_redirect/gcc/write.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/stdio_redirect/stdio_io.o: stdio_redirect/stdio_io.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/stdio_redirect" 
	@${RM} ${OBJECTDIR}/stdio_redirect/stdio_io.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/stdio_io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/stdio_redirect/stdio_io.o.d" -o ${OBJECTDIR}/stdio_redirect/stdio_io.o stdio_redirect/stdio_io.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/stdio_redirect/stdio_io.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o: winc1500/bsp/source/nm_bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/bsp/source" 
	@${RM} ${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o.d 
	@${RM} ${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o.d" -o ${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o winc1500/bsp/source/nm_bsp.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o: winc1500/bus_wrapper/source/nm_bus_wrapper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/bus_wrapper/source" 
	@${RM} ${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o.d 
	@${RM} ${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o.d" -o ${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o winc1500/bus_wrapper/source/nm_bus_wrapper.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/common/source/nm_common.o: winc1500/common/source/nm_common.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/common/source" 
	@${RM} ${OBJECTDIR}/winc1500/common/source/nm_common.o.d 
	@${RM} ${OBJECTDIR}/winc1500/common/source/nm_common.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/common/source/nm_common.o.d" -o ${OBJECTDIR}/winc1500/common/source/nm_common.o winc1500/common/source/nm_common.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/common/source/nm_common.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o: winc1500/driver/source/m2m_ate_mode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o winc1500/driver/source/m2m_ate_mode.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o: winc1500/driver/source/m2m_crypto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o winc1500/driver/source/m2m_crypto.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_hif.o: winc1500/driver/source/m2m_hif.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_hif.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_hif.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_hif.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_hif.o winc1500/driver/source/m2m_hif.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_hif.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_ota.o: winc1500/driver/source/m2m_ota.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ota.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ota.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_ota.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_ota.o winc1500/driver/source/m2m_ota.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_ota.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_periph.o: winc1500/driver/source/m2m_periph.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_periph.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_periph.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_periph.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_periph.o winc1500/driver/source/m2m_periph.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_periph.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o: winc1500/driver/source/m2m_ssl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o winc1500/driver/source/m2m_ssl.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o: winc1500/driver/source/m2m_wifi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o winc1500/driver/source/m2m_wifi.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmasic.o: winc1500/driver/source/nmasic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmasic.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmasic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmasic.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmasic.o winc1500/driver/source/nmasic.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmasic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmbus.o: winc1500/driver/source/nmbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmbus.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmbus.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmbus.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmbus.o winc1500/driver/source/nmbus.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmbus.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmdrv.o: winc1500/driver/source/nmdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmdrv.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmdrv.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmdrv.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmdrv.o winc1500/driver/source/nmdrv.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmdrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmi2c.o: winc1500/driver/source/nmi2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmi2c.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmi2c.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmi2c.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmi2c.o winc1500/driver/source/nmi2c.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmi2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmspi.o: winc1500/driver/source/nmspi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmspi.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmspi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmspi.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmspi.o winc1500/driver/source/nmspi.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmspi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmuart.o: winc1500/driver/source/nmuart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmuart.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmuart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmuart.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmuart.o winc1500/driver/source/nmuart.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmuart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/socket/source/socket.o: winc1500/socket/source/socket.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/socket/source" 
	@${RM} ${OBJECTDIR}/winc1500/socket/source/socket.o.d 
	@${RM} ${OBJECTDIR}/winc1500/socket/source/socket.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/socket/source/socket.o.d" -o ${OBJECTDIR}/winc1500/socket/source/socket.o winc1500/socket/source/socket.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/socket/source/socket.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o: winc1500/spi_flash/source/spi_flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/spi_flash/source" 
	@${RM} ${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o.d 
	@${RM} ${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o.d" -o ${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o winc1500/spi_flash/source/spi_flash.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/driver_init.o: driver_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/driver_init.o.d 
	@${RM} ${OBJECTDIR}/driver_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/driver_init.o.d" -o ${OBJECTDIR}/driver_init.o driver_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/driver_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/stdio_start.o: stdio_start.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/stdio_start.o.d 
	@${RM} ${OBJECTDIR}/stdio_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/stdio_start.o.d" -o ${OBJECTDIR}/stdio_start.o stdio_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/stdio_start.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib_main.o: cryptoauthlib_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cryptoauthlib_main.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib_main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib_main.o.d" -o ${OBJECTDIR}/cryptoauthlib_main.o cryptoauthlib_main.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc_init.o: winc_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/winc_init.o.d 
	@${RM} ${OBJECTDIR}/winc_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc_init.o.d" -o ${OBJECTDIR}/winc_init.o winc_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/atmel_start.o: atmel_start.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/atmel_start.o.d 
	@${RM} ${OBJECTDIR}/atmel_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/atmel_start.o.d" -o ${OBJECTDIR}/atmel_start.o atmel_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/atmel_start.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cert_def_1_signer.o: cert_def_1_signer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cert_def_1_signer.o.d 
	@${RM} ${OBJECTDIR}/cert_def_1_signer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cert_def_1_signer.o.d" -o ${OBJECTDIR}/cert_def_1_signer.o cert_def_1_signer.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cert_def_1_signer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cert_def_2_device.o: cert_def_2_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cert_def_2_device.o.d 
	@${RM} ${OBJECTDIR}/cert_def_2_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cert_def_2_device.o.d" -o ${OBJECTDIR}/cert_def_2_device.o cert_def_2_device.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cert_def_2_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cert_def_3_device_csr.o: cert_def_3_device_csr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cert_def_3_device_csr.o.d 
	@${RM} ${OBJECTDIR}/cert_def_3_device_csr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cert_def_3_device_csr.o.d" -o ${OBJECTDIR}/cert_def_3_device_csr.o cert_def_3_device_csr.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cert_def_3_device_csr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cert_def_4_device.o: cert_def_4_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cert_def_4_device.o.d 
	@${RM} ${OBJECTDIR}/cert_def_4_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cert_def_4_device.o.d" -o ${OBJECTDIR}/cert_def_4_device.o cert_def_4_device.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cert_def_4_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cert_def_4_signer.o: cert_def_4_signer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cert_def_4_signer.o.d 
	@${RM} ${OBJECTDIR}/cert_def_4_signer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cert_def_4_signer.o.d" -o ${OBJECTDIR}/cert_def_4_signer.o cert_def_4_signer.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cert_def_4_signer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/convertions.o: convertions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/convertions.o.d 
	@${RM} ${OBJECTDIR}/convertions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/convertions.o.d" -o ${OBJECTDIR}/convertions.o convertions.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/convertions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc_certs_functions.o: winc_certs_functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/winc_certs_functions.o.d 
	@${RM} ${OBJECTDIR}/winc_certs_functions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc_certs_functions.o.d" -o ${OBJECTDIR}/winc_certs_functions.o winc_certs_functions.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc_certs_functions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/samd21a/gcc/gcc/newfile.o: samd21a/gcc/gcc/newfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/samd21a/gcc/gcc" 
	@${RM} ${OBJECTDIR}/samd21a/gcc/gcc/newfile.o.d 
	@${RM} ${OBJECTDIR}/samd21a/gcc/gcc/newfile.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/samd21a/gcc/gcc/newfile.o.d" -o ${OBJECTDIR}/samd21a/gcc/gcc/newfile.o samd21a/gcc/gcc/newfile.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/samd21a/gcc/gcc/newfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o: cryptoauthlib/lib/atcacert/atcacert_client.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o cryptoauthlib/lib/atcacert/atcacert_client.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_client.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o: cryptoauthlib/lib/atcacert/atcacert_date.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o cryptoauthlib/lib/atcacert/atcacert_date.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_date.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o: cryptoauthlib/lib/atcacert/atcacert_def.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o cryptoauthlib/lib/atcacert/atcacert_def.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_def.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o: cryptoauthlib/lib/atcacert/atcacert_der.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o cryptoauthlib/lib/atcacert/atcacert_der.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_der.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o: cryptoauthlib/lib/atcacert/atcacert_host_hw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o cryptoauthlib/lib/atcacert/atcacert_host_hw.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o: cryptoauthlib/lib/atcacert/atcacert_host_sw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o cryptoauthlib/lib/atcacert/atcacert_host_sw.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o: cryptoauthlib/lib/basic/atca_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o cryptoauthlib/lib/basic/atca_basic.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o: cryptoauthlib/lib/basic/atca_basic_aes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o cryptoauthlib/lib/basic/atca_basic_aes.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o: cryptoauthlib/lib/basic/atca_basic_aes_cbc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o cryptoauthlib/lib/basic/atca_basic_aes_cbc.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cbc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o: cryptoauthlib/lib/basic/atca_basic_aes_cmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o cryptoauthlib/lib/basic/atca_basic_aes_cmac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_cmac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o: cryptoauthlib/lib/basic/atca_basic_aes_ctr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o cryptoauthlib/lib/basic/atca_basic_aes_ctr.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_ctr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o: cryptoauthlib/lib/basic/atca_basic_aes_gcm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o cryptoauthlib/lib/basic/atca_basic_aes_gcm.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_aes_gcm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o: cryptoauthlib/lib/basic/atca_basic_checkmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o cryptoauthlib/lib/basic/atca_basic_checkmac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o: cryptoauthlib/lib/basic/atca_basic_counter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o cryptoauthlib/lib/basic/atca_basic_counter.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_counter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o: cryptoauthlib/lib/basic/atca_basic_derivekey.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o cryptoauthlib/lib/basic/atca_basic_derivekey.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o: cryptoauthlib/lib/basic/atca_basic_ecdh.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o cryptoauthlib/lib/basic/atca_basic_ecdh.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o: cryptoauthlib/lib/basic/atca_basic_gendig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o cryptoauthlib/lib/basic/atca_basic_gendig.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_gendig.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o: cryptoauthlib/lib/basic/atca_basic_genkey.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o cryptoauthlib/lib/basic/atca_basic_genkey.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_genkey.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o: cryptoauthlib/lib/basic/atca_basic_hmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o cryptoauthlib/lib/basic/atca_basic_hmac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_hmac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o: cryptoauthlib/lib/basic/atca_basic_info.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o cryptoauthlib/lib/basic/atca_basic_info.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_info.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o: cryptoauthlib/lib/basic/atca_basic_kdf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o cryptoauthlib/lib/basic/atca_basic_kdf.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_kdf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o: cryptoauthlib/lib/basic/atca_basic_lock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o cryptoauthlib/lib/basic/atca_basic_lock.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_lock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o: cryptoauthlib/lib/basic/atca_basic_mac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o cryptoauthlib/lib/basic/atca_basic_mac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_mac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o: cryptoauthlib/lib/basic/atca_basic_nonce.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o cryptoauthlib/lib/basic/atca_basic_nonce.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_nonce.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o: cryptoauthlib/lib/basic/atca_basic_privwrite.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o cryptoauthlib/lib/basic/atca_basic_privwrite.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o: cryptoauthlib/lib/basic/atca_basic_random.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o cryptoauthlib/lib/basic/atca_basic_random.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_random.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o: cryptoauthlib/lib/basic/atca_basic_read.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o cryptoauthlib/lib/basic/atca_basic_read.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_read.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o: cryptoauthlib/lib/basic/atca_basic_secureboot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o cryptoauthlib/lib/basic/atca_basic_secureboot.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o: cryptoauthlib/lib/basic/atca_basic_selftest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o cryptoauthlib/lib/basic/atca_basic_selftest.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_selftest.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o: cryptoauthlib/lib/basic/atca_basic_sha.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o cryptoauthlib/lib/basic/atca_basic_sha.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sha.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o: cryptoauthlib/lib/basic/atca_basic_sign.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o cryptoauthlib/lib/basic/atca_basic_sign.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_sign.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o: cryptoauthlib/lib/basic/atca_basic_updateextra.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o cryptoauthlib/lib/basic/atca_basic_updateextra.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o: cryptoauthlib/lib/basic/atca_basic_verify.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o cryptoauthlib/lib/basic/atca_basic_verify.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_verify.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o: cryptoauthlib/lib/basic/atca_basic_write.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o cryptoauthlib/lib/basic/atca_basic_write.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_basic_write.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o: cryptoauthlib/lib/basic/atca_helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o cryptoauthlib/lib/basic/atca_helpers.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/basic/atca_helpers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o: cryptoauthlib/lib/crypto/hashes/sha1_routines.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o cryptoauthlib/lib/crypto/hashes/sha1_routines.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o: cryptoauthlib/lib/crypto/hashes/sha2_routines.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o cryptoauthlib/lib/crypto/hashes/sha2_routines.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o: cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o: cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o: cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o: cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o: cryptoauthlib/lib/hal/atca_hal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o cryptoauthlib/lib/hal/atca_hal.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/hal/atca_hal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o: cryptoauthlib/lib/hal/hal_sam_i2c_start.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o cryptoauthlib/lib/hal/hal_sam_i2c_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_i2c_start.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o: cryptoauthlib/lib/hal/hal_sam_timer_start.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o cryptoauthlib/lib/hal/hal_sam_timer_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/hal/hal_sam_timer_start.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o: cryptoauthlib/lib/host/atca_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/host" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o cryptoauthlib/lib/host/atca_host.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/host/atca_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o: cryptoauthlib/lib/jwt/atca_jwt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib/jwt" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o cryptoauthlib/lib/jwt/atca_jwt.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/jwt/atca_jwt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o: cryptoauthlib/lib/atca_cfgs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o cryptoauthlib/lib/atca_cfgs.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atca_cfgs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atca_command.o: cryptoauthlib/lib/atca_command.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_command.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_command.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atca_command.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atca_command.o cryptoauthlib/lib/atca_command.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atca_command.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atca_device.o: cryptoauthlib/lib/atca_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_device.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atca_device.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atca_device.o cryptoauthlib/lib/atca_device.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atca_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o: cryptoauthlib/lib/atca_execution.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o cryptoauthlib/lib/atca_execution.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atca_execution.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o: cryptoauthlib/lib/atca_iface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o.d" -o ${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o cryptoauthlib/lib/atca_iface.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib/lib/atca_iface.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/examples/driver_examples.o: examples/driver_examples.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/examples" 
	@${RM} ${OBJECTDIR}/examples/driver_examples.o.d 
	@${RM} ${OBJECTDIR}/examples/driver_examples.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/examples/driver_examples.o.d" -o ${OBJECTDIR}/examples/driver_examples.o examples/driver_examples.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/examples/driver_examples.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_atomic.o: hal/src/hal_atomic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_atomic.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_atomic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_atomic.o.d" -o ${OBJECTDIR}/hal/src/hal_atomic.o hal/src/hal_atomic.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_atomic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_delay.o: hal/src/hal_delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_delay.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_delay.o.d" -o ${OBJECTDIR}/hal/src/hal_delay.o hal/src/hal_delay.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_gpio.o: hal/src/hal_gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_gpio.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_gpio.o.d" -o ${OBJECTDIR}/hal/src/hal_gpio.o hal/src/hal_gpio.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_gpio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_i2c_m_sync.o: hal/src/hal_i2c_m_sync.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_i2c_m_sync.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_i2c_m_sync.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_i2c_m_sync.o.d" -o ${OBJECTDIR}/hal/src/hal_i2c_m_sync.o hal/src/hal_i2c_m_sync.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_i2c_m_sync.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_init.o: hal/src/hal_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_init.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_init.o.d" -o ${OBJECTDIR}/hal/src/hal_init.o hal/src/hal_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_io.o: hal/src/hal_io.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_io.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_io.o.d" -o ${OBJECTDIR}/hal/src/hal_io.o hal/src/hal_io.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_io.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_sleep.o: hal/src/hal_sleep.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_sleep.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_sleep.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_sleep.o.d" -o ${OBJECTDIR}/hal/src/hal_sleep.o hal/src/hal_sleep.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_sleep.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_ext_irq.o: hal/src/hal_ext_irq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_ext_irq.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_ext_irq.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_ext_irq.o.d" -o ${OBJECTDIR}/hal/src/hal_ext_irq.o hal/src/hal_ext_irq.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_ext_irq.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_spi_m_sync.o: hal/src/hal_spi_m_sync.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_spi_m_sync.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_spi_m_sync.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_spi_m_sync.o.d" -o ${OBJECTDIR}/hal/src/hal_spi_m_sync.o hal/src/hal_spi_m_sync.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_spi_m_sync.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/src/hal_usart_sync.o: hal/src/hal_usart_sync.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_usart_sync.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_usart_sync.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/src/hal_usart_sync.o.d" -o ${OBJECTDIR}/hal/src/hal_usart_sync.o hal/src/hal_usart_sync.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/src/hal_usart_sync.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/utils/src/utils_assert.o: hal/utils/src/utils_assert.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_assert.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_assert.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_assert.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_assert.o hal/utils/src/utils_assert.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/utils/src/utils_assert.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/utils/src/utils_event.o: hal/utils/src/utils_event.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_event.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_event.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_event.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_event.o hal/utils/src/utils_event.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/utils/src/utils_event.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/utils/src/utils_list.o: hal/utils/src/utils_list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_list.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_list.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_list.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_list.o hal/utils/src/utils_list.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/utils/src/utils_list.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hal/utils/src/utils_syscalls.o: hal/utils/src/utils_syscalls.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_syscalls.o hal/utils/src/utils_syscalls.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o: hpl/core/hpl_core_m0plus_base.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/core" 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o.d 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o.d" -o ${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o hpl/core/hpl_core_m0plus_base.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/core/hpl_core_m0plus_base.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/core/hpl_init.o: hpl/core/hpl_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/core" 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_init.o.d 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/core/hpl_init.o.d" -o ${OBJECTDIR}/hpl/core/hpl_init.o hpl/core/hpl_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/core/hpl_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/dmac/hpl_dmac.o: hpl/dmac/hpl_dmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/dmac" 
	@${RM} ${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d 
	@${RM} ${OBJECTDIR}/hpl/dmac/hpl_dmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d" -o ${OBJECTDIR}/hpl/dmac/hpl_dmac.o hpl/dmac/hpl_dmac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/eic/hpl_eic.o: hpl/eic/hpl_eic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/eic" 
	@${RM} ${OBJECTDIR}/hpl/eic/hpl_eic.o.d 
	@${RM} ${OBJECTDIR}/hpl/eic/hpl_eic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/eic/hpl_eic.o.d" -o ${OBJECTDIR}/hpl/eic/hpl_eic.o hpl/eic/hpl_eic.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/eic/hpl_eic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/gclk/hpl_gclk.o: hpl/gclk/hpl_gclk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/gclk" 
	@${RM} ${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d 
	@${RM} ${OBJECTDIR}/hpl/gclk/hpl_gclk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d" -o ${OBJECTDIR}/hpl/gclk/hpl_gclk.o hpl/gclk/hpl_gclk.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/pm/hpl_pm.o: hpl/pm/hpl_pm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/pm" 
	@${RM} ${OBJECTDIR}/hpl/pm/hpl_pm.o.d 
	@${RM} ${OBJECTDIR}/hpl/pm/hpl_pm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/pm/hpl_pm.o.d" -o ${OBJECTDIR}/hpl/pm/hpl_pm.o hpl/pm/hpl_pm.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/pm/hpl_pm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/sercom/hpl_sercom.o: hpl/sercom/hpl_sercom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/sercom" 
	@${RM} ${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d 
	@${RM} ${OBJECTDIR}/hpl/sercom/hpl_sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d" -o ${OBJECTDIR}/hpl/sercom/hpl_sercom.o hpl/sercom/hpl_sercom.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o: hpl/sysctrl/hpl_sysctrl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/sysctrl" 
	@${RM} ${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o.d 
	@${RM} ${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o.d" -o ${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o hpl/sysctrl/hpl_sysctrl.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/sysctrl/hpl_sysctrl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hpl/systick/hpl_systick.o: hpl/systick/hpl_systick.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hpl/systick" 
	@${RM} ${OBJECTDIR}/hpl/systick/hpl_systick.o.d 
	@${RM} ${OBJECTDIR}/hpl/systick/hpl_systick.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/hpl/systick/hpl_systick.o.d" -o ${OBJECTDIR}/hpl/systick/hpl_systick.o hpl/systick/hpl_systick.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/hpl/systick/hpl_systick.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o: samd21a/gcc/gcc/startup_samd21.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/samd21a/gcc/gcc" 
	@${RM} ${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o.d 
	@${RM} ${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o.d" -o ${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o samd21a/gcc/gcc/startup_samd21.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/samd21a/gcc/gcc/startup_samd21.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/samd21a/gcc/system_samd21.o: samd21a/gcc/system_samd21.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/samd21a/gcc" 
	@${RM} ${OBJECTDIR}/samd21a/gcc/system_samd21.o.d 
	@${RM} ${OBJECTDIR}/samd21a/gcc/system_samd21.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/samd21a/gcc/system_samd21.o.d" -o ${OBJECTDIR}/samd21a/gcc/system_samd21.o samd21a/gcc/system_samd21.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/samd21a/gcc/system_samd21.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/stdio_redirect/gcc/read.o: stdio_redirect/gcc/read.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/stdio_redirect/gcc" 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/read.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/read.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/stdio_redirect/gcc/read.o.d" -o ${OBJECTDIR}/stdio_redirect/gcc/read.o stdio_redirect/gcc/read.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/stdio_redirect/gcc/read.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/stdio_redirect/gcc/write.o: stdio_redirect/gcc/write.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/stdio_redirect/gcc" 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/write.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/write.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/stdio_redirect/gcc/write.o.d" -o ${OBJECTDIR}/stdio_redirect/gcc/write.o stdio_redirect/gcc/write.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/stdio_redirect/gcc/write.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/stdio_redirect/stdio_io.o: stdio_redirect/stdio_io.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/stdio_redirect" 
	@${RM} ${OBJECTDIR}/stdio_redirect/stdio_io.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/stdio_io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/stdio_redirect/stdio_io.o.d" -o ${OBJECTDIR}/stdio_redirect/stdio_io.o stdio_redirect/stdio_io.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/stdio_redirect/stdio_io.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o: winc1500/bsp/source/nm_bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/bsp/source" 
	@${RM} ${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o.d 
	@${RM} ${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o.d" -o ${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o winc1500/bsp/source/nm_bsp.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/bsp/source/nm_bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o: winc1500/bus_wrapper/source/nm_bus_wrapper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/bus_wrapper/source" 
	@${RM} ${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o.d 
	@${RM} ${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o.d" -o ${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o winc1500/bus_wrapper/source/nm_bus_wrapper.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/bus_wrapper/source/nm_bus_wrapper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/common/source/nm_common.o: winc1500/common/source/nm_common.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/common/source" 
	@${RM} ${OBJECTDIR}/winc1500/common/source/nm_common.o.d 
	@${RM} ${OBJECTDIR}/winc1500/common/source/nm_common.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/common/source/nm_common.o.d" -o ${OBJECTDIR}/winc1500/common/source/nm_common.o winc1500/common/source/nm_common.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/common/source/nm_common.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o: winc1500/driver/source/m2m_ate_mode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o winc1500/driver/source/m2m_ate_mode.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_ate_mode.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o: winc1500/driver/source/m2m_crypto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o winc1500/driver/source/m2m_crypto.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_crypto.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_hif.o: winc1500/driver/source/m2m_hif.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_hif.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_hif.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_hif.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_hif.o winc1500/driver/source/m2m_hif.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_hif.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_ota.o: winc1500/driver/source/m2m_ota.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ota.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ota.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_ota.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_ota.o winc1500/driver/source/m2m_ota.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_ota.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_periph.o: winc1500/driver/source/m2m_periph.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_periph.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_periph.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_periph.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_periph.o winc1500/driver/source/m2m_periph.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_periph.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o: winc1500/driver/source/m2m_ssl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o winc1500/driver/source/m2m_ssl.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_ssl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o: winc1500/driver/source/m2m_wifi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o.d" -o ${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o winc1500/driver/source/m2m_wifi.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/m2m_wifi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmasic.o: winc1500/driver/source/nmasic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmasic.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmasic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmasic.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmasic.o winc1500/driver/source/nmasic.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmasic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmbus.o: winc1500/driver/source/nmbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmbus.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmbus.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmbus.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmbus.o winc1500/driver/source/nmbus.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmbus.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmdrv.o: winc1500/driver/source/nmdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmdrv.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmdrv.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmdrv.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmdrv.o winc1500/driver/source/nmdrv.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmdrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmi2c.o: winc1500/driver/source/nmi2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmi2c.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmi2c.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmi2c.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmi2c.o winc1500/driver/source/nmi2c.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmi2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmspi.o: winc1500/driver/source/nmspi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmspi.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmspi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmspi.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmspi.o winc1500/driver/source/nmspi.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmspi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/driver/source/nmuart.o: winc1500/driver/source/nmuart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/driver/source" 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmuart.o.d 
	@${RM} ${OBJECTDIR}/winc1500/driver/source/nmuart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/driver/source/nmuart.o.d" -o ${OBJECTDIR}/winc1500/driver/source/nmuart.o winc1500/driver/source/nmuart.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/driver/source/nmuart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/socket/source/socket.o: winc1500/socket/source/socket.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/socket/source" 
	@${RM} ${OBJECTDIR}/winc1500/socket/source/socket.o.d 
	@${RM} ${OBJECTDIR}/winc1500/socket/source/socket.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/socket/source/socket.o.d" -o ${OBJECTDIR}/winc1500/socket/source/socket.o winc1500/socket/source/socket.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/socket/source/socket.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o: winc1500/spi_flash/source/spi_flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/winc1500/spi_flash/source" 
	@${RM} ${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o.d 
	@${RM} ${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o.d" -o ${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o winc1500/spi_flash/source/spi_flash.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc1500/spi_flash/source/spi_flash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/driver_init.o: driver_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/driver_init.o.d 
	@${RM} ${OBJECTDIR}/driver_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/driver_init.o.d" -o ${OBJECTDIR}/driver_init.o driver_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/driver_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/stdio_start.o: stdio_start.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/stdio_start.o.d 
	@${RM} ${OBJECTDIR}/stdio_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/stdio_start.o.d" -o ${OBJECTDIR}/stdio_start.o stdio_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/stdio_start.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cryptoauthlib_main.o: cryptoauthlib_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cryptoauthlib_main.o.d 
	@${RM} ${OBJECTDIR}/cryptoauthlib_main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cryptoauthlib_main.o.d" -o ${OBJECTDIR}/cryptoauthlib_main.o cryptoauthlib_main.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cryptoauthlib_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc_init.o: winc_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/winc_init.o.d 
	@${RM} ${OBJECTDIR}/winc_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc_init.o.d" -o ${OBJECTDIR}/winc_init.o winc_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/atmel_start.o: atmel_start.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/atmel_start.o.d 
	@${RM} ${OBJECTDIR}/atmel_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/atmel_start.o.d" -o ${OBJECTDIR}/atmel_start.o atmel_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/atmel_start.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cert_def_1_signer.o: cert_def_1_signer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cert_def_1_signer.o.d 
	@${RM} ${OBJECTDIR}/cert_def_1_signer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cert_def_1_signer.o.d" -o ${OBJECTDIR}/cert_def_1_signer.o cert_def_1_signer.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cert_def_1_signer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cert_def_2_device.o: cert_def_2_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cert_def_2_device.o.d 
	@${RM} ${OBJECTDIR}/cert_def_2_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cert_def_2_device.o.d" -o ${OBJECTDIR}/cert_def_2_device.o cert_def_2_device.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cert_def_2_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cert_def_3_device_csr.o: cert_def_3_device_csr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cert_def_3_device_csr.o.d 
	@${RM} ${OBJECTDIR}/cert_def_3_device_csr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cert_def_3_device_csr.o.d" -o ${OBJECTDIR}/cert_def_3_device_csr.o cert_def_3_device_csr.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cert_def_3_device_csr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cert_def_4_device.o: cert_def_4_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cert_def_4_device.o.d 
	@${RM} ${OBJECTDIR}/cert_def_4_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cert_def_4_device.o.d" -o ${OBJECTDIR}/cert_def_4_device.o cert_def_4_device.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cert_def_4_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cert_def_4_signer.o: cert_def_4_signer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cert_def_4_signer.o.d 
	@${RM} ${OBJECTDIR}/cert_def_4_signer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/cert_def_4_signer.o.d" -o ${OBJECTDIR}/cert_def_4_signer.o cert_def_4_signer.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/cert_def_4_signer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/convertions.o: convertions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/convertions.o.d 
	@${RM} ${OBJECTDIR}/convertions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/convertions.o.d" -o ${OBJECTDIR}/convertions.o convertions.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/convertions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/winc_certs_functions.o: winc_certs_functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/winc_certs_functions.o.d 
	@${RM} ${OBJECTDIR}/winc_certs_functions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/winc_certs_functions.o.d" -o ${OBJECTDIR}/winc_certs_functions.o winc_certs_functions.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/winc_certs_functions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/samd21a/gcc/gcc/newfile.o: samd21a/gcc/gcc/newfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/samd21a/gcc/gcc" 
	@${RM} ${OBJECTDIR}/samd21a/gcc/gcc/newfile.o.d 
	@${RM} ${OBJECTDIR}/samd21a/gcc/gcc/newfile.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "hpl/port" -I "./." -I "hpl/systick" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "hri" -I "hpl/gclk" -I "hpl/sercom" -I "hpl/sysctrl" -I "hpl/core" -I "hpl/dmac" -I "winc1500" -I "winc1500/driver/include" -I "examples" -I "cryptoauthlib/lib" -I "hpl/eic" -I "config" -I "stdio_redirect"  -I "samd21a/include" -I "CMSIS/Core/Include"  -Os -ffunction-sections -mlong-calls -Wall -MMD -MF "${OBJECTDIR}/samd21a/gcc/gcc/newfile.o.d" -o ${OBJECTDIR}/samd21a/gcc/gcc/newfile.o samd21a/gcc/gcc/newfile.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD) 
	@${FIXDEPS} "${OBJECTDIR}/samd21a/gcc/gcc/newfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SAM_IOT_WINC_PROV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m0plus   -gdwarf-2  -D__$(MP_PROCESSOR_OPTION)__    -mthumb --specs=nosys.specs -Wl,-Map="dist/${CND_CONF}/${IMAGE_TYPE}/SAM_IOT_WINC_PROV.X.${IMAGE_TYPE}.map"  -o dist/${CND_CONF}/${IMAGE_TYPE}/SAM_IOT_WINC_PROV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1  -L"samd21a/gcc/gcc" -Wl,--gc-sections   -T"samd21a/gcc/gcc/samd21g18a_flash.ld" 
	
	
	
	
	
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/SAM_IOT_WINC_PROV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m0plus  -D__$(MP_PROCESSOR_OPTION)__    -mthumb --specs=nosys.specs -Wl,-Map="dist/${CND_CONF}/${IMAGE_TYPE}/SAM_IOT_WINC_PROV.X.${IMAGE_TYPE}.map"  -o dist/${CND_CONF}/${IMAGE_TYPE}/SAM_IOT_WINC_PROV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)  -L"samd21a/gcc/gcc" -Wl,--gc-sections   -T"samd21a/gcc/gcc/samd21g18a_flash.ld" 
	
	${MP_CC_DIR}/arm-none-eabi-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature "dist/${CND_CONF}/${IMAGE_TYPE}/SAM_IOT_WINC_PROV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/SAM_IOT_WINC_PROV.X.${IMAGE_TYPE}.hex"
	
	
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Default
	${RM} -r dist/Default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
