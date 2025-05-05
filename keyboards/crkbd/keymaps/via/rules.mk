MOUSEKEY_ENABLE = yes     # Mouse keys
VIA_ENABLE      = yes    # Enable VIA
# COMBO_ENABLE        = yes

PIMORONI_TRACKBALL_SCALE=15

SERIAL_DRIVER = vendor
# PS2_MOUSE_ENABLE = yes
# PS2_ENABLE = yes
PS2_DRIVER = vendor

SRC += features/achordion.c


## FLASH COMMAND:
# make crkbd/rev1:via:flash -e USER_NAME=idank -e POINTING_DEVICE=trackball_trackpoint -e TRACKBALL_RGB_RAINBOW=yes  -e SIDE=right -j8
# make crkbd/rev1:via:flash -e USER_NAME=idank -e POINTING_DEVICE=trackball_trackpoint -e TRACKBALL_RGB_RAINBOW=yes  -e SIDE=left -j8
