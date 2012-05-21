/*
    Energy management

    Battery voltage
    Current through motors

    Adc.h
*/

#ifndef __ADC_H
#define __ADC_H

void adc_init(void);
void adc_start(void);

void sample_add(uint8 ch, uint32 sample);
uint32 sample_avg(uint8 ch);
uint32 sample_voltage(uint8 ch);

#endif  // __ADC_H
