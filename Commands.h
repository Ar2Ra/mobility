/*
    Command interpreter
*/

#ifndef __COMMANDS_H
#define __COMMANDS_H

/*
    Simple 1 character commands
    Basic robot functions
*/
void simple_cmd(uint8 ch);

/*
    All debug commands start with '_' character
    For now, UART only (printf function)
*/
void debug_cmd(uint8 *str);

#endif  // __COMMANDS_H
