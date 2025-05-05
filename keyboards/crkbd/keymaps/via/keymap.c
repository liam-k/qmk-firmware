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

#include "action.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "features/achordion.h"

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
        KC_ESC,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      LT(2,KC_Y),    KC_U,  KC_I,  KC_O, KC_LBRC, KC_P,
        LT(3, KC_TAB), KC_A, CTL_T(KC_S), OPT_T(KC_D), CMD_T(KC_F), KC_G,  KC_H, CMD_T(KC_J), OPT_T(KC_K), CTL_T(KC_L), KC_SCLN, LT(2,KC_QUOT),

        LT(1, KC_ESC),   LT(1,KC_Z),    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,    KC_COMM, KC_DOT,  LT(2,	KC_SLSH), LT(2,KC_BSPC),

                           LT(2,KC_SPACE), LT(3, KC_SPACE), OSM(MOD_LSFT),   KC_MS_BTN1, OSM(MOD_RSFT), LT(1,KC_ENT)
    ),
    [1] = LAYOUT_split_3x6_3(
        KC_TRNS,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,         	 KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_TRNS,  KC_NO,   KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,        	KC_LEFT, MT(MOD_LGUI,KC_DOWN), MT(MOD_LALT|MOD_RALT,KC_UP), MT(MOD_LCTL|MOD_RCTL,KC_RGHT), KC_NO, KC_TRNS,
        KC_TRNS,  RGB_TOG, KC_NO,   KC_NO,  KC_NO,  KC_NO,              	KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_TRNS,
                           KC_TRNS, KC_TRNS, KC_TRNS,                       KC_MS_BTN1, KC_MS_BTN2, KC_MS_BTN3
    ),
    [2] = LAYOUT_split_3x6_3(
        KC_VOLU,   KC_MPRV, KC_BSPC, KC_UP,   KC_DEL,  KC_MNXT,              S(KC_6), KC_7,    KC_8,    KC_9,    S(KC_P), KC_BSPC,
        KC_MPLY,  LAG(KC_LEFT), KC_LEFT, KC_DOWN, KC_RGHT, LAG(KC_RGHT),    KC_0,    KC_4,    KC_5,    KC_6,    KC_BSLS, KC_GRV,
        KC_VOLD,  KC_WWW_BACK,   OPT_T(KC_Z),  OPT_T(KC_I), OPT_T(KC_Y),  KC_WWW_FORWARD,       KC_0,    KC_1,    KC_2,    KC_3,    KC_ENT,  S(KC_GRV),
                           KC_LGUI, KC_TRNS, KC_SPC,                        KC_MS_BTN1, KC_MS_BTN2, KC_MS_BTN3
    ),
    [3] = LAYOUT_split_3x6_3(

    	KC_ESC,   LALT(KC_Q),    LALT(KC_W),    LALT(KC_E),    LALT(KC_R),    LALT(KC_T),   LALT(KC_Y),    LALT(KC_U),    LALT(KC_I),    LALT(KC_O),    LALT(KC_P),    LALT(KC_LBRC),
     	KC_TRNS, LALT(KC_A), LALT(KC_S), LALT(KC_D), LALT(KC_F), LALT(KC_G), LALT(KC_H), LALT(KC_J), LALT(KC_K),LALT(KC_L), LALT(KC_SCLN),LALT(KC_QUOT),
      KC_ESC,  LALT(KC_Z),    LALT(KC_X),    LALT(KC_C),    LALT(KC_V),    LALT(KC_B),                      LALT(KC_N),    LALT(KC_M),    LALT(KC_COMM), LALT(KC_DOT),  LALT(KC_SLSH), KC_BSPC,
                           KC_TRNS, KC_TRNS, KC_TRNS,                       KC_MS_BTN1, KC_MS_BTN2, KC_MS_BTN3
    )
};
// Modify these alues to adjust the scrolling speed
#define SCROLL_DIVISOR_H 50.0
#define SCROLL_DIVISOR_V 50.0

// Variables to store accumulated scroll values
float scroll_accumulated_h = 0;
float scroll_accumulated_v = 0;

// Modify these to adjust non-linear mouse scaling
#define MAX_SCALE 2
#define MIN_SCALE 1
#define GROWTH_FACTOR 1.7
#define MOMENTUM 0.01

// Variable to store an exponential moving average scaling factor to denoise the non-linear scaling
float accumulated_factor = MIN_SCALE;

// Arrow keys slight slowing
#define ARROW_STEP 26
#define ARROW_THRESH 2
#define ARROW_RESET_THRESH 1
int arrow_mode = 0;
int arrow_x = 0;
int arrow_y = 0;

// Alt-Tab parameteres
#define ALT_TAB_STEP 46
int accumulated_alt_tab = 0;
int pressed_command = 0;

#define ALT_TAB_LAYER 1
#define SCROLL_LAYER 3
#define ARROW_LAYER 2
// add non-linear scaling to all mouse movements
void ps2_mouse_moved_user(report_mouse_t* mouse_report) {
    // alt-tab operation

    // arrow key emulation
    if (layer_state_is(ARROW_LAYER) || layer_state_is(ALT_TAB_LAYER)) {
        // with the trackpoint, it is nice to have it lock into a single
        // direction of travel until it is released

        if (arrow_mode == 0) {
          if (abs(mouse_report->x) > ARROW_THRESH) {
            arrow_mode = 1;
          }
          if (abs(mouse_report->y) > ARROW_THRESH) {
            arrow_mode = 2;
          }
        }
        if (arrow_mode == 1) {
          if (mouse_report->x > ARROW_RESET_THRESH) {
            arrow_x += mouse_report->x - ARROW_RESET_THRESH;
            if (arrow_x > ARROW_STEP) {
              arrow_x = 0;
              tap_code(KC_RIGHT);
            }
          }
          else if (mouse_report->x < -ARROW_RESET_THRESH) {
            arrow_x += -mouse_report->x - ARROW_RESET_THRESH;
            if (arrow_x > ARROW_STEP) {
              arrow_x = 0;
              tap_code(KC_LEFT);
            }
          }
          else {
            arrow_mode = 0;
            arrow_x = 0;
          }
        }
        if (arrow_mode == 2) {
          if (mouse_report->y > ARROW_RESET_THRESH) {
            arrow_y += mouse_report->y - ARROW_RESET_THRESH;
            if (arrow_y > ARROW_STEP) {
              arrow_y = 0;
              tap_code(KC_DOWN);
            }
          }
          else if (mouse_report->y < -ARROW_RESET_THRESH) {
            arrow_y += -mouse_report->y - ARROW_RESET_THRESH;
            if (arrow_y > ARROW_STEP) {
              arrow_y = 0;
              tap_code(KC_UP);
            }
          }
          else {
            arrow_mode = 0;
            arrow_x = 0;
            arrow_y = 0;
          }
        }

        // return a null report
        mouse_report->x = 0;
        mouse_report->y = 0;
    }

    // compute the size of the last mouse movement
    float mouse_length = sqrtf(mouse_report->x*mouse_report->x + mouse_report->y*mouse_report->y);

    // compute an instantaneous scaling factor and update exponential moving average
    float factor =  GROWTH_FACTOR*mouse_length+ MIN_SCALE;
    accumulated_factor = accumulated_factor*(1-MOMENTUM) + factor*MOMENTUM;

    if (accumulated_factor > MAX_SCALE) {
        // clamp the scaling factor to avoid overflowing mouse_report
        mouse_report->x *= MAX_SCALE;
        mouse_report->y *= MAX_SCALE;
    }
    else {
        // scale up the mouse movement by the average factor
        mouse_report->x = (int16_t)(mouse_report->x * accumulated_factor);
        mouse_report->y = (int16_t)(mouse_report->y * accumulated_factor);
    }

    // switch to scrolling on every layer but 2 (where my mousekeys live)
    if (layer_state_is(SCROLL_LAYER)) {
        // Calculate and accumulate scroll values based on mouse movement and divisors
        scroll_accumulated_h += (float)mouse_report->x / SCROLL_DIVISOR_H;
        scroll_accumulated_v += (float)mouse_report->y / SCROLL_DIVISOR_V;

        // Assign integer parts of accumulated scroll values to the mouse report
        mouse_report->h = (int16_t)scroll_accumulated_h;
        mouse_report->v = -(int16_t)scroll_accumulated_v;

        // Update accumulated scroll values by subtracting the integer parts
        scroll_accumulated_h -= (int16_t)scroll_accumulated_h;
        scroll_accumulated_v -= (int16_t)scroll_accumulated_v;

        // Clear the X and Y values of the mouse report
        mouse_report->x = 0;
        mouse_report->y = 0;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
  if ((!layer_state_is(ARROW_LAYER)) && (arrow_mode != 0)) {
    // if you are not on layer 1, and the arrow mode is not zero, make it zero.
    arrow_mode = 0;
    arrow_x = 0;
    arrow_y = 0;
  }
  return state;
}

void keyboard_post_init_user(void) {
    pointing_device_set_cpi_on_side(true, 4000); //Set cpi on left side to a low value for slower scrolling.

    // pointing_device_set_cpi_on_side(false, 8000); //Set cpi on right side to a reasonable value for mousing.
}

report_mouse_t pointing_device_task_combined_user(report_mouse_t left_report, report_mouse_t right_report) {
	#define ARROW_LEFT_STEP 16
	#define ARROW_LEFT_THRESH 1
	#define ARROW_LEFT_RESET_THRESH 0.1
	int arrow_mode = 0;
	int arrow_x = 0;
	int arrow_y = 0;


    if (layer_state_is(ALT_TAB_LAYER)) {
        if (arrow_mode == 0) {
            if (abs(left_report.x) > ARROW_LEFT_THRESH) {
                arrow_mode = 1;
            }
            if (abs(left_report.y) > ARROW_LEFT_THRESH) {
                arrow_mode = 2;
            }
        }
        if (arrow_mode == 1) {
            if (left_report.x > ARROW_LEFT_RESET_THRESH) {
            arrow_x += left_report.x - ARROW_LEFT_RESET_THRESH;
            if (arrow_x > ARROW_LEFT_STEP) {
                    arrow_x = 0;
                    tap_code(KC_RIGHT);
                }
            }
            else if (left_report.x < -ARROW_LEFT_RESET_THRESH) {
                arrow_x += -left_report.x - ARROW_LEFT_RESET_THRESH;
            if (arrow_x > ARROW_LEFT_STEP) {
                    arrow_x = 0;
                    tap_code(KC_LEFT);
                }
            }
            else {
                arrow_mode = 0;
                arrow_x = 0;
            }
        }
        if (arrow_mode == 2) {
            if (left_report.y > ARROW_LEFT_RESET_THRESH) {
            arrow_y += left_report.y - ARROW_LEFT_RESET_THRESH;
            if (arrow_y > ARROW_LEFT_STEP) {
                    arrow_y = 0;
                    tap_code(KC_DOWN);
                }
            }
            else if (left_report.y < -ARROW_LEFT_RESET_THRESH) {
                arrow_y += -left_report.y -ARROW_LEFT_RESET_THRESH;
            if (arrow_y > ARROW_LEFT_STEP) {
                    arrow_y = 0;
                    tap_code(KC_UP);
                }
            }
            else {
                arrow_mode = 0;
                arrow_x = 0;
                arrow_y = 0;
            }
        }
        // return a null report
        left_report.x = 0;
        left_report.y = 0;
    }
    else {
    	left_report.h = -left_report.x;
     	left_report.v = -left_report.y;
        left_report.x = 0;
        left_report.y = 0;
    }
    return pointing_device_combine_reports(left_report, right_report);
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }

  return true;
}

void matrix_scan_user(void) {
  achordion_task();
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
  	case LT(1, KC_ESC):
    case CMD_T(KC_J):
    case LT(1,KC_Z):
      return 200;
    case LT(3, KC_SPACE):
    case LT(3, KC_TAB):
    case LT(1,KC_ENT):
    case LT(2,KC_TAB):
    case LT(2,KC_BSPC):
    case LT(2,KC_SPACE):
    case LT(2,KC_QUOT):
    case OSM(MOD_LSFT):
    case OSM(MOD_RSFT):
      return 0;  // Bypass Achordion for these keys.
  }

  return 450;  // Otherwise use a timeout of 800 ms.
}
