###########################################################################
#
#	Makefile to build crc32
#
###########################################################################

ifeq ($(MK_OS),)
MK_OS			= host
MK_HOST_OS      = linux
endif

debug           = 1
MK_ADD_SYMBOLS  = 1

MK_ELF_TARGET 	= hello-world

MK_SRC_FILES 	= hello-world.c

include ../rules/mkRules.mk

