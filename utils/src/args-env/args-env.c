#include <stdio.h>
#include <stdlib.h>

int main( int argc, char **argv, char **envp )
{
    int     i;
    char **env;

    for ( i = 0; i < argc; i++ ) 
    {
        printf( "argv[ %d ] = '%s'\n", i, argv[ i ] );
    }
    env = envp;
    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }

    return 0;
}
