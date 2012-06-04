/*
    Digital control system

    Pid.c
*/

#include "Type.h"
#include "Pid.h"
#include "Hall.h"

typedef struct _pid_struct pid_struct;

struct _pid_struct
{
    pid_type pGain;        //Proportional Gain
    pid_type iGain;        //Integral Gain
    pid_type dGain;        //Derivative Gain

    pid_type iMin;         //Minimum allowable integrator state
    pid_type iMax;         //Maximum allowable integrator state

    pid_type iState;       //Integrator State
    pid_type dState;       //Last value input

    pid_type target;       //Command value
};

pid_struct pid[PID_NR];

void pid_init(void)
{
    uint8 i;

    for (i = 0; i < PID_NR; i++)
    {
        pid[i].iMax = INTEGRATOR_MAX;
        pid[i].iMin = INTEGRATOR_MIN;
    }
}

pid_type pid_execute(uint8 nr, pid_type readVal)
{
    pid_type pTerm, iTerm, dTerm, _error;
    
    if (nr > PID_NR - 1) return -1;

    _error = pid[nr].target - readVal;             //Compute error
    pTerm = pid[nr].pGain * _error;                //Calculate proportional term

    pid[nr].iState += _error;                      //Calculate the integral state with appropriate limiting
    
    if (pid[nr].iState > pid[nr].iMax)
        pid[nr].iState = pid[nr].iMax;

    if (pid[nr].iState < pid[nr].iMin)
        pid[nr].iState = pid[nr].iMin;

    iTerm = pid[nr].iGain * pid[nr].iState;        //Calculate the integral term

    dTerm = pid[nr].dGain *                        //Calculate the derivative term
        (readVal - pid[nr].dState);

    pid[nr].dState = readVal;

    return pTerm + iTerm - dTerm;
}

pid_type pid_get_target(uint8 nr)
{
    if (nr > PID_NR - 1) return -1;

    return pid[nr].target;
}

int32 pid_reset(uint8 nr)
{
    if (nr > PID_NR - 1) return -1;

    pid[nr].iState = 0;
    pid[nr].dState = 0;

    return 0;
}

int32 pid_set_target(uint8 nr, pid_type target)
{
    if (nr > PID_NR - 1) return -1;

    if (target < CRITICAL_SPEED * hall_get_res())
    {
        pid_reset(nr);
    }

    pid[nr].target = target * hall_get_res();
    return 0;
}

int32 pid_set_gain(uint8 nr, pid_type pGain, pid_type iGain, pid_type dGain)
{
    if (nr > PID_NR - 1) return -1;

    if (pGain != -1) pid[nr].pGain = pGain;
    if (iGain != -1) pid[nr].iGain = iGain;
    if (dGain != -1) pid[nr].dGain = dGain;

    return 0;
}
