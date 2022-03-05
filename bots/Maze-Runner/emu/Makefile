###########################################################################
#
# Maz-Runner Makefile
#
###########################################################################

MK_OS = host

MK_ELF_TARGET = emu

include ../../../rules/mkEnv.mk

CPPFLAGS += -I ../src

CXXFLAGS += -std=gnu++17

MK_SRC_FILES	= \
	BioloidGadget.cpp \
	BioloidPacket.cpp \
	DumpMem.c \
	emu.cpp	\
	Log.c \

MK_STYLE_DIRS += ../src ../../../common ../../../common/posix

vpath %.cpp ../src

LDLIBS += -lpthread

include ../../../rules/mkRules.mk

