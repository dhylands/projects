#include <avr/io.h>
/*
    ARC 1.0 Lib
*/

// Tuned for reasonably accurate delays w/o interrupts @ 8mhz (likely to change with compiler rev).

void waitms(int delay)
{
    while (delay)
    {
        delay--;
        int i;
        for (i = 1597; i; i--) asm("nop");
    }
}

// Simple polled ADC input

void initialize_adc(void)
{
    ADCSR = BV(ADEN) | BV(ADSC) | BV(ADPS1) | BV(ADPS2);
    while (ADCSR & BV(ADSC));
}

int getadc(char Channel)
{
    ADMUX = Channel;
    ADCSR = ADCSR | BV(ADSC);
    while (ADCSR & BV(ADSC));        // Wait conversion done
    return ADCW;
}

void initialize_pwm(void)
{
}
