#############################################################################
#
#	Makefile for building the hello-module sample module
#
#############################################################################

ifeq ($(DEBUG),y)
	EXTRA_CFLAGS += -O -g		# -O is need to expand inlines
else
	EXTRA_CFLAGS += -O2
endif

ifneq ($(KERNELRELEASE),)

#----------------------------------------------------------------------------
# call from kernel build system

obj-m	:= hello-module.o

#
#----------------------------------------------------------------------------
else
#----------------------------------------------------------------------------
#

PWD                 := $(shell pwd)

GUMSTIX_BUILDROOT   = $(PWD)/../gumstix-buildroot
BUILD_ARM           = $(wildcard $(GUMSTIX_BUILDROOT)/build_arm*)
KERNELDIR 		    ?= $(wildcard $(BUILD_ARM)/linux-*)
CROSS_COMPILE		= $(BUILD_ARM)/staging_dir/bin/arm-linux-

kernel-module:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers

#
#----------------------------------------------------------------------------
endif


