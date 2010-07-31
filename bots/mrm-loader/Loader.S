/*****************************************************************************
*
* Loader program.
*
* This program sets up the serial port for 57600 baud and then transfers
* control to a program found in RAM.
*
******************************************************************************/

start:

    // The value of SCCRO[1] MUST be changed with different clock speeds
    // as enumerated by this table of values for 0xfffc09: 
    // 
    // Clock Freq (MHz)
    // Baud   || 8  | 16 | 25 |
    // -----------------------
    // 9600   || 27 | 55 | 82 |
    // 19200  || 14 | 27 | 41 |
    // 38400  || 7  | 14 | 20 |
    // 57600  || 5  | 9  | 14 |
    // 115200 || -- | -- | 7  |
    
    // Set the CPU to run at 25 MHz

    move.l  #0xfffffa00,%a0  // SIM
    move.w  %a0@(4),%d0
    andi.w  #0x00ff,%d0
    or.w    #0xd700,%d0     // 0xd700 = 25 MHz
    move.w  %d0,%a0@(4)
    
    // Wait for the clock to stabilize
    
stabalize:
    move.w  %a0@(4),%d0
    andi.b  #0x0008,%d0
    tst.b   %d0
    beq     stabalize    

    // Set the serial port to run at the desired speed
    
    move.l  #0xfffc09,%a0
	move.b  #14,(%a0)       // 14 = 57600
    
    lea     banner,%a0
    jsr     puts

    move.l  #20,%d2
        
wait_loop1:
    move.b  #'.',%d0
    jsr     putc
    move.l  #0xffff,%d0
wait_loop2:
    dbra    %d0,wait_loop2
    jsr     havebyte
    tst.l   %d0
    bne     start_cpu32bug
    dbra    %d2,wait_loop1
    
    // Speed has already been changed. Check for program in RAM
    
    move.l  #0x3000,%a3
    move.l  (%a3)+,%d0
    cmp.l   #0xbeefbeef,%d0
    bne     no_pgm_ram
    
start_pgm:
    
    // Program found in RAM, jump to it.

    lea     starting_pgm,%a0
    jsr     puts
    
    move.l  (%a3)+,%a7
    move.l  (%a3)+,%a0
    jmp     (%a3)
    
no_pgm_ram:

    lea     no_pgm,%a0
    jsr     puts
    
start_cpu32bug:

    lea     crlf,%a0
    jsr     puts

    trap    #15             // CPU32Bug .RETURN
	dc.w    0x0063
    
crlf:
    .string "\r\n"
    
banner:
    .string "\r\n\r\n57600 Loader\r\nPress any key to enter cpu32bug"
    
no_pgm:
    .string "\r\nNo program detected in RAM"
    
starting_pgm:
    .string "\r\nStarting program found in RAM\r\n"
    
wait_xmitr:
	jsr     sendingbyte
	tst.l   %d0
	bne     wait_xmitr
    rts

sendingbyte:
	move.w  -1012.w,%d0     // -1012 == fffc09 (SCCR)
	lsr.w   #7,%d0
	not.l   %d0
	and.l   #1,%d0
	rts
    
havebyte:
	move.w  -1012.w,%d0     // -1012 == fffc09 (SCCR)
	lsr.w   #6,%d0
	and.l   #1,%d0
	rts
    

putc:
    clr.l   %d1
    move.b  %d0,%d1
    jsr     wait_xmitr
    move.w  %d1,-1010.w     // -1010 == fffc0e (SCDR)
    jsr     wait_xmitr
    rts    
    
puts:
    move.l  %a0,%a2
puts_loop:
    move.b  (%a2)+,%d0
    jsr     putc    
    tst.b   (%a2)
    jbne    puts_loop
    rts    
