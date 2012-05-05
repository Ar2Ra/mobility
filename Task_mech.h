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
int task_config(unsigned int nr, unsigned int state);

#endif  // __TASK_MECH_H
