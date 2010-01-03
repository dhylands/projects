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

all: copy-files

MK_DEVICE_TYPE_FILES := $(wildcard reg-*.bld)
MK_HTM_FILES := $(wildcard *.htm)
MK_NSI_FILES := $(wildcard *.nsi)

MK_COPY_FILES := $(MK_NSI_FILES) $(MK_HTM_FILES) $(MK_DEVICE_TYPE_FILES)

$(addprefix $(MK_BIN_DIR)/,$(MK_COPY_FILES)) : $(MK_BIN_DIR)/% : %
	@echo "Copying $< to $@ ..."
	cp $< $@
	
#
# Now go and build an installer.
#
# We build the installer from withing the bin directory, so
# filenames need to be relative to that
#

MK_INSTALL_FILES := $(MK_ELF_TARGET)$(MK_ELF_EXT)
MK_INSTALL_FILES += $(MK_HTM_FILES)
MK_INSTALL_FILES += $(MK_DEVICE_TYPE_FILES)

installer : $(addprefix $(MK_BIN_DIR)/,$(MK_COPY_FILES))
	@$(ECHO) -e "file $(subst $(space),\nfile ,$(MK_INSTALL_FILES))" > $(MK_BIN_DIR)/file-list.nsh
	@$(ECHO) -e "delete \$$INSTDIR\\\\$(subst $(space),\ndelete \$$INSTDIR\\\\,$(MK_INSTALL_FILES))" > $(MK_BIN_DIR)/file-delete.nsh
	(cd $(MK_BIN_DIR); makensis installer.nsi)
