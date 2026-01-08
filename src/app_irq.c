#include <stdio.h>

#include "debug_uart.h"
#include "irq.h"
#include "ssiof0.h"

/*############################################################################*/
/*#                               Prototype                                  #*/
/*############################################################################*/
void EXI_IRQHandler( void );
void NMI_Handler( void );
void UAF0_IRQHandler( void );

void EXI_IRQHandler( void )
{
}

void NMI_Handler( void )
{
	/* No process */
}

void UAF0_IRQHandler( void )
{
	debug_procUartfInt();
}

void SIOF0_IRQHandler(void)
{
  // ssiof0_continue();
}

