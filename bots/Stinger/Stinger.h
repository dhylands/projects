//---------------------------------------------------------------------------
//
// Defines which are specific to the platform used on the MageBitty
//
//---------------------------------------------------------------------------

#define ENABLE_BIT_DEFINITIONS
#include <avr/io.h>
#define CPUCLK = 16000000

#if !defined( TYPES_H )
#   include "Types.h"
#endif

// ADC Channels are just numbers (passed to a2d)

enum
{
    LineLeftInside      = 2,
    LineLeftOutside     = 3,
    LineRightOutside    = 4,
    LineRightInside     = 5,

    EyeL                = 6,
    EyeR                = 7,

};

// Port B

enum
{
    MotorA_Dir  = 0,    // Yellow Wire  - D1
    MotorA_PWM  = 1,    // White Wire   - D2

    MotorB_PWM  = 2,    // Blue Wire    - D1
    MotorB_Dir  = 5,    // Orange Wire  - D2

    GreenLEDPin = 3,
    BlueLEDPin  = 4,

    // 6 & 7 are used for the 16 MHz Oscillator

};

#define GreenLEDMask    (uint8_t)( 1 << GreenLEDPin )
#define BlueLEDMask     (uint8_t)( 1 << BlueLEDPin )

// Port D

enum
{
    UartRxD         = 0,
    UartTxD         = 1,

    PgmSelect1      = 2,    // Blue
    PgmSelect2      = 3,    // White
    PgmSelect4      = 4,    // Yellow

    LeftEyeEnable   = 5,    
    RightEyeEnable  = 6,

    BlackButton     = 7,

};

static inline void InitLED( void )
{
    DDRB |= BlueLEDMask | GreenLEDMask;
}

static inline void BlueLED( int onOff )
{
    if ( onOff )
    {
        PORTB |= BlueLEDMask;
    }
    else
    {
        PORTB &= ~BlueLEDMask;
    }
}

static inline void GreenLED( int onOff )
{
    if ( onOff )
    {
        PORTB |= GreenLEDMask;
    }
    else
    {
        PORTB &= ~GreenLEDMask;
    }
}
