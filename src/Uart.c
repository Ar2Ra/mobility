/******************************************************************************/
/* Uart.c: Low Level Uart Routines                                            */
/******************************************************************************/

#include <LPC21xx.H>                    /* LPC21xx definitions                */
#include "Uart.h"
#include "Commands.h"

// <o> Use UART <0-1>
#define UART 0

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
#define BAUD_ID 11
#define BAUD_RATE \
  ((BAUD_ID ==  0) ?    600 : \
   (BAUD_ID ==  1) ?   1200 : \
   (BAUD_ID ==  2) ?   2400 : \
   (BAUD_ID ==  3) ?   4800 : \
   (BAUD_ID ==  4) ?   9600 : \
   (BAUD_ID ==  5) ?  14400 : \
   (BAUD_ID ==  6) ?  19200 : \
   (BAUD_ID ==  7) ?  28800 : \
   (BAUD_ID ==  8) ?  38400 : \
   (BAUD_ID ==  9) ?  56000 : \
   (BAUD_ID == 10) ?  57600 : \
   (BAUD_ID >= 11) ? 115200 : 115200)

// </h>


#if (UART == 0)
  #define UxPINS  0x00000005
  #define UxRBR   U0RBR
  #define UxTHR   U0THR
  #define UxLCR   U0LCR
  #define UxLSR   U0LSR
  #define UxDLL   U0DLL
  #define UxDLM   U0DLM
  #define UxIER   U0IER
  #define UxIIR   U0IIR
  #define VIC_MASK 6
#else
  #define UxPINS  0x00050000
  #define UxRBR   U1RBR
  #define UxTHR   U1THR
  #define UxLCR   U1LCR
  #define UxLSR   U1LSR
  #define UxDLL   U1DLL
  #define UxDLM   U1DLM
  #define UxIER   U1IER
  #define UxIIR   U1IIR
  #define VIC_MASK 7
#endif

#define BUF_SIZE 16

char buffer[BUF_SIZE];

int pointer = 0;
int flag_debug = 0;

void UARTisr(void)	__irq;

void init_uart (void)                                 //Initialize Serial Interface
{              
    PINSEL0 |= UxPINS;                                //Enable RxD and TxD pins
    UxLCR = 0x83;                                     //8 bits, no Parity, 1 Stop bit
    UxDLL = (VPB_CLOCK/16/BAUD_RATE) & 0xFF;          //Setup Baudrate
    UxDLM = ((VPB_CLOCK/16/BAUD_RATE) >> 8) & 0xFF;
    UxLCR = 0x03;                                     //DLAB = 0
    UxIER |= (1 << 0);

    VICVectAddr1 = (unsigned) UARTisr;                //Set the UART ISR vector address
    VICVectCntl1 = 0x00000020 | VIC_MASK;             //Set channel for UART
    VICIntEnable |= (1 << VIC_MASK);                  //Enable the interrupt UART
}


//Implementation of putchar (also used by printf() function to output data)
int sendchar (int ch)  
{                                                 //Write character to Serial Port
  while (!(UxLSR & 0x20));
  return (UxTHR = ch);
}

int getkey (void)                                 //Read character from Serial Port
{
  while (!(UxLSR & 0x01));
  return (UxRBR);
}

void UARTisr(void)	__irq
{
    char ch;
    unsigned int status = (UxIIR >> 1) & 0x00000007;

    if (status == 2)                              //Receive Data Available (RDA).
    {
        ch = UxRBR;                               //Read received character
        
        if (flag_debug)
        {
            if (ch != '\r')                       //All advanced commands end with this char
            {
                buffer[pointer] = ch;             //Store character in buffer
                pointer++;                        //and increment pointer
            }
            else
            {
                buffer[pointer] = '\0';
                debug_cmd(buffer);                //String completed, ready to send
                
                pointer = 0;
                buffer[0] = '\0';
                flag_debug = 0;
            }
        }
        else
        {
            switch (ch)
            {
            case '_':                             //Debug commands start with this char
                flag_debug = 1;
                break;
            default:
                simple_cmd(ch);                   //No specified characters found, sending simple command
            }
        }
    }

    VICVectAddr  = 0x00000000;                    //Dummy write to signal end of interrupt
}
