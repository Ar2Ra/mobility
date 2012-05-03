//Serial.H

#ifndef __SERIAL_H
#define __SERIAL_H

void init_serial (void);     /* Initialize Serial Interface */
int sendchar (int ch);       /* implementation of putchar (also used by printf function to output data) */
int getkey (void);           /* Read character from Serial Port */

#endif  // __SERIAL_H
