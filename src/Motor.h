/*
    Motor Control
    Speed and Direction

    Motor.h
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
    nr - 1 (left motor pwm)
    nr - 2 (right motor pwm)
    nr - 3 (set same duty cycle for both motors)

    pwm_set(3, 0);    (soft stop)
    pwm_set(3, 100);  (full speed)
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

void robot_speed(unsigned int percent);     //set speed
                                            //if robot is stopped this will take effect
                                            //after "robot_start()" is called
//==============================================================

#endif  // __MOTOR_H
