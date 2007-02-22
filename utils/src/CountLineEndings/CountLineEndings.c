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
	int		ch;
	int		foundCr;
	int     lfCount;
	int     crCount;
	int     crLfCount;
	int     crcrLfCount;
	int     otherCount;

	if ( argc < 2 )
	{
		fprintf( stderr, "Usage: CountLineEndings file ...\n" );
		exit( 1 );
	}

	printf( "    LF     CR   CRLF CRCRLF  Other FileName\n" );
	printf( "------ ------ ------ ------ ------ ---------------------------\n" );

	for ( i = 1; i < argc; i++ )
	{
		if (( src_fs = fopen( argv[ i ], "rb" )) == NULL )
		{
			fprintf( stderr, "Unable to open file %s\n", argv[ i ] );
			exit( 1 );
		}

		foundCr = 0;
		lfCount = 0;
		crCount = 0;
		crLfCount = 0;
		crcrLfCount = 0;
		otherCount = 0;

		while (( ch = fgetc( src_fs )) != EOF )
		{
			if ( ch == 0x0D )
			{
				foundCr++;
				continue;
			}

			if ( ch == 0x0A )
			{
				switch ( foundCr )
				{
					case 0:     lfCount++;      break;
					case 1:     crLfCount++;    break;
					case 2:     crcrLfCount++;  break;
					default:    otherCount++;   break;
				}

				foundCr = 0;
				continue;
			}

			if ( foundCr > 0 )
			{
                crCount += foundCr;
                foundCr = 0;
			}
		}

		fclose(src_fs);

		printf( "%6d %6d %6d %6d %6d %s\n", lfCount, crCount, crLfCount, crcrLfCount, otherCount, argv[ i ] );
	}

	exit( 0 );
	return 0;
}

