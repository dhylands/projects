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
*   @file   stm.c 
*
*   @brief  This file contains the functions which implements the
*           State Machine Engine.
*
*   The State Machine Engine takes a state machine, generated from the
*   state machine compiler (smc) and executes it.
*
*	Currently these routines can only handle one instance of a state
*	machine, since the current state and input queue are stored in the
*	state machine object itself.
*
*	If the need for instanced state machines arises, then the current state
*	and input information should be split into one data structure, with all
*	of the static information which would be shared amongst multiple
*	instances stored in another data structure.
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <Log.h>

#include "stm.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

#if defined( STM_DEBUG )

#define	DBG_ACTION( fmt, args... )	do { if ( sm->dbgActions ) Log( fmt, ##args ); } while (0)
#define	DBG_INPUT( fmt, args... )	do { if ( sm->dbgInputs )  Log( fmt, ##args ); } while (0)
#define	DBG_STATE( fmt, args... )	do { if ( sm->dbgStates )  Log( fmt, ##args ); } while (0)

#else

#define	DBG_ACTION( fmt, args... )
#define	DBG_INPUT( fmt, args... )
#define	DBG_STATE( fmt, args... )

#endif

#if !defined( TRUE )
#   define  TRUE    1
#endif

#if !defined( FALSE )
#   define  FALSE   0
#endif

/* ---- Functions --------------------------------------------------------- */

/**
 * @addtogroup StateMachineCompiler
 * @{
 */

//***************************************************************************
/**
*   Description goes here
*/

/** @} */

static	void		ExecuteAction( STM_StateMachine *sm, int actionIdx,
                                   const char *actionStr );
static	STM_Input	GetInput( STM_StateMachine *sm );

/******************* Functions **********************************************/

//***************************************************************************
/**
*	Adds `input` onto the end of input queue for state 
*	machine `sm`. The input will eventually be processed via
*	STM_Execute or STM_Execute1.
*/

void STM_AddInput( STM_StateMachine *sm, STM_Input input )
{
	ASSERT( sm != NULL );
#if defined( STM_DEBUG )
	ASSERT( sm->magic == STM_MAGIC );
#endif
	ASSERT(( input >= STM_INPUT_NONE ) && ( input < sm->numInputs ));
	ASSERT( sm->queueLen < sm->queueMax );
	
	if ( input != STM_INPUT_NONE )
	{
		if ( sm->queueLen < sm->queueMax )
		{
			sm->queueLen++;
			sm->queue[ sm->queuePutIdx++ ] = input;
			if ( sm->queuePutIdx >= sm->queueMax )
			{
				sm->queuePutIdx = 0;
			}
		}
#if defined( STM_DEBUG )
		else
		{
            LogError( "SM %s: queue 0x%06lx full, get=%d, len=%d\n",
					  sm->name, sm->queue, sm->queueGetIdx, sm->queueMax );
		}
#endif
	}

} // STM_AddInput

//***************************************************************************
/**
*	Executes the state machine given by `sm` until no further
*	inputs are available on the input queue.
*
*	The `currSm` parameter is the address of a task-specific global variable
*	which will track the currently executing state machine. This is intended
*	to be used for those actions which need to generate furtherinput, or to
*	access the user data associated with a state machine. Since most actions
*	do not require these accesses, it was decided to make a global variable
*	available, rather than requiring a parameter to an action routine that
*	would almost never be used.
*
*	It is acceptable to pass NULL for `currSm`.
*
*   @returns	TRUE if something was executed, FALSE if nothing was done.
*/

int STM_Execute( STM_StateMachine *sm, STM_StateMachine **currSm )
{
	int	executed = FALSE;

	ASSERT( sm != NULL );
	ASSERT( sm->magic == STM_MAGIC );

	while ( STM_Execute1( sm, currSm ))
	{
		executed = TRUE;
	}
	return executed;

} // STM_Execute

//***************************************************************************
/**
*	Executes a single input, if one is available, for the
*	state machine given by `sm`.
*
*	This function takes care of calling the enter, exit and transition
*	actions associated with changing states, as well as providing debug
*	information.
*
*	The `currSm` parameter is the address of a task-specific global variable
*	which will track the currently executing state machine. This is intended
*	to be used for those actions which need to generate furtherinput, or to
*	access the user data associated with a state machine. Since most actions
*	do not require these accesses, it was decided to make a global variable
*	available, rather than requiring a parameter to an action routine that
*	would almost never be used.
*
*	It is acceptable to pass NULL for `currSm`.
*
*	The return code indicates whether an input was processed or not.
*	
*   @returns	TRUE if an input was processed, FALSE otherwise.
*   
*/

int STM_Execute1( STM_StateMachine *sm, STM_StateMachine **currSm )
{
	int			action;			/* Action to execute.						*/
	STM_Input	input;			/* Input being processed.					*/
	int			nextState;		/* Next state to change to.					*/
	uint8_t    *stateData;  	/* Ptr to state data for current state.		*/
	STM_StateMachine   *svSm;	/* Saved value of STM_CurrStateMachine		*/

	ASSERT( sm != NULL );
	ASSERT( sm->magic == STM_MAGIC );

	if (( input = GetInput( sm )) == STM_INPUT_NONE )
	{
		/*
		 * No input, indicate that we didn't do anything
		 */

		return FALSE;
	}

	/* 
	 * Set the global state machine variable, so that actions which use it
	 * can access this state machine. Save the old value off so that action
	 * routines can execute other state machines.
	 */

	if ( currSm != NULL )
	{
		svSm = *currSm;
		*currSm = sm;
	}

	/*
	 * We've got an input, run it through the state machine
	 */

	DBG_INPUT( "STM %s: processing input %s (%d)\n",
				sm->name, sm->currInputStr, sm->currInput );

	stateData = &sm->stateData[ sm->currState * STM_BytesPerState( sm )];

	nextState = stateData[ STM_NextStateOffset( input )];

	/*
	 * Exit the old state
	 */

	if ( nextState != sm->currState )
	{
		int		exitAction;

		/*
		 * We're leaving the old state, call the exit routine if one is
		 * defined
		 */

		exitAction = stateData[ STM_ExitOffset() ];
		ExecuteAction( sm, exitAction, "exit " );
	}

	/*
	 * Call the action routine
	 */

	action = stateData[ STM_ActionOffset( input )];
	ExecuteAction( sm, action, "" );

	if ( nextState != sm->currState )
	{
		int		enterAction;

		/*
		 * Enter the new state
		 */
	
		DBG_STATE( "STM %s: Changing states from %s (%d) to %s (%d)\n",
				   sm->name,
				   sm->stateStr[ sm->currState ], sm->currState,
				   sm->stateStr[ nextState ], nextState );

		sm->currState = nextState;
		stateData = &sm->stateData[ sm->currState * STM_BytesPerState( sm )];

		/*
		 * We're entering the new state, call the enter routine if one is
		 * defined
		 */

		enterAction = stateData[ STM_EnterOffset() ];
		ExecuteAction( sm, enterAction, "enter " );
	}

	if ( currSm != NULL )
	{
		*currSm = svSm;
	}

	/*
	 * We processed an input, tell the caller
	 */

	sm->currInput = STM_INPUT_NONE;
#if defined( STM_DEBUG )
	sm->currInputStr = "- None -";
#endif

	return TRUE;

} // STM_Execute1

#if defined( STM_DEBUG )

//***************************************************************************
/**
*	Sets the debug options for the various debug flags
*	associated with the state machine `sm`. `dbgInputs` controls whether
*	inputs are displayed as they're taken off the input queue. `dbgActions`
*	controls whether the names of the action routines are printed as they
*	are executed. `dbgStates` controls whether the state transitions are
*	printed as they occur.
*/

void STM_SetDebug
(
    STM_StateMachine *sm,   ///< State machine to set options for.
    int dbgInputs,          ///< Should inputs be printed as they're processed?
    int dbgActions,         ///< Should actions be printed as they're executed?
    int dbgStates           ///< Should states be printed as the current state changes?
)
{
	ASSERT( sm != NULL );
	ASSERT( sm->magic == STM_MAGIC );
	
	sm->dbgInputs  = dbgInputs;
	sm->dbgActions = dbgActions;
	sm->dbgStates  = dbgStates;

} // STM_SetDebug

#endif	/* defined( STM_DEBUG )													*/

//***************************************************************************
/**
*	Sets the user-defined parameter associated with state
*	machine `sm` to `userParm`. This may then be used by action routines
*	to access state machine specific data structures.
*/

void STM_SetUserParm( STM_StateMachine *sm, void *userParm )
{
	ASSERT( sm != NULL );
	ASSERT( sm->magic == STM_MAGIC );
	
	sm->userParm = userParm;

} // STM_SetUserParm

#if defined( STM_DEBUG )

//***************************************************************************
/**
*	Returns an ASCII representation of the current state
*	for state machine `sm`.
*/

char *STM_CurrStateStr( STM_StateMachine *sm )
{
	ASSERT( sm != NULL );
	ASSERT( sm->magic == STM_MAGIC );
	
	if ( sm->currState == STM_STATE_NONE )
	{
		return "None";
	}

	return sm->stateStr[ sm->currState ];

} // STM_CurrStateStr

#endif	/* defined( STM_DEBUG )													*/

//***************************************************************************
/**
*	Executes the action indicated by `actionIdx` for state
*	machine `sm`. This function also does all of the appropriate debug
*	tracing, and incorporates `actionStr` into the debug messages.
*/

void ExecuteAction( STM_StateMachine *sm, int actionIdx, const char *actionStr )
{
	/*
	 * ActionIdx 0 is reserved as the 'do-nothing' action, or idle action
	 * (but only if one wasn't provided by the user).
	 */

	if ( actionIdx > 0 )
	{
		ASSERT( actionIdx < sm->numActions );

		if ( sm->action != NULL )
		{
			ASSERT( sm->actionProc == NULL );

			DBG_ACTION( "STM %s: Calling %s action %s\n",
						sm->name, actionStr, sm->actionStr[ actionIdx ] );

			(*sm->action[ actionIdx ])();
		}
		else
		{
			ASSERT( sm->actionProc != NULL );

			DBG_ACTION( "STM %s: Invoking %s action %s\n",
						sm->name, actionStr, sm->actionStr[ actionIdx ] );

			(*sm->actionProc)( sm, actionIdx );
		}
	}

} // ExecuteAction

//***************************************************************************
/**
*	Retrieves the next input from the input queue of the state
*	machine given by `sm`. STM_INPUT_NONE is returned if no further inputs
*	are available.
*/

STM_Input GetInput( STM_StateMachine *sm )
{
	STM_Input	input;

	ASSERT( sm != NULL );
	ASSERT( sm->magic == STM_MAGIC );
	
	/*
	 * Check to see if anything is on the queue
	 */

	if ( sm->queueLen == 0 )
	{
		return STM_INPUT_NONE;
	}
	sm->queueLen--;

	/*
	 * Remove the next input from the queue
	 */

	input = sm->queue[ sm->queueGetIdx++ ];
	if ( sm->queueGetIdx >= sm->queueMax )
	{
		sm->queueGetIdx = 0;
	}

	sm->currInput = input;

#if defined( STM_DEBUG )
	sm->currInputStr = sm->inputStr[ input ];
#endif

	return input;

} // GetInput

