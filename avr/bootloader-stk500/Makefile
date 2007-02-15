###########################################################################
#
# STK500 compatible serial bootloader
#
###########################################################################

MK_OS				= avr
MK_AVR_MCU			= m168
MK_AVR_FREQ			= 8MHz
MK_AVR_PROGRAMMER  ?= stk500

MK_HEX_TARGET		= Boot-m$(MK_AVR_MCU)-$(MK_AVR_FREQ)
MK_SRC_FILES		= start.c boot.c

# Determnine the starting address for the bootloader. Note that this is a byte
# address, whereas the datasheet uses a word address.
#
# The fiinal optimized bootloader should fit in under 1K bytes.

BOOT_START_m8  	= 0x1C00
BOOT_START_m16 	= 0x3C00
BOOT_START_m168 = 0x3C00
BOOT_START_m32 	= 0x7C00

MK_AVR_FUSES_m168	= FA DF E2
					  
# Setup the default status LED. We use SS, except on the ATMega8/48/88/168 
# where SS is shared with OC1B which is normally used as a motor control

LED_PORT_m8		= D
LED_PIN_m8		= 7

LED_PORT_m16	= B
LED_PIN_m16		= 4

LED_PORT_m168	= D
LED_PIN_m168	= 7

LED_PORT_m32	= B
LED_PIN_m32		= 4

LED_PORT		= PORT$(LED_PORT_$(MK_AVR_MCU))
LED_DDR			= DDR$(LED_PORT_$(MK_AVR_MCU))
LED				= P$(LED_PORT_$(MK_AVR_MCU))$(LED_PIN_$(MK_AVR_MCU))

# By default, the SW is on the MISO pin. This figures out where it is for the
# various processors

SW_PORT_m8		= B
SW_PIN_m8		= 4

SW_PORT_m16		= B
SW_PIN_m16 		= 6

SW_PORT_m168	= B
SW_PIN_m168 	= 4

SW_PORT_m32		= B
SW_PIN_m32		= 6

SW_DDR			= DDR$(SW_PORT_$(MK_AVR_MCU))
SW_PORT			= PORT$(SW_PORT_$(MK_AVR_MCU))
SW_PIN			= PIN$(SW_PORT_$(MK_AVR_MCU))
SW				= P$(SW_PORT_$(MK_AVR_MCU))$(SW_PIN_$(MK_AVR_MCU))

# Check to make sure that something has been assigned for various paramters
		   
ifeq ($(BOOT_START_$(MK_AVR_MCU)),)
$(error Need to set BOOT_START for $(MK_AVR_MCU))
endif

ifeq ($(LED_PORT_$(MK_AVR_MCU)),)
$(error Need to set LED_PORT for $(MK_AVR_MCU))
endif

ifeq ($(LED_PIN_$(MK_AVR_MCU)),)
$(error Need to set LED_PIN for $(MK_AVR_MCU))
endif

ifeq ($(SW_PORT_$(MK_AVR_MCU)),)
$(error Need to set SW_PORT for $(MK_AVR_MCU))
endif

ifeq ($(SW_PIN_$(MK_AVR_MCU)),)
$(error Need to set SW_PIN for $(MK_AVR_MCU))
endif

BOOT_START = $(BOOT_START_$(MK_AVR_MCU))

DEFS = \
	-DLED_DDR=$(LED_DDR) \
	-DLED_PORT=$(LED_PORT) \
	-DLED=$(LED) \
	-DSW_DDR=$(SW_DDR) \
	-DSW_PORT=$(SW_PORT) \
	-DSW=$(SW) \
	-DSW_PIN=$(SW_PIN) \
	-DBAUD_RATE=38400

CFLAGS += $(DEFS) -mshort-calls -Os

LDFLAGS += -Wl,--section-start=.text=$(BOOT_START),-e,boot,--defsym,zero_addr=0 -nostdlib

include ../../rules/mkRules.mk

