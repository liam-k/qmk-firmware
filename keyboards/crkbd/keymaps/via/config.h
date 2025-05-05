/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* Select hand configuration */

// #define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

#define TAPPING_TERM 160  // Sets the maximum time (in milliseconds) for a key to be considered a tap
#define ONESHOT_TAP_TOGGLE 2

#define OLED_FONT_H "keyboards/crkbd/lib/glcdfont.c"

#define SERIAL_USART_TX_PIN GP1  // Sets the USART TX pin to GP1 for serial communication

#define MOUSE_EXTENDED_REPORT  // Enables extended mouse reports for more precise cursor control
#define PS2_MOUSE_USE_REMOTE_MODE  // Uses remote mode for PS/2 mouse, which can be more reliable than streaming mode

#define POINTING_DEVICE_DEBUG

#define PS2_PIO_USE_PIO1  // Uses PIO1 for PS/2 communication instead of PIO0, which is used for serial

#define PS2_MOUSE_INVERT_X  // Inverts the X-axis of the PS/2 mouse input
#define PS2_MOUSE_INVERT_Y  // Inverts the Y-axis of the PS/2 mouse input
#define PS2_DATA_PIN    GP2  // Sets the PS/2 data pin to GP2
#define PS2_CLOCK_PIN   GP3  // Sets the PS/2 clock pin to GP3
