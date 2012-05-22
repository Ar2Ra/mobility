/*
    Command interpreter
*/

#include <stdio.h>
#include <string.h>
#include "Type.h"

#include "Commands.h"
#include "Task_mech.h"
#include "Pwm.h"
#include "Hall.h"
#include "Adc.h"

char *CompileTime = __TIME__;
char *CompileDate = __DATE__;

typedef struct _ccb_struct ccb_struct;

struct _ccb_struct
{
    uint8 cmd[CMD_SIZE];
    uint8 source;
};

ccb_struct ccb[CMD_NUMB];   //Command Circular Buffer

uint8 nr_cmds = 0;
uint8 add_pos = 0;
uint8 exec_pos = 0;

uint8 commands_pending(void)
{
    return nr_cmds;
}

int32 add_cmd(uint8 *cmd, uint8 source)
{
    if (nr_cmds == CMD_NUMB) return -1;

    ccb[add_pos].source = source;
    strcpy((char *) ccb[add_pos].cmd, (const char *) cmd);
    nr_cmds++;

    add_pos = (add_pos + 1) % CMD_NUMB;
    
    return 0;
}

void interpret(uint8 *cmd, uint8 source)
{
    if (cmd[0] == '_') //debug command
    {
        cmd = cmd + 1;
        debug_cmd(cmd);
        return;
    }

    simple_cmd(cmd[0]);
}

int32 exec_cmd(void)
{
    if (nr_cmds == 0) return -1;
    
    interpret(ccb[exec_pos].cmd, ccb[exec_pos].source);
    nr_cmds--;

    exec_pos = (exec_pos + 1) % CMD_NUMB;
    
    return 0;
}

void simple_cmd(uint8 ch)
{
    switch (ch)
    {
    case 'w':               //forward
        robot_fw();
        break;
    
    case 's':               //backward
        robot_bw();
        break;
    
    case 'a':               //left
        robot_left();
        break;
    
    case 'd':               //right
        robot_right();
        break;

    case 'b':
        robot_brake();      //brake forced stop
        break;

    case 'q':
        robot_stop();       //duty cycle 0 both motors
        break;
   
    case '1':
        robot_speed(80);    //slow
        break;
    
    case '2':
        robot_speed(90);    //medium
        break;
    
    case '3':
        robot_speed(100);   //fast
        break;
    }
}

void debug_cmd(uint8 *str)
{
    uint8 i;
    uint8 nr, state, percent;
    uint8 motor, dir;
    uint32 period, data;
      
    if (str[0] == 'p')  //PWM set duty cycle
    {
       nr = str[1] - '0';
       
       percent = 0;
       for (i = 0; i < 3; i++)
       {
         percent = percent * 10 + str[i + 2] - '0';
       }
       
       if (pwm_set(nr, percent) < 0)
           printf("[PWM] input err\n\r");
       else
           printf("[PWM] %d set %d\n\r", nr, percent);
    }
    
    if (str[0] == 'f')  //Read frequency [capture signals]
    {
        nr = str[1] - '0';

        switch (nr)
        {
            case 1:
                printf("[Hall] f1: %d\n\r", hall_get(1));
                break;
            case 2:
                printf("[Hall] f2: %d\n\r", hall_get(2));
                break;
            default:
                printf("[Hall] input err\n\r");
        }
    }

    if (str[0] == 'd')  //Set direction
    {
        motor = str[1] - '0';
        dir = str[2] - '0';

        if (dir_set(motor, dir) < 0)
            printf("[DIR] input err\n\r");
        else
            printf("[DIR] motor: %d dir: %d\n\r", motor, dir);
    }

    if (str[0] == 'v')  //Get version (testing)
    {
        printf( "Compiled on %s at %s\n\r", CompileDate, CompileTime );
    }
    
    if (str[0] == 't') //Task configuration
    {
        nr = str[1] - '0';
        
        if (str[2] == 'p')  //Set period
        {
            period = 0;
            for (i = 3; str[i] != '\0'; i++)
                period = (period * 10) + (str[i] - '0');

            task_set_period(nr, period);
            printf("[TASK] %d period: %d\n\r", nr, period);
            return;
        }

        state = str[2] - '0';
        task_config(nr, state);
        printf("[TASK] %d state: %d\n\r", nr, state);
    }

    if (str[0] == 'a') //ADC
    {
        nr = str[1] - '0';
        data = sample_voltage(nr);

        printf("[ADC] %d ch: %d\n\r", nr, data);
    }
}
