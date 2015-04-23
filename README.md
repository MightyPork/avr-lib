# MightyPork's AVR Library

I program my Arduinos in plain C, compile it with `avr-gcc` and flash with `avrdude` (all on Linux).

Whenever I learn how to do something, I make a library file and put it here.

The code is tested and optimized for **ATmega328P**, which is used in most Arduinos. I use "Pro Mini" and "Nano".

# How to use

Link the `lib/` folder to your project, and make sure you add all lib `.c` files to your `Makefile`, so it builds are the needed code.

Some library files don't have `.c`, but many do.

## Useful things

- To easily alias I/O pins, use `lib/pins.h`.
- For Arduino pins, there are presets in `lib/arduino-pins.h`
- Binary/byte manipulation utilities are in `lib/calc.h`
- `lib/meta.h` contains some generally useful things that didn't fit elsewhere

Each header file contains a comment block with explanation, which will help you understand them.

