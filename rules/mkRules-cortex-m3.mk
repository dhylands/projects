###########################################################################
#
#	mkRules-cortex-m3.mk
#
#	This file contains rules for building under WinAVR
#
###########################################################################

include $(MK_RULES_DIR)/mkRules-gcc.mk

ifneq ($(MK_ELF_TARGET),)

download d : $(call MK_BIN_NAME, $(MK_ELF_TARGET))
	$(ECHO) "Downloading $< ..."
	$(Q)$(LMIFLASH) -f $< -v -r

.PHONY: download d

endif

#mon: 
#	$(ECHO) "Launching BootHost ..."
#	$(Q)$(BOOTHOST) $(BOOTHOST_OPTS) -b 38400
#
#.PHONY: mon

