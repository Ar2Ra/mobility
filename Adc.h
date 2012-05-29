/*
    Energy management

    Battery voltage
    Current through motors

    Adc.h
*/

#ifndef __ADC_H
#define __ADC_H

#define ADC_REF        3300        //Reference = 3.3V or 3300mV
#define ADC_BAT_DIV       3        //Battery voltage is divided by this value by a resistor divider circuit

#define ADC_SEL        0x02        //Select channel AD0.1
#define ADC_CLKDIV        3        //15MHz / (3 + 1) = 3.75 Mhz < 4.5 MHz = max allowed

#define ADC_NR_CHANNELS   1
/*
    Channel 0 - Battery voltage
    Channel 1 - Current through motor 1
    Channel 2 - Current through motor 2
*/

#define ADC_NR_SAMPLES   10

void adc_init(void);
void adc_start(void);

uint32 adc_read_battery(void);

void sample_add(uint8 ch, uint32 sample);
uint32 sample_avg(uint8 ch);
uint32 sample_voltage(uint8 ch);

#endif  // __ADC_H
