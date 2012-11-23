###########################################################################
#
# timer-uart Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m168
MK_AVR_FREQ			= 8MHz
MK_AVR_PROGRAMMER  ?= BootHost
					  
MK_HEX_TARGET	= tmr-uart
MK_SRC_FILES	= tmr-uart.c Log.c StrPrintf.c Timer-UART.c UART.c Timer.c

include ../../rules/mkRules.mk

