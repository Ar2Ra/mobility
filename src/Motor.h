/*
    Motor Control
    Speed and Direction
*/

#ifndef __MOTOR_H
#define __MOTOR_H

/*
    Initialize PWM module
*/
void pwm_init(void);

/*
    Set duty cycle in percentage [0-100] %
    for one of the two PWM channels
*/
int pwm_set(unsigned int nr, unsigned int percent);

/*
    Initialize the 4 output pins for motor direction
*/
void dir_init(void);

/*
    motor = 1,2
	
    dir
	1 - forward
	2 - backward
	3 - brake
*/
int dir_set(unsigned int motor, unsigned int dir);

#endif  // __MOTOR_H
