#include <stdio.h>
#include <dirent.h>

int main( int argc, char **argv )
{
    int arg;

    for ( arg = 1; arg < argc; arg++ )
    {
        DIR *d;

        if (( d = opendir( argv[arg] )) == NULL )
        {
            fprintf( stderr, "opendir( '%s' ) failed\n", argv[arg] );
        }
        else
        {
            struct dirent *de;

            while (( de = readdir( d )) != NULL )
            {
                printf( "name '%s' type 0x%08x\n", de->d_name, de->d_type );
            }
            closedir( d );
        }
    }

    return 0;
}
