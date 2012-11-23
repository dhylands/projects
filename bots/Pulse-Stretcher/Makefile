MCU_TARGET      = atmega8
MCU_PROG_TARGET = m8
OPTIMIZE        = -O2

DEFS            =
LIBS            =
OBJ_DIR         = obj

# You should not have to change anything below here.

CC      = avr-gcc
AS	= avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override ASFLAGS       = -g -Wall -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,Pulse-Stretcher.map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump
PROGRAMMER     = avrdude

PROGRAMMER_OPTS = -p $(MCU_PROG_TARGET) -c avrisp -P COM1

.PHONY: Pulse-Stretcher

ASM_FILES = RCPulseIrq.S

SRC_FILES =  \
    Hardware.c \
    Pulse-Stretcher.c \
    RCPulse.c \
    Uart.c

DEP_FILES   = $(addprefix $(OBJ_DIR)/,$(SRC_FILES:.c=.d))
OBJ_FILES   = $(addprefix $(OBJ_DIR)/,$(SRC_FILES:.c=.o)) $(addprefix $(OBJ_DIR)/,$(ASM_FILES:.S=.o))

all: Pulse-Stretcher

xxx:
	echo OBJ_FILES = '$(OBJ_FILES)'

Pulse-Stretcher: Pulse-Stretcher.hex

$(OBJ_DIR)/Pulse-Stretcher.elf : $(OBJ_FILES)

FORCE:

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)
        
d download: Pulse-Stretcher.dld
    
$(OBJ_DIR)/%.elf :
    @echo "Linking $@ ..."
    @$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.dld : %.hex
    $(PROGRAMMER) $(PROGRAMMER_OPTS) -e -U flash:w:$<

%.cod: %.c FORCE
    $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -g -Wa,-ahdl=$@ -o $(@:.cod=.o) $<
        
%.pp: %.c
    $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -E -Wp,-dN,-C -o $@ $<
    
$(OBJ_DIR)/%.o: %.c
    @echo "Compiling $< ..."
    @$(COMPILE.c) $(OUTPUT_OPTION) $<
                                               
$(OBJ_DIR)/%.o: %.S
    @echo "Assembling $< ..."
    $(COMPILE.S) $(OUTPUT_OPTION) $<
                                               

lst:  $(PRG).lst

%.lst: %.elf
    $(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images

text: hex bin srec

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: $(OBJ_DIR)/%.elf
    @echo Creating $@ ...
    @$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: $(OBJ_DIR)/%.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: $(OBJ_DIR)/%.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec

ehex:  $(PRG)_eeprom.hex
ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: $(OBJ_DIR)/%.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

%_eeprom.srec: $(OBJ_DIR)/%.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@

%_eeprom.bin: $(OBJ_DIR)/%.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@

# Every thing below here is used by avr-libc's build system and can be ignored
# by the casual user.

FIG2DEV                 = fig2dev
EXTRA_CLEAN_FILES       = *.hex *.bin *.srec

dox: eps png pdf

eps: $(PRG).eps
png: $(PRG).png
pdf: $(PRG).pdf

%.eps: %.fig
	$(FIG2DEV) -L eps $< $@

%.pdf: %.fig
	$(FIG2DEV) -L pdf $< $@

%.png: %.fig
	$(FIG2DEV) -L png $< $@

#
# Dependency file generation
#

$(OBJ_DIR)/%.d : %.c
    @echo "Creating Dependency file for $< ..."
    @set -e; avr-cpp -M $< | sed 's=$(*F).o[ :]*=$(@:.d=.o) $@ : =g' > $@; [ -s $@ ] || ( rm -f $@; exit 1 )

ifneq ($(DEP_FILES),)
ifeq ($(strip $(filter clean% exec print-%, $(MAKECMDGOALS))),)
-include $(DEP_FILES)
endif
endif

