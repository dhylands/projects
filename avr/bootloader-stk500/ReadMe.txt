Bootloader
Copyright 2005, Larry Barello

Derived from Jason Kyle 's bootstk500.c covered under GNU license

Major changes:
	1. Smaller: used register based variables
	2. Reworked makefiles to make more flexible
	3. added missing STK500 commands
	4. Added direct entry for applications that can scan for
	   the STK500 startup string (0x30, 0x20)

Use the .CMD file to program boards using an STK500, or AVRISP programmer.

When powered up, the bootloader will flash the board status LED
for 6 seconds while waiting for a connection with an STK500 compatible
bootloader (avrdude, uisp, etc) that can have variable baud rate.

If there is no application code, OR the user shorts out the boot loader
switch (cal button or Program A on the ARC ISP header, etc.) the status LED
flashes three times and the board enters the bootloader.

The default communications rate is 38,400 for RS80D and RS160D.  The
baud rate is set for 38400 with a 16mhz crystal on the ARC board.  Since
the default ARC oscillator is internal and 8mhz, the comm rate is actually
19200 and it flashes the LED for about 6 seconds...

Typical avrdude command line to communicate with the bootloader:

avrdude -p <MCU> -c stk500 -b <Baudrate> -U flash:w:<yourfile.hex>

BAUD rates:  The following baud rates are possible with various CPU
main clock frequencies.  Internal to boot.c, the USART init routine
uses CPU_CLK/8 rather than 16 (standard).  This allows less tolerance
for error, but allows wider possibilities for baud rates.  See the chip
data sheet for full details.

8 & 16 mhz (internal or external)
	4800
	9600
	19200
	38400

6 & 12 mhz (external crystal)
	4800
	9600
	19200
	38400
	57600
	115200

Other baud rates are possible, in particular if a baud rate crystal
is used (e.g. 14.7456 mhz) then any standard baud rate will be zero
error.
