/**************************** lpc210x.ld ********************************/
/* Copyright 2003/12/30 Aeolus Development				*/
/*									*/
/* Freely modifiable and redistributable.  Modify to suit your own needs*/
/* Please remove Aeolus Development copyright for any significant	*/
/* modifications or add explanatory notes to explain the mods and	*/
/* list authour(s).							*/
/*									*/
/* THIS SOFTWARE IS PROVIDED BY THE AEOULUS DEVELOPMENT "AS IS" AND ANY	*/
/* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE	*/
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR	*/
/* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AEOLUS DEVELOPMENT BE	*/
/* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR	*/
/* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF	*/
/* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 	*/
/* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,*/
/* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE */
/* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 	*/
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.			*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 lpc210x.ld 16-Jun-2004,12:08:04,`RADSETT' Original archived version
*   2 lpc210x.ld 21-Jul-2004,11:31:02,`RADSETT' Increase interrupt stack sizes.
*   3 lpc210x.ld 01-Feb-2005,11:14:22,`RADSETT' Correct spelling of SPDR.
*   TLIB revision history ends.
*/

/* Search directories for libraries.  Modify as needed. */

/*SEARCH_DIR( /home/radsett/arm/lib)*/
/*SEARCH_DIR( /home/radsett/install/arm-elf/lib)*/
/*SEARCH_DIR( /home/radsett/install/lib/gcc-lib/arm-elf/3.3.2)*/

/*  Memory layout for processor.  Modify RAM size upwards for 2105 and 	*/
/* 2106									*/

MEMORY {
  flash : ORIGIN = 0, LENGTH = 120K
  ram : ORIGIN = 0x40000000, LENGTH = 16K
  }

__ram_size__ = 16K;

__STACK_SIZE_FIQ__ = 0x100;
__STACK_SIZE_IRQ__ = 0x100;
__STACK_SIZE_SUPERVISOR__ = 0x4;
__STACK_SIZE_ABORT__ = 0x4;
__STACK_SIZE_UNDEFINED__ = 0x4;

__stack_end__ = 0x40000000 + __ram_size__ - 4 - __STACK_SIZE_FIQ__ - 
	__STACK_SIZE_IRQ__ - __STACK_SIZE_SUPERVISOR__ - __STACK_SIZE_ABORT__ -
	__STACK_SIZE_UNDEFINED__;
__stack_end_undefined__ = 0x40000000 + __ram_size__ - 4 - __STACK_SIZE_FIQ__ - 
	__STACK_SIZE_IRQ__ - __STACK_SIZE_SUPERVISOR__ - __STACK_SIZE_ABORT__;
__stack_end_abort__ = 0x40000000 + __ram_size__ - 4 - __STACK_SIZE_FIQ__ - 
	__STACK_SIZE_IRQ__ - __STACK_SIZE_SUPERVISOR__;
__stack_end_supervisor__ = 0x40000000 + __ram_size__ - 4 - __STACK_SIZE_FIQ__ - 
	__STACK_SIZE_IRQ__;
__stack_end_irq__ = 0x40000000 + __ram_size__ - 4 - __STACK_SIZE_FIQ__;
__stack_end_fiq__ = 0x40000000 + __ram_size__ - 4;


SECTIONS {
 . = 0;					/*  Start at address 0.		*/
 startup : { *(.startup)} >flash	/*  Place startup first.	*/

.init    : { KEEP (*(.init)) } >flash

 prog : {				/*  Program (.text) sections 	*/
 	*(.text)			/* are next, then constant data.*/
	*(.rodata)
	*(.rodata*)
	*(.glue_7)
	*(.glue_7t)
 	} >flash

.fini : { KEEP (*(.fini)) } > flash

 __end_of_text__ = .;			/*  Used by startup to find 	*/
					/* initialized vars.		*/

	/*  Initialized data, located in ram but a copy is placed	*/
	/* in flash so it can be used to init the ram on startup.	*/
 .data : { 
	__data_beg__ = .;			/* Used by startup.	*/
	__data_beg_src__ = __end_of_text__;	/* Used by startup.	*/
 	*(.data)
	__data_end__ = .;			/* Used by startup.	*/
 	} >ram AT>flash

	/*  Unitialized data, located in ram, no copy in flash needed	*/
	/* since startup will zero associated area in RAM.		*/
 .bss : { 
	__bss_beg__ = .;	/*  Used by startup to find start of	*/
				/* unitialized variables.		*/
 	*(.bss)
 	} >ram
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.  */
   . = ALIGN(32 / 8);
  }
  . = ALIGN(32 / 8);

	/* Used by startup to find end of unitialized variables.	*/
  _end = .;
  _bss_end__ = . ; __bss_end__ = . ; __end__ = . ;	
  PROVIDE (end = .);

	/*  Libraries to link against.					*/
/*INPUT( -lc -lnewlib-lpc -lc -lgcc )*/

	/*  Provide a default vector for any unhandled interrupts.	*/
PROVIDE( undefined_instruction_exception = endless_loop);
PROVIDE( software_interrupt_exception = endless_loop);
PROVIDE( prefetch_abort_exception = endless_loop);
PROVIDE( data_abort_exception = endless_loop);
PROVIDE( reserved_exception = endless_loop);
PROVIDE( interrupt_exception = endless_loop);
PROVIDE( fast_interrupt_exception = endless_loop);

	/*  Provide address definitions for any peripheral registers	*/
	/* used.							*/

/* WD */

PROVIDE( WDMOD = 0xE0000000);
PROVIDE( WDTC = 0xE0000004);
PROVIDE( WDFEED = 0xE0000008);
PROVIDE( WDTV = 0xE000000C);

/* TIMER 0 */

PROVIDE( T0IR = 0xE0004000);
PROVIDE( T0TCR = 0xE0004004);
PROVIDE( T0TC = 0xE0004008);
PROVIDE( T0PR = 0xE000400C);
PROVIDE( T0PC = 0xE0004010);
PROVIDE( T0MCR = 0xE0004014);
PROVIDE( T0MR0 = 0xE0004018);
PROVIDE( T0MR1 = 0xE000401C);
PROVIDE( T0MR2 = 0xE0004020);
PROVIDE( T0MR3 = 0xE0004024);
PROVIDE( T0CCR = 0xE0004028);
PROVIDE( T0CR0 = 0xE000402C);
PROVIDE( T0CR1 = 0xE0004030);
PROVIDE( T0CR2 = 0xE0004034);
PROVIDE( T0EMR = 0xE000403C);

/* TIMER 1 */

PROVIDE( T1IR = 0xE0008000);
PROVIDE( T1TCR = 0xE0008004);
PROVIDE( T1TC = 0xE0008008);
PROVIDE( T1PR = 0xE000800C);
PROVIDE( T1PC = 0xE0008010);
PROVIDE( T1MCR = 0xE0008014);
PROVIDE( T1MR0 = 0xE0008018);
PROVIDE( T1MR1 = 0xE000801C);
PROVIDE( T1MR2 = 0xE0008020);
PROVIDE( T1MR3 = 0xE0008024);
PROVIDE( T1CCR = 0xE0008028);
PROVIDE( T1CR0 = 0xE000802C);
PROVIDE( T1CR1 = 0xE0008030);
PROVIDE( T1CR2 = 0xE0008034);
PROVIDE( T1CR3 = 0xE0008038);
PROVIDE( T1EMR = 0xE000803C);

/* UART 0 */

PROVIDE( U0RBR = 0xE000C000);
PROVIDE( U0THR = 0xE000C000);
PROVIDE( U0DLL = 0xE000C000);
PROVIDE( U0IER = 0xE000C004);
PROVIDE( U0DLM = 0xE000C004);
PROVIDE( U0IIR = 0xE000C008);
PROVIDE( U0FCR = 0xE000C008);
PROVIDE( U0LCR = 0xE000C00C);
PROVIDE( U0LSR = 0xE000C014);
PROVIDE( U0SCR = 0xE000C01C);

/* UART 1 */

PROVIDE( U1RBR = 0xE0010000);
PROVIDE( U1THR = 0xE0010000);
PROVIDE( U1DLL = 0xE0010000);
PROVIDE( U1IER = 0xE0010004);
PROVIDE( U1DLM = 0xE0010004);
PROVIDE( U1IIR = 0xE0010008);
PROVIDE( U1FCR = 0xE0010008);
PROVIDE( U1LCR = 0xE001000C);
PROVIDE( U1MCR = 0xE0010010);
PROVIDE( U1LSR = 0xE0010014);
PROVIDE( U1MSR = 0xE0010018);
PROVIDE( U1SCR = 0xE001001C);

/* PWM */

PROVIDE( PWMIR = 0xE0014000);
PROVIDE( PWMTCR = 0xE0014004);
PROVIDE( PWMTC = 0xE0014008);
PROVIDE( PWMPR = 0xE001400C);
PROVIDE( PWMPC = 0xE0014010);
PROVIDE( PWMMCR = 0xE0014014);
PROVIDE( PWMMR0 = 0xE0014018);
PROVIDE( PWMMR1 = 0xE001401C);
PROVIDE( PWMMR2 = 0xE0014020);
PROVIDE( PWMMR3 = 0xE0014024);
PROVIDE( PWMMR4 = 0xE0014040);
PROVIDE( PWMMR5 = 0xE0014044);
PROVIDE( PWMMR6 = 0xE0014048);
PROVIDE( PWMPCR = 0xE001404C);
PROVIDE( PWMLER = 0xE0014050);

/* IIC */

PROVIDE( I2CONSET = 0xE001C000);
PROVIDE( I2STAT = 0xE001C004);
PROVIDE( I2DAT = 0xE001C008);
PROVIDE( I2ADR = 0xE001C00C);
PROVIDE( I2SCLH = 0xE001C010);
PROVIDE( I2SCLL = 0xE001C014);
PROVIDE( I2CONCLR = 0xE001C018);

/* SPI */

PROVIDE( SPCR = 0xE0020000);
PROVIDE( SPSR = 0xE0020004);
PROVIDE( SPDR = 0xE0020008);
PROVIDE( SPCCR = 0xE002000C);
PROVIDE( SPINT = 0xE002001C);

/* RTC */

PROVIDE( ILR = 0xE0024000);
PROVIDE( CTC = 0xE0024004);
PROVIDE( CCR = 0xE0024008);
PROVIDE( CIIR = 0xE002400C);
PROVIDE( AMR = 0xE0024010);
PROVIDE( CTIME0 = 0xE0024014);
PROVIDE( CTIME1 = 0xE0024018);
PROVIDE( CTIME2 = 0xE002401C);
PROVIDE( SEC = 0xE0024020);
PROVIDE( MINUTE = 0xE0024024);
PROVIDE( HOUR = 0xE0024028);
PROVIDE( DOM = 0xE002402C);
PROVIDE( DOW = 0xE0024030);
PROVIDE( DOY = 0xE0024034);
PROVIDE( MONTH = 0xE0024038);
PROVIDE( YEAR = 0xE002403C);
PROVIDE( ALSEC = 0xE0024060);
PROVIDE( ALMIN = 0xE0024064);
PROVIDE( ALHOUR = 0xE0024068);
PROVIDE( ALDOM = 0xE002406C);
PROVIDE( ALDOW = 0xE0024070);
PROVIDE( ALDOY = 0xE0024074);
PROVIDE( ALMON = 0xE0024078);
PROVIDE( ALYEAR = 0xE002407C);
PROVIDE( PREINT = 0xE0024080);
PROVIDE( PREFRAC = 0xE0024084);

/* GPIO */

PROVIDE( IOPIN = 0xE0028000);
PROVIDE( IODIR = 0xE0028008);
PROVIDE( IOCLR = 0xE002800C);
PROVIDE( IOSET = 0xE0028004);

/* PIN CONNECT BLOCK */

PROVIDE( PINSEL0 = 0xE002C000);
PROVIDE( PINSEL1 = 0xE002C004);


/* SYSTEM CONTROL BLOCK */
	/* MAM */

PROVIDE( MAMCR = 0xE01FC000);
PROVIDE( MAMTIM = 0xE01FC004);


PROVIDE( MEMAP = 0xE01FC040);

	/* PLL */

PROVIDE( PLLCON = 0xE01FC080);
PROVIDE( PLLCFG = 0xE01FC084);
PROVIDE( PLLSTAT = 0xE01FC088);
PROVIDE( PLLFEED = 0xE01FC08C);

	/* POWER CONTROL */

PROVIDE( PCON = 0xE01FC0C0);
PROVIDE( PCONP = 0xE01FC0C4);

	/* VPB */

PROVIDE( VPBDIV = 0xE01FC100);

	/* EXTERNAL INTERUPT/WAKE */

PROVIDE( EXTINT = 0xE01FC140);
PROVIDE( EXTWAKE = 0xE01FC144);


	/* Vector Interrupt Controller (VIC) */

PROVIDE( VICIRQStatus = 0xFFFFF000);
PROVIDE( VICFIQStatus = 0xFFFFF004);
PROVIDE( VICRawIntr = 0xFFFFF008);
PROVIDE( VICIntSelect = 0xFFFFF00C);
PROVIDE( VICIntEnable = 0xFFFFF010);
PROVIDE( VICIntEnClr = 0xFFFFF014);
PROVIDE( VICSoftInt = 0xFFFFF018);
PROVIDE( VICSoftIntClear = 0xFFFFF01C);
PROVIDE( VICProtection = 0xFFFFF020);
PROVIDE( VICVectAddrRead = 0xFFFFF030);
PROVIDE( VICDefVectAddr = 0xFFFFF034);
PROVIDE( VICVectAddr = 0xFFFFF100);
PROVIDE( VICVectCntl = 0xFFFFF200);

