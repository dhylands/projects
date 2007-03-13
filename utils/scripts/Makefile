###########################################################################
#
#	Makefile to install the scripts
#
###########################################################################

MK_OS = none

MK_SCRIPT_FILES = $(filter-out Makefile, $(wildcard *))

include ../../rules/mkRules.mk

install: install-scripts

install-scripts: $(addprefix $(MK_UTILS_DIR)/,$(MK_SCRIPT_FILES))

$(MK_UTILS_DIR)/% : %
	@$(ECHO) "Installing $@ ..."
	$(Q)$(MKDIR) -p $(@D)
	$(Q)$(INSTALL) $< $@
	@$(ECHO)
