###########################################################################
#
# 	mkEnv-avr.mk
#
# 	This file contains AVR specific defines for the build environment.
#
# 	This file should only define variables and not have any rules in it.
#
###########################################################################

MK_CROSS_COMPILE		= avr-

ifeq ($(MK_AVR_MCU),)
$(error MK_AVR_MCU not specified)
endif

MK_AVR_MCU_LONG = $(patsubst t%,attiny%,$(patsubst m%,atmega%,$(MK_AVR_MCU)))

$(info MK_AVR_MCU = $(MK_AVR_MCU))
$(info MK_AVR_MCU_LONG = $(MK_AVR_MCU_LONG))

ifeq ($(MK_AVR_FREQ),)
$(error MK_AVR_FREQ not specified)
endif

MK_OS_DIR = avr-$(MK_AVR_MCU)-$(MK_AVR_FREQ)
MK_AVR_FREQ_HZ  = $(patsubst %MHz,%000000,$(MK_AVR_FREQ))
MK_AVR_FREQ_DEF = -DCFG_CPU_CLOCK=$(MK_AVR_FREQ_HZ) -DF_CPU=$(MK_AVR_FREQ_HZ)

MK_AVR_MCU_FLAGS	= -mmcu=$(MK_AVR_MCU_LONG)

MK_COMMON_AVR_DIR	= $(MK_COMMON_DIR)/avr

CPPFLAGS += $(MK_AVR_MCU_FLAGS) -I $(MK_COMMON_AVR_DIR) $(MK_AVR_FREQ_DEF)
LDFLAGS  += $(MK_AVR_MCU_FLAGS)

CXXFLAGS += -fno-exceptions -fno-rtti

# On the AVR, we always optimize for size

CFLAGS += -Os
CXXFLAGS += -Os

MK_PRINT_BIN_SIZE	= avr-mem.sh $@ $(MK_AVR_MCU_LONG)
MK_OBJCOPY_SECTS	= -j .text -j .data

vpath %.c   $(MK_COMMON_AVR_DIR)
vpath %.cpp $(MK_COMMON_AVR_DIR)

include $(MK_RULES_DIR)/mkEnv-gcc.mk

# avr-gcc doesn't use rdynamic
LDFLAGS_RDYNAMIC =

# Note: MK_AVR_FUSES is XX HH LL

MK_avrdude	 		= avrdude
MK_avrdude_PORT		?= usb
MK_avrdude_PROG    	?= avrispmkII
MK_avrdude_CMD		= $(Q)$(MK_avrdude) -P $(MK_avrdude_PORT) -c $(MK_avrdude_PROG) -p $(MK_AVR_MCU_LONG) -U flash:w:$<:a
MK_avrdude_CMD_FUSE	= $(Q)$(MK_avrdude) -P $(MK_avrdude_PORT) -c $(MK_avrdude_PROG) -p $(MK_AVR_MCU_LONG) -U efuse:w:0x$(word 1,$(MK_AVR_FUSES)):m -U hfuse:w:0x$(word 2,$(MK_AVR_FUSES)):m -U lfuse:w:0x$(word 3,$(MK_AVR_FUSES)):m
MK_avrdude_RD_FUSE	= $(Q)$(MK_avrdude) -P $(MK_avrdude_PORT) -c $(MK_avrdude_PROG) -p $(MK_AVR_MCU_LONG) -v
MK_avrdude_RD_PGM	= $(Q)$(MK_avrdude) -P $(MK_avrdude_PORT) -c $(MK_avrdude_PROG) -p $(MK_AVR_MCU_LONG) -U flash:r:flash.hex:i
MK_avrdude_RD_EEPROM= $(Q)$(MK_avrdude) -P $(MK_avrdude_PORT) -c $(MK_avrdude_PROG) -p $(MK_AVR_MCU_LONG) -U eeprom:r:eeprom.hex:i

MK_BootHost 		= boothost
MK_BootHost_BAUD	?= 38400
MK_BootHost_PORT	?= ttyS0
MK_BootHost_OPTS	?= --baud=$(MK_BootHost_BAUD) --port=$(MK_BootHost_PORT) -r
MK_BootHost_CMD		= $(Q)$(MK_BootHost) $(MK_BootHost_OPTS) $<

MK_stk500	 		= "c:/Program Files/Atmel/AVR Tools/stk500/stk500.exe"
MK_stk500_PORT		?= USB
MK_stk500_CMD 		= $(Q)$(MK_stk500) -c$(MK_stk500_PORT) -d$(MK_AVR_MCU_LONG) -e -if$< -pf
MK_stk500_CMD_FUSE	= $(Q)$(MK_stk500) -c$(MK_stk500_PORT) -d$(MK_AVR_MCU_LONG) -E$(word 1,$(MK_AVR_FUSES)) -f$(word 2,$(MK_AVR_FUSES))$(word 3,$(MK_AVR_FUSES))
MK_stk500_RD_FUSE	= $(Q)$(MK_stk500) -c$(MK_stk500_PORT) -d$(MK_AVR_MCU_LONG) -s -q

MK_teensy			= teensy_loader_cli
MK_teensy_CMD		= $(Q)$(MK_teensy) -mmcu=$(MK_AVR_MCU_LONG) -v -w $<

