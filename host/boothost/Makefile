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

#PTHREADS_DIR = ../../../pthreads-w32-2-8-0/pthreads.2 												 
#PTS += -mno-cygwin -I $(PTHREADS_DIR)
					   
#CFLAGS += $(OPTS)
#CXXFLAGS += $(OPTS)

#LDFLAGS += -L $(PTHREADS_DIR)
									  
LDLIBS += -lpthread

include ../../rules/mkRules.mk

