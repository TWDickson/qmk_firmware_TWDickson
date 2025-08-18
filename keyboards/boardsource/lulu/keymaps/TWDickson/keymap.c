// keymap.c - TWDickson keymap for Boardsource Lulu keyboard
#include "keymap.h"

// Custom Key Definitions
#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)
#define KC_CAD LALT(LCTL(KC_DEL))  // Ctrl+Alt+Del Macro, I use this for my work laptop


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
#pragma region: QWERTY
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
 * ,-----------------------------------------------.                    ,-----------------------------------------.
 * |       |        |       |       |       |      |                    |      |       |      |      |      |      |
 * |-------+--------+-------+-------+-------+------|                    |------+-------+------+------+------+------|
 * |       |        |       |       |       |      |                    |      |       |      |      |      |      |
 * |-------+--------+-------+-------+-------+------|                    |------+-------+------+------+------+------|
 * |KC_CAPS|RGB_MOD |RGB_HUI|RGB_SAI|RGB_VAI|GME MD|-------.    ,-------|      |       |      |      |      |      |
 * |-------+--------+-------+-------+-------+------|RGB_TOG|    |       |------+-------+------+------+------+------|
 * |       |RGB_RMOD|RGB_HUD|RGB_SAD|RGB_VAD|      |-------|    |-------|      |CG_TOGG|      |      |      |      |
 * `------------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
  [_ADJUST] = LAYOUT(
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  KC_CAPS, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, GAME_TOGGLE,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, RGB_TOG, XXXXXXX, XXXXXXX, CG_TOGG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                             _______, _______, _______, _______, _______,  _______, KC_CAD, _______
  ),
#pragma endregion

#pragma region: Game
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
 *                   |  NO  | LALT |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

[_GAME] = LAYOUT(
  QK_GESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
  KC_LCTL,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_LBRC,   KC_MPLY,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,
                          KC_NO,    KC_LALT, LOWER, KC_SPC,           KC_ENT,   RAISE,   KC_BSPC, KC_RGUI
) // Placeholder for game layer
#pragma endregion

};


#pragma region: Encoder Map
#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
  [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
  [_GAME] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
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
    #if defined(RGB_MATRIX_ENABLE) || defined(OLED_ENABLE)
    // Call regular RGB timeout handler
    handle_rgb_timeout();

    // Call the housekeeping function for timeout_fade
    timeout_fade_housekeeping();
    #endif
}

// Replace the process_record_user function:
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Log the keycode and event
        uprintf("Key pressed: 0x%04X\n", keycode);
         // Get the key position info
        uint8_t row = record->event.key.row;
        uint8_t col = record->event.key.col;

        // Log key info
        uprintf("Key pressed: 0x%04X at position [%d,%d]\n", keycode, row, col);

        // Find the LED corresponding to this key
        #ifdef RGB_MATRIX_ENABLE
        // LED matrix index lookup
        uint8_t led_index = g_led_config.matrix_co[row][col];

        // If valid LED index found (not 255)
        if (led_index != NO_LED) {
            // Get LED position
            int16_t led_x = g_led_config.point[led_index].x;
            int16_t led_y = g_led_config.point[led_index].y;
            uint8_t led_flags = g_led_config.flags[led_index];

            // Log LED info
            uprintf("  LED: index=%d, x=%d, y=%d, flags=0x%02X\n",
                    led_index, led_x, led_y, led_flags);
        } else {
            uprintf("  No LED mapped to this key position\n");
        }
        #endif

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

        switch (keycode) {
            case RGB_MOD:
            case RGB_RMOD:
            case RGB_HUI:
            case RGB_HUD:
            case RGB_SAI:
            case RGB_SAD:
            case RGB_VAI:
            case RGB_VAD:
            case RGB_TOG: {
                // Get current RGB mode
                uint8_t mode = rgb_matrix_get_mode();

                // Get current HSV values
                hsv_t hsv = rgb_matrix_get_hsv();

                // Log the current RGB state
                uprintf("RGB State - Mode: %d, H: %d, S: %d, V: %d\n",
                        mode, hsv.h, hsv.s, hsv.v);

                // Optional: Log mode name for better readability
                const char* mode_name;
                switch (mode) {
                    case RGB_MATRIX_SOLID_COLOR: mode_name = "SOLID_COLOR"; break;
                    case RGB_MATRIX_ALPHAS_MODS: mode_name = "ALPHAS_MODS"; break;
                    case RGB_MATRIX_GRADIENT_UP_DOWN: mode_name = "GRADIENT_UP_DOWN"; break;
                    case RGB_MATRIX_GRADIENT_LEFT_RIGHT: mode_name = "GRADIENT_LEFT_RIGHT"; break;
                    case RGB_MATRIX_BREATHING: mode_name = "BREATHING"; break;
                    case RGB_MATRIX_BAND_SAT: mode_name = "BAND_SAT"; break;
                    case RGB_MATRIX_BAND_VAL: mode_name = "BAND_VAL"; break;
                    default: mode_name = "UNKNOWN"; break;
                }
                uprintf("RGB Mode Name: %s\n", mode_name);
                break;
            }

            case GAME_TOGGLE:
                // Toggle the game layer on/off
                if (biton(default_layer_state) == _GAME) {
                    set_single_persistent_default_layer(_QWERTY);
                    uprintf("Game layer OFF\n");
                } else {
                    set_single_persistent_default_layer(_GAME);
                    uprintf("Game layer ON\n");
                }
                layer_clear();
                return false; // Prevent further processing of this keycode
        }
    } else {
        // Log the key release event
        uprintf("Key released: 0x%04X\n", keycode);
    }
    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Only process if the LED is in the current range to minimize processing
    if (led_min <= 23 && led_max > 23) {
        // Check if Caps Lock is active and LEDs are currently active
        if (host_keyboard_led_state().caps_lock && led_active) {
            // Only show the Caps Lock indicator if LEDs are active
            // This will let the LED fade out completely with the timeout
            rgb_matrix_set_color(23, 255, 0, 0);
        }
    }

    return false; // Don't continue to keyboard-level callback
}
