// timeout_fade.h - LED and OLED timeout and fade animation for Boardsource Lulu keyboard
// Copyright 2025 TWDickson
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>

// Variables for animation functions
extern bool oled_panel_active;
extern bool led_active;


// Function prototypes
void init_timeout_fade(void);
void handle_rgb_timeout(void);
void restore_rgb_state(void);
void reset_idle_timer(void);
bool is_display_timeout_active(void);
void timeout_fade_housekeeping(void);
