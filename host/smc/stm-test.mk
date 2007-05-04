###########################################################################
#
#	Makefile to build stm-test
#
###########################################################################

MK_OS			= host
MK_ELF_TARGET 	= stm-test

MK_SRC_FILES = \
	stm-test.c \
	cruise-sm.c \
	stm.c \
	Log.c

CFLAGS += -DSTM_DEBUG

include ../../rules/mkRules.mk
	
