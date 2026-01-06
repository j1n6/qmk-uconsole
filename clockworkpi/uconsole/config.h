#pragma once

#include_next <config.h>

#define BACKLIGHT_PWM_PERIOD 2000
#define BACKLIGHT_PWM_DRIVER PWMD1
#define BACKLIGHT_PWM_CHANNEL 1

/*
 * To match stm32duino default used ny OEM firmware.
 *
 * https://github.com/stm32duino/Arduino_Core_STM32/blob/5b59097f828f9775cbaa5f55000c5a315bab2c32/system/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_def.h#L82
 */
#define USB_MAX_POWER_CONSUMPTION 100
