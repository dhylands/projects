/****************************************************************************
*
*  RCPulse.h
*
****************************************************************************/

#if !defined( RCPULSE_H )
#define RCPULSE_H

#if !defined( TYPES_H )
#   include "Types.h"
#endif

typedef struct
{
    // The order/size of the first 3 fields MUST NOT BE CHANGED without
    // a corresponding change in RCPulseIrq.S

    uns8    start;
    uns8    end;
    uns8    done;

    uns8    width;

} RCPulse;

extern  RCPulse RC0;
extern  RCPulse RC1;

#endif  // RCPULSE_H

