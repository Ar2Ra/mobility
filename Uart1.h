/*
    UART1
    Low level serial routines

    Uart1.h
*/

#ifndef __UART1_H
#define __UART1_H

// <h> Setup Communication Speed

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
#define UART1_BAUD_ID 4
#define UART1_BAUD_RATE \
  ((UART1_BAUD_ID ==  0) ?    600 : \
   (UART1_BAUD_ID ==  1) ?   1200 : \
   (UART1_BAUD_ID ==  2) ?   2400 : \
   (UART1_BAUD_ID ==  3) ?   4800 : \
   (UART1_BAUD_ID ==  4) ?   9600 : \
   (UART1_BAUD_ID ==  5) ?  14400 : \
   (UART1_BAUD_ID ==  6) ?  19200 : \
   (UART1_BAUD_ID ==  7) ?  28800 : \
   (UART1_BAUD_ID ==  8) ?  38400 : \
   (UART1_BAUD_ID ==  9) ?  56000 : \
   (UART1_BAUD_ID == 10) ?  57600 : \
   (UART1_BAUD_ID >= 11) ? 115200 : 115200)

// </h>

void init_uart1 (void);             //Initialize Serial Interface
uint8 uart1_sendchar (uint8 ch);    //Implementation of putchar (also used by printf function to output data)
uint8 uart1_getkey (void);          //Read character from Serial Port

#endif  // __UART1_H
