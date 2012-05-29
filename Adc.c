/*
    Energy management

    Adc.c
*/

#include <LPC214x.H>
#include <stdio.h>
#include "Type.h"

#include "Adc.h"

void ADC0isr(void)	__irq;

typedef struct _adc_struct adc_struct;

struct _adc_struct
{
    uint32 buffer[ADC_NR_SAMPLES];
    uint16 counter;
};

adc_struct adc_data[ADC_NR_CHANNELS];

void sample_add(uint8 ch, uint32 sample)
{
    uint16 pos;
    pos = adc_data[ch].counter;

    adc_data[ch].buffer[pos] = sample;
    pos = (pos + 1) % ADC_NR_SAMPLES;

    adc_data[ch].counter = pos;
}

uint32 adc_read_battery(void)
{
    return (sample_avg(0) * ADC_BAT_DIV);
}

uint32 sample_avg(uint8 ch)
{
    uint16 i;
    uint32 sum = 0;

    for (i = 0; i < ADC_NR_SAMPLES; i++)
        sum += adc_data[ch].buffer[i];

    return (sum / ADC_NR_SAMPLES);
}

uint32 sample_voltage(uint8 ch)
{
    uint32 data;
    data = sample_avg(ch);

    return (data * ADC_REF / 1023);
}

void adc_init(void)
{
    PINSEL1 |= (1 << 24);                      //P0.28 - AD0.1

    AD0CR |= (ADC_CLKDIV << 8);                //11 clocks / 10 bits @ 3.75MHz
    AD0INTEN |= (1 << 1);                      //Enable channel 1 interrupt

    VICVectAddr5 = (uint32) ADC0isr;           //Set the ADC0 ISR vector address
    VICVectCntl5 = 0x00000020 | 18;            //Set channel for ADC0
    VICIntEnable |= (1 << 18);                 //Enable the interrupt ADC0

    AD0CR |= ADC_SEL;                          //Select channel 1
    AD0CR |= (1 << 21);                        //AD0 is operational
}

void adc_start(void)
{
    AD0CR |= (1 << 24);                        //Start conversion now
}

void ADC0isr(void)	__irq
{
    uint32 result;
    
    if (AD0STAT & (1 << 1))                    //Channel 1 is Done
    {
        result = AD0DR1;
        result = (result >> 6) & 0x03FF;

        sample_add(0, result);
    }

    AD0CR &= ~(7 << 24);                       //Stop ADC now
    VICVectAddr  = 0x00000000;                 //Dummy write to signal end of interrupt
}
