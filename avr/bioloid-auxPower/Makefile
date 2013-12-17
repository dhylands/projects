###########################################################################
#
# bioloid-io Makefile
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m168
MK_AVR_FREQ			= 8MHz

USE_BOOTLOADER = 0

ifeq ($(USE_BOOTLOADER),1)

MK_AVR_PROGRAMMER  ?= BootHost
MK_BootHost_PORT   ?= /dev/ttyUSB0
MK_AVR_FUSES_m168   = 02 DC E2

else

MK_AVR_PROGRAMMER  ?= avrdude
# Extended fuse byte should be FB for standalone, FA for bootloader-stk500
MK_AVR_FUSES_m168   = 01 DC E2

endif

TARGET		= aux-power
$(info TARGET = $(TARGET))
MK_HEX_TARGET	= $(TARGET)
MK_SRC_FILES	= $(MK_HEX_TARGET).c Delay.c adc.c Timer.c UART.c

include ../../rules/mkRules.mk
