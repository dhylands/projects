###########################################################################
#
# bioloidFoot Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m168
MK_AVR_FREQ			= 16MHz

MK_AVR_PROGRAMMER  ?= stk500

# Extended fuse byte should be FB for standalone, FA for bootloader-stk500
MK_AVR_FUSES_m168   = FB DF FF

MK_HEX_TARGET	= bioloidFoot
MK_SRC_FILES	= bioloidFoot.c bioloid-pkt.c Delay.c adc.c Log.c StrPrintf.o Timer-UART.c bioloid-uart.c Timer.c

include ../../rules/mkRules.mk
