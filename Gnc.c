/*
    Guidance, navigation and control

    Gnc.c
*/

#include <stdio.h>
#include "Type.h"

#include "Pid.h"
#include "Gnc.h"
#include "Pwm.h"
#include "Task_mech.h"
#include "Task_list.h"

uint32 pulse[2];
uint32 pulse_active[2];
uint32 pulses_needed = 0;

void gnc_full_stop(void)
{
    robot_stop();
    pid_set_target(0, 0);
    pid_set_target(1, 0);
}

void gnc_motor_stop(uint8 nr)
{
    pid_set_target(nr, 0);
}

void gnc_scheduled_stop(uint32 period)
{
    task_set_period(TASK_SCHEDULED_STOP, period);
    task_enable(TASK_SCHEDULED_STOP);
}

void gnc_set_speed(pid_type target)
{
    pid_set_target(0, target);
    pid_set_target(1, target);
}

void gnc_hall_pulse(uint8 nr)
{
    if (!pulse_active[nr]) return;

    if (pulse[nr] < pulses_needed)
    {
        pulse[nr]++;
    }
    else
    {
        //fprintf(stdout, "%d %d\r\n", pulse_active[nr], pulse[nr]);

        pulse_active[nr] = 0;
        gnc_motor_stop(nr);       
    }
}

void gnc_hall_set(uint32 pulses)
{
    pulse[0] = 0;
    pulse[1] = 0;

    pulse_active[0] = 1;
    pulse_active[1] = 1;

    pulses_needed = pulses;
}
