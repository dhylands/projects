###########################################################################
#
# flasher-m168 Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m168
MK_AVR_FREQ			= 8MHz
MK_AVR_PROGRAMMER  ?= stk500

# Extended fuse byte should be FB for standalone, FA for bootloader-stk500
MK_AVR_FUSES_m168   = FB DF E2
					  
MK_HEX_TARGET	= flasher
MK_SRC_FILES	= flasher.c

include ../../rules/mkRules.mk

