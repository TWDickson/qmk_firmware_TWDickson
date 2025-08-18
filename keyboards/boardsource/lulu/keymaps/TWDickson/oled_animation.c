// oled_animation.c
#include "oled_animation.h"
#include <stdio.h>
#include <stdint.h>
#include "oled_driver.h" // Ensure this header is included for oled_rotation_t
#include <print.h> // Debug printing
#include <stdbool.h>
#include "lib/oled.h"
#include "timeout_fade.h" // Include the timeout_fade header for OLED functions
#include "keymap.h" // Layer state

unsigned int state = 0;

// Your rendering function
// Enhanced render_space function with retro GAME overlay (corrected for OLED rotation)
static void render_space(bool debug) {
    char wpm = get_current_wpm();
    bool game_mode = (biton(default_layer_state) == _GAME);

    if (debug) {
        printf("Render Space: %d, State: %d, Game Mode: %s\n", wpm, state, game_mode ? "ON" : "OFF");
    }

    char render_row[128];
    int i;

    // Row 1 (top row on master, bottom row on rotated secondary)
    oled_set_cursor(0,0);
    for(i=0; i<wpm/4; i++) {
        render_row[i] = pgm_read_byte(space_row_1+i+state);
    };
    for(i=wpm/4; i<128; i++) {
        render_row[i] = (pgm_read_byte(space_row_1+i+state)&pgm_read_byte(mask_row_1+i-wpm/4)) | pgm_read_byte(ship_row_1+i-wpm/4);
    };

    // Add GAME overlay to row 1 if in game mode (this will be BOTTOM of text on rotated display)
    if (game_mode) {
        // Simple horizontal text pattern for "GAME" using basic pixel patterns
        // G - position 90-95
        if (90 < 128) render_row[90] |= 0x7E;  // 01111110
        if (91 < 128) render_row[91] |= 0x81;  // 10000001
        if (92 < 128) render_row[92] |= 0x81;  // 10000001
        if (93 < 128) render_row[93] |= 0x89;  // 10001001
        if (94 < 128) render_row[94] |= 0x8F;  // 10001111
        if (95 < 128) render_row[95] |= 0x4E;  // 01001110

        // A - position 96-101
        if (96 < 128) render_row[96] |= 0xFE;  // 11111110
        if (97 < 128) render_row[97] |= 0x11;  // 00010001
        if (98 < 128) render_row[98] |= 0x11;  // 00010001
        if (99 < 128) render_row[99] |= 0x11;  // 00010001
        if (100 < 128) render_row[100] |= 0x11; // 00010001
        if (101 < 128) render_row[101] |= 0xFE; // 11111110

        // M - position 102-107
        if (102 < 128) render_row[102] |= 0xFF; // 11111111
        if (103 < 128) render_row[103] |= 0x06; // 00000110
        if (104 < 128) render_row[104] |= 0x18; // 00011000
        if (105 < 128) render_row[105] |= 0x18; // 00011000
        if (106 < 128) render_row[106] |= 0x06; // 00000110
        if (107 < 128) render_row[107] |= 0xFF; // 11111111

        // E - position 108-113
        if (108 < 128) render_row[108] |= 0xFF; // 11111111
        if (109 < 128) render_row[109] |= 0x91; // 10010001
        if (110 < 128) render_row[110] |= 0x91; // 10010001
        if (111 < 128) render_row[111] |= 0x91; // 10010001
        if (112 < 128) render_row[112] |= 0x91; // 10010001
        if (113 < 128) render_row[113] |= 0x81; // 10000001
    }

    oled_write_raw(render_row, 128);
    if (debug) printf("Row 1 rendered\n");

    // Row 2
    oled_set_cursor(0,1);
    for(i=0; i<wpm/4; i++) {
        render_row[i] = pgm_read_byte(space_row_2+i+state);
    };
    for(i=wpm/4; i<128; i++) {
        render_row[i] = (pgm_read_byte(space_row_2+i+state)&pgm_read_byte(mask_row_2+i-wpm/4)) | pgm_read_byte(ship_row_2+i-wpm/4);
    };

    oled_write_raw(render_row, 128);
    if (debug) printf("Row 2 rendered\n");

    // Row 3
    oled_set_cursor(0,2);
    for(i=0; i<wpm/4; i++) {
        render_row[i] = pgm_read_byte(space_row_3+i+state);
    };
    for(i=wpm/4; i<128; i++) {
        render_row[i] = (pgm_read_byte(space_row_3+i+state)&pgm_read_byte(mask_row_3+i-wpm/4)) | pgm_read_byte(ship_row_3+i-wpm/4);
    };

    oled_write_raw(render_row, 128);
    if (debug) printf("Row 3 rendered\n");

    // Row 4 (bottom row on master, TOP row on rotated secondary)
    oled_set_cursor(0,3);
    for(i=0; i<wpm/4; i++) {
        render_row[i] = pgm_read_byte(space_row_4+i+state);
    };
    for(i=wpm/4; i<128; i++) {
        render_row[i] = (pgm_read_byte(space_row_4+i+state)&pgm_read_byte(mask_row_4+i-wpm/4)) | pgm_read_byte(ship_row_4+i-wpm/4);
    };

    oled_write_raw(render_row, 128);
    if (debug) printf("Row 4 rendered\n");

    int old_state = state;
    state = (state + 1 + (wpm/15)) % (128*2);
    if (debug) printf("State updated: %d -> %d (WPM increment: %d)\n", old_state, state, wpm/15);
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
    printf("Logo rendered on secondary side\n");
}

// Rotated version of the layer state renderer
void render_rotated_layer_state(void) {
    // Get current layer
    uint8_t layer = get_highest_layer(layer_state);
    // For non-rotated display, just call the original function
    if (is_keyboard_master()) {
        render_layer_state();
        return;
    }

    // For rotated display, we need to flip the image
    const char* layer_img;
    uint16_t layer_img_size;

    // Select the appropriate image based on the layer
    switch (layer) {
        case 0:
            layer_img = layer0_img;
            layer_img_size = sizeof(layer0_img);
            break;
        case 1:
            layer_img = layer1_img;
            layer_img_size = sizeof(layer1_img);
            break;
        case 2:
            layer_img = layer2_img;
            layer_img_size = sizeof(layer2_img);
            break;
        case 3:
            layer_img = layer3_img;
            layer_img_size = sizeof(layer3_img);
            break;
        default:
            layer_img = layer0_img;
            layer_img_size = sizeof(layer0_img);
            break;
    }

    // Temporary buffer for the flipped image data
    // Change to char type to match what oled_write_raw expects
    static char flipped_data[sizeof(layer0_img)];

    // Reverse the byte order and bit order
    for (uint16_t i = 0; i < layer_img_size; i++) {
        // Read the byte from the end of the array
        uint8_t src_byte = pgm_read_byte(&layer_img[layer_img_size - 1 - i]);

        // Reverse the bits within the byte
        uint8_t reversed_byte = 0;
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (src_byte & (1 << bit)) {
                reversed_byte |= (1 << (7 - bit));
            }
        }

        flipped_data[i] = (char)reversed_byte;
    }

    // Write the flipped data to the display
    oled_write_raw(flipped_data, layer_img_size);
}

// OLED task handler specific to the animation
bool oled_task_user_animation(bool debug) {
    if (!oled_panel_active) {
        if (debug) {
            printf("OLED Task User Animation - OLED is off, skipping rendering\n");
        }
        // Call OLED Off Again
        oled_clear();
        if (!is_oled_on()) {
            oled_off();
        }
        return false; // Skip rendering if OLED is off
    }


    if (debug) {
        printf("OLED Task User Animation - Debug mode: %d\n", debug);
    }
    static uint8_t last_layer = 255; // Initialize to an impossible layer
    uint8_t current_layer = get_highest_layer(layer_state);


    if (is_keyboard_master()) {
        if (debug) {
            // printf("OLED Task User Animation - Master side (Layer: %d)\n", current_layer);
        }

        render_space(false);
    } else {
        // On secondary half
        if (debug) {
            printf("OLED Task User Animation - Secondary side (Layer: %d)\n", current_layer);
        }
        // Only redraw if the layer has actually changed
        if (last_layer != current_layer || debug) {
            if (debug) {
                printf("Layer changed: %d -> %d, redrawing\n", last_layer, current_layer);
            }
            render_rotated_layer_state();
        }
    }

    last_layer = current_layer;
    return false;
}

// You can add an initialization function if needed
void oled_animation_init(void) {
    printf("OLED Animation Init - Starting state: %d\n", state);
    state = 0;
    // Any other initialization needed
}

void oled_process_key_press(uint16_t keycode) {
    // Handle any OLED-specific actions on keypress
    // For example, you might want to reset animation state
    // or update WPM calculations, etc.
    printf("OLED Key Pressed: %d (0x%04x)\n", keycode, keycode);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        printf("Secondary side OLED initialized with rotation 180\n");
        return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
    }
    printf("Master side OLED initialized with rotation %d\n", rotation);
    return rotation;
}
void render_bootmagic_status(bool status) {
    printf("Render Bootmagic Status: %d\n", status);
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

bool oled_task_user(void) {
    // Call the function from our animation file
    // printf("OLED Task User - Master: %d\n", is_keyboard_master());
    return oled_task_user_animation(false);
}
