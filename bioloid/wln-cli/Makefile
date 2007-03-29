###########################################################################
#
# tiny45-flasher Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m168
MK_AVR_FREQ			= 8MHz
MK_AVR_PROGRAMMER  ?= BootHost
					  
MK_HEX_TARGET	= wln-cli
MK_SRC_FILES	= wln-cli.c Log.c StrPrintf.c UART.c Timer.c

include ../../rules/mkRules.mk

