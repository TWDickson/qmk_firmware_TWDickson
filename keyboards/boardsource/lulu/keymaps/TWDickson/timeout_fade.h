// timeout_fade.h - LED timeout and fade animation for Boardsource Lulu keyboard
// Copyright 2025 TWDickson
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>

// Function prototypes
void init_timeout_fade(void);
void handle_rgb_timeout(void);
void restore_rgb_state(void);
void reset_idle_timer(void);