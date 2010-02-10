#############################################################################
#
#	Makefile for building the gpio module, lib, and app
#
#############################################################################

SUBDIRS = module lib app

CLEAN_SUBDIRS = $(SUBDIRS)

all: $(SUBDIRS)
clean: $(SUBDIRS)

.PHONY: $(SUBDIRS)

$(SUBDIRS) : 
	$(MAKE) -C $@ $(MAKECMDGOALS)

