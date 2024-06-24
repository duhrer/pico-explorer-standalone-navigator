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

## Building and Installing

### VS Code and a PiProbe

If you have a PiProbe, you should be able to use the debugger configuration in this project to build, install (and
debug) the code here.

First, you should check the paths in `.vscode/launch.json`and `.vscode/settings.json` and update them as
needed to match your system. Then, you should be able to just hit the debugger icon and choose the configuration
defined in `.vscode/launch.json`.

The application will be built, deployed, and will pause execution at the beginning of the `main()` function.

### Command Line Tools

To build the application from the command line, you can use commands like the following, starting at the root the
repository:

```
mkdir build
cd build
cmake ..
make -j4
```

The last command assumes you have four cores, adjust as needed. Once the build completes, there are two ways to install
the application.

If you don't have a PiProbe, reboot your Pico while holding the "Bootsel" button, then drag the generated UF2 file
onto the USB drive that appears.

If you have a PiProbe, you can install the program without resetting your Pico using a command like:

```
sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program pico-explorer-clock.elf verify reset exit"
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
