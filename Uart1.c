/*
    UART1
    Low level serial routines

    Uart1.c
*/

#include <LPC21xx.H>
#include <stdio.h>
#include "Type.h"

#include "Misc.h"
#include "Uart1.h"
#include "Commands.h"

uint8 cmd_buffer1[CMD_SIZE];
uint8 pointer1 = 0;

void UART1isr(void)	__irq;

void init_uart1 (void)                                      //Initialize Serial Interface
{              
    PINSEL0 |= 0x00050000;                                  //Enable RxD and TxD pins
    
    U1LCR = 0x83;                                           //8 bits, no Parity, 1 Stop bit
    U1DLL = (VPB_CLOCK/16/UART1_BAUD_RATE) & 0xFF;          //Setup Baudrate
    U1DLM = ((VPB_CLOCK/16/UART1_BAUD_RATE) >> 8) & 0xFF;
    U1LCR = 0x03;                                           //DLAB = 0
    U1IER |= (1 << 0);

    VICVectAddr2 = (uint32) UART1isr;                       //Set the UART1 ISR vector address
    VICVectCntl2 = 0x00000020 | 7;                          //Set channel for UART
    VICIntEnable |= (1 << 7);                               //Enable the interrupt UART
}


//Implementation of putchar (also used by printf() function to output data)
uint8 uart1_sendchar (uint8 ch)  
{                                                   //Write character to Serial Port
    while (!(U1LSR & 0x20));
    return (U1THR = ch);
}

uint8 uart1_getkey (void)                           //Read character from Serial Port
{
    while (!(U1LSR & 0x01));
    return (U1RBR);
}

void UART1isr(void)	__irq
{
    uint8 ch;
    uint32 status = (U1IIR >> 1) & 0x00000007;

    if (status == 2)                              //Receive Data Available (RDA).
    {
        ch = U1RBR;                               //Read received character
        
        if (ch != '\r' && ch != '\n')             //All commands end with these chars
        {
            cmd_buffer1[pointer1] = ch;           //Store character in buffer
            pointer1++;                           //and increment pointer
        }
        else
        {
            cmd_buffer1[pointer1] = '\0';
            
            /* cmd_buffer ready */
            
            if (pointer1 > 0)                     //If command is not empty
                add_cmd(1, cmd_buffer1);
            
            /* end */
            
            pointer1 = 0;
            cmd_buffer1[0] = '\0';
        }
    }

    VICVectAddr  = 0x00000000;                    //Dummy write to signal end of interrupt
}
