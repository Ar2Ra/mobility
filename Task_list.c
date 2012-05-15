/*
    Task_list.c

    Tasks
*/

#include <stdio.h>
#include "Type.h"

#include "Task_list.h"
#include "Hall.h"

//extern

void hall_timeout(void)
{
    uint32 a, b;

    a = hall_get(1);
    if (a > 0)
    {
        b = hall_now(1);

        if (b < a)
        {
            printf("%u\n\r", b); 
            hall_update(1, b);
        }
    }
}

void my_task1(void)
{
    printf("a");
}
