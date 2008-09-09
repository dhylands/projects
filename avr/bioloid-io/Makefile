###########################################################################
#
# bioloid-io Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m168
MK_AVR_FREQ			= 8MHz

MK_AVR_PROGRAMMER  ?= stk500

# Extended fuse byte should be FB for standalone, FA for bootloader-stk500
MK_AVR_FUSES_m168   = F9 DD E2

MK_HEX_TARGET	= bioloid-io
MK_SRC_FILES	= bioloid-io.c bioloid-pkt.c Delay.c adc.c Motor.c Log.c StrPrintf.o Timer-UART.c bioloid-uart.c Timer.c

include ../../rules/mkRules.mk
