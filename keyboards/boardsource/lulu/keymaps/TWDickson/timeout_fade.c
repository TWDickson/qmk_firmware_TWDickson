// timeout_fade.c - Improved version with reliability focus
#include "timeout_fade.h"
#include <stdbool.h>  // For true and false
#include <stdint.h>   // For uint16_t and other fixed-width integer types
#include <quantum/color.h> // For RGB Color functions
#include <print.h>    // Debug printing
#include "transactions.h" // Include the transactions header for split keyboard sync
#include "timer.h"    // Explicitly include timer functions

// LED timeout and dimming settings
#define LED_TIMEOUT_MS (60 * 1000)  // 60 seconds in milliseconds
#define FADE_DURATION 5000 // Fade duration in milliseconds (5 seconds)

// Dynamically calculate fade steps based on fade duration and refresh rate
#define FADE_STEPS (FADE_DURATION / RGB_MATRIX_LED_FLUSH_LIMIT) // Calculate fade steps dynamically
static hsv_t original_led_hsv = {.h = 0, .s = 255, .v = 255}; // Original values before any fading

// Update fade interval based on dynamic fade steps
#define FADE_INTERVAL (FADE_DURATION / FADE_STEPS)

// OLED dimming settings
#define OLED_MAX_BRIGHTNESS 255
#define OLED_MIN_BRIGHTNESS 0
#define OLED_TIMEOUT_MS LED_TIMEOUT_MS  // Use the same timeout for OLED and LEDs

// Sync status and retry logic
#define SYNC_RETRY_INTERVAL 100  // Time in ms between sync retries (increased)
#define SYNC_MAX_RETRIES 3       // Maximum number of retries before giving up (reduced)
#define STARTUP_DELAY 2000       // Delay before first sync attempt (ms) (increased)
#define SYNC_WATCHDOG_TIMEOUT 10000  // 10 seconds without sync is considered a problem (increased)

// Simple sync structure - minimize size and complexity for troubleshooting
#define TF_SYNC_ID TIMEOUT_FADE_SYNC
typedef struct _timeout_sync_t {
    bool oled_panel_active;
    bool led_active;
    uint8_t oled_brightness;
} timeout_sync_t;

// Declare variables to be shared between sides
static uint32_t idle_timer = 0;
static uint32_t fade_timer = 0;
static uint32_t sync_timer = 0;
static uint32_t last_successful_sync = 0;

static bool led_active = true;
bool oled_panel_active = true;  // To share with other functions
static bool fade_running = false;
static uint16_t fade_step = 0;
static uint8_t oled_brightness = OLED_MAX_BRIGHTNESS;
static uint8_t oled_saved_brightness = OLED_MAX_BRIGHTNESS;
static hsv_t led_hsv = {.h = 0, .s = 255, .v = 255};
static bool startup_complete = false;
static bool sync_initialized = false;

// Debug flag to enable verbose logging
static bool debug_sync = true;

// Slave-side handler for user sync
void timeout_sync_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    if (in_buflen >= sizeof(timeout_sync_t)) {
        const timeout_sync_t *sync_data = (const timeout_sync_t*)in_data;

        // Log the first few syncs for debugging
        static uint8_t sync_count = 0;
        if (sync_count < 5 || debug_sync) {
            uprintf("Slave received sync #%d. OLED: %s, LEDs: %s, Brightness: %d\n",
                    sync_count++,
                    sync_data->oled_panel_active ? "ON" : "OFF",
                    sync_data->led_active ? "ON" : "OFF",
                    sync_data->oled_brightness);
        }

        // Update local variables with data from master
        bool oled_state_changed = (oled_panel_active != sync_data->oled_panel_active);
        // bool led_state_changed = (led_active != sync_data->led_active);
        bool brightness_changed = (oled_brightness != sync_data->oled_brightness);

        // Update local variables with data from master
        oled_panel_active = sync_data->oled_panel_active;
        led_active = sync_data->led_active;
        oled_brightness = sync_data->oled_brightness;

        // Update watchdog timer
        last_successful_sync = timer_read32();

        // Apply OLED settings in a consistent order
        #ifdef OLED_ENABLE
        // First update brightness if it changed
        if (brightness_changed && oled_panel_active) {
            oled_set_brightness(oled_brightness);
        }

        // Then turn OLED on/off if needed
        if (oled_state_changed) {
            if (!oled_panel_active && is_oled_on()) {
                uprintf("Slave turning OLED OFF\n");
                // Ensure brightness is at minimum first
                oled_set_brightness(OLED_MIN_BRIGHTNESS);
                oled_off();
            } else if (oled_panel_active && !is_oled_on()) {
                uprintf("Slave turning OLED ON\n");
                oled_on();
                // Set brightness after turning on
                oled_set_brightness(oled_brightness);
            }
        }
        #endif
    }
}

// Function to sync data from master to slave
bool sync_timeout_data(void) {
    if (!is_keyboard_master()) {
        return true; // Slave doesn't need to sync
    }

    // Wait until initialization and startup delay
    if (!sync_initialized || !startup_complete) {
        if (!sync_initialized) {
            uprintf("Sync not initialized yet, skipping sync attempt\n");
            return false;
        }

        if (timer_elapsed32(sync_timer) < STARTUP_DELAY) {
            return false;
        }

        startup_complete = true;
        uprintf("Startup delay complete, beginning sync\n");
    }

    static uint32_t last_sync = 0;

    // Sync data every SYNC_RETRY_INTERVAL ms
    if (timer_elapsed32(last_sync) > SYNC_RETRY_INTERVAL) {
        // Prepare simple sync data
        timeout_sync_t sync_data = {
            .oled_panel_active = oled_panel_active,
            .led_active = led_active,
            .oled_brightness = oled_brightness
        };

        last_sync = timer_read32();

        // Log occasional sync attempts
        static uint8_t sync_attempt_count = 0;
        if (sync_attempt_count++ % 10 == 0 || debug_sync) {
            uprintf("Master sending sync #%d. OLED: %s, LEDs: %s, Brightness: %d\n",
                   sync_attempt_count,
                   oled_panel_active ? "ON" : "OFF",
                   led_active ? "ON" : "OFF",
                   oled_brightness);
        }

        // Use one-way transaction_rpc_send
        if (transaction_rpc_send(TF_SYNC_ID, sizeof(sync_data), &sync_data)) {
            if (debug_sync) {
                uprintf("Sync successful\n");
            }
            return true;
        } else {
            uprintf("Timeout sync to slave failed!\n");
            return false;
        }
    }
    return true;
}

void init_timeout_fade(void) {
    // Initialize the timers
    idle_timer = timer_read32();
    fade_timer = timer_read32();
    sync_timer = timer_read32();
    last_successful_sync = timer_read32();

    #ifdef RGB_MATRIX_ENABLE
    // Save initial HSV color to both current and original
    led_hsv = rgb_matrix_get_hsv();
    original_led_hsv = led_hsv;  // Store original values

    // Important: Make sure we start with full brightness
    if (led_hsv.v < 255) {
        uprintf("Initial brightness low (%d), setting to full\n", led_hsv.v);
        original_led_hsv.v = 255;  // Ensure we start with full brightness
    }
    #endif

    #ifdef OLED_ENABLE
    // Initialize OLED state
    oled_panel_active = is_oled_on();  // Match current state
    oled_brightness = oled_get_brightness();
    oled_saved_brightness = oled_brightness;
    uprintf("OLED init state: %s, brightness: %d\n",
        oled_panel_active ? "ON" : "OFF", oled_brightness);
    #endif
    // Register the transaction handler
    transaction_register_rpc(TF_SYNC_ID, timeout_sync_slave_handler);

    // Mark initialization complete
    sync_initialized = true;

    // Log that initialization is complete
    uprintf("Timeout Fade: Initialization complete\n");
    uprintf("Dynamic fade steps: %d, Fade interval: %d ms\n", FADE_STEPS, FADE_INTERVAL);
    uprintf("Startup delay: %d ms, Sync retry interval: %d ms\n", STARTUP_DELAY, SYNC_RETRY_INTERVAL);
}

void handle_rgb_timeout(void) {
    // Handle slave-side watchdog
    if (!is_keyboard_master()) {
        #ifdef OLED_ENABLE
        if (is_oled_on() && timer_elapsed32(last_successful_sync) > SYNC_WATCHDOG_TIMEOUT) {
            // We haven't received a sync in too long, emergency shut down OLED
            oled_off();
            uprintf("Watchdog timeout - emergency OLED shutdown\n");
        }
        #endif
        return;
    }

    // Check if we should start fade-out animation
    if ((led_active || oled_panel_active) && !fade_running && timer_elapsed32(idle_timer) > LED_TIMEOUT_MS) {
        // Log that the fade-out process is starting
        uprintf("Starting fade-out process. Idle time: %lu ms\n", timer_elapsed32(idle_timer));

        // Start the fade-out process
        fade_running = true;
        fade_step = 0;
        fade_timer = timer_read32();

        // Save the current RGB mode to restore later
        #ifdef RGB_MATRIX_ENABLE
        led_hsv = rgb_matrix_get_hsv();
        // Also update the original_led_hsv if the user changed it
        if (!fade_running) {
            original_led_hsv = led_hsv;
        }
        uprintf("Saved current HSV: H=%d, S=%d, V=%d\n", led_hsv.h, led_hsv.s, led_hsv.v);
        #endif

        // Save current OLED brightness
        #ifdef OLED_ENABLE
        oled_saved_brightness = oled_get_brightness();
        uprintf("Saved OLED brightness: %d\n", oled_saved_brightness);
        #endif

        // Force a sync immediately when starting fade
        sync_timeout_data();
    }

    // Update fade animation with easing for smooth transitions
    if (fade_running) {
        if (timer_elapsed32(fade_timer) > FADE_INTERVAL) {
            fade_timer = timer_read32();
            fade_step++;

            // Calculate the fade progress with integer math
            uint16_t remaining_steps = (FADE_STEPS > fade_step) ? (FADE_STEPS - fade_step) : 0;
            uint8_t progress_percent = (remaining_steps * 100) / FADE_STEPS;
            uint8_t eased_percent = (progress_percent * progress_percent) / 100;

            // Update OLED brightness
            oled_brightness = (oled_saved_brightness * eased_percent) / 100;

            uprintf("Fade step: %d/%d, Progress: %d%%, Eased: %d%%\n",
                   fade_step, FADE_STEPS, progress_percent, eased_percent);

            bool state_changed = false;

            // Handle RGB fading on master
            #ifdef RGB_MATRIX_ENABLE
            if (fade_step <= FADE_STEPS && led_active) {
                uint8_t new_value = (led_hsv.v * eased_percent) / 100;

                // Set all LEDs to get progressively dimmer
                rgb_matrix_sethsv_noeeprom(led_hsv.h, led_hsv.s, new_value);

                // If we've reached near-zero brightness, turn off the LEDs
                if (new_value <= 2 && led_active) {
                    uprintf("RGB Fade-out complete. Turning off LEDs.\n");
                    rgb_matrix_disable_noeeprom();
                    led_active = false;
                    state_changed = true;
                }
            }
            #endif

            // Handle OLED fading on master
            #ifdef OLED_ENABLE
            if (fade_step <= FADE_STEPS && oled_panel_active) {
                // Update OLED brightness
                oled_set_brightness(oled_brightness);
                uprintf("OLED Fade step: %d/%d, Brightness: %d\n", fade_step, FADE_STEPS, oled_brightness);

                // If we've reached near-zero brightness, turn off the OLED
                if (oled_brightness <= 2 && oled_panel_active) {
                    uprintf("OLED Fade-out complete.\n");

                    // First, set all state variables
                    oled_panel_active = false;

                    // Force minimum brightness first
                    oled_set_brightness(OLED_MIN_BRIGHTNESS);

                    // Explicitly call oled_off() and verify it took effect
                    oled_off();

                    // Add additional verification
                    if (is_oled_on()) {
                        uprintf("WARNING: OLED still on after oled_off() call! Retrying...\n");

                        // Second attempt with a small delay
                        wait_ms(10);
                        oled_off();

                        if (is_oled_on()) {
                            uprintf("ERROR: OLED failed to turn off after second attempt!\n");
                        } else {
                            uprintf("OLED successfully turned off after second attempt.\n");
                        }
                    } else {
                        uprintf("OLED confirmed off.\n");
                    }

                    state_changed = true;
                }
            }
            #endif

            // Sync state after any changes
            if (state_changed) {
                sync_timeout_data();
            }

            // Check if all fading is complete
            bool rgb_inactive = !led_active;
            bool oled_inactive = !oled_panel_active;

            #ifndef RGB_MATRIX_ENABLE
            rgb_inactive = true; // If RGB matrix isn't enabled at compile time, consider it inactive
            #endif

            #ifndef OLED_ENABLE
            oled_inactive = true; // If OLED isn't enabled at compile time, consider it inactive
            #endif

            if (rgb_inactive && oled_inactive) {
                // All fading complete
                uprintf("All fade-out complete.\n");
                fade_running = false;
                idle_timer = UINT32_MAX; // Prevent accidental restore
                // Force a final sync when fade is complete
                sync_timeout_data();
            } else if (!state_changed) {
                // Sync periodically during animation for smoother transition
                if (fade_step % 10 == 0) {
                    sync_timeout_data();
                }
            }
        }
    }
}

void restore_rgb_state(void) {
    // Only restore if LEDs are off or fading
    if (!led_active || !oled_panel_active || fade_running) {
        // Cancel any ongoing fade
        fade_running = false;

        // Enable the RGB matrix if it was disabled
        #ifdef RGB_MATRIX_ENABLE
        if (!led_active) {
            rgb_matrix_enable_noeeprom();

            // Force full brightness for value when restoring
            uprintf("Restoring RGB state with original HSV: H=%d, S=%d, V=%d\n",
                    original_led_hsv.h, original_led_hsv.s, original_led_hsv.v);

            // FIXED: Use original_led_hsv.v to restore full brightness
            rgb_matrix_sethsv_noeeprom(original_led_hsv.h, original_led_hsv.s, original_led_hsv.v);

            led_active = true;

            // Update led_hsv to match the original values for next time
            led_hsv = original_led_hsv;
        }
        #endif

        // Restore OLED brightness
        #ifdef OLED_ENABLE
        if (!oled_panel_active || !is_oled_on()) {
            // Force a clean state
            oled_on();
            wait_ms(5); // Small delay to ensure the command takes effect
            oled_set_brightness(oled_saved_brightness);
            oled_panel_active = true;
            oled_brightness = oled_saved_brightness;
            uprintf("OLED brightness restored to %d, state: %s\n",
                oled_saved_brightness,
                is_oled_on() ? "ON" : "OFF");
        }
        #endif

        // Reset the idle timer
        idle_timer = timer_read32();

        // Force a sync immediately when restoring state
        sync_timeout_data();
    }
}

void reset_idle_timer(void) {
    idle_timer = timer_read32();

    // If not currently fading, update the original HSV values
    #ifdef RGB_MATRIX_ENABLE
    if (!fade_running && led_active) {
        // Get the current HSV values
        hsv_t current_hsv = rgb_matrix_get_hsv();

        // Only update hue and saturation, keep the original brightness
        // This is the fix: Preserve the original brightness value
        original_led_hsv.h = current_hsv.h;
        original_led_hsv.s = current_hsv.s;
        // Don't update original_led_hsv.v here unless it's higher
        if (current_hsv.v > original_led_hsv.v) {
            original_led_hsv.v = current_hsv.v;
            uprintf("Updated original brightness to higher value: %d\n", original_led_hsv.v);
        }

        // Update led_hsv to match for consistent behavior
        led_hsv.h = original_led_hsv.h;
        led_hsv.s = original_led_hsv.s;
        led_hsv.v = original_led_hsv.v;
    }
    #endif
}

// Helper function to check if displays are in timeout state
bool is_display_timeout_active(void) {
    return fade_running || (!led_active && !oled_panel_active);
}

// Add a housekeeping function to be called from keymap.c
void timeout_fade_housekeeping(void) {
    // Call sync function from here for more reliable timing
    if (is_keyboard_master()) {
        sync_timeout_data();
    }
}
