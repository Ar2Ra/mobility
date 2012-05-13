/*
    Energy management

    Energy.c
*/

#include <LPC214x.H>
#include "Energy.h"

#define SEL       0x02        //Select channel AD0.1
#define CLKDIV    3           //15MHz / (3 + 1) = 3.75 Mhz < 4.5 MHz = max allowed

#define NR_CHANNELS  3
#define NR_SAMPLES   10

typedef struct _adc_struct
{
    unsigned int buffer[NR_SAMPLES];
    unsigned int counter;
} adc_struct;

adc_struct adc_data[NR_CHANNELS];

void sample_add(unsigned int ch, unsigned int sample)
{
    unsigned int pos;
    pos = adc_data[ch].counter;

    adc_data[ch].buffer[pos] = sample;
    pos = (pos + 1) % NR_SAMPLES;

    adc_data[ch].counter = pos;
}

unsigned int sample_avg(unsigned int ch)
{
    unsigned int i, sum = 0;

    for (i = 0; i < NR_SAMPLES; i++)
        sum += adc_data[ch].buffer[i];

    return (sum / NR_SAMPLES);
}

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
