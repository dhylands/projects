###########################################################################
#
#	Makefile to build the state machine compiler (smc)
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

smc_tab.c: smc_tab.y
	$(ECHO) "Creating parser from $< ..."
	$(RM) -f $@
	$(Q)bison -d -o $@ $<

smc_lex.c: smc_lex.l smc_tab.c
	$(ECHO) "Creating lexical analyzer from $< ..."
	$(RM) -f $@
	$(Q)flex -o $@ $<

target: stm-test

.PHONY: stm-test

stm-test: cruise-sm.c stm-test.c
	$(MAKE) -f stm-test.mk	

cruise-sm.c : cruise.smc $(call MK_ELF_NAME, $(MK_ELF_TARGET))
	$(ECHO) "Compiling state machine $< ..."
	$(call MK_ELF_NAME, $(MK_ELF_TARGET)) $<

squeaky-clean: svn-clean
	$(ECHO) "Removing genenrated sources..."
	$(RM) -f smc_tab.[ch] smc_lex.c cruise-sm.[ch]
	
