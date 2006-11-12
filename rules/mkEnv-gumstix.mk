###########################################################################
#
# 	mkEnv-gumstix.mk
#
# 	This file contains cygwin specific defines for the build environment.
#
# 	This file should only define variables and not have any rules in it.
#
###########################################################################

MK_LIB_PREFIX	= lib
MK_LIB_EXT    	= .a
MK_OBJ_EXT    	= .o
MK_BIN_EXT    	=

include $(MK_RULES_DIR)/mkEnv-gcc.mk

