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
#include "Type.h"

#include "Uart.h"
#include "Pwm.h"
#include "Hall.h"
#include "Adc.h"
#include "Spi.h"
#include "Task_mech.h"

//=============================================
void delay(uint32 cnt);

void led_init(void);
void led_set(uint8 state);
void led_bits(uint8 set, uint8 bit);
//=============================================

int main(void)
{
    /*
        Initialize peripherals
    */
    dir_init();

    init_uart();
    pwm_init();
    capture_init();
    adc_init();
    //spi_init();    
    task_init();
    led_init();
    //=======================================    

    //task_enable(0);

    led_set(0x00);
    
    robot_stop();       //Initially stopped
    robot_fw();         //The default direction of the robot is forward
           
    while (1) 
    {
    }
    
}

//simple delay function [for testing only]
void delay(uint32 cnt) 
{
    uint32 i, j;

    for (i = 0; i < cnt; i++)
        for (j = 0; j < 1000; j++);
}

void led_init(void)
{
    IO1DIR |= (0xFF << 16);
    IO1CLR |= (0xFF << 16);
}

void led_set(uint8 state)
{
    IO1CLR |= (0xFF << 16);
    IO1SET |= (state << 16);
}

void led_bits(uint8 set, uint8 bit)
{
    if (set)
        IO1SET |= (bit << 16);
    else
        IO1CLR |= (bit << 16);
}
