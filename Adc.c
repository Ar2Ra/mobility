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
    return (sample_voltage(0) * ADC_BAT_DIV);
}

uint8 adc_low_battery(void)
{
    uint32 v_bat;

    v_bat = adc_read_battery();
    if (v_bat < ADC_LOW_BAT) return 1;

    return 0;
}

uint32 adc_read_current(uint8 motor)
{
    uint32 voltage;

    voltage = sample_voltage(motor);

    return (voltage / ADC_AMPLIF * ADC_SHUNT);
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
    PINSEL1 |= (1 << 24) | (1 << 26) | (1 << 28);  //P0.28 - AD0.1, P0.29 - AD0.2, P0.30 - AD0.3

    AD0CR |= (ADC_CLKDIV << 8);                    //11 clocks / 10 bits @ 3.75MHz
    AD0INTEN |= (1 << 1) | (1 << 2) | (1 << 3);    //Enable channel 1, 2 and 3 interrupt

    VICVectAddr5 = (uint32) ADC0isr;               //Set the ADC0 ISR vector address
    VICVectCntl5 = 0x00000020 | 18;                //Set channel for ADC0
    VICIntEnable |= (1 << 18);                     //Enable the interrupt ADC0

    AD0CR |= (1 << 21);                            //AD0 is operational
}

void adc_start(uint8 ch)
{
    if (ch > ADC_NR_CHANNELS - 1) return;

    AD0CR &= ~(0xFF);
    AD0CR |= (1 << (ch + 1));

    AD0CR |= (1 << 24);                            //Start conversion now
}

void ADC0isr(void)	__irq
{
    uint32 status;
    uint32 result;
    
    status = AD0STAT;

    if (status & (1 << 1))                         //Channel 1 is Done
    {
        result = AD0DR1;
        result = (result >> 6) & 0x03FF;

        sample_add(0, result);
    }

    if (status & (1 << 2))                         //Channel 2 is Done
    {
        result = AD0DR2;
        result = (result >> 6) & 0x03FF;

        sample_add(1, result);
    }

    if (status & (1 << 3))                         //Channel 3 is Done
    {
        result = AD0DR3;
        result = (result >> 6) & 0x03FF;

        sample_add(2, result);
    }

    AD0CR &= ~(7 << 24);                           //Stop ADC now
    VICVectAddr  = 0x00000000;                     //Dummy write to signal end of interrupt
}
