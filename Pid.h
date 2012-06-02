/*
    Digital control system

    Pid.h
*/

#ifndef __PID_H
#define __PID_H

/*
    Number of PIDS, one for each motor
*/
#define PID_NR 2

#define INTEGRATOR_MAX  3750
#define INTEGRATOR_MIN  0

void pid_init(void);

int32 pid_execute(uint8 nr, int32 readVal);
int32 pid_reset(uint8 nr);

int32 pid_set_target(uint8 nr, int32 target);
int32 pid_set_gain(uint8 nr, int32 pGain, int32 iGain, int32 dGain);

#endif  // __PID_H
