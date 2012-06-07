/*
    Guidance, navigation and control

    Gnc.h
*/

#ifndef __GNC_H
#define __GNC_H


//Signals on LED2 and P1.28 if special commands (scheduled stop, hall counter)
//are currently executing
void gnc_signal_command(uint8 state);

// Robot full stop
void gnc_full_stop(void);
 
//nr = 0 - M1 right motor
//nr = 1 - M2 left motor
void gnc_motor_stop(uint8 nr);

//Robot will stop in "period" miliseconds
void gnc_scheduled_stop(uint32 period);

//Maintain robot speed at "target" rotations per second
void gnc_set_speed(pid_type target);

//Increment Hall pulses (used by Hall ISR or Timer Capture ISR)
void gnc_hall_pulse(uint8 nr);

//How many Hall pulses to count for motor 'nr' (0 or 1) until it will stop
void gnc_hall_set(uint8 nr, uint32 pulses);

//How many Hall pulses to count for each motor until it will stop
void gnc_hall_set_all(uint32 pulses);

//Deactivate pulse counter stop
void gnc_hall_reset(void);

//Distance traveled related functions (for each motor)
//for now, just pulse count
void gnc_distance_reset(void);
uint32 gnc_distance_get(uint8 nr);

#endif  // __GNC_H
