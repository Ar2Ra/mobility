/*
    Digital control system

    Pid.c
*/

#include "Type.h"
#include "Pid.h"

typedef struct _pid_struct pid_struct;

struct _pid_struct
{
    int32 pGain;        //Proportional Gain
    int32 iGain;        //Integral Gain
    int32 dGain;        //Derivative Gain

    int32 iMin;         //Minimum allowable integrator state
    int32 iMax;         //Maximum allowable integrator state

    int32 iState;       //Integrator State
    int32 dState;       //Last value input

    int32 target;
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

int32 pid_execute(uint8 nr, int32 readVal)
{
    int32 pTerm, iTerm, dTerm, _error;
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

int32 pid_reset(uint8 nr)
{
    if (nr > PID_NR - 1) return -1;

    pid[nr].iState = 0;
    pid[nr].dState = 0;
    
    return 0;
}

int32 pid_set_target(uint8 nr, int32 target)
{
    if (nr > PID_NR - 1) return -1;

    pid[nr].target = target;
    return 0;
}

int32 pid_set_gain(uint8 nr, int32 pGain, int32 iGain, int32 dGain)
{
    if (nr > PID_NR - 1) return -1;

    if (pGain != -1) pid[nr].pGain = pGain;
    if (iGain != -1) pid[nr].iGain = iGain;
    if (dGain != -1) pid[nr].dGain = dGain;

    return 0;
}
