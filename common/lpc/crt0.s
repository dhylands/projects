/**************************** crt0.s ************************************/
/* Copyright 2003/12/28 Aeolus Development				*/
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
/*									*/
/*  Startup for LPC210X.  Use with associated linker script and newlib.	*/
/* Developed from source and hints from multiple startup modules 	*/
/* developed by others for various ARM systems.				*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 crt0.s 30-Dec-2003,10:32:32,`RADSETT' First archival version.
*   2 crt0.s 03-Mar-2004,15:56:50,`RADSETT' Add stack setup for each CPU mode to
*        allow support of interrupts.
*   TLIB revision history ends.
*/

	.extern	main 		/*  Usual C startup.			*/

 /* .text is used instead of .section .text so it works with
arm-aout too. */ 

	.text 
	.code 32 

	.align 	0

	/*  Defined in link script so startup knows where everything 	*/
	/* is.								*/
	.extern	__bss_beg__	
	.extern	__bss_end__
	.extern __stack_end__
	.extern __data_beg__
	.extern __data_end__
	.extern __data_beg_src__

	.global	start 
	.global	endless_loop

	.set INIT_FIQ_MODE,		0x11
	.set INIT_IRQ_MODE,		0x12
	.set INIT_SUPERVISOR_MODE,	0x13
	.set INIT_ABORT_MODE,		0x17
	.set INIT_UNDEFINED_MODE,	0x1B
	.set INIT_SYSTEM_MODE,		0x1F

/********************* start ********************************************/
/*  start (AKA _start, _mainCRTStartup) -- Gains control on reset and	*/
/* set up environment before running the operating C program. 		*/
start: 
_start:
_mainCRTStartup:

	/*  Start by setting up a stack					*/
	/* Set up the stack pointer to end of bss 			*/
	msr	cpsr_c, #INIT_FIQ_MODE
	ldr	sp, =__stack_end_fiq__
	msr	cpsr_c, #INIT_IRQ_MODE
	ldr	sp, =__stack_end_irq__
	msr	cpsr_c, #INIT_SUPERVISOR_MODE
	ldr	sp, =__stack_end_supervisor__
	msr	cpsr_c, #INIT_ABORT_MODE
	ldr	sp, =__stack_end_abort__
	msr	cpsr_c, #INIT_UNDEFINED_MODE
	ldr	sp, =__stack_end_undefined__
	msr	cpsr_c, #INIT_SYSTEM_MODE
	ldr	sp, =__stack_end__

	sub	sl, sp, #512	/* Assumes 512 bytes below sp 		*/

	mov 	a2, #0		/* Fill value 				*/
	mov	fp, a2		/* Null frame pointer 			*/ 
	mov	r7, a2		/* Null frame pointer for Thumb 	*/
	
	ldr 	r1, .LC1	/*  __bss_beg__ set in link script to 	*/
				/* point at beginning of uninitialized	*/
				/* ram.					*/ 
	ldr 	r3, .LC2	/*  __bss_beg__ set in link script to 	*/
				/* point at end of uninitialized ram.	*/ 
	subs 	r3, r3, r1	/*  Subtract two to find length of 	*/
				/* uninitialized ram.			*/ 
	beq	.end_clear_loop	/*  If no uninitialzed ram skip init.	*/

	mov 	r2, #0		/*  Value used to init ram.		*/

.clear_loop:
	strb	r2, [r1], #1	/*  Clear byte at r1, advance to next	*/
	subs	r3, r3, #1	/*  One less to do			*/
	bgt	.clear_loop	/*  If not done go the next.		*/

.end_clear_loop:
	
	ldr	r1, .LC3	/*  __data_beg__ set in link script to 	*/
				/* point at beginning of initialized ram*/
	ldr 	r2, .LC4	/*  __data_beg_src__ set in link script	*/
				/* to point to beginning of flash copy 	*/
				/* of the initial values of initialized	*/
				/* variables.				*/ 
	ldr 	r3, .LC5	/*  __data_end__ set in link script to 	*/
				/* point at end of initialized ram	*/
	subs	r3, r3, r1	/*  Calculate length of area in ram	*/
				/* holding initialzed variables.	*/
	beq	.end_set_loop	/*  If no initialized vars skip init.	*/

.set_loop: 
	ldrb	r4, [r2], #1	/* Read byte from flash (increment ptr),*/
	strb	r4, [r1], #1  	/* store it in ram (increment ptr) and, */
	subs	r3, r3, #1  	/* reduce bytes to copy by 1.		*/
	bgt 	.set_loop	/* Continue until all copied.		*/

.end_set_loop: 

	/*  Set up arguments to main and call.				*/

	mov		r0, #0		/*  no arguments  */ 
	mov		r1, #0		/* no argv either */

	bl		main 

	/*  Returning from main in this environment is really an error.	*/
	/* Go into a dead loop.						*/
endless_loop:
	b		endless_loop

	/* For Thumb, constants must be after the code since only 
	positive offsets are supported for PC relative addresses. */
	
	.align 0
.LC1: 
.word	__bss_beg__ 
.LC2:   
.word	__bss_end__
.LC3:
.word	__data_beg__
.LC4:
.word	__data_beg_src__
.LC5:
.word	__data_end__

	/**** Exception/Interrupt table ****/

	/*  defaults are defined in the link script.  The reserved	*/
	/* exception should be overridden by the download program 	*/
	/* (see LPC210X documentation).					*/

.section .startup,"ax" 
	.code 32 
	.align 0

	b	start
	b	undefined_instruction_exception
	b	software_interrupt_exception
	b	prefetch_abort_exception
	b	data_abort_exception
	b	reserved_exception
/*	b	interrupt_exception*/
	ldr	pc,[pc,#-0xFF0]	    	/* Vector via VIC		*/
	b	fast_interrupt_exception
