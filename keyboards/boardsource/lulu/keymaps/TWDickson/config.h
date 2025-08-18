#pragma once

// Split keyboard settings
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_TRANSPORT_MIRROR

// Specific settings for OLED and timeout sharing
// #define SPLIT_OLED_ENABLE
#define SPLIT_TRANSACTION_IDS_USER TIMEOUT_FADE_SYNC

// Default RGB matrix values
// H: 240, S: 239, V: 150
#define RGB_MATRIX_STARTUP_HUE 240    // Blue base color (0-255)
#define RGB_MATRIX_STARTUP_SAT 239    // Full saturation (0-255)
#define RGB_MATRIX_STARTUP_VAL 150    // 50% brightness (0-255)
#define RGB_MATRIX_STARTUP_MODE ALPHAS_MODS // Dual colors mode for alphas/mods

// More accurate RGB matrix settings in the adjust layer
#define RGBLIGHT_HUE_STEP 8           // Steps per hue change
#define RGBLIGHT_SAT_STEP 8           // Steps per saturation change
#define RGBLIGHT_VAL_STEP 8           // Steps per brightness change

#define DYNAMIC_KEYMAP_LAYER_COUNT 5 // Accomdate game mode, default 4 for lulu
