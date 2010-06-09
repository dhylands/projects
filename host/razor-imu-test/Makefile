###########################################################################
#
#	Makefile to build the Razor IMU test program
#
###########################################################################

MK_OS		= host

ifeq ($(findstring CYGWIN_NT,$(shell uname -s)),CYGWIN_NT)
MK_HOST_OS = mingw
endif

$(info MK_OS      = $(MK_OS))
$(info MK_HOST_OS = $(MK_HOST_OS))

MK_ELF_TARGET 	= razor-imu

MK_SRC_FILES = \
	razor-imu-test.cpp	\
	Log.c \
	Razor-IMU.c \
	SerialPort.cpp \
	Error.cpp \
	StrPrintf.c \
	DumpMem.c \
	Str.c \
	StrToken.cpp

MK_ADD_SYMBOLS := 0

#CFLAGS += $(OPTS)
#CXXFLAGS += $(OPTS)

include ../../rules/mkRules.mk

