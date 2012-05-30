/*
    Hall sensor read data

    Hall.h
*/

#ifndef __HALL_H
#define __HALL_H

/*
    Resolution
    1 - Hz
    1000 - mHz
*/
#define HALL_RES 1

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

#endif  // __HALL_H
