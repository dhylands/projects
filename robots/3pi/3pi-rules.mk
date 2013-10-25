###########################################################################
#
# Common makefile snippets for all of the 3pi sub-projects
#
###########################################################################

MK_OS			= avr
MK_AVR_MCU		= m328p
MK_AVR_FREQ		= 20MHz
#MK_AVR_PROGRAMMER  	?= BootHost
MK_BootHost_PORT	?= /dev/ttyUSB0
MK_BootHost_BAUD	?= 57600
MK_AVR_PROGRAMMER  	?= avrdude

#MK_AVR_FUSES_m328p	= 04 D2 F6	# 2k bootloader
MK_AVR_FUSES_m328p	= 04 D1 F6	# For standalone  mode
					  
# Add the robots/3pi directory into the search path, so that we
# can have some centralized header files.
#
# Note: In this case, .. is relative to a subdirectory of robots/3pi

CPPFLAGS += -I ..

include ../../../rules/mkRules.mk
