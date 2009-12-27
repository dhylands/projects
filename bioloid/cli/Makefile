###########################################################################
#
#	Makefile to build the bioloid command line interface
#
###########################################################################

MK_OS			= host
#MK_OS           = avr

ifeq ($(MK_OS),avr)
MK_AVR_MCU      = m168
MK_AVR_FREQ     = 8MHz
endif

ifeq ($(MK_OS),host)
MK_HOST_OS = mingw
endif

MK_ELF_TARGET 	= bioloid

MK_SRC_FILES = \
	bioloid.cpp	\
	Log.c \
	BioloidBus.cpp \
	BioloidDevice.cpp \
	BioloidPacket.cpp \
	SerialPort.cpp \
	SerialBus.cpp \
	bioloid-reg.cpp \
	StrPrintf.c \
	BioloidCommandLine.cpp \
	bioloid-reg.c \
	bioloid-reg-servo.c \
	DumpMem.c \
	Str.c \
	StrToken.cpp

ifeq ($(MK_HOST_OS),mingw)
MK_SRC_FILES += Error.cpp
endif

ifeq ($(MK_OS),avr)
MK_SRC_FILES += c++-support.cpp
endif

#CFLAGS += $(OPTS)
#CXXFLAGS += $(OPTS)

#LDLIBS += -lpthread

include ../../rules/mkRules.mk

