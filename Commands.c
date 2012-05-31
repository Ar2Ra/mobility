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
#include "Bluetooth.h"

char *CompileTime = __TIME__;
char *CompileDate = __DATE__;

//Command circular buffer
typedef struct _ccb_struct ccb_struct;

struct _ccb_struct
{
    uint8 cmd[CMD_NUMB][CMD_SIZE];
    uint8 nr_cmds;
    uint8 add_pos;
    uint8 exec_pos;
};

/*
    id:
        0 - UART0
        1 - UART1
        2 - SSP (SPI) //to do
*/
ccb_struct ccb[CCB_NR];   //Command Circular Buffers

uint8 commands_pending(uint8 id)
{
    return ccb[id].nr_cmds;
}

int32 add_cmd(uint8 id, uint8 *cmd)
{
    uint8 p;
    if (ccb[id].nr_cmds == CMD_NUMB) return -1;

    p = ccb[id].add_pos;
    strcpy((char *) ccb[id].cmd[p], (const char *) cmd);
    ccb[id].nr_cmds++;

    ccb[id].add_pos = (ccb[id].add_pos + 1) % CMD_NUMB;
    
    return 0;
}

void interpret(uint8 id, uint8 *cmd)
{
    if (id == 1)                    //on UART1
    {
        if (bt_valid_cmd(cmd))      //Is this a bluetooth system command?
        {
            bt_command(cmd);        //Send the command to the appropriate handler
        }
    }

    if (cmd[0] == '_')              //debug command
    {
        cmd = cmd + 1;
        debug_cmd(id, cmd);
        return;
    }  

    simple_cmd(cmd[0]);
}

int32 exec_cmd(uint8 id)
{
    uint8 p;
    if (ccb[id].nr_cmds == 0) return -1;

    p = ccb[id].exec_pos;    
    interpret(id, ccb[id].cmd[p]);
    ccb[id].nr_cmds--;

    ccb[id].exec_pos = (ccb[id].exec_pos + 1) % CMD_NUMB;
    
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

void debug_cmd(uint8 id, uint8 *str)
{
    uint8 i;
    uint8 nr, state, percent;
    uint8 motor, dir;
    uint32 period, data;
    
    FILE *f;
    f = (id == 0) ? stdout : stderr;
      
    if (str[0] == 'p')  //PWM set duty cycle
    {
       motor = str[1] - '0';
       
       percent = 0;
       for (i = 0; i < 3; i++)
       {
         percent = percent * 10 + str[i + 2] - '0';
       }
       
       if (pwm_set_percent(motor, percent) < 0)
           fprintf(f, "[PWM] input err\r\n");
       else
           fprintf(f, "[PWM] %d set %d\r\n", motor, percent);
    }
    
    if (str[0] == 'f')  //Read frequency [capture signals]
    {
        motor = str[1] - '0';
        if (motor < 1 || motor > 2)
        {
            fprintf(f, "[HALL] input err\r\n");
            return;
        }

        fprintf(f, "[HALL %d] PWM - %d Freq - %d\r\n", motor, pwm_get_raw(motor), hall_get(motor));
    }

    if (str[0] == 'd')  //Set direction
    {
        motor = str[1] - '0';
        dir = str[2] - '0';

        if (dir_set(motor, dir) < 0)
            fprintf(f, "[DIR] input err\r\n");
        else
            fprintf(f, "[DIR] motor: %d dir: %d\r\n", motor, dir);
    }

    if (str[0] == 'm') //Motor status
    {
        motor = str[1] - '0';
        if (motor < 1 || motor > 2)
        {
            fprintf(f, "[MOTOR] input err\r\n");
            return;
        }

        data = pwm_get_raw(motor);
        percent = pwm_get_percent(motor);
        dir = dir_get(motor);

        fprintf(f, "[MOTOR %d] PWM - %d (%d%%) DIR - %d\r\n", motor, data, percent, dir);
    }

    if (str[0] == 'v')  //Get compile time
    {
        fprintf(f, "[COMPILED] %s - %s\r\n", CompileDate, CompileTime );
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
            fprintf(f, "[TASK] %d period: %d\r\n", nr, period);
            return;
        }

        state = str[2] - '0';
        task_config(nr, state);
        fprintf(f, "[TASK] %d state: %d\r\n", nr, state);
    }

    if (str[0] == 'a') //ADC
    {
        nr = str[1] - '0';       

        switch (nr)
        {
        case 0:
            fprintf(f, "[ADC] Battery: %d\r\n", adc_read_battery());
            break;
        case 1:
            fprintf(f, "[ADC] Motor1: %d\r\n", adc_read_current(1));
            break;
        case 2:
            fprintf(f, "[ADC] Motor2: %d\r\n", adc_read_current(2));
            break;
        default:
            fprintf(f, "[ADC] input err\r\n");
        }
    }

    if (str[0] == 'b') //Bluetooth
    {
        //Redirect Bluetooth AT commands directly from UART0 to UART1
        if ( (str[1] == '_')  && (f == stdout) )
        {
            fprintf(stderr, "%s\r", str + 2);
            return;
        }
        
        //If only "_b" command was issued, then print out connection status
        fprintf(f, "[BT] Connected: %d\r\n", bt_connected());
    }
}
