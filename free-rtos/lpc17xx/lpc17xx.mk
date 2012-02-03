###########################################################################
#
#	lpc17xx.mk
#
#	Contains definitions specific to the NXP LPC17xx
#
###########################################################################

LPC17XX_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

CROSS_COMPILE	= arm-bare_newlib_cortex_m3_nommu-eabi-

LD_SCRIPT	= $(LPC17XX_DIR)/lpc17xx.ld

CPPFLAGS	= -I. -I$(LPC17XX_DIR) -I$(FREE_RTOS_SRC)/include -I$(FREE_RTOS_SRC)/portable/GCC/ARM_CM3

CFLAGS		= -mthumb         \
		  -mcpu=cortex-m3 \
		  -O1             \
		  -g		  \
		  -DPACK_STRUCT_END=__attribute\(\(packed\)\) \
		  -DALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) \
		  -MD

include $(LPC17XX_DIR)/../free-rtos.mk

vpath	%.c	$(LPC17XX_DIR) $(FREE_RTOS_SRC) $(FREE_RTOS_SRC)/portable/MemMang $(FREE_RTOS_SRC)/portable/GCC/ARM_CM3

