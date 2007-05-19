###########################################################################
#
# Stinger Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m8
MK_AVR_FREQ			= 16MHz
MK_AVR_PROGRAMMER  ?= BootHost

# Extended fuse byte should be FB for standalone, FA for bootloader-stk500
#MK_AVR_FUSES_m168   = FB DF E2
					  
MK_HEX_TARGET	= Stinger
MK_SRC_FILES	= \
	MiniSumo.c	\
	adc.c		\
	Avoid.c		\
	DebugKey.c	\
	Delay.c		\
	Log.c		\
	Motor.c		\
	Sensor.c	\
	StrPrintf.c	\
	Timer.c		\
	UART.c

include ../../rules/mkRules.mk

