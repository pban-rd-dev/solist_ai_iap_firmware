/*****************************************************************************
 lp_manage.h

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
 * @file    lp_manage.h
 *
 * This module is 'System' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup System
 *  @{
 *
 *  @brief System Control Function register related
 *
 *  @file
 */

#ifndef _LP_MANAGE_H_
#define _LP_MANAGE_H_

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
void            lp_setStopMode( void );
void            lp_setHaltMode( void );

#endif /*_LP_MANAGE_H_*/

/** @} */
/** @} */
