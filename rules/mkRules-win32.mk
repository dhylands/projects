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

PREPROCESS.c = $(CC) $(CPPFLAGS) $(TARGET_ARCH) -E

OUTPUT_OPTION = -Fo$@

#--------------------------------------------------------------------------
#
# Since we can generate the dependencies as part of the compilation, we
# do that rather than generate the dependency files separately.

#--------------------------------------------------------------------------
#
# Making the .d's be phony and providing a dummy rule to make them, 
# combined with the %.o : %.c %.d causes make to work properly when the .d 
# file is removed, but the .o file isn't. make will now regenerate the .d 
# by recompiling the .o
#
	
.PHONY: $(MK_OBJ_DIR)/%.d
$(MK_OBJ_DIR)/%.d: ;

#--------------------------------------------------------------------------
#
# Compile C source files
#
#   COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
#

$(MK_OBJ_DIR)/%.obj : %.c $(MK_OBJ_DIR)/%.d
	$(ECHO) "Compiling $< ..."
	$(Q)makedepend -- $(CFLAGS) $(CPPFLAGS) -- $<
	$(Q)$(COMPILE.c) $(DEP_OUTPUT_OPTION) $(OUTPUT_OPTION) $<

#--------------------------------------------------------------------------
#
# Compile C++ source files
#
#   COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
#

$(MK_OBJ_DIR)/%.obj : %.cpp $(MK_OBJ_DIR)/%.d
	$(ECHO) "Compiling $< ..."
	rm -f $(MK_OBJ_DIR)/$*.d
	touch $(MK_OBJ_DIR)/$*.d
	$(Q)makedepend -Y -f$(MK_OBJ_DIR)/$*.d -- $(CXXFLAGS) $(CPPFLAGS) -- $<
	$(Q)$(COMPILE.cc) $(DEP_OUTPUT_OPTION) $(OUTPUT_OPTION) $<

#--------------------------------------------------------------------------
#
# Assemble source files
#
#   COMPILE.s = $(AS) $(ASFLAGS) $(TARGET_MACH)
#

$(MK_OBJ_DIR)/%.obj : %.s
	$(ECHO) "Assembling $< ..."
	$(Q)$(COMPILE.s) -c $(DEP_OUTPUT_OPTION) $(OUTPUT_OPTION) $<

#--------------------------------------------------------------------------
#
# Assemble source files which require pre-processing
#
#   COMPILE.S = $(CC) $(ASFLAGS) $(CPPFLAGS) $(TARGET_MACH) -c
#

$(MK_OBJ_DIR)/%.obj : %.S $(MK_OBJ_DIR)/%.d
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

$(call MK_HEX_NAME, $(MK_HEX_TARGET)) : $(call MK_ELF_NAME, $(MK_HEX_TARGET))
	$(ECHO) "Creating $@ ..."
	$(Q)$(OBJCOPY) $(MK_OBJCOPY_SECTS) -O ihex $< $@
	$(ECHO)

MK_LINK_TARGET += $(MK_HEX_TARGET)

endif


#--------------------------------------------------------------------------
#
# Create simple executables
#

ifneq ($(MK_ELF_TARGET),)

$(info MK_ELF_TARGET depends on $(call MK_ELF_NAME, $(MK_ELF_TARGET)))

$(MK_ELF_TARGET): mkdir_bin mkdir_obj $(call MK_ELF_NAME, $(MK_ELF_TARGET))

.PHONY: $(MK_ELF_TARGET)

MK_LINK_TARGET += $(MK_ELF_TARGET)

install-bin: install-bin_$MK_ELF_TARGET)	   
install-bin_$MK_ELF_TARGET): $(MK_ELF_TARGET) $(MK_UTILS_DIR)/$(MK_ELF_TARGET)$(MK_ELF_EXT)

.PHONY: install

#
# Install executable into utils dir
#

$(MK_UTILS_DIR)/$(MK_ELF_TARGET)$(MK_ELF_EXT) : $(call MK_ELF_NAME, $(MK_ELF_TARGET))
	@$(ECHO) "Installing $@ ..."
	$(Q)$(MKDIR) -p $(@D)
	$(Q)$(INSTALL) --strip $< $@
	@$(ECHO)

endif

ifneq ($(MK_LINK_TARGET),)

ifneq ($(filter %.cpp,$(MK_SRC_FILES)),)
LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)
endif

#
# Link executable (aka .elf) file from object files
#

$(info $(call MK_ELF_NAME, $(MK_LINK_TARGET)) depends on $(MK_OBJ_FILES))

$(call MK_ELF_NAME, $(MK_LINK_TARGET)) : $(MK_CRT_FILES) $(MK_OBJ_FILES)
	$(ECHO) "Linking $@ ..."
	$(Q)$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) /Fe$@ $(LD_CRTFLAGS)
	$(ECHO)
ifneq ($(MK_PRINT_ELF_SIZE),)
	$(Q)$(MK_PRINT_ELF_SIZE)
	$(ECHO)
endif

#
# Create .bin file from .elf file
#

$(call MK_BIN_NAME, $(MK_LINK_TARGET)) : $(call MK_ELF_NAME, $(MK_LINK_TARGET))
	$(ECHO) "Creating $@ ..."
	$(Q)$(OBJCOPY) -O binary $< $@	

clean-bin: clean-link_$(MK_LINK_TARGET)
clean-link_$(MK_LINK_TARGET):
	$(ECHO) "Removing $(MK_LINK_TARGET) bin files..."
	$(Q)$(RM) -rf $(MK_BIN_DIR)

endif

