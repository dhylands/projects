/****************************************************************************
*
*  Program for Twister, a Line Maze robot.
*
****************************************************************************/

#include <stdio.h>
#include <68332/isr.h>
#include <68332/qsm.h>
#include <68332/sim.h>
#include <68332/mrm/led.h>
#include <68332/mrm/lcd.h>
#include <../libsim/68332/sim_regs.h>

#define BAUD_RATE 57600

int main( void )
{
   set_processor_speed( CLOCK25MHZ );

   /*
    * since sciinit is required for all programs that do i/o
    * perhaps it should be in mrm_crt0.S also?  It would still
    * be available if the user wanted to change baud rate.
    */
   
   sciinit( BAUD_RATE );

   led_init();
   lcd_init( 2, 24, 0, FALSE );

   lcd_printf( "Loaded from RAM" );
   iprintf( "Loaded from RAM\n" );

   while ( 1 );
}


