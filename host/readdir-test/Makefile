###########################################################################
#
#	Makefile to build readdir-test
#
###########################################################################

MK_OS			= host
MK_ELF_TARGET 	= readdir-test

MK_SRC_FILES = readdir-test.c

LDLIBS += 

ifeq ($(findstring CYGWIN_NT,$(shell uname -s)),CYGWIN_NT)
MK_HOST_OS 		= mingw
MK_SRC_FILES	+= dirent.c
endif

include ../../rules/mkRules.mk

