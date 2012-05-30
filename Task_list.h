/*
    Task_list.h

    Tasks
*/

#ifndef __TASK_LIST_H
#define __TASK_LIST_H

#define NR_TASKS 3

/*
    hall_timeout

    This function solves the following problem:
        if the motors are running and then they are stopped quickly, it's possible that
        the Hall sensors will retain a value greater than zero, even though the axis isn't rotating
        [because no more timer capture interrupts are generated to update the motor speed]
*/
void hall_timeout(void);

/*
    energy_adc

    This function starts an ADC conversion on one of the three channels
    (battery voltage, motor1 resistor shunt voltage, motor2 resistor shunt voltage)
    Each time this function is called a different channel is sampled (through rotation)
*/
void energy_adc(void);

/*
    bt_broadcast

    Broadcasts over bluetooth: battery voltage and current through the motors.
*/
void bt_broadcast(void);

#endif  // __TASK_LIST_H
