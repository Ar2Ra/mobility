/*
    UART
    Low level serial routines

    Uart.h
*/

#ifndef __UART_H
#define __UART_H

// <h> Setup Communication Speed
//   <o> VPB Clock <0-60000000>
#define VPB_CLOCK 15000000

//   <o> Baudrate 
//        <0=>     600 Bauds
//        <1=>    1200 Bauds
//        <2=>    2400 Bauds
//        <3=>    4800 Bauds
//        <4=>    9600 Bauds
//        <5=>   14400 Bauds
//        <6=>   19200 Bauds
//        <7=>   28800 Bauds
//        <8=>   38400 Bauds
//        <9=>   56000 Bauds
//        <10=>  57600 Bauds
//        <11=> 115200 Bauds
#define UART0_BAUD_ID 11
#define UART0_BAUD_RATE \
  ((UART0_BAUD_ID ==  0) ?    600 : \
   (UART0_BAUD_ID ==  1) ?   1200 : \
   (UART0_BAUD_ID ==  2) ?   2400 : \
   (UART0_BAUD_ID ==  3) ?   4800 : \
   (UART0_BAUD_ID ==  4) ?   9600 : \
   (UART0_BAUD_ID ==  5) ?  14400 : \
   (UART0_BAUD_ID ==  6) ?  19200 : \
   (UART0_BAUD_ID ==  7) ?  28800 : \
   (UART0_BAUD_ID ==  8) ?  38400 : \
   (UART0_BAUD_ID ==  9) ?  56000 : \
   (UART0_BAUD_ID == 10) ?  57600 : \
   (UART0_BAUD_ID >= 11) ? 115200 : 115200)

// </h>

void init_uart0 (void);             //Initialize Serial Interface
uint8 uart0_sendchar (uint8 ch);    //Implementation of putchar (also used by printf function to output data)
uint8 uart0_getkey (void);          //Read character from Serial Port

#endif  // __UART_H
