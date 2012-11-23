__ram_start  = 0x03000;
__ram_size   = 0x7D000; /* 0x05000 for 32K RAM, 0x7D000 for 512K RAM */
__rom_start  = 0x90000;
__rom_size   = 0x70000;
__stack      = __ram_start + __ram_size - 0x4;
__prog_start = __rom_start + 0x10;

SECTIONS
{
  .text __rom_start :
  {
    LONG (0xbeefbeef);
    LONG (__stack);
    LONG (__prog_start);
    LONG (0x0);

    *(.text)
  }
}    
