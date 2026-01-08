/*****************************************************************************
 smpl_common.h

 Copyright (C) 2024 ROHM Co., Ltd.
 All rights reserved.

 This software is provided "as is" and any expressed or implied
 warranties, including, but not limited to, the implied warranties of
 merchantability and fitness for a particular purpose are disclaimed.
 ROHM shall not be liable for any direct, indirect, consequential or
 incidental damages arising from using or modifying this software.
 You (customer) can modify and use this software in whole or part on
 your own responsibility, only for the purpose of developing the software
 for use with microcontroller manufactured by ROHM.

 History
    2024.07.31 Ver 1.0.0

******************************************************************************/
/**
 * @file    smpl_common.h
 *
 * This file is common definition for sample programs.
 *
 */
#ifndef SMPL_COMMON_H__
#define SMPL_COMMON_H__

#include "mcu.h"
#include "clock.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
#define TM0_PERI        ( 0x00000001U )
#define TM1_PERI        ( 0x00000002U )
#define TM2_PERI        ( 0x00000004U )
#define TM3_PERI        ( 0x00000008U )
#define TM4_PERI        ( 0x00000010U )
#define TM5_PERI        ( 0x00000020U )
#define FTM0_PERI       ( 0x00000100U )
#define FTM1_PERI       ( 0x00000200U )
#define NTMS_PERI       ( 0x00001000U )
#define SAD0_PERI       ( 0x00002000U )
#define SAD1_PERI       ( 0x00004000U )
#define TM1K_PERI       ( 0x00008000U )
#define SIOF0_PERI      ( 0x00010000U )
#define SIOF1_PERI      ( 0x00020000U )
#define UAF0_PERI       ( 0x00040000U )
#define UAF1_PERI       ( 0x00080000U )
#define UAF2_PERI       ( 0x00100000U )
#define I2CF0_PERI      ( 0x00200000U )
#define UAF3_PERI       ( 0x00400000U )
#define CAN_PERI        ( 0x00800000U )
#define RTC_PERI        ( 0x04000000U )
#define VLS_PERI        ( 0x08000000U )
#define CMP_PERI        ( 0x1000
#define DMAC_PERI       ( 0x20000000U )
#define AI_PERI         ( 0x40000000U )
#define CRC_PERI        ( 0x80000000U )

#define MCS_INT_FLCINT      ( 0x00000001U )
#define MCS_INT_RAM_PAR     ( 0x00000010U )
#define MCS_INT_AI_PAR      ( 0x00000020U )
#define MCS_INT_CAN_PAR     ( 0x00000040U )
#define MCS_INT_CAN_BUS_ERR ( 0x00000100U )

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/* Prototype */
void smpl_setLsCrystal32Khz( void );
void smpl_setHsCrystal20Mhz( void );
void smpl_setHsCrystal40Mhz( void );
void smpl_setHsPll48Mhz( uint32_t xspen, uint32_t hxspen );
void smpl_enablePeripheral( uint32_t on );
void smpl_disablePeripheral( uint32_t off );
void smpl_enableMcsInterrupt( uint32_t on );
void smpl_disableMcsInterrupt( uint32_t off );
uint32_t smpl_getMcsInterruptStatus( void );
void smpl_clearMcsInterruptStatus( uint32_t clear );

#endif /* SMPL_COMMON_H__ */
