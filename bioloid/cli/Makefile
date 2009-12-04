###########################################################################
#
#	Makefile to build the bioloid command line interface
#
###########################################################################

MK_OS			= host
MK_ELF_TARGET 	= bioloid

MK_SRC_FILES = \
	bioloid.cpp	\
	Log.c \
	BioloidBus.cpp \
	BioloidDevice.cpp \
	BioloidPacket.cpp \
	SimBus.cpp \
	SimPacket.cpp \
	bioloid-reg.cpp \
	StrPrintf.c \
	BioloidCommandLine.cpp \
	bioloid-reg.c \
	bioloid-reg-servo.c \
	StrToken.cpp

#CFLAGS += $(OPTS)
#CXXFLAGS += $(OPTS)

LDLIBS += -lpthread

include ../../rules/mkRules.mk

