/****************************************************************************
*
*  Sensor.c
*
*  ReadSensors routine.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Phantom.h"
#include "a2d.h"
#include "Puts.h"
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

uns16 gEyeRSide;
uns16 gEyeLSide;

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

    DDRC  = 0;  // All input pins
    PORTC = 0;  // All pullups off

    {
        uns8 pins = ( 1 << LeftRightSwitch )
                  | ( 1 << RunStopSwitch )
                  | ( 1 << BlackButton );

        DDRD &= ~pins;
        PORTB &= ~pins;
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

   gLineLO  = a2d( LineLeftOutside );
   gLineLI  = a2d( LineLeftInside );
   gLineRI  = a2d( LineRightInside );
   gLineRO  = a2d( LineRightOutside );

   // Read the Eye sensors

   gEyeR = a2d( EyeR );
   gEyeL = a2d( EyeL );

   gEyeRSide = a2d( EyeRSide );
   gEyeLSide = a2d( EyeLSide );

   gPortD = PIND;

#if 0
//   puts( "ReadSensors called\n" );
   puts( "LineL: " );
   putu8( gLineL );
   puts( " LineR: " );
   putu8( gLineR );
   puts( " EyeL: " );
   putu8( gEyeL);
   puts( " EyeR: " );
   putu8( gEyeR );
   puts( "\n" );
#endif

} // ReadSensors

void InitLineThresh( void )
{
    ReadSensors();

    gLineAvg  = gLineLO;
    gLineAvg += gLineLI;
    gLineAvg += gLineRI;
    gLineAvg += gLineRO;

    gLineAvg /= 4;

    gLineThresh = gLineAvg / 5;

    puts( "Line Threshold: " );
    putu8( gLineThresh );
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
    return ( PIND & ( 1 << BlackButton )) == 0;
}

int IsDirectionSwitchLeft( void )
{

    return ( PIND & ( 1 << LeftRightSwitch )) == 0;
}

int IsRunStopRun( void )
{
    return ( PIND & ( 1 << RunStopSwitch )) != 0;
}

int IsRunStopStop( void )
{
    return ( PIND & ( 1 << RunStopSwitch )) == 0;
}
