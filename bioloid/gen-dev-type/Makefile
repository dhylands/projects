###########################################################################
#
#	Makefile to build the gen-dev-type program
#
###########################################################################

MK_OS		= host

ifeq ($(findstring CYGWIN_NT,$(shell uname -s)),CYGWIN_NT)
MK_HOST_OS = mingw
endif

$(info MK_OS      = $(MK_OS))
$(info MK_HOST_OS = $(MK_HOST_OS))

MK_ELF_TARGET 	= gen-dev-type

MK_SRC_FILES = \
	gen-dev-type.cpp \
	Log.c \
	bioloid-reg.c \
	DevTypeParser.cpp \
	Str.c \
	StrPrintf.c \
	StrToken.cpp

MK_ADD_SYMBOLS := 0

CPPFLAGS += -I "../cli"

vpath	%.cpp	../cli

include ../../rules/mkRules.mk



