###########################################################################
#
#	Makefile to build the bioloid command line interface
#
###########################################################################

MK_OS		= host
#MK_OS           = avr

ifeq ($(MK_OS),avr)
MK_AVR_MCU      = m168
MK_AVR_FREQ     = 8MHz
endif

ifeq ($(MK_OS),host)
ifeq ($(findstring CYGWIN_NT,$(shell uname -s)),CYGWIN_NT)
MK_HOST_OS = mingw
endif
endif

$(info MK_OS      = $(MK_OS))
$(info MK_HOST_OS = $(MK_HOST_OS))

MK_ELF_TARGET 	= bioloid

MK_SRC_FILES = \
	bioloid.cpp	\
	Log.c \
	BioloidBus.cpp \
	BioloidDevice.cpp \
	BioloidPacket.cpp \
	SerialPort.cpp \
	SerialBus.cpp \
	Error.cpp \
	bioloid-reg.cpp \
	StrPrintf.c \
	BioloidCommandLine.cpp \
	bioloid-reg.c \
	DevTypeParser.cpp \
	DumpMem.c \
	Str.c \
	StrToken.cpp

ifeq ($(MK_OS),avr)
MK_SRC_FILES += c++-support.cpp
endif

MK_ADD_SYMBOLS := 0

#CFLAGS += $(OPTS)
#CXXFLAGS += $(OPTS)

ifeq ($(MK_HOST_OS),mingw)
MK_SRC_FILES += NetBus.cpp
LDLIBS += -lwsock32
endif

include ../../rules/mkRules.mk

#
# bioloid.exe looks in the directory with the executable to find 
# the register (reg-*.bld) files. So we need to copy them.
#

all: copy-files

MK_DEVICE_TYPE_FILES := $(wildcard reg-*.bld)
MK_HTM_FILES := $(wildcard *.htm)
MK_NSI_FILES := $(wildcard *.nsi) $(wildcard *.nsh)
MK_ICO_FILES := $(wildcard *.ico)

MK_COPY_FILES := $(addprefix $(MK_BIN_DIR)/,$(MK_NSI_FILES) $(MK_HTM_FILES) $(MK_DEVICE_TYPE_FILES) $(MK_ICO_FILES))

$(MK_COPY_FILES) : $(MK_BIN_DIR)/% : %
	@echo "Copying $< to $@ ..."
	cp $< $@

copy-files: $(MK_COPY_FILES)
	
#
# Now go and build an installer.
#
# We build the installer from withing the bin directory, so
# filenames need to be relative to that
#

MK_INSTALL_FILES := $(MK_ELF_TARGET)$(MK_ELF_EXT)
MK_INSTALL_FILES += $(MK_HTM_FILES)
MK_INSTALL_FILES += $(MK_DEVICE_TYPE_FILES)

installer : copy-files                                                         
	@$(ECHO) -e "file $(subst $(space),\nfile ,$(MK_INSTALL_FILES))" > $(MK_BIN_DIR)/file-list.nsh
	@$(ECHO) -e "delete \$$INSTDIR\\\\$(subst $(space),\ndelete \$$INSTDIR\\\\,$(MK_INSTALL_FILES))" > $(MK_BIN_DIR)/file-delete.nsh
	(cd $(MK_BIN_DIR); makensis installer.nsi)
