/*
    Motor Control
    Speed and Direction

    Pwm.h
*/

#ifndef __PWM_H
#define __PWM_H

/*
    Initialize PWM module
*/
void pwm_init(void);

/*
    Set duty cycle in percentage [0-100] %
    for one of the two PWM channels
    nr - 1 (left motor pwm)
    nr - 2 (right motor pwm)
    nr - 3 (set same duty cycle for both motors)

    pwm_set(3, 0);    (soft stop)
    pwm_set(3, 100);  (full speed)
*/
int32 pwm_set(uint8 nr, uint8 percent);

/*
    Initialize the 4 output pins for motor direction
*/
void dir_init(void);

/*
    motor = 1, 2
	
    dir
	1 - forward
	2 - backward
	3 - brake
*/
int32 dir_set(uint8 motor, uint8 dir);

/*
    Robot basic movement functions [alpha]
*/
//==============================================================

//These 5 functions only set the direction and do not start the robot.
void robot_fw(void);                        //set movement forward
void robot_bw(void);                        //set movement backward
void robot_left(void);                      //set turn left
void robot_right(void);                     //set turn right
void robot_brake(void);                     //activate forced stop (brake)
//--------------------------------------------------------------
void robot_stop(void);                      //disengage motors
                                            //pwm set to 0%

void robot_speed(uint8 percent);            //set speed
//==============================================================

#endif  // __PWM_H
