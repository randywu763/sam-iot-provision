#ifndef _EFUSE_H_
#define _EFUSE_H_

#define NUM_EFUSE_BANKS (6)

#ifdef _FIRMWARE_
#include "m2m_common.h"
#include "common.h"
#else
#include "bsp/include/nm_bsp.h"
#include "driver/source/nmbus.h"
#endif

#define EFUSE_ERR_CANT_LOAD_DATA        -2
#define EFUSE_ERR_INVALID_BANK_OR_DATA  -1
#define EFUSE_SUCCESS                   0

typedef struct {
    uint8 ver:3;
    uint8 bank_idx:3;           /* bank number (index) */
    uint8 bank_used:1;          /* =1 if bank is used otherwise =0 */
    uint8 bank_invalid:1;       /* =1 if bank has INVALID data otherwise = 0 */
    uint8 MAC_addr_used:1;      /* =1 if mac address is used otherwise =0 */

    uint8 PATxGainCorr:7;
    uint8 PATxGainCorr_used:1;
    uint16 FreqOffset:15;       /* has frequency for tuner */
    uint16 FreqOffset_used:1;   /* =1 if frequency for tuner is used otherwise =0 */

    uint8 MAC_addr[6];          /* has The MAC address value */

   	uint8 bIsIQAmpValid;
	uint16 IQAmpdB;
	uint8 bIsIQPhaValid;
	uint16 IQPhaDeg;

} EFUSEProdStruct;

sint8 read_efuse_struct(EFUSEProdStruct *efuse_struct, uint8 skip_bank_check);
void dump_efuse_struct(EFUSEProdStruct *efuse_struct);
sint8 is_efuse_bank_loaded(uint8 bankIdx);
void program_tx_iq_aic_pic_ctl(void);

extern EFUSEProdStruct g_efuse_struct;

#ifndef _FIRMWARE_

sint8 overwrite_efuse_struct(EFUSEProdStruct *efuse_struct, int bankIdx);

#endif /* _FIRMWARE_ */
#endif /* _EFUSE_H_ */
