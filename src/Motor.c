/*
    Motor Control
    Speed and Direction

    Motor.c
*/

#include <LPC214x.H>
#include "Motor.h"

//Match 0 - frequency PWM [4 kHz] 15e6 / 4e3 = 3750
#define PWM_M0  3750

void pwm_init(void)
{
    PINSEL0 |= (1 << 17) | (1 << 19);             //PWM4 & PWM6

    PWMPR = 0x00000000;                           //Load prescaler maximum

    PWMMR0 = PWM_M0;                              //Match 0 - frequency

    //currently duty cycle is 0%    
    PWMMR4 = 0;                                   //Duty cycle PWM4
    PWMMR6 = 0;                                   //Duty cycle PWM6
    PWMLER |= (1 << 0) | (1 << 4) | (1 << 6);     //Latch enable Match4 & Match6

    PWMMCR = 0x00000002;                          //On match with timer reset the counter
    PWMPCR |= (1 << 12) | (1 << 14);              //The PWM4 & PWM6 output enabled.
    PWMTCR = 0x00000002;                          //Reset counter and prescaler
    PWMTCR = 0x00000009;                          //PWM Enable & Counter Enable

    PWMLER = 0;
}

int pwm_set(unsigned int nr, unsigned int percent)
{
    static unsigned int val;

    if (percent > 100) return 1;

    val = PWM_M0 * ( (float) percent / 100.0 );

    switch (nr)
    {
        case 1:
            PWMMR4 = val;
            break;
        case 2:
            PWMMR6 = val;
            break;
        case 3:
            PWMMR4 = val;
            PWMMR6 = val;
            break;
        default:
            return 2;           
    }

    PWMLER |= (1 << 4) | (1 << 6);      //Latch enable Match4 & Match6
    return 0;                           //SUCCESS
}

void dir_init(void)
{
    /*
        L298N
        Input 1	- P0.12
        Input 2	- P0.13
        Input 3	- P0.14
        Input 4	- P0.15
    */

    //Pin 12, 13, 14, 15 as output (motor direction)
    IO0DIR |= (0x0000000F << 12);

    //Clear all outputs
    IO0CLR |= (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15);
}

int dir_set(unsigned int motor, unsigned int dir)
{
    unsigned int in1, in2;

    switch (motor)
    {
        case 1:             //MOTOR 1
            in1 = 13;
            in2 = 12;
            break;
        case 2:             //MOTOR 2
            in1 = 14;
            in2 = 15;
            break;
        default:
            return 1;
    }

    switch (dir)
    {
        case 1:             //Forward
            IO0SET |= (1 << in1);
            IO0CLR |= (1 << in2);
            break;
        case 2:             //Backward
            IO0CLR |= (1 << in1);
            IO0SET |= (1 << in2);
            break;
        case 3:             //Break
            IO0SET |= (1 << in1);
            IO0SET |= (1 << in2);
            break;
        default:
            return 2;
    }

    return 0;
}

void robot_fw(void)        //move forward
{
    dir_set(1, 1);
    dir_set(2, 1);
}

void robot_bw(void)        //move backward
{
    dir_set(1, 2);
    dir_set(2, 2);
}

void robot_left(void)      //turn left (on spot)
{
    dir_set(1, 2);
    dir_set(2, 1);
}

void robot_right(void)     //turn right (on spot)
{
    dir_set(1, 1);
    dir_set(2, 2);
}

void robot_brake(void)     //forced stop
{
    dir_set(1, 3);
    dir_set(2, 3);
}

void robot_stop(void)                      //disengage motors, duty cycle 0%
{
    pwm_set(3, 0);
}

void robot_speed(unsigned int percent)     //set speed
{
    pwm_set(3, percent);
}
