# Makefile for building heartbeat kernel moduule

all: modules

obj-m += heartbeat.o

modules:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
