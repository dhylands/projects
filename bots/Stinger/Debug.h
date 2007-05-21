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
#define  DEBUG 0
#endif



#if DEBUG
#  define  DBG( fmt, args... )  Log( fmt, ## args )
#else
#  define  DBG( fmt, args... )
#endif

#endif   // DEBUG_H

