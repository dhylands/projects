//
// This utility was used to recover photos from a formatted CF card 
// from my Canon Rebel XTi.
// 
// The command to copy the image off the camera was:
//     dd if=/dev/sdb1 of=CF.img blocksize-1024
// 
// And then run this utility on the CF.img file.

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

unsigned char buf[ 4096 ];

void ExtractImage( FILE *fs, int imgNum, long startOffset, long endOffset )
{
    char    imgFileName[ 20 ];
    FILE   *imgFs;
    size_t  bytesToRead;
    long    offset;

    printf( "ExtractImage %d 0x%08lx 0x%08lx\n", imgNum, startOffset, endOffset );

    snprintf( imgFileName, sizeof( imgFileName ), "rec_%04d.jpg", imgNum );

    if (( imgFs = fopen( imgFileName, "wb" )) == NULL )
    {
        fprintf( stderr, "Unable to open '%s' for writing\n", imgFileName );
        exit( 1 );
    }

    if ( fseek( fs, startOffset, SEEK_SET ) != 0 )
    {
        fprintf( stderr, "Seek to offset 0x%08lx failed\n", startOffset );
        exit( 1 );
    }

    offset = startOffset;
    while ( offset < endOffset )
    {
        if (( bytesToRead = ( endOffset - offset )) > sizeof( buf ))
        {
            bytesToRead = sizeof( buf );
        }
        if ( fread( buf, 1, bytesToRead, fs ) != bytesToRead )
        {
            fprintf( stderr, "Error reading %d bytes @ %08lx\n", bytesToRead, offset );
            exit( 1 );
        }
        if ( fwrite( buf, 1, bytesToRead, imgFs ) != bytesToRead )
        {
            fprintf( stderr, "Error writing %d bytes @ %08lx\n", bytesToRead, offset );
            exit( 1 );
        }

        offset += bytesToRead;
    }

    fclose( imgFs );
}

int main( int argc, char **argv )
{
    char    *fileName;
    FILE    *fs;
    int     imgNum = 0;
    struct stat sb;
    long    offset;
    long    prevOffset = 0;

    if ( argc != 2 )
    {
        fprintf( stderr, "Must specify a filename\n" );
        exit( 1 );
    }
    fileName = argv[ 1 ];

    if (( fs = fopen( fileName, "rb" )) == NULL )
    {
        fprintf( stderr, "Unable to open '%s' for reading\n", fileName );
        exit( 1 );
    }

    if ( fstat( fileno( fs ), &sb ) != 0 )
    {
        perror( "fstat" );
        exit( 1 );
    }

    offset = 0;
    while ( offset < sb.st_size )
    {
        fseek( fs, offset, SEEK_SET );
        if ( fread( buf, 10, 1, fs ) != 1 )
        {
            fprintf( stderr, "Error reading 10 bytes from offset: %lx\n", offset );
            exit( 1 );
        }
        if ( memcmp( &buf[6], "Exif", 4 ) == 0 )
        {
            imgNum++;

            if ( prevOffset > 0 )
            {
                ExtractImage( fs, imgNum, prevOffset, offset );
            }

            printf( "Detected Exif %d @ 0x%08lx\n", imgNum, offset );

            prevOffset = offset;
        }
        offset += 512;
    }
    imgNum++;
    if ( prevOffset > 0 )
    {
        ExtractImage( fs, imgNum, prevOffset, sb.st_size );
    }

    fclose( fs );

    exit( 0 );
    return 0;
}
