###########################################################################
#
# 	mkEnv-gumstix.mk
#
# 	This file contains gumstix specific defines for the build environment.
#
# 	This file should only define variables and not have any rules in it.
#
###########################################################################

ifeq ($(OVEROTOP),)
$(error OVEROTOP must be defined to use the gusmtix target)
endif
$(info OVEROTOP = $(OVEROTOP))

MK_TOOLS_DIR		= $(OVEROTOP)/tmp/cross/armv7a/bin
MK_CROSS_COMPILE	= $(MK_TOOLS_DIR)/arm-angstrom-linux-gnueabi-

MK_COMMON_POSIX_DIR	= $(MK_COMMON_DIR)/posix

vpath %.c   $(MK_COMMON_POSIX_DIR)
vpath %.cpp $(MK_COMMON_POSIX_DIR)

# linux uses -rdynamic
LDFLAGS_RDYNAMIC = -rdynamic

include $(MK_RULES_DIR)/mkEnv-gcc.mk

MK_ELF_EXT    		=

