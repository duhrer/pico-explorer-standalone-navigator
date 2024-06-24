# Standalone Pico Explorer Keyboard Navigation

This project is a proof of concept for using a Raspberry Pi Pico and a Pico Explorer Base for keyboard navigation in a web browser.

## Hardware Requirements

In theory this project could be adapted for a range of products that use the
[RP 2040 chipset](https://www.raspberrypi.com/documentation/microcontrollers/rp2040.html), but currently, it's only
tested with a Pi Pico and the Pico Explorer Base.

## Installation

### Configuring Your Development Environment

Although this will eventually be better documented, for now this package follows the conventions used in the
[Getting Started with Pico Guide](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf).  Follow those
instructions to set up your development environment.  Although they mention installing the examples and using a visual
IDE, all you really need is the [Pico SDK](https://github.com/raspberrypi/pico-sdk) and the required command line tools
(CMake, et cetera).

### Building the Binaries

These instructions assume a unix-like shell like the terminal on OS X.  You'll first need to either temporarily or
permanently set the `PICO_SDK_PATH` environment variable. If you followed the instructions in the Pico guide, you'll be
working in a directory next to the pico-sdk downloads, and this path would be `../../pico-sdk`.  You can also find the
path by navigating to the SDK in a terminal and typing `pwd`.


```
export PICO_SDK_PATH=<your path>
mkdir build
cd build
cmake ..
make
```

### Installing on a Pico

1. If your Pico has already got something installed on it, you should power it off and then power it on while holding
   the "bootsel" button.  The Pico should show up as a USB drive.
2. Drag the UF2 file you created above onto that USB drive.

## Controls and Feedback

If the binary is succesfully installed, the screen on your Pico Explorer base should display solid white with "heartbeat" light that blinks on and off every second.

The `A`, `B`, `X`, and `Y` buttons will result in keyboard input, as follows:

| Button | Key         |
| ------ | ----------- |
| `A`    | `Shift+Tab` |
| `B`    | `Tab`       |
| `X`    | `Escape`    |
| `Y`    | `Space`     |

As with a full keyboard, you can navigate in a browser using `A` and `B` to move between focusable elements, and using `Y` to "click".  `X` is used for things like closing modal dialogs.

## Credits and Licensing

This demo was adapted from various examples in the
[Raspberry Pi Pico Examples repository](https://github.com/raspberrypi/pico-examples).  See `EXAMPLES-LICENSE.TXT` for
details on (re)using those.
