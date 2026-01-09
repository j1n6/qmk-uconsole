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


## 🎯 Installation Guide

**⚠️ WARNING:** Use SSH or an external keyboard when performing these operations. In case of issues, you'll still be able to interact with the device to re-flash or troubleshoot.

### Prerequisites

Before starting, ensure you have the necessary tools installed on your uConsole:

1. **Install DFU utilities:**
   ```bash
   sudo apt install -y dfu-util
   ```

2. **Download the original stock firmware package:**
   ```bash
   wget https://github.com/clockworkpi/uConsole/raw/master/Bin/uconsole_keyboard_flash.tar.gz
   tar zxvf uconsole_keyboard_flash.tar.gz
   cd uconsole_keyboard_flash
   ```

3. **Download the latest QMK firmware:**
   - Go to [Releases](https://github.com/j1n6/qmk-uconsole/releases)
   - Download the `clockworkpi_uconsole_default.bin` file
   - Move it to the `uconsole_keyboard_flash` folder

---

### Option 1: First-Time Flash from Stock Firmware

If you're upgrading from the original ClockworkPi firmware:

1. **Edit the upload script for better reliability:**
   
   Open `maple_upload` and change all delay values from `750` to `1500` milliseconds. This prevents "serial port not ready" errors.

2. **Flash the firmware:**
   ```bash
   sudo ./maple_upload ttyACM0 2 1EAF:0003 clockworkpi_uconsole_default.bin
   ```

---

### Option 2: Upgrade from Existing QMK Firmware

If you already have QMK installed and want to update:

1. **Run the DFU utility:**
   ```bash
   sudo dfu-util -w -d 1eaf:0003 -a 2 -D clockworkpi_uconsole_default.bin -R
   ```

2. **Enter bootloader mode:**
   
   When you see `waiting for device, exit with ctrl-C`, press these three keys **simultaneously**:
   
   **`Left Alt` + `Right Alt` + `Start`**

---

### 🆘 Recovery: Unbricking Your Keyboard

Don't panic if your keyboard becomes unresponsive! Follow these steps to restore it:

1. **Connect the micro-USB cable** from your uConsole to the keyboard

2. **Enter bootloader mode:**
   - Locate the **S1 pin** on the keyboard PCB
   - Short the S1 pin (you'll see a green LED flash)
![Bootloading illustration](https://github.com/j1n6/qmk-uconsole/blob/main/images/uconsole%20keyboard%20bootloading.jpeg?raw=true)

3. **Flash the stock firmware:**
   ```bash
   cd uconsole_keyboard_flash
   sudo ./flash
   ```
   
   **Note:** You may need to try this several times. Keep the S1 pin shorted while running the command.

4. **Verify:** Once successful, your keyboard should be responsive again. You can then re-flash QMK if needed.

## ☕ Support My Work
If you like my work, please consider [Buy me a Coffee](https://www.paypal.com/donate?token=0VRjowcZ_vZADGjy8UpFzoxQ9fgRgK27nz7Aw80MCTsNo8EzsxBGzcvTxYOGbU9r9nCraSPyB9LN4_Nn). Thank you.
 
 
## 📜 License

This project is licensed under the **GNU General Public License v3.0 (GPL-3.0)**.

You are free to use, modify, and distribute this software under the terms of the GPL-3.0 license. Any derivative works must also be distributed under the same license terms.

For the full license text, see the [LICENSE](LICENSE) file in this repository or visit [https://www.gnu.org/licenses/gpl-3.0.html](https://www.gnu.org/licenses/gpl-3.0.html).

**Note:** This firmware is provided "as is" without warranty of any kind. Use at your own risk.


#### 🤝 Acknowledgments
Special thanks to **[oesmith](https://github.com/oesmith/qmk_firmware)** for the initial groundwork and porting the base layout to the uConsole hardware.

