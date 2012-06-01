/*
    Energy management

    Battery voltage
    Current through motors

    Adc.h
*/

#ifndef __ADC_H
#define __ADC_H

/* ===================================== */

#define ADC_SHUNT        10        //Shunt conductance (inverse of resistance) [ex. 0.1 ohm => 10]
#define ADC_AMPLIF        1        //Amplification value of shunt voltage (using an operational amplifier)
                                   //If no OPAMP is present in circuit, then this value is 1

#define ADC_REF        3300        //Reference = 3.3V or 3300mV
#define ADC_BAT_DIV       3        //Battery voltage is divided by this value by a resistor divider circuit

#define ADC_LOW_BAT    6000        //Under this value the battery voltage [mV] is considered to be low

#define ADC_CLKDIV        3        //15MHz / (3 + 1) = 3.75 Mhz < 4.5 MHz = max allowed

#define ADC_NR_CHANNELS   3
/*
    Channel 0 - Battery voltage
    Channel 1 - Current through motor 1
    Channel 2 - Current through motor 2
*/

#define ADC_NR_SAMPLES   10

/* ===================================== */

/*
    Initialize ADC0 peripheral. Pins used:
    P0.28 - battery
    P0.29 - motor 1 shunt
    P0.30 - motor 2 shunt
*/
void adc_init(void);

/*
    Start an ADC conversion on the channel
    ch - 0 [battery]
    ch - 1 [motor 1 shunt]
    ch - 2 [motor 2 shunt]
*/
void adc_start(uint8 ch);

/*
    Returns the battery voltage in milivolts
*/
uint32 adc_read_battery(void);

/*
    Returns 1 if battery voltage is lower than ADC_LOW_BAT (mV)
    returns 0 otherwise
*/
uint8 adc_low_battery(void);

/*
    Returns the electric current through motor 1 or 2 in miliamps   
*/
uint32 adc_read_current(uint8 motor);

/*
    Low level sampling buffer related functions
*/
void sample_add(uint8 ch, uint32 sample);
uint32 sample_avg(uint8 ch);
uint32 sample_voltage(uint8 ch);

#endif  // __ADC_H
