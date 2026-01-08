// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum {
  LY0 = 0,
  LY1,
  LY2
};

enum {
  JS_LEFT = SAFE_RANGE,
  JS_RGHT,
  JS_UP,
  JS_DOWN,
  KB_LOCK
};

const key_override_t vol_key_override =
  ko_make_basic(MOD_MASK_SHIFT, KC_VOLD, KC_VOLU);

const key_override_t *key_overrides[] = {&vol_key_override};

const uint16_t PROGMEM bootloader_combo[] = {KC_LALT, KC_RALT, JS_5, COMBO_END};
combo_t key_combos[] = {COMBO(bootloader_combo, QK_BOOT)};

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
        KC_UP,   KC_DOWN, KC_LEFT, KC_RGHT, JS_0,    JS_1,    JS_2,    JS_3,
        KC_LSFT, KC_RSFT, KC_LCTL, KC_RCTL, KC_LALT, MS_BTN1, KC_RALT, MS_BTN2,
        MS_BTN3, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,

        JS_4,    JS_5,    KC_VOLD, KC_GRV,  KC_LBRC, KC_RBRC, KC_MINS, KC_EQL,
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,
        KC_9,    KC_0,    KC_ESC,  KC_TAB,  KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,
        KC_O,    KC_P,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,
        KC_J,    KC_K,    KC_L,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,
        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS, KC_SCLN, KC_QUOT,
        KC_BSPC, KC_ENT,  MO(LY1), MO(LY1), KC_SPC,  KC_NO,   KC_NO,   KC_NO
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
     * (   )(   )(   )(   )(   )(   )(   )(   )(Bdn)(Bup)(   )
     *  (  )(   )(Cmd)(      BlStp       )(Cmd)(   )(  )
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
     *   (JS_L)           (   ) (   )          (   ) (   )
     * (JS_U) (JS_D)                             (   ) (   )
     *   (JS_R)                                   (   )
     *
     * [Note: D-pad keys mapped to Joystick Axis]
     */
    [LY2] = LAYOUT(
        JS_LEFT, JS_RGHT, JS_UP,   JS_DOWN, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,

        _______, _______, _______, _______, _______, _______, _______, _______,
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

// Extern from trackball.c to control scroll mode
extern volatile bool select_button_pressed;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (is_locked && keycode != KB_LOCK && keycode != MO(LY1)) {
    return false;
  }

  switch (keycode) {
    case KB_LOCK:
      if (record->event.pressed) {
        is_locked = !is_locked;
      }
      return false;
    case MO(LY1):
      // Fn key toggles scroll mode (trackball scrolling)
      select_button_pressed = record->event.pressed;
      return true;  // Allow normal layer switching to continue
    case JS_LEFT:
      joystick_set_axis(1, record->event.pressed ? -127 : 0);
      return false;
    case JS_RGHT:
      joystick_set_axis(1, record->event.pressed ? 127 : 0);
      return false;
    case JS_UP:
      joystick_set_axis(0, record->event.pressed ? -127 : 0);
      return false;
    case JS_DOWN:
      joystick_set_axis(0, record->event.pressed ? 127 : 0);
      return false;
    default:
      return true;
  }
}
