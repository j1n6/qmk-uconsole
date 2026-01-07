/*
 * Lite custom matrix for ClockworkPi uConsole
 *
 * - CUSTOM_MATRIX = lite
 * - Implements diode-driven scanning locally (no calls to core helpers).
 * - Reads DIRECT_PINS and ORs those bits into the matrix.
 *
 * Assumes:
 * - DIRECT_PINS uses NO_PIN for unused entries and is shaped [MATRIX_ROWS][MATRIX_COLS]
 *   (or compatible). We build a real array from the initializer macro.
 * - MATRIX_ROW_PINS and MATRIX_COL_PINS are defined for the diode matrix.
 * - Direct pins are active-low (input pull-up). If yours are active-high, invert gpio_read_pin().
 * - Non-split keyboard (safe for split too if you adapt rows-per-hand mapping).
 */

#include "quantum.h"
#include "gpio.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* If split support needed later, adjust this; for uConsole non-split equals MATRIX_ROWS. */
#if defined(SPLIT_KEYBOARD)
#    define ROWS_PER_HAND_LOCAL (MATRIX_ROWS / 2)
#else
#    define ROWS_PER_HAND_LOCAL (MATRIX_ROWS)
#endif

#ifdef DIRECT_PINS
/* Build a real array from the DIRECT_PINS initializer macro. */
static const pin_t direct_pins[][MATRIX_COLS] = DIRECT_PINS;
#endif

#ifdef MATRIX_ROW_PINS
static const pin_t matrix_row_pins[] = MATRIX_ROW_PINS;
#endif
#ifdef MATRIX_COL_PINS
static const pin_t matrix_col_pins[] = MATRIX_COL_PINS;
#endif

/* Small settling delay (tune if needed for reliable reads). */
static inline void small_delay(void) {
    volatile uint32_t i;
    for (i = 0; i < 200; i++) { __asm__ volatile("nop"); }
}

/* Keep previous matrix to report changes (matrix_scan_custom must return true if changed). */
static matrix_row_t last_matrix[MATRIX_ROWS];

void matrix_init_custom(void) {
    /* Initialize direct pins (input with pull-up) */
#ifdef DIRECT_PINS
    for (uint8_t r = 0; r < ROWS_PER_HAND_LOCAL; r++) {
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            pin_t p = direct_pins[r][c];
            if (p == NO_PIN) continue;
            gpio_set_pin_input_high(p);
        }
    }
#endif

    /* Initialize diode-driven matrix pins depending on diode direction */
#if defined(DIODE_DIRECTION)
#    if (DIODE_DIRECTION == COL2ROW)
    /* COL2ROW: columns are inputs (pull-up), rows are outputs (idle HIGH) */
#        ifdef MATRIX_COL_PINS
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        pin_t p = matrix_col_pins[c];
        if (p == NO_PIN) continue;
        gpio_set_pin_input_high(p);
    }
#        endif
#        ifdef MATRIX_ROW_PINS
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        pin_t p = matrix_row_pins[r];
        if (p == NO_PIN) continue;
        gpio_set_pin_output(p);
        gpio_write_pin_high(p); // deselect
    }
#        endif

#    elif (DIODE_DIRECTION == ROW2COL)
    /* ROW2COL: rows are inputs (pull-up), cols are outputs (idle HIGH) */
#        ifdef MATRIX_ROW_PINS
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        pin_t p = matrix_row_pins[r];
        if (p == NO_PIN) continue;
        gpio_set_pin_input_high(p);
    }
#        endif
#        ifdef MATRIX_COL_PINS
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        pin_t p = matrix_col_pins[c];
        if (p == NO_PIN) continue;
        gpio_set_pin_output(p);
        gpio_write_pin_high(p); // deselect
    }
#        endif

#    else
#        error "DIODE_DIRECTION must be COL2ROW or ROW2COL"
#    endif
#endif

    /* Clear last_matrix */
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) last_matrix[r] = 0;
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;

    /* Start with zeros */
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) current_matrix[r] = 0;

    /* Read direct pins and OR into current_matrix */
#ifdef DIRECT_PINS
    for (uint8_t r = 0; r < ROWS_PER_HAND_LOCAL; r++) {
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            pin_t p = direct_pins[r][c];
            if (p == NO_PIN) continue;
            /* active-low (pull-up): gpio_read_pin() == 0 when pressed */
            if (!gpio_read_pin(p)) {
                current_matrix[r] |= ((matrix_row_t)1 << c);
            }
        }
    }
#endif

    /* Diode-driven scanning implemented locally (no core helper calls) */
#if defined(DIODE_DIRECTION)
#    if (DIODE_DIRECTION == COL2ROW)
    /* For each row: drive row low, read columns */
#        ifdef MATRIX_ROW_PINS
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        pin_t rp = matrix_row_pins[r];
        if (rp == NO_PIN) continue;
        gpio_write_pin_low(rp);   /* select row */
        small_delay();
#            ifdef MATRIX_COL_PINS
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            pin_t cp = matrix_col_pins[c];
            if (cp == NO_PIN) continue;
            if (!gpio_read_pin(cp)) {
                current_matrix[r] |= ((matrix_row_t)1 << c);
            }
        }
#            endif
        gpio_write_pin_high(rp);  /* unselect row */
        small_delay();
    }
#        else
#            error "MATRIX_ROW_PINS must be defined for COL2ROW"
#        endif

#    elif (DIODE_DIRECTION == ROW2COL)
    /* For each column: drive column low, read rows */
#        ifdef MATRIX_COL_PINS
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        pin_t cp = matrix_col_pins[c];
        if (cp == NO_PIN) continue;
        gpio_write_pin_low(cp);  /* select column */
        small_delay();
#            ifdef MATRIX_ROW_PINS
        for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
            pin_t rp = matrix_row_pins[r];
            if (rp == NO_PIN) continue;
            if (!gpio_read_pin(rp)) {
                current_matrix[r] |= ((matrix_row_t)1 << c);
            }
        }
#            endif
        gpio_write_pin_high(cp); /* unselect column */
        small_delay();
    }
#        else
#            error "MATRIX_COL_PINS must be defined for ROW2COL"
#        endif

#    else
#        error "DIODE_DIRECTION must be COL2ROW or ROW2COL"
#    endif
#endif

    /* Compare with last_matrix, update, and report change */
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        if (current_matrix[r] != last_matrix[r]) {
            changed = true;
            last_matrix[r] = current_matrix[r];
        }
    }

    return changed;
}