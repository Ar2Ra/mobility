//Uart.H

#ifndef __UART_H
#define __UART_H

void init_uart (void);       /* Initialize Serial Interface */
int sendchar (int ch);       /* implementation of putchar (also used by printf function to output data) */
int getkey (void);           /* Read character from Serial Port */

#endif  // __UART_H
