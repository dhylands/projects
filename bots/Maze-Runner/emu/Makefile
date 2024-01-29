###########################################################################
#
# Maze-Runner Makefile
#
###########################################################################

MK_OS = host

MK_ELF_TARGET = emu
MK_PROJECT_NAME = "Maze Runner Emulator"

include ../../../rules/mkEnv.mk

CPPFLAGS += -I ../src

CXXFLAGS += -std=gnu++17

MK_SRC_FILES	= \
	EmuGadget.cpp \
	BioloidControlTable.cpp \
	BioloidGadget.cpp \
	BioloidPacket.cpp \
	Delay.c \
	DumpMem.c \
	emu.cpp	\
	Log.c \

MK_STYLE_DIRS += ../src ../../../common ../../../common/posix
#MK_STYLE_DIRS += ../src

vpath %.cpp ../src

LDLIBS += -lpthread

include ../../../rules/mkRules.mk

