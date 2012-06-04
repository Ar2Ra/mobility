/*
    Motor Control
    Speed and Direction

    Pwm.c
*/

#include <LPC214x.H>
#include "Type.h"

#include "Pwm.h"

uint8 motor1_dir;
uint8 motor2_dir;

void pwm_init(void)
{
    PINSEL0 |= (1 << 15);                         //PWM2
    PINSEL1 |= (1 << 10);                         //PWM5

    PWMPR = 0x00000000;                           //Load prescaler maximum

    PWMMR0 = PWM_M0;                              //Match 0 - frequency

    //currently duty cycle is 0%    
    PWMMR2 = 0;                                   //Duty cycle PWM2
    PWMMR5 = 0;                                   //Duty cycle PWM5
    PWMLER |= (1 << 0) | (1 << 2) | (1 << 5);     //Latch enable Match2 & Match5

    PWMMCR = 0x00000002;                          //On match with timer reset the counter
    PWMPCR |= (1 << 10) | (1 << 13);              //The PWM2 & PWM5 output enabled.
    PWMTCR = 0x00000002;                          //Reset counter and prescaler
    PWMTCR = 0x00000009;                          //PWM Enable & Counter Enable

    PWMLER = 0;                                   //isis
}

int32 pwm_set_raw(uint8 motor, int32 val)
{
    if (val < 0) val = 0;
    if (val > PWM_M0) val = PWM_M0;

    switch (motor)
    {
        case 1:                         //Set channel 1 only
            PWMMR2 = val;
            break;
        case 2:                         //Set channel 2 only
            PWMMR5 = val;
            break;
        case 3:                         //Set both channels
            PWMMR2 = val;
            PWMMR5 = val;
            break;
        default:
            return -1;
    }

    PWMLER |= (1 << 2) | (1 << 5);      //Latch enable Match2 & Match5
    return 0;                           //SUCCESS
}

int32 pwm_set_percent(uint8 motor, uint8 percent)
{
    uint32 val;

    if (percent > 100) percent = 100;

    val = PWM_M0 * ( (float) percent / 100.0 );

    return (pwm_set_raw(motor, val));
}

uint32 pwm_get_raw(uint8 motor)
{
    if (motor == 1) return PWMMR2;
    if (motor == 2) return PWMMR5;

    return 0;
}

uint8 pwm_get_percent(uint8 motor)
{
    uint32 raw;
    raw = pwm_get_raw(motor);

    return ( ((float) raw / PWM_M0) * 100 );
}

void dir_init(void)
{
    /*
        L298N
        Input 1	- P1.24
        Input 2	- P1.25
        Input 3	- P1.26
        Input 4	- P1.27
    */

    //Pin 24, 25, 26, 27 (PORT 1) as output (motor direction)
    IO1DIR |= (0x0000000F << 24);

    //Clear all outputs
    IO1CLR |= (0x0000000F << 24);
}

int32 dir_set(uint8 motor, uint8 dir)
{
    uint8 in1, in2;
    
    switch (motor)
    {
        case 1:                 //MOTOR 1
            in1 = 24;           //Input 1
            in2 = 25;           //Input 2
            motor1_dir = dir;   //Store current direction
            break;
        case 2:                 //MOTOR 2
            in1 = 26;           //Input 3
            in2 = 27;           //Input 4
            motor2_dir = dir;   //Store current direction
            break;
        default:
            return -1;
    }

    switch (dir)
    {
        case 1:             //Forward
            IO1SET |= (1 << in1);
            IO1CLR |= (1 << in2);
            break;
        case 2:             //Backward
            IO1CLR |= (1 << in1);
            IO1SET |= (1 << in2);
            break;
        case 3:             //Break
            IO1SET |= (1 << in1);
            IO1SET |= (1 << in2);
            break;
        default:
            return -2;
    }

    return 0;
}

uint8 dir_get(uint8 motor)
{
    if (motor == 1) return motor1_dir;
    if (motor == 2) return motor2_dir;

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
    dir_set(1, 1);
    dir_set(2, 2);
}

void robot_right(void)     //turn right (on spot)
{
    dir_set(1, 2);
    dir_set(2, 1);
}

void robot_brake(void)     //forced stop
{
    dir_set(1, 3);
    dir_set(2, 3);
}

void robot_stop(void)               //disengage motors, duty cycle 0%
{
    pwm_set_percent(3, 0);
}

void robot_pwm(uint8 percent)     //set speed
{
    pwm_set_percent(3, percent);
}
