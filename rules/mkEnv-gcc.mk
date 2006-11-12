###########################################################################
#
# 	mkEnv-gcc.mk
#
# 	This file contains variables which are common to gcc toolchains.
#
###########################################################################

AR		= $(MK_CROSS_COMPILE)ar
CC		= $(MK_CROSS_COMPILE)gcc
CXX		= $(MK_CROSS_COMPILE)g++
OBJCOPY	= $(MK_CROSS_COMPILE)objcopy
OBJDUMP	= $(MK_CROSS_COMPILE)objdump
SIZE	= $(MK_CROSS_COMPILE)size
			   
CFLAGS += -Wall
LDFLAGS += -Wl,-Map,$(basename $@).map

DEP_OUTPUT_OPTION = -MMD -MF $(@:.o=.d)

