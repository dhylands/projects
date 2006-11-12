#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <sys\stat.h>
#include <string.h>
#include "GetOpt.h"

#define	LINE_WIDTH	16

typedef unsigned char byte;
typedef byte          boolean;

#define	TRUE	1
#define	FALSE	0

void    dump_file( FILE *, char *, long, long );
void    print_help( void );


/************************************************************************
 *   Synopsis:
 *	int main( argc, argv )
 *
 *   Purpose:
 *	This routine is the driver for dumping files. If no files are
 *	specified then stdin is assumed.
 *
 *   Parameters:
 *	int	argc	: (in) Number of arguments
 *	char	**argv	: (in) Array of ptrs to arguments
 *
 *   Values Returned:
 *	0		: always
 *
 *   Date Created	: Feb 13, 1991
 *   Programmer		: Dave Hylands2
 *
 *   Modified		: August 4, 1994
 *   Programmer		: Leila Hawkins
 *   Modifications	: Added parsing for S, E, and ? switches.
 *
 ***********************************************************************/

int main( argc, argv )
int argc;
char    **argv;
{
	char   *fname = NULL;
	int		arg;
	FILE   *fs;
	long    s_offset = 0L;
	long    e_offset = -1L;
	long	length = -1L;

	while (( arg = getopt( argc, argv, "e:E:l:L:s:S:hH" )) != -1 )
	{
		switch ( arg )
		{
			case 'e':	
			case 'E':
			{
				e_offset = strtol( optarg, NULL, 0 );
				break;
			}

			case 'l':	
			case 'L':
			{
				length = strtol( optarg, NULL, 0 );
				break;
			}

			case 's':	
			case 'S':
			{
				s_offset = strtol( optarg, NULL, 0 );
				break;
			}

			case 'h':	
			case 'H':
			default:
			{
				print_help();
				exit( 1 );
				break;
			}

		}
	}

	if (( e_offset >= 0 ) && ( length >= 0 ))
	{
		fprintf( stderr, "Can't specify -e and -l together\n" );
		print_help();
		exit( 1 );
	}
	if ( length >= 0 )
	{
		e_offset = s_offset + length;
	}

	if ( optind >= argc )
	{
		// No input file specified, use stdin

		dump_file( stdin, "stdin", s_offset, e_offset );
	}
	else
	{
		for (; optind < argc; optind++ )
		{
			fname = argv[ optind ];

			if (( fs = fopen( fname, "r" )) == NULL )
			{
				perror( fname );
				fprintf( stderr, "Unable to open file: %s\n", fname );
				exit( 1 );
			}
			dump_file( fs, fname, s_offset, e_offset );
			fclose( fs );
		}
	}

	exit( 0 );
	return( 0 );
}


/************************************************************************
 *   Synopsis:
 *      void	print_help( void )
 *
 *   Purpose:
 *      This routine prints help information to the screen in response
 *      to the /? switch.
 *
 *   Parameters:
 *      none
 *
 *   Values Returned:
 *      none
 *
 *   Date Created	: August 3, 1994
 *   Programmer		: Leila Hawkins
 *
 ***********************************************************************/

void    print_help( void )
{
	fprintf( stderr, "Starts the hex dump utility.\n");
	fprintf( stderr, "\n" );
	fprintf( stderr, "HD [-s offset] [-l length] [-e offset] filename\n");
	fprintf( stderr, "\n" );
	fprintf( stderr, "  -s offset   Specifies an offset into the file to start at\n");
	fprintf( stderr, "  -l length   Specifies the number of bytes to dump\n" );
	fprintf( stderr, "  -e offset   Specifies an offset to end at\n");
	fprintf( stderr, "\n" );
	fprintf( stderr, "  filename    Specifies the name of the file to dump\n");
	fprintf( stderr, "  If no filename is given, stdin is assumed and any offsets\n");
	fprintf( stderr, "  specified are ignored.\n");
	fprintf( stderr, "\n" );
	fprintf( stderr, "  The offsets may be specified in hex, octal, or in decimal.\n");
	fprintf( stderr, "  Hex offsets must be prefixed by 0x, the octal prefix is 0\n");
	fprintf( stderr, "  (without the X), and decimal offsets require no prefix.\n");
	fprintf( stderr, "\n" );
	fprintf( stderr, "  The dump begins on the byte specified by \"from\" and ends\n");
	fprintf( stderr, "  on the byte prior to that specified by \"to\" or the end \n");
	fprintf( stderr, "  of the file -- whichever comes first.\n");
}

/************************************************************************
 *   Synopsis:
 *	void	dump_file( fs, fname, s_offset, e_offset )
 *
 *   Purpose:
 *	This routine dumps the contents of the file stream fs using a
 *	header indicated by fname.
 *
 *   Parameters:
 *	FILE	*fs	  : (in) File stream to dump
 *	char	*fname	  : (in) File name to use in the header
 *      long    s_offset  : (in) Offset to start at in the file
 *      long    e_offset  : (in) Offset to end at in the file
 *
 *   Values Returned:
 *	none
 *
 *   Date Created	: Feb 13, 1991
 *   Programmer		: Dave Hylands2
 *
 *   Modified		: Aug 4, 1994
 *   Programmer		: Leila Hawkins
 *   Modifications	: Added the s_offset and e_offset functionality.
 *
 ***********************************************************************/

void    dump_file( fs, fname, s_offset, e_offset )
FILE    *fs;
char    *fname;
long    s_offset;
long    e_offset;
{
	long		addr;
	int         cnt;
	int         i;
	char        line[LINE_WIDTH];
	int         val;
	char        prev_line[LINE_WIDTH];
	boolean     same_as;
	long        cur_offset;

	cur_offset = s_offset;
	addr = s_offset;
	same_as = FALSE;

	printf( "File: %s\n\n", fname );
	if ( strcmp( fname, "stdin" ) == 0 )
	{
		for ( cur_offset = 0; cur_offset < s_offset; cur_offset++ )
		{
			if ( getc( fs ) == EOF )
			{
				fprintf( stderr, "stdin didn't contain %ld characters\n", s_offset );
				return;
			}
		}
	}
	{
		fseek( fs, s_offset, SEEK_SET );
		if ( ftell( fs ) != s_offset )
		{
			(void) fprintf( stderr,
							"Could not go to start offset: %ld\n", s_offset );
			return;
		}
	}

	do
	{
		for ( cnt = 0; cnt < LINE_WIDTH; cnt++, cur_offset++ )
		{
			if ((( val = getc( fs )) == EOF ) || ( cur_offset == e_offset ))
			{
				break;
			}
			line[cnt] = val;
		}
		if (( cnt == LINE_WIDTH ) && ( addr > s_offset ))
		{
			/*
			 *  Check to see if this line is the same as the previous
			 */

			if( memcmp( line, prev_line, LINE_WIDTH ) == 0 )
			{
				if( !same_as )
				{
					same_as = TRUE;
					(void) printf(
								 "        ***** Same as previous line *****\n" );
				}
				addr += cnt;
				continue;
			}
		}
		same_as = FALSE;
		(void) memcpy( prev_line, line, LINE_WIDTH );
		(void) printf( "%6.6lX: ", addr );
		for( i = 0; i < LINE_WIDTH; i++ )
		{
			if( i < cnt )
			{
				(void) printf( "%2.2X ", (unsigned char) line[i] );
			}
			else
			{
				(void) printf( "   " );
			}
		}
		for( i = 0; i < cnt; i++ )
		{
			if( ( line[i] < ' ' ) || ( line[i] > '~' ) )
			{
				putchar( '.' );
			}
			else
			{
				putchar( line[i] );
			}
		}
		putchar( '\n' );
		addr += cnt;
	} while( !feof( fs ) && (cur_offset != e_offset) );
}
