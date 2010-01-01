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
	DevTypeParser.cpp \
	DumpMem.c \
	Str.c \
	StrToken.cpp

ifeq ($(MK_HOST_OS),mingw)
MK_SRC_FILES += Error.cpp
endif

ifeq ($(MK_OS),avr)
MK_SRC_FILES += c++-support.cpp
endif

MK_ADD_SYMBOLS := 1

#CFLAGS += $(OPTS)
#CXXFLAGS += $(OPTS)

#LDLIBS += -lpthread

include ../../rules/mkRules.mk

#
# bioloid.exe looks in the directory with the executable to find 
# the register (reg-*.bld) files. So we need to copy them.
#

all: copy-register-files

copy-register-files: $(addprefix $(MK_BIN_DIR)/,$(wildcard reg-*.bld))

$(MK_BIN_DIR)/reg-%.bld : reg-%.bld
	@echo "Copying $< to $@ ..."
	cp $< $@

