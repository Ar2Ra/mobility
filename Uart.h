/*
    UART
    Low level serial routines

    Uart.h
*/

#ifndef __UART_H
#define __UART_H

void init_uart (void);           //Initialize Serial Interface
uint8 sendchar (uint8 ch);       //Implementation of putchar (also used by printf function to output data)
uint8 getkey (void);             //Read character from Serial Port

#endif  // __UART_H
