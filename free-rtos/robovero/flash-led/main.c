/****************************************************************************
*
*	A super simple program which launches a single task which flashes 
*	the red led on the robovero. 
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"

/* ----------------------------------------------------------------------- */

#define 	HEARBEAT_PRIORITY		( tskIDLE_PRIORITY + 1 )

#define		HEARTBEAT_STACK_SIZE	configMINIMAL_STACK_SIZE

#define	LED	( 1UL << 25UL )	/* RED LED on Port 3 pin 25 */

/*************************************************************************** 
* 
*   Initialize the GPIO pin that the LED is connected to.
*  
***************************************************************************/

static void LED_Init( void )
{
	LPC_GPIO3->FIODIR = LED;
	LPC_GPIO3->FIOCLR = LED;
}

/*************************************************************************** 
* 
*   Turns the LED on or off. On the robovero, driving the pin low turns the
*   LED on.
*  
***************************************************************************/

static void SetLED( unsigned value )
{
	if( value )
		LPC_GPIO3->FIOCLR = LED;
	else
		LPC_GPIO3->FIOSET = LED;
}

/*************************************************************************** 
* 
*   Initializes the chip.
*  
***************************************************************************/

static void InitHardware( void )
{
	/* Disable peripherals power. */
	LPC_SC->PCONP = 0;

	/* Enable GPIO power. */
	LPC_SC->PCONP = PCONP_PCGPIO;

	/* Disable TPIU. */
	LPC_PINCON->PINSEL10 = 0;

	if ( LPC_SC->PLL0STAT & ( 1 << 25 ) )
	{
		/* Enable PLL, disconnected. */
		LPC_SC->PLL0CON = 1;			
		LPC_SC->PLL0FEED = PLLFEED_FEED1;
		LPC_SC->PLL0FEED = PLLFEED_FEED2;
	}
	
	/* Disable PLL, disconnected. */
	LPC_SC->PLL0CON = 0;				
	LPC_SC->PLL0FEED = PLLFEED_FEED1;
	LPC_SC->PLL0FEED = PLLFEED_FEED2;
	    
	/* Enable main OSC. */
	LPC_SC->SCS |= 0x20;			
	while( !( LPC_SC->SCS & 0x40 ) );
	
	/* select main OSC, 12MHz, as the PLL clock source. */
	LPC_SC->CLKSRCSEL = 0x1;		
	
	LPC_SC->PLL0CFG = 0x20031;
	LPC_SC->PLL0FEED = PLLFEED_FEED1;
	LPC_SC->PLL0FEED = PLLFEED_FEED2;
	      
	/* Enable PLL, disconnected. */
	LPC_SC->PLL0CON = 1;				
	LPC_SC->PLL0FEED = PLLFEED_FEED1;
	LPC_SC->PLL0FEED = PLLFEED_FEED2;
	
	/* Set clock divider. */
	LPC_SC->CCLKCFG = 0x03;
	
	/* Configure flash accelerator. */
	LPC_SC->FLASHCFG = 0x403a;
	
	/* Check lock bit status. */
	while( ( ( LPC_SC->PLL0STAT & ( 1 << 26 ) ) == 0 ) );	
	    
	/* Enable and connect. */
	LPC_SC->PLL0CON = 3;				
	LPC_SC->PLL0FEED = PLLFEED_FEED1;
	LPC_SC->PLL0FEED = PLLFEED_FEED2;
	while( ( ( LPC_SC->PLL0STAT & ( 1 << 25 ) ) == 0 ) );	
	
	/* Configure the clock for the USB. */
	  
	if( LPC_SC->PLL1STAT & ( 1 << 9 ) )
	{
		/* Enable PLL, disconnected. */
		LPC_SC->PLL1CON = 1;			
		LPC_SC->PLL1FEED = PLLFEED_FEED1;
		LPC_SC->PLL1FEED = PLLFEED_FEED2;
	}
	
	/* Disable PLL, disconnected. */
	LPC_SC->PLL1CON = 0;				
	LPC_SC->PLL1FEED = PLLFEED_FEED1;
	LPC_SC->PLL1FEED = PLLFEED_FEED2;
	
	LPC_SC->PLL1CFG = 0x23;
	LPC_SC->PLL1FEED = PLLFEED_FEED1;
	LPC_SC->PLL1FEED = PLLFEED_FEED2;
	      
	/* Enable PLL, disconnected. */
	LPC_SC->PLL1CON = 1;				
	LPC_SC->PLL1FEED = PLLFEED_FEED1;
	LPC_SC->PLL1FEED = PLLFEED_FEED2;
	while( ( ( LPC_SC->PLL1STAT & ( 1 << 10 ) ) == 0 ) );
	
	/* Enable and connect. */
	LPC_SC->PLL1CON = 3;				
	LPC_SC->PLL1FEED = PLLFEED_FEED1;
	LPC_SC->PLL1FEED = PLLFEED_FEED2;
	while( ( ( LPC_SC->PLL1STAT & ( 1 << 9 ) ) == 0 ) );

	/*  Setup the peripheral bus to be the same as the PLL output (64 MHz). */
	LPC_SC->PCLKSEL0 = 0x05555555;

	/* Setup the LED */
	
	LED_Init();
}

/*************************************************************************** 
* 
*   The Hearbeat task
*  
***************************************************************************/

static portTASK_FUNCTION( Heartbeat_Task, parameters )
{
	(void)parameters;

	for(;;)
	{
		SetLED(1);
		vTaskDelay( 100 / portTICK_RATE_MS );
		SetLED(0);
		vTaskDelay( 100 / portTICK_RATE_MS );
		SetLED(1);
		vTaskDelay( 100 / portTICK_RATE_MS );
		SetLED(0);
		vTaskDelay( 700 / portTICK_RATE_MS );
	}
}

/*************************************************************************** 
* 
*   main
*  
***************************************************************************/

int main( void )
{
	InitHardware();

	xTaskCreate( Heartbeat_Task,		// Task Function
				 "Heartbeat", 			// Task Name
				 HEARTBEAT_STACK_SIZE, 	// Stack Size
				 NULL, 					// Parameters
				 HEARBEAT_PRIORITY, 	// Task Priority
				 NULL );				// Place to store task handle

	vTaskStartScheduler();

	/* 
	 * Will only get here if there was insufficient memory to create the idle
	 * task.  The idle task is created within vTaskStartScheduler().
	 */

	for( ;; )
		;
}

