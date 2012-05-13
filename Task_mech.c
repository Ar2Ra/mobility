/*
    Task_mech.c

    Task mechanism
*/

#include <stdio.h>
#include <LPC214x.H>

#include "Task_mech.h"
#include "Task_list.h"

#define ISR_CYCLE 1000 //ISR @ every ISR_CYCLE useconds
void T0isr (void)    __irq;

typedef struct _task_struct
{
    unsigned int period;
    void (*name)(void);
    unsigned int enable;
    volatile unsigned int counter;
} task_struct;

/*
    Tasks config
*/
//====================================================
#define NR_TASKS 2

task_struct task[NR_TASKS] = 
{
    {200, my_task1, 0, 0},
    {400, my_task2, 0, 0}
};

//====================================================

int task_config(unsigned int nr, unsigned int state)
{
    if (nr < NR_TASKS)
    {
        task[nr].enable = state;
        if (task[nr].enable == 0) task[nr].counter = 0;
        return 0;
    }

    return -1;
}

int task_enable(unsigned int nr)
{
    if (nr < NR_TASKS)
    {
        task[nr].enable = 1;
        return 0;
    }

    return -1;
}

int task_disable(unsigned int nr)
{
    if (nr < NR_TASKS)
    {
        task[nr].enable = 0;
        task[nr].counter = 0;
        return 0;
    }

    return -1;
}

void task_init(void)
{
    T0PR  = 0x0000000E;                  //Load prescaler (14 + 1). PCLK / 15 = 1 MHz

    T0TCR = 0x00000002;                  //Reset counter and prescaler
    T0MCR = 0x00000003;                  //On match reset the counter and generate an interrupt
    T0MR0 = ISR_CYCLE;                   //Set the cycle time
    
    T0TCR = 0x00000001;                  //Enable timer

    VICVectAddr2 = (unsigned) T0isr;     //Set the timer ISR vector address
    VICVectCntl2 = 0x00000020 | 4;       //Set channel for TIMER0
    VICIntEnable |= 0x00000010;          //Enable the interrupt TIMER1
}

void T0isr (void)	__irq
{
    int i;

    for (i = 0; i < NR_TASKS; i++)
        if (task[i].enable)
        {
            task[i].counter++;
            if (task[i].counter >= task[i].period)
            {
                task[i].counter = 0;
                (*task[i].name)();
            }
        }

    T0IR |= 0x00000001;            //Clear match 0 interrupt
    VICVectAddr = 0x00000000;      //Dummy write to signal end of interrupt
}
