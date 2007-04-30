/****************************************************************************
*
*   Copyright (c) 2007 Dave Hylands     <dhylands@gmail.com>
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
*   @file   smc.h
*
*   @brief  Contains internal definitions, used by the parser for the
*           the internal representation of the state machine.
*
****************************************************************************/

#if !defined( SMC_H )
#define SMC_H               /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

/* ---- Constants and Types ---------------------------------------------- */

typedef struct
{
	char   *name;			/* Name of this input.							*/

} SMC_Input;

typedef struct
{
	char   *name;			/* Name of this action.							*/

} SMC_Action;

typedef enum
{
	NoBranch		= -1,
	BranchPrevious	= -2,
	BranchNext		= -3,
	BranchId		= -4,

} SMC_Branch;

typedef struct
{
	int 	    dflt;			/* Is this a default state input?				*/
	char       *actionStr;		/* Name of the action before being resolved.	*/
	char       *nextStateStr;	/* Name of the next state before being resolved.*/
	int		    action;			/* Index of the action after being resolved.	*/
	SMC_Branch	branch;			/* Type of branch to perform					*/
	int		    nextState;		/* Index of the input after being resolved.		*/

} SMC_StateInput;

typedef struct
{
	char	   *name;			/* Name of this state.						*/

	char	   *enterActionStr;	/* Unresolved name of enter action.			*/
	char	   *exitActionStr;	/* Unresolved name of exit action.			*/

	int			enterAction;	/* Input simulated when a state is entered	*/
	int			exitAction;		/* Input simulated when a state is entered	*/

	SMC_StateInput *stateInput;	/* Array of input infor for this state		*/

} SMC_State;

typedef struct
{
	char       *name;			/* Name of the state machine.					*/
	char       *actionPrefix;	/* Prefix to use for each action.				*/
	char       *inputPrefix;	/* Prefix to use for each input.				*/
	char       *statePrefix;	/* Prefix to use for each state.				*/

	int		    numActions;		/* Number of actions for this state machine		*/
	SMC_Action *action;			/* Ptr to array of actions.						*/

	int		    numInputs;		/* Number of inputs for this state machine		*/
	SMC_Input  *input;			/* Ptr to array of inputs.						*/

	int		    numStates;		/* Number of states for this state machine		*/
	SMC_State  *state;			/* Ptr to array of states.						*/

	int		    queueLen;		/* Length of input queue						*/

} SMC_StateMachine;

#define TRUE    1
#define FALSE   0

typedef int     BOOL;

/* ---- Variable Externs ------------------------------------------------- */

extern  BOOL    gDebugYacc;
extern  BOOL    gDebugLex;
extern  BOOL    gParsedOk;

extern	SMC_StateMachine	gSM;

/* ---- Function Prototypes ---------------------------------------------- */

#endif  // SMC_H

