###########################################################################
#
# Simple-Flasher Makefile
#
###########################################################################

AVR_MCU := atmega16
				   
TARGET	= Flash-LED

COMMON_OBJS = \
	a2d_8.o \
	DebugKey.o \
	Delay.o \
	Encoder.o \
	Expansion.o \
	Hardware.o \
	LCD.o \
	Log.o \
	Motor.o \
	StrPrintf.o \
	Timer.o \
	UART.o

include ../Rules.mk

BOOTHOST = /MyDocuments/Robotics/MyBots/Orion/BootHost/exe/BootHost.exe
BOOTHOST_OPTS = --baud=38400 #--port=com5

download d : $(TARGET).hex
	$(ECHO) "Downloading $< ..."
	$(Q)$(BOOTHOST) $(BOOTHOST_OPTS) $<

.PHONY: download d

mon: 
	$(ECHO) "Launching BootHost ..."
	$(Q)$(BOOTHOST) $(BOOTHOST_OPTS)

.PHONY: mon

#
# Download the STK500 bootloader using avrdude
#

ad_bootloader: 
