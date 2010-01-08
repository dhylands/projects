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
MK_EXE_EXT     = .exe

MK_COMMON_POSIX_DIR	= $(MK_COMMON_DIR)/posix

vpath %.c   $(MK_COMMON_POSIX_DIR)
vpath %.cpp $(MK_COMMON_POSIX_DIR)

# cygwin gcc doesn't use rdynamic
LDFLAGS_RDYNAMIC =

include $(MK_RULES_DIR)/mkEnv-gcc.mk
