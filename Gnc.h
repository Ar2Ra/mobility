/*
    Guidance, navigation and control

    Gnc.h
*/

#ifndef __GNC_H
#define __GNC_H

/*
    Robot full stop
*/
void gnc_full_stop(void);

/*    
    nr = 0 - M1 right motor
    nr = 1 - M2 left motor
*/
void gnc_motor_stop(uint8 nr);

/*
    Robot will stop in "period" miliseconds
*/
void gnc_scheduled_stop(uint32 period);

/*
    Maintain robot speed at "target" rotations per second
*/
void gnc_set_speed(pid_type target);

/*
    Increment Hall pulses (used by Hall ISR or Timer Capture ISR)
*/
void gnc_hall_pulse(uint8 nr);

/*
    How many Hall pulses to count for each motor until it will stop
*/
void gnc_hall_set(uint32 pulses);

#endif  // __GNC_H
