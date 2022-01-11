
#ifndef __FLEXIBLE_FLASH_H__
#define __FLEXIBLE_FLASH_H__

typedef struct {
    uint32 magic;
    uint32 max_size;
}tstrFlashLUTHeader;

// NOTE: Don't use enums for id/status here,
// they need to be 16 bit but the enums end up as
// 32 bit even if the __packed__ attribute is used
typedef struct {
    uint16 id;
    uint8 sector;
    uint8 size;
    uint32 reserved;
}tstrFlashLUTEntry;

#define FLASHMAP_MAGIC_VALUE			0x1ABCDEF9
#define FLASHMAP_MAX_ENTRIES			32

// + 8 is for the number of entries value (uint32) and CRC (uint32)
// * 2 is for the new lookup table to apply
// + 48 is for the progress monitor
#define FLASHMAP_MAX_SIZE				(sizeof(tstrFlashLUTHeader) + (((sizeof(tstrFlashLUTEntry) * FLASHMAP_MAX_ENTRIES) + 8) * 2) + 48)


/** @defgroup SPiFlashRead spi_flexible_flash_find_section
 *  @ingroup SPIFLASHAPI
 */
  /**@{*/
/*!
 * @fn          sint8 spi_flexible_flash_find_section(uint16 u16EntryIDToLookFor, uint32 *pu32StartOffset, uint32 *pu32Size);
 * @brief       Read the Flash Map to extract the host file starting offset.\n
 * @param [in]  u16EntryIDToLookFor
 *                  The ID of the location in Flash we are looking for. See @ref tenuFlashLUTEntryID.
 * @param [in]  pu32StartOffset
 *                  Pointer to the variable where the Flash section start address should be stored.
 * @param [in]  pu32Size
 *                  Pointer to the variable where the Flash section size should be stored.
 * @warning
 *              In case there is a running WINC firmware, it is required to pause the firmware
 *              first before any trial to access SPI flash to avoid any racing between host and
 *              running firmware on bus. @ref m2m_wifi_download_mode can be used to pause the firmware.
 * @sa          m2m_wifi_download_mode
 *              m2m_wifi_init_hold
 * @return      The function returns @ref M2M_SUCCESS for successful operations and a negative value otherwise.

 */
 sint8 spi_flexible_flash_find_section(uint16 u16EntryIDToLookFor, uint32 *pu32StartOffset, uint32 *pu32Size);
 /**@}*/

#endif /* __FLEXIBLE_FLASH_H__ */
