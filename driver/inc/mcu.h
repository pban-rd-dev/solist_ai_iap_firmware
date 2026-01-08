/*****************************************************************************
 mcu.h

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
 * @file    mcu.h
 *
 * This module is 'Common' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup Common
 *  @{
 *
 *  @brief The common definition of ML63Q25XX group
 *
 *  @file
 */

#ifndef MCU_H__
#define MCU_H__

/*
 * ML63Q25XX is defined because there are functions that does not exist depending on the group.
 *  Model pin, program and ram memory size, and ram start address are defined.
 */

#if defined(ML63Q2557)
	#include <ML63Q25x7.h>
	#define PRG_START_ADDRESS    0x10000000UL
	#define PRG_START_ADDRESS2   0x10002000UL
	#define DATA_START_ADDRESS   0x18000000UL
	#define RAM_START_ADDRESS    0x20000000UL
#elif defined(ML63Q2537)
	#include <ML63Q25x7.h>
	#define PRG_START_ADDRESS    0x10000000UL
	#define PRG_START_ADDRESS2   0x10002000UL
	#define DATA_START_ADDRESS   0x18000000UL
	#define RAM_START_ADDRESS    0x20000000UL
#elif defined(ML63Q2554)
	#include <ML63Q25x4.h>
	#define PRG_START_ADDRESS    0x10000000UL
	#define DATA_START_ADDRESS   0x18000000UL
	#define RAM_START_ADDRESS    0x20000000UL
#elif defined(ML63Q2534)
	#include <ML63Q25x4.h>
	#define PRG_START_ADDRESS    0x10000000UL
	#define DATA_START_ADDRESS   0x18000000UL
	#define RAM_START_ADDRESS    0x20000000UL
#else
#error "Unknown target MCU"
#endif /* ML63Q25XX */

#endif /* MCU_H__ */

/** @} */
/** @} */
