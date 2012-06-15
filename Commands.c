/*
    Command interpreter
*/

#include <stdio.h>
#include <string.h>
#include "Type.h"

#include "Commands.h"
#include "Task_mech.h"
#include "Task_list.h"
#include "Pwm.h"
#include "Hall.h"
#include "Adc.h"
#include "Bluetooth.h"
#include "Pid.h"
#include "Gnc.h"

char *CompileTime = __TIME__;
char *CompileDate = __DATE__;

extern uint8 motor_debug;

uint32 s_pulse1 = 80;
uint32 s_target1 = 70;

uint32 s_pulse2 = 85;
uint32 s_target2 = 75;

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

    if (cmd[0] == '.')              //advanced command
    {
        cmd = cmd + 1;
        advanced_cmd(id, cmd);
        return;
    }

    if (cmd[0] == '_')              //debug command
    {
        cmd = cmd + 1;
        debug_cmd(id, cmd);
        return;
    }

    simple_cmd(cmd[0]);             //simple command
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
        gnc_full_stop();    //duty cycle 0 both motors and PID target 0
        break;
   
    case '1':
        //robot_pwm(80);      //slow
        pwm_set_percent(1, 82);
        pwm_set_percent(2, 70);
        break;
    
    case '2':
        //robot_pwm(90);      //medium
        pwm_set_percent(1, 90);
        pwm_set_percent(2, 88);
        break;
    
    case '3':
        //robot_pwm(100);     //fast
        pwm_set_percent(1, 100);
        pwm_set_percent(2, 97);
        break;
    }
}

void advanced_cmd(uint8 id, uint8 *str)
{
    uint8 flag_dual_op = 0;
    uint8 i, nr;
    uint32 period, target, pulses, op1, op2;

    pid_type pGain, iGain, dGain;

    FILE *f;
    f = (id == 0) ? stdout : stderr;

    if (str[0] == 's')  //Scheduled stop
    {
        period = 0;

        for (i = 1; str[i] != '\0'; i++)
            period = (period * 10) + (str[i] - '0');

        gnc_scheduled_stop(period);

        return;
    }

    if (str[0] == 'w')  //Set angular speed for both motors
    {
        target = 0;
        for (i = 1; str[i] != '\0'; i++)
        {                       
            if (str[i] == '-')
            {
                op1 = target;
                target = 0;
                flag_dual_op = 1;
            }
            else
                target = (target * 10) + (str[i] - '0');
        }
        
        if (flag_dual_op)
        {
            op2 = target;
            pid_set_target(0, op1);
            pid_set_target(1, op2);
            flag_dual_op = 0;
        }
        else
            gnc_set_speed(target);

        return;
    }

    if (str[0] == 'l') //left
    {
        robot_left();

        if (str[1] != '\0')
        {
            sscanf((const char *) str, "l%d-%d", &pulses, &target);
            s_pulse1 = pulses;
            s_target1 = target;
        }
        
        gnc_hall_set_all(s_pulse1);
        gnc_set_speed(s_target1);
    }

    if (str[0] == 'r') //right
    {
        robot_right();

        if (str[1] != '\0')
        {
            sscanf((const char *) str, "r%d-%d", &pulses, &target);
            s_pulse2 = pulses;
            s_target2 = target;
        }

        gnc_hall_set_all(s_pulse2);
        gnc_set_speed(s_target2);
    }

    if (str[0] == 'h')  //Hall pulses
    {
        pulses = 0;
        for (i = 1; str[i] != '\0'; i++)
        {
            if (str[i] == '-')
            {
                op1 = pulses;
                pulses = 0;
                flag_dual_op = 1;
            }
            else
                pulses = (pulses * 10) + (str[i] - '0');
        }

        if (flag_dual_op)
        {
            op2 = pulses;
            gnc_hall_set(0, op1);
            gnc_hall_set(1, op2);
            flag_dual_op = 0;
        }
        else
            gnc_hall_set_all(pulses);

        return;
    }

    if (str[0] == 'd') //distance traveled
    {
        if (str[1] == 'r')
        {
            gnc_distance_reset();
            fprintf(f, "(D) reset\r\n");
            return;
        }

        nr = str[1] - '0';
        fprintf(f, "(D) nr: %d dist: %d\r\n", nr, gnc_distance_get(nr));

        return;
    }

    if (str[0] == 'v')  //which motor data to collect [MATLAB]
    {
        nr = str[1] - '0';

        if (nr == 1 || nr == 2)
        {
            motor_debug = nr;
            fprintf(f, "(V) motor %d\r\n", nr);
        }

        return;
    }

    if (str[0] == 'p')  //PID debug
    {
        nr = str[1] - '0';

        if (str[2] == 'r')  //reset
        {
            pid_reset(nr);
            fprintf(f, "(PID) %d reset\r\n", nr);
            
            return;
        }      

        if (str[2] == 't')  //target
        {
            target = 0;
            for (i = 3; str[i] != '\0'; i++)
                target = (target * 10) + (str[i] - '0');
            
            pid_set_target(nr, target);
            fprintf(f, "(PID) %d target %d\r\n", nr, target);

            return;
        }

        if (str[2] == 'p')  //gain
        {
            #if (PID_INT == 1)
            
            sscanf((const char *) str, "p%dp%di%dd%d", 
                (int *) &nr, (int *) &pGain, (int *) &iGain, (int *) &dGain);
            fprintf(f, "(PID) %d pGain: %d iGain: %d dGain: %d\r\n", nr, pGain, iGain, dGain);
            
            #else
            
            sscanf((const char *) str, "p%dp%fi%fd%f", 
                (int *) &nr, (float *) &pGain, (float *) &iGain, (float *) &dGain);
            fprintf(f, "(PID) %d pGain: %.3f iGain: %.3f dGain: %.3f\r\n", nr, pGain, iGain, dGain);
            
            #endif
            
            pid_set_gain(nr, pGain, iGain, dGain);
        }
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
        if (strlen((const char *) str) < 5)
        {
            fprintf(f, "[PWM] input err\r\n");
            return;
        }

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

        return;
    }

    if (str[0] == 'f')  //Read frequency [capture signals]
    {
        if (str[1] == 'r') //resolution
        {
            data = 0;
            for (i = 2; str[i] != '\0'; i++)
                data = (data * 10) + (str[i] - '0');

            hall_set_res(data);
            fprintf(f, "[HALL] res: %d\r\n", data);
            return;
        }

        motor = str[1] - '0';
        if (motor < 1 || motor > 2)
        {
            fprintf(f, "[HALL] input err\r\n");
            return;
        }      

        fprintf(f, "[HALL %d] PWM: %d Avg: %d Freq: %d\r\n",
            motor, pwm_get_raw(motor), hall_filter_get(motor - 1), hall_get(motor));

        return;
    }

    if (str[0] == 'd')  //Set direction
    {
        motor = str[1] - '0';
        dir = str[2] - '0';

        if (dir_set(motor, dir) < 0)
            fprintf(f, "[DIR] input err\r\n");
        else
            fprintf(f, "[DIR] motor: %d dir: %d\r\n", motor, dir);

        return;
    }

    if (str[0] == 'm')  //Motor status
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

        fprintf(f, "[MOTOR %d] PWM: %d (%d%%) DIR: %d\r\n", motor, data, percent, dir);

        return;
    }

    if (str[0] == 'v')  //Get compile time
    {
        fprintf(f, "[COMPILED] %s - %s\r\n", CompileDate, CompileTime );

        return;
    }
    
    if (str[0] == 't')  //Task configuration
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
        
        if (str[2] == 's') //Get status (if enabled or not)
        {
            fprintf(f, "[TASK] %d enabled: %d\r\n", nr, task_get_status(nr));
            return;
        }

        state = str[2] - '0';
        
        if ( task_config(nr, state) < 0 )
            fprintf(f, "[TASK] input err\r\n");
        else
            fprintf(f, "[TASK] %d state: %d\r\n", nr, state);

        return;
    }

    if (str[0] == 'a')  //ADC
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

        return;
    }

    if (str[0] == 'b')  //Bluetooth
    {
        //Redirect Bluetooth AT commands directly from UART0 to UART1
        if ( (str[1] == '_')  && (f == stdout) )
        {
            fprintf(stderr, "%s\r", str + 2);
            return;
        }

        //If only "_b" command was issued, then print out connection status
        fprintf(f, "[BT] Con: %d\r\n", bt_connected());
    }
}
