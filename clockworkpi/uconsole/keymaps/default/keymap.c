// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum {
  LY0 = 0,
  LY1,
  LY2
};

enum {
  // Tap-Hold keycodes for letters (Tap = lowercase, Hold = uppercase)
  LH_A = SAFE_RANGE,
  LH_B,
  LH_C,
  LH_D,
  LH_E,
  LH_F,
  LH_G,
  LH_H,
  LH_I,
  LH_J,
  LH_K,
  LH_L,
  LH_M,
  LH_N,
  LH_O,
  LH_P,
  LH_Q,
  LH_R,
  LH_S,
  LH_T,
  LH_U,
  LH_V,
  LH_W,
  LH_X,
  LH_Y,
  LH_Z,
  // Tap-Hold keycodes for numbers (Tap = number, Hold = shifted symbol)
  LH_0,
  LH_1,
  LH_2,
  LH_3,
  LH_4,
  LH_5,
  LH_6,
  LH_7,
  LH_8,
  LH_9,
  // Tap-Hold keycodes for special characters
  LH_GRV,   // ` -> ~
  LH_LBRC,  // [ -> {
  LH_RBRC,  // ] -> }
  LH_MINS,  // - -> _
  LH_EQL,   // = -> +
  LH_SLSH,  // / -> ?
  LH_BSLS,  // \ -> |
  LH_SCLN,  // ; -> :
  LH_QUOT,  // ' -> "
  LH_COMM,  // , -> <
  LH_DOT,   // . -> >
  // Original keycodes
  JS_LEFT,
  JS_RGHT,
  JS_UP,
  JS_DOWN,
  KB_LOCK,
  KB_TAP_HOLD      // Toggle tap-hold feature
};

const key_override_t vol_key_override =
  ko_make_basic(MOD_MASK_SHIFT, KC_VOLD, KC_VOLU);

const key_override_t *key_overrides[] = {&vol_key_override};

const uint16_t PROGMEM bootloader_combo[] = {KC_LALT, KC_RALT, KC_LGUI, COMBO_END};
#ifndef DISABLE_TAP_HOLD
const uint16_t PROGMEM tap_hold_combo[] = {LH_T, KC_HOME, COMBO_END};
#endif
combo_t key_combos[] = {
  COMBO(bootloader_combo, QK_BOOT),
#ifndef DISABLE_TAP_HOLD
  COMBO(tap_hold_combo, KB_TAP_HOLD),
#endif
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * Layer 0: Default
     *
     *   (Up)             ( L ) ( R )          ( Y ) ( X )
     * (Lt)  (Rt)                               ( B ) ( A )
     *   (Dn)                                     (o - middle click - trackball.c)
     *
     * (Esc)(Sel)(Sta)     (Vol)( [ )( ] )( / )( - )( = )( \ )
     * ( ~ )( 1 )( 2 )( 3 )( 4 )( 5 )( 6 )( 7 )( 8 )( 9 )( 0 )(Bsp)
     * (Tab)( Q )( W )( E )( R )( T )( Y )( U )( I )( O )( P )
     * ( ' )( A )( S )( D )( F )( G )( H )( J )( K )( L )( ; )(Ent)
     * (Sft)( Z )( X )( C )( V )( B )( N )( M )( , )( . )(Sft)
     *  (Fn)(Ctl)(Alt)(       Space      )(Alt)(Ctl)(Fn)
     *
     */
    [LY0] = LAYOUT(
        /* JS_0 A -> Execute
         * JS_1 B -> Stop
         * JS_2 Y -> Menu
         * JS_3 X -> SysRq */   // formerly JS_0,    JS_1,    JS_2,    JS_3,
        KC_UP,   KC_DOWN, KC_LEFT, KC_RGHT, KC_EXEC, KC_STOP, KC_SYRQ, KC_MENU,
        KC_LSFT, KC_RSFT, KC_LCTL, KC_RCTL, KC_LALT, MS_BTN1, KC_RALT, MS_BTN2,
        MS_BTN3, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,

        /* JS_4 Select -> (Keyboard) Select
         * JS_5 Start  -> Left GUI */
        KC_SELECT, KC_LGUI, KC_VOLD, LH_GRV, LH_LBRC, LH_RBRC, LH_MINS, LH_EQL,
        LH_1,      LH_2,    LH_3,    LH_4,    LH_5,    LH_6,    LH_7,    LH_8,
        LH_9,      LH_0,    KC_ESC,  KC_TAB,  KC_NO,   KC_NO,   KC_NO,   KC_NO,
        LH_Q,      LH_W,    LH_E,    LH_R,    LH_T,    LH_Y,    LH_U,    LH_I,
        LH_O,      LH_P,    LH_A,    LH_S,    LH_D,    LH_F,    LH_G,    LH_H,
        LH_J,      LH_K,    LH_L,    LH_Z,    LH_X,    LH_C,    LH_V,    LH_B,
        LH_N,      LH_M,    LH_COMM, LH_DOT,  LH_SLSH, LH_BSLS, LH_SCLN, LH_QUOT,
        KC_BSPC,   KC_ENT,  MO(LY1), MO(LY1), KC_SPC,  KC_NO,   KC_NO,   KC_NO
    ),

    /*
     * Layer 1: Fn Keys
     *
     *   (PgU)            ( L ) ( R )          (   ) (   )
     * (Hom) (End)                               (   ) (   )
     *   (PgD)                                    (o - trackball.c)
     *
     * (Lck)(Prt)(Pau)     (Mut)(   )(   )(   )(F11)(F12)(   )
     * (   )(F1 )(F2 )(F3 )(F4 )(F5 )(F6 )(F7 )(F8 )(F9 )(F10)(Del)
     * (Cap)(   )(   )(   )(   )(   )(   )(PgU)(Ins)(   )(   )
     * (   )(   )(   )(   )(   )(Tg2)(Hom)(End)(PgD)(   )(   )(   )
     * (Hom)(PgD)(   )(   )(   )(   )(   )(   )
     * (   )(   )(Cmd)(      BlStp       )(Cmd)(   )(  )
     * Fn+T+H = Tap-Hold Toggle
     */

    [LY1] = LAYOUT(
        KC_PGUP, KC_PGDN, KC_HOME, KC_END,  _______, _______, _______, _______,
        _______, _______, _______, _______, KC_LGUI, _______, KC_RGUI, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,

        KC_PSCR, KC_PAUS, KC_MUTE, _______, _______, _______, KC_F11,  KC_F12,
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,
        KC_F9,   KC_F10,  KB_LOCK, KC_CAPS, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, KC_PGUP, KC_INS,
        _______, _______, _______, _______, _______, _______, TG(LY2), KC_HOME,
        KC_END,  KC_PGDN, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_BRID, KC_BRIU, _______, _______, _______, _______,
        KC_DEL,  _______, _______, _______, BL_STEP, _______, _______, _______
    ),

    /*
     * Layer 2: Gamepad (Toggled by Fn+G)
     *
     *   (JS_L)           (   ) (   )          ( Y ) ( X )
     * (JS_U) (JS_D)                             ( B ) ( A )
     *   (JS_R)                                   (   )
     *
     * (   )(Sel)(Sta)     (   )(   )(   )(   )(   )(   )(   )
     * [Note: D-pad keys mapped to Joystick Axis]
     */
    [LY2] = LAYOUT(
        JS_LEFT, JS_RGHT, JS_UP,   JS_DOWN, JS_0,    JS_1,    JS_2,    JS_3,
        _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,

        JS_4,    JS_5,    _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______
    ),
};

static bool is_locked = false;

// D-pad joystick state. Left/Right share axis 1, Up/Down share axis 0.
// These directions behave like discrete keys rather than a physical d-pad, so
// we use last-input priority: when both opposing keys are held, the most
// recently pressed one wins, and releasing it falls back to the opposite key
// if it is still held.
static bool js_left_held  = false;
static bool js_right_held = false;
static bool js_up_held    = false;
static bool js_down_held  = false;
// Most recently pressed direction on each axis (+1 / -1), 0 if neither held.
static int8_t js_h_last = 0;  // axis 1: -1 = left, +1 = right
static int8_t js_v_last = 0;  // axis 0: -1 = up,   +1 = down

static void js_update_axes(void) {
  // Horizontal (axis 1): last-pressed wins while both held.
  if (js_left_held && js_right_held) {
    joystick_set_axis(1, js_h_last * 127);
  } else if (js_left_held) {
    joystick_set_axis(1, -127);
  } else if (js_right_held) {
    joystick_set_axis(1, 127);
  } else {
    joystick_set_axis(1, 0);
  }

  // Vertical (axis 0): last-pressed wins while both held.
  if (js_up_held && js_down_held) {
    joystick_set_axis(0, js_v_last * 127);
  } else if (js_up_held) {
    joystick_set_axis(0, -127);
  } else if (js_down_held) {
    joystick_set_axis(0, 127);
  } else {
    joystick_set_axis(0, 0);
  }
}

// Extern from trackball.c to control scroll mode
extern volatile bool select_button_pressed;
extern volatile bool select_button_scrolled;
extern volatile bool precision_mode;

// EEPROM Configuration
typedef union {
  uint32_t raw;
  struct {
    bool tap_hold_enabled :1;  // Bit 0: tap-hold feature enabled/disabled
  };
} keyboard_config_t;

keyboard_config_t keyboard_config;

// Tap-hold timing tracking
#define TAP_HOLD_TIMEOUT 200  // milliseconds
#define TAP_HOLD_KEY_COUNT 47

// Mapping of tap-hold keycodes to their base keycodes
static const uint16_t tap_hold_map[][2] = {
  {LH_A, KC_A},  {LH_B, KC_B},  {LH_C, KC_C},  {LH_D, KC_D},  {LH_E, KC_E},
  {LH_F, KC_F},  {LH_G, KC_G},  {LH_H, KC_H},  {LH_I, KC_I},  {LH_J, KC_J},
  {LH_K, KC_K},  {LH_L, KC_L},  {LH_M, KC_M},  {LH_N, KC_N},  {LH_O, KC_O},
  {LH_P, KC_P},  {LH_Q, KC_Q},  {LH_R, KC_R},  {LH_S, KC_S},  {LH_T, KC_T},
  {LH_U, KC_U},  {LH_V, KC_V},  {LH_W, KC_W},  {LH_X, KC_X},  {LH_Y, KC_Y},
  {LH_Z, KC_Z},
  {LH_0, KC_0},  {LH_1, KC_1},  {LH_2, KC_2},  {LH_3, KC_3},  {LH_4, KC_4},
  {LH_5, KC_5},  {LH_6, KC_6},  {LH_7, KC_7},  {LH_8, KC_8},  {LH_9, KC_9},
  {LH_GRV, KC_GRV},   {LH_LBRC, KC_LBRC}, {LH_RBRC, KC_RBRC},
  {LH_MINS, KC_MINS},  {LH_EQL, KC_EQL},   {LH_SLSH, KC_SLSH},
  {LH_BSLS, KC_BSLS},  {LH_SCLN, KC_SCLN}, {LH_QUOT, KC_QUOT},
  {LH_COMM, KC_COMM},  {LH_DOT, KC_DOT},
};

#ifndef DISABLE_TAP_HOLD
static uint16_t tap_hold_key_press_times[TAP_HOLD_KEY_COUNT] = {0};
static bool tap_hold_passthrough[TAP_HOLD_KEY_COUNT] = {false};
static bool tap_hold_key_pressed[TAP_HOLD_KEY_COUNT] = {false};
#endif

// Helper function to get base keycode from tap-hold keycode
static uint16_t get_base_keycode(uint16_t keycode) {
  for (int i = 0; i < TAP_HOLD_KEY_COUNT; i++) {
    if (tap_hold_map[i][0] == keycode) {
      return tap_hold_map[i][1];
    }
  }
  return KC_NO;
}

// Helper function to check if keycode is a tap-hold key
static bool is_tap_hold_key(uint16_t keycode) {
  return (keycode >= LH_A && keycode <= LH_9) || (keycode >= LH_GRV && keycode <= LH_DOT);
}

#ifndef DISABLE_TAP_HOLD
// Helper function to get the index of a tap-hold key
static int get_tap_hold_index(uint16_t keycode) {
  if (keycode >= LH_A && keycode <= LH_Z) {
    return keycode - LH_A;
  }
  if (keycode >= LH_0 && keycode <= LH_9) {
    return 26 + (keycode - LH_0);
  }
  if (keycode >= LH_GRV && keycode <= LH_DOT) {
    return 36 + (keycode - LH_GRV);
  }
  return -1;
}

static bool is_tap_hold_passthrough_active(void) {
  uint8_t active_mods = get_mods() | get_weak_mods() | get_oneshot_mods();
  return (active_mods & MOD_MASK_CSAG) || layer_state_is(LY1);
}

static void resolve_pending_tap_hold_keys(uint16_t current_keycode, uint32_t current_time) {
  int current_index = is_tap_hold_key(current_keycode) ? get_tap_hold_index(current_keycode) : -1;
  for (int i = 0; i < TAP_HOLD_KEY_COUNT; i++) {
    if (tap_hold_key_pressed[i] && i != current_index) {
      uint16_t base_key = tap_hold_map[i][1];
      uint16_t elapsed = current_time - tap_hold_key_press_times[i];

      if (elapsed < TAP_HOLD_TIMEOUT) {
        // Tap - send key as-is (lowercase/number)
        register_code(base_key);
        unregister_code(base_key);
      } else {
        // Hold - send shift + key (uppercase/shifted symbol)
        register_code(KC_LSFT);
        register_code(base_key);
        unregister_code(base_key);
        unregister_code(KC_LSFT);
      }
      tap_hold_key_pressed[i] = false;
    }
  }
}
#endif

void keyboard_post_init_user(void) {
  keyboard_config.raw = eeconfig_read_user();
  // Default to disabled if EEPROM is uninitialized (raw == 0)
}

void eeconfig_init_user(void) {
  // Set default EEPROM values
  keyboard_config.raw = 0;  // Tap-hold disabled by default
  eeconfig_update_user(keyboard_config.raw);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (is_locked && keycode != KB_LOCK && keycode != MO(LY1)) {
    return false;
  }

#ifndef DISABLE_TAP_HOLD
  if (record->event.pressed) {
    resolve_pending_tap_hold_keys(keycode, record->event.time);
  }
#endif

  // Handle tap-hold keys
  if (is_tap_hold_key(keycode)) {
    uint16_t base_key = get_base_keycode(keycode);

#ifdef DISABLE_TAP_HOLD
    if (record->event.pressed) {
      register_code(base_key);
    } else {
      unregister_code(base_key);
    }
    return false;
#else
    int index = get_tap_hold_index(keycode);
    bool passthrough_active = record->event.pressed && is_tap_hold_passthrough_active();

    if (!keyboard_config.tap_hold_enabled || passthrough_active || tap_hold_passthrough[index]) {
      if (record->event.pressed) {
        tap_hold_passthrough[index] = passthrough_active;
        register_code(base_key);
      } else {
        unregister_code(base_key);
        tap_hold_passthrough[index] = false;
      }
      return false;
    }

    // Tap-hold is enabled: use timing-based logic
    if (record->event.pressed) {
      // Key pressed - record the timestamp
      tap_hold_key_press_times[index] = record->event.time;
      tap_hold_key_pressed[index] = true;
    } else {
      // Key released - determine if tap or hold
      if (tap_hold_key_pressed[index]) {
        uint16_t elapsed = record->event.time - tap_hold_key_press_times[index];

        if (elapsed < TAP_HOLD_TIMEOUT) {
          // Tap - send key as-is (lowercase/number)
          register_code(base_key);
          unregister_code(base_key);
        } else {
          // Hold - send shift + key (uppercase/shifted symbol)
          register_code(KC_LSFT);
          register_code(base_key);
          unregister_code(base_key);
          unregister_code(KC_LSFT);
        }
        tap_hold_key_pressed[index] = false;
      }
    }
    return false;  // Don't let QMK handle this key
#endif
  }

  switch (keycode) {
    case KB_LOCK:
      if (record->event.pressed) {
        is_locked = !is_locked;
      }
      return false;
    case KB_TAP_HOLD:
#ifndef DISABLE_TAP_HOLD
      if (record->event.pressed) {
        keyboard_config.tap_hold_enabled = !keyboard_config.tap_hold_enabled;
        eeconfig_update_user(keyboard_config.raw);
      }
#endif
      return false;
    case MO(LY1):
      // Fn: only perform normal layer switching; do not toggle scroll mode
      return true;  // Allow normal layer switching to continue
    case KC_SELECT: case JS_4:
      // Select key enables scroll mode while held (preserve tap behavior)
      select_button_pressed = record->event.pressed;
      if (record->event.pressed) {
          select_button_scrolled = false;
      } else {
          if (!select_button_scrolled) {
              if (keycode == KC_SELECT) {
                  register_code(KC_SELECT);
                  unregister_code(KC_SELECT);
              } else {
                  register_joystick_button(keycode - JS_0);
                  unregister_joystick_button(keycode - JS_0);
              }
          }
      }
      return false;
    case JS_LEFT:
      js_left_held = record->event.pressed;
      if (record->event.pressed) js_h_last = -1;
      js_update_axes();
      return false;
    case JS_RGHT:
      js_right_held = record->event.pressed;
      if (record->event.pressed) js_h_last = 1;
      js_update_axes();
      return false;
    case JS_UP:
      js_up_held = record->event.pressed;
      if (record->event.pressed) js_v_last = -1;
      js_update_axes();
      return false;
    case JS_DOWN:
      js_down_held = record->event.pressed;
      if (record->event.pressed) js_v_last = 1;
      js_update_axes();
      return false;
    case MS_BTN3:
      if (record->event.pressed && select_button_pressed) {
          precision_mode = !precision_mode;
          select_button_scrolled = true;
          return false;
      }
      return true;
    default:
      return true;
  }
}
