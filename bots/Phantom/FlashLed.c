#include "Phantom.h"
#include "a2d.h"
#include "DebugKey.h"
#include "Delay.h"
#include "Motor.h"
#include "Sensor.h"
#include "Uart.h"

uns16 gHuntCounter;

int main(void)
{
    int i, j;

    UART_Init();
    InitializeA2D();
    InitSensors();
    
    UART_PutStr( "FlashLED Test Program\r\n" );
    InitTimer();
    InitMotors();

    InitLED();

    TWCR = 0;

    while( 1 )
    {
        // Turn LED on
        
        ReadSensors();


        BlueLED( 1 );
        GreenLED( 0 );

        // Tick rate is 100/sec so waiting for 5 * 10 = 50 waits for 1/2 sec

        for ( i = 0; i < 10; i++ ) 
        {
            if ( UART_IsCharAvailable() )
            {
                UART_GetChar();
                DebugKey();
            }
            for ( j = 0; j < 5; j++ ) 
            {
                WaitForTimer0Rollover();
            }
        }

        // Turn LED off

        ReadSensors();

        BlueLED( 0 );
        GreenLED( 1 );

        for ( i = 0; i < 10; i++ ) 
        {
            if ( UART_IsCharAvailable() )
            {
                UART_GetChar();
                DebugKey();
            }
            for ( j = 0; j < 5; j++ ) 
            {
                WaitForTimer0Rollover();
            }
        }

    }
    return 0;
}
