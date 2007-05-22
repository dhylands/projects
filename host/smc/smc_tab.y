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
*   @file   smc.y
*
*   @brief  Implements the parser for the state machine compiler.
*
*****************************************************************************/

%{

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
   
#include <Log.h>

#include "smc.h"

#define  YYDEBUG  1

#define	DBG(fmt,args...)	if ( gDebugYacc ) Log( fmt, ## args );

BOOL     gDebugYacc;
BOOL     gParsedOk = TRUE;

#define	MAX_INPUTS	100
#define	MAX_ACTIONS	100
#define	MAX_STATES	100
#define	MAX_IDS		MAX_INPUTS

typedef struct
{
   char	name[ 64 ];
} Id;

BOOL		gParsingDefaultState;

int		gNumIds;
Id			gId[ MAX_IDS ];

typedef struct
{
	char		      name[ 64 ];
	char		      enterActionStr[ 64 ];
	char		      exitActionStr[ 64 ];
	SMC_StateInput	input[ MAX_INPUTS ];

} ParseState;

SMC_Input	   gSmInput[ MAX_INPUTS ];
SMC_Action		gSmAction[ MAX_ACTIONS ];
SMC_State		gSmState[ MAX_STATES ];
ParseState	   gCurrState;
ParseState	   gDefaultState;

char		      gCurrActionStr[ 64 ];
SMC_Branch		gCurrBranch;
char		      gCurrNextStateStr[ 64 ];

SMC_StateMachine	gSM;

static	void	AddId( char *idStr );
static	void	AddInput( char *inputStr, char *actionStr, SMC_Branch branch, char *nextStateStr );
static	void	AddState( char *stateStr );
static	int	FindAction( char *actionStr );
static	void	ResolveStates( void );

extern	void	yyerror( char *str );
extern	int	yylex( void );

%}

%union	{
	long	 num;
	char	*str;
};

%token			ACTION_PREFIX
%token			BRANCH
%token			DEFAULT
%token			ELSE
%token			ENTER			  1
%token			EXIT
%token	<str>	IDENTIFIER
%token			IDLE
%token			INPUTS
%token			INPUT_PREFIX
%token			INPUT_QUEUE
%token			NAME
%token			NEXT
%token			NO_BRANCH
%token	<num>	NUMBER
%token			PREVIOUS
%token			STATE
%token			STATE_PREFIX
				
%%

stateMachine: declarations
				{
					int		i;

					/*
					 * Initialize the default state inputs
					 */

					memset( &gDefaultState, 0, sizeof( gDefaultState ));

					for ( i = 0; i < gSM.numInputs; i++ )
					{
						gDefaultState.input[ i ].dflt = TRUE;
						gDefaultState.input[ i ].actionStr = gSmAction[ 0 ].name;
						gDefaultState.input[ i ].nextStateStr = NULL;
						gDefaultState.input[ i ].action = 0;
						gDefaultState.input[ i ].branch = NoBranch;
						gDefaultState.input[ i ].nextState = -1;
					}
					gSM.queueLen = 10;
				}
			  stateList
				{
					gSM.input = gSmInput;
					gSM.action = gSmAction;
					gSM.state = gSmState;

					ResolveStates();
					if ( !gParsedOk )
					{
						YYABORT;
					}

					DBG( "Parsing completed\n" );
				}
			| error
				{
					gParsedOk = FALSE;
					YYABORT;
				}
			;

declarations: smName optDeclList inputList idleAction
			;

smName		: NAME IDENTIFIER
				{
					gSM.name = strdup( $2 );
					DBG( "Parsed SM %s\n", gSM.name );
				}
			;

optDeclList	: declList
			| /* empty */
			;

declList	: decl
			| declList decl
			;

decl		: ACTION_PREFIX IDENTIFIER
				{
					gSM.actionPrefix = strdup( $2 );
					DBG( "Parsed ActionPrefix %s\n", gSM.actionPrefix );
				}
			| INPUT_PREFIX IDENTIFIER
				{
					gSM.inputPrefix = strdup( $2 );
					DBG( "Parsed InputPrefix %s\n", gSM.inputPrefix );
				}
			| STATE_PREFIX IDENTIFIER
				{
					gSM.statePrefix = strdup( $2 );
					DBG( "Parsed StatePrefix %s\n", gSM.statePrefix );
				}
			| INPUT_QUEUE NUMBER
				{
					gSM.queueLen = $2;
					DBG( "Parsed InputQueue %ld\n", gSM.queueLen );
				}
			;

inputList	: INPUTS '[' 
				{
					gNumIds = 0;
				}
			  idList
			  	{
					int		i;

					/*
					 * Copy the inputs that were collected in idList into
					 * the gSmInput array.
					 */

					for ( i = 0; i < gNumIds; i++ )
					{
						gSmInput[ i ].name = strdup( gId[ i ].name );
						DBG( "Parsed input %2d: %s\n", i, gId[ i ].name );
					}
					gSM.input = gSmInput;
					gSM.numInputs = gNumIds;
			  	}
			  ']'
			;

idleAction	: IDLE IDENTIFIER
				{
					/*
					 * The idle action was specified, make it entry 1
					 */

					ASSERT( gSM.numActions == 0 );
					gSM.numActions = 2;
					gSmAction[ 0 ].name = NULL;
					gSmAction[ 1 ].name = strdup( $2 );

					DBG( "Parsed IdleAction %s\n", gSmAction[ 1 ].name );
				}
			| /* empty */
				{
					/*
					 * No idle action specified, make a NULL one anyway
					 */

					ASSERT( gSM.numActions == 0 );
					gSM.numActions = 1;
					gSmAction[ 0 ].name = NULL;

					DBG( "Parsed default IdleAction\n" );
				}
			;

stateList	: stateItem
			| stateList stateItem
			;

stateItem	: STATE
				{
					gCurrState = gDefaultState;
				}
			  stateId '[' optEnterItem optExitItem optCaseList optElseItem ']'
				{
					int		i;

					/*
					 * Copy the state inputs from what we've just parsed
					 * into the appropriate state entry.
					 */

					if ( gParsingDefaultState )
					{
						gDefaultState = gCurrState;
						for ( i = 0; i < gSM.numInputs; i++ )
						{
							gDefaultState.input[ i ].dflt = TRUE;
						}
					}
					else
					{
						SMC_State	*st = &gSmState[ gSM.numStates++ ];

						/*
						 * Allocate an array of state inputs
						 */

						st->name = strdup( gCurrState.name );
						if ( strlen( gCurrState.enterActionStr ) > 0 )
						{
							st->enterActionStr = strdup( gCurrState.enterActionStr );
						}
						else
						{
							st->enterActionStr = NULL;
						}
						if ( strlen( gCurrState.exitActionStr ) > 0 )
						{
							st->exitActionStr = strdup( gCurrState.exitActionStr );
						}
						else
						{
							st->exitActionStr = NULL;
						}

						st->stateInput = calloc( gSM.numInputs,
												 sizeof( SMC_StateInput ));

						for ( i = 0; i < gSM.numInputs; i++ )
						{
							st->stateInput[ i ] = gCurrState.input[ i ];
						}
					}
				}
			;

stateId		: IDENTIFIER
				{
					gParsingDefaultState = FALSE;
					AddState( $1 );
					if ( !gParsedOk )
					{
						YYABORT;
					}
					DBG( "Parsed State %s\n", gCurrState.name );
				}
			| DEFAULT
				{
					gParsingDefaultState = TRUE;
					DBG( "Parsed Default State\n" );
				}
			| /* empty */
				{
					char	stateName[ 10 ];

					gParsingDefaultState = FALSE;

					sprintf( stateName, "S%2d", gSM.numStates );
					AddState( stateName );
					if ( !gParsedOk )
					{
						YYABORT;
					}
					DBG( "Parsed Empty State\n" );
				}
			;

optCaseList : caseList
			| /* empty */
			;

caseList	: caseItem
			| caseList caseItem
			;

optEnterItem: ENTER action
				{
					/*
					 * This action is performed every time the state is entered
					 * regardless of how it got here.
					 */

					strcpy( gCurrState.enterActionStr, gCurrActionStr );
				}
			| /* empty */
			;

optExitItem	: EXIT action
				{
					/*
					 * This action is performed every time the state is exited
					 * regardless of how it was exited.
					 */

					strcpy( gCurrState.exitActionStr, gCurrActionStr );
				}
			| /* empty */
			;

caseItem	:	{
					gNumIds = 0;
				}
			  idList
				{
					int		i;

					if ( gDebugYacc )
					{
						for ( i = 0; i < gNumIds; i++ )
						{
							printf( "Parsed Input %s\n", gId[ i ].name );
						}
					}
				}
			  ':' action branch
				{
					int		i;

					for ( i = 0; i < gNumIds; i++ )
					{
						AddInput( gId[ i ].name, gCurrActionStr,
								  gCurrBranch, gCurrNextStateStr );
						if ( !gParsedOk )
						{
							YYABORT;
						}
					}
				}
			;

optElseItem	: ELSE ':' action branch
				{
					int		i;

					/*
					 * Else applies to all other inputs which haven't been
					 * explicitly specified. This causes any defaults which
					 * have been specified to be over-ridden.
					 */

					for ( i = 0; i < gSM.numInputs; i++ )
					{
						if ( gCurrState.input[ i ].dflt )
						{
							AddInput( gSmInput[ i ].name, gCurrActionStr,
									  gCurrBranch, gCurrNextStateStr );
							if ( !gParsedOk )
							{
								YYABORT;
							}
						}
					}
				}
			| /* empty */
			;

action		: IDENTIFIER
				{
					strncpy( gCurrActionStr, $1, sizeof( gCurrActionStr ) - 1 );
					gCurrActionStr[ sizeof( gCurrActionStr ) - 1 ] = '\0';
					DBG( "Parsed action %s\n", gCurrActionStr );
				}
			| IDLE
				{
					strcpy( gCurrActionStr, "" );
					DBG( "Parsed action IDLE\n" );
				}
			;

branch		: NO_BRANCH
				{
					gCurrBranch = NoBranch;
					DBG( "Parsed NoBranch\n" );
				}
			| PREVIOUS
				{
					gCurrBranch = BranchPrevious;
					DBG( "Parsed BranchPrevious\n" );
				}
			| NEXT
				{
					gCurrBranch = BranchNext;
					DBG( "Parsed BranchNext\n" );
				}
			| BRANCH IDENTIFIER
				{
					gCurrBranch = BranchId;
					strncpy( gCurrNextStateStr, $2, sizeof( gCurrNextStateStr ) - 1 );
					gCurrNextStateStr[ sizeof( gCurrNextStateStr ) - 1 ] = '\0';
					DBG( "Parsed BranchId %s\n", gCurrNextStateStr );
				}
			;

idList		: idItem
			| idList idItem
			;

idItem		: IDENTIFIER
				{
					AddId( $1 );
					if ( !gParsedOk )
					{
						YYABORT;
					}
				}
			;

%%

/************************************************************************
 *
 *	AddId
 *
 ************************************************************************/

void AddId( char *idStr )
{
	char		msg[ 100 ];

	if (( gNumIds + 1 ) >= MAX_IDS )
	{
		sprintf( msg, "ID Table size exceeded, size = %d", MAX_IDS );
		yyerror( msg );
		return;
	}

	if ( strlen( idStr ) >= sizeof( gId[ gNumIds ].name ))
	{
		sprintf( msg, "ID limited to %d characters", sizeof( gId[ gNumIds ].name ));
		yyerror( msg );
		return;
	}

	strcpy( gId[ gNumIds++ ].name, idStr );
}

/************************************************************************
 *
 *	AddInput
 *
 ************************************************************************/

void AddInput( char *inputStr, char *actionStr,
			   SMC_Branch branch, char *nextStateStr )
{
	int		i;
	char	msg[ 100 ];

	/*
	 * Determine the index of the input
	 */

	for ( i = 0; i < gSM.numInputs; i++ )
	{
		if ( strcmp( gSmInput[ i ].name, inputStr ) == 0 )
		{
			break;
		}
	}
	if ( i >= gSM.numInputs )
	{
		/*
		 * The input wasn't found
		 */

		sprintf( msg, "Undefined input: %s", inputStr );
		yyerror( msg );		
		return;
	}

	/*
	 * Add the action and the branch
	 */

	ASSERT( actionStr != NULL );

	gCurrState.input[ i ].dflt = FALSE;
	gCurrState.input[ i ].actionStr = strdup( actionStr );
	gCurrState.input[ i ].action = -1;
	gCurrState.input[ i ].branch = branch;
	if ( branch == BranchId )
	{
		ASSERT( nextStateStr != NULL );
		gCurrState.input[ i ].nextStateStr = strdup( nextStateStr );
	}
	else
	{
		gCurrState.input[ i ].nextStateStr = NULL;
	}
}

/************************************************************************
 *
 *	AddState
 *
 ************************************************************************/

void AddState( char *stateStr )
{
	int		i;
	char	msg[ 100 ];

	if (( gSM.numStates + 1 ) >= MAX_STATES )
	{
		sprintf( msg, "State Table size exceeded, size = %d", MAX_STATES );
		yyerror( msg );
		return;
	}

	/*
	 * Make sure that it's not a duplicate state name
	 */

	for ( i = 0; i < gSM.numStates; i++ )
	{
		if ( strcmp( gSmState[ i ].name, stateStr ) == 0 )
		{
			sprintf( msg, "State %s already defined", stateStr );
			yyerror( msg );
			return;
		}
	}

	strncpy( gCurrState.name, stateStr, sizeof( gCurrState.name ) - 1 );
	gCurrState.name[ sizeof( gCurrState.name ) - 1 ] = '\0';
}

/************************************************************************
 *
 *	FindAction
 *
 ************************************************************************/

int FindAction( char *actionStr )
{
	int		actionIdx;
	char	msg[ 100 ];

	if ( actionStr == NULL )
	{
		return 0;
	}

	/*
	 * Resolve actions into indexes
	 */

	for ( actionIdx = 1; actionIdx < gSM.numActions; actionIdx++ )
	{
		if  ( strcmp( actionStr, gSmAction[ actionIdx ].name ) == 0 )
		{
			return actionIdx;
		}
	}

	/*
	 * The action hasn't been resolved yet, add a new one
	 */

	if (( gSM.numActions + 1 ) >= MAX_ACTIONS )
	{
		sprintf( msg, "Action Table size exceeded, size = %d",
				 MAX_ACTIONS );
		yyerror( msg );
		return 0;
	}

	gSmAction[ actionIdx ].name = actionStr;
	gSM.numActions = actionIdx + 1;

	return actionIdx;
}

/************************************************************************
 *
 *	ResolveStates
 *
 ************************************************************************/

void ResolveStates( void )
{
	int		stateIdx;
	int		inputIdx;
	int		branchIdx;

	for ( stateIdx = 0; stateIdx < gSM.numStates; stateIdx++ )
	{
		SMC_State	*st = &gSM.state[ stateIdx ];

		st->enterAction = FindAction( st->enterActionStr );
		st->exitAction = FindAction( st->exitActionStr );

		for ( inputIdx = 0; inputIdx < gSM.numInputs; inputIdx++ )
		{
			SMC_StateInput	*si = &st->stateInput[ inputIdx ];

			/*
			 * Resolve actions into indexes
			 */

			si->action = FindAction( si->actionStr );

			/*
			 * Resolve branches into states
			 */

			switch ( si->branch )
			{
				case NoBranch:
				{
					si->nextState = stateIdx;
					break;
				}

				case BranchPrevious:
				{
					if ( stateIdx <= 0 )
					{
						fprintf( stderr, "ERROR: Can't branch to a previous "
								 "state from state %s (input %s)\n",
								 st->name, gSmInput[ inputIdx ].name );
						gParsedOk = FALSE;
					}	
					else
					{
						si->nextState = stateIdx - 1;
					}
					break;
				}

				case BranchNext:
				{
					if (( stateIdx + 1 ) >= gSM.numStates )
					{
						fprintf( stderr, "ERROR: Can't branch to a next state "
								 "from state %s (input %s)\n",
								 st->name, gSmInput[ inputIdx ].name );
						gParsedOk = FALSE;
					}
					else
					{
						si->nextState = stateIdx + 1;
					}
					break;
				}

				case BranchId:
				{
					for ( branchIdx = 0; branchIdx < gSM.numStates; branchIdx++ )
					{
						if ( strcmp( gSM.state[ branchIdx ].name,
									 si->nextStateStr ) == 0 )
						{
							si->nextState = branchIdx;
							break;
						}
					}
					if ( branchIdx >= gSM.numStates )
					{
						fprintf( stderr, "ERROR: Unrecognized branch: %s in "
								 "state %s, input %s\n", si->nextStateStr,
								 st->name, gSmInput[ inputIdx ].name );
						gParsedOk = FALSE;
					}
					break;
				}

				default:
				{
					fprintf( stderr, "ERROR: Internal error, unrecognized "
							 "branch during resolve\n" );
					gParsedOk = FALSE;
					break;
				}
			}
		}
	}
}

