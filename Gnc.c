/*
    Guidance, navigation and control

    Gnc.c
*/

#include <LPC214x.H>
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

extern void led_bits(uint8 set, uint8 bit);

void gnc_signal_command(uint8 state)
{
    if (state)
    {
        led_bits(1, 2); //set led2
        IO1SET |= (1 << 28);

        return;
    }

    led_bits(0, 2); //set led2
    IO1CLR |= (1 << 28);
}

void gnc_full_stop(void)
{
    robot_stop();
    pid_set_target(0, 0);
    pid_set_target(1, 0);
}

void gnc_motor_stop(uint8 nr)
{    
    pid_set_target(nr, 0);
    pwm_set_raw(nr + 1, 0);
}

void gnc_scheduled_stop(uint32 period)
{
    task_set_period(TASK_SCHEDULED_STOP, period);
    task_enable(TASK_SCHEDULED_STOP);

    gnc_signal_command(1);
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
        if (pulse_active[0] == 0 && pulse_active[1] == 0)
            gnc_signal_command(0);
    }
}

void gnc_hall_counter(uint32 pulses)
{
    gnc_signal_command(1);

    pulse[0] = 0;
    pulse[1] = 0;

    pulse_active[0] = 1;
    pulse_active[1] = 1;

    pulses_needed = pulses;
}

void gnc_hall_reset(void)
{
    gnc_signal_command(0);

    pulse_active[0] = 0;
    pulse_active[1] = 0;
}
