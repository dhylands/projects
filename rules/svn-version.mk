###########################################################################
#
# svn-version.mk - Creates the svn-version.h file
#
###########################################################################

#--------------------------------------------------------------------------
#
# Rule to update svn-version.h file. If a C file includes this file then
# the dependcy generator will cause this file to be generated.
#
# To prevent unnecessary updates, a temporary file is created and compared
# to svn-version.h, so that svn-version.h is only touched if a change
# is detected.
#

ifeq ($(findstring not found,$(shell type svnversion$(MK_EXE_EXT) 2>&1)),not found)
svn-revision := 0
else
svn-revision = $(shell svnversion)
ifeq ($(strip $(svn-revision)),exported)
svn-revision := 0
endif
endif

ifeq ($(verbose),1)
$(info svn-revision = $(svn-revision))
endif

svn-version.h: FORCE
	@echo "Creating svn-version.h ..."
	@echo "/*" > svn-version.tmp
	@echo " * Generated file - Do NOT edit" >> svn-version.tmp
	@echo " */" >> svn-version.tmp
	@echo >> svn-version.tmp
	@echo '#define	SVN_REVISION "'$(svn-revision)'"' >> svn-version.tmp
	@cmp --quiet svn-version.tmp svn-version.h || cp svn-version.tmp svn-version.h
	@rm svn-version.tmp

ifneq ($(wildcard svn-version.h),)
clean: clean-svn-version

clean-svn-version:
	@echo "Removing svn-version.h ..."
	@rm svn-version.h
endif

