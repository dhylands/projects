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
*   @file   pthread-test.c 
*
*   @brief  Test program for playing around with pthreads
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

static              sigset_t    gTermSig;
static  volatile    int         gQuit = 0;
static              pthread_t   gSignalTid;
static              pthread_t   gSleeperTid;

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

//***************************************************************************
/**
*   This thread waits around for a signal to occur and tells the main thread
*   to quit.
*/

static void *SignalThread( void *param )
{
    int         err;
    int         sig;
    sigset_t    sigs;
    sigset_t    oldSigs;

    (void)param;

    sigfillset( &sigs );
    pthread_sigmask( SIG_BLOCK, &sigs, &oldSigs );

    if (( err = sigwait( &gTermSig, &sig )) != 0 )
    {
        fprintf( stderr, "SignalThread: sigwait failed\n" );
    }
    else
    {
        fprintf( stderr, "Terminating signal caught...\n" );
        gQuit = 1;

        pthread_cancel( gSleeperTid );
    }

    return NULL;

} // SignalThread

//***************************************************************************
/**
*   Cleanup thread called when the SleeperThread exits
*/

static void SleeperCleanup( void *param )
{
    (void)param;

    fprintf( stderr, "SleeperCleanup called\n" );

} // SleeperCleanup

//***************************************************************************
/**
*   This thread sleeps for a long time.
*/

static void *SleeperThread( void *param )
{
    (void)param;

    pthread_cleanup_push( SleeperCleanup, NULL );

    while ( 1 )
    {
        fprintf( stderr, "SleeperThread: About to sleep for 1 seconds\n" );
        sleep( 1 );
        fprintf( stderr, "SleeperThread: Finished sleeping, gQuit: %d\n", gQuit );
    }

    pthread_cleanup_pop( 1 );

    return NULL;

} // SleeperThread

//***************************************************************************
/**
*   Main loop
*/

int main( int argc, char **argv )
{
    int     sig;
    int     err;

    (void)argc;
    (void)argv;

    sigemptyset( &gTermSig );
    sigaddset( &gTermSig, SIGINT );
    sigaddset( &gTermSig, SIGTERM );
    
    pthread_sigmask( SIG_BLOCK, &gTermSig, NULL );

    //pthread_create( &gSignalTid, NULL, SignalThread, NULL );
    pthread_create( &gSleeperTid, NULL, SleeperThread, NULL );

    if (( err = sigwait( &gTermSig, &sig )) != 0 )
    {
        fprintf( stderr, "SignalThread: sigwait failed\n" );
    }
    else
    {
        fprintf( stderr, "Terminating signal caught...\n" );
        gQuit = 1;

        pthread_cancel( gSleeperTid );
    }

    pthread_join( gSleeperTid, NULL );

    fprintf( stderr, "main: Exiting\n" );

    return 0;

} // main


