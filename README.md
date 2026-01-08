# WARNING - Still Testing - DO NOT USE

# uConsole QMK Keyboard Firmware

This repository provides a streamlined, standalone QMK firmware distribution specifically optimized for the **ClockworkPi uConsole**.

Unlike the standard QMK repository, this project isolates the uConsole keyboard logic. By removing unrelated drivers and keyboard definitions, provide a lightweight environment dedicated to perfecting the uConsole typing experience.

## 🎯 Project Goals

* **Clean Workspace:** Only contains code relevant to the uConsole keyboard.
* **Minimal Context:** Makes it easy to identify specific hardware changes and logic.
* **Rapid Development:** Faster compile times and easier testing of experimental QMK features.
* **Streamlined CI/CD:** Simple workflows for rebuilding and releasing binaries.
* **Low Barrier to Entry:** Easier for community members to contribute layouts without learning the entire QMK ecosystem.

## 🧪 Keyboard Tester

Test your uConsole keyboard layout and functionality with our interactive keyboard tester:

**[https://j1n6.github.io/qmk-uconsole/](https://j1n6.github.io/qmk-uconsole/)**

This web-based tool provides:
* **Visual Feedback:** See which keys are being pressed in real-time
* **Layer Detection:** Shows Layer 2 (Fn) key combinations
* **D-Pad & Gamepad Testing:** Test arrow keys, joystick buttons (X, Y, A, B), and mouse buttons (L, R, Middle)
* **Scroll & Cursor Tracking:** Visualize trackball movement and scroll events

Perfect for verifying your firmware installation and familiarizing yourself with the uConsole's unique keyboard layout!


## 🎯 How to Install
These steps need to be operated on your uConsole. 
WARNING: Use SSH session or alternative input devices to operate these upgrade. In case of issues, you can still interact with the device to re-flash or troubleshoot. 

 1. Ensure the DFU Tools is installed
 ```
 sudo apt install -y dfu-util
 ```
 2. Download the [orignal stock firmware packages](https://github.com/clockworkpi/uConsole/raw/master/Bin/uconsole_keyboard_flash.tar.gz).
 ```
 wget https://github.com/clockworkpi/uConsole/raw/master/Bin/uconsole_keyboard_flash.tar.gz && tar zxvf uconsole_keyboard_flash.tar.gz
 ```
 
 3. Download latest QMK firmware .bin file from the [Releases](https://github.com/j1n6/qmk-uconsole/releases), move the .bin file inside the `uconsole_keyboard_flash` folder.
 3. Edit the `maple_upload`, change all the default `750` to `1500` millionsecond delay, othewires you might see the serial port isn’t ready or not found errors.
 4. Flash the QMK firmware by the following command:
```
sudo ./maple_upload ttyACM0 2 1EAF:0003 clockworkpi_uconsole_default.bin`
```
 5. To upgrade QMK firmware from existing QMK, enter the command below. When the screen shows `waiting for device, exit with ctrl-C`, enter the bootloader mode for the uConsole keyboard by pressing three button down at all the same time: `LeftAlt`+`RightAlt`+`Start`
```
 sudo dfu-util -w -d 1eaf:0003 -a 2 -D clockworkpi_uconsole_default.bin -R
```
 
## 📜 License

This project is licensed under the **GNU General Public License v3.0 (GPL-3.0)**.

You are free to use, modify, and distribute this software under the terms of the GPL-3.0 license. Any derivative works must also be distributed under the same license terms.

For the full license text, see the [LICENSE](LICENSE) file in this repository or visit [https://www.gnu.org/licenses/gpl-3.0.html](https://www.gnu.org/licenses/gpl-3.0.html).

**Note:** This firmware is provided "as is" without warranty of any kind. Use at your own risk.


#### 🤝 Acknowledgments
Special thanks to **[oesmith](https://github.com/oesmith/qmk_firmware)** for the initial groundwork and porting the base layout to the uConsole hardware.
