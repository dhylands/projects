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

MK_HEX_TARGET	= compassTest
MK_SRC_FILES	= compassTest.c UART.c Delay.c Log.c StrPrintf.o Timer.c i2cmaster.c

include ../../rules/mkRules.mk
