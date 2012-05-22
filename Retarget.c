/******************************************************************************/
/* Retarget.c: 'Retarget' layer for target-dependent low level functions      */
/******************************************************************************/

#include <stdio.h>
#include <rt_misc.h>
#include "Type.h"
#include "Uart0.h"

#pragma import(__use_no_semihosting_swi)

struct __FILE 
{ 
    int handle;                           /* Add whatever you need here         */ 
};

int fputc(int ch, FILE *f) 
{
    return (uart0_sendchar(ch));          /* Retarget fputc to serial UART      */
}

void _sys_exit(int return_code) 
{ 
    for (;;);                             /* Endless loop                       */
}
