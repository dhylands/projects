
# Note, TOPDIR needs to point to the directory where Make.rules lives
TOPDIR = ../../MotoRobots/src
include $(TOPDIR)/Make.vars

# list all .o files to be built in OBJS, there is no need to define
# rules for them due to our .depend and Make.rules files
OBJS = Twister.o Motor.o OS-Support.o DebugKey.o


# Select linking for ram/rom, and which MRM version by 
# uncommenting the appropriate below:

RAM_512=$(TOPDIR)/startup/68332/mrm/mrm_ram_512k.ld
ROM_512=$(TOPDIR)/startup/68332/mrm/mrm_rom_512k.ld
RAM_32=$(TOPDIR)/startup/68332/mrm/mrm_ram_32k.ld
ROM_32=$(TOPDIR)/startup/68332/mrm/mrm_rom_32k.ld
LDSCRIPT=$(RAM_512)

# Eventually we can have all libs in an "installed" directory but
# for now each is in it's source directory so we need lots of -L args
EXE_LIBDIRS = \
		-L$(TOPDIR)/startup/68332/mrm \
		-L$(TOPDIR)/libbase/68332 \
		-L$(TOPDIR)/libqsm/68332 \
		-L$(TOPDIR)/libsim/68332 \
		-L$(TOPDIR)/libtpu/68332 \
		-L$(TOPDIR)/libled/68332/mrm \
		-L$(TOPDIR)/liblcd/68332/mrm \
                #-L/usr/local/gcc-m68k/lib/gcc-lib/m68k-elf/2.95.3\mcpu32 \
                #-L/usr/local/gcc-m68k/lib/gcc-lib/m68k-elf/2.95.3 \
                #-L/usr/local/gcc-m68k/m68k-elf/lib/mcpu32 \
                #-L/usr/local/gcc-m68k/m68k-elf/lib
		# many more to come ...
                
# /usr/local/gcc-m68k/lib/gcc-lib/m68k-elf/2.95.3/collect2.exe /usr/local/gcc-m68k/m68k-elf/lib/crt0.o 
#-L/usr/local/gcc-m68k/lib/gcc-lib/m68k-elf/2.95.3 -L/usr/local/gcc-m68k/m68k-elf/lib /c/Temp/ccZqWvc6.o -lstdc++ -lm -lgcc -lgcc                

EXE_LIBS = 	-lqsm -lsim -llcd -lled -ltpu

all: Twister.s19 Twister.num

Twister.s19: Twister.exe
Twister.exe: $(OBJS)
	$(LD) -M --verbose -o $@ $^ $(EXE_LIBDIRS) $(EXE_LIBS) $(COLDFIRE_LIBDIRS) -T$(LDSCRIPT) > $(@:.exe=.map)


#	$(G++) -v -o $@ $^ $(EXE_LIBDIRS) $(EXE_LIBS) $(COLDFIRE_LIBDIRS) -T$(LDSCRIPT)


Flash.s19: Flash.exe
Flash.exe: Flash.o
	$(LD) -M --verbose -o $@ $^ $(EXE_LIBDIRS) $(EXE_LIBS) $(COLDFIRE_LIBDIRS) -T$(ROM_512) > $(@:.exe=.map)


Ram.s19: Ram.exe
Ram.exe: Ram.o

depend: depend_default
clean: clean_default
install:

include $(TOPDIR)/Make.rules

