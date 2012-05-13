/*
    Task_list.c

    Tasks
*/

#include <stdio.h>
#include "Task_list.h"
#include "Hall.h"

void hall_timeout(void)
{
    unsigned int a, b;

    a = hall_get(1);
    if (a > 0)
    {
        b = hall_now(1);

        if (b < a) hall_update(1, b);
    }
}
