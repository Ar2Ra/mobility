/*
    Hall sensor read data

    Hall.h
*/

#ifndef __HALL_H
#define __HALL_H

/*
    Initialize timer and the 2 capture pins
*/
void capture_init(void);

/*
    Read the motor speed (rotations / sec)
    for each motor (nr - 1, 2)
*/
unsigned int hall_get(int nr);

/*
    Change speed manually, from outside the capture ISR
    This function is used in a task for Hall sensor timeout
*/
void hall_update(int nr, unsigned int new_value);

/*
    Reset the measurements to 0 for
    1 - left motor, 2 - right motor, 3 - both motors
*/
void hall_reset(int nr);

/*
    Computes a virtual speed using the difference between the
    current Timer1 counter (T1TC) and the last captured value
    nr - 1 or 2 [motor 1 or 2]
*/
unsigned int hall_now(int nr);

#endif  // __HALL_H
