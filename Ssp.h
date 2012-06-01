/*
    Spi mode - SSP (Synchronous Serial Port)

    Ssp.h
*/

#ifndef __SSP_H
#define __SSP_H

#define FIFOSIZE 8

/*
    SSP should be in slave mode, but for testing purposes it is configured
    in master loopback mode.
*/

/*
    Initialize SSP communications
*/
void ssp_init(void);

/*
    Send a character through SSP
*/
void ssp_send(uint8 ch);

#endif  // __SSP_H
