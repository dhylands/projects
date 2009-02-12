/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*   See README and COPYING for more details.
*
****************************************************************************/
/**
*
*   @file    QD.h
*
*   @brief   Performs quadrature decoding.
*
*   This file uses macros and expects a certain naming convention to be 
*   followed.
*
*   If you call QD_Update( Left ) then the following #defines should exist:
*
*   #define Left_A_PIN  PINC
*   #define Left_A_BIT  0
*   #define Left_B_PIN  PINC
*   #define Left_B_BIT  1
*
*   QD_Counter_t    Left_Count;
*   QD_State_t      Left_PrevState;
*
*   Use QD_Update( Left ) periodically to update the encoder, and use
*   QD_Count( Left ) to retrieve the counter value.
*
*   Note that the type of the _Count variable can be set by setting the
*   CFG_QD_COUNTER_TYPE to be the desired type.
*
*   #define CFG_QD_COUNTER_TYPE     uint16_t
*
*   NOTE: If you plan on calling QD_Update from within an ISR, then the
*         _Count variable should be declared volatile.
*
*****************************************************************************/

#if !defined( QD_H )
#define QD_H

/* ---- Include Files ----------------------------------------------------- */

#include <inttypes.h>
#include <avr/interrupt.h>

/* ---- Constants and Types ----------------------------------------------- */

typedef uint8_t     QD_State_t;

typedef struct
{
    int8_t  increment[ 4 ];

} QD_Increment_t;

#if defined( CFG_QD_COUNTER_TYPE )

typedef CFG_QD_COUNTER_TYPE QD_Counter_t;

#else

typedef uint16_t    QD_Counter_t;

#endif


/* ---- Variable Externs -------------------------------------------------- */

extern QD_Increment_t   QD_gIncrement[ 4 ];

/* ---- Function Prototypes ----------------------------------------------- */

#define QD_Update( qd ) \
    do \
    { \
        uint8_t state = (((((qd ## _A_PIN) >> (qd ## _A_BIT)) & 1 ) << 1 )  \
                      |   (((qd ## _B_PIN) >> (qd ## _B_BIT)) & 1 ));       \
                                                                            \
        (qd ## _Count) += QD_gIncrement[ (qd ## _PrevState) ].increment[ state ]; \
        (qd ## _PrevState) = state;   \
    } while ( 0 )

// Since the count could be a multi-byte and may be updated from within an ISR
// we need to disable interrupts around the retrieval.

static inline QD_Counter_t QD_CountInternal( volatile QD_Counter_t *qd )
{   
    if ( sizeof( QD_Counter_t ) == 1 )
    {
        return *qd;
    }
    else
    {
        QD_Counter_t    counter;
        uint8_t         sreg;

        sreg = SREG;
        cli();
        counter = *qd;
        SREG = sreg;

        return counter;
    }
}

static inline void QD_SetCountInternal( volatile QD_Counter_t *qd, QD_Counter_t counter )
{   
    if ( sizeof( QD_Counter_t ) == 1 )
    {
        *qd = counter;
    }
    else
    {
        uint8_t         sreg;

        sreg = SREG;
        cli();
        *qd = counter;
        SREG = sreg;
    }
}

#define QD_Count( qd )              QD_CountInternal( &(qd ## _Count) )

#define QD_SetCount( qd, count )    QD_SetCountInternal( &(qd ## _Count), (count))


#endif  // QD_H

