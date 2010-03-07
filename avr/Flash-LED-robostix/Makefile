###########################################################################
#
# Flash-LED-robostix Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m128
MK_AVR_FREQ			= 16MHz
MK_AVR_PROGRAMMER  ?= BootHost

# Note: MK_AVR_FUSES is XX HH LL
MK_AVR_FUSES_m128	= FF C9 BF
MK_HEX_TARGET		= Flash-LED-robostix
MK_SRC_FILES		= Flash-LED.c UART.c Hardware.c Timer.c

include ../../rules/mkRules.mk

