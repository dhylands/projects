#############################################################################
#
#	Makefile for building the gpio-event module, and app
#
#############################################################################

SUBDIRS = module app

CLEAN_SUBDIRS = $(SUBDIRS)

all: $(SUBDIRS)
clean: $(SUBDIRS)

.PHONY: $(SUBDIRS)

$(SUBDIRS) : 
	$(MAKE) -C $@ $(MAKECMDGOALS)

