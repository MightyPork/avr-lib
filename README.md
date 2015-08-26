# "porklib", MightyPork's Arduino Library

This library aims to make development for AVR in C easy and fun.

Modules of the library provide facilities for pin numbering and aliasing,
bitwise operations and accessing internal and external peripherals of the AVR.

It takes some inspiration from Arduino, but is written in pure C, which makes it faster
and smaller.

Pull requests to add new modules are welcome, please go ahead!

## Makefile

You can use the provided Makefile to build your project with this library.

A project typically consists of one `main.c` file, and some `*_config.c` files where the
library headers request it (those define eg. IO pin mapping).

Adjust the Makefile to your needs!

## License

The library is provided under MIT license, see the LICENSE file for more info.
