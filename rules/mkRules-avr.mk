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
	$(Q)$(BOOTHOST) $(BOOTHOST_OPTS) -b 38400 $<

.PHONY: download d

endif

mon: 
	$(ECHO) "Launching BootHost ..."
	$(Q)$(BOOTHOST) $(BOOTHOST_OPTS) -b 38400

.PHONY: mon

