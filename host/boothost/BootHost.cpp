/****************************************************************************
*
*     Copyright (c) 2005 Dave Hylands
*           All Rights Reserved
*
*   Permission is granted to any individual or institution to use, copy, or
*   redistribute this software so long as it is not sold for profit, and that
*   this copyright notice is retained.
*
****************************************************************************/
/**
*
*   @file   BootHost.cpp
*
*   @brief  Implements the i2c BootLoader Host
*
****************************************************************************/
/**
*   @defgroup   BootHost   i2c BootLoader Host.
*
*   @brief      BootLoad Host program for downloading to target via i2c.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <pthread.h>
#include <getopt.h>
#include <signal.h>

// unix is defined for cywin builds as well as linux builds
                 
#if defined( unix )
#include <sys/ioctl.h>
#include <termios.h>
#endif

//#include "BootLoader.h"
#include "DumpMem.h"
#include "SerialPort.h"
#include "ReadFile.h"
#include "State.h"
#include "StateAVReflash.h"
#include "StateMegaLoad.h"
#include "Log.h"

#include "StateSTK500.h"
#include "stk500-command.h"

#define USE_I2C 0

#if USE_I2C
#include "I2C_BootLoader.h"
#include "I2C_SerialDongle.h"
#endif

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

// ---- Private Variables ---------------------------------------------------

enum
{
    OPT_DEBUG_DONGLE    = 0x80,
    OPT_DEBUG_DONGLE_DATA,
};

struct option gLongOption[] =
{
    // option               A  Flag   V  (has_arg, flag, val)
    // -------------------  -  ----  ---
    { "baud",               1, NULL, 'b' },
    { "debug",              0, NULL, 'd' },
#if USE_I2C
    { "debug-dongle",       0, NULL, OPT_DEBUG_DONGLE },
    { "debug-dongle-data",  0, NULL, OPT_DEBUG_DONGLE_DATA },
    { "dongle",             0, NULL, 'g' }, // Use Serial <==> I2C dongle
#endif
    { "help",               0, NULL, 'h' },
    { "megaload",           0, NULL, 'm' }, // Use MegaLoad protocol
    { "port",               1, NULL, 'p' },
    { "rts-reset",          0, NULL, 'r' }, // Use RTS as reset for device
    { "stk500",             0, NULL, 's' }, // Use STK500 protocol
    { "verbose",            0, NULL, 'v' },
    { 0 },

};

bool gMegaLoad = false;
bool gStk500 = true;
bool gDongle = false;
bool gBootLoadAllowed = true;
bool gUseRtsToReset = false;

// We can program the serial <=> i2c dongle, which is connected to the
// serial port, or we can program a device on the i2c bus. To avoid confusion
// we turn off some reporting if we don't intend to program the dongle.

bool gProgramDongle = true;

SerialPort          gSerialPort;

#if USE_I2C
I2C_SerialDongle    gSerialDongle;
unsigned char       gSerialDongleAddr = 0;
unsigned char       gSlaveAddr = BL_DEFAULT_I2C_ADDR;
#endif

DeviceInfo      gDeviceInfo;
const char     *gDownloadFileName = NULL;
FileInfo       *gDownloadInfo = NULL;
unsigned        gRetryCount = 0;
int             gCounter = 111;
volatile bool   gKeyboardInputReceived = false;
volatile bool   gReadSerialThreadRunning = false;

// Used by 
FileSegment    *gDownloadSegment = NULL;
unsigned        gDownloadOffset;

FILE           *gLogFs2;

volatile bool   gQuit = 0;

#if defined( unix )
struct termios gTio_org;
#endif

// ---- Private Function Prototypes -----------------------------------------

void   *ReadSerialThread( void *param );
void   *ReadStdinThread( void *param );
void    Usage();

void LogUnrecognizedChar( char ch, const char *label );

#if USE_I2C
void PacketTextChar( unsigned char ch, void *userData );
void PacketSendChar( unsigned char ch, void *userData );
void PacketReceived( PKT_Packet *packet, void *userData );
#endif

void SendByte( unsigned char ch );

// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup BootHost
 * @{
 */

#if 0
//***************************************************************************
/**
*   ControlC
*/

void ControlC( int sigNum )
{
    fprintf( stderr, "Shutting down...\n" );

    // Closing the serial port will cause the ReadSerialThread to unblock and exit

    gSerialPort.Close();

    if ( tcsetattr( fileno( stdin ), TCSANOW, &gTio_org ) == -1 )
    {
        LogError( "Unable to restore terminal settings\n" );
    }

    if ( gLogFs2 != NULL )
    {
        fclose( gLogFs2 );
    }

    // Now do whatever the default handling for the signal would be

    signal( sigNum, SIG_DFL );
    raise( sigNum );

} // ControlC

#endif

//***************************************************************************
/**
*   main loop
*/

int main( int argc, char **argv )
{
    int         sig;
    sigset_t    termSig;
    pthread_t   readSerialThreadId;
    pthread_t   readStdinThreadId;
    int         rc;
    int         opt;
    const char *baudStr = NULL;
#if defined( linux )
    const char *portStr = "ttyS0";
#else
    const char *portStr = "com1";
#endif

#if USE_I2C
    PKT_TextChar        = PacketTextChar;
    PKT_SendChar        = PacketSendChar;
    PKT_PacketReceived  = PacketReceived;
#endif

//    signal( SIGINT, ControlC );
//    signal( SIGTERM, ControlC );

    LogInit( stdout );

    while (( opt = getopt_long( argc, argv, "b:dhmp:sv", gLongOption, NULL )) > 0 )
    {
        switch ( opt )
        {
            case 'b':
            {
                baudStr = optarg;
                break;
            }

            case 'd':
            {
                gDebug = true;
                break;
            }

            case 'g':
            {
                gDongle = true;
                break;
            }

            case 'm':
            {
                gMegaLoad = true;
                break;
            }

            case 'p':
            {
                portStr = optarg;
                break;
            }

            case 'r':
            {
                gUseRtsToReset = true;
                break;
            }

            case 's':
            {
                gStk500 = true;
                break;
            }

            case 'v':
            {
                gVerbose = true;
                break;
            }

#if USE_I2C
            case OPT_DEBUG_DONGLE:
            {
                gSerialDongle.m_debugDongle = true;
                break;
            }

            case OPT_DEBUG_DONGLE_DATA:
            {
                gSerialDongle.m_debugDongleData = true;
                break;
            }
#endif

            case '?':
            case 'h':
            {
                Usage();
                return 1;
            }
        }
    }

    if ( optind < argc )
    {
        if (( optind + 1 ) != argc )
        {
            fprintf( stderr, "Only one download file supported\n" );
            return 1;
        }

        gDownloadFileName = argv[ optind ];
    }

    // Open the file to download

    if ( gDownloadFileName != NULL )
    {
        // If we are asked to download a file, then read the entire file
        // into memory.

        if (( gDownloadInfo = ReadFile( gDownloadFileName )) == NULL )
        {
            return 1;
        }
    }

    if ( !gSerialPort.Open( portStr, baudStr ))
    {
        return 1;
    }
    gSerialPort.UseRTStoReset( gUseRtsToReset );
    gSerialPort.ResetTarget();

    // Put stdin in raw mode

    setbuf( stdin, NULL );
    setbuf( stdout, NULL );

#if defined( unix )
    sigemptyset( &termSig );
    sigaddset( &termSig, SIGINT );
    sigaddset( &termSig, SIGTERM );

    pthread_sigmask( SIG_BLOCK, &termSig, NULL );

    struct termios tio_new;

    if ( tcgetattr( fileno( stdin ), &gTio_org ) < 0 )
    {
        LogError( "Unable to retrieve terminal settings\n" );
        return 1;
    }
    
    tio_new = gTio_org;
    tio_new.c_lflag &= ~( ICANON | ECHO );
    tio_new.c_cc[VMIN] = 1;
    tio_new.c_cc[VTIME] = 0;

    if ( tcsetattr( fileno( stdin ), TCSANOW, &tio_new ) < 0 )
    {
        LogError( "Unable to update terminal settings\n" );
        return 1;
    }
#endif

    const char *bootLoaderType = "*** Unknown ***";

    if ( gDongle )
    {
        bootLoaderType = "Serial Dongle";
    }
    else
    if ( gMegaLoad )
    {
        bootLoaderType = "MegaLoad v2.3";
    }
    else
    if ( gStk500 )
    {
        bootLoaderType = "STK500";
    }

    gLogFs2 = fopen( "BootHost.log", "wb" );

    Log( "BootHost - BootLoader: %s\n", bootLoaderType );

    // Kick off the serial port reader thread.

    rc = pthread_create( &readSerialThreadId, NULL, ReadSerialThread, &gSerialPort );
    if ( rc != 0 )
    {
        fprintf( stderr, "Error creating ReadSerialThread: %d\n", rc );
        return 1;
    }

    // Kick off the stdin reader thread.

    rc = pthread_create( &readStdinThreadId, NULL, ReadStdinThread, NULL );
    if ( rc != 0 )
    {
        fprintf( stderr, "Error creating ReadSerialThread: %d\n", rc );
        return 1;
    }

#if defined( unix )

    // Wait for a termmination signal

    if (( rc = sigwait( &termSig, &sig )) != 0 )
    {
        fprintf( stderr, "sigwait failed\n" );
    }
    else
    {
        fprintf( stderr, "Exiting...\n" );
    }

    pthread_cancel( readSerialThreadId );
    pthread_cancel( readStdinThreadId );

    // Restore stdin back to the way it was when we started

    if ( tcsetattr( fileno( stdin ), TCSANOW, &gTio_org ) == -1 )
    {
        LogError( "Unable to restore terminal settings\n" );
    }
#endif

#if defined( __CYGWIN__ )

    // Under Windows closing the serial port and stdin will cause the reads
    // to unblock. Under linux, this isn't required, but it doesn't hurt 
    // either.

    gSerialPort.Close();
    fclose( stdin );
#endif

    // Unblock the termination signals so the user can kill us if we hang up
    // waiting for the reader threads to exit.

#if defined( unix )
    pthread_sigmask( SIG_UNBLOCK, &termSig, NULL );
#endif

    pthread_join( readSerialThreadId, NULL );
    pthread_join( readStdinThreadId, NULL );

#if !defined( __CYGWIN__ )
    gSerialPort.Close();
    fclose( stdin );
#endif

    if ( gVerbose )
    {
        printf( "Done\n" );
    }

    return 0;

} // main

/***************************************************************************/
/**
*   Checks the file information to see if the file will fit on the device.
*/

bool CheckFile( FileInfo *fileInfo, DeviceInfo *devInfo )
{
    unsigned    devEndAddr;

    devEndAddr = devInfo->flashSize - devInfo->bootSize;

    FileSegment *segment = fileInfo->head;
    while ( segment != NULL )
    {
        if (( segment->address + segment->lenThisSegment ) > devEndAddr )
        {
            LogError( "File '%s' is too big to fit\n", fileInfo->fileName );
            return false;
        }
        segment = segment->next;
    }

    return true;

} // CheckFile

/***************************************************************************/
/**
*   Logs an unrecognized character while processing in the state machine
*/

void LogUnrecognizedChar( char ch, const char *label )
{
    LogError( "Unrecognized character received: 0x%02x while waiting for %s\n", ch, label );

} // LogUnrecognizedChar

#if USE_I2C
/***************************************************************************/
/**
*   Callback which is called when a text char is received on the serial port.
*/

void PacketTextChar( unsigned char ch, void *userData )
{
    (void)userData;

    Log( "\033[1;32m%c\033[0m", ch );

} // PacketTextChar

/***************************************************************************/
/**
*   Callback which is called when a text char is received on the serial port.
*/

void PacketSendChar( unsigned char ch, void *userData )
{
    if ( gDebug )
    {
        Log( "Sending: 0x%02x\n", ch );
    }
    SendByte( ch );

} // PacketSendChar

/***************************************************************************/
/**
*   Callback which is called when a packet is received on the serial port.
*/

void PacketReceived( PKT_Packet *packet, void *userData )
{
    gSerialDongle.PacketReceived( packet );

} // PacketReceived
#endif

/***************************************************************************/
/**
*   Thread which processes the keyboard data
*/

#if 0
void *KeyboardThread( void *param )
{


} // KeyboardThread
#endif

/***************************************************************************/
/**
*   Thread which processes the incoming serial data.
*/

void *ReadSerialThread( void *param )
{
    SerialPort *serialPort = (SerialPort *)param;

    char    buf[ 256 ];

    StatePtrProxy   state = NULL;

    LogDebug( "ReadSerialThread started\n" );
    gReadSerialThreadRunning = true;

    if ( gDongle )
    {
        state = StateLog;
    }
    else
    if ( gStk500 )
    {
        state = StartSTK500();
    }
    else
    {
        state = StateDownload;
    }

    while ( 1 )
    {
        size_t  bytesRead = serialPort->Read( buf, sizeof( buf ));

        if ( bytesRead == 0 )
        {
            break;
        }

        for ( unsigned i = 0; i < bytesRead; i++ ) 
        {
#if 1
            if ( gDebug )
            {
                unsigned char ch = buf[ i ];

                if (( ch < ' ' ) || ( ch > '~' ))
                {
                    Log( "BH-Rcvd: 0x%02x '.'\n", ch );
                }
                else
                {
                    Log( "BH-Rcvd: 0x%02x '%c'\n", ch, ch );
                }

            }
#endif
            if (( state == NULL ) || gKeyboardInputReceived )
            {
                state = StateLog;
            }
            state = state( buf[ i ]);
        }
    }

    LogDebug( "ReadSerialThread ending\n" );
    gReadSerialThreadRunning = false;

    return 0;

} // ReadSerialThread

/***************************************************************************/
/**
*   Thread which processes the incoming serial data.
*/

void *ReadStdinThread( void *param )
{
    (void)param;

    while ( 1 )
    {
        char    ch;

        if ( read( fileno( stdin ), &ch, 1 ) < 1 )
        {
            LogDebug( "read of stdin failed\n" );
            break;
        }

        gKeyboardInputReceived = true;

#if USE_I2C
        if ( gDongle )
        {
            if ( gSerialDongleAddr == 0 )
            {
                Log( "Retrieving i2c address of serial dongle ...\n" );
                if ( !gSerialDongle.GetI2CAddress( &gSerialDongleAddr ))
                {
                    LogError( "Unable to retrieve serial dongle i2c address\n" );
                    break;
                }
                Log( "    Serial Dongle i2c Address: 0x%02x\n", gSerialDongleAddr );
            }

            switch ( ch )
            {
                case 'c':
                {
                    LogDebug( "Call\n" );

                    I2C_Adapter::Buffer  writeBuf;
                    I2C_Adapter::Buffer  readBuf;

                    char    writeMem[ 40 ];
                    char    readMem[ 40 ];

                    writeMem[ 0 ] = gSerialDongleAddr;
                    sprintf( &writeMem[ 1 ], "-c-%d-c-", gCounter++ );

                    writeBuf.data = (unsigned char *)writeMem;
                    writeBuf.dataLen = strlen( writeMem ) + 2;  // +1 for i2c addr +1 for terminating null

                    readBuf.data = (unsigned char *)readMem;
                    readBuf.dataLen = sizeof( readMem );

                    if ( gSerialDongle.Call( gSlaveAddr, BL_CMD_TEST_CALL, &writeBuf, &readBuf ))
                    {
                        printf( "Call: Wrote: '%s' to 0x%02x, read '%s' from 0x%02x\n", &writeMem[ 1 ], writeMem[ 0 ], &readMem[ 1 ], readMem[ 0 ] );
                    }
                    else
                    {
                        printf( "Call: Wrote: '%s' to 0x%02x, read failed\n", &writeMem[ 1 ], writeMem[ 0 ] );
                    }
                    break;
                }

                case 'd':
                {
                    LogDebug( "Download\n" );

                    if ( gDownloadFileName == NULL )
                    {
                        LogError( "No filename specified to download\n" );
                        break;
                    }

                    I2C_BootLoader bootLoader( &gSerialDongle, gSlaveAddr );

                    if ( !bootLoader.DownloadFile( gDownloadInfo ))
                    {
                        LogError( "Unable to download file\n" );
                    }
                    break;
                }

                case 'i':
                {
                    LogDebug( "GetBootInfo\n" );

                    I2C_BootLoader bootLoader( &gSerialDongle, gSlaveAddr );

                    BootLoaderInfo_t    bootInfo;

                    if ( bootLoader.GetBootLoaderInfo( &bootInfo ))
                    {
                        bootLoader.PrintBootLoaderInfo( bootInfo );
                    }
                    else
                    {
                        LogError( "Unable to retrieve bootloader info\n" );
                    }
                    break;
                }

                case 'r':
                {
                    LogDebug( "Read\n" );

                    I2C_Adapter::Buffer readBuf;
                    char                readMem[ 40 ];

                    readBuf.data = (unsigned char *)readMem;
                    readBuf.dataLen = sizeof( readMem );

                    if ( gSerialDongle.Read( gSlaveAddr, BL_CMD_TEST_READ, &readBuf ))
                    {
                        printf( "Read: '%s' from 0x%02x\n", &readMem[ 1 ], readMem[ 0 ] );
                    }
                    else
                    {
                        printf( "Read: failed\n" );
                    }
                    break;
                }

                case 'w':
                {
                    LogDebug( "Write\n" );

                    I2C_Adapter::Buffer writeBuf;
                    char                writeMem[ 40 ];

                    writeMem[ 0 ] = gSerialDongleAddr;
                    sprintf( &writeMem[ 1 ], "-w-%d-w-", gCounter++ );

                    writeBuf.data = (unsigned char *)writeMem;
                    writeBuf.dataLen = strlen( writeMem ) + 2;  // +1 for i2c addr, +1 for temrinating null

                    printf( "Write: Wrote: '%s' to 0x%02x\n", &writeMem[ 1 ], writeMem[ 0 ] );

                    if ( !gSerialDongle.Write( gSlaveAddr, BL_CMD_TEST_WRITE, &writeBuf ))
                    {
                        printf( "Write: failed\n" );
                    }
                    break;
                }

                default:
                {
                    printf( "Rcvd: 0x%02x\n", ch );

                    //gSerialPort.Write( &ch, 1 );
                    break;
                }
            }
        }
        else
#endif
        {
            gSerialPort.Write( &ch, 1 );
        }
    }

} // ReadStdinThread

/***************************************************************************/
/**
*   Sends a character out the serial port.
*/

void SendByte( unsigned char ch )
{
    if ( gDebug )
    {
        Log( "Sending: 0x%02x\n", ch );
    }
    gSerialPort.Write( &ch, 1 );

} // SendByte

void SendData( const void *buf, size_t bytesToWrite )
{
    if ( gDebug )
    {
        DumpMem( "Send", 0, buf, bytesToWrite );
    }
    gSerialPort.Write( buf, bytesToWrite );

} // SendData

/***************************************************************************/
/**
*   Normal starting state. We log each character received, unless it looks
*   like the start of a packet, or a MegaLoad boot request.
*/

StatePtrProxy StateDownload( char ch )
{
    if ( gBootLoadAllowed )
    {
        if ( ch == '>' )
        {
            SendByte( '<' );
            gBootLoadAllowed = false;
            memset( &gDeviceInfo, 0, sizeof( gDeviceInfo ));

            if ( gMegaLoad )
            {
                return StateMegaDeviceId;
            }

            return StateAVReflashPageSize;
        }

        // Log the character, but stay in the download state

        StateLog( ch );

        return StateDownload;
    }

    // Switch over to logging

    return StateLog( ch );

} // StateDownload

/***************************************************************************/
/**
*   Just performs logging.
*/

StatePtrProxy StateLog( char ch )
{
#if USE_I2C
    PKT_ProcessChar( ch, NULL );
#else

    if ( ch != '\0' )
    {
        Log( "\033[1;32m%c\033[0m", ch );
    }
#endif

    if ( gLogFs2 != NULL )
    {
        fputc( ch, gLogFs2 );
    }

    return StateLog;

} // StateLog

/***************************************************************************/
/**
*   Print out the program usage.
*/

void Usage()
{
    fprintf( stderr, "Usage: BootHost <option(s)> <file>\n" );
    fprintf( stderr, "  Download a program via serial/i2c\n" );
    fprintf( stderr, "\n" );
    fprintf( stderr, "  -b, --baud=baud   Set the baudrate used\n" );
    fprintf( stderr, "  -h, --help        Display this message\n" );
    fprintf( stderr, "  -m, --megaload    Use megaload 2.3 protocol\n" );
    fprintf( stderr, "  -s, --stk500      Use stk500 bootloader protocol (default)\n" );
    fprintf( stderr, "  -v, --verbose     Turn on verbose messages\n" );

} // Usage

/** @} */

