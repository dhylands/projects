/****************************************************************************
*
*  Sensor.c
*
*  ReadSensors routine.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "a2d.h"
#include "Puts.h"
#include "Sensor.h"

// ---- Private Constants and Types -----------------------------------------

// ---- Private Variables ---------------------------------------------------

uns8  gLineL;
uns8  gLineR;

uns16 gEyeL;
uns16 gEyeR;

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/***************************************************************************
*
*  ReadSensors
*
*  Reads the various sensors
*/

extern void ReadSensors( void )
{  
   // Read the line sensors

   gLineR  = a2d( LINE_R );
   gLineL  = a2d( LINE_L );

   // Read the Eye sensors

   gEyeR = a2d( EYE_R );
   gEyeL = a2d( EYE_L );

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

