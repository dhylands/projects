#include "arc_10.h"

int main(void)
{
    outp(BV(prog_led), DDRB);   // Make LED port bit an output
    
    while(1)
    {
        sbi(PORTB, 4);          // Turn LED off
        waitms(500);
        cbi(PORTB, 4);          // Turn LED on
        waitms(500);
    }
    return 0;
}
      