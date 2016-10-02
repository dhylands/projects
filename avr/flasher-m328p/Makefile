###########################################################################
#
# flasher-m328p Makefile - for Kamloops Makerspace Arduino board
#
###########################################################################

MK_OS			= avr
MK_AVR_MCU		= m328p
MK_AVR_FREQ		= 16MHz
MK_AVR_PROGRAMMER	?= avrdude
MK_avrdude_PORT		?= usb
MK_avrdude_PROG		?= avrisp2

# XX HH LL: High byte should be D2 for bootloader D3 for standalone

MK_AVR_FUSES_m328p   = 05 DE FF  	# bootloader
#MK_AVR_FUSES_m328p   = 05 DF FF		# standalone
					  
MK_HEX_TARGET	= flasher
MK_SRC_FILES	= flasher.c

include ../../rules/mkRules.mk

