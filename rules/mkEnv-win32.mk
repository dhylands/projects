###########################################################################
#
# 	mkEnv-win32.mk
#
# 	This file contains cygwin specific defines for the build environment.
#
# 	This file should only define variables and not have any rules in it.
#
###########################################################################

MK_ELF_EXT     = .exe
MK_EXE_EXT     = .exe

MK_COMMON_WIN32_DIR	= $(MK_COMMON_DIR)/win32

vpath %.c   $(MK_COMMON_WIN32_DIR)
vpath %.cpp $(MK_COMMON_WIN32_DIR)

ifeq ($(MK_LIB_PREFIX),)
MK_LIB_PREFIX	=
endif

ifeq ($(MK_LIB_EXT),)
MK_LIB_EXT    	= .lib
endif

ifeq ($(MK_OBJ_EXT),)
MK_OBJ_EXT    	= .obj
endif

ifeq ($(MK_BIN_EXT),)
MK_BIN_EXT    	= .bin
endif

ifeq ($(MK_HEX_EXT),)
MK_HEX_EXT		= .hex
endif

CC		= cl /nologo
CXX		= cl /nologo

CFLAGS   += -Ox -MDd
CXXFLAGS += -Ox -MDd

ifeq ($(MK_ADD_SYMBOLS),1)
CXXFLAGS += /Zi
CFLAGS += /Zi
endif

#DEP_OUTPUT_OPTION = -MMD -MF $(@:.o=.d)

