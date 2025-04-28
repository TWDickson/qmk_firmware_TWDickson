// Copyright 2022 Cole Smith <cole@boadsource.xyz>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <stdbool.h> // For true and false
#include <stdint.h>  // For uint16_t and other fixed-width integer types

enum layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST
};

// LED timeout settings
#define LED_TIMEOUT 3     // Timeout in minutes
#define LED_TIMEOUT_MS (LED_TIMEOUT * 60 * 1000)  // Convert to milliseconds
static uint16_t idle_timer = 0;
static bool led_on = true;
static uint8_t saved_rgb_mode = 0;
static uint8_t saved_rgb_val = 0;

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)
#define KC_CAD	LALT(LCTL(KC_DEL))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  =   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LCTRL |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|   [   |    |KC_MPLY|------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LGUI | LALT |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT(
  QK_GESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
  KC_LCTL,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
LSFT_T(KC_LBRC),  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_LBRC,   KC_MPLY,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  RSFT_T(KC_RBRC),
                          KC_LGUI,    KC_LALT, LOWER, KC_SPC,           KC_ENT,   RAISE,   KC_BSPC, KC_RGUI
),

/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |KC_GRV|  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |  Up  |      |      | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      | Left | Down | Right|      |      |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /      /       \      \   |      |      |      |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_LOWER] = LAYOUT(
   KC_GRV,  KC_F1,  KC_F2,   KC_F3,   KC_F4,   KC_F5,                       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  _______, _______, _______, _______, _______, _______,                   _______, _______,   KC_UP, _______, _______,   KC_F12,
  _______, _______, _______, _______, _______, _______,                   _______, KC_LEFT, KC_DOWN,KC_RIGHT, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BSLS, _______,
                             _______, _______, _______, _______, _______, _______,  KC_DEL, _______
),

/* RAISE
 * ,-----------------------------------------.                    ,------------------------------------------.
 * |      |      |      |      |      |      |                    |      |       |       |      |      |      |
 * |------+------+------+------+------+------|                    |------+-------+-------+------+------+------|
 * |      |      |      |      |      |      |                    |      |       |KC_PGUP|      |      |      |
 * |------+------+------+------+------+------|                    |------+-------+-------+-------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |KC_HOME|KC_PGDN|KC_END|      |      |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+-------+-------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |       |       |      |      |      |
 * `-----------------------------------------/       /     \      \------------------------------------------'
 *                   |      |      |      | /      /       \      \  |      |      |      |
 *                   |      |      |      |/       /         \      \|      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_RAISE] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______,_______, _______, _______,
  _______, _______, _______, _______, _______, _______,                   _______, _______, KC_PGUP,_______, _______, _______,
  _______, _______, _______, _______, _______, _______,                   _______, KC_HOME, KC_PGDN, KC_END,  _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                             _______, _______, _______, _______, _______, _______, _______, _______
),

/* ADJUST
 * ,----------------------------------------------.                    ,-----------------------------------------.
 * |      |        |       |       |       |      |                    |      |       |      |      |      |      |
 * |------+--------+-------+-------+-------+------|                    |------+-------+------+------+------+------|
 * |      |        |       |       |       |      |                    |      |       |      |      |      |      |
 * |------+--------+-------+-------+-------+------|                    |------+-------+------+------+------+------|
 * |      |RGB_MOD |RGB_HUI|RGB_SAI|RGB_VAI|      |-------.    ,-------|      |       |      |      |      |      |
 * |------+--------+-------+-------+-------+------|RGB_TOG|    |       |------+-------+------+------+------+------|
 * |      |RGB_RMOD|RGB_HUD|RGB_SAD|RGB_VAD|      |-------|    |-------|      |CG_TOGG|      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
  [_ADJUST] = LAYOUT(
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, RGB_TOG, XXXXXXX, XXXXXXX, CG_TOGG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                             _______, _______, _______, _______, _______,  _______, KC_CAD, _______
  )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
  [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
  [_LOWER]  = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD), ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
  [_RAISE]  = { ENCODER_CCW_CW(RGB_SAD, RGB_SAI),  ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
  [_ADJUST] = { ENCODER_CCW_CW(_______, _______),  ENCODER_CCW_CW(_______, _______) }
};
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
   return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}


// Handle dimming LEDs when idle
void keyboard_post_init_user(void) {
  // Initialize the timer
  idle_timer = timer_read();
  // Store initial RGB settings
  saved_rgb_mode = rgb_matrix_get_mode();
  saved_rgb_val = rgb_matrix_get_val();
}

// This function runs on every matrix scan
void housekeeping_task_user(void) {
  // Check if LEDs should fade
  if (led_on && timer_elapsed(idle_timer) > LED_TIMEOUT_MS) {
      // Save current settings before dimming
      saved_rgb_mode = rgb_matrix_get_mode();
      saved_rgb_val = rgb_matrix_get_val();

      // Fade out LEDs (you can adjust based on your preference)
      rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv_noeeprom(HSV_WHITE);
      rgb_matrix_set_val_noeeprom(0);  // 0 brightness = off

      led_on = false;
  }
}

// Any keypress will reset the timer and restore LEDs
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
      // Reset timer on key press
      idle_timer = timer_read();

      // If LEDs were off, turn them back on
      if (!led_on) {
          // Restore saved settings
          rgb_matrix_mode_noeeprom(saved_rgb_mode);
          rgb_matrix_set_val_noeeprom(saved_rgb_val);
          led_on = true;
      }
  }
  return true;
}
