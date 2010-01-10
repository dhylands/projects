###########################################################################
#
# 	mkEnv-gcc.mk
#
# 	This file contains variables which are common to gcc toolchains.
#
###########################################################################

ifeq ($(MK_LIB_PREFIX),)
MK_LIB_PREFIX	= lib
endif

ifeq ($(MK_LIB_EXT),)
MK_LIB_EXT    	= .a
endif

ifeq ($(MK_OBJ_EXT),)
MK_OBJ_EXT    	= .o
endif

ifeq ($(MK_BIN_EXT),)
MK_BIN_EXT    	= .bin
endif

ifeq ($(MK_ELF_EXT),)
MK_ELF_EXT    	= .elf
endif

ifeq ($(MK_HEX_EXT),)
MK_HEX_EXT		= .hex
endif

AR		= $(MK_CROSS_COMPILE)ar
AS		= $(MK_CROSS_COMPILE)gcc
CC		= $(MK_CROSS_COMPILE)gcc
CXX		= $(MK_CROSS_COMPILE)g++
OBJCOPY	= $(MK_CROSS_COMPILE)objcopy
OBJDUMP	= $(MK_CROSS_COMPILE)objdump
SIZE	= $(MK_CROSS_COMPILE)size
			   
CFLAGS += \
	-std=gnu99 \
	-Wall \
	-Wimplicit \
	-Wpointer-arith \
	-Wswitch \
	-Wredundant-decls \
	-Wreturn-type \
	-Wshadow \
	-Wunused \
	-Wcast-qual \
	-Wnested-externs \
	-Wmissing-prototypes \
	-Wstrict-prototypes \
	-Wmissing-declarations

CXXFLAGS += \
	-Wall \
	-Wimplicit \
	-Wpointer-arith \
	-Wswitch \
	-Wredundant-decls \
	-Wreturn-type \
	-Wshadow \
	-Wunused \
	-Wcast-qual

ifeq ($(MK_ADD_SYMBOLS),1)
CXXFLAGS += -ggdb
CFLAGS += -ggdb
LDFLAGS += -ggdb
endif

LDFLAGS += -Wl,-Map,$(basename $@).map $(LDFLAGS_RDYNAMIC)

DEP_OUTPUT_OPTION = -MMD -MF $(@:.o=.d)

