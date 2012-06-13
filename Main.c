/******************************************************************************/
/*
    Mobility Platform Main File

    Clock info:
        CCLK = 60 MHz
        PCLK = 15 MHz
*/
/******************************************************************************/

#include <LPC214x.H>
#include <stdio.h>
#include "Type.h"

#include "Uart0.h"
#include "Uart1.h"
#include "Pwm.h"
#include "Hall.h"
#include "Adc.h"
#include "Ssp.h"
#include "Task_mech.h"
#include "Commands.h"
#include "Bluetooth.h"
#include "Pid.h"

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
    //=======================================
    IO1DIR |= (1 << 28);  //Output for command execution signal
    IO1CLR |= (1 << 28);

    dir_init();           //GPIO direction outputs for H-Bridge

    init_uart0();         //UART0 for debugging
    init_uart1();         //UART1 for bluetooth
    ssp_init();           //SSP in SPI mode

    pwm_init();           //PWM 2-channels driving the H-Bridge
    capture_init();       //Capture inputs for Hall Sensors [Timer 1]
    adc_init();           //Analog to Digital Converters [Battery voltage & Motor currents]
    task_init();          //Simple task mechanism [functions that execute at x miliseconds] [Timer0]
    led_init();           //8 LEDs onboard MCB2140 P1.16 - P1.23
    
    hall_set_res(1);      //Hall resolution [Hz * res] Hz, dHz, cHz, mHz
    pid_init();           //Initialize PID for both motors
    //=======================================

    /*
        Default state (values)
    */
    //=======================================
    task_enable(0);     //Enable Startup Task
    //bt_set_connected();
    
    task_enable(1);     //Enable Hall Timeout Task nr. 0
    task_enable(2);     //Enable PID
    task_enable(3);     //Enable ADC conversion initiation Task nr. 3
    //task_enable(4);     //Enable Bluetooth Broadcast Task nr. 4 [doesnt work without task 3]
    task_enable(5);     //Enable Check Battery Task nr. 5 [doesnt work without task 3]
                                                                                       
    //task_enable(7);     //Enable Debug Task 1

    led_set(0x00);      //All LEDs are off

    robot_stop();       //Initially stopped
    robot_fw();         //The default direction of the robot is forward
    //=======================================

    /*
        PID Tuning
        pid_set_gain(nr, pGain, iGain, dGain);
    */

    pid_set_gain(0, 14, 10, 0);    
    pid_set_gain(1, 14, 10, 0);
    //=======================================

    while (1)
    {
        if (commands_pending(0))
        {
            exec_cmd(0);
        }

        if (commands_pending(1))
        {
            exec_cmd(1);
        }
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
    if (bit > 7) return;

    if (set)
        IO1SET |= (1 << (16 + bit));
    else
        IO1CLR |= (1 << (16 + bit));
}
