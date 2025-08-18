// keymap.h - TWDickson keymap for Boardsource Lulu keyboard

#pragma once

#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>
#include <quantum/color.h> // For RGB Color functions
#include <config.h>
#include <print.h> // Debug printing
#include "oled_animation.h"
#include "timeout_fade.h"

// Layer definitions
enum layers {
    _QWERTY,
    _GAME,
    _LOWER,
    _RAISE,
    _ADJUST
};

// Custom keycodes
enum custom_keycodes {
    GAME_TOGGLE = SAFE_RANGE,
};

// Define RGB_MATRIX_TIMEOUT in config.h if not already defined
#ifndef RGB_MATRIX_TIMEOUT
#define RGB_MATRIX_TIMEOUT
#endif

// Function declarations
// Function declarations
bool process_record_user(uint16_t keycode, keyrecord_t *record);
layer_state_t layer_state_set_user(layer_state_t state);
void keyboard_post_init_user(void);
void housekeeping_task_user(void);
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max);

// External declarations for shared variables
extern const uint16_t PROGMEM base_keymap[MATRIX_ROWS][MATRIX_COLS];
