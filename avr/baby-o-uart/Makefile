###########################################################################
#
# Simple-Flasher Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m168
MK_AVR_FREQ			= 20MHz
MK_AVR_PROGRAMMER  ?= stk500
					  
MK_HEX_TARGET	= baby-o-uart
MK_SRC_FILES	= baby-o-uart.c UART.c Delay.c Timer.c Hardware.c

include ../../rules/mkRules.mk

