# BioloidUART test for RP2040

This particular test assumes that the Tx & Rx pins on UART0 and UART1 are all connected together.

To load the sketch for the first time, Press BOOTSEL while plugging in (or pressing RESET if
your board has that), so that the RPI-RP2 filesystem is available.

Once the firmware has been uploaded the first time, it has an auto-reset feature that is utilized
for further uploads.

From the serial terminal the following keys are defined:
* p - sends a ping packet to the current ID (defaults to 1)
* d - toggles debug mode (default is off)
* 1-9 - Changes the ID to send to

# Install find_port.py

Copy find_port.py from https://github.com/dhylands/dotfiles/blob/master/bin/find_port.py
somewhere in your PATH and mark it as executable. I usually put it into `~/bin`

# Building for different boards.

The arduino-cli supports the notion of a profile, and the current boards are defined in the
sketch.yml file.

The VID/PID also varies slightly by the firmware, and the Makefile needs to know about the
VID/PID for the particular firmware.

# Using GNUmakefile

If you find you're using a different profile than the default set in the Makefile, then you
can override this by creating a `GNUmakefile` with the following contents:

```
$(info ==========================================================)
$(info Running GNUmakefile)

PROFILE = pico

$(info PROFILE = $(PROFILE))
$(info ==========================================================)

include Makefile
```

The `GNUmakefile` will never be checked into git, so it will essentially be local overrides to
the `Makefile`.
