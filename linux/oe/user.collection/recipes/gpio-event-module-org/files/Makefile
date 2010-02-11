#############################################################################
#
#	Makefile for building the gpio-event kernel module
#
#############################################################################

obj-m	:= gpio-event.o

PWD = $(shell pwd)
KBUILD_FLAGS = -C $(KERNEL_PATH) M=$(PWD) KERNELRELEASE=$(KERNELRELEASE)

$(info ==============================================================)
$(info KERNEL_PATH = $(KERNEL_PATH))
$(info KERNELRELEASE = $(KERNELRELEASE))
$(info ARCH = $(ARCH))
$(info ==============================================================)
$(error Die)

default: modules

modules:
	$(MAKE) $(KBUILD_FLAGS) modules

clean:
	rm -rf *.o *~ *.ko *.mod.c

