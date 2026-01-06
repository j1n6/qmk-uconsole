# clockworkpi/uconsole

This is a firmware for the built-in keyboard on the ClockworkPi uConsole. It's
intended to replace the original Arduino-based firmware supplied from the
factory.

* Keyboard Maintainer: [Olly Smith](https://github.com/Olly Smith)
* Hardware Supported: ClockworkPI uConsole STM32F103 keyboard/gamepad/trackball module
* Hardware Availability: https://clockworkpi.com/uconsole

Make example for this keyboard (after setting up your build environment):

    make clockworkpi/uconsole:default

Flashing example for this keyboard:

    make clockworkpi/uconsole:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 2 ways:

* **Physical reset button**: Briefly press the button on the back of the PCB - some may have pads you must short instead
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available (usually Fn+Esc)
