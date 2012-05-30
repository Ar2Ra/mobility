/*
    Task_list.c

    Tasks
*/

//test

#include <stdio.h>
#include "Type.h"

#include "Task_list.h"
#include "Hall.h"
#include "Adc.h"

uint8 sample_channel = 0;

void hall_timeout(void)
{
    uint8 motor_nr;
    uint32 motor_speed, motor_speed_now;

    for (motor_nr = 1; motor_nr <=2; motor_nr++)
    {
        motor_speed = hall_get(motor_nr);
        if (motor_speed > 0)
        {
            motor_speed_now = hall_now(motor_nr);
    
            if (motor_speed_now < motor_speed)
            {
                //printf("%d: %d\r\n", motor_nr, motor_speed_now); 
                hall_update(motor_nr, motor_speed_now);
            }
        }
    }
}

void energy_adc(void)
{
    adc_start(sample_channel);
    sample_channel = (sample_channel + 1) % ADC_NR_CHANNELS;
}
