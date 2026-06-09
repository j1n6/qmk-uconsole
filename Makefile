#!/usr/bin/env -S make -f

SUDO = sudo
USB_ID = 1eaf:0003

.PHONY: default
default: clockworkpi_uconsole_default.bin clockworkpi_uconsole_no_tap_hold.bin

.PHONY: reflash
reflash: clockworkpi_uconsole_default.bin
	# dfu-util exits nonzero even without errors
	sh -c '$(SUDO) dfu-util -w -d $(USB_ID) -a 2 -D clockworkpi_uconsole_default.bin -R || true'

clockworkpi_uconsole_default.bin: qmk_firmware/.build/clockworkpi_uconsole_default.bin
	mv qmk_firmware/.build/clockworkpi_uconsole_default.bin $@

clockworkpi_uconsole_no_tap_hold.bin: qmk_firmware/.build/clockworkpi_uconsole_no_tap_hold.bin
	mv qmk_firmware/.build/clockworkpi_uconsole_no_tap_hold.bin $@

qmk_firmware/.build/clockworkpi_uconsole_default.bin: clockworkpi/uconsole/keymaps/default/keymap.c qmk_firmware
	sh -c 'cd qmk_firmware; qmk compile -kb clockworkpi/uconsole -km default'

qmk_firmware/.build/clockworkpi_uconsole_no_tap_hold.bin: clockworkpi/uconsole/keymaps/no_tap_hold/keymap.c clockworkpi/uconsole/keymaps/default/keymap.c qmk_firmware
	sh -c 'cd qmk_firmware; qmk compile -kb clockworkpi/uconsole -km no_tap_hold'

qmk_firmware:
	rm -rf $@.tmp
	git clone --depth 1 https://github.com/qmk/qmk_firmware.git $@.tmp
	git -C qmk_firmware.tmp submodule sync --recursive
	git -C qmk_firmware.tmp submodule update --init --recursive
	mv $@.tmp $@
	ln -s $(PWD)/clockworkpi $(PWD)/qmk_firmware/keyboards/
