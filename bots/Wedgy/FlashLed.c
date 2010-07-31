#include "arc_10.h"
#include "a2d.h"
#include "DebugKey.h"
#include "Delay.h"
#include "Motor.h"
#include "Sensor.h"
#include "Uart.h"

int main(void)
{
    int i, j;

    InitializeA2D();
    UART_Init();
    UART_PutStr( "FlashLED Test Program\r\n" );
    InitTimer();
    InitMotors();

    DDRB = BV( prog_led ) | BV( 6 );
    
    while( 1 )
    {
        // Turn LED on
        
        ReadSensors();
        PORTB &= ~( BV( prog_led ) | BV( 6 ));

        for ( i = 0; i < 10; i++ ) 
        {
            if ( UART_IsCharAvailable() )
            {
                UART_GetChar();
                DebugKey();
            }
            for ( j = 0; j < 50; j++ ) 
            {
                WaitForTimer0Rollover();
            }
        }

        // Turn LED off

        ReadSensors();
        PORTB |= ( BV( prog_led ) | BV( 6 ));

        for ( i = 0; i < 10; i++ ) 
        {
            if ( UART_IsCharAvailable() )
            {
                UART_GetChar();
                DebugKey();
            }
            for ( j = 0; j < 50; j++ ) 
            {
                WaitForTimer0Rollover();
            }
        }

    }
    return 0;
}
