/****************************************************************************
*
*  Sensor.c
*
*  ReadSensors routine.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Stinger.h"
#include "adc.h"
#include "Debug.h"
#include "Delay.h"
#include "Log.h"
#include "Sensor.h"

// ---- Private Constants and Types -----------------------------------------

// ---- Private Variables ---------------------------------------------------

uns8  gLineLO;
uns8  gLineLI;
uns8  gLineRO;
uns8  gLineRI;

uns16 gLineAvg;
uns8  gLineThresh;

uns16 gEyeL;
uns16 gEyeR;

uns8  gPortD;

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/***************************************************************************
*
*  InitSensors
*
*  Configures the pins
*/

void InitSensors( void )
{
    TWCR = 0;   // Force TWI off (AD4 & 5 share the TWI pins)

    // For DDRx pins, 1 = output, 0 = input.
    // For input pins, PORTx = 0 = pullup disabled
    //                 PORTx = 1 = pullup enabled 

    DDRC  = 0;  // All input pins
    PORTC = 0;  // All pullups off

    {
        // Setup the input pins on Port D

        uns8 pins = ( 1 <<  PgmSelect1  )
                  | ( 1 <<  PgmSelect2  )
                  | ( 1 <<  PgmSelect4  )
                  | ( 1 <<  BlackButton  );

        ClearBits( DDRD, pins );    // Configure inputs
        ClearBits( PORTD, pins );   // Disable pullups
    }
    ADC_Init( ADC_PRESCALAR_AUTO );

    // Delay for a short time to allow the ADC to initialize

    ms_spin( 1 );
}

uns8 PgmSelect( void )
{
    uns8    portD = PIND;

    return ( portD >> PgmSelect1 ) & 0x07;
}

void EnableLeftEye( int enable )
{
    if ( enable )
    {
        SetBits( PORTD, ( 1 <<  LeftEyeEnable ) );
    }
    else
    {
        ClearBits( PORTD, ( 1 <<  LeftEyeEnable ) );
    }
}

void EnableRightEye( int enable )
{
    if ( enable )
    {
        SetBits( PORTD, ( 1 <<  RightEyeEnable ) );
    }
    else
    {
        ClearBits( PORTD, ( 1 <<  RightEyeEnable ) );
    }
}


/***************************************************************************
*
*  ReadSensors
*
*  Reads the various sensors
*/

void ReadSensors( void )
{  
   // Read the line sensors

   gLineLO  = ADC_Read( LineLeftOutside );
   gLineLI  = ADC_Read( LineLeftInside );
   gLineRI  = ADC_Read( LineRightInside );
   gLineRO  = ADC_Read( LineRightOutside );

   // Read the Eye sensors

   gEyeR = ADC_Read( EyeR );
   gEyeL = ADC_Read( EyeL );

   gPortD = PIND;

} // ReadSensors

void InitLineThresh( void )
{
    ReadSensors();

    gLineAvg  = gLineLO;
    gLineAvg += gLineLI;
    gLineAvg += gLineRI;
    gLineAvg += gLineRO;

    gLineAvg /= 4;

    gLineThresh = gLineAvg / 3;

    DBG( "Line Threshold: %3u\n", gLineThresh );
}

int EyeDetectedLeft( void )
{
    return gEyeL < 250;
}

int EyeDetectedRight( void )
{
    return gEyeR < 250;
}

int LineDetectedLeft( void )
{
    return ( gLineLO < gLineThresh )
        || ( gLineLI < gLineThresh );
}

int LineDetectedRight( void )
{
    return ( gLineRO < gLineThresh )
        || ( gLineRI < gLineThresh );
}

int IsBlackButtonPressed( void )
{
    return ( PIND & ( 1 <<  BlackButton ) ) == 0;
}

