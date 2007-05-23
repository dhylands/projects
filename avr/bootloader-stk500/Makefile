###########################################################################
#
# STK500 compatible serial bootloader
#
###########################################################################

MK_OS               = avr
MK_AVR_PROGRAMMER  ?= stk500
MK_stk500_PORT		= COM1

ifeq ($(TARGET),)

# No TARGET specified, so use some default

MK_AVR_MCU          = m16
MK_AVR_FREQ         = 4MHz
all:

else

# For Stinger we set the LED up to the green LED
# and we set SW_OPT to be empty so we don't have an override switch

Stinger_MCU			= m8
Stinger_FREQ		= 16MHz
Stinger_LED			= B 3
Stinger_SW			=

MK_AVR_MCU			= $($(TARGET)_MCU)
MK_AVR_FREQ			= $($(TARGET)_FREQ)
MK_OBJ_DIR			= ./obj/$(TARGET)
MK_BIN_DIR			= ./bin/$(TARGET)

endif

MK_HEX_TARGET       = Boot-$(MK_AVR_MCU)-$(MK_AVR_FREQ)
MK_SRC_FILES        = start.c boot.c

# Determnine the starting address for the bootloader. Note that this is a byte
# address, whereas the datasheet uses a word address.
#
# The fiinal optimized bootloader should fit in under 1K bytes.

BOOT_START_m8   = 0x1C00
BOOT_START_m16  = 0x3C00
BOOT_START_m168 = 0x3C00
BOOT_START_m32  = 0x7C00
BOOT_START_m128 = 0x1F000

MK_AVR_FUSES_m8     = FF D8 E4	# 1k bootloader
MK_AVR_FUSES_m168   = FA DF E2	# 1k bootloader
MK_AVR_FUSES_m128   = FF C2 BF	# 4k bootloader
                      
ifeq ($(TARGET),)

# Setup the default status LED. We use SS, except on the ATMega8/48/88/168 
# where SS is shared with OC1B which is normally used as a motor control

LED_m8			= D 7
LED_m16			= B 4
LED_m168		= D 7
LED_m32			= B 4

# G.4 is Red LED on robsotix				   
LED_m128   		= G 4

ifeq ($(LED_$(MK_AVR_MCU)),)
$(error Need to set LED_PORT for $(MK_AVR_MCU))
endif

LED_PORT        = PORT$(word 1, $(LED_$(MK_AVR_MCU)))
LED_DDR         = DDR$(word 1, $(LED_$(MK_AVR_MCU)))
LED             = P$(word 1, $(LED_$(MK_AVR_MCU)))$(word 2, $(LED_$(MK_AVR_MCU)))
LED_OPT			= -DLED_DDR=$(LED_DDR) -DLED_PORT=$(LED_PORT) -DLED=$(LED)

else

ifeq ($($(TARGET)_LED),)
LED_OPT			=
else

LED_PORT		= PORT$(word 1, $($(TARGET)_LED))
LED_DDR			= DDR$(word 1, $($(TARGET)_LED))
LED				= P$(word 1, $($(TARGET)_LED))$(word 2, $($(TARGET)_LED))
LED_OPT			= -DLED_DDR=$(LED_DDR) -DLED_PORT=$(LED_PORT) -DLED=$(LED)

endif
endif

ifeq ($(TARGET),)
# By default, the SW is on the MISO pin. This figures out where it is for the
# various processors

SW_m8			= B 4
SW_m16     		= B 6
SW_m168    		= B 4
SW_m32     		= B 6

# B.1 is SCK - PDI and RxD are the same pin				   
SW_m128    		= B 1

ifeq ($(SW_$(MK_AVR_MCU)),)
$(error Need to set SW_PORT for $(MK_AVR_MCU))
endif

ifeq ($(SW_$(MK_AVR_MCU)),)
$(error Need to set SW_PIN for $(MK_AVR_MCU))
endif

SW_DDR          = DDR$(word 1, $(SW_$(MK_AVR_MCU)))
SW_PORT         = PORT$(word 1, $(SW_$(MK_AVR_MCU)))
SW_PIN          = PIN$(word 1, $(SW_$(MK_AVR_MCU)))
SW              = P$(word 1, $(SW_$(MK_AVR_MCU)))$(word 2, $(SW_$(MK_AVR_MCU)))
SW_OPT			= -DSW_DDR=$(SW_DDR) -DSW_PORT=$(SW_PORT) -DSW=$(SW) -DSW_PIN=$(SW_PIN)

else

ifeq ($($(TARGET)_SW),)
SW_OPT			=
else

SW_DDR          = DDR$(word 1, $($(TARGET)_SW))
SW_PORT         = PORT$(word 1, $($(TARGET)_SW))
SW_PIN          = PIN$(word 1, $($(TARGET)_SW))
SW              = P$(word 1, $($(TARGET)_SW))$(word 2, $($(TARGET)_SW))
SW_OPT			= -DSW_DDR=$(SW_DDR) -DSW_PORT=$(SW_PORT) -DSW=$(SW) -DSW_PIN=$(SW_PIN)

endif
endif

# Check to make sure that something has been assigned for various paramters
           
ifeq ($(BOOT_START_$(MK_AVR_MCU)),)
$(error Need to set BOOT_START for $(MK_AVR_MCU))
endif

BOOT_START = $(BOOT_START_$(MK_AVR_MCU))

DEFS = $(LED_OPT) $(SW_OPT) -DBAUD_RATE=38400

CFLAGS += $(DEFS) -mshort-calls -Os -fno-inline-functions

LDFLAGS += -Wl,--section-start=.text=$(BOOT_START),-e,boot,--defsym,zero_addr=0 -nostdlib

include ../../rules/mkRules.mk


