###########################################################################
#
# flasher-m168 Makefile
#
###########################################################################

MK_OS			= avr
MK_AVR_MCU		= m128
MK_AVR_FREQ		= 16MHz
MK_AVR_PROGRAMMER	?= avrdude
MK_avrdude_PORT		?= /dev/ttyS0
MK_avrdude_PROG		?= avrispv2

# Extended fuse byte should be FB for standalone, FA for bootloader-stk500
MK_AVR_FUSES_m128   = FF C9 BF
					  
MK_HEX_TARGET	= simple-flasher
MK_SRC_FILES	= simple-flasher.c

include ../../rules/mkRules.mk

