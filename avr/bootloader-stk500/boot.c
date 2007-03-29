/*********************************************************
Serial Bootloader for Atmel megaAVR Controllers

boot.c
Copyright (c) 2005, Larry Barello
Cleaned up source, added timeout function, stripped out monitor stuff.
Rewrote to be much smaller (common waitspm(), all variables in registers.

Based upon ATmegaBOOT.c by E. Lins
based on stk500boot.c
Copyright (c) 2003, Jason P. Kyle
All rights reserved.
see avr1.org for original file and information

This program is free software; you can redistribute it
and/or modify it under the terms of the GNU General
Public License as published by the Free Software
Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public
License for more details.

You should have received a copy of the GNU General
Public License along with this program; if not, write
to the Free Software Foundation, Inc.,
59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

Licence can be viewed at
http://www.fsf.org/licenses/gpl.txt

Target = Atmel AVR m128,m64,m32,m16,m8,m162,m163,m169,
m8515,m8535. ATmega161 has a very small boot block so
isn't supported.

Tested with m128,m8,m163 - feel free to let me know
how/if it works for you.

*********************************************************/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "boot.h"

#define F_CPU   CFG_CPU_CLOCK

/* SW_MAJOR and MINOR needs to be updated from time to time to avoid warning message from AVR Studio */
#define HW_VER	 0x02
#define SW_MAJOR 0x01
#define SW_MINOR 0x12

#if !defined(SW_DDR) && !defined(LED_DDR)
#error Apparently no hardware has been defined!
#endif

#define SIG1	0x1E	// Yep, Atmel is the only manufacturer of AVR micros.  Single source :(
#if defined __AVR_ATmega128__
#define SIG2	0x97
#define SIG3	0x02
#define PAGE_SIZE	0x80U	//128 words
#elif defined __AVR_ATmega64__
#define SIG2	0x96
#define SIG3	0x02
#define PAGE_SIZE	0x80U	//128 words
#elif defined __AVR_ATmega32__
#define SIG2	0x95
#define SIG3	0x02
#define PAGE_SIZE	0x40U	//64 words
#elif defined __AVR_ATmega16__
#define SIG2	0x94
#define SIG3	0x03
#define PAGE_SIZE	0x40U	//64 words
#elif defined __AVR_ATmega8__
#define SIG2	0x93
#define SIG3	0x07
#define PAGE_SIZE	0x20U	//32 words
#elif defined __AVR_ATmega162__
#define SIG2	0x94
#define SIG3	0x04
#define PAGE_SIZE	0x40U	//64 words
#elif defined __AVR_ATmega163__
#define SIG2	0x94
#define SIG3	0x02
#define PAGE_SIZE	0x40U	//64 words
#elif defined __AVR_ATmega168__
#define SIG2	0x94
#define SIG3	0x06
#define PAGE_SIZE	0x40U	//64 words
#define EEWE    EEPE
#define EEMWE   EEMPE
#define SPMCR   SPMCSR
#elif defined __AVR_ATmega169__
#define SIG2	0x94
#define SIG3	0x05
#define PAGE_SIZE	0x40U	//64 words
#elif defined __AVR_ATmega8515__
#define SIG2	0x93
#define SIG3	0x06
#define PAGE_SIZE	0x20U	//32 words
#elif defined __AVR_ATmega8535__
#define SIG2	0x93
#define SIG3	0x08
#define PAGE_SIZE	0x20U	//32 words
#else
#error Not a recognized CPU type
#endif

#if defined( UCSR0A )
#   define  UBRRL   UBRR0L
#   define  UBRRH   UBRR0H
#   define  UCSRA   UCSR0A
#   define  UCSRB   UCSR0B
#   define  UCSRC   UCSR0C
#   define  UDR     UDR0
#endif

#if !defined( UDRE )
#   define  UDRE    UDRE0
#   define  RXC     RXC0
#   define  RXEN    RXEN0
#   define  TXEN    TXEN0
#   define  U2X     U2X0
#endif

#define PRINT_BANNER    0

#if PRINT_BANNER
static const char gBanner[] PROGMEM = "***** STK500 BootLoader *****\n\n";
static const char *s;
#endif

void putch(uint8_t);
uint8_t getch(void);
uint8_t checkchar(void);
void getNch(uint8_t);
void byte_response(uint8_t);
void nothing_response(void);
void waitspm(uint8_t);
void InitUART(void);
void DelayMS(uint16_t delay);
void eeprom_write_byte(uint8_t *addr, uint8_t data);
uint8_t eeprom_read_byte(const uint8_t *addr);

uint8_t buff[256];

void app_start( void );

#if defined(LED_DDR)
	#define ToggleLed()	(LED_PORT ^= _BV(LED))
	#define InitLed()	(LED_DDR |= _BV(LED))
	#define LedOn()		(LED_PORT |= _BV(LED))
	#define LedOff() 	(LED_PORT &= ~_BV(LED))
#else
	#define ToggleLed()
	#define InitLed()
	#define LedOn()
	#define LedOff()
#endif

#if defined(SW_DDR)
	#define InitSW()	(SW_PORT |= _BV(SW))
	#define SWInput()	(SW_PIN & _BV(SW))
#else
	#define InitSW()
	#define SWInput()	(1)
#endif

void boot_1(void)
{
	asm volatile(
	"\tldi	R24, 1\n"
	"\trjmp	boot\n"
	);
}

void boot_0(void)
{
	asm volatile(
	"\tldi	R24, 0\n"
	"\trjmp	boot\n"
	);
}

void boot(char) __attribute__ ((noreturn));
void boot(char bCalled)
{
    uint8_t ch, ch2, bEEPROM, i, bAppStart;
    uint16_t w;

    union address_union
    {
    	uint32_t dword;		// Use this to left shift & far memory access.
    	uint16_t word;		// Read word address into here
    	uint8_t byte[4];	// access rampz as byte[2]
    }
    address;

	union length_union
	{
	    uint16_t word;
	    uint8_t  byte[2];
	}
	length;

	// Set up bare minimum for C to operate...

	asm volatile(
	"\tcli\n"
	"\tclr	__zero_reg__\n"
	);
    SPL = (RAMEND-1)&0xFF;
    SPH = (RAMEND-1)>>8;

	// Now, ready to go.

	InitUART();
    InitSW();      // Enable BootLoader input pullup
    InitLed();

#if PRINT_BANNER
    {
        s = gBanner;

        while (( ch = pgm_read_byte( s )) != '\0' ) 
        {
            putch( ch );
            s++;
        }
    }
#endif

	if (bCalled)
	{
		putch(0x14);
		putch(0x10);
	}
	else
	{
	    bAppStart = (pgm_read_byte_near(0x00) != 0xFF && SWInput());	// Forced override into bootloader.

	    if (bAppStart)
	    	i = 30;	// 3 seconds
	    else
	    	i = 6;	// 1/2 second

        while(( bAppStart == 0 ) || i--)
        {

            putch('\0'); // Wakes up BootHost

            DelayMS(100);

            ToggleLed();
            if (checkchar())
            {
                if (getch() == '0')
                {
                    nothing_response();
                    bAppStart = 0;
                    break;
                }
            }
        }
		if (bAppStart)
        {
            app_start();
        }
	}

	LedOff();

    for (;;)
    {

        ch = getch();			/* get character from UART */
        if(ch=='0')				/* Hello is anyone home ? */
        {
            nothing_response();
        }
        else if(ch=='1')        // Request Programmer ID
        {
            if (getch() == ' ')
            {
                putch(0x14);
                putch('A');
                putch('V');
                putch('R');
                putch(' ');
                putch('S');
                putch('T');
                putch('K');
                putch(0x10);
            }
        }
        else if(ch == '@')  // AVR ISP/STK500 board commands  DON'T CARE so default nothing_response
        {
            ch2 = getch();
            if (ch2 > 0x85)
                getch();
            nothing_response();
        }
        else if(ch=='A')   // AVR ISP/STK500 board requests
        {
            ch2 = getch();
            if(ch2==0x80) byte_response(HW_VER);		// Hardware version
            else if(ch2==0x81) byte_response(SW_MAJOR);	// Software major version
            else if(ch2==0x82) byte_response(SW_MINOR);	// Software minor version
            else if(ch2==0x98) byte_response(0x03);		// Unknown but seems to be required by avr studio 3.56
            else byte_response(0x00);				    // Covers various unnecessary responses we don't care about
        }
        else if(ch=='B')        // Device Parameters  DON'T CARE, DEVICE IS FIXED
        {
            getNch(20);
            nothing_response();
        }
        else if(ch=='E')        // Parallel programming stuff  DON'T CARE  */
        {
            getNch(5);
            nothing_response();
        }
        else if(ch=='P' || ch == 'R')	// Ignore Enter, Leave and Erase
        {
        	LedOn();
            nothing_response();
        }
        else if (ch == 'Q')
        {
        	LedOff();
        	nothing_response();
        	DelayMS(2);	// Wait for previous response to clear.
        	start();	// Re-enter the bootloader
		}
#if 0	// Pointless since avrdude doesn't use these commands to access eeprom
        else if (ch == 'a')
        {
        	eeprom_write_byte(address.word++, getch());	// Do these auto increment?
        	nothing_response();
        }
        else if (ch == 'q')
        {
        	byte_response(eeprom_read_byte(address.word++));
        }
#endif
        /* Set address, little endian. EEPROM in bytes, FLASH in words  */
        /* Perhaps extra address bytes may be added in future to support > 128kB FLASH.  */
        /* This might explain why little endian was used here, big endian used everywhere else.  */
        else if(ch=='U')
        {
            address.byte[0] = getch();
            address.byte[1] = getch();
            nothing_response();
        }
        else if(ch == 'V')      // Universal SPI programming command, disabled.  Would be used for fuses and lock bits.
        {
            getNch(4);
            byte_response(0x00);
        }
        else if(ch=='d')         /* Write memory, length is big endian and is in bytes  */
        {
            length.byte[1] = getch();
            length.byte[0] = getch();
            bEEPROM = 0;
            if (getch() == 'E')
                bEEPROM = 1;
            for (w=0;w<length.word;w++)
            {
                buff[w] = getch();	 // Store data in buffer, can't keep up with serial data stream whilst programming pages
            }
            if (getch() == ' ')
            {
                if (bEEPROM)
                {		                //Write to EEPROM one byte at a time
                    for(w=0;w<length.word;w++)
                    {
                        eeprom_write_byte((uint8_t *)address.word,buff[w]);
                        address.word++;
                    }
                }
                else
                {					        //Write to FLASH one page at a time
                    address.dword = address.dword << 1;	        //address * 2 -> byte location
#ifdef RAMPZ
                    RAMPZ = address.byte[2];
#endif

                    /* if ((length.byte[0] & 0x01) == 0x01) length.word++;	//Even up an odd number of bytes */
                    if ((length.byte[0] & 0x01))
                        length.word++;	//Even up an odd number of bytes
                    cli();					//Disable interrupts, just to be sure
                    while(bit_is_set(EECR,EEWE));			//Wait for previous EEPROM writes to complete
                    asm volatile(
                    	"StartAsm:\n\t"
                    	   "clr	r17 		\n\t"	    //page_word_count
						   "mov	r30, %A2\t\n"
						   "mov	r31, %B2\t\n"
						   "mov	r26, %A3\t\n"
						   "mov	r27, %B3\t\n"
                           "ldi	r28,lo8(buff)\n\t"	    //Start of buffer array in RAM
                           "ldi	r29,hi8(buff)\n\t"
                           "length_loop:	\n\t"	    //Main loop, repeat for number of words in block
                           "cpi	r17,0x00	\n\t"	        //If page_word_count=0 then erase page
                           "brne	no_page_erase\n\t"
                           "ldi	r24, 0x03\n\t"
                           "rcall	waitspm	\n\t"
                    #ifdef __AVR_ATmega163__
                           ".word 0xFFFF	\n\t"
                           "nop			    \n\t"
                    #endif
                    	   "ldi	r24, 0x11\n\t"
                           "rcall	waitspm	\n\t"
                    #ifdef __AVR_ATmega163__
                           ".word 0xFFFF	\n\t"
                           "nop			    \n\t"
                    #endif
                           "no_page_erase:	\n\t"
                           "ld	r0,Y+		\n\t"	//Write 2 bytes into page buffer
                           "ld	r1,Y+		\n\t"
						   "ldi	r24, 0x01\n\t"
                           "rcall	waitspm	\n\t"

                           "inc	r17		    \n\t"	//page_word_count++
                           "cpi r17,%1	    \n\t"
                           "brlo	same_page\n\t"	//Still same page in FLASH
                           "write_page:		\n\t"
                           "clr	r17		    \n\t"	//New page, write current one first
                    #ifdef __AVR_ATmega163__
                           "andi	r30,0x80\n\t"	// m163 requires Z6:Z1 to be zero during page write
                    #endif
                    	   "ldi	r24, 0x05\n\t"
                           "rcall	waitspm	\n\t"
                    #ifdef __AVR_ATmega163__
                           ".word 0xFFFF	\n\t"
                           "nop			    \n\t"
                           "ori	r30,0x7E	\n\t"	// recover Z6:Z1 state after page write (had to be zero during write)
                    #endif
                    	   "ldi	r24, 0x11\n\t"
                           "rcall	waitspm	\n\t"
                    #ifdef __AVR_ATmega163__
                           ".word 0xFFFF	\n\t"
                           "nop			    \n\t"
                    #endif
                           "same_page:		\n\t"
                           "adiw	r30,2	\n\t"	//Next word in FLASH
                           "sbiw	r26,2	\n\t"	//length-2
                           "breq	final_write	\n\t"	//Finished
                           "rjmp	length_loop	\n\t"
                           "final_write:	\n\t"
                           "cpi	r17,0		\n\t"
                           "breq	block_done	\n\t"
                           "adiw	r26,2	\n\t"	//length+2, fool above check on length after short page write
                           "rjmp	write_page	\n\t"
                           "block_done:		\n\t"
                           "clr	__zero_reg__\n\t"	//restore zero register
                           : "=m" (SPMCR) : "M" (PAGE_SIZE), "r" (address), "r" (length) : "r0","r16","r17","r24","r25","r26","r27","r28","r29","r30","r31");

                    /* Should really add a wait for RWW section to be enabled, don't actually need it since we never */
                    /* exit the bootloader without a power cycle anyhow */
                }
                putch(0x14);
                putch(0x10);
            }
        }
        else if(ch=='t')         /* Read memory block mode, length is big endian.  */
        {
            length.byte[1] = getch();
            length.byte[0] = getch();
            if (getch() == 'E')
                bEEPROM = 1;
            else
            {
                bEEPROM = 0;
                address.dword = address.dword << 1;	        // address * 2 -> byte location
#if defined RAMPZ
				RAMPZ = address.byte[2];
#endif
            }
            if (getch() == ' ')
            {		                // Command terminator
                putch(0x14);
                for (w=0;w < length.word;w++)
                {		        // Can handle odd and even lengths okay
                    if (bEEPROM)
                    {	                        // Byte access EEPROM read
                        putch(eeprom_read_byte((const uint8_t *)address.word));
                        address.word++;
                    }
                    else
                    {
#if defined(RAMPZ)
						putch(pgm_read_byte_far(address.dword));
#else
						putch(pgm_read_byte_near(address.word));
#endif
                        address.word++;
                    }
                }
                putch(0x10);
            }
        }
        else if(ch=='u')         /* Get device signature bytes  */
        {
            if (getch() == ' ')
            {
                putch(0x14);
                putch(SIG1);
                putch(SIG2);
                putch(SIG3);
                putch(0x10);
            }
        }
        else if(ch=='v')          // Read oscillator calibration byte
        {
            byte_response(0x00);
        }
    }  /* end of forever loop */
}

typedef void (*FuncPtr)( void );

void app_start( void )
{
#if defined __AVR_ATmega8__

    asm volatile("rjmp	zero_addr\n");
#else
    asm volatile("jmp	zero_addr\n");
#endif
}

void putch(uint8_t ch)
{
    while (!(UCSRA & _BV(UDRE)))
    	;
    UDR = ch;
}

uint8_t getch(void)
{
    while((UCSRA & _BV(RXC)) == 0)
    	;
    return UDR;
}

#if 1
uint8_t checkchar(void)
{
    return (UCSRA & _BV(RXC))?1:0;
//		return 1;
//	else
//		return 0;

}
#endif

void getNch(uint8_t count)
{
    uint8_t i;
    for(i=0;i<count;i++)
        getch();
}

void byte_response(uint8_t val)
{
    if (getch() == ' ')
    {
        putch(0x14);
        putch(val);
        putch(0x10);
    }
}

void nothing_response(void)
{
    if (getch() == ' ')
    {
        putch(0x14);
        putch(0x10);
    }
}

void waitspm(uint8_t command)
{
#if 0
	while (SPMCR & _BV(SPMEN))	// in R0, 37
		;						// sbrc R0, 0, rjmp .-6
	SPMCR = command;			// out 37, R24
	asm volatile("spm");		// spm, ret
#else
	asm volatile(
	"\tlds	r25, %0\n"
	"\tsbrc	r25, 0\n"
	"\trjmp	.-8\n"
	"\tsts	%0, r24\n"
	"\tspm\n"
	: "=m" (SPMCR) );
#endif
}

// Copied out of stdlib since we need insruct the compiler to not link them in
// in order to get the code size down.

uint8_t eeprom_read_byte(const uint8_t *addr)
{
	while (EECR & _BV(EEWE))
		;
	EEAR = (uint16_t)addr;
	EECR |= _BV(EERE);
	return EEDR;
}

void eeprom_write_byte(uint8_t *addr, uint8_t data)
{
	uint8_t sreg = SREG;
	while (EECR & _BV(EEWE))
		;
	EEAR = (uint16_t)addr;
	EEDR = data;
	cli();
	EECR |= _BV(EEMWE);
	EECR |= _BV(EEWE);
	SREG = sreg;
	return;
}

void InitUART(void)
{
    /* initialize UART(s) depending on CPU defined */
#if defined __AVR_ATmega163__
    UBRR = (uint8_t)(F_CPU/(BAUD_RATE*16L)-1);
    UBRRHI = (F_CPU/(BAUD_RATE*16L)-1) >> 8;
    UCSRA = 0x00;
    UCSRB = _BV(TXEN)|_BV(RXEN);
#else
    /* m8,m16,m32,m169,m8515,m8535 */
    UBRRL = (F_CPU/(BAUD_RATE*8L)-1);
    UBRRH = (F_CPU/(BAUD_RATE*8L)-1) >> 8;
    UCSRA = _BV(U2X);
    UCSRC = 0x86;
    UCSRB = _BV(TXEN)|_BV(RXEN);
#endif
}
void DelayMS(uint16_t delay)
{
	asm volatile(
	"\tldi	R26, lo8(%0/4000)\n"
	"\tldi	R27, hi8(%0/4000)\n"
	"\tsbiw	R26, 1\n"
	"\tbrne	.-4\n"
	"\tsbiw	R24, 1\n"
	"\tbrne	.-12\n"
	:: "i" (F_CPU) );
}


/* end of file ATmegaBOOT.c */
