/*
    Hall sensor read data

    Hall.c
*/

#include <LPC214x.H>
#include "Type.h"

#include "Hall.h"

/*
    Resolution
    1 - Hz
    1000 - mHz
*/
#define RES 1

uint32 last_capture1 = 0, signal_freq1 = 0;
uint32 last_capture2 = 0, signal_freq2 = 0;

void T1isr(void)    __irq;

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

uint32 hall_get(uint8 nr)
{
    if (nr == 1) return signal_freq1;   //Rotations per second for left motor
    if (nr == 2) return signal_freq2;   //Rotations per second for right motor

    return 0;
}

void hall_update(uint8 nr, uint32 new_value)
{
    if (nr == 1) signal_freq1 = new_value;
    if (nr == 2) signal_freq2 = new_value;
    
    return;
}

void hall_reset(uint8 nr)
{
    switch (nr)
    {
    case 1:
        signal_freq1 = 0;
        break;
    case 2:
        signal_freq2 = 0;
        break;
    case 3:
        signal_freq1 = 0;
        signal_freq2 = 0;
    }
}

uint32 compute_freq(uint32 current, uint32 last)
{
    uint32 den;
    den = current - last;

    return (1000000 * RES / den);
}

uint32 hall_now(uint8 nr)
{
    if (nr == 1)
        return compute_freq(T1TC, last_capture1);

    if (nr == 2)
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
