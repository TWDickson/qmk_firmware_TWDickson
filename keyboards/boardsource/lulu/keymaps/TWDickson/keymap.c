// Copyright 2022 Cole Smith <cole@boadsource.xyz>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <stdbool.h> // For true and false
#include <stdint.h>  // For uint16_t and other fixed-width integer types
#include <quantum/color.h> // For RGB Color functions
#include <config.h>
#include <print.h> // Debug printing

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

// LED timeout and dimming settings
// #define LED_TIMEOUT 3     // Timeout in minutes
// #define LED_TIMEOUT_MS (LED_TIMEOUT * 60 * 1000)  // Convert to milliseconds
#define LED_TIMEOUT_MS (30 * 1000)  // Convert to milliseconds
#define FADE_DURATION 5000 // Fade duration in milliseconds (2 seconds)

// Dynamically calculate fade steps based on fade duration and refresh rate
#define FADE_STEPS (FADE_DURATION / RGB_MATRIX_LED_FLUSH_LIMIT) // Calculate fade steps dynamically

// Update fade interval based on dynamic fade steps
#define FADE_INTERVAL (FADE_DURATION / FADE_STEPS)

static uint32_t idle_timer = 0;      // Timer for LED timeout
static uint32_t fade_timer = 0;      // Timer for fade animation steps
static uint16_t fade_step = 0;        // Current fade step
static bool led_active = true;       // Whether LEDs are active
static bool fade_running = false;    // Whether fade animation is running
static hsv_t led_hsv = {.h = 0, .s = 255, .v = 255};    // Initial HSV Colour


// Boardsource Lulu specific defines
#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)
#define KC_CAD LALT(LCTL(KC_DEL))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// region: QWERTY
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
// endregion

// region: Lower
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
// endregion

// region: Raise
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
// endregion

// region: Adjust
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
// endregion

// region: Encoder Map
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
// endregion

#ifdef RGB_MATRIX_ENABLE
// Properly handle fading the LEDs out/in

void handle_rgb_timeout(void) {
    // Check if we should start fade-out animation
    if (led_active && !fade_running && timer_elapsed32(idle_timer) > LED_TIMEOUT_MS) {
        // Log that the fade-out process is starting
        uprintf("Starting fade-out process. Idle time: %lu ms\n", timer_elapsed32(idle_timer));

        // Start the fade-out process
        fade_running = true;
        fade_step = 0;
        fade_timer = timer_read32();

        // Save the current RGB mode to restore later
        led_hsv = rgb_matrix_get_hsv();
        uprintf("Saved current HSV: H=%d, S=%d, V=%d\n", led_hsv.h, led_hsv.s, led_hsv.v);
    }

    // Update fade animation with easing for smooth transitions
    if (fade_running) {
        if (timer_elapsed32(fade_timer) > FADE_INTERVAL) {
            fade_timer = timer_read32();
            fade_step++;

            // If fade_step within range or already at 0, continue fading
            if ((fade_step <= FADE_STEPS) && (rgb_matrix_get_hsv().v != 0)) {
                // Use quadratic easing for smoother fade-out
                float progress = (float)(FADE_STEPS - fade_step) / FADE_STEPS;
                float eased = progress * progress; // Quadratic ease-out
                uint8_t new_value = (uint8_t)(led_hsv.v * eased);

                // Log the current fade step and brightness value
                uprintf("Fade step: %d/%d, Brightness: %d\n", fade_step, FADE_STEPS, new_value);

                // Set all LEDs to get progressively dimmer
                rgb_matrix_sethsv_noeeprom(led_hsv.h, led_hsv.s, new_value);
            } else {
                // Fade complete, turn off LEDs and prevent restore
                uprintf("Fade-out complete. Turning off LEDs.\n");
                rgb_matrix_disable_noeeprom();
                fade_running = false;
                led_active = false;
                idle_timer = UINT32_MAX; // Prevent accidental restore
            }
        }
    }
}

// Reset the LED state to full brightness
void restore_rgb_state(void) {
    // Only restore if LEDs are off or fading
    if (!led_active || fade_running) {
        // Cancel any ongoing fade
        fade_running = false;

        // Enable the RGB matrix
        rgb_matrix_enable_noeeprom();

        // Set mode back to what it was before fade-out

        // Set brightness to full
        rgb_matrix_sethsv_noeeprom(led_hsv.h, led_hsv.s, led_hsv.v);

        led_active = true;

        // Reset the idle timer
        idle_timer = timer_read32();
    }
}
#endif

// Handle LED initialization
void keyboard_post_init_user(void) {
    // Initialize the timer
    idle_timer = timer_read32();

    #ifdef RGB_MATRIX_ENABLE
    // Enable RGB matrix and set initial mode
    rgb_matrix_enable();
    led_hsv = rgb_matrix_get_hsv();
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

// This function runs on every matrix scan
void housekeeping_task_user(void) {
    #ifdef RGB_MATRIX_ENABLE
    handle_rgb_timeout();
    #endif
}

// Any keypress will reset the timer and restore LEDs
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Log the keycode and event
        uprintf("Key pressed: 0x%04X\n", keycode);

        // Reset timer on key press
        idle_timer = timer_read32();

        #ifdef RGB_MATRIX_ENABLE
        // Restore LEDs if they were dimmed or off
        restore_rgb_state();
        #endif
    } else {
        // Log the key release event
        uprintf("Key released: 0x%04X\n", keycode);
    }
    return true;
}