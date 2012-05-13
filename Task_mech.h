/*
    Task_mech.h

    Task mechanism
*/

#ifndef __TASK_MECH_H
#define __TASK_MECH_H

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

#endif  // __TASK_MECH_H
