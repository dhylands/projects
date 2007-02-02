###########################################################################
#
#	mkRules-avr.mk
#
#	This file contains rules for building under WinAVR
#
###########################################################################

MK_PRINT_ELF_SIZE = avr-size --mcu=$(MK_AVR_MCU_LONG) $@

include $(MK_RULES_DIR)/mkRules-gcc.mk

ifneq ($(MK_HEX_TARGET),)

download d : mkdir_bin mkdir_obj d_target

d_target: $(call MK_HEX_NAME, $(MK_HEX_TARGET))
	$(ECHO) "Downloading $< ..."
ifeq ($(MK_AVR_PROGRAMMER),STK500)
	$(STK500) -cUSB -d$(MK_AVR_MCU_LONG) -e -if$< -pf
else
	$(Q)$(BOOTHOST) $(BOOTHOST_OPTS) -b 38400 $<
endif

MK_AVR_FUSES = $(MK_AVR_FUSES_$(MK_AVR_MCU))

fuses:
ifeq ($(MK_AVR_FUSES),)
	$(error Need to set MK_AVR_FUSES_$(MK_AVR_MCU))
else
	$(ECHO) "Programming fuses to $(MK_AVR_FUSES) ..."
ifeq ($(MK_AVR_PROGRAMMER),STK500)
	$(STK500) -cUSB -d$(MK_AVR_MCU_LONG) -E$(word 1,$(MK_AVR_FUSES)) -f$(word 2,$(MK_AVR_FUSES))$(word 3,$(MK_AVR_FUSES))
else
	$(error Programmer $(MK_AVR_PROGRAMMER) doesn't support programming fuses)
endif
	
endif

.PHONY: download d d_target fuses

endif

mon: 
	$(ECHO) "Launching BootHost ..."
	$(Q)$(BOOTHOST) $(BOOTHOST_OPTS) -b 38400

.PHONY: mon

