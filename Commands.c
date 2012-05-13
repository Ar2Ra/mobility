/*
    Command interpreter
*/

#include <stdio.h>
#include "Type.h"

#include "Commands.h"
#include "Motor.h"
#include "Hall.h"

#define VERSION "Alpha-5"

void simple_cmd(uint8 ch)
{
    switch (ch)
    {
    case 'w':               //forward
        robot_fw();
        break;
    
    case 's':               //backward
        robot_bw();
        break;
    
    case 'a':               //left
        robot_left();
        break;
    
    case 'd':               //right
        robot_right();
        break;

    case 'b':
        robot_brake();      //brake forced stop
        break;

    case 'q':
        robot_stop();       //duty cycle 0 both motors
        break;
   
    case '1':
        robot_speed(80);    //slow
        break;
    
    case '2':
        robot_speed(90);    //medium
        break;
    
    case '3':
        robot_speed(100);   //fast
        break;
    }
}

void debug_cmd(uint8 *str)
{
    uint8 i;
    uint8 nr, percent;
    uint8 motor, dir;
      
    if (str[0] == 'p')  //PWM set duty cycle
    {
       nr = str[1] - '0';
       
       percent = 0;
       for (i = 0; i < 3; i++)
       {
         percent = percent * 10 + str[i + 2] - '0';
       }
       
       if (pwm_set(nr, percent) < 0)
           printf("[PWM] input err\n\r");
       else
           printf("[PWM] %d set %d\n\r", nr, percent);
    }
    
    if (str[0] == 'f')  //Read frequency [capture signals]
    {
        nr = str[1] - '0';

        switch (nr)
        {
            case 1:
                printf("[Hall] f1: %d\n\r", hall_get(1));
                break;
            case 2:
                printf("[Hall] f2: %d\n\r", hall_get(2));
                break;
            default:
                printf("[Hall] input err\n\r");
        }
    }

    if (str[0] == 'd')
    {
        motor = str[1] - '0';
        dir = str[2] - '0';

        if (dir_set(motor, dir) < 0)
            printf("[DIR] input err\n\r");
        else
            printf("[DIR] motor: %d dir: %d\n\r", motor, dir);
    }

    if (str[0] == 'v')
    {
        printf(VERSION);
        printf("\n\r");
    }       
}
