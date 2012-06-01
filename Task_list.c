/*
    Task_list.c

    Tasks
*/

#include <stdio.h>
#include "Type.h"

#include "Task_list.h"
#include "Task_mech.h"
#include "Hall.h"
#include "Adc.h"
#include "Pwm.h"
#include "Bluetooth.h"
#include "Ssp.h"

uint8 sample_channel = 0;

extern void led_bits(uint8 set, uint8 bit);

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
    //start conversion on current sample channel
    adc_start(sample_channel);

    //change the sample channel for future task executions
    sample_channel = (sample_channel + 1) % ADC_NR_CHANNELS;
}

void bt_broadcast(void)
{
    uint32 battery, motor1, motor2;  

    //Send only if bluetooth is connected
    if (bt_connected())
    {
        battery = adc_read_battery();
        motor1 = adc_read_current(1);
        motor2 = adc_read_current(2);

        fprintf(stderr, "%d;%d;%d\r\n", battery, motor1, motor2);
    }
}

void check_battery(void)
{
    if (adc_low_battery())
        led_bits(1, 1); //set led1
    else
        led_bits(0, 1); //clear led1
}

void robot_scheduled_stop(void)
{
    robot_stop();
    task_disable(4);
}

void task_debug1(void)
{
    ssp_send('A');
}
