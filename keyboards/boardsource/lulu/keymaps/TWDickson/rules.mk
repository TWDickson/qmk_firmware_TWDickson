# rules.mk - TWDickson keymap for Boardsource Lulu keyboard
# Local Imports
SRC += oled_animation.c
SRC += timeout_fade.c
SRC += lib/oled.c

# Compiler Options; this is to fold code without compilier errors, for some reason // region: My Region isn't working
CFLAGS += -Wno-unknown-pragmas

# Options
VIA_ENABLE = yes
ENCODER_MAP_ENABLE = yes

AUTO_SHIFT_ENABLE = no // disable auto-shift with home row mods
NKRO_ENABLE = yes
MAGIC_ENABLE = yes

RGB_MATRIX_ENABLE = yes
OLED_ENABLE = yes
LTO_ENABLE = yes

# Debugging
CONSOLE_ENABLE = no

# Required for OLED
WPM_ENABLE = yes  # used in Rocket Ship Animation
# Not sure if this does anything but we need layer state syncing between sides
SPLIT_LAYER_STATE_ENABLE = yes
