#include <errno.h>

int kill( int pid, int sig )
{
    errno = ENOSYS;
    return -1;
}

int getpid( void )
{
    return 0;
}

int open( const char *name, int oflag, ... )
{
    errno = ENOSYS;
    return -1;
}
