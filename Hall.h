/*
    Hall sensor read data

    Hall.h
*/

#ifndef __HALL_H
#define __HALL_H

/*
    The angular speed or frequency [in Hz] at which the current motor speed can be
    considered practically 0.
*/
#define CRITICAL_SPEED 4

/*
    Two sensors - one for each motor
*/
#define HALL_NR_SENSORS   2

/*
    How many samples for one filter
*/
#define HALL_NR_SAMPLES  20

/*
    Resolution
    1 - Hz
    1000 - mHz
*/
uint32 hall_get_res(void);
void hall_set_res(uint32 res);

/*
    Initialize timer and the 2 capture pins
*/
void capture_init(void);

/*
    Read the motor speed (rotations / sec)
    for each motor (1, 2)
*/
uint32 hall_get(uint8 motor);

/*
    Change speed manually, from outside the capture ISR
    This function is used in a task for Hall sensor timeout
*/
void hall_update(uint8 motor, uint32 new_value);

/*
    Reset the measurements to 0 for
    1 - left motor, 2 - right motor, 3 - both motors
*/
void hall_reset(uint8 motor);

/*
    Computes a virtual speed using the difference between the
    current Timer1 counter (T1TC) and the last captured value
    motor (1, 2)
*/
uint32 hall_now(uint8 motor);

/*
    Functions for Hall filters - sample averaging of motor speed
*/
void hall_filter_add(uint8 ch, uint32 sample);
uint32 hall_filter_get(uint8 ch);

#endif  // __HALL_H
