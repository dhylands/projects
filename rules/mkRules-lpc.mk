###########################################################################
#
#	mkRules-lpc.mk
#
#	This file contains rules for building under using the ARM/LPC toolchain
#
###########################################################################

MK_PRINT_ELF_SIZE = arm-elf-size $@

include $(MK_RULES_DIR)/mkRules-gcc.mk

ifneq ($(MK_HEX_TARGET),)

download d : mkdir_bin mkdir_obj d_target

d_target: $(call MK_HEX_NAME, $(MK_HEX_TARGET))
	$(ECHO) "Downloading $< ..."
	$(MK_$(MK_LPC_PROGRAMMER)_CMD)

endif

.PHONY: download d d_target

