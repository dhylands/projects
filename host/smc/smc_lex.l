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
*   @file   smc.l 
*
*   @brief  Implements the lexical anaylzer for the state machine compiler.
*
*****************************************************************************/

%{

#include <Log.h>

#include "smc_tab.h"

#define  YY_NO_UNISTD_H    // It contans isatty which causes a warning

/*
 * We want to use our own yygetc function (found in smc.c). Our function
 * knows how to strip the filename and line number from the beginning of
 * each line produced by the pre-processor.
 */

//#undef	yygetc

int   yygetc( void );

int   gDebugLex = 0;

#define	DBG(fmt,args...)		if ( gDebugLex )  Log( fmt, ## args );
						
#define	PARSE(x)	\
   DBG( "LEX: parsed " #x "\n" );\
	return x;
						
#define	PARSE_CHR(x) \
   DBG( "LEX: parsed %c\n", (x) );\
	return x;
						
#define	PARSE_TXT(x) \
   DBG( "LEX: parsed " #x ": %s\n", yytext );\
	return x;
						
void  yyerror( char *str );

// Adding the following prototypes gets rid of a bunch of compiler warnings

FILE *yyget_in ( void );
FILE *yyget_out( void );
int yyget_leng ( void );
char *yyget_text( void );
void yyset_in( FILE *  in_str );
void yyset_out( FILE *  out_str );
int yyget_debug( void );
void yyset_debug( int  bdebug );
int yylex_destroy( void );
char *last_yytext( void );

%}

%%

(A|a)ction(P|p)refix	{ PARSE(ACTION_PREFIX); }
(B|b)ranch				{ PARSE(BRANCH); }
(D|d)efault				{ PARSE(DEFAULT); }
(E|e)lse				   { PARSE(ELSE); }
(E|e)nter				{ PARSE(ENTER); }
(E|e)xit				   { PARSE(EXIT); }
(I|i)dle				   { PARSE(IDLE); }
(I|i)nput(s)?  		{ PARSE(INPUTS); }
(I|i)nput(P|p)refix	{ PARSE(INPUT_PREFIX); }
(I|i)nput(Q|q)ueue	{ PARSE(INPUT_QUEUE); }
(N|n)ame	   			{ PARSE(NAME); }
(N|n)ext	   			{ PARSE(NEXT); }
(N|n)o(B|b)ranch		{ PARSE(NO_BRANCH); }
(P|p)rev(ious)?		{ PARSE(PREVIOUS); }
(S|s)tate	   		{ PARSE(STATE); }
(S|s)tate(P|p)refix	{ PARSE(STATE_PREFIX); }
					 
[\[\]:]					{ PARSE_CHR(*yytext); }

[0-9]+					{
							   yylval.num = atol( (char *)yytext );
							   PARSE_TXT(NUMBER);
						   }

[_A-Za-z][_A-Za-z0-9]*	{
							   yylval.str = strdup( (char *)yytext );
							   PARSE_TXT(IDENTIFIER);
						   }

#.*						;	// Skip comments
;.*						;	// Skip comments
						
[ \t\n,]				   ;	/* Skip white space (we treat commas as white
							      space to make the parsing easier). */

						
.						   yyerror( "Unknown character" );

%%

//***************************************************************************
/**
*   Returns the yytext that was last read.
*/

char *last_yytext( void )
{
	return (char *)yytext;
}

//***************************************************************************
/**
*   Called at EOF to see if there are more files to process.
*
*  @return  1 if there is no further input, should return 0 if there is
*           further input available.
*/

int	yywrap( void )
{
	return 1;
}

// The following gets rid of a compiler warning about yyunput not being used.
void (*yyunput_ptr)(int c, register char * yy_bp ) = yyunput;

