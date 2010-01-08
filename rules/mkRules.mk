###########################################################################
#
#	mkRules.mk
#
#	This file is the common file which includes all of the rules
#	required to build.
#
###########################################################################

ifneq ($(lastword a b),b)
$(error These Makefiles require make 3.81 or newer)
endif

ifneq ($(MK_ENV_INCLUDED),1)
include $(dir $(lastword $(MAKEFILE_LIST)))mkEnv.mk
endif

#--------------------------------------------------------------------------
#
#	You can run arbitrary commands from within the context that is setup by
# 	the makefile by using:
#
#  		make exec="command to execute"
#  
# 	Note that we're also specifying the default target here.
#
# 	You can also specify arbitrary targets by using the target=foo
# 	This is useful when trying to build preprocessor (%.pp) and such.
#

ifeq ($(exec),)
ifeq ($(target),)
all: target
else
MAKECMDGOALS += $(target)
export target
all: $(target)
endif
else
MAKECMDGOALS += exec
export exec
all: exec
endif
.PHONY: all target exec

all: $(filter %/,$(MK_SRC_FILES))

#--------------------------------------------------------------------------
#
# 	Figure out what files need to be built. Note that even though all of 
#	the components of MK_LIB_NAME haven't been defined yet, this is fine 
#	because of the deferred way that make evaluates things.
#
# 	The "EXTRA" versions are files that need to be compiled, but NOT 
#	added to a library. Typically this is used when mixing libraries and 
#	executables in the same makefile.
#
# 	We use the $(addsuffix $(MK_OBJ_EXT),$(basename ...)) so that we can
#	get .c and .cpp files.
#

MK_OBJ_FILES = $(strip $(addprefix $(MK_OBJ_DIR)/, $(addsuffix $(MK_OBJ_EXT),$(basename $(filter-out %/,$(MK_SRC_FILES))))))
MK_EXTRA_OBJ_FILES = $(strip $(addprefix $(MK_OBJ_DIR)/, $(addsuffix $(MK_OBJ_EXT),$(basename $(filter-out,%/,$(MK_EXTRA_SRC_FILES))))))

MK_DEP_FILES = $(strip $(MK_OBJ_FILES:$(MK_OBJ_EXT)=.d) $(MK_EXTRA_DEP_FILES))
MK_EXTRA_DEP_FILES = $(MK_EXTRA_OBJ_FILES:$(MK_OBJ_EXT)=.d)

#--------------------------------------------------------------------------
#
#	If the calling makefile is building a library, then it should define
#	MK_LIB_BASE, and a rule to build the library will automatically be 
#	defined.
#

ifneq ($(MK_LIB_BASE),)
MK_LIB_NAME  = $(call MK_MAKE_LIB_NAME,$(MK_LIB_BASE))
endif

#--------------------------------------------------------------------------
#
# Get the OS specific stuff
#

MK_OS_RULES_FILE = $(MK_RULES_DIR)/mkRules-$(MK_OS).mk
include $(MK_OS_RULES_FILE)

#--------------------------------------------------------------------------
#
# 	Since MK_LIB_NAME can't be evaluated until the OS specific rules have
# 	been included, we need to put the targets that use MK_LIB_NAME after
# 	the include.
#

ifneq ($(MK_LIB_NAME),)
lib_target: $(MK_LIB_NAME)
target: lib_target
.PHONY: lib_target
endif

ifneq ($(MK_ELF_TARGET),)
target: elf_target
elf_target: $(MK_ELF_TARGET)
.PHONY: elf_target
endif

ifneq ($(MK_HEX_TARGET),)
target: hex_target
hex_target: $(MK_HEX_TARGET)
.PHONY: hex_target
endif

#--------------------------------------------------------------------------
#
#	The FORCE target can be used for things that you want to have rebuild
# 	every single time (like *.pp files)
#

FORCE:

.PHONY: FORCE

#--------------------------------------------------------------------------
#
# Include dependencies.
#
# The '-' at the beginning tells make not to complain about missing
# dependency files.
#

ifneq ($(MK_DEP_FILES),)
ifeq ($(strip $(filter clean% exec print-%, $(MAKECMDGOALS))),)
-include $(MK_DEP_FILES)
endif
endif

#--------------------------------------------------------------------------
#
# 	You can run arbitrary commands from within the context that is setup 
#	by the makefile by using:
#
#  make exec="command to execute"
#  

exec:
	 @$(ECHO) "About to execute '$(exec)' ..."
	 $(exec)

#--------------------------------------------------------------------------
#
#	Targets for cleaning stuff up
#

clean: clean-dirs clean-obj clean-dep clean-lib clean-bin

clean-dirs: $(filter %/,$(MK_SRC_FILES))
				  
clean-dep:
clean-bin:
clean-lib:
clean-obj:

MK_CLEAN_DESCR = $(strip $(MK_LIB_BASE) $(MK_ELF_TARGET) $(MK_HEX_TARGET))

ifneq ($(MK_DEP_FILES),)
clean-dep: mk_clean-dep
mk_clean-dep:
	@$(ECHO) "Removing $(MK_CLEAN_DESCR) dependencies ..."
	$(Q)$(RM) -f $(MK_DEP_FILES)
	 
.PHONY: mk_clean-dep
endif

ifneq ($(MK_OBJ_FILES),)
clean-obj: mk_clean-obj
mk_clean-obj:
	 @$(ECHO) "Removing $(MK_CLEAN_DESCR) objects ..."
	 $(Q)$(RM) -f $(MK_OBJ_FILES)
	 
.PHONY: mk_clean-obj
endif

# Note: clean-bin and clean-lib are found in the OS specific rules files

.PHONY: clean clean-dirs clean-obj clean-dep clean-lib clean-bin

#--------------------------------------------------------------------------
#
#	Remove all generated directories, regardless of architeture
#

svn-clean: svn-clean-dirs svn-clean-gen
	$(Q)$(RM) -f BootHost.log

svn-clean-dirs: $(filter %/,$(MK_SRC_FILES))

svn-clean-gen:
	$(ECHO) "Removing $(MK_CLEAN_DESCR) $(MK_GEN_DIRS) ..."
	$(Q)$(RM) -rf $(MK_GEN_DIRS)

#--------------------------------------------------------------------------
#
#	Targets for installing
#

install: install-dirs install-bin

install-dirs: $(filter %/,$(MK_SRC_FILES))

install-bin:

.PHONY: install install-dirs install-bin

#--------------------------------------------------------------------------
#
# 	Print out a bunch of useful information
#
# 	Note that print-include is provided within the OS specific rules file.
#

.PHONY: print-info print-cfg print-include print-path

print-info: print-cfg print-include print-path

print-cfg:
	 @$(ECHO)
	 @$(ECHO) "MK_OS        = $(MK_OS)"
	 @$(ECHO) "MK_HOST_OS   = $(MK_HOST_OS)"
	 @$(ECHO) "MK_ROOT      = $(MK_ROOT)"
	 @$(ECHO) "MK_OBJ_DIR   = $(MK_OBJ_DIR)"
	 @$(ECHO) "MK_LIB_DIR   = $(MK_LIB_DIR)"
	 @$(ECHO) "MK_BIN_DIR   = $(MK_BIN_DIR)"
	 @$(ECHO) "MK_RULES_DIR = $(MK_RULES_DIR)"
	 @$(ECHO)

print-path:
	 @$(ECHO)
	 @$(ECHO) -e "PATH =\n  $(subst :,\n  ,$(PATH))"
	 @$(ECHO)

print-lib:
	 @$(ECHO) "MK_LIB_BASE = $(MK_LIB_BASE)"
	 @$(ECHO) "MK_LIB_NAME = $(MK_LIB_NAME)"
	 
print-src:
	 @$(ECHO) "MK_SRC_FILES = $(MK_SRC_FILES)"

print-obj:
	 @$(ECHO) "MK_OBJ_FILES = $(MK_OBJ_FILES)"

print-dep:
	 @$(ECHO) "MK_DEP_FILES = $(MK_DEP_FILES)"

print-def:
	 @$(ECHO) "The following defines are built into the $(CC) compiler"
	 touch __empty__.c
	 @$(CC) -E -Wp,-dM __empty__.c
	 @$(RM) -f __empty__.c

#--------------------------------------------------------------------------
#
# 	Rule to build subdirectories
#

ifneq ($(MK_DIR_PREFIX),)
MK_DIR_PREFIX := $(MK_DIR_PREFIX)/
endif

%/: FORCE
	$(ECHO) "Making $(MK_DIR_PREFIX)$@ ..."
	$(Q)$(MAKE) MK_DIR_PREFIX=$(MK_DIR_PREFIX)$@ -C $@ $(MAKECMDGOALS)

include $(MK_RULES_DIR)/svn-version.mk
