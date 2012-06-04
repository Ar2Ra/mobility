/*
    Guidance, navigation and control

    Gnc.c
*/

#include "Type.h"

#include "Pid.h"
#include "Gnc.h"
#include "Pwm.h"
#include "Task_mech.h"
#include "Task_list.h"

void gnc_full_stop(void)
{
    robot_stop();
    pid_set_target(0, 0);
    pid_set_target(1, 0);
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
