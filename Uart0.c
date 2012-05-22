/*
    UART
    Low level serial routines

    Uart0.c
*/

#include <LPC21xx.H>
#include "Type.h"

#include "Uart0.h"
#include "Commands.h"

#define BUF_SIZE 16

uint8 buffer[BUF_SIZE];

uint8 pointer = 0;
uint8 flag_debug = 0;

void UART0isr(void)	__irq;

void init_uart0 (void)                                      //Initialize Serial Interface
{              
    PINSEL0 |= 0x00000005;                                  //Enable RxD and TxD pins
    U0LCR = 0x83;                                           //8 bits, no Parity, 1 Stop bit
    U0DLL = (VPB_CLOCK/16/UART0_BAUD_RATE) & 0xFF;          //Setup Baudrate
    U0DLM = ((VPB_CLOCK/16/UART0_BAUD_RATE) >> 8) & 0xFF;
    U0LCR = 0x03;                                           //DLAB = 0
    U0IER |= (1 << 0);

    VICVectAddr1 = (uint32) UART0isr;                       //Set the UART0 ISR vector address
    VICVectCntl1 = 0x00000020 | 6;                          //Set channel for UART
    VICIntEnable |= (1 << 6);                               //Enable the interrupt UART
}


//Implementation of putchar (also used by printf() function to output data)
uint8 uart0_sendchar (uint8 ch)  
{                                                   //Write character to Serial Port
    while (!(U0LSR & 0x20));
    return (U0THR = ch);
}

uint8 uart0_getkey (void)                           //Read character from Serial Port
{
    while (!(U0LSR & 0x01));
    return (U0RBR);
}

void UART0isr(void)	__irq
{
    uint8 ch;
    uint32 status = (U0IIR >> 1) & 0x00000007;

    if (status == 2)                              //Receive Data Available (RDA).
    {
        ch = U0RBR;                               //Read received character
        
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