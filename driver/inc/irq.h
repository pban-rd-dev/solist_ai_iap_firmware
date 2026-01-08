/*****************************************************************************
 irq.h

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
 * @file    irq.h
 *
 * This module is 'IRQ' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup IRQ
 *  @{
 *
 *  @brief Interrupt register related
 *
 *  @file
 */

#ifndef IRQ_H__
#define IRQ_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== bit number of registers ===*/
/* EXICONn */
#define EXI0_EDGE_BIT_NO                ( 0 )                   /**< EXICONn Register (EXI0 edge configuration) bit No.  */
#define EXI1_EDGE_BIT_NO                ( 4 )                   /**< EXICONn Register (EXI1 edge configuration) bit No.  */
#define EXI2_EDGE_BIT_NO                ( 8 )                   /**< EXICONn Register (EXI2 edge configuration) bit No.  */
#define EXI3_EDGE_BIT_NO                (12 )                   /**< EXICONn Register (EXI3 edge configuration) bit No.  */
#define EXI4_EDGE_BIT_NO                (16 )                   /**< EXICONn Register (EXI4 edge configuration) bit No.  */
#define EXI5_EDGE_BIT_NO                (20 )                   /**< EXICONn Register (EXI5 edge configuration) bit No.  */
#define EXI6_EDGE_BIT_NO                (24 )                   /**< EXICONn Register (EXI6 edge configuration) bit No.  */
#define EXI7_EDGE_BIT_NO                (28 )                   /**< EXICONn Register (EXI7 edge configuration) bit No.  */

#define EXI0_SAMPLE_BIT_NO              ( 2 )                   /**< EXICONn Register (EXI0 sampling configuration) bit No.  */
#define EXI1_SAMPLE_BIT_NO              ( 6 )                   /**< EXICONn Register (EXI1 sampling configuration) bit No.  */
#define EXI2_SAMPLE_BIT_NO              (10 )                   /**< EXICONn Register (EXI2 sampling configuration) bit No.  */
#define EXI3_SAMPLE_BIT_NO              (14 )                   /**< EXICONn Register (EXI3 sampling configuration) bit No.  */
#define EXI4_SAMPLE_BIT_NO              (18 )                   /**< EXICONn Register (EXI4 sampling configuration) bit No.  */
#define EXI5_SAMPLE_BIT_NO              (22 )                   /**< EXICONn Register (EXI5 sampling configuration) bit No.  */
#define EXI6_SAMPLE_BIT_NO              (26 )                   /**< EXICONn Register (EXI6 sampling configuration) bit No.  */
#define EXI7_SAMPLE_BIT_NO              (30 )                   /**< EXICONn Register (EXI7 sampling configuration) bit No.  */

#define EXI0_FILTER_BIT_NO              ( 3 )                   /**< EXICONn Register (EXI0 filtering configuration) bit No.  */
#define EXI1_FILTER_BIT_NO              ( 7 )                   /**< EXICONn Register (EXI1 filtering configuration) bit No.  */
#define EXI2_FILTER_BIT_NO              (11 )                   /**< EXICONn Register (EXI2 filtering configuration) bit No.  */
#define EXI3_FILTER_BIT_NO              (15 )                   /**< EXICONn Register (EXI3 filtering configuration) bit No.  */
#define EXI4_FILTER_BIT_NO              (19 )                   /**< EXICONn Register (EXI4 filtering configuration) bit No.  */
#define EXI5_FILTER_BIT_NO              (23 )                   /**< EXICONn Register (EXI5 filtering configuration) bit No.  */
#define EXI6_FILTER_BIT_NO              (27 )                   /**< EXICONn Register (EXI6 filtering configuration) bit No.  */
#define EXI7_FILTER_BIT_NO              (31 )                   /**< EXICONn Register (EXI7 filtering configuration) bit No.  */

/*=== API parameters value ===*/
/* external interrupt edge */
#define EXIn_EDGE_DISABLE               ( 0 )                   /**< [EXIn]Interrupt disabled             */
#define EXIn_EDGE_FALLING               ( 1 )                   /**< [EXIn]Falling-edge interrupt mode    */
#define EXIn_EDGE_RISING                ( 2 )                   /**< [EXIn]Rising-edge interrupt mode     */
#define EXIn_EDGE_BOTH                  ( 3 )                   /**< [EXIn]Both-edge interrupt mode       */

/* external interrupt sampling mode */
#define EXIn_SAMPLING_DIS               ( 0 )                   /**< [EXIn]Without sampling               */
#define EXIn_SAMPLING_ENA               ( 1 )                   /**< [EXIn]With sampling                  */

/* external interrupt filter mode */
#define EXIn_FILTER_DIS                 ( 0 )                   /**< [EXIn]Without noise filter           */
#define EXIn_FILTER_ENA                 ( 1 )                   /**< [EXIn]With noise filter              */

/* external interrupt port */
#define EXIn_PORT_SEL_P00               ( 0x00 | 0x00 )         /**< [EXIn]Select Port (P00)              */
#define EXIn_PORT_SEL_P01               ( 0x00 | 0x01 )         /**< [EXIn]Select Port (P01)              */
#define EXIn_PORT_SEL_P02               ( 0x00 | 0x02 )         /**< [EXIn]Select Port (P02)              */
#define EXIn_PORT_SEL_P03               ( 0x00 | 0x03 )         /**< [EXIn]Select Port (P03)              */
#define EXIn_PORT_SEL_P04               ( 0x00 | 0x04 )         /**< [EXIn]Select Port (P04)              */
#define EXIn_PORT_SEL_P05               ( 0x00 | 0x05 )         /**< [EXIn]Select Port (P05)              */

#define EXIn_PORT_SEL_P20               ( 0x20 | 0x00 )         /**< [EXIn]Select Port (P20)              */
#define EXIn_PORT_SEL_P21               ( 0x20 | 0x01 )         /**< [EXIn]Select Port (P21)              */
#define EXIn_PORT_SEL_P22               ( 0x20 | 0x02 )         /**< [EXIn]Select Port (P22)              */
#define EXIn_PORT_SEL_P23               ( 0x20 | 0x03 )         /**< [EXIn]Select Port (P23)              */

#define EXIn_PORT_SEL_P30               ( 0x30 | 0x00 )         /**< [EXIn]Select Port (P30)              */
#define EXIn_PORT_SEL_P31               ( 0x30 | 0x01 )         /**< [EXIn]Select Port (P31)              */
#define EXIn_PORT_SEL_P32               ( 0x30 | 0x02 )         /**< [EXIn]Select Port (P32)              */
#define EXIn_PORT_SEL_P33               ( 0x30 | 0x03 )         /**< [EXIn]Select Port (P33)              */
#define EXIn_PORT_SEL_P34               ( 0x30 | 0x04 )         /**< [EXIn]Select Port (P34)              */
#define EXIn_PORT_SEL_P35               ( 0x30 | 0x05 )         /**< [EXIn]Select Port (P35)              */
#define EXIn_PORT_SEL_P36               ( 0x30 | 0x06 )         /**< [EXIn]Select Port (P36)              */
#define EXIn_PORT_SEL_P37               ( 0x30 | 0x07 )         /**< [EXIn]Select Port (P37)              */

#define EXIn_PORT_SEL_P40               ( 0x40 | 0x00 )         /**< [EXIn]Select Port (P40)              */
#define EXIn_PORT_SEL_P41               ( 0x40 | 0x01 )         /**< [EXIn]Select Port (P41)              */
#define EXIn_PORT_SEL_P42               ( 0x40 | 0x02 )         /**< [EXIn]Select Port (P42)              */
#define EXIn_PORT_SEL_P43               ( 0x40 | 0x03 )         /**< [EXIn]Select Port (P43)              */
#define EXIn_PORT_SEL_P44               ( 0x40 | 0x04 )         /**< [EXIn]Select Port (P44)              */
#define EXIn_PORT_SEL_P45               ( 0x40 | 0x05 )         /**< [EXIn]Select Port (P45)              */
#define EXIn_PORT_SEL_P46               ( 0x40 | 0x06 )         /**< [EXIn]Select Port (P46)              */
#define EXIn_PORT_SEL_P47               ( 0x40 | 0x07 )         /**< [EXIn]Select Port (P47)              */

#define EXIn_PORT_SEL_P50               ( 0x50 | 0x00 )         /**< [EXIn]Select Port (P50)              */
#define EXIn_PORT_SEL_P51               ( 0x50 | 0x01 )         /**< [EXIn]Select Port (P51)              */
#define EXIn_PORT_SEL_P52               ( 0x50 | 0x02 )         /**< [EXIn]Select Port (P52)              */
#define EXIn_PORT_SEL_P53               ( 0x50 | 0x03 )         /**< [EXIn]Select Port (P53)              */
#define EXIn_PORT_SEL_P54               ( 0x50 | 0x04 )         /**< [EXIn]Select Port (P54)              */
#define EXIn_PORT_SEL_P55               ( 0x50 | 0x05 )         /**< [EXIn]Select Port (P55)              */
#define EXIn_PORT_SEL_P56               ( 0x50 | 0x06 )         /**< [EXIn]Select Port (P56)              */
#define EXIn_PORT_SEL_P57               ( 0x50 | 0x07 )         /**< [EXIn]Select Port (P57)              */

#define EXIn_PORT_SEL_P60               ( 0x60 | 0x00 )         /**< [EXIn]Select Port (P60)              */
#define EXIn_PORT_SEL_P61               ( 0x60 | 0x01 )         /**< [EXIn]Select Port (P61)              */
#define EXIn_PORT_SEL_P62               ( 0x60 | 0x02 )         /**< [EXIn]Select Port (P62)              */
#define EXIn_PORT_SEL_P63               ( 0x60 | 0x03 )         /**< [EXIn]Select Port (P63)              */
#define EXIn_PORT_SEL_P64               ( 0x60 | 0x04 )         /**< [EXIn]Select Port (P64)              */
#define EXIn_PORT_SEL_P65               ( 0x60 | 0x05 )         /**< [EXIn]Select Port (P65)              */
#define EXIn_PORT_SEL_P66               ( 0x60 | 0x06 )         /**< [EXIn]Select Port (P66)              */

#define EXIn_PORT_SEL_P70               ( 0x70 | 0x00 )         /**< [EXIn]Select Port (P70)              */
#define EXIn_PORT_SEL_P71               ( 0x70 | 0x01 )         /**< [EXIn]Select Port (P71)              */
#define EXIn_PORT_SEL_P72               ( 0x70 | 0x02 )         /**< [EXIn]Select Port (P72)              */
#define EXIn_PORT_SEL_P73               ( 0x70 | 0x03 )         /**< [EXIn]Select Port (P73)              */
#define EXIn_PORT_SEL_P74               ( 0x70 | 0x04 )         /**< [EXIn]Select Port (P74)              */
#define EXIn_PORT_SEL_P75               ( 0x70 | 0x05 )         /**< [EXIn]Select Port (P75)              */
#define EXIn_PORT_SEL_P76               ( 0x70 | 0x06 )         /**< [EXIn]Select Port (P76)              */
#define EXIn_PORT_SEL_P77               ( 0x70 | 0x07 )         /**< [EXIn]Select Port (P77)              */

#define EXIn_PORT_SEL_P80               ( 0x80 | 0x00 )         /**< [EXIn]Select Port (P80)              */
#define EXIn_PORT_SEL_P81               ( 0x80 | 0x01 )         /**< [EXIn]Select Port (P81)              */
#define EXIn_PORT_SEL_P82               ( 0x80 | 0x02 )         /**< [EXIn]Select Port (P82)              */
#define EXIn_PORT_SEL_P83               ( 0x80 | 0x03 )         /**< [EXIn]Select Port (P83)              */
#define EXIn_PORT_SEL_P84               ( 0x80 | 0x04 )         /**< [EXIn]Select Port (P84)              */
#define EXIn_PORT_SEL_P85               ( 0x80 | 0x05 )         /**< [EXIn]Select Port (P85)              */

#define EXIn_PORT_SEL_VDDI              ( 0xF0 | 0x0F )         /**< [EXIn]Select Port (sel_vddi signal)  */

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/*--- Enables interrupts ---*/
#define         irq_exi_ena()            NVIC_EnableIRQ( EXI_IRQn )         /**< Enables EXIINT   */
#define         irq_siof0_ena()          NVIC_EnableIRQ( SIOF0_IRQn )       /**< Enables SIOF0INT */
#define         irq_siof1_ena()          NVIC_EnableIRQ( SIOF1_IRQn )       /**< Enables SIOF1INT */
#define         irq_uaf0_ena()           NVIC_EnableIRQ( UAF0_IRQn )        /**< Enables UAF0INT  */
#define         irq_uaf1_ena()           NVIC_EnableIRQ( UAF1_IRQn )        /**< Enables UAF1INT  */
#define         irq_uaf2_ena()           NVIC_EnableIRQ( UAF2_IRQn )        /**< Enables UAF2INT  */
#define         irq_uaf3_ena()           NVIC_EnableIRQ( UAF3_IRQn )        /**< Enables UAF3INT  */
#define         irq_i2cf0_ena()          NVIC_EnableIRQ( I2CF0_IRQn )       /**< Enables I2CF0INT */
#define         irq_clk_ena()            NVIC_EnableIRQ( CLK_IRQn )         /**< Enables CLKINT   */
#define         irq_vls_ena()            NVIC_EnableIRQ( VLS_IRQn )         /**< Enables VLSINT   */
#define         irq_sad0_ena()           NVIC_EnableIRQ( SAD_IRQn )         /**< Enables SADINT   */
#define         irq_sad1_ena()           NVIC_EnableIRQ( SAD1_IRQn )        /**< Enables SAD1INT  */
#define         irq_tm0_ena()            NVIC_EnableIRQ( TM0_IRQn )         /**< Enables TM0INT   */
#define         irq_tm1_ena()            NVIC_EnableIRQ( TM1_IRQn )         /**< Enables TM1INT   */
#define         irq_tm2_ena()            NVIC_EnableIRQ( TM2_IRQn )         /**< Enables TM2INT   */
#define         irq_tm3_ena()            NVIC_EnableIRQ( TM3_IRQn )         /**< Enables TM3INT   */
#define         irq_tm4_ena()            NVIC_EnableIRQ( TM4_IRQn )         /**< Enables TM4INT   */
#define         irq_tm5_ena()            NVIC_EnableIRQ( TM5_IRQn )         /**< Enables TM5INT   */
#define         irq_ftm0_ena()           NVIC_EnableIRQ( FTM0_IRQn )        /**< Enables FTM0INT  */
#define         irq_ftm1_ena()           NVIC_EnableIRQ( FTM1_IRQn )        /**< Enables FTM1INT  */
#define         irq_tm1k_ena()           NVIC_EnableIRQ( TM1K_IRQn )        /**< Enables TM1KINT  */
#define         irq_tbc0_ena()           NVIC_EnableIRQ( LTBC_IRQn )        /**< Enables LTBC0INT */
#define         irq_rtc0_ena()           NVIC_EnableIRQ( RTC_IRQn )         /**< Enables RTC0INT  */
#define         irq_dmac0_ena()          NVIC_EnableIRQ( DMAC0_IRQn )       /**< Enables DMAC0INT */
#define         irq_dmac1_ena()          NVIC_EnableIRQ( DMAC1_IRQn )       /**< Enables DMAC1INT */
#define         irq_mcs_ena()            NVIC_EnableIRQ( MCU_IRQn )         /**< Enables MCUINT   */
#define         irq_ntms_ena()           NVIC_EnableIRQ( NTMS_IRQn )        /**< Enables NTMSINT  */
#define         irq_cmp_ena()            NVIC_EnableIRQ( CMP_IRQn )         /**< Enables CMPINT   */
#define         irq_can0_int0_ena()      NVIC_EnableIRQ( CAN0_INT0_IRQn )   /**< Enables CAN0INT0 */
#define         irq_can0_int1_ena()      NVIC_EnableIRQ( CAN0_INT1_IRQn )   /**< Enables CAN0INT1 */
#define         irq_ai_ena()             NVIC_EnableIRQ( AI_INT_IRQn )      /**< Enables AIINT    */


/*--- Disables interrupts ---*/
#define         irq_exi_dis()            NVIC_DisableIRQ( EXI_IRQn )        /**< Disables EXIINT */
#define         irq_ext0_dis()           clear_bit( NINT->EXICON, (0x3 << EXI0_EDGE_BIT_NO) )        /**< Disables EXI0INT */
#define         irq_ext1_dis()           clear_bit( NINT->EXICON, (0x3 << EXI1_EDGE_BIT_NO) )        /**< Disables EXI1INT */
#define         irq_ext2_dis()           clear_bit( NINT->EXICON, (0x3 << EXI2_EDGE_BIT_NO) )        /**< Disables EXI2INT */
#define         irq_ext3_dis()           clear_bit( NINT->EXICON, (0x3 << EXI3_EDGE_BIT_NO) )        /**< Disables EXI3INT */
#define         irq_ext4_dis()           clear_bit( NINT->EXICON, (0x3 << EXI4_EDGE_BIT_NO) )        /**< Disables EXI4INT */
#define         irq_ext5_dis()           clear_bit( NINT->EXICON, (0x3 << EXI5_EDGE_BIT_NO) )        /**< Disables EXI5INT */
#define         irq_ext6_dis()           clear_bit( NINT->EXICON, (0x3 << EXI6_EDGE_BIT_NO) )        /**< Disables EXI6INT */
#define         irq_ext7_dis()           clear_bit( NINT->EXICON, (0x3 << EXI7_EDGE_BIT_NO) )        /**< Disables EXI7INT */
#define         irq_siof0_dis()          NVIC_DisableIRQ( SIOF0_IRQn )       /**< Disables SIOF0INT */
#define         irq_siof1_dis()          NVIC_DisableIRQ( SIOF1_IRQn )       /**< Disables SIOF1INT */
#define         irq_uaf0_dis()           NVIC_DisableIRQ( UAF0_IRQn )        /**< Disables UAF0INT  */
#define         irq_uaf1_dis()           NVIC_DisableIRQ( UAF1_IRQn )        /**< Disables UAF1INT  */
#define         irq_uaf2_dis()           NVIC_DisableIRQ( UAF2_IRQn )        /**< Disables UAF2INT  */
#define         irq_uaf3_dis()           NVIC_DisableIRQ( UAF3_IRQn )        /**< Disables UAF3INT  */
#define         irq_i2cf0_dis()          NVIC_DisableIRQ( I2CF0_IRQn )       /**< Disables I2CF0INT */
#define         irq_clk_dis()            NVIC_DisableIRQ( CLK_IRQn )         /**< Disables CLKINT   */
#define         irq_vls_dis()            NVIC_DisableIRQ( VLS_IRQn )         /**< Disables VLSINT   */
#define         irq_sad0_dis()           NVIC_DisableIRQ( SAD_IRQn )         /**< Disables SADINT   */
#define         irq_sad1_dis()           NVIC_DisableIRQ( SAD1_IRQn )        /**< Disables SAD1INT  */
#define         irq_tm0_dis()            NVIC_DisableIRQ( TM0_IRQn )         /**< Disables TM0INT   */
#define         irq_tm1_dis()            NVIC_DisableIRQ( TM1_IRQn )         /**< Disables TM1INT   */
#define         irq_tm2_dis()            NVIC_DisableIRQ( TM2_IRQn )         /**< Disables TM2INT   */
#define         irq_tm3_dis()            NVIC_DisableIRQ( TM3_IRQn )         /**< Disables TM3INT   */
#define         irq_tm4_dis()            NVIC_DisableIRQ( TM4_IRQn )         /**< Disables TM4INT   */
#define         irq_tm5_dis()            NVIC_DisableIRQ( TM5_IRQn )         /**< Disables TM5INT   */
#define         irq_ftm0_dis()           NVIC_DisableIRQ( FTM0_IRQn )        /**< Disables FTM0INT  */
#define         irq_ftm1_dis()           NVIC_DisableIRQ( FTM1_IRQn )        /**< Disables FTM1INT  */
#define         irq_tm1k_dis()           NVIC_DisableIRQ( TM1K_IRQn )        /**< Disables TM1KINT  */
#define         irq_tbc0_dis()           NVIC_DisableIRQ( LTBC_IRQn )        /**< Disables LTBC0INT */
#define         irq_rtc0_dis()           NVIC_DisableIRQ( RTC_IRQn )         /**< Disables RTC0INT  */
#define         irq_dmac0_dis()          NVIC_DisableIRQ( DMAC0_IRQn )       /**< Disables DMAC0INT */
#define         irq_dmac1_dis()          NVIC_DisableIRQ( DMAC1_IRQn )       /**< Disables DMAC1INT */
#define         irq_mcs_dis()            NVIC_DisableIRQ( MCU_IRQn )         /**< Disables MCUINT   */
#define         irq_ntms_dis()           NVIC_DisableIRQ( NTMS_IRQn )        /**< Disables NTMSINT  */
#define         irq_cmp_dis()            NVIC_DisableIRQ( CMP_IRQn )         /**< Disables CMPINT   */
#define         irq_can0_int0_dis()      NVIC_DisableIRQ( CAN0_INT0_IRQn )   /**< Disables CAN0INT0 */
#define         irq_can0_int1_dis()      NVIC_DisableIRQ( CAN0_INT1_IRQn )   /**< Disables CAN0INT1 */
#define         irq_ai_dis()             NVIC_DisableIRQ( AI_INT_IRQn )      /**< Disables AIINT    */

/*--- Check IRQ ---*/
#define         irq_exi_checkIRQ()       NVIC_GetPendingIRQ( EXI_IRQn )           /**< Check EXIINT     */
#define         irq_ext0_checkIRQ()      get_bit( NINT->EXIST, (1 << 0) )          /**< Check EXI0INT   */
#define         irq_ext1_checkIRQ()      get_bit( NINT->EXIST, (1 << 1) )          /**< Check EXI1INT   */
#define         irq_ext2_checkIRQ()      get_bit( NINT->EXIST, (1 << 2) )          /**< Check EXI2INT   */
#define         irq_ext3_checkIRQ()      get_bit( NINT->EXIST, (1 << 3) )          /**< Check EXI3INT   */
#define         irq_ext4_checkIRQ()      get_bit( NINT->EXIST, (1 << 4) )          /**< Check EXI4INT   */
#define         irq_ext5_checkIRQ()      get_bit( NINT->EXIST, (1 << 5) )          /**< Check EXI5INT   */
#define         irq_ext6_checkIRQ()      get_bit( NINT->EXIST, (1 << 6) )          /**< Check EXI6INT   */
#define         irq_ext7_checkIRQ()      get_bit( NINT->EXIST, (1 << 7) )          /**< Check EXI7INT   */
#define         irq_siof0_checkIRQ()     NVIC_GetPendingIRQ( SIOF0_IRQn )         /**< Check SIOF0INT   */
#define         irq_siof1_checkIRQ()     NVIC_GetPendingIRQ( SIOF1_IRQn )         /**< Check SIOF1INT   */
#define         irq_uaf0_checkIRQ()      NVIC_GetPendingIRQ( UAF0_IRQn )          /**< Check UAF0INT    */
#define         irq_uaf1_checkIRQ()      NVIC_GetPendingIRQ( UAF1_IRQn )          /**< Check UAF1INT    */
#define         irq_uaf2_checkIRQ()      NVIC_GetPendingIRQ( UAF2_IRQn )          /**< Check UAF2INT    */
#define         irq_uaf3_checkIRQ()      NVIC_GetPendingIRQ( UAF3_IRQn )          /**< Check UAF3INT    */
#define         irq_i2cf0_checkIRQ()     NVIC_GetPendingIRQ( I2CF0_IRQn )         /**< Check I2CF0INT   */
#define         irq_clk_checkIRQ()       NVIC_GetPendingIRQ( CLK_IRQn )           /**< Check CLKINT     */
#define         irq_vls_checkIRQ()       NVIC_GetPendingIRQ( VLS_IRQn )           /**< Check VLSINT     */
#define         irq_sad0_checkIRQ()      NVIC_GetPendingIRQ( SAD_IRQn )           /**< Check SADINT     */
#define         irq_sad1_checkIRQ()      NVIC_GetPendingIRQ( SAD1_IRQn )          /**< Check SAD1INT    */
#define         irq_tm0_checkIRQ()       NVIC_GetPendingIRQ( TM0_IRQn )           /**< Check TM0INT     */
#define         irq_tm1_checkIRQ()       NVIC_GetPendingIRQ( TM1_IRQn )           /**< Check TM1INT     */
#define         irq_tm2_checkIRQ()       NVIC_GetPendingIRQ( TM2_IRQn )           /**< Check TM2INT     */
#define         irq_tm3_checkIRQ()       NVIC_GetPendingIRQ( TM3_IRQn )           /**< Check TM3INT     */
#define         irq_tm4_checkIRQ()       NVIC_GetPendingIRQ( TM4_IRQn )           /**< Check TM4INT     */
#define         irq_tm5_checkIRQ()       NVIC_GetPendingIRQ( TM5_IRQn )           /**< Check TM5INT     */
#define         irq_ftm0_checkIRQ()      NVIC_GetPendingIRQ( FTM0_IRQn )          /**< Check FTM0INT    */
#define         irq_ftm1_checkIRQ()      NVIC_GetPendingIRQ( FTM1_IRQn )          /**< Check FTM1INT    */
#define         irq_rtc0_checkIRQ()      NVIC_GetPendingIRQ( RTC_IRQn )           /**< Check RTC0INT    */
#define         irq_tm1k_checkIRQ()      NVIC_GetPendingIRQ( TM1K_IRQn )          /**< Check TM1KINT    */
#define         irq_tbc0_checkIRQ()      NVIC_GetPendingIRQ( LTBC_IRQn )          /**< Check LTBC0INT   */
#define         irq_dmac0_checkIRQ()     NVIC_GetPendingIRQ( DMAC0_IRQn )         /**< Check DMAC0INT   */
#define         irq_dmac1_checkIRQ()     NVIC_GetPendingIRQ( DMAC1_IRQn )         /**< Check DMAC1INT   */
#define         irq_mcs_checkIRQ()       NVIC_GetPendingIRQ( MCU_IRQn )           /**< Check MCUINT     */
#define         irq_ntms_checkIRQ()      NVIC_GetPendingIRQ( NTMS_IRQn )          /**< Check NTMSINT    */
#define         irq_cmp_checkIRQ()       NVIC_GetPendingIRQ( CMP_IRQn )           /**< Check CMPINT     */
#define         irq_can0_int0_checkIRQ() NVIC_GetPendingIRQ( CAN0_INT0_IRQn )     /**< Check CAN0INT0   */
#define         irq_can0_int1_checkIRQ() NVIC_GetPendingIRQ( CAN0_INT1_IRQn )     /**< Check CAN0INT1   */
#define         irq_ai_checkIRQ()        NVIC_GetPendingIRQ( AI_INT_IRQn )        /**< Check AIINT      */

/*--- Clear IRQ ---*/
#define         irq_exi_clearIRQ()       NVIC_ClearPendingIRQ(EXI_IRQn)         /**< Clear EXIINT     */
#define         irq_ext0_clearIRQ()      write_reg32( NINT->EXIST, (1 << 0) )   /**< Clear EXI0INT    */
#define         irq_ext1_clearIRQ()      write_reg32( NINT->EXIST, (1 << 1) )   /**< Clear EXI1INT    */
#define         irq_ext2_clearIRQ()      write_reg32( NINT->EXIST, (1 << 2) )   /**< Clear EXI2INT    */
#define         irq_ext3_clearIRQ()      write_reg32( NINT->EXIST, (1 << 3) )   /**< Clear EXI3INT    */
#define         irq_ext4_clearIRQ()      write_reg32( NINT->EXIST, (1 << 4) )   /**< Clear EXI4INT    */
#define         irq_ext5_clearIRQ()      write_reg32( NINT->EXIST, (1 << 5) )   /**< Clear EXI5INT    */
#define         irq_ext6_clearIRQ()      write_reg32( NINT->EXIST, (1 << 6) )   /**< Clear EXI6INT    */
#define         irq_ext7_clearIRQ()      write_reg32( NINT->EXIST, (1 << 7) )   /**< Clear EXI7INT    */
#define         irq_siof0_clearIRQ()     NVIC_ClearPendingIRQ(SIOF0_IRQn)       /**< Clear SIOF0INT   */
#define         irq_siof1_clearIRQ()     NVIC_ClearPendingIRQ(SIOF1_IRQn)       /**< Clear SIOF1INT   */
#define         irq_uaf0_clearIRQ()      NVIC_ClearPendingIRQ(UAF0_IRQn)        /**< Clear UAF0INT    */
#define         irq_uaf1_clearIRQ()      NVIC_ClearPendingIRQ(UAF1_IRQn)        /**< Clear UAF1INT    */
#define         irq_uaf2_clearIRQ()      NVIC_ClearPendingIRQ(UAF2_IRQn)        /**< Clear UAF2INT    */
#define         irq_uaf3_clearIRQ()      NVIC_ClearPendingIRQ(UAF3_IRQn)        /**< Clear UAF3INT    */
#define         irq_i2cf0_clearIRQ()     NVIC_ClearPendingIRQ(I2CF0_IRQn)       /**< Clear I2CF0INT   */
#define         irq_clk_clearIRQ()       NVIC_ClearPendingIRQ(CLK_IRQn)         /**< Clear CLKINT     */
#define         irq_vls_clearIRQ()       NVIC_ClearPendingIRQ(VLS_IRQn)         /**< Clear VLSINT     */
#define         irq_sad0_clearIRQ()      NVIC_ClearPendingIRQ(SAD_IRQn)         /**< Clear SADINT     */
#define         irq_sad1_clearIRQ()      NVIC_ClearPendingIRQ(SAD1_IRQn)        /**< Clear SAD1INT    */
#define         irq_tm0_clearIRQ()       NVIC_ClearPendingIRQ(TM0_IRQn)         /**< Clear TM0INT     */
#define         irq_tm1_clearIRQ()       NVIC_ClearPendingIRQ(TM1_IRQn)         /**< Clear TM1INT     */
#define         irq_tm2_clearIRQ()       NVIC_ClearPendingIRQ(TM2_IRQn)         /**< Clear TM2INT     */
#define         irq_tm3_clearIRQ()       NVIC_ClearPendingIRQ(TM3_IRQn)         /**< Clear TM3INT     */
#define         irq_tm4_clearIRQ()       NVIC_ClearPendingIRQ(TM4_IRQn)         /**< Clear TM4INT     */
#define         irq_tm5_clearIRQ()       NVIC_ClearPendingIRQ(TM5_IRQn)         /**< Clear TM5INT     */
#define         irq_ftm0_clearIRQ()      NVIC_ClearPendingIRQ(FTM0_IRQn)        /**< Clear FTM0INT    */
#define         irq_ftm1_clearIRQ()      NVIC_ClearPendingIRQ(FTM1_IRQn)        /**< Clear FTM1INT    */
#define         irq_tm1k_clearIRQ()      NVIC_ClearPendingIRQ(TM1K_IRQn)        /**< Clear TM1KINT    */
#define         irq_tbc0_clearIRQ()      NVIC_ClearPendingIRQ(LTBC_IRQn)        /**< Clear LTBC0INT   */
#define         irq_rtc0_clearIRQ()      NVIC_ClearPendingIRQ(RTC_IRQn)         /**< Clear RTC0INT    */
#define         irq_dmac0_clearIRQ()     NVIC_ClearPendingIRQ(DMAC0_IRQn)       /**< Clear DMAC0INT   */
#define         irq_dmac1_clearIRQ()     NVIC_ClearPendingIRQ(DMAC1_IRQn)       /**< Clear DMAC0INT   */
#define         irq_mcs_clearIRQ()       NVIC_ClearPendingIRQ(MCU_IRQn)         /**< Clear MCUINT     */
#define         irq_ntms_clearIRQ()      NVIC_ClearPendingIRQ( NTMS_IRQn )      /**< Clear NTMSINT    */
#define         irq_cmp_clearIRQ()       NVIC_ClearPendingIRQ( CMP_IRQn )       /**< Clear CMPINT     */
#define         irq_can0_int0_clearIRQ() NVIC_ClearPendingIRQ( CAN0_INT0_IRQn ) /**< Clear CAN0INT0   */
#define         irq_can0_int1_clearIRQ() NVIC_ClearPendingIRQ( CAN0_INT1_IRQn ) /**< Clear CAN0INT1   */
#define         irq_ai_clearIRQ()        NVIC_ClearPendingIRQ( AI_INT_IRQn )    /**< Clear AIINT      */

/*--- Setting interrupts priority level ---*/
/**
 * Setting interrupt priority level (EXIINT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after EXIINT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_exi_setLevel( level )                       NVIC_SetPriority( EXI_IRQn, level )

/**
 * Setting interrupt priority level (SIOF0INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after SIOF0INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_siof0_setLevel( level )                     NVIC_SetPriority( SIOF0_IRQn, level )

/**
 * Setting interrupt priority level (SIOF1INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after SIOF1INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_siof1_setLevel( level )                     NVIC_SetPriority( SIOF1_IRQn, level )

/**
 * Setting interrupt priority level (UAF0INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after UAF0INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_uaf0_setLevel( level )                      NVIC_SetPriority( UAF0_IRQn, level )

/**
 * Setting interrupt priority level (UAF1INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after UAF1INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_uaf1_setLevel( level )                      NVIC_SetPriority( UAF1_IRQn, level )

/**
 * Setting interrupt priority level (UAF2INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after UAF2INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_uaf2_setLevel( level )                      NVIC_SetPriority( UAF2_IRQn, level )

/**
 * Setting interrupt priority level (UAF3INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after UAF3INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_uaf3_setLevel( level )                      NVIC_SetPriority( UAF3_IRQn, level )

/**
 * Setting interrupt priority level (I2CF0INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after I2CF0INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_i2cf0_setLevel( level )                      NVIC_SetPriority( I2CF0_IRQn, level )

/**
 * Setting interrupt priority level (CLKINT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after CLKINT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_clk_setLevel( level )                      NVIC_SetPriority( CLK_IRQn, level )

/**
 * Setting interrupt priority level (VLSINT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after VLSINT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_vls_setLevel( level )                       NVIC_SetPriority( VLS_IRQn, level )

/**
 * Setting interrupt priority level (SADINT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after SADINT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_sad0_setLevel( level )                       NVIC_SetPriority( SAD_IRQn, level )

/**
 * Setting interrupt priority level (SAD1INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after SAD1INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_sad1_setLevel( level )                       NVIC_SetPriority( SAD1_IRQn, level )

/**
 * Setting interrupt priority level (TM0INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after TM0INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_tm0_setLevel( level )                       NVIC_SetPriority( TM0_IRQn, level )

/**
 * Setting interrupt priority level (TM1INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after TM1INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_tm1_setLevel( level )                       NVIC_SetPriority( TM1_IRQn, level )

/**
 * Setting interrupt priority level (TM2INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after TM2INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_tm2_setLevel( level )                       NVIC_SetPriority( TM2_IRQn, level )

/**
 * Setting interrupt priority level (TM3INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after TM3INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_tm3_setLevel( level )                       NVIC_SetPriority( TM3_IRQn, level )

/**
 * Setting interrupt priority level (TM4INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after TM4INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_tm4_setLevel( level )                       NVIC_SetPriority( TM4_IRQn, level )

/**
 * Setting interrupt priority level (TM5INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after TM5INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_tm5_setLevel( level )                       NVIC_SetPriority( TM5_IRQn, level )

/**
 * Setting interrupt priority level (FTM0INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after FTM0INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_ftm0_setLevel( level )                      NVIC_SetPriority( FTM0_IRQn, level )

/**
 * Setting interrupt priority level (FTM1INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after FTM1INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_ftm1_setLevel( level )                      NVIC_SetPriority( FTM1_IRQn, level )

/**
 * Setting interrupt priority level (RTCINT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after RTCINT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_rtc0_setLevel( level )                      NVIC_SetPriority( RTC_IRQn, level )

/**
 * Setting interrupt priority level (TM1KINT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after TM1KINT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_tm1k_setLevel( level )                      NVIC_SetPriority( TM1K_IRQn, level )


/**
 * Setting interrupt priority level (LTBC0INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after LTBC0INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_tbc0_setLevel( level )                      NVIC_SetPriority( LTBC_IRQn, level )

/**
 * Setting interrupt priority level (DMAC0INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after DMAC0INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_dmac0_setLevel( level )                      NVIC_SetPriority( DMAC0_IRQn, level )

/**
 * Setting interrupt priority level (DMAC1INT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after DMAC1INT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_dmac1_setLevel( level )                      NVIC_SetPriority( DMAC1_IRQn, level )

/**
 * Setting interrupt priority level (MCUINT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after MCUINT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_mcs_setLevel( level )                       NVIC_SetPriority( MCU_IRQn, level )

/**
 * Setting interrupt priority level (NTMSINT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after NTMSINT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_ntms_setLevel( level )                       NVIC_SetPriority( NTMS_IRQn, level )

/**
 * Setting interrupt priority level (CMPINT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after CMPINT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_cmp_setLevel( level )                       NVIC_SetPriority( CMP_IRQn, level )

/**
 * Setting interrupt priority level (CAN0INT0)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after CAN0INT0 enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_can0_int0_setLevel( level )                       NVIC_SetPriority( CAN0_INT0_IRQn, level )

/**
 * Setting interrupt priority level (CAN0INT1)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after CAN0INT1 enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_can0_int1_setLevel( level )                       NVIC_SetPriority( CAN0_INT1_IRQn, level )

/**
 * Setting interrupt priority level (AIINT)
 *
 * @param[in]   level   Setting value of interrupt priority level <br>
 *                      (from 0 to 3)
 * @return      None
 *
 * @note    Do not call this macro, after AIINT enabled.
 * @note    Do not call this macro, in the state that disabled the interrupt priority level control.
 */
#define         irq_ai_setLevel( level )                       NVIC_SetPriority( AI_INT_IRQn, level )

/*--- Setting external interrupt parameters ---*/
void            irq_ext0_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port );
void            irq_ext1_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port );
void            irq_ext2_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port );
void            irq_ext3_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port );
void            irq_ext4_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port );
void            irq_ext5_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port );
void            irq_ext6_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port );
void            irq_ext7_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port );

#endif /*IRQ_H__*/

/** @} */
/** @} */
