/*
    Command interpreter
*/

#ifndef __COMMANDS_H
#define __COMMANDS_H

/*
    Simple 1 character commands
    Basic robot functions
*/
void simple_cmd(char ch);

/*
    All debug commands start with '_' character
    For now, UART only (printf function)
*/
void debug_cmd(char *str);

#endif  // __COMMANDS_H
