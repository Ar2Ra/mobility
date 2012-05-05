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

/*
    Tasks config
*/
//====================================================
#define NR_TASKS 2

//modify period of every task
unsigned int period[NR_TASKS] =
{
    200,
    400
};

unsigned int enable[NR_TASKS];
volatile unsigned int counter[NR_TASKS];

void (*task_list[NR_TASKS])(void) =
{
    my_task1,
    my_task2
};
//====================================================

int task_config(unsigned int nr, unsigned int state)
{
    if (nr < NR_TASKS)
    {
        enable[nr] = state;
        if (enable[nr] == 0) counter[nr] = 0;
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
        if (enable[i])
        {
            counter[i]++;
            if (counter[i] >= period[i])
            {
                counter[i] = 0;
                (*task_list[i])();
            }
        }

    T0IR |= 0x00000001;            //Clear match 0 interrupt
    VICVectAddr = 0x00000000;      //Dummy write to signal end of interrupt
}
