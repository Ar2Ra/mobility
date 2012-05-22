/*
    Command interpreter
*/

#ifndef __COMMANDS_H
#define __COMMANDS_H

#define CMD_NUMB 8
#define CMD_SIZE 24

/*
    Circular Command Buffer (CCB) interface
    Used in UART ISR to add commands
    and in the main routine to check and execute them.
*/
uint8 commands_pending(void);
int32 add_cmd(uint8 *cmd, uint8 source);
int32 exec_cmd(void);

/*
    Interpret a command
*/
void interpret(uint8 *cmd, uint8 source);

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
