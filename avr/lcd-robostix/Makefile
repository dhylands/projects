###########################################################################
#
# lcd-robostix Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m128
MK_AVR_FREQ			= 16MHz
MK_AVR_PROGRAMMER  ?= BootHost

MK_HEX_TARGET	= lcd-robostix
MK_SRC_FILES	= lcd-robostix.c lcd-api.c lcd-hal-avr.c Delay.c Timer.c UART.c Hardware.c

include ../../rules/mkRules.mk

