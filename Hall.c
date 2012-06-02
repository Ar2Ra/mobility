/*
    Hall sensor read data

    Hall.c
*/

#include <LPC214x.H>
#include "Type.h"

#include "Hall.h"

uint32 hall_res;

uint32 last_capture1 = 0, signal_freq1 = 0;
uint32 last_capture2 = 0, signal_freq2 = 0;

void T1isr(void)    __irq;

typedef struct _hall_struct hall_struct;

struct _hall_struct
{
    uint32 buffer[HALL_NR_SAMPLES];
    uint16 counter;
};

hall_struct hall_filter[HALL_NR_SENSORS];

uint32 hall_get_res(void)
{
    return hall_res;
}

void hall_set_res(uint32 res)
{
    hall_res = res;
}

void hall_filter_add(uint8 ch, uint32 sample)
{
    uint16 pos;
    pos = hall_filter[ch].counter;

    hall_filter[ch].buffer[pos] = sample;
    pos = (pos + 1) % HALL_NR_SAMPLES;

    hall_filter[ch].counter = pos;
}

uint32 hall_filter_get(uint8 ch)
{
    uint16 i;
    uint32 sum = 0;

    for (i = 0; i < HALL_NR_SAMPLES; i++)
        sum += hall_filter[ch].buffer[i];

    return (sum / HALL_NR_SAMPLES);
}

void capture_init(void)
{
    PINSEL0 |= (1 << 21);               //Capture 1.0 (Timer 1) P0.10
    PINSEL0 |= (1 << 23);               //Capture 1.1 (Timer 1) P0.11  

    T1PR = 0x0000000E;                  //Load prescaler maximum [1 usec tick] (+1 stuff)
    T1TCR = 0x00000002;                 //Reset counter and prescaler
    T1CCR = 0x00000005 | 0x00000028;    //Capture on rising edge of channel [0 + 1] + interrupt
    T1TCR = 0x00000001;                 //Enable timer

    VICVectAddr4 = (uint32) T1isr;      //Set the timer ISR vector address
    VICVectCntl4 = 0x00000020 | 5;      //Set channel for TIMER1
    VICIntEnable |= (1 << 5);           //Enable the interrupt TIMER1
}

uint32 hall_get(uint8 motor)
{
    if (motor == 1) return signal_freq1;            //Rotations per second for left motor
    if (motor == 2) return signal_freq2;            //Rotations per second for right motor

    return 0;
}

void hall_update(uint8 motor, uint32 new_value)
{
    if (motor == 1) signal_freq1 = new_value;       //Override data from 1st Hall sensor
    if (motor == 2) signal_freq2 = new_value;       //Override data from 2nd Hall sensor
    
    return;
}

void hall_reset(uint8 motor)
{
    switch (motor)
    {
    case 1:                                         //Reset data from 1st Hall sensor
        signal_freq1 = 0;
        break;
    case 2:                                         //Reset data from 2nd Hall sensor
        signal_freq2 = 0;
        break;
    case 3:                                         //Reset data from both Hall sensors
        signal_freq1 = 0;
        signal_freq2 = 0;
    }
}

uint32 compute_freq(uint32 current, uint32 last)
{
    uint32 den;
    den = current - last;

    return (1000000 * hall_get_res() / den);
}

uint32 hall_now(uint8 motor)
{
    if (motor == 1)
        return compute_freq(T1TC, last_capture1);

    if (motor == 2)
        return compute_freq(T1TC, last_capture2);

    return 0;
}

void T1isr(void)	__irq
{
    uint32 value;

    if (T1IR & (1 << 4))                        //Capture channel 0
    {
        value = T1CR0;       
    
        signal_freq1 = compute_freq(value, last_capture1);
        last_capture1 = value;
    
        T1IR |= (1 << 4);                       //Clear capture 0 interrupt
    }

    if (T1IR & (1 << 5))                        //Capture channel 1
    {
        value = T1CR1;

        signal_freq2 = compute_freq(value, last_capture2);
        last_capture2 = value;

        T1IR |= (1 << 5);                       //Clear capture 1 interrupt
    }
    
    VICVectAddr  = 0x00000000;                  //Dummy write to signal end of interrupt
}
