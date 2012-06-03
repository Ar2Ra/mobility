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

/*
    Integration state limits
*/
#define INTEGRATOR_MAX  3750
#define INTEGRATOR_MIN  0

/*
    PID should work with fixed point coefficients or with floating point?
    PID_INT - 1 (int)
            - 0 (float)
*/
#define PID_INT 1

#if (PID_INT == 1)
typedef int32 pid_type;
#else
typedef float pid_type;
#endif

void pid_init(void);

/*
    These functions return a negative number if an error occured
*/
pid_type pid_execute(uint8 nr, pid_type readVal);
pid_type pid_get_target(uint8 nr);

int32 pid_reset(uint8 nr);
int32 pid_set_target(uint8 nr, pid_type target);
int32 pid_set_gain(uint8 nr, pid_type pGain, pid_type iGain, pid_type dGain);

#endif  // __PID_H
