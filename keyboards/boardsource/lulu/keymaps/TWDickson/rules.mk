# rules.mk - TWDickson keymap for Boardsource Lulu keyboard
SRC += oled_animation.c
SRC += lib/oled.c
VIA_ENABLE = yes
ENCODER_MAP_ENABLE = yes

AUTO_SHIFT_ENABLE = no // disable auto-shift with home row mods
NKRO_ENABLE = yes
MAGIC_ENABLE = yes

RGB_MATRIX_ENABLE = yes
LTO_ENABLE = yes

OLED_ENABLE = yes

# Debugging
CONSOLE_ENABLE = yes
SPLIT_LAYER_STATE_ENABLE = yes

WPM_ENABLE		= yes
SPLIT_LAYER_STATE_ENABLE = yes