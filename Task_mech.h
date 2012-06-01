/*
    Task_mech.h

    Task mechanism
*/

#ifndef __TASK_MECH_H
#define __TASK_MECH_H

#define ISR_CYCLE 1000 //ISR @ every ISR_CYCLE useconds

/*
    Initialize Timer0 for tasks
*/
void task_init(void);

/*
    Configure tasks
    if state > 0 then task with number "nr" is enabled
*/
int32 task_config(uint8 nr, uint8 state);

/*
    Instead of using "task_config()" one can also use
    these functions:
*/
int32 task_enable(uint8 nr);
int32 task_disable(uint8 nr);

/*
    Set the period of the task with number "nr"
*/
int32 task_set_period(uint8 nr, uint32 period);

/*
    Returns 1 if task is activated and 0 otherwise
*/
uint8 task_get_status(uint8 nr);

#endif  // __TASK_MECH_H
