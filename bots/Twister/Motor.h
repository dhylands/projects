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
*   @file   Motor.h
*
*   @brief  Describes the interface to a motor.
*
****************************************************************************/

#if !defined( MOTOR_H )
#define MOTOR_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include <68332/tpu.h>

/**
 * @addtogroup Peripheral
 * @{
 */

//---------------------------------------------------------------------------
/**
*   The Motor class encapsulates the TPU signals required to drive a motor.
*/

class Motor
{
public:

    enum
    {
        StartSpeed  = 0,
        MaxSpeed    = 1000,

        AccelRate   = 20,    // increments per tick
    };

    //------------------------------------------------------------------------
    // Default constructor

    Motor( const char *name );

    //------------------------------------------------------------------------
    // Destructor

    ~Motor();

    //------------------------------------------------------------------------
    // Initialize the TPU pins

    void Init( int d1Pin, int d2pin, int enablePin );

    //------------------------------------------------------------------------
    // Brake - causes the motor to stop quickly.

    void Brake();

    //------------------------------------------------------------------------
    // Coast - causes the motor to coast to a stop.

    void Coast();

    //------------------------------------------------------------------------
    // SetSpeed - Sets the speed +ve = forward, -ve = backward.

    void SetSpeed( int speed );
    int GetSpeed() { return m_speed; }

    //------------------------------------------------------------------------
    // AdjustSpeed - adjusts the current speed and delta speed down

    int AdjustSpeed( int deltaSpeed );

    //------------------------------------------------------------------------
    // SetFrequency - Sets the frequency used by the motor.

    void     SetFrequency( unsigned freq );
    unsigned GetFrequency() { return m_freq; }

    //------------------------------------------------------------------------
    // ProcessTick - does processing to adjust speed. Should be called
    // once each tick.

    void ProcessTick();

private:


    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    Motor( const Motor & copy );
    Motor &operator =( const Motor &rhs );

    //------------------------------------------------------------------------

public:
    const char  *m_name;
    TPUChannel  *m_d1;
    TPUChannel  *m_d2;
    TPUChannel  *m_enable;
    uint16       m_pulseWidth;  // in TCR1 ticks
    unsigned     m_freq;
    int          m_speed;
    int          m_desiredSpeed;

};

// ---- Inline Functions ----------------------------------------------------

/** @} */

#endif // MOTOR_H

