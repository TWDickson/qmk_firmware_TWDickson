// keymap.c - TWDickson keymap for Boardsource Lulu keyboard
#include QMK_KEYBOARD_H
#include <stdbool.h> // For true and false
#include <stdint.h>  // For uint16_t and other fixed-width integer types
#include <quantum/color.h> // For RGB Color functions
#include <config.h>
#include <print.h> // Debug printing
#include "oled_animation.h"
#include "timeout_fade.h"

// Define RGB_MATRIX_TIMEOUT in config.h if not already defined
#ifndef RGB_MATRIX_TIMEOUT
#define RGB_MATRIX_TIMEOUT
#endif

enum layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST
};

// Boardsource Lulu specific defines
#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)
#define KC_CAD LALT(LCTL(KC_DEL))  // Ctrl+Alt+Del Macro, I use this for my work laptop

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

#pragma region: QWERTY
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
#pragma endregion

#pragma region: Lower
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
#pragma endregion

#pragma region: Raise
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
#pragma endregion

#pragma region: Adjust
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
#pragma endregion
};

#pragma region: Encoder Map
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
#pragma endregion

void keyboard_post_init_user(void) {
    // Initialize the timer system for LED timeout
    init_timeout_fade();

    #ifdef RGB_MATRIX_ENABLE
    // Enable RGB matrix and set initial mode
    rgb_matrix_enable();
    #endif

    // Log that the keyboard initialization is complete
    uprintf("Keyboard post-init complete. RGB Matrix enabled: %s\n",
            #ifdef RGB_MATRIX_ENABLE
            "Yes"
            #else
            "No"
            #endif
    );
}

// Replace the housekeeping_task_user function:
void housekeeping_task_user(void) {
    #ifdef RGB_MATRIX_ENABLE
    handle_rgb_timeout();
    #endif
}

// Replace the process_record_user function:
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Log the keycode and event
        uprintf("Key pressed: 0x%04X\n", keycode);

        // Reset timer on key press
        reset_idle_timer();

        #ifdef RGB_MATRIX_ENABLE
        // Restore LEDs if they were dimmed or off
        restore_rgb_state();
        #endif

        #ifdef OLED_ENABLE
        // Call any OLED-specific keypress handling
        oled_process_key_press(keycode);
        #endif
    } else {
        // Log the key release event
        uprintf("Key released: 0x%04X\n", keycode);
    }
    return true;
}
