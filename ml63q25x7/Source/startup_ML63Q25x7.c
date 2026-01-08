/******************************************************************************
 * @file     startup_ML63Q25x7.c
 * @brief    CMSIS-Core(M) Device Startup File for ML63Q25x7
 * @version  0.40
 * @date     27 Jun 2024
 ******************************************************************************/
/*
 * Copyright (C) 2024 ROHM Co., Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file is based on the following Device Example of CMSIS-Core(M) Version 5.8.0.
 *   - startup_ARMCM3.c
 *     Copyright (c) 2009-2020 Arm Limited. All rights reserved.
 */

#if defined (ML63Q25x7)
  #include "ML63Q25x7.h"
#else
  #error device not specified!
#endif

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;

extern __NO_RETURN void __PROGRAM_START(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler  (void);
            void Default_Handler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler      (void) __attribute__ ((weak));
void SVC_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));

void EXI_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void TM0_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void TM1_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void FTM0_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void DMAC0_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void NTMS_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void SIOF1_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void MCU_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void UAF1_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void TM2_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void TM3_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void FTM1_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void DMAC1_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void UAF3_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void SIOF0_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void I2CF0_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void UAF0_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void TM4_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void TM5_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void CMP_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void CLK_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void VLS_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void SAD_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void UAF2_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void AI_INT2_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void CAN0_INT0_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler")));
void CAN0_INT1_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler")));
void SAD1_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void LTBC_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void TM1K_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void AI_INT_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));


/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const VECTOR_TABLE_Type __VECTOR_TABLE[48];
       const VECTOR_TABLE_Type __VECTOR_TABLE[48] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),       /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  NMI_Handler,                              /* -14 NMI Handler */
  HardFault_Handler,                        /* -13 Hard Fault Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  SVC_Handler,                              /*  -5 SVCall Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  PendSV_Handler,                           /*  -2 PendSV Handler */
  SysTick_Handler,                          /*  -1 SysTick Handler */

  /* Interrupts */
  EXI_IRQHandler,
  TM0_IRQHandler,
  TM1_IRQHandler,
  FTM0_IRQHandler,
  DMAC0_IRQHandler,
  NTMS_IRQHandler,
  SIOF1_IRQHandler,
  MCU_IRQHandler,
  UAF1_IRQHandler,
  TM2_IRQHandler,
  TM3_IRQHandler,
  FTM1_IRQHandler,
  DMAC1_IRQHandler,
  UAF3_IRQHandler,
  SIOF0_IRQHandler,
  I2CF0_IRQHandler,
  UAF0_IRQHandler,
  TM4_IRQHandler,
  TM5_IRQHandler,
  CMP_IRQHandler,
  CLK_IRQHandler,
  VLS_IRQHandler,
  SAD_IRQHandler,
  UAF2_IRQHandler,
  AI_INT2_IRQHandler,
  CAN0_INT0_IRQHandler,
  CAN0_INT1_IRQHandler,
  SAD1_IRQHandler,
  RTC_IRQHandler,
  LTBC_IRQHandler,
  TM1K_IRQHandler,
  AI_INT_IRQHandler
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void)
{
  SystemInit();                             /* CMSIS System Initialization */
  __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

/*----------------------------------------------------------------------------
  Hard Fault Handler
 *----------------------------------------------------------------------------*/
void HardFault_Handler(void)
{
  while(1);
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void)
{
  while(1);
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#endif


