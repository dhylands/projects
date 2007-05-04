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
*   @file   smc.c 
*
*   @brief  Main program for the smc (State Machine Compiler)
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <Log.h>

#include "stm.h"
#include "smc.h"

/* ---- Public Variables -------------------------------------------------- */

extern	BOOL	parsedOk;
extern  FILE   *yyin;

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

static	char   *curr_fname = "** none **";
static	int		curr_lineno = 0;
static	char   *curr_ptr = "";
static	char	line[ 512 ];
static	FILE   *curr_fs;

BOOL	verbose = FALSE;

/* ---- Private Function Prototypes --------------------------------------- */

static	void	DumpSm( SMC_StateMachine *sm, FILE *fs );
static	void	GenStmSmCsrc( SMC_StateMachine *sm, FILE *fs, char *smcFname,
							  char *cFname, char *hFname );
static	void	GenStmSmHsrc( SMC_StateMachine *sm, FILE *fs, char *smcFname,
							  char *hFname );
static	void	Usage( char *pgm );

extern  int yydebug;

int     yyparse( void );
void    yyerror( char *str );
int	    yygetc( void );

/* ---- Functions --------------------------------------------------------- */

/************************************************************************
 *
 * main - does command line parsing and program control
 *
 ************************************************************************/

int main( int argc, char **argv )
{
	int		i, arg;
    char   *s;
	char   *base;
	char   *pp_fname;
	char   *smcName;
	char   *cFname;
	char   *hFname;
	BOOL	skip_cpp = FALSE;
	BOOL	keep_pp = FALSE;
	FILE   *fs;

	int		cpp_argc = 0;
	char   *cpp_argv[ 20 ];
    size_t  cpp_len;
    char   *cpp_cmdline;

    LogInit( stdout );

	smcName = NULL;

	cpp_argv[ cpp_argc++ ] = "gcc -E -x c";

	/*
	 * Parse the command line
	 */

	for ( arg = 1; arg < argc; arg++ )
	{
		if ( strcmp( argv[ arg ], "-l" ) == 0 )
		{
			gDebugLex = TRUE;
		}
		else
		if ( strcmp( argv[ arg ], "-y" ) == 0 )
		{
			gDebugYacc = TRUE;
            yydebug = TRUE;
		}
		else
		if ( strcmp( argv[ arg ], "-k" ) == 0 )
		{
			keep_pp = TRUE;
		}
		else
		if ( strcmp( argv[ arg ], "-v" ) == 0 )
		{
			verbose = TRUE;
		}
		else
		if ( strcmp( argv[ arg ], "-s" ) == 0 )
		{
			skip_cpp = TRUE;
			keep_pp = TRUE;
		}
		else
		if (( argv[ arg ][ 0 ] == '+' )
		||  ( strncmp( argv[ arg ], "-I", 2 ) == 0 )
		||  ( strncmp( argv[ arg ], "-U", 2 ) == 0 )
		||  ( strncmp( argv[ arg ], "-D", 2 ) == 0 ))
		{
			/*
			 * Pass these options on to the pre-processor
			 */

			cpp_argv[ cpp_argc++ ] = argv[ arg ];
		}
		else
		if ( arg == ( argc - 1 ))
		{
            smcName = argv[ arg ];
		}
		else
		{
			fprintf( stderr, "Unrecognized option: %s\n", argv[ arg ]);
			Usage( argv[ 0 ]);
		}
	}

	if ( smcName == NULL )
	{
		Usage( argv[ 0 ]);
	}

	/*
	 * Determine if an extension was provided. Assume smc if none given
	 */

    base = malloc( strlen( smcName ) + 5 );
    strcpy( base, smcName );

    if (( s = strrchr( base, '/' )) == NULL )
    {
        s = base;
    }
    if (( s = strrchr( s,  '.' )) != NULL )
    {
        *s = '\0';
    }
    cFname = malloc( strlen( base ) + 6 );
    strcpy( cFname, base );
    strcat( cFname, "-sm.c" );
    
    hFname = malloc( strlen( base ) + 6 );
    strcpy( hFname, base );
    strcat( hFname, "-sm.h" );

    pp_fname = malloc( strlen( base ) + 7 );
	strcpy( pp_fname, base );
	strcat( pp_fname, "-sm.pp" );

	/*
	 * See if the file is accessible
	 */

	if ( access( smcName, 4 ) != 0 )
	{
		perror( smcName );
		exit( 1 );
	}

	if ( !skip_cpp )
	{
		/*
		 * Run the preprocessor on the file
		 *
		 *	This leaves fname.pp in the current directory
		 */

        cpp_argv[ cpp_argc++ ] = "-o";
        cpp_argv[ cpp_argc++ ] = pp_fname;
		cpp_argv[ cpp_argc++ ] = smcName;
		cpp_argv[ cpp_argc ] = NULL;

		if ( verbose )
		{
			printf( "About to run" );
			for ( i = 0; i < cpp_argc; i++ )
			{
				printf( " %s", cpp_argv[ i ]);
			}
			printf( "\n" );
		}

        cpp_len = 0;
        for ( i = 0; i < cpp_argc; i++ )
        {
            cpp_len += strlen( cpp_argv[ i ]);
            cpp_len++;
        }
        cpp_cmdline = malloc( cpp_len );
        cpp_cmdline[ 0 ] = '\0';

        for ( i = 0; i < cpp_argc; i++ )
        {
            if ( i > 0 )
            {
                strcat( cpp_cmdline, " " );
            }
            strcat( cpp_cmdline, cpp_argv[ i ]);
        }

		if ( system( cpp_cmdline ) != 0 )
		{
			perror( "sysetm" );
			fprintf( stderr, "Unable to execute: '%s'\n", cpp_cmdline );
			exit(1);
		}

		if ( verbose )
		{
			printf( "%s done\n", cpp_argv[ 0 ]);
		}
	}

	if ( access( pp_fname, 4 ) != 0 )
	{
		perror( pp_fname );
		exit( 1 );
	}

	/*
	 * Open the file for input
	 */

	if ( verbose )
	{
		printf( "Parsing file: %s\n", pp_fname );
	}

	if (( curr_fs = fopen( pp_fname, "r" )) == NULL )
	{
		fprintf( stderr, "Unable to open file %s\n", pp_fname );
		exit( 1 );
	}
    yyin = curr_fs;
	curr_fname = pp_fname;

	/*
	 * Parse the file building a database template in memory
	 */

	if ( yyparse() == 0 )
	{
		if ( verbose )
		{
			printf( "File %s parsed successfully\n", pp_fname );
			DumpSm( &gSM, stdout );
		}

		if (( fs = fopen( cFname, "w" )) == NULL )
		{
			fprintf( stderr, "Unable to open %s for writing\n", cFname );
			exit( 1 );
		}
		GenStmSmCsrc( &gSM, fs, smcName, cFname, hFname );
		fclose( fs );

		if (( fs = fopen( hFname, "w" )) == NULL )
		{
			fprintf( stderr, "Unable to open %s for writing\n", hFname );
			exit( 1 );
		}
		GenStmSmHsrc( &gSM, fs, smcName, hFname );
		fclose( fs );
	}
	else
	{
		fprintf( stderr, "Error parsing file %s", pp_fname );
	}

	/*
	 * Remove the .i file
	 */

	fclose( curr_fs );

	if ( !keep_pp )
	{
		if ( verbose )
		{
			printf( "Removing %s\n", pp_fname );
		}
		if ( remove( pp_fname ) != 0 )
		{
			fprintf( stderr, "Unable to remove " );
			perror( pp_fname );
		}
	}

	exit( 0 );
	return 0;
}

/************************************************************************
 *
 * DumpSm - Dump the state machine in a parsable form.
 *
 ************************************************************************/

void DumpSm( SMC_StateMachine *sm, FILE *fs )
{
	int		i;
	int		inputIdx;
	int		stateIdx;

	fprintf( fs, "/************************************"
	              "************************************\n" );
	fprintf( fs, " *\n" );
	fprintf( fs, " * Dump of state machine: %s\n", sm->name );
	fprintf( fs, " *\n" );
	fprintf( fs, " ************************************"
	              "************************************/\n" );
	fprintf( fs, "\n" );

	/*
	 * Print the state machine name
	 */

	fprintf( fs, "Name %s\n\n", sm->name );

	/*
	 * Print any prefixes which have been entered
	 */

	if ( sm->actionPrefix != NULL )
	{
		fprintf( fs, "ActionPrefix %s\n", sm->actionPrefix );
	}
	if ( sm->inputPrefix != NULL )
	{
		fprintf( fs, "InputPrefix  %s\n", sm->inputPrefix );
	}
	if ( sm->statePrefix != NULL )
	{
		fprintf( fs, "StatePrefix  %s\n", sm->statePrefix );
	}
	fprintf( fs, "InputQueue   %d\n", sm->queueLen );

	/*
	 * Print the state machine inputs
	 */

	fprintf( fs, "Inputs\n[\n" );
	for ( i = 0; i < sm->numInputs; i++ )
	{
		fprintf( fs, "    %s\n", sm->input[ i ].name );
	}
	fprintf( fs, "]\n\n" );

	/*
	 * Print the state machine idle action
	 */

	if ( sm->action[ 0 ].name != NULL )
	{
		fprintf( fs, "Idle %s\n\n", sm->action[ 0 ].name );
	}

	/*
	 * Print the states themselves
	 */

	for ( stateIdx = 0; stateIdx < sm->numStates; stateIdx++ )
	{
		SMC_State	*st = &sm->state[ stateIdx ];

		fprintf( fs, "State %s\n[\n", st->name );
		for ( inputIdx = 0; inputIdx < sm->numInputs; inputIdx++ )
		{
			SMC_StateInput	*si = &st->stateInput[ inputIdx ];

			fprintf( fs, "    %-20s : %-20s ",
					 sm->input[ inputIdx ].name,
					 sm->action[ si->action ].name );

			switch ( si->branch )
			{
				case NoBranch:			fprintf( fs, "NoBranch" );	break;
				case BranchPrevious:	fprintf( fs, "Previous" );	break;
				case BranchNext:		fprintf( fs, "Next" );		break;
				case BranchId:
				{
					fprintf( fs, "Branch %s",
							 sm->state[ si->nextState ].name );
					break;
				}
				default:				fprintf( fs, "???" );		break;
			}
			fprintf( fs, "\n" );
		}
		fprintf( fs, "]\n\n" );
	}
}

/************************************************************************
 *
 * genStmSmCsrc - Generate the C file which is useable by the STM module
 *
 ************************************************************************/

void GenStmSmCsrc( SMC_StateMachine *sm, FILE *fs, char *smcFname, char *cFname,
				   char *hFname )
{
	int		actionIdx;
	int		inputIdx;
	int		stateIdx;
	int		byteIdx;
	int		numBytes;
	time_t	currTime = time( (time_t)NULL );
	uint8_t *data;

    numBytes = STM_CalcBytesPerState( sm );
    data = malloc( numBytes );

	fprintf( fs, "/************************************"
	              "************************************\n" );
	fprintf( fs, " *\n" );
	fprintf( fs, " * NAME\n" );
	fprintf( fs, " *  %s\n", cFname );
	fprintf( fs, " *\n" );
	fprintf( fs, " * State machine: %s\n", sm->name );
	fprintf( fs, " *\n" );
	fprintf( fs, " * This file was automatically generated from: %s\n", smcFname );
	fprintf( fs, " *\n" );
	fprintf( fs, " * DO NOT EDIT THIS FILE. YOUR CHANGES WILL BE\n" );
	fprintf( fs, " * OVER-WRITTEN THE NEXT TIME THAT DDL IS RUN.\n" );
	fprintf( fs, " *\n" );
	fprintf( fs, " * GENERATED\n" );
	fprintf( fs, " *  %s", ctime( &currTime ));
	fprintf( fs, " *\n" );
	fprintf( fs, " ************************************"
	              "************************************/\n" );
	fprintf( fs, "\n" );
	fprintf( fs, "#include \"stm.h\"\n" );
	fprintf( fs, "#include \"%s\"\n", hFname );
	fprintf( fs, "\n" );
    fprintf( fs, "#if !defined( NULL )\n" );
    fprintf( fs, "#   define NULL 0\n" );
    fprintf( fs, "#endif\n" );
    fprintf( fs, "\n" );
	fprintf( fs, "static STM_Input %s_inputQueue[ %d ];\n",
				 sm->name, sm->queueLen );
	fprintf( fs, "\n" );

	fprintf( fs, "static uint8_t %s_stateData[ %d * %d ] =\n",
				 sm->name, sm->numStates, numBytes );
	fprintf( fs, "{\n" );

	for ( stateIdx = 0; stateIdx < sm->numStates; stateIdx++ )
	{
        int         width;
		SMC_State  *st = &sm->state[ stateIdx ];

        memset( data, 0, numBytes );

		data[ STM_EnterOffset() ] = st->enterAction;
		data[ STM_ExitOffset() ] = st->exitAction;

		for ( inputIdx = 0; inputIdx < sm->numInputs; inputIdx++ )
		{
			SMC_StateInput *si = &st->stateInput[ inputIdx ];

			data[ STM_ActionOffset( inputIdx )] = si->action;
			data[ STM_NextStateOffset( inputIdx )] = si->nextState;
		}

		fprintf( fs, "    /* S%02d */ ", stateIdx );

        width = 1;
        for ( byteIdx = 1; byteIdx < numBytes; byteIdx++ )
        {
            if (( data[ byteIdx ] > 10 ) && ( width < 2 ))
            {
                width = 2;
            }
            else
            if (( data[ byteIdx ] > 100 ) && ( width < 3 ))
            {
                width = 3;
            }
        }


		fprintf( fs, "%*d", width, data[ 0 ] );
		for ( byteIdx = 1; byteIdx < numBytes; byteIdx++ )
		{
			fprintf( fs, ", %*d", width, data[ byteIdx ] );
		}
		fprintf( fs, "%s\n", ( stateIdx + 1 ) < sm->numStates ? "," : "" );
	}

#undef setByte

	fprintf( fs, "};\n" );
	fprintf( fs, "\n" );
	fprintf( fs, "static STM_Action %s_action[ %d ] =\n",
				 sm->name, sm->numActions );
	fprintf( fs, "{\n" );
	for ( actionIdx = 0; actionIdx < sm->numActions; actionIdx++ )
	{
		if ( sm->action[ actionIdx ].name == NULL )
		{
			fprintf( fs, "    NULL%s\n", 
					 ( actionIdx + 1 ) < sm->numActions ? "," : "" );
		}
		else
		{
			fprintf( fs, "    %s%s%s\n",
					 sm->actionPrefix != NULL ? sm->actionPrefix : "",
					 sm->action[ actionIdx ].name,
					 ( actionIdx + 1 ) < sm->numActions ? "," : "" );
		}
	}
	fprintf( fs, "};\n" );
	fprintf( fs, "\n" );
	fprintf( fs, "#if defined( STM_DEBUG )\n" );
	fprintf( fs, "\n" );

	fprintf( fs, "static char     *%s_actionStr[ %d ] =\n",
				 sm->name, sm->numActions );
	fprintf( fs, "{\n" );
	for ( actionIdx = 0; actionIdx < sm->numActions; actionIdx++ )
	{
		fprintf( fs, "    \"%s\"%s\n",
				 sm->action[ actionIdx ].name != NULL
				 	? sm->action[ actionIdx ].name : "",
				 ( actionIdx + 1 ) < sm->numActions ? "," : "" );
	}
	fprintf( fs, "};\n" );
	fprintf( fs, "\n" );
	
	fprintf( fs, "static char     *%s_inputStr[ %d ] =\n",
				 sm->name, sm->numInputs );
	fprintf( fs, "{\n" );
	for ( inputIdx = 0; inputIdx < sm->numInputs; inputIdx++ )
	{
		fprintf( fs, "    \"%s\"%s\n",
				 sm->input[ inputIdx ].name, 
				 ( inputIdx + 1 ) < sm->numInputs ? "," : "" );
	}
	fprintf( fs, "};\n" );
	fprintf( fs, "\n" );

	fprintf( fs, "static char     *%s_stateStr[ %d ] =\n",
				 sm->name, sm->numStates );
	fprintf( fs, "{\n" );
	for ( stateIdx = 0; stateIdx < sm->numStates; stateIdx++ )
	{
		fprintf( fs, "    \"%s\"%s\n",
				 sm->state[ stateIdx ].name, 
				 ( stateIdx + 1 ) < sm->numStates ? "," : "" );
	}
	fprintf( fs, "};\n" );
	fprintf( fs, "\n" );

	fprintf( fs, "#endif\n" );
	fprintf( fs, "\n" );

	fprintf( fs, "STM_StateMachine %sSm =\n", sm->name );
	fprintf( fs, "{\n" );
	fprintf( fs, "    %5d,  /* numActions           */\n", sm->numActions );
	fprintf( fs, "    %5d,  /* numInputs            */\n", sm->numInputs );
	fprintf( fs, "    %5d,  /* numStates            */\n", sm->numStates );
	fprintf( fs, "    %5d,  /* currState            */\n", 0 );
	fprintf( fs, "    %s_inputQueue,\n", sm->name );
	fprintf( fs, "    %5d,  /* queueLen             */\n", 0 );
	fprintf( fs, "    %5d,  /* queuePutIdx          */\n", 0 );
	fprintf( fs, "    %5d,  /* queueGetIdx          */\n", 0 );
	fprintf( fs, "    %5d,  /* queueMax             */\n", sm->queueLen );
    fprintf( fs, "    %5d,  /* stateBytes           */\n", numBytes );
	fprintf( fs, "    %s_stateData,\n", sm->name );
	fprintf( fs, "    %s_action,\n", sm->name );
	fprintf( fs, "    NULL,   /* actionProc           */\n" );
	fprintf( fs, "    NULL    /* userParm             */\n" );
	fprintf( fs, "\n" );
	fprintf( fs, "#if defined( STM_DEBUG )\n" );
	fprintf( fs, "    ,\n" );
	fprintf( fs, "    STM_MAGIC,\n" );
	fprintf( fs, "    \"%s\",\n", sm->name );
	fprintf( fs, "    %s_actionStr,\n", sm->name );
	fprintf( fs, "    %s_inputStr,\n", sm->name );
	fprintf( fs, "    %s_stateStr,\n", sm->name );
	fprintf( fs, "    0,  /* FALSE - dbgActions       */\n" );
	fprintf( fs, "    0,  /* FALSE - dbgInputs        */\n" );
	fprintf( fs, "    0   /* FALSE - dbgStates        */\n" );
	fprintf( fs, "#endif\n" );
	fprintf( fs, "};\n" );
}

/************************************************************************
 *
 * GenStmSmHsrc - Generate the H file which is useable by the STM module
 *
 ************************************************************************/

void GenStmSmHsrc( SMC_StateMachine *sm, FILE *fs, char *smcFname, char *hFname )
{
	int		actionIdx;
	int		inputIdx;
	time_t	currTime = time( (time_t)NULL );

	fprintf( fs, "/************************************"
	              "************************************\n" );
	fprintf( fs, " *\n" );
	fprintf( fs, " * NAME\n" );
	fprintf( fs, " *  %s\n", hFname );
	fprintf( fs, " *\n" );
	fprintf( fs, " * State machine: %s\n", sm->name );
	fprintf( fs, " *\n" );
	fprintf( fs, " * This file was automatically generated from: %s\n", smcFname );
	fprintf( fs, " *\n" );
	fprintf( fs, " * DO NOT EDIT THIS FILE. YOUR CHANGES WILL BE\n" );
	fprintf( fs, " * OVER-WRITTEN THE NEXT TIME THAT DDL IS RUN.\n" );
	fprintf( fs, " *\n" );
	fprintf( fs, " * GENERATED\n" );
	fprintf( fs, " *  %s", ctime( &currTime ));
	fprintf( fs, " *\n" );
	fprintf( fs, " ************************************"
	              "************************************/\n" );
	fprintf( fs, "\n" );
	fprintf( fs, "#if !defined( STM_H )\n" );
	fprintf( fs, "#    include \"stm.h\"\n" );
	fprintf( fs, "#endif\n" );
	fprintf( fs, "\n" );

	fprintf( fs, "typedef enum\n" );
	fprintf( fs, "{\n" );

	for ( inputIdx = 0; inputIdx < sm->numInputs; inputIdx++ )
	{
		fprintf( fs, "    %s%-30s = %d%s\n",
				 sm->inputPrefix != NULL ? sm->inputPrefix : "",
				 sm->input[ inputIdx ].name,
				 inputIdx,
				 ( inputIdx + 1 ) < sm->numInputs ? "," : "" );
	}
	fprintf( fs, "\n" );
	fprintf( fs, "} %sInput;\n", sm->name );
	fprintf( fs, "\n" );

	for ( actionIdx = 1; actionIdx < sm->numActions; actionIdx++ )
	{
		fprintf( fs, "void %s%s( void );\n",
				 sm->actionPrefix != NULL ? sm->actionPrefix : "",
				 sm->action[ actionIdx ].name );
	}
	fprintf( fs, "\n" );

	fprintf( fs, "extern STM_StateMachine %sSm;\n", sm->name );
}

/************************************************************************
 *
 * Usage - prints out program usage
 *
 ************************************************************************/

void Usage( char *pgm )
{
	fprintf( stderr, "\nUsage: %s [-l] [-y] [-k] [-s] <fname[.smc]>\n\n", pgm );
	fprintf( stderr, "-k  Keep CPP file (fname.i)\n" );
	fprintf( stderr, "-l  Turn on LEX debug\n" );
	fprintf( stderr, "-s  Do not run CPP\n" );
	fprintf( stderr, "-y  Turn on YACC debug\n\n" );
	fprintf( stderr, "-Ixxx -Dxxx -Uxxx and +fname are passed through to "
					 "CPP\n" );

	exit( 1 );
}

/************************************************************************
 *
 * yyerror - report error encountered in yyparse()
 *
 ************************************************************************/

char	*last_yytext( void );	/* Found in smc.l */

void yyerror( char *str )
{
	fprintf( stderr, "ERROR: %s, file %s: line %d, near %s\n",
			 str, curr_fname, curr_lineno, last_yytext() );

	gParsedOk = FALSE;
}

/************************************************************************
 *
 * yygetc - called by yylex to get input characters
 *
 ************************************************************************/

int	yygetc( void )
{
	int		ln;
	char	*s1, *s2;

    if ( verbose )
    {
        Log( "yygetc called\n" );
    }
    if ( *curr_ptr == '\0' )
    {
        /*
         * We've run out of line, go and get another one
         */

        if ( fgets( line, sizeof( line ), curr_fs ) == NULL )
        {
            return -1;
        }

        /*
         * Skip over the filename
         */

        if ((( s1 = strchr( line, ' ' )) != NULL )
        &&  (( ln = (int)strtol( s1, &s2, 10 )) != 0 )
        &&	( *s2 == ':' ))
        {
            /*
             * The line looks like the right format use it
             */

            *s1 = '\0';
            curr_fname = line;
            curr_lineno = ln;
            curr_ptr = ++s2;
        }
        else
        {
            curr_ptr = line;
            curr_lineno = 0;
        }

        /*
         * Note: There is always at least 1 space after the ':'
         *		 so we don't have to fetch more than one line.
         */
    }
	return *curr_ptr++;
}

