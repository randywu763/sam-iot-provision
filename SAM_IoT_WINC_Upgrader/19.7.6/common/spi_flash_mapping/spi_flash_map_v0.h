/**
*  @file        spi_flash_map.h
*  @brief       This module contains spi flash CONTENT
*  @author      M.S.M
*  @date        17 SEPT 2013
*  @version     1.0
*/
#ifndef __SPI_MAP_V0_H__
#define __SPI_MAP_V0_H__

#include "spi_flash_map.h"

#define _PROGRAM_POWER_SAVE_
/*
 * Detailed Sizes and locations for Flash Memory:
 *  ____________________ ___________ ___________________________ _______________________________________________
 * | Starting Address   |   Size    |    Location's Name        |    Description                                |
 * |____________________|___________|___________________________|_______________________________________________|
 * |      0 K           |     4 K   |    Boot Firmware          |    Firmware to select which version to run    |
 * |      4 K           |     8 K   |    Control Section        |    Structured data used by Boot firmware      |
 * |     12 K           |     4 K   |    PLL+GAIN :             |    LookUp Table for PLL and Gain calculations |
 * |                    |                PLL  Size = 1K         |       PLL                                     |
 * |                    |           |    GAIN Size = 3K         |       Gain configuration                      |
 * |     16 K           |     4 K   |    CERTIFICATE            |    X.509 Certificate storage                  |
 * |     20 K           |     4 K   |    Scratch Section        |    Empty Section                              |
 * |     24 K           |     2 K   |    Program Firmware       |                                               |
 * |     26 K           |   194 K   |    Main Firmware          |    Main Firmware to run WiFi Chip             |
 * |    220 K           |     8 K   |    HTTP Files             |    Files used with Provisioning Mode          |
 * |    228 K           |     0 K   |    PS_Firmware            |    Power Save Firmware                        |
 * |    228 K           |     4 K   |    Connection Parameters  |    Parameters for success connection to AP    |
 * |    232 K           |    24 K   |    Application            |    Cortus App                                 |
 *
 * |    256 K           |     2 K   |    Program Firmware       |                                               |
 * |    258 K           |   194 K   |    Main Firmware          |    Main Firmware to run WiFi Chip             |
 * |    452 K           |     8 K   |    HTTP Files             |    Files used with Provisioning Mode          |
 * |    460 K           |     0 K   |    PS_Firmware            |    Power Save Firmware                        |
 * |    460 K           |     4 K   |    Connection Parameters  |    Parameters for success connection to AP    |
 * |    464 k           |     8 K   |   Empty                   |                                               |
 * |    472 k           |    40 k   |                           |   CORTUS APP                                  |
 * |____________________|___________|___________________________|_______________________________________________|
 *
 *
 * *Keys for Comments with each MACRO:
 *      "L:xxxK" -means-> location  :xxxK
 *      "S:xxxK" -means-> Size is   :xxxK
 */

/* Boot Firmware: which used to select which firmware to run
 * starting Address L:  0 K |
 * Size             S:  4 K |
 *
 */
#define SFMV0_BOOT_FIRMWARE_STARTING_ADDR       (FLASH_START_ADDR)
#define SFMV0_BOOT_FIRMWARE_SZ                  (FLASH_SECTOR_SZ)

/* Control Section: which used by Boot firmware
 * starting Address L:  4 K |
 * Size             S:  8 K |
 *
 */
#define SFMV0_CONTROL_OFFSET            (SFMV0_BOOT_FIRMWARE_STARTING_ADDR + SFMV0_BOOT_FIRMWARE_SZ)
#define SFMV0_CONTROL_BKP_OFFSET        (SFMV0_CONTROL_OFFSET + FLASH_SECTOR_SZ)
#define SFMV0_CONTROL_SEC_SZ            (FLASH_SECTOR_SZ)
#define SFMV0_CONTROL_TOTAL_SZ          (FLASH_SECTOR_SZ * 2)

/* LUT for PLL and TX Gain settings:
 * starting Address L: 12 K |
 * Size             S:  4 K |
 *
 */
#define SFMV0_PLL_OFFSET                (SFMV0_CONTROL_OFFSET + SFMV0_CONTROL_TOTAL_SZ)
#define SFMV0_PLL_SZ                    (1024 * 1)
#define SFMV0_GAIN_OFFSET               (SFMV0_PLL_OFFSET + SFMV0_PLL_SZ)
#define SFMV0_GAIN_SZ                   (SFMV0_CONFIG_SECT_TOTAL_SZ - SFMV0_PLL_SZ)
#define SFMV0_CONFIG_SECT_TOTAL_SZ      (FLASH_SECTOR_SZ)

/* Certificate:
 * starting Address     L: 16 K |
 * Size for CERTIFICATE S:  3 K |
 * Size     RESERVED    S:  1 K |
 *
 */
#define SFMV0_TLS_ROOTCERT_CACHE_OFFSET         (SFMV0_PLL_OFFSET + SFMV0_CONFIG_SECT_TOTAL_SZ)
#define SFMV0_TLS_ROOTCERT_CACHE_SIZE           (FLASH_SECTOR_SZ * 1)

/* Scratch:
 * starting Address     L: 20 K |
 * Size for Scratch     S:  4 K |
 *
 */
#define SFMV0_TLS_SESSION_CACHE_OFFSET      (SFMV0_TLS_ROOTCERT_CACHE_OFFSET + SFMV0_TLS_ROOTCERT_CACHE_SIZE)
#define SFMV0_TLS_SESSION_CACHE_SIZE        (FLASH_SECTOR_SZ * 1)

/*
 *
 * OTA image1 Offset
 */
#define SFMV0_OTA_IMAGE1_OFFSET                 (SFMV0_TLS_SESSION_CACHE_OFFSET + SFMV0_TLS_SESSION_CACHE_SIZE)
/*
 * Firmware Offset
 *
 */
#if (defined _FIRMWARE_)||(defined OTA_GEN)
#define SFMV0_FIRMWARE_OFFSET            (0UL)
#else
#if (defined DOWNLOAD_ROLLBACK)
#define SFMV0_FIRMWARE_OFFSET            (SFMV0_OTA_IMAGE2_OFFSET)
#else
#define SFMV0_FIRMWARE_OFFSET            (SFMV0_OTA_IMAGE1_OFFSET)
#endif
#endif
/*
 * To define Starting Address and size for Firmware
 *  ____________________ _______________________ ___________________________
 * |--------------------|   Power Save Program  |   Non Power Save Program  |
 * |____________________|_______________________|___________________________|
 * |SFMV0_PROGRAM_SZ    |            2 K        |          0 K              |
 * |Starting Address    |            2 K        |          0 K              |
 * |Size                |          194 K        |        196 K              |
 * |____________________|_______________________|___________________________|
 *
 */
#ifdef _PROGRAM_POWER_SAVE_
#define SFMV0_PROGRAM_SZ                (1024 * 2UL)
#else
#define SFMV0_PROGRAM_SZ                (0UL)
#endif  /* _PROGRAM_POWER_SAVE_ */
#define SFMV0_FIRMWARE_SZ               (196*1024UL)

/* HTTP Files
 * starting Address L:204 K |
 * Size             S:  8 K |
 *
 */
#define SFMV0_HTTP_MEM_OFFSET               (SFMV0_FIRMWARE_OFFSET + SFMV0_FIRMWARE_SZ)
#define SFMV0_HTTP_MEM_SZ                   (FLASH_SECTOR_SZ * 2)

/* ps_Firmware(Power Save Firmware): App. which runs for power saving purpose
 * starting Address L:212 K |
 * Size             S: 0 K |
 *
 */
#define SFMV0_PS_FIRMWARE_OFFSET            (SFMV0_HTTP_MEM_OFFSET + SFMV0_HTTP_MEM_SZ)
#define SFMV0_PS_FIRMWARE_SZ                (FLASH_SECTOR_SZ * 0)

/* Saved Connection Parameters:
 * starting Address L:224 K |
 * Size             S:  4 K |
 *
 */
#define SFMV0_CACHED_CONNS_OFFSET           (SFMV0_PS_FIRMWARE_OFFSET + SFMV0_PS_FIRMWARE_SZ)
#define SFMV0_CACHED_CONNS_SZ               (FLASH_SECTOR_SZ * 1)

/* App(Cortus App): App. which runs over firmware
 * starting Address L:232 K |
 * Size for Code    S: 20 K |
 * Size for Data    S:  4 K |
 *
 */
#define SFMV0_APP_2M_MEM_OFFSET             (SFMV0_CACHED_CONNS_OFFSET + SFMV0_CACHED_CONNS_SZ)
#define SFMV0_APP_2M_MEM_SZ                 (FLASH_SECTOR_SZ * 6)
#define SFMV0_APP_4M_MEM_SZ                 (FLASH_SECTOR_SZ * 10)
#define SFMV0_APP_4M_MEM_OFFSET             (FLASH_4M_TOTAL_SZ - SFMV0_APP_4M_MEM_SZ)
/**
 *
 * OTA image Size
 */
#define SFMV0_OTA_IMAGE_SIZE                (SFMV0_CACHED_CONNS_SZ + SFMV0_HTTP_MEM_SZ + SFMV0_FIRMWARE_SZ)
/**
 *
 * Flash Total size
 */
#define SFMV0_IMAGE1_CONTENT_SZ             (SFMV0_BOOT_FIRMWARE_SZ + SFMV0_CONTROL_TOTAL_SZ + SFMV0_CONFIG_SECT_TOTAL_SZ + \
                                            SFMV0_TLS_ROOTCERT_CACHE_SIZE + SFMV0_TLS_SESSION_CACHE_SIZE + OTA_IMAGE_SIZE + \
                                            SFMV0_PS_FIRMWARE_SZ + SFMV0_APP_2M_MEM_SZ)
/**
 *
 * OTA image 2 offset
 */
#define SFMV0_OTA_IMAGE2_OFFSET             (FLASH_IMAGE1_CONTENT_SZ)



#endif /* __SPI_MAP_V0_H__ */
