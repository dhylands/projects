###########################################################################
#
# reflow-oven Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m88
MK_AVR_FREQ			= 8MHz
MK_AVR_PROGRAMMER  ?= stk500

MK_HEX_TARGET	= reflow-oven
MK_SRC_FILES	= reflow-oven.c lcd-api.c lcd-printf.c StrPrintf.c lcd-hal-avr.c adc.c UART.c Delay.c Timer.c

include ../../rules/mkRules.mk

