###########################################################################
#
#	free-rtos.mk
#
#	Contains definitions common to all Free-RTOS builds.
#
###########################################################################

#--------------------------------------------------------------------------
#
# 	Run make with v=1 or verbose=1 to get verbose output
#

ifeq ($(v),)
export verbose = 0
else
export verbose = 1
endif

ifeq ($(verbose),)
export verbose = 0
endif

ifeq ($(verbose),0)
	Q = @
	MAKEFLAGS += -s
else
	Q =
endif
export Q

#--------------------------------------------------------------------------

FREE_RTOS	= /home/dhylands/software/FreeRTOSV7.1.0
FREE_RTOS_SRC	= $(FREE_RTOS)/Source

#
# Do some sanity checks on the FREE_RTOS directory structure
#

ifeq (,$(wildcard $(FREE_RTOS)))
$(error FREE_RTOS '$(FREE_RTOS)' doesn't exist)
endif
ifeq (,$(wildcard $(FREE_RTOS_SRC)))
$(error FREE_RTOS_SRC '$(FREE_RTOS_SRC)' doesn't exist)
endif
ifeq (,$(wildcard $(FREE_RTOS_SRC)/include/FreeRTOS.h))
$(error Unable to find '$(FREE_RTOS_SRC)/include/FreeRTOS.h')
endif

CC 	= $(CROSS_COMPILE)gcc
LD 	= $(CROSS_COMPILE)ld
OBJCOPY	= $(CROSS_COMPILE)objcopy

LIBGCC	= $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
LIBC	= $(shell $(CC) $(CFLAGS) -print-file-name=libc.a)

OBJS = $(addprefix obj/,$(SRCS:%.c=%.o))
DEPS = $(addprefix obj/,$(SRCS:%.c=%.d))

all : obj/$(TARGET).bin

obj/%.o : %.c
	echo "Compiling $< ..."
	$(Q)mkdir -p $(@D)
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

obj/$(TARGET).elf : $(OBJS)
	echo "Linking $@ ..."
	$(Q)$(LD) -T $(LD_SCRIPT) -Map ${@:%.elf=%.map} -o $@ $^ $(LIBC) $(LIBGCC)

obj/%.bin : obj/%.elf
	echo "Creating $@ from $<"
	$(Q)$(OBJCOPY) -O binary $< $@

clean:
	echo "Removing obj directory ..."
	$(Q)rm -rf obj

ifeq ($(strip $(filter clean% exec print-%, $(MAKECMDGOALS))),)
-include $(DEPS)
endif

