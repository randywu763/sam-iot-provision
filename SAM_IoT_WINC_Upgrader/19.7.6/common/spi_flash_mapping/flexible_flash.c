
#include "driver/include/m2m_types.h"
#include "spi_flash/include/spi_flash.h"
#include "flexible_flash.h"

#define FLASH_MAP_TABLE_ADDR        (FLASH_SECTOR_SZ+sizeof(tstrOtaControlSec)+8)
#define N_ENTRIES_MAX               32

sint8 spi_flexible_flash_find_section(uint16 u16EntryIDToLookFor, uint32 *pu32StartOffset, uint32 *pu32Size)
{
    sint8 s8Ret = M2M_ERR_INVALID_ARG;
    if((NULL == pu32StartOffset) || (NULL == pu32Size)) goto EXIT;

    uint8 au8buff[8];
    uint8 u8CurrEntry = 0;
    s8Ret = spi_flash_read(&au8buff[0], FLASH_MAP_TABLE_ADDR, 4);
    if(M2M_SUCCESS != s8Ret) goto EXIT;

    uint8 u8nEntries = au8buff[0];     // Max number is 32, reading one byte will suffice
    if(u8nEntries > N_ENTRIES_MAX)
    {
        s8Ret = M2M_ERR_FAIL;
        goto EXIT;
    }

    while(u8nEntries > u8CurrEntry)
    {
        s8Ret = spi_flash_read(&au8buff[0], FLASH_MAP_TABLE_ADDR + 4 + (u8CurrEntry*8), 8);
        u8CurrEntry++;
        if(M2M_SUCCESS != s8Ret) break;
        uint16 u16EntryID = (au8buff[1] << 8) | au8buff[0];
        if(u16EntryID != u16EntryIDToLookFor) continue;
        *pu32StartOffset = au8buff[2] * FLASH_SECTOR_SZ;
        *pu32Size        = au8buff[3] * FLASH_SECTOR_SZ;
        break;
    }
EXIT:
    return s8Ret;
}