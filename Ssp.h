/*
    Spi mode - SSP (Synchronous Serial Port)
    Slave mode

    Ssp.h
*/

#ifndef __SSP_H
#define __SSP_H

#define FIFOSIZE 8

/*
    Initialize SSP communications
*/
void ssp_init(void);

/*
    Send a character through SSP
*/
void ssp_send(uint8 ch);

#endif  // __SSP_H
