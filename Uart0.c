/*
    UART0
    Low level serial routines

    Uart0.c
*/

#include <LPC21xx.H>
#include <stdio.h>
#include "Type.h"

#include "Misc.h"
#include "Uart0.h"
#include "Commands.h"

uint8 cmd_buffer0[CMD_SIZE];
uint8 pointer0 = 0;

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
        
        if (ch != '\r')                           //All commands end with this char
        {
            cmd_buffer0[pointer0] = ch;           //Store character in buffer
            pointer0++;                           //and increment pointer
        }
        else
        {
            cmd_buffer0[pointer0] = '\0';
            
            /* cmd_buffer ready */
            
            add_cmd(0, cmd_buffer0);
            
            /* end */
            
            pointer0 = 0;
            cmd_buffer0[0] = '\0';
        }
    }

    VICVectAddr  = 0x00000000;                    //Dummy write to signal end of interrupt
}
