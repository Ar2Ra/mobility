/******************************************************************************/
/*
    Mobility Platform Main File

    Clock info:
        CCLK = 60 MHz
        PCLK = 15 MHz
*/
/******************************************************************************/

#include <LPC214x.H>                    /* LPC214x definitions                */
#include <stdio.h>

#include "Serial.h"
#include "Motor.h"
#include "Hall.h"
#include "Energy.h"
//#include ""

//=============================================
void delay(unsigned int cnt);

void led_init(void);
void led_set(char state);
void led_bit(int set, char bit);
//=============================================

int main(void)
{
    /*
        Initialize peripherals
    */
    dir_init();

    init_serial();
    pwm_init();
    capture_init();
    //adc_init();    

    led_init();
    //=======================================    

    led_set(0x00);
    robot_fw();         //The default direction of the robot is forward
           
    while (1) 
    {
    }
    
}

void delay(unsigned int cnt) 
{
    unsigned int i, j;

    for (i = 0; i < cnt; i++)
        for (j = 0; j < 1000; j++);
}

void led_init(void)
{
    IO1DIR |= (0xFF << 16);
    IO1CLR |= (0xFF << 16);
}

void led_set(char state)
{
    IO1CLR |= (0xFF << 16);
    IO1SET |= (state << 16);
}

void led_bits(int set, char bit)
{
    if (set)
        IO1SET |= (bit << 16);
    else
        IO1CLR |= (bit << 16);
}
