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

#define LED_0 ( 1UL << 18UL )
#define LED_1 ( 1UL << 20UL )
#define LED_2 ( 1UL << 21UL )
#define LED_3 ( 1UL << 23UL )

static unsigned long gLed[] = { LED_0, LED_1, LED_2, LED_3 };

/*************************************************************************** 
* 
*   Initialize the GPIO pin that the LED is connected to.
*  
***************************************************************************/

static void LED_Init( void )
{
	LPC_GPIO1->FIODIR = LED_0 | LED_1 | LED_2 | LED_3;
	LPC_GPIO1->FIOCLR = LED_0 | LED_1 | LED_2 | LED_3;
}

/*************************************************************************** 
* 
*   Turns the LED on or off. On the robovero, driving the pin low turns the
*   LED on.
*  
***************************************************************************/

static void SetLED( unsigned led, unsigned value )
{
	if( value )
		LPC_GPIO1->FIOSET = gLed[led];
	else
		LPC_GPIO1->FIOCLR = gLed[led];
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
	unsigned int 	led[] = { 0, 1, 2, 3, 2, 1 };
	unsigned int	led_idx = 0;

	for(;;)
	{
		SetLED(led[led_idx], 1);
		vTaskDelay( 100 / portTICK_RATE_MS );
		SetLED(led[led_idx], 0);
		vTaskDelay( 100 / portTICK_RATE_MS );
		SetLED(led[led_idx], 1);
		vTaskDelay( 100 / portTICK_RATE_MS );
		SetLED(led[led_idx], 0);
		vTaskDelay( 700 / portTICK_RATE_MS );

		led_idx++;
		if( led_idx >=6 )
		{
			led_idx = 0;
		}
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

