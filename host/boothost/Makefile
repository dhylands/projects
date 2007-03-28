###########################################################################
#
#	Makefile to build boothost
#
###########################################################################

MK_OS		= host
MK_ELF_TARGET 	= boothost

MK_SRC_FILES = \
	Crc8.c		\
	Log.c		\
	PKT_Packet.c	\
	AvrInfo.c	\
	FileParser.cpp	\
	ParserCallback.cpp	\
	BootHost.cpp	\
	DumpMem.cpp	\
	ReadFile.cpp	\
	SerialPort.cpp	\
	StateAVReflash.cpp	\
	StateMegaLoad.cpp	\
	StateSTK500.cpp		\
	StrMaxCat.cpp		\
	StrMaxCpy.cpp

LDLIBS += -lpthread

include ../../rules/mkRules.mk

