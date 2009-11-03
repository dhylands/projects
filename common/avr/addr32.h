// The addr32 macro returns a byte address of a symbol located anywhere in 
// the 128K space of the ATMega128.

#define addr32(symbol)        \
({                                          \
    uint32_t __result;                      \
    __asm__                                 \
    (                                       \
        "ldi %A0, lo8(pm(" #symbol "))\n\t" \
        "ldi %B0, hi8(pm(" #symbol "))\n\t" \
        "eor %C0, %C0"              "\n\t"  \
        "eor %D0, %D0"              "\n\t"  \
        "add %A0, %A0"              "\n\t"  \
        "adc %B0, %B0"              "\n\t"  \
        "adc %C0, %C0"              "\n\t"  \
        : "=r" (__result)                   \
    );                                      \
    __result;                               \
})

