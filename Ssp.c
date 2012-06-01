/*
    Spi mode - SSP (Synchronous Serial Port)

    Ssp.c
*/

#include <LPC214x.H>
#include <stdio.h>

#include "Type.h"

#include "Ssp.h"

void SSPisr(void)	__irq;

void ssp_init(void)
{   
    /*
        P0.17 SCK1   bit 3
        P0.18 MISO1  bit 5
        P0.19 MOSI1  bit 7
        PO.20 SSEL1  bit 9
    */
    PINSEL1 |= 0x000002A8;

    SSPCR1 = (1 << 0);                         //Loopback Mode

    SSPCR0 = 0x0007;                           //Set data size to 8 bits, frame format SPI, CPOL = 0, CPHA = 0
    
    /* [ SCLK = PCLK / (CPSDVSR * (SCR+1)) ] */
    SSPCR0 |= (7 << 8);                        //SCR = 7   
    SSPCPSR = 2;                               //CPSDVSRmin = 2  

    VICVectAddr0 = (uint32) SSPisr;            //Set the SSP ISR vector address
    VICVectCntl0 = 0x00000020 | 11;            //Set channel for SSP
    VICIntEnable |= (1 << 11);                 //Enable the interrupt SSP

    SSPCR1 = (1 << 0) | (1 << 1);              //Loopback Mode & SSP Enabled

    SSPIMSC = 0x06;                            //Enable RTIM, RXIM
}

void ssp_send(uint8 ch)
{
    while ( !(SSPSR & (1 << 1)) );
    SSPDR = ch;
    
    while ( (SSPSR & (1 << 4)) );
}

void SSPisr(void)	__irq
{
    uint8 data;
    uint32 reg;

    reg = SSPMIS;

    if ( (reg & (1 << 1)) || (reg & (1 << 2)) )
    {
        while (SSPSR & (1 << 2))
        {
            data = SSPDR;
            fprintf(stdout, "%c\r\n", data);

            while ( (SSPSR & (1 << 4)) );
        }

        if (reg & (1 << 1)) SSPICR |= (1 << 1);
    }

    VICVectAddr  = 0x00000000;                 //Dummy write to signal end of interrupt
}
