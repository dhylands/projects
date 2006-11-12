/***************************************************************************
*
*     Copyright (c) 2002 by Dave Hylands
*           All Rights Reserved
*
*	Permission is granted to any individual or institution to use, copy, 
*  modify, or redistribute this file so long as it is not sold for profit, 
*  and that this copyright notice is retained.
*
***************************************************************************
*
*  This program translates all line-endings (DOS, UNIX or Mac) into DOS
*  style line endings.
*
*  Usage:   2dos file ...
*
*  Currently it does not accept wildcards (I use bash which does the 
*  wildcard expansion for me).
*
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define	TRUE	1
#define	FALSE	0

int main( int argc, char **argv )
{
	int		i;
	FILE	*src_fs;
	FILE	*dst_fs;
	int		ch;
	int		foundCr = FALSE;

	if ( argc < 2 )
	{
		fprintf( stderr, "Usage: 2dos file ...\n" );
		exit( 1 );
	}

	for ( i = 1; i < argc; i++ )
	{
		if (( src_fs = fopen( argv[ i ], "rb" )) == NULL )
		{
			fprintf( stderr, "Unable to open file %s\n", argv[ i ] );
			exit( 1 );
		}
		if (( dst_fs = fopen( "2dos.tmp", "wb" )) == NULL )
		{
			fprintf( stderr, "Unable to open 2dos.tmp for output\n" );
			exit( 1 );
		}

		/*
		 * We need to translate 0x0D 0x0A into 0x0D 0x0A
		 * We need to translate 0x0D      into 0x0D 0x0A
		 * We need to translate 0x0A	  into 0x0D 0x0A
		 */

		while (( ch = fgetc( src_fs )) != EOF )
		{
			if ( foundCr )
			{
				fputc( 0x0D, dst_fs );
				fputc( 0x0A, dst_fs );

				switch ( ch )
				{
					case 0x0D:
					{
						break;
					}
					case 0x0A:
					{
						foundCr = FALSE;
						break;
					}
					default:
					{
						foundCr = FALSE;
						fputc( ch, dst_fs );
						break;
					}
				}
			}
			else
			{
				switch ( ch )
				{
					case 0x0D:
					{
						foundCr = TRUE;
						break;
					}

					case 0x0A:
					{
						fputc( 0x0D, dst_fs );
						fputc( 0x0A, dst_fs );
						break;
					}
					default:
					{
						fputc( ch, dst_fs );
						break;
					}
				}
			}
		}

		fclose(src_fs);
		fclose(dst_fs);

		/* Delete the original file & rename unix2dos.tmp */

		if ( remove( argv[ i ] ) != 0 )
		{
			perror( argv[ i ] );
			exit( 1 );
		}
		if ( rename( "2dos.tmp", argv[ i ] ) != 0 )
		{
			perror( argv[ i ] );
			exit( 1 );
		}
	}

	exit( 0 );
	return 0;
}

