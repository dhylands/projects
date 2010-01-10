###########################################################################
#
# 	mkEnv.mk
#
# 	This file contains common variable definitions used during the build.
#
#	This Makefile requires make version 3.81
#
###########################################################################

MK_ENV_INCLUDED = 1

ifneq ($(lastword a b),b)
$(error These Makefiles require make 3.81 or newer)
endif

#--------------------------------------------------------------------------
#
# 	Run make with v=1 or verbose=1 to get verbose output
#

ifeq ($(v),)
export verbose = 0
else
export verbose = 1
endif

ifeq ($(verbose),)
export verbose = 0
endif

ifeq ($(verbose),0)
	Q = @
	MAKEFLAGS += -s
else
	Q =
endif
export Q

#--------------------------------------------------------------------------
# 	MK_ROOT describes the path to get from the directory containing the
# 	Makefile which included this file,

MK_ROOT := $(patsubst %/,%,$(dir $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))))
export MK_ROOT

ifeq ($(verbose),1)
$(info MK_ROOT = $(MK_ROOT))
endif

#--------------------------------------------------------------------------
#
# Define space to be a single space character. Using " " gets the quotes
# as well, which we don't want.
#

empty :=
space := $(empty) $(empty)


#--------------------------------------------------------------------------
#
#	MK_HOST_OS describes the OS that the make system is running on.
#

MK_SUPPORTED_HOST_OS = cygwin linux mingw

ifeq ($(MK_HOST_OS),)
MK_HOST_UNAME = $(shell uname -s)

ifeq ($(findstring CYGWIN_NT,$(MK_HOST_UNAME)),CYGWIN_NT)
export MK_HOST_OS = cygwin
endif

ifeq ($(MK_HOST_UNAME),Linux)
export MK_HOST_OS = linux
endif

ifeq ($(findstring MINGW32_NT,$(MK_HOST_UNAME)),MINGW32_NT)
export MK_HOST_OS = mingw
endif

endif

ifeq ($(filter $(MK_HOST_OS),$(MK_SUPPORTED_HOST_OS)),)
$(warning Unsupported value for MK_HOST_OS: '$(MK_HOST_OS)')
$(warning MK_HOST_UNAME = '$(MK_HOST_UNAME)')
$(warning MK_OS must be one of the following:)
$(warning $(space))
$(foreach os,$(MK_SUPPORTED_HOST_OS),$(warning $(space)  $(os)))
endif

ifeq ($(MK_OS),host)
MK_OS = $(MK_HOST_OS)
endif
  
#--------------------------------------------------------------------------
#
# 	MK_OS describes the "Operating system" which is the source code is 
# 	currently being compiled for. For each supported OS, there is another
# 	rules file in the $(MK_ROOT)/rules directory which is named using
# 	mkRules_$(MK_OS).mk
#

MK_SUPPORTED_OS = avr cortex-m3 cygwin gumstix linux lpc mingw none win32

ifeq ($(filter $(MK_OS),$(MK_SUPPORTED_OS)),)
$(warning Unsupported value for MK_OS: '$(MK_OS)')
$(warning MK_OS must be one of the following:)
$(warning $(space))
$(foreach os,$(MK_SUPPORTED_OS),$(warning $(space)  $(os)))
$(warning $(space))
$(warning Please note that these are case sensitive)
$(warning $(space))
$(error Run 'make MK_OS=DesiredOS' to correct)
endif

#--------------------------------------------------------------------------
#
# 	Run make with debug=1 to have non-optimized code generated.
#

ifeq ($(debug),)
export debug = 0
endif

ifeq ($(debug),1)
MK_OPTIMIZE    	= 0
MK_REL_EXT	= -dbg
else
MK_OPTIMIZE    	= 3
MK_REL_EXT	=
endif

MK_ADD_SYMBOLS 	?= 1

#--------------------------------------------------------------------------
#
# 	MK_OS_DIR is used for uniquely identifying directories that ojects are
#	are placed in. The mkEnx-$(MK_OS).mk script can override this default
#
#	The AVR uses something like: obj/avr-m64-16MHz
#

MK_OS_DIR	= $(MK_OS)

#--------------------------------------------------------------------------
#
# 	MK_GEN_DIRS desribes the list of generated directories which can be
#	rm -rf'd to remove all generated files.
#

MK_GEN_DIRS = obj lib bin

#--------------------------------------------------------------------------
#
# 	MK_OBJ_DIR describes the location where the generated object and 
# 	dependancy files will be placed. 
#

ifeq ($(MK_OBJ_DIR),)
MK_OBJ_DIR = ./obj/$(MK_OS_DIR)$(MK_REL_EXT)
endif

#--------------------------------------------------------------------------
#
# 	MK_LIB_DIR describes the location where generated libraries files will
# 	be placed.
#

ifeq ($(MK_LIB_DIR),)
MK_LIB_DIR = ./lib/$(MK_OS_DIR)$(MK_REL_EXT)
endif

#--------------------------------------------------------------------------
#
# 	MK_BIN_DIR describes the location where generated binary (aka executable)
#	files will be placed.
#

ifeq ($(MK_BIN_DIR),)
MK_BIN_DIR = ./bin/$(MK_OS_DIR)$(MK_REL_EXT)
endif

#--------------------------------------------------------------------------
#
# 	MK_RULES_DIR describes the location where all of the support makefiles
#	live.
#

MK_RULES_DIR = $(MK_ROOT)/rules

#--------------------------------------------------------------------------
#
# 	MK_UTILS_DIR describes the location where all of the host utilities
#	are installed.
#

MK_UTILS_DIR = $(shell echo ~)/bin

#--------------------------------------------------------------------------
#
# 	The MK_xxx_NAME macro can be used to construct the name of a library,
#	executable, or object file, by providing the basename.
#
# 	For example:
#
#  		STR_LIB_NAME = $(call MK_LIB_NAME,str)
#
# 	will assign STR_LIB_NAME with the full version of the 'str' library.
#

MK_LIB_NAME = $(foreach libBase,$(1),$(MK_LIB_DIR)/$(MK_LIB_PREFIX)$(libBase)$(MK_LIB_EXT))
MK_BIN_NAME = $(foreach binBase,$(1),$(MK_BIN_DIR)/$(binBase)$(MK_BIN_EXT))
MK_ELF_NAME = $(foreach binBase,$(1),$(MK_BIN_DIR)/$(binBase)$(MK_ELF_EXT))
MK_HEX_NAME = $(foreach hexBase,$(1),$(MK_BIN_DIR)/$(hexBase)$(MK_HEX_EXT))
MK_OBJ_NAME = $(foreach objBase,$(1),$(MK_OBJ_DIR)/$(objBase)$(MK_OBJ_EXT))

#--------------------------------------------------------------------------
#
# Setup variables for the various utilities that are required.
# 

ECHO	= @echo
MKDIR	= mkdir
RM		= rm
INSTALL	= install

#--------------------------------------------------------------------------
#
# Setup stuff for the Common directory
#

MK_COMMON_DIR = $(MK_ROOT)/common

CPPFLAGS += -I . -I $(MK_COMMON_DIR)

vpath %.c   $(MK_COMMON_DIR)
vpath %.cpp $(MK_COMMON_DIR)

#--------------------------------------------------------------------------
#
# Get OS specific environment makefile.
#

MK_OS_ENV_FILE = $(MK_RULES_DIR)/mkEnv-$(MK_OS).mk
include $(MK_OS_ENV_FILE)

