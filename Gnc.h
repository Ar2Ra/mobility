/*
    Guidance, navigation and control

    Gnc.h
*/

#ifndef __GNC_H
#define __GNC_H

/*
    Robot full stop
*/
void gnc_full_stop(void);

/*
    Robot will stop in "period" miliseconds
*/
void gnc_scheduled_stop(uint32 period);

/*
    Maintain robot speed at "target" rotations per second
*/
void gnc_set_speed(pid_type target);

#endif  // __GNC_H
