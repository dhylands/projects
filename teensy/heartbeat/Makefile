###########################################################################
#
# flasher-m168 Makefile
#
###########################################################################

#
# As received the fuses were:
#
#	L: 5F
#	H: DF
#	X: F4

MK_OS				= avr
MK_AVR_MCU			= m32u4
MK_AVR_FREQ			= 16MHz
MK_AVR_PROGRAMMER  ?= teensy

MK_HEX_TARGET	= heartbeat
MK_SRC_FILES	= heartbeat.c

include ../../rules/mkRules.mk

