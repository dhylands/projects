###########################################################################
#
# bioloidGripper Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m168
MK_AVR_FREQ			= 8MHz

MK_AVR_PROGRAMMER  ?= stk500

# Extended fuse byte should be FB for standalone, FA for bootloader-stk500
MK_AVR_FUSES_m168   = FB DF FF

MK_HEX_TARGET	= bioloidGripper
MK_SRC_FILES	= bioloidGripper.c Motor.c Delay.c adc.c Log.c StrPrintf.o Timer-UART.c Timer.c

include ../../rules/mkRules.mk
