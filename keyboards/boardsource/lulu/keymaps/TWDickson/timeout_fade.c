// timeout_fade.c - LED timeout and fade animation for Boardsource Lulu keyboard
// Copyright 2025 TWDickson
// SPDX-License-Identifier: GPL-2.0-or-later

#include "timeout_fade.h"
#include <stdbool.h>  // For true and false
#include <stdint.h>   // For uint16_t and other fixed-width integer types
#include <quantum/color.h> // For RGB Color functions
#include <print.h>    // Debug printing

// LED timeout and dimming settings
#define LED_TIMEOUT_MS (30 * 1000)  // 30 seconds in milliseconds
#define FADE_DURATION 5000 // Fade duration in milliseconds (5 seconds)

// Dynamically calculate fade steps based on fade duration and refresh rate
#define FADE_STEPS (FADE_DURATION / RGB_MATRIX_LED_FLUSH_LIMIT) // Calculate fade steps dynamically

// Update fade interval based on dynamic fade steps
#define FADE_INTERVAL (FADE_DURATION / FADE_STEPS)

// Static variables to track LED state
static uint32_t idle_timer = 0;      // Timer for LED timeout
static uint32_t fade_timer = 0;      // Timer for fade animation steps
static uint16_t fade_step = 0;        // Current fade step
static bool led_active = true;       // Whether LEDs are active
static bool fade_running = false;    // Whether fade animation is running
static hsv_t led_hsv = {.h = 0, .s = 255, .v = 255};    // Initial HSV Colour
// static bool oled_active = true;     // Whether OLED is active
// static oled_brightness = 255; // Initial OLED brightness

void init_timeout_fade(void) {
    // Initialize the timer
    idle_timer = timer_read32();

    #ifdef RGB_MATRIX_ENABLE
    // Save initial HSV color
    led_hsv = rgb_matrix_get_hsv();
    #endif

    // Log that initialization is complete
    uprintf("Timeout Fade: Initialization complete\n");
}

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

void restore_rgb_state(void) {
    // Only restore if LEDs are off or fading
    if (!led_active || fade_running) {
        // Cancel any ongoing fade
        fade_running = false;

        // Enable the RGB matrix
        rgb_matrix_enable_noeeprom();

        // Set brightness to full
        rgb_matrix_sethsv_noeeprom(led_hsv.h, led_hsv.s, led_hsv.v);

        led_active = true;

        // Reset the idle timer
        idle_timer = timer_read32();

        // Log that LEDs have been restored
        uprintf("RGB state restored to H=%d, S=%d, V=%d\n", led_hsv.h, led_hsv.s, led_hsv.v);
    }
}

void reset_idle_timer(void) {
    idle_timer = timer_read32();
}
