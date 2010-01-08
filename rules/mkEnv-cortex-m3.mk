###########################################################################
#
# 	mkEnv-arm-thumb2.mk
#
# 	This file contains AVR specific defines for the build environment.
#
# 	This file should only define variables and not have any rules in it.
#
###########################################################################

MK_CROSS_COMPILE	= arm-none-eabi-

MK_OS_DIR = cortex-m3

MK_COMMON_CORTEX_M3_DIR	= $(MK_COMMON_DIR)/cortex-m3

TARGET_ARCH	= -mthumb -mcpu=cortex-m3 -mthumb-interwork 

CPPFLAGS += \
	-I $(MK_COMMON_CORTEX_M3_DIR) \
	-DROM_RUN \
	-Dgcc \
	-D__WinARM__ \
	-D__WINARMSUBMDL_lm3s811__ \
	-Os \
	-ffunction-sections \
	-fdata-sections


LDFLAGS  += \
	-Wl,-gc-sections
#	-Wl,--verbose

LD_CRTFLAGS += \
	-nostartfiles \
	-lc  -lm -lc -lgcc -lluminary \
	-T $(MK_COMMON_CORTEX_M3_DIR)/lm3s811-ROM.ld
	
CXXFLAGS += -fno-exceptions -fno-rtti

# On the AVR, we always optimize

CFLAGS += -Os
CXXFLAGS += -Os

vpath %.c   $(MK_COMMON_CORTEX_M3_DIR)
vpath %.cpp $(MK_COMMON_CORTEX_M3_DIR)

LMIFLASH	= $(MK_ROOT)/WinARM/bin/lmiflash

include $(MK_RULES_DIR)/mkEnv-gcc.mk

ifeq ($(CYGPATH),)

# Code Sourcery uses the CYGPATH environment variable for performing
# pathname conversions

export CYGPATH = c:/cygwin/bin/cygpath

endif
