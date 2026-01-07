#ifndef TRACKBALL_H
#define TRACKBALL_H

#include "quantum.h"

/**
 * @brief Initializes the trackball hardware, GPIOs, and interrupts.
 * Configures the pins for the trackball axis inputs and enables edge-triggered events.
 */
void pointing_device_driver_init(void);

/**
 * @brief Calculates and returns the mouse report.
 * Processes movement data, applies the velocity curve for precision, 
 * and handles the switch between mouse and wheel modes.
 * * @param mouse_report The current mouse report to be modified.
 * @return The updated report_mouse_t containing movement and wheel data.
 */
report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report);

/**
 * @brief Returns the current CPI (Counts Per Inch) setting.
 * Currently returns 0 as a placeholder.
 */
uint16_t pointing_device_driver_get_cpi(void);

/**
 * @brief Sets the CPI (Counts Per Inch) for the trackball.
 * @param cpi The desired CPI value.
 */
void pointing_device_driver_set_cpi(uint16_t cpi);

/**
 * @brief Standard QMK record processing.
 * Detects the JS_4 keypress to toggle between cursor movement and scroll wheel modes.
 */
bool process_record_kb(uint16_t keycode, keyrecord_t *record);

#endif /* TRACKBALL_H */