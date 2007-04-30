###########################################################################
#
#	Makefile to build boothost
#
###########################################################################

MK_OS			= host
MK_ELF_TARGET 	= smc

MK_SRC_FILES = \
	smc_tab.c \
	smc_lex.c \
	smc.c \
	Log.c

include ../../rules/mkRules.mk

%_tab.c: %.y
	$(ECHO) "Creating parser from $< ..."
	$(RM) -f $@
	$(Q)bison -d -o $@ $<

%_lex.c: %.l %_tab.c
	$(ECHO) "Creating lexical analyzer from $< ..."
	$(RM) -f $@
	$(Q)flex -o $@ $<

clean: clean-local

clean-local:
	$(RM) -f smc_tab.* smc_lex.c
	
