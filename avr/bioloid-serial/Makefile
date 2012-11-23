###########################################################################
#
# bioloid Makefile
#
###########################################################################

ifeq ($(TARGET),)

TARGETS = bioloid-128

all $(MAKECMDGOALS): $(TARGETS)

$(TARGETS):
	@echo "Making $(MK_DIR_PREFIX) $@ $(MAKECMDGOALS) ..."
	@$(MAKE) --no-print-directory TARGET=$@ $(MAKECMDGOALS)

.PHONY: all $(TARGETS) $(MAKECMDGOALS)

else


MK_OS			= avr
MK_AVR_MCU		= m164p
MK_AVR_FREQ		= 16MHz
MK_AVR_PROGRAMMER  ?= stk500

MK_HEX_TARGET	= $(TARGET)
MK_SRC_FILES	= $(TARGET).c UART.c bioloid-uart.c Timer.c

include ../../rules/mkRules.mk

endif
