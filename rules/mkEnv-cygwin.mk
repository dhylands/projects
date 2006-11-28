###########################################################################
#
# 	mkEnv-cygwin.mk
#
# 	This file contains cygwin specific defines for the build environment.
#
# 	This file should only define variables and not have any rules in it.
#
###########################################################################

MK_ELF_EXT    	= .exe

include $(MK_RULES_DIR)/mkEnv-gcc.mk
