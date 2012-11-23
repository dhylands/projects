

//---------------------------------------------------------------------------
//
// Defines which are specific to the platform used on the MageBitty
//
//---------------------------------------------------------------------------

#define ENABLE_BIT_DEFINITIONS
#include <avr/io.h>
#define CPUCLK = 16000000

// ADC Channels are just numbers (passed to a2d)

enum
{
    EyeRSide            = 0,
    EyeLSide            = 1,
    EyeR                = 2,
    EyeL                = 3,
    LineRightOutside    = 4,
    LineRightInside     = 5,
    LineLeftInside      = 6,
    LineLeftOutside     = 7,
};

// Port B

enum
{
    MotorA_Dir  = 0,    // Brown Wire  - Enable
    MotorA_PWM  = 1,    // Yellow Wire - PWM - OC1A

    MotorB_PWM  = 2,    // White Wire - PWM - OC1B
    MotorB_Dir  = 5,    // Orange Wire - Enable

    GreenLEDPin = 3,
    BlueLEDPin  = 4,

    // 6 & 7 are used for the 16 MHz Oscillator

};

// Port D

enum
{
    UartRxD         = 0,
    UartTxD         = 1,

    LeftRightSwitch = 5,    // Orange
    RunStopSwitch   = 6,    // Yellow

    BlackButton     = 7,

};

static inline void InitLED()
{
    DDRB |= ( ( 1 << BlueLEDPin ) | ( 1 << GreenLEDPin ));
}

static inline void BlueLED( int onOff )
{
    if ( onOff )
    {
        PORTB |= ( 1 << BlueLEDPin );
    }
    else
    {
        PORTB &= ~( 1 << BlueLEDPin );
    }
}

static inline void GreenLED( int onOff )
{
    if ( onOff )
    {
        PORTB |= ( 1 << GreenLEDPin );
    }
    else
    {
        PORTB &= ~( 1 << GreenLEDPin );
    }
}
