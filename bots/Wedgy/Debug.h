/****************************************************************************
*
*  Debug.h
*
*  Debug control
*
****************************************************************************/

#if !defined( DEBUG_H )
#define  DEBUG_H

#if !defined( DEBUG )
#define  DEBUG 1
#endif

#if DEBUG
#  define  DBG(x)      x
#else
#  define  DBG(x)
#endif

#endif   // DEBUG_H

