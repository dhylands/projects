###########################################################################
#
#	mkRules-avr.mk
#
#	This file contains rules for building under WinAVR
#
###########################################################################

#MK_PRINT_ELF_SIZE = avr-size --mcu=$(MK_AVR_MCU_LONG) $@
MK_PRINT_ELF_SIZE = avr-size $@

include $(MK_RULES_DIR)/mkRules-gcc.mk

ifneq ($(MK_HEX_TARGET),)

download d : mkdir_bin mkdir_obj pgm

pgm: $(call MK_HEX_NAME, $(MK_HEX_TARGET))
	$(ECHO) "Downloading $< ..."
	$(MK_$(MK_AVR_PROGRAMMER)_CMD)

MK_AVR_FUSES = $(MK_AVR_FUSES_$(MK_AVR_MCU))

fuses:
ifeq ($(MK_AVR_FUSES),)
	$(error Need to set MK_AVR_FUSES_$(MK_AVR_MCU))
else
	$(ECHO) "Programming fuses to $(MK_AVR_FUSES) ..."
ifeq ($(MK_$(MK_AVR_PROGRAMMER)_CMD_FUSE),)
	$(error Programmer $(MK_AVR_PROGRAMMER) doesn't support programming fuses)
endif
	$(MK_$(MK_AVR_PROGRAMMER)_CMD_FUSE)
endif

read-fuses:
	$(ECHO) "Reading fuses ..."
ifeq ($(MK_$(MK_AVR_PROGRAMMER)_RD_FUSE),)
	$(error Programmer $(MK_AVR_PROGRAMMER) doesn't support reading fuses)
endif
	$(MK_$(MK_AVR_PROGRAMMER)_RD_FUSE)

read-flash:
	$(ECHO) "Reading flash ..."
ifeq ($(MK_$(MK_AVR_PROGRAMMER)_RD_PGM),)
	$(error Programmer $(MK_AVR_PROGRAMMER) doesn't support reading program)
endif
	$(MK_$(MK_AVR_PROGRAMMER)_RD_PGM)

read-eeprom:
	$(ECHO) "Reading eeprom ..."
ifeq ($(MK_$(MK_AVR_PROGRAMMER)_RD_EEPROM),)
	$(error Programmer $(MK_AVR_PROGRAMMER) doesn't support reading eeprom)
endif
	$(MK_$(MK_AVR_PROGRAMMER)_RD_EEPROM)

.PHONY: download d d_target fuses

endif

mon: 
	$(ECHO) "Launching BootHost ..."
	$(Q)$(MK_BootHost) $(MK_BootHost_OPTS)

.PHONY: mon

