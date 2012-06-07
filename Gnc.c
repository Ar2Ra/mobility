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
#include "Hall.h"

uint32 pulse[HALL_NR_SENSORS];
uint32 pulse_active[HALL_NR_SENSORS];
uint32 pulse_needed[HALL_NR_SENSORS];

uint32 distance[HALL_NR_SENSORS];

extern void led_bits(uint8 set, uint8 bit);

void gnc_signal_command(uint8 state)
{
    if (state)
    {
        led_bits(1, 2); //set led2
        IO1SET |= (1 << 28); //set P1.28

        return;
    }

    led_bits(0, 2); //clear led2
    IO1CLR |= (1 << 28); //clear P1.28
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
    distance[nr]++;
    if (!pulse_active[nr]) return;

    if (pulse[nr] < pulse_needed[nr])
    {
        pulse[nr]++;
    }
    else
    {
        pulse_active[nr] = 0;
        gnc_motor_stop(nr);
        if (pulse_active[0] == 0 && pulse_active[1] == 0)
            gnc_signal_command(0);
    }
}

void gnc_hall_set(uint8 nr, uint32 pulses)
{
    gnc_signal_command(1);

    pulse[nr] = 0;
    pulse_active[nr] = 1;
    pulse_needed[nr] = pulses;
}

void gnc_hall_set_all(uint32 pulses)
{
    gnc_hall_set(0, pulses);
    gnc_hall_set(1, pulses);
}

void gnc_hall_reset(void)
{
    gnc_signal_command(0);

    pulse_active[0] = 0;
    pulse_active[1] = 0;
}

void gnc_distance_reset(void)
{
    distance[0] = 0;
    distance[1] = 0;
}

uint32 gnc_distance_get(uint8 nr)
{
    if (nr > HALL_NR_SENSORS - 1) return 0;
    return distance[nr];
}
