/*****************************************************************************
 irq.c

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
 * @file    irq.c
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

#include "mcu.h"
#include "rdwr_reg.h"
#include "irq.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/*############################################################################*/
/*#                                Variable                                  #*/
/*############################################################################*/

/*############################################################################*/
/*#                               Prototype                                  #*/
/*############################################################################*/
static void s_irq_common_setExiconBit( uint32_t *dest, uint8_t bit_no, uint8_t val, uint8_t mask );

/*############################################################################*/
/*#                              Subroutine                                  #*/
/*############################################################################*/
/**
 * Set EXICON register value
 *
 * @param[out]  dest        the pointer to area where the EXICON register value is stored
 *
 * @param[in]   bit_no      the bit number of EXICON register
 *
 * @param[in]   val         setting value
 *
 * @param[in]   mask        bitmask of 'val'
 * @return      None
 */
__INLINE static void s_irq_common_setExiconBit( uint32_t *dest, uint8_t bit_no, uint8_t val, uint8_t mask )
{
	uint32_t mask32;
	uint32_t sh;

	sh = bit_no;

	mask32 = ((uint32_t)mask << sh);
	*dest &= ~mask32;
	*dest |= ((uint32_t)val << sh);    /* set bit */
}


/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Setting external interrupt parameters (EXI0INT)
 *
 * @param[in]   edge        EXIn_EDGE_DISABLE : interrupt disable               <br>
 *                          EXIn_EDGE_FALLING : falling edge                    <br>
 *                          EXIn_EDGE_RISING  : rising edge                     <br>
 *                          EXIn_EDGE_BOTH    : both edge
 *
 * @param[in]   sampling    EXIn_SAMPLING_DIS : whithout sampling               <br>
 *                          EXIn_SAMPLING_ENA : with sampling
 *
 * @param[in]   filter      EXIn_FILTER_DIS   : without filtering               <br>
 *                          EXIn_FILTER_ENA   : with filtering
 *
 * @param[in]   port        Select interrupt source
 * @arg                         EXIn_PORT_SEL_P20 to EXIn_PORT_SEL_P23
 * @arg                         EXIn_PORT_SEL_P30 to EXIn_PORT_SEL_P37
 * @arg                         EXIn_PORT_SEL_P40 to EXIn_PORT_SEL_P47
 * @arg                         EXIn_PORT_SEL_P50 to EXIn_PORT_SEL_P57
 * @arg                         EXIn_PORT_SEL_P60 to EXIn_PORT_SEL_P66
 * @arg                         EXIn_PORT_SEL_P70 to EXIn_PORT_SEL_P77
 * @arg                         EXIn_PORT_SEL_P80 to EXIn_PORT_SEL_P85
 * @return      None
 */
void irq_ext0_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port )
{
	uint32_t reg32;

	/* Select port */
	reg32 = read_reg32(NINT->EXI03SEL) & 0xffffff00;
	write_reg32( NINT->EXI03SEL, reg32 | port );

	reg32 = read_reg32(NINT->EXICON);
	/* Set edge */
	s_irq_common_setExiconBit( &reg32, EXI0_EDGE_BIT_NO, edge, 0x3 );
	/* Set enable/disable sampling */
	s_irq_common_setExiconBit( &reg32, 2, sampling, 0x1 );
	/* Set enable/disable noise filter */
	s_irq_common_setExiconBit( &reg32, 3, filter, 0x1 );
	write_reg32( NINT->EXICON, reg32 );
}

/**
 * Setting external interrupt parameters (EXI1INT)
 *
 * @param[in]   edge        EXIn_EDGE_DISABLE : interrupt disable               <br>
 *                          EXIn_EDGE_FALLING : falling edge                    <br>
 *                          EXIn_EDGE_RISING  : rising edge                     <br>
 *                          EXIn_EDGE_BOTH    : both edge
 *
 * @param[in]   sampling    EXIn_SAMPLING_DIS : whithout sampling               <br>
 *                          EXIn_SAMPLING_ENA : with sampling
 *
 * @param[in]   filter      EXIn_FILTER_DIS   : without filtering               <br>
 *                          EXIn_FILTER_ENA   : with filtering
 *
 * @param[in]   port        Select interrupt source
 * @arg                         EXIn_PORT_SEL_P20 to EXIn_PORT_SEL_P23
 * @arg                         EXIn_PORT_SEL_P30 to EXIn_PORT_SEL_P37
 * @arg                         EXIn_PORT_SEL_P40 to EXIn_PORT_SEL_P47
 * @arg                         EXIn_PORT_SEL_P50 to EXIn_PORT_SEL_P57
 * @arg                         EXIn_PORT_SEL_P60 to EXIn_PORT_SEL_P66
 * @arg                         EXIn_PORT_SEL_P70 to EXIn_PORT_SEL_P77
 * @arg                         EXIn_PORT_SEL_P80 to EXIn_PORT_SEL_P85
 * @return      None
 */
void irq_ext1_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port )
{
	uint32_t reg32;
	
	/* Select port */
	reg32 = read_reg32(NINT->EXI03SEL) & 0xffff00ff;
	write_reg32( NINT->EXI03SEL, reg32 | ((uint32_t)port << 8) );

	reg32 = read_reg32(NINT->EXICON);
	/* Set edge */
	s_irq_common_setExiconBit( &reg32, EXI1_EDGE_BIT_NO, edge, 0x3 );
	/* Set enable/disable sampling */
	s_irq_common_setExiconBit( &reg32, 6, sampling, 0x1 );
	/* Set enable/disable noise filter */
	s_irq_common_setExiconBit( &reg32, 7, filter, 0x1 );
	write_reg32( NINT->EXICON, reg32 );
}
/**
 * Setting external interrupt parameters (EXI2INT)
 *
 * @param[in]   edge        EXIn_EDGE_DISABLE : interrupt disable               <br>
 *                          EXIn_EDGE_FALLING : falling edge                    <br>
 *                          EXIn_EDGE_RISING  : rising edge                     <br>
 *                          EXIn_EDGE_BOTH    : both edge
 *
 * @param[in]   sampling    EXIn_SAMPLING_DIS : whithout sampling               <br>
 *                          EXIn_SAMPLING_ENA : with sampling
 *
 * @param[in]   filter      EXIn_FILTER_DIS   : without filtering               <br>
 *                          EXIn_FILTER_ENA   : with filtering
 *
 * @param[in]   port        Select interrupt source
 * @arg                         EXIn_PORT_SEL_P20 to EXIn_PORT_SEL_P23
 * @arg                         EXIn_PORT_SEL_P30 to EXIn_PORT_SEL_P37
 * @arg                         EXIn_PORT_SEL_P40 to EXIn_PORT_SEL_P47
 * @arg                         EXIn_PORT_SEL_P50 to EXIn_PORT_SEL_P57
 * @arg                         EXIn_PORT_SEL_P60 to EXIn_PORT_SEL_P66
 * @arg                         EXIn_PORT_SEL_P70 to EXIn_PORT_SEL_P77
 * @arg                         EXIn_PORT_SEL_P80 to EXIn_PORT_SEL_P85
 * @return      None
 */
void irq_ext2_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port )
{
	uint32_t reg32;
	
	/* Select port */
	reg32 = read_reg32(NINT->EXI03SEL) & 0xff00ffff;
	write_reg32( NINT->EXI03SEL, reg32 | ((uint32_t)port << 16) );

	reg32 = read_reg32(NINT->EXICON);
	/* Set edge */
	s_irq_common_setExiconBit( &reg32, EXI2_EDGE_BIT_NO, edge, 0x3 );
	/* Set enable/disable sampling */
	s_irq_common_setExiconBit( &reg32, 10, sampling, 0x1 );
	/* Set enable/disable noise filter */
	s_irq_common_setExiconBit( &reg32, 11, filter, 0x1 );
	write_reg32( NINT->EXICON, reg32 );
}

/**
 * Setting external interrupt parameters (EXI3INT)
 *
 * @param[in]   edge        EXIn_EDGE_DISABLE : interrupt disable               <br>
 *                          EXIn_EDGE_FALLING : falling edge                    <br>
 *                          EXIn_EDGE_RISING  : rising edge                     <br>
 *                          EXIn_EDGE_BOTH    : both edge
 *
 * @param[in]   sampling    EXIn_SAMPLING_DIS : whithout sampling               <br>
 *                          EXIn_SAMPLING_ENA : with sampling
 *
 * @param[in]   filter      EXIn_FILTER_DIS   : without filtering               <br>
 *                          EXIn_FILTER_ENA   : with filtering
 *
 * @param[in]   port        Select interrupt source
 * @arg                         EXIn_PORT_SEL_P20 to EXIn_PORT_SEL_P23
 * @arg                         EXIn_PORT_SEL_P30 to EXIn_PORT_SEL_P37
 * @arg                         EXIn_PORT_SEL_P40 to EXIn_PORT_SEL_P47
 * @arg                         EXIn_PORT_SEL_P50 to EXIn_PORT_SEL_P57
 * @arg                         EXIn_PORT_SEL_P60 to EXIn_PORT_SEL_P66
 * @arg                         EXIn_PORT_SEL_P70 to EXIn_PORT_SEL_P77
 * @arg                         EXIn_PORT_SEL_P80 to EXIn_PORT_SEL_P85
 * @return      None
 */
void irq_ext3_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port )
{
	uint32_t reg32;
	
	/* Select port */
	reg32 = read_reg32(NINT->EXI03SEL) & 0x00ffffff;
	write_reg32( NINT->EXI03SEL, reg32 | ((uint32_t)port << 24) );

	reg32 = read_reg32(NINT->EXICON);
	/* Set edge */
	s_irq_common_setExiconBit( &reg32, EXI3_EDGE_BIT_NO, edge, 0x3 );
	/* Set enable/disable sampling */
	s_irq_common_setExiconBit( &reg32, 14, sampling, 0x1 );
	/* Set enable/disable noise filter */
	s_irq_common_setExiconBit( &reg32, 15, filter, 0x1 );
	write_reg32( NINT->EXICON, reg32 );
}

/**
 * Setting external interrupt parameters (EXI4INT)
 *
 * @param[in]   edge        EXIn_EDGE_DISABLE : interrupt disable               <br>
 *                          EXIn_EDGE_FALLING : falling edge                    <br>
 *                          EXIn_EDGE_RISING  : rising edge                     <br>
 *                          EXIn_EDGE_BOTH    : both edge
 *
 * @param[in]   sampling    EXIn_SAMPLING_DIS : whithout sampling               <br>
 *                          EXIn_SAMPLING_ENA : with sampling
 *
 * @param[in]   filter      EXIn_FILTER_DIS   : without filtering               <br>
 *                          EXIn_FILTER_ENA   : with filtering
 *
 * @param[in]   port        Select interrupt source
 * @arg                         EXIn_PORT_SEL_P20 to EXIn_PORT_SEL_P23
 * @arg                         EXIn_PORT_SEL_P30 to EXIn_PORT_SEL_P37
 * @arg                         EXIn_PORT_SEL_P40 to EXIn_PORT_SEL_P47
 * @arg                         EXIn_PORT_SEL_P50 to EXIn_PORT_SEL_P57
 * @arg                         EXIn_PORT_SEL_P60 to EXIn_PORT_SEL_P66
 * @arg                         EXIn_PORT_SEL_P70 to EXIn_PORT_SEL_P77
 * @arg                         EXIn_PORT_SEL_P80 to EXIn_PORT_SEL_P85
 * @return      None
 */
void irq_ext4_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port )
{
	uint32_t reg32;
	
	/* Select port */
	reg32 = read_reg32(NINT->EXI47SEL) & 0xffffff00;
	write_reg32( NINT->EXI47SEL, reg32 | port );

	reg32 = read_reg32(NINT->EXICON);
	/* Set edge */
	s_irq_common_setExiconBit( &reg32, EXI4_EDGE_BIT_NO, edge, 0x3 );
	/* Set enable/disable sampling */
	s_irq_common_setExiconBit( &reg32, 18, sampling, 0x1 );
	/* Set enable/disable noise filter */
	s_irq_common_setExiconBit( &reg32, 19, filter, 0x1 );
	write_reg32( NINT->EXICON, reg32 );
}

/**
 * Setting external interrupt parameters (EXI5INT)
 *
 * @param[in]   edge        EXIn_EDGE_DISABLE : interrupt disable               <br>
 *                          EXIn_EDGE_FALLING : falling edge                    <br>
 *                          EXIn_EDGE_RISING  : rising edge                     <br>
 *                          EXIn_EDGE_BOTH    : both edge
 *
 * @param[in]   sampling    EXIn_SAMPLING_DIS : whithout sampling               <br>
 *                          EXIn_SAMPLING_ENA : with sampling
 *
 * @param[in]   filter      EXIn_FILTER_DIS   : without filtering               <br>
 *                          EXIn_FILTER_ENA   : with filtering
 *
 * @param[in]   port        Select interrupt source
 * @arg                         EXIn_PORT_SEL_P20 to EXIn_PORT_SEL_P23
 * @arg                         EXIn_PORT_SEL_P30 to EXIn_PORT_SEL_P37
 * @arg                         EXIn_PORT_SEL_P40 to EXIn_PORT_SEL_P47
 * @arg                         EXIn_PORT_SEL_P50 to EXIn_PORT_SEL_P57
 * @arg                         EXIn_PORT_SEL_P60 to EXIn_PORT_SEL_P66
 * @arg                         EXIn_PORT_SEL_P70 to EXIn_PORT_SEL_P77
 * @arg                         EXIn_PORT_SEL_P80 to EXIn_PORT_SEL_P85
 * @return      None
 */
void irq_ext5_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port )
{
	uint32_t reg32;
	
	/* Select port */
	reg32 = read_reg32(NINT->EXI47SEL) & 0xffff00ff;
	write_reg32( NINT->EXI47SEL, reg32 | ((uint32_t)port << 8) );

	reg32 = read_reg32(NINT->EXICON);
	/* Set edge */
	s_irq_common_setExiconBit( &reg32, EXI5_EDGE_BIT_NO, edge, 0x3 );
	/* Set enable/disable sampling */
	s_irq_common_setExiconBit( &reg32, 22, sampling, 0x1 );
	/* Set enable/disable noise filter */
	s_irq_common_setExiconBit( &reg32, 23, filter, 0x1 );
	write_reg32( NINT->EXICON, reg32 );

}

/**
 * Setting external interrupt parameters (EXI6INT)
 *
 * @param[in]   edge        EXIn_EDGE_DISABLE : interrupt disable               <br>
 *                          EXIn_EDGE_FALLING : falling edge                    <br>
 *                          EXIn_EDGE_RISING  : rising edge                     <br>
 *                          EXIn_EDGE_BOTH    : both edge
 *
 * @param[in]   sampling    EXIn_SAMPLING_DIS : whithout sampling               <br>
 *                          EXIn_SAMPLING_ENA : with sampling
 *
 * @param[in]   filter      EXIn_FILTER_DIS   : without filtering               <br>
 *                          EXIn_FILTER_ENA   : with filtering
 *
 * @param[in]   port        Select interrupt source
 * @arg                         EXIn_PORT_SEL_P20 to EXIn_PORT_SEL_P23
 * @arg                         EXIn_PORT_SEL_P30 to EXIn_PORT_SEL_P37
 * @arg                         EXIn_PORT_SEL_P40 to EXIn_PORT_SEL_P47
 * @arg                         EXIn_PORT_SEL_P50 to EXIn_PORT_SEL_P57
 * @arg                         EXIn_PORT_SEL_P60 to EXIn_PORT_SEL_P67
 * @arg                         EXIn_PORT_SEL_P70 to EXIn_PORT_SEL_P77
 * @arg                         EXIn_PORT_SEL_P80 to EXIn_PORT_SEL_P85
 * @return      None
 */
void irq_ext6_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port )
{
	uint32_t reg32;
	
	/* Select port */
	reg32 = read_reg32(NINT->EXI47SEL) & 0xff00ffff;
	write_reg32( NINT->EXI47SEL, reg32 | ((uint32_t)port << 16) );

	reg32 = read_reg32(NINT->EXICON);
	/* Set edge */
	s_irq_common_setExiconBit( &reg32, EXI6_EDGE_BIT_NO, edge, 0x3 );
	/* Set enable/disable sampling */
	s_irq_common_setExiconBit( &reg32, 26, sampling, 0x1 );
	/* Set enable/disable noise filter */
	s_irq_common_setExiconBit( &reg32, 27, filter, 0x1 );
	write_reg32( NINT->EXICON, reg32 );
}

/**
 * Setting external interrupt parameters (EXI7INT)
 *
 * @param[in]   edge        EXIn_EDGE_DISABLE : interrupt disable               <br>
 *                          EXIn_EDGE_FALLING : falling edge                    <br>
 *                          EXIn_EDGE_RISING  : rising edge                     <br>
 *                          EXIn_EDGE_BOTH    : both edge
 *
 * @param[in]   sampling    EXIn_SAMPLING_DIS : whithout sampling               <br>
 *                          EXIn_SAMPLING_ENA : with sampling
 *
 * @param[in]   filter      EXIn_FILTER_DIS   : without filtering               <br>
 *                          EXIn_FILTER_ENA   : with filtering
 *
 * @param[in]   port        Select interrupt source
 * @arg                         EXIn_PORT_SEL_P20 to EXIn_PORT_SEL_P23
 * @arg                         EXIn_PORT_SEL_P30 to EXIn_PORT_SEL_P37
 * @arg                         EXIn_PORT_SEL_P40 to EXIn_PORT_SEL_P47
 * @arg                         EXIn_PORT_SEL_P50 to EXIn_PORT_SEL_P57
 * @arg                         EXIn_PORT_SEL_P60 to EXIn_PORT_SEL_P66
 * @arg                         EXIn_PORT_SEL_P70 to EXIn_PORT_SEL_P77
 * @arg                         EXIn_PORT_SEL_P80 to EXIn_PORT_SEL_P85
 * @return      None
 */
void irq_ext7_init( uint8_t edge, uint8_t sampling, uint8_t filter, uint8_t port )
{
	uint32_t reg32;
	
	/* Select port */
	reg32 = read_reg32(NINT->EXI47SEL) & 0x00ffffff;
	write_reg32( NINT->EXI47SEL, reg32 | ((uint32_t)port << 24) );

	reg32 = read_reg32(NINT->EXICON);
	/* Set edge */
	s_irq_common_setExiconBit( &reg32, EXI7_EDGE_BIT_NO, edge, 0x3 );
	/* Set enable/disable sampling */
	s_irq_common_setExiconBit( &reg32, 30, sampling, 0x1 );
	/* Set enable/disable noise filter */
	s_irq_common_setExiconBit( &reg32, 31, filter, 0x1 );
	write_reg32( NINT->EXICON, reg32 );
}

/** @} */
/** @} */
