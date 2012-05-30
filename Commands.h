/*
    Command interpreter
*/

#ifndef __COMMANDS_H
#define __COMMANDS_H

/* ===================================== */

#define CCB_NR   2
#define CMD_NUMB 8
#define CMD_SIZE 24

/* ===================================== */

/*
    Circular Command Buffer (CCB) interface
    Used in UART ISR to add commands
    and in the main routine to check and execute them.
*/
uint8 commands_pending(uint8 id);
int32 add_cmd(uint8 id, uint8 *cmd);
int32 exec_cmd(uint8 id);

/*
    Interpret a command
*/
void interpret(uint8 id, uint8 *cmd);

/*
    Simple 1 character commands
    Basic robot functions
*/
void simple_cmd(uint8 ch);

/*
    All debug commands start with '_' character
    For now, UART only (printf function)
*/
void debug_cmd(uint8 id, uint8 *str);

#endif  // __COMMANDS_H
