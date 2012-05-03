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

#define VERSION "Alpha-4"

//=============================================
void delay(unsigned int cnt);
void led_init(void);
void led_set(unsigned char state);
//=============================================

int main(void)
{
    unsigned int ch, i;
    unsigned int nr, percent;
    unsigned int motor, dir;

    dir_init();

    init_serial();
    pwm_init();
    capture_init();
    
    led_init();
    led_set(0xAA);
           
    while (1) 
    {
        ch = getkey();
        
        if (ch == 'p')  //PWM set duty cycle
        {
           ch = getkey();
           nr = ch - '0';
           
           percent = 0;
           for (i = 0; i < 3; i++)
           {
             ch = getkey();
             percent = percent * 10 + ch - '0';
           }
           
           if (pwm_set(nr, percent))
               printf("[PWM] invalid input\n\r");
           else
               printf("[PWM] %d set %d\n\r", nr, percent);
        }
        
        if (ch == 'f')  //Read frequency [capture signals]
        {
            ch = getkey();
            nr = ch - '0';

            switch (nr)
            {
                case 1:
                    printf("[Frequency] signal 1: %d\n\r", hall_get(1));
                    break;
                case 2:
                    printf("[Frequency] signal 2: %d\n\r", hall_get(2));
                    break;
                default:
                    printf("[Frequency] invalid input\n\r");
            }
        }

        if (ch == 'd')
        {
            ch = getkey();
            motor = ch - '0';
            ch = getkey();
            dir = ch - '0';

            if (dir_set(motor, dir))
                printf("[DIR] invalid input");
            else
                printf("[DIR] motor: %d dir: %d\n\r", motor, dir);
        }

        if (ch == 'v')
        {
            printf(VERSION);
            printf("\n\r");
        }
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
