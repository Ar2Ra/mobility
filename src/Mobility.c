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

//=============================================
void delay(unsigned int cnt);
void led_init(void);
void led_set(unsigned char state);
//=============================================

int main(void)
{
    dir_init();

    init_serial();
    pwm_init();
    capture_init();
    
    led_init();
    led_set(0xAA);
    
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

void led_set(unsigned char state)
{
    IO1CLR |= (0xFF << 16);
    IO1SET |= (state << 16);
}
