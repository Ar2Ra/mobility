/*
    Energy management

    Energy.c
*/

#include <LPC214x.H>
#include "Energy.h"

#define SEL       0x02        //Select channel AD0.1
#define CLKDIV    3           //15MHz / (3 + 1) = 3.75 Mhz < 4.5 MHz = max allowed

void adc_init(void)
{
    PINSEL1 |= (1 << 24);     //P0.28 - AD0.1

    AD0CR |= SEL;
    AD0CR |= (CLKDIV << 8);
    AD0CR |= (1 << 16);       //Burst enable
    AD0CR |= (1 << 21);       //AD0 is operational
}

/*
unsigned int adc_get(void)
{
}
*/
