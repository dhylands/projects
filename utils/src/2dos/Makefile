###########################################################################
#
#	Makefile to build crc32
#
###########################################################################

ifeq ($(TARGET),)

TARGETS = 2dos 2unix 2mac 0d0d0a

all $(MAKECMDGOALS): $(TARGETS)

$(TARGETS):
	@echo "Making $(MK_DIR_PREFIX) $@ $(MAKECMDGOALS) ..."
	@$(MAKE) --no-print-directory TARGET=$@ $(MAKECMDGOALS)

.PHONY: all $(TARGETS) $(MAKECMDGOALS)

else

MK_OS			= host
MK_ELF_TARGET	= $(TARGET)

MK_SRC_FILES = $(TARGET).c

include ../../../rules/mkRules.mk

endif
