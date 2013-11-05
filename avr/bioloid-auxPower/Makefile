###########################################################################
#
# bioloid-io Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m168
MK_AVR_FREQ			= 8MHz

MK_AVR_PROGRAMMER  ?= avrdude

# Extended fuse byte should be FB for standalone, FA for bootloader-stk500
MK_AVR_FUSES_m168   = F9 DC E2

MK_HEX_TARGET	= aux-power
MK_SRC_FILES	= aux-power.c Delay.c adc.c Timer.c UART.c

include ../../rules/mkRules.mk
