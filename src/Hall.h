/*
    Hall sensor read data
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
    Reset the measurements to 0 for
    1 - left motor, 2 - right motor, 3 - both motors
*/
void hall_reset(int nr);

#endif  // __HALL_H
