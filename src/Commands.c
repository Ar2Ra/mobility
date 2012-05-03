/*
    Command interpreter
*/

#include <stdio.h>
#include "Commands.h"

void simple_cmd(char ch)
{
    printf("Simple cmd: %c\n\r", ch);
}

void debug_cmd(char *str)
{
    printf("Debug cmd: %s\n\r", str);
}
