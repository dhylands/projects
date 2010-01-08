/****************************************************************************
*
*   Copyright (c) 1993-2007 Dave Hylands     <dhylands@gmail.com>
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
*   @file   stm.h
*
*   @brief  This file contains the declarations for the State Machine Engine. 
*
*   The state machine engine takes a state machine, generated from smc,
*	and executes it.
*
****************************************************************************/

#if !defined( STM_H )
#define STM_H               /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

#include <stdint.h>

/* ---- Constants and Types ---------------------------------------------- */

/*
 * We declare STM_Input as an Int so that it is compatible with an enum,
 * and so we can return negative values to indicate no further inputs.
 */

struct	STM_StateMachine;
typedef struct STM_StateMachine STM_StateMachine;

typedef int		STM_Input;
typedef int		STM_State;

typedef	void	(*STM_Action)( void );
typedef	void	(*STM_ActionProc)( STM_StateMachine *sm, int actionIdx );

#define	STM_INPUT_NONE	( (STM_Input) -1 )
#define	STM_STATE_NONE	( (STM_State) -1 )
#define	STM_ACTION_NONE	( (STM_Action) NULL )

#define	STM_MAGIC		0x53746D20uL	/* 'Stm '							*/

struct STM_StateMachine
{
	/*
	 * These fields are initialized by the automatically generated code found
	 * in the -sm.c file.
	 */

	int			numActions;		/* Num actions for this state machine		*/
	int			numInputs;		/* Num inputs for this state machine		*/
	int			numStates;		/* Num states for this state machine		*/

	int			currState;		/* Current state							*/

	STM_Input  *queue;			/* Ptr to input queue						*/
	int			queueLen;		/* Num entries currently in input queue		*/
	int			queuePutIdx;	/* Place to put next input in the queue		*/
	int			queueGetIdx;	/* Place to get next input from the queue	*/
	int			queueMax;		/* Max entries in input queue				*/

    int			stateBytes;		/* Width of state entry in bytes			*/
    uint8_t    *stateData;      /* Ptr to the state data                    */
	STM_Action *action;			/* Ptr to array of action routines			*/
	STM_ActionProc	actionProc;	/* Central action dispatch routine			*/
	void	   *userParm;		/* User defined parameter					*/

#if defined( STM_DEBUG )
	unsigned	magic;			/* Magic number for sanity					*/

	char	   *name;			/* ASCII representation of state mach		*/
	char	  **actionStr;		/* ASCII representation of actions			*/
	char	  **inputStr;		/* ASCII representation of inputs			*/
	char	  **stateStr;		/* ASCII representation of statess			*/

	int 		dbgActions;		/* Should we trace action executions?		*/
	int 		dbgInputs;		/* Should we trace inputs processed?		*/
	int 		dbgStates;		/* Should we trace state changes?			*/
#endif

	/* 
	 * The following variables are initialized dynamically.
	 */

	STM_Input	currInput;		/* Input currently being processed.			*/
#if defined( STM_DEBUG )
	char	   *currInputStr;	/* ASCII representation of `currInput`.		*/
#endif
};

// The state data is arranged as an array of bytes. Each state occupies the
// same number of bytes, namely sm->entryBytes.
// 
// The first two bytes are the index of the enter and exit action, and
// this is followed by (action index, next state index) tuples.

#define STM_BytesPerState( sm )         ( (sm)->stateBytes )
#define STM_CalcBytesPerState( sm )     (( (sm)->numInputs * 2 ) + 2 )

#define	STM_EnterOffset()				0
#define	STM_ExitOffset()				1
#define	STM_ActionOffset( input )		(( (input) * 2 ) + 2 )
#define	STM_NextStateOffset( input )	(( (input) * 2 ) + 3 )

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

#define	STM_GetUserParm( sm )		( (sm)->userParm )
#define	STM_GetCurrState( sm )		( (sm)->currState )
#define	STM_GetCurrInput( sm )		( (sm)->currInput )
#define	STM_GetCurrInputStr( sm )	( (sm)->currInputStr )

void	STM_AddInput( STM_StateMachine *sm, STM_Input input );
int 	STM_Execute( STM_StateMachine *sm, STM_StateMachine **currSm );
int 	STM_Execute1( STM_StateMachine *sm, STM_StateMachine **currSm );
void	STM_SetUserParm( STM_StateMachine *sm, void *userParm );
void	STM_SetDebug( STM_StateMachine *sm, 
                      int dbgInputs,
					  int dbgActions, 
                      int dbgStates );
char   *STM_CurrStateStr( STM_StateMachine *sm );

/** @} */

#endif /* STM_H */

