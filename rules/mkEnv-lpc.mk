###########################################################################
#
# 	mkEnv-lpc.mk
#
# 	This file contains LPC specific defines for the build environment.
#
# 	This file should only define variables and not have any rules in it.
#
###########################################################################

MK_CROSS_COMPILE		= arm-elf-

MK_COMMON_LPC_DIR	= $(MK_COMMON_DIR)/lpc

CPPFLAGS += -I $(MK_COMMON_LPC_DIR)

CXXFLAGS += -fno-exceptions -fno-rtti

# On the LPC, we always optimize

ifeq ($(MK_OPTIMIZE),1)
CFLAGS += -Os
CXXFLAGS += -Os
else
CFLAGS += -O0
CXXFLAGS += -O0
endif

LOADLIBES	+= -lc -lnewlib-lpc
LDFLAGS += -nostartfiles -Wl,-T,$(MK_COMMON_LPC_DIR)/lpc210x.ld

MK_CRT_FILES = $(call MK_OBJ_NAME, crt0)

MK_OBJCOPY_SECTS	=

vpath %.s	$(MK_COMMON_LPC_DIR)
vpath %.c   $(MK_COMMON_LPC_DIR)
vpath %.cpp $(MK_COMMON_LPC_DIR)

include $(MK_RULES_DIR)/mkEnv-gcc.mk

# Typical lpc21isp command line:
#
#   lpc21isp -term test10.hex /dev/ttyUSB0 38400 14746

MK_lpc21isp	 		= lpc21isp
MK_lpc21isp_PORT	?= /dev/ttyS0
MK_lpc21isp_CMD		= $(Q)$(MK_lpc21isp) -term $< $(MK_lpc21isp_PORT) 38400 14746

