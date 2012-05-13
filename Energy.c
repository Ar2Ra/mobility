/*
    Energy management

    Energy.c
*/

#include <LPC214x.H>
#include "Type.h"

#include "Energy.h"

#define SEL       0x02        //Select channel AD0.1
#define CLKDIV    3           //15MHz / (3 + 1) = 3.75 Mhz < 4.5 MHz = max allowed

#define NR_CHANNELS  3
#define NR_SAMPLES   10

typedef struct _adc_struct
{
    uint32 buffer[NR_SAMPLES];
    uint16 counter;
} adc_struct;

adc_struct adc_data[NR_CHANNELS];

void sample_add(uint8 ch, uint32 sample)
{
    uint16 pos;
    pos = adc_data[ch].counter;

    adc_data[ch].buffer[pos] = sample;
    pos = (pos + 1) % NR_SAMPLES;

    adc_data[ch].counter = pos;
}

uint32 sample_avg(uint8 ch)
{
    uint16 i;
    uint32 sum = 0;

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
