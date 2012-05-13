/*
    Spi mode - SSP (Synchronous Serial Port)
    Slave mode

    Spi.c
*/

#include <LPC214x.H>
#include "Type.h"

#include "Spi.h"

#define FIFOSIZE 8

void SSPisr(void)	__irq;

void spi_init(void)
{   
    /*
        P0.17 SCK1   bit 3
        P0.18 MISO1  bit 5
        P0.19 MOSI1  bit 7
        PO.20 SSEL1  bit 9
    */
    PINSEL1 |= 0x000002A8;

    SSPCR1 = (1 << 2);                         //Slave mode
    SSPCR0 = 0x0007;                           //Set data size to 8 bits, frame format SPI
                                               //CPOL = 0, CPHA = 0

    VICVectAddr0 = (uint32) SSPisr;            //Set the SSP ISR vector address
    VICVectCntl0 = 0x00000020 | 11;            //Set channel for SSP
    VICIntEnable |= (1 << 11);                 //Enable the interrupt SSP

    SSPCR1 = (1 << 2) | (1 << 1);              //Slave mode & Enable the SSP controller
}

void SSPisr(void)	__irq
{
    VICVectAddr  = 0x00000000;                 //Dummy write to signal end of interrupt
}
