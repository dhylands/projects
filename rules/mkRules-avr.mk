###########################################################################
#
#	mkRules-avr.mk
#
#	This file contains rules for building under WinAVR
#
###########################################################################

include $(MK_RULES_DIR)/mkRules-gcc.mk

ifneq ($(MK_HEX_TARGET),)

download d : $(call MK_HEX_NAME, $(MK_HEX_TARGET))
	$(ECHO) "Downloading $< ..."
ifeq ($(MK_AVR_PROGRAMMER),STK500)
	$(STK500) -cUSB -d$(MK_AVR_MCU_LONG) -e -if$< -pf
else
	$(Q)$(BOOTHOST) $(BOOTHOST_OPTS) -b 38400 $<
endif

.PHONY: download d

endif

mon: 
	$(ECHO) "Launching BootHost ..."
	$(Q)$(BOOTHOST) $(BOOTHOST_OPTS) -b 38400

.PHONY: mon

