###########################################################################
#
# bioloid Makefile
#
# NOTE: This program was an early investigation program and was used for
#	developing common/bioloid-uart.c. User avr/bioloid-serial instead
#	(it uses bioloid-uart.c)
#
###########################################################################

ifeq ($(TARGET),)

TARGETS = bioloid-rs bioloid-128 serial-echo

all $(MAKECMDGOALS): $(TARGETS)

$(TARGETS):
	@echo "Making $(MK_DIR_PREFIX) $@ $(MAKECMDGOALS) ..."
	@$(MAKE) --no-print-directory TARGET=$@ $(MAKECMDGOALS)

.PHONY: all $(TARGETS) $(MAKECMDGOALS)

else


MK_OS			= avr
MK_AVR_MCU		= m128
MK_AVR_FREQ		= 16MHz
MK_AVR_PROGRAMMER  ?= stk500

MK_HEX_TARGET	= $(TARGET)
MK_SRC_FILES	= $(TARGET).c

include ../../rules/mkRules.mk

endif
