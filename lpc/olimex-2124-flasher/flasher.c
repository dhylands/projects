/**************************** test10.c **********************************/
/* Copyright 2004/07/07 Aeolus Development				*/
/*									*/
/* Example program.  Feel free to copy and modify as desired.  Please	*/
/* remove Aeolus Development copyright from any modifications (although	*/
/* references to source of the original copy may remain).		*/
/*									*/
/*  Simple echo and timer test program.  Similar to test2 but uses the	*/
/* interrupt driven serial I/O driver.					*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 test10.c 14-Jul-2004,09:48:28,`RADSETT' Original archived version.
*   2 test10.c 12-Mar-2006,12:27:22,`RADSETT' Make echo a little cleaner to make
*        high volume echo test easier.
*   TLIB revision history ends.
*/

#include <stdio.h>
#include <errno.h>
#include <arch/philips/newlib-lpc/lpc210x.h>
#include <arch/philips/newlib-lpc/dev_cntrl.h>
#include <arch/philips/newlib-lpc/lpc_ioctl.h>

#define YELLOW_LED  ( 1 << 12 )
#define GREEN_LED   ( 1 << 13 )

	/**** Device table.  List of device drivers for newlib.	****/
const struct device_table_entry *device_table[] = {
	&com1_int,	/* stdin  */
	&com1_int,	/* stdout */
	&com1_int,	/* stderr */
	0 };		/* end of list */


/********************* main *********************************************/
/*  main -- program start point.  Simple echo program with some timing	*/
/* tests thrown in.	  						*/
int main( void)
 {
 struct serial_param sp;
 struct interrupt_param irq;


 (void)SetNativeSpeed( 14745uL);

	/* Desired serial line characteristics 9600,n82			*/
 sp.baud = 38400uL;
 sp.length = UART_WORD_LEN_8;
 sp.parity = UART_PARITY_NONE;
 sp.stop = UART_STOP_BITS_1;
 irq.FIQ = 0;
 irq.pri = (INT_PRIORITY)5;

 PINSEL0 &= 0x0FFFFu;	/*  Set pin P0.8-P0.15 to I/O.		*/
 IODIR |= 0xFFFFFF00u; 	/*  Set pin P0.8-P0.32 to output.	*/

	/*  Quick toggles to show we got started.			*/
    IOCLR = YELLOW_LED;
    IOSET = YELLOW_LED;	/* 1 */
    IOCLR = YELLOW_LED;
    IOSET = YELLOW_LED;	/* 2 */
    IOCLR = YELLOW_LED;
    IOSET = YELLOW_LED;	/* 3 */
    IOCLR = YELLOW_LED;
    IOSET = YELLOW_LED;	/* 4 */
    IOCLR = YELLOW_LED;
    IOSET = YELLOW_LED;	/* 5 */
    IOCLR = YELLOW_LED;
    IOSET = YELLOW_LED;	/* 6 */
    IOCLR = YELLOW_LED;


	/*  Set up memory access, CPU and bus speeds.			*/
 (void)SetMAM( 3u, MAM_full_enable);
 (void)VPBControl( VPB_DIV1);
 (void)SetDesiredSpeed( 60000uL);

 (void)ioctl( fileno(stdout), INTERRUPT_SETUP, &irq);
	/*  Set up serial port to desired rate.				*/
 (void)ioctl( fileno(stdout), UART_SETUP, &sp);

	/*  Start timer.						*/
 (void)StartClock();



 IOSET = YELLOW_LED;
 (void)iprintf( "Minimum Wait %u\r\n", MinimumAchievableWait());

 IOCLR = YELLOW_LED;
 puts( "flasher.c Hello World\r\n");		/*  It's alive !!		*/
 IOSET = YELLOW_LED;
 

	/*  Another sequence of toggles.  This time high and low should	*/
	/* be 1 second each for a 20sec total delay.  Should be easily 	*/
	/* measureable to confirm timer operation.			*/
 puts( "Starting wait\r\n");

 IOCLR = GREEN_LED;
 WaitUs( 1000000u);
 IOSET = GREEN_LED;
 WaitUs( 1000000u);
 IOCLR = GREEN_LED;
 WaitUs( 1000000u);
 IOSET = GREEN_LED;
 WaitUs( 1000000u);
 IOCLR = GREEN_LED;
 WaitUs( 1000000u);
 IOSET = GREEN_LED;
 WaitUs( 1000000u);
 IOCLR = GREEN_LED;
 WaitUs( 1000000u);
 IOSET = GREEN_LED;
 WaitUs( 1000000u);
 IOCLR = GREEN_LED;
 WaitUs( 1000000u);
 IOSET = GREEN_LED;
 WaitUs( 1000000u);
 IOCLR = GREEN_LED;
 WaitUs( 1000000u);
 IOSET = GREEN_LED;
 WaitUs( 1000000u);
 IOCLR = GREEN_LED;
 WaitUs( 1000000u);
 IOSET = GREEN_LED;
 WaitUs( 1000000u);
 IOCLR = GREEN_LED;
 WaitUs( 1000000u);
 IOSET = GREEN_LED;
 WaitUs( 1000000u);
 IOCLR = GREEN_LED;
 WaitUs( 1000000u);
 IOSET = GREEN_LED;
 WaitUs( 1000000u);
 IOCLR = GREEN_LED;
 WaitUs( 1000000u);
 IOSET = GREEN_LED;
 WaitUs( 1000000u);
 IOCLR = GREEN_LED;
 WaitUs( 1000000u);
 IOSET = GREEN_LED;

 puts( "End wait\r\n");

	/*  Two possible ways to end, either sit toggling a pin or run	*/
	/* a simple character echo.					*/
#if 0
 PINSEL0 = 0u;
 IODIR |= 0x100;

 while( 1) {  /*lint !e716*/
    IOSET = GREEN_LED;
    IOCLR = GREEN_LED;
    }
#endif

#if 1
    while ( 1 )
    {
        int     charAvail;

        (void)ioctl( fileno(stdout), UART_CHAR_WAITING, &charAvail );

        if ( charAvail )
        {
            fputc( getchar(), stdout);
        }
    }
#endif
 return( 0);			/*lint !e527 unreachable		*/
 }
