/**
*  @file    common_values.h
*  @brief   This module contains macros and variables between all projects and firmware
*  @author  Awad A. Bekhet
*  @date    1 JAN 2015
*  @version 1.0
*/

#ifndef __COMMON_VALUES__
#define __COMMON_VALUES__
/********************************************/
/* INCLUDES                                 */
/********************************************/




/********************************************/
/* MACROS                                   */
/********************************************/
#define TX_GAIN_NUM_OF_REGISTERS            7
/*!< Number of registers to hold TX gain values
*/
#define TX_GAIN_REG_BASE_ADDRESS            0x1240
/*!< Base address register of gain registers
*/
#ifdef NMC1003A0_HP_MULT_GAIN
#define TX_MULT_GAIN_VALIDATION_NUMBER      0x5FB72E6C
/*!< Values to check if there is a valid data in Flash or not
*/
#endif
#define TX_GAIN_VALIDATION_NUMBER           0x4FA61D5B
/*!< Values to check if there is a valid data in Flash or not
 */
#define TX_GAIN_VER_NUMBER                  (uint32)(0x00000100)
/*!< Values to check if there is a valid data in Flash or not
 */


#define PLL_MAGIC_NUMBER                    0x12345675
/*!< Magic Value to check if there is a valid PLL data in Flash or not
 * if the pll settings changed you need to change the magic number
 */
#define PLL_MAX_NUM_CHANNELS                14
/*!< Max Number of Channels supported in 2G4 Band
 */
#define VBAT_NUM_LEVELS                     3

#define HP_MAX_NUM_GAIN_TABLES              4
/*!< Max number of gain tables supported
*/



/********************************************/
/* UNIONS                                   */
/********************************************/



/********************************************/
/* STRUCTURES                                */
/********************************************/
/*!
@struct \
     tstrChannelParm

@brief
     Used to hold PLL values
 */
typedef struct
{
    uint32  u32PllInternal1;
    uint32  u32PllInternal4;
    uint32  WlanRx1;
    uint32  WlanRx2;
    uint32  WlanRx3;
    uint32  WlanTx1;
    uint32  WlanTx2;
    uint32  WlanTx3;
}tstrChannelParm;




/*!
@struct \
     tstrGain_Struct

@brief
     Used to hold TX gain settings values
 */
typedef struct
{
    uint32 reg_40;
    uint32 reg_44;
    uint32 reg_48;
    uint32 reg_4C;
    uint32 reg_50;
    uint32 reg_54;
    uint32 reg_58;
    /*!< Variables to hold Gain registers' values
     *   Where: 40:-> means 0x1240 and so on.
     */
}tstrDGain;

#ifdef NMC1003A0_HP_MULT_GAIN

/*!
@struct \
     tstrGainHdr

@brief
     Used to hold TX gain settings Hdr
 */

#define __PADDING_tstrGainHdr_ (32-8-(HP_MAX_NUM_GAIN_TABLES *4))
typedef struct
{
    uint32 magic;
    /*!< Used to check if Flash has a valid data or not
     */
    uint32 HpGainTbValid[HP_MAX_NUM_GAIN_TABLES];
    uint32 HpActiveGTbIndx; /* HP Gain Table to be used/currently in use */
    uint8  __PAD__[__PADDING_tstrGainHdr_];
}tstrGainHdr;

typedef struct
{
    tstrDGain strHpGain[PLL_MAX_NUM_CHANNELS];
} tstrDGain_Hp;

typedef struct
{
    tstrGainHdr  GainHdr;
    tstrDGain_Hp HpGain[HP_MAX_NUM_GAIN_TABLES]; /* HP_MAX_NUM_GAIN_TABLES=4 */
}tstrGain_Struct;

#else

typedef struct
{
    uint32 magic;
    /*!< Used to check if Flash has a valid data or not
     */
    uint32 HpGainValid;
    uint32 LpPAGainValidVbat[VBAT_NUM_LEVELS];
    uint32 LpDGainValidVbat[VBAT_NUM_LEVELS];

}tstrGainHdr;


/*!
@struct \
     tstrGain_Struct

@brief
     Used to hold TX gain settings values
 */
typedef struct
{
    uint32 Vbat;
    uint32 PA_1e9c;
    uint32 PA_1edc;
}tstrLpPAGain_Vbat;

/*!
@struct \
     tstrGain_Struct

@brief
     Used to hold TX gain settings values
 */
typedef struct
{
    tstrDGain strlPGain[PLL_MAX_NUM_CHANNELS];
}tstrLpDGain_Vbat;
/*!
@struct \
     tstrGain_Struct

@brief
     Used to hold  gain settings values
 */
typedef struct
{
    tstrGainHdr GainHdr;
    tstrDGain   strHpGain[PLL_MAX_NUM_CHANNELS];
    tstrLpPAGain_Vbat strLpPAGain[VBAT_NUM_LEVELS];
    tstrLpDGain_Vbat  strLPGainVbat[VBAT_NUM_LEVELS];
}tstrGain_Struct;

#endif // NMC1003A0_HP_MULT_GAIN

#endif  //__COMMON_VALUES__
