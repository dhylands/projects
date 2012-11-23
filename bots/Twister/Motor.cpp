/****************************************************************************
*
*     Copyright (c) 2003 Dave Hylands
*           All Rights Reserved
*
*   Permission is granted to any individual or institution to use, copy, or
*   redistribute this software so long as it is not sold for profit, and that
*   this copyright notice is retained.
*
****************************************************************************/
/**
*
*   @file   Motor.cpp
*
*   @brief  Implementation of the Motor class
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <stdio.h>
#include <assert.h>

#include "Log.h"
#include "Motor.h"

#include <68332/tpu.h>
#include <68332/qomdo.h>
#include <68332/qompwm.h>

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

// With TCR1 running at 6,291,456 Hz, a 15 bit counter can get this down
// to 192 Hz or 5.2 msec.

#if !defined( MOTOR_DEFAULT_PWM_FREQ )
#define MOTOR_DEFAULT_PWM_FREQ  256
#endif

// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------

#define MOTOR_DEBUG_ENABLED 1
#if MOTOR_DEBUG_ENABLED
#   define MOTOR_DEBUG(x)  LOG(x)
#else
#   define MOTOR_DEBUG(x)
#endif

#define MOTOR_DEBUG_TICK_ENABLED  ( MOTOR_DEBUG_ENABLED && 0 )


// ---- Functions -----------------------------------------------------------

// En D1 D2
//  H  H  H - Motor Brake
//  H  L  L - Motor Brake
//  H  H  L - Direction 1
//  H  L  H - Direction 2 
//  L  x  x - Motor Coasts

/**
 * @addtogroup Peripheral
 * @{
 */

//***************************************************************************
/**
*   Constructor
*/

Motor::Motor( const char *name )
{
    m_name = name;
    m_d1 = NULL;
    m_d2 = NULL;
    m_enable = NULL;
    m_pulseWidth = 0;
    m_speed = 0;
    m_desiredSpeed = 0;
}

//***************************************************************************
/**
*   Destructor
*/

Motor::~Motor()
{
    if ( m_d1 != NULL )
    {
        tpu_release_channel( 0, m_d1->channel_num );
    }
    if ( m_d2 != NULL )
    {
        tpu_release_channel( 0, m_d2->channel_num );
    }
    if ( m_enable != NULL )
    {
        tpu_release_channel( 0, m_enable->channel_num );
    }
}
    
//***************************************************************************
/**
*  AdjustSpeed - Adjusts the current speed and desired speed immediately
*                (i.e. doesn't wait for acceleration).
*
* Returns the amount of deltaSpeed that couldn't be adjusted for. This can
* be negated and applied to the other wheel.
*/

int Motor::AdjustSpeed( int deltaSpeed )
{
    int deltaSpeedUsed = deltaSpeed;

    if (( m_desiredSpeed + deltaSpeed  ) > MaxSpeed )
    {
        // Don't go faster forward than our max speed.

        deltaSpeedUsed = MaxSpeed - m_desiredSpeed;
    }
    else
    if (( m_desiredSpeed + deltaSpeed ) < -MaxSpeed )
    {
        // Don't go faster backwards than out max speed

        deltaSpeedUsed = -MaxSpeed - m_desiredSpeed;
    }
    else
    if (( m_desiredSpeed > 0 ) && (( m_desiredSpeed + deltaSpeed ) < StartSpeed ))
    {
        // Don't slow down lower than StartSpeed

        deltaSpeedUsed = StartSpeed - m_desiredSpeed;
    }
    else
    if (( m_desiredSpeed < 0 ) && (( m_desiredSpeed + deltaSpeed  ) > -StartSpeed ))
    {
        deltaSpeedUsed = -StartSpeed - m_desiredSpeed;
    }

    MOTOR_DEBUG(( "%s Adj%4d used%4d speed%4d->%4d desired%4d->%4d\n",
                  m_name, deltaSpeed, deltaSpeedUsed,
                  m_speed, m_speed + deltaSpeedUsed,
                  m_desiredSpeed, m_desiredSpeed + deltaSpeedUsed ));

    m_speed += deltaSpeedUsed;
    m_desiredSpeed += deltaSpeedUsed;

    return deltaSpeed - deltaSpeedUsed;
}

//***************************************************************************
/**
*  Init - Initialize the TPU pins
*/

void Motor::Init( int d1Pin, int d2Pin, int enablePin )
{
    if (( m_d1 = tpu_request_channel( 0, d1Pin )) == NULL )
    {
        LOG_ERROR(( "Unable to get TPU pin %d for %s Motor D1", d1Pin, m_name ));
        return;
    }
    if (( m_d2 = tpu_request_channel( 0, d2Pin )) == NULL )
    {
        LOG_ERROR(( "Unable to get TPU pin %d for %s Motor D2", d2Pin, m_name ));
        return;
    }
    if (( m_enable = tpu_request_channel( 0, enablePin )) == NULL )
    {
        LOG_ERROR(( "Unable to get TPU pin %d for %s Motor Enable", enablePin, m_name ));
        return;
    }
    SetSpeed( 0 );
    SetFrequency( MOTOR_DEFAULT_PWM_FREQ );

    Coast();

} // Init

//***************************************************************************
/**
*  Brake - causes the motor to stop quickly.
*/

void Motor::Brake()
{
    MOTOR_DEBUG(( "%s Motor: Brake\n", m_name ));

    qomdo( m_d1, false );
    qomdo( m_d2, false );
    qomdo( m_enable, true );

    m_speed = 0;
    m_desiredSpeed = 0;

} // Brake

//***************************************************************************
/**
*  Coast - causes the motor to coast to a stop.
*/

void Motor::Coast()
{
    MOTOR_DEBUG(( "%s Motor: Coast\n", m_name ));

    qomdo( m_enable, false );
    qomdo( m_d1, false );
    qomdo( m_d2, false );

    m_speed = 0;
    m_desiredSpeed = 0;

} // Coast

//***************************************************************************
/**
*   ProcessTick - does processing to adjust speed. Should be called
*   once each tick.
*/

void Motor::ProcessTick()
{
    // First of check to see if really have to do anything.

    if ( m_speed == m_desiredSpeed )
    {
        return;
    }

#if MOTOR_DEBUG_TICK_ENABLED
    bool report = false;
    static int counter = 0;
    if ( ++counter >= 100 )
    {
        report = true;
        MOTOR_DEBUG(( "ProcessTick: m_speed = %d, m_desiredSpeed = %d\n", m_speed, m_desiredSpeed ));
        counter = 0;
    }
#endif

#if 1
    m_speed = m_desiredSpeed;
    int speed       = m_speed;
#else
    int desired     = m_desiredSpeed;

    // First of all, subtract out StartSpeed to make the calculations easier

    if ( speed > 0 )
    {
        if ( speed > StartSpeed )
        {
            speed -= StartSpeed;
        }
        else
        {
            speed = 0;
        }
    }
    else
    if ( speed < 0 )
    {
        if ( speed < StartSpeed )
        {
            speed += StartSpeed;
        }
        else
        {
            speed = 0;
        }
    }
    if ( desired > 0 )
    {
        if ( desired > StartSpeed )
        {
            desired -= StartSpeed;
        }
        else
        {
            desired = 0;
        }
    }
    else
    if ( desired < 0 )
    {
        if ( desired < StartSpeed )
        {
            desired += StartSpeed;
        }
        else
        {
            desired = 0;
        }
    }

    // Now move speed towards desiredSpeed. Note that if m_speed == 0 and
    // m_desired == StartSpeed, then speed == desired.

    if ( speed < desired )
    {
        speed += AccelRate;
        if ( speed > desired )
        {
            speed = desired;
        }
    }
    else
    if ( speed > desired )
    {
        speed -= AccelRate;
        if ( speed < desired )
        {
            speed = desired;
        }
    }

    // Add StartSpeed back in.

    if (( speed > 0 ) || (( speed == 0 ) && ( m_desiredSpeed > 0 )))
    {
        speed += StartSpeed;
    }
    else
    if (( speed < 0 ) || (( speed == 0 ) && ( m_desiredSpeed < 0 )))
    {
        speed -= StartSpeed;
    }
    m_speed = speed;
#endif

    // Now change the motor speed

    if ( speed > 0 )
    {
        qomdo( m_d1, true );
        qomdo( m_d2, false );
    }
    else
    {
        qomdo( m_d1, false );
        qomdo( m_d2, true );

        speed = -speed;
    }

    uint16 hiTime = (uint32)m_pulseWidth * (uint32)speed / MaxSpeed;
    uint16 loTime = m_pulseWidth - hiTime;

#if MOTOR_DEBUG_TICK_ENABLED
    if ( report )
    {
        MOTOR_DEBUG(( "%s Motor: P( %d ) hiTime: %hu loTime: %hu\n", m_name, speed, hiTime, loTime ));
    }
#endif

    qompwm( m_enable, hiTime, loTime );

} // ProcessTick

//***************************************************************************
/**
*   SetFrequency - Sets the PWM frequency used.
*
*   @param   freq     (in)  Frequency to use.
*/

void Motor::SetFrequency( unsigned freq )
{
    MOTOR_DEBUG(( "%s Motor: SetFrequency( %u )\n", m_name, freq ));

    m_freq = freq;

    m_pulseWidth = tpu_get_tcr1_freq( 0 ) / m_freq;

    // Make sure that the pulse width will work with the TPU

    assert( m_pulseWidth < 0x8000 );

} // SetFrequency

//***************************************************************************
/**
*   SetSpeed - Sets the speed +ve = forward, -ve = backward.
*
*   @param   speed     (in)  Speed in the range: -MaxSpeed to +MaxSpeed
*/

void Motor::SetSpeed( int speed )
{
#if MOTOR_DEBUG_ENABLED
    int orgSpeed = speed;
#endif

    if (( speed > 0 ) && ( speed < StartSpeed ))
    {
        speed = StartSpeed;
    }
    else
    if ( speed < 0 && ( speed > -StartSpeed ))
    {
        speed = -StartSpeed;
    }
    m_desiredSpeed = speed;

#if MOTOR_DEBUG_ENABLED
    if ( speed == orgSpeed )
    {
        MOTOR_DEBUG(( "%s Motor: SetSpeed( %d ) currSpeed = %d\n", 
                      m_name, speed, m_speed ));
    }
    else
    {
        MOTOR_DEBUG(( "%s Motor: SetSpeed( %d ) clamped to %d currSpeed = %d\n", 
                      m_name, orgSpeed, speed, m_speed ));
    }
#endif

} // SetSpeed

/** @} */

