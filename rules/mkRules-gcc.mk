###########################################################################
#
#	mkRules-gcc.mk
#
#	This file contains rules for building using a gcc toolchain
#
###########################################################################

#--------------------------------------------------------------------------
#
#	CFLAGS 		= flags used for C compiler
#	CXXFLAGS	= flags for the C++ compiler
#	CPPFLAGS	= flags for the C preprocessor (-D, -I)
#

PREPROCESS.c = $(CC) $(CPPFLAGS) $(TARGET_ARCH) -E -Wp,-C,-dD,-dI

#--------------------------------------------------------------------------
#
# Since we can generate the dependencies as part of the compilation, we
# do that rather than generate the dependency files separately.
		   
#--------------------------------------------------------------------------
#
# Compile C source files
#
#   COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
#

$(MK_OBJ_DIR)/%.o : %.c
	$(ECHO) "Compiling $< ..."
	$(Q)$(COMPILE.c) $(DEP_OUTPUT_OPTION) $(OUTPUT_OPTION) $<

#--------------------------------------------------------------------------
#
# Compile C++ source files
#
#   COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
#

$(MK_OBJ_DIR)/%.o : %.cpp
	$(ECHO) "Compiling $< ..."
	$(Q)$(COMPILE.cc) $(DEP_OUTPUT_OPTION) $(OUTPUT_OPTION) $<

#--------------------------------------------------------------------------
#
# Assemble source files
#
#   COMPILE.s = $(AS) $(ASFLAGS) $(TARGET_MACH)
#

$(MK_OBJ_DIR)/%.o : %.s
	$(ECHO) "Assembling $< ..."
	$(Q)$(COMPILE.s) $(DEP_OUTPUT_OPTION) $(OUTPUT_OPTION) $<

#--------------------------------------------------------------------------
#
# Assemble source files which require pre-processing
#
#   COMPILE.S = $(CC) $(ASFLAGS) $(CPPFLAGS) $(TARGET_MACH) -c
#

$(MK_OBJ_DIR)/%.o : %.S
	$(ECHO) "Assembling $< ..."
	$(Q)$(COMPILE.S) $(DEP_OUTPUT_OPTION) $(OUTPUT_OPTION) $<

#--------------------------------------------------------------------------
#
# Generate Preprocessed files from C/C++ source (useful for debugging)
#

%.pp : %.c FORCE
	$(ECHO) "Preprocessing $< ..."
	$(Q)$(PREPROCESS.c) $< > $@

%.pp : %.cpp FORCE
	$(ECHO) "Preprocessing $< ..."
	$(Q)$(PREPROCESS.c) $< > $@

#--------------------------------------------------------------------------
#
# Generate Assembly listing files from C/C++ source (useful for debugging)
#

%.cod : %.c FORCE
	$(ECHO) "Listing $< ..."
	$(Q)$(COMPILE.c) -gstabs -Wa,-ahldms=$@ $< -o $(MK_OBJDIR)/$(@:.cod=.o)

%.cod : %.cpp FORCE
	$(ECHO) "Preprocessing $< ..."
	$(Q)$(PREPROCESS.c) $< > $@

#--------------------------------------------------------------------------
#
# Make the object directory
#

mkdir_obj: $(MK_OBJ_DIR)

$(MK_OBJ_DIR):
	$(ECHO) "Creating directory $(MK_OBJ_DIR) ..."
	$(Q)$(MKDIR) -p $(MK_OBJ_DIR)

.PHONY: mkdir_obj

#--------------------------------------------------------------------------
#
# Make the executable directory
#

mkdir_bin: $(MK_BIN_DIR)

$(MK_BIN_DIR):
	$(ECHO) "Creating directory $(MK_BIN_DIR) ..."
	$(Q)$(MKDIR) -p $(MK_BIN_DIR)

.PHONY: mkdir_bin

#--------------------------------------------------------------------------
#
# Create the project archive file
#

ifneq ($(MK_LIB_BASE),)

$(MK_LIB_BASE) : mkdir_obj 	$(call MK_LIB_NAME, $(MK_LIB_BASE))

$(call MK_LIB_NAME, $(MK_LIB_BASE)) : $(MK_OBJ_FILES)
	$(ECHO) "Creating Archive $@ ..."
	$(Q)$(MKDIR) -p $(@D)
	$(Q)$(RM) -f $@
	$(Q)$(AR) -rcs $@ $^
	$(ECHO)

clean-lib : clean-lib_$(MK_LIB_BASE)
clean-lib_$(MK_LIB_BASE) :
	$(ECHO) "Removing $(MK_LIB_BASE) library ..."
	$(Q)$(RM) -f $(MK_LIB_NAME)

.PHONY: clean-lib_$(MK_LIB_BASE)

endif

#--------------------------------------------------------------------------
#
# Create a hex file from an elf file
#

ifneq ($(MK_HEX_TARGET),)

$(MK_HEX_TARGET): mkdir_bin mkdir_obj $(call MK_HEX_NAME, $(MK_HEX_TARGET))

.PHONY: $(MK_HEX_TARGET)

$(call MK_HEX_NAME, $(MK_HEX_TARGET)) : $(call MK_BIN_NAME, $(MK_HEX_TARGET))
	$(ECHO) "Creating $@ ..."
	$(Q)$(OBJCOPY) -j .text -j .data -O ihex $< $@
	$(ECHO)

MK_LINK_TARGET += $(MK_HEX_TARGET)

endif


#--------------------------------------------------------------------------
#
# Create simple executables
#

ifneq ($(MK_BIN_TARGET),)

$(MK_BIN_TARGET): mkdir_bin mkdir_obj $(call MK_BIN_NAME, $(MK_BIN_TARGET))

.PHONY: $(MK_BIN_TARGET)

MK_LINK_TARGET += $(MK_BIN_TARGET)

install-bin: install-bin_$MK_BIN_TARGET)	   
install-bin_$MK_BIN_TARGET): $(MK_BIN_TARGET) $(MK_UTILS_DIR)/$(MK_BIN_TARGET)$(MK_BIN_EXT)

.PHONY: install

$(MK_UTILS_DIR)/$(MK_BIN_TARGET)$(MK_BIN_EXT) : $(call MK_BIN_NAME, $(MK_BIN_TARGET))
	@$(ECHO) "Installing $@ ..."
	$(Q)$(INSTALL) $< $@
	@$(ECHO)

endif

ifneq ($(MK_LINK_TARGET),)

ifneq ($(filter %.cpp,$(MK_SRC_FILES)),)
LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)
endif

$(call MK_BIN_NAME, $(MK_LINK_TARGET)) : $(MK_OBJ_FILES)
	$(ECHO) "Linking $@ ..."
	$(Q)$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
	$(ECHO)
ifneq ($(MK_PRINT_BIN_SIZE),)
	$(Q)$(MK_PRINT_BIN_SIZE)
	$(ECHO)
endif

clean-bin: clean-link_$(MK_LINK_TARGET)
clean-link_$(MK_LINK_TARGET):
	$(ECHO) "Removing $(MK_LINK_TARGET) bin file..."
	$(Q)$(RM) -rf $(MK_BIN_DIR)

endif

