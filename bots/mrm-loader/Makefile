
OBJS = Loader.o

all: Loader.s19 Loader.num

Loader.o: Loader.S
	m68k-elf-gcc -c -o $@ $<
                         
Loader.s19: Loader.exe
	m68k-elf-objcopy -O srec $< $@

Loader.exe: $(OBJS) Loader.ld
	m68k-elf-ld -M --verbose -o Loader.exe Loader.o -TLoader.ld > Loader.map

