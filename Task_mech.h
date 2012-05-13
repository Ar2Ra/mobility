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

/*
    Instead of using "task_config()" one can also use
    these functions:
*/
int task_enable(unsigned int nr);
int task_disable(unsigned int nr);

#endif  // __TASK_MECH_H
