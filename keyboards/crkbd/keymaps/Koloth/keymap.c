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
#include QMK_KEYBOARD_H

#define BW_TAP_TIME 200  //configure max tap time, 200ms here

enum custom_keycodes {
  BW_BSPC_L1 = SAFE_RANGE,
  BW_TAB_L2,
  BW_BSPC_L3,
  BW_TAB_L3,
  BW_DELETE_CMD,
  BW_ENTER_CTL,
  BW_ESC_ALT
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t bspc_l1_timer;
  static uint16_t tab_l2_timer;
  static uint16_t bspc_l3_timer;
  static uint16_t tab_l3_timer;
  static uint16_t delete_cmd_timer;
  static uint16_t enter_ctl_timer;
  static uint16_t esc_alt_timer;
  switch (keycode) {
    // combined backspace and layer 1 key
    case BW_BSPC_L1:
      if (record->event.pressed) {
        bspc_l1_timer = timer_read();
        layer_on(1); 
      } else {
        layer_off(1); 
        if (timer_elapsed(bspc_l1_timer) < BW_TAP_TIME)
          tap_code(KC_BSPC);
      }
      return false;
    // combined tab and layer 2 key
    case BW_TAB_L2:
      if (record->event.pressed) {
        tab_l2_timer = timer_read();
        layer_on(2); 
      } else {
        layer_off(2);
        if (timer_elapsed(tab_l2_timer) < BW_TAP_TIME)
          tap_code(KC_TAB);
      }
      return false;
      // combined backspace and layer 3 key
    case BW_BSPC_L3:
      if (record->event.pressed) {
        bspc_l3_timer = timer_read();
        layer_on(3); 
      } else {
        layer_off(3); 
        if (timer_elapsed(bspc_l3_timer) < BW_TAP_TIME)
          tap_code(KC_BSPC);
      }
      return false;
    // combined tab and layer 3 key
    case BW_TAB_L3:
      if (record->event.pressed) {
        tab_l3_timer = timer_read();
        layer_on(3); 
      } else {
        layer_off(3);
        if (timer_elapsed(tab_l3_timer) < BW_TAP_TIME)
          tap_code(KC_TAB);
      }
      return false;
    // combined delete and command key
    case BW_DELETE_CMD:
      if (record->event.pressed) {
        delete_cmd_timer = timer_read();
        register_code(KC_LGUI); 
      } else {
        unregister_code(KC_LGUI); 
        if (timer_elapsed(delete_cmd_timer) < BW_TAP_TIME)
          tap_code(KC_DEL);
      }
      return false;
    // combined enter and control key
    case BW_ENTER_CTL:
      if (record->event.pressed) {
        enter_ctl_timer = timer_read();
        register_code(KC_LCTL); 
      } else {
        unregister_code(KC_LCTL); 
        if (timer_elapsed(enter_ctl_timer) < BW_TAP_TIME)
          tap_code(KC_ENT);
      }
      return false;
    // combined escape and alt key
    case BW_ESC_ALT:
      if (record->event.pressed) {
        esc_alt_timer = timer_read();
        register_code(KC_RALT); 
      } else {
        unregister_code(KC_RALT); 
        if (timer_elapsed(esc_alt_timer) < BW_TAP_TIME)
          tap_code(KC_ESC);
      }
      return false; 
  }
  return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_split_3x6_3(
        KC_GRV,  KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS, 
        CW_TOGG, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, 
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, 
        BW_DELETE_CMD, BW_BSPC_L1, BW_ENTER_CTL, KC_SPC, BW_TAB_L2, BW_ESC_ALT
        ),
	[1] = LAYOUT_split_3x6_3(
        KC_NO, KC_EXLM, KC_AT, KC_LBRC, KC_RBRC, KC_AMPR, KC_PSLS, KC_P7, KC_P8, KC_P9, KC_PMNS, KC_NO, 
        KC_NO, KC_HASH, KC_DLR, KC_LPRN, KC_RPRN, KC_PEQL, KC_ASTR, KC_P4, KC_P5, KC_P6, KC_PLUS, KC_NO, 
        KC_LSFT, KC_PERC, KC_CIRC, KC_LCBR, KC_RCBR, KC_UNDS, KC_P0, KC_P1, KC_P2, KC_P3, KC_PDOT, KC_RSFT, 
        BW_DELETE_CMD, KC_TRNS, BW_ENTER_CTL, KC_SPC, BW_TAB_L3, BW_ESC_ALT
        ),
	[2] = LAYOUT_split_3x6_3(
        KC_NO, KC_PGUP, KC_HOME, KC_UP, KC_END, KC_NO, LCA(KC_D), LCA(KC_LEFT), LCA(KC_ENT), LCA(KC_T), LCA(KC_RGHT), KC_NO, 
        KC_NO, KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT,  KC_NO, KC_BTN1, KC_BTN2, KC_BTN3, RCS(KC_TAB), RCTL(KC_TAB), RGUI(KC_BSLS), 
        KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, LCAG(KC_SPC), LALT(KC_GRV), LGUI(KC_GRV), SGUI(KC_4), LCAG(KC_SLSH), KC_RSFT, 
        BW_DELETE_CMD, BW_BSPC_L3, BW_ENTER_CTL, KC_SPC, KC_TRNS, BW_ESC_ALT
        ),
	[3] = LAYOUT_split_3x6_3(
        KC_NO, KC_NO, KC_BRIU, KC_VOLU, KC_MFFD, KC_NO, KC_NO, KC_F7, KC_F8, KC_F9, KC_F12, KC_NO, 
        KC_NO, KC_NO, KC_BRID, KC_VOLD, KC_MPLY, KC_NO, KC_NO, KC_F4, KC_F5, KC_F6, KC_F11, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_MUTE, KC_MRWD, KC_NO, KC_NO, KC_F1, KC_F2, KC_F3, KC_F10, KC_NO, 
        BW_DELETE_CMD, KC_TRNS, BW_ENTER_CTL, KC_SPC, KC_TRNS, BW_ESC_ALT
        )};

// Modify these alues to adjust the scrolling speed
#define SCROLL_DIVISOR_H 8.0
#define SCROLL_DIVISOR_V 8.0

// Variables to store accumulated scroll values
float scroll_accumulated_h = 0;
float scroll_accumulated_v = 0;

// Modify these to adjust non-linear mouse scaling
#define MAX_SCALE 32
#define MIN_SCALE 1
#define GROWTH_FACTOR 64
#define MOMENTUM 0.01

// Variable to store an exponential moving average scaling factor to denoise the non-linear scaling
float accumulated_factor = MIN_SCALE;

// Arrow keys slight slowing
#define ARROW_STEP 2
int accumulated_arrow_x = 0;
int accumulated_arrow_y = 0;

float average_arrow_x = 0;
float average_arrow_y = 0;

#define ARROW_MOMENTUM 0.99

// Alt-Tab parameteres
#define ALT_TAB_STEP 4
int accumulated_alt_tab = 0;
int pressed_command = 0;

// add non-linear scaling to all mouse movements
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    // alt-tab operation
    if (layer_state_is(3)) {
        // send alt-tab when on this layer. On first mouse movement, press
        // command.  Accumulate and sent tab or shift tab.  Release command
        // when you leave this layer.

        if ((mouse_report.x != 0) && (pressed_command == 0)) {
          pressed_command = 1;
          register_code(KC_LGUI);
        }
        accumulated_alt_tab += mouse_report.x;
    
        // process queued clicks
        if (accumulated_alt_tab <= -ALT_TAB_STEP){
            tap_code16(LSFT(KC_TAB));
            accumulated_alt_tab += ALT_TAB_STEP;
        }
        if (accumulated_alt_tab >= ALT_TAB_STEP) {
            tap_code(KC_TAB);
            accumulated_alt_tab -= ALT_TAB_STEP;
        }
        
        // return a null report
        mouse_report.x = 0;
        mouse_report.y = 0;
        return mouse_report;
    }
    if (pressed_command == 1) {
      // if you are not on layer 3, and command is pressed, release it.
      pressed_command = 0;
      unregister_code(KC_LGUI);
    }

    // arrow key emulation
    if (layer_state_is(1)) {
        // move one space per click here, biasing towards vertical
        // if you are moving in text horizontally and click an accidental
        // vertical, you can just click back.  If you are moving vertically
        // and send an accidental horizontal, you can't just click back
        // if you are in a line shorter than where you started.  Thus,
        // you should strongly prefer vertical movement to bias against
        // accidental sideways clicks.  Almost all text scrolling is
        // orthogonal, so this helps suppress diagonal motion.  Think of it
        // as if you are in a box, and you move when you hit an edge, 
        // resetting to the center each time.  

        // This version additionally keeps a running average and only allows
        // motion in the direction of the recent average 

        // update the accumulated arrow momentum
        average_arrow_x = average_arrow_x*ARROW_MOMENTUM + (float) mouse_report.x*(1-ARROW_MOMENTUM);
        average_arrow_y = average_arrow_y*ARROW_MOMENTUM + (float) mouse_report.y*(1-ARROW_MOMENTUM);

        // kill accumulated clicks orthogonal to average direction           
        if (fabs(average_arrow_x) > fabs(average_arrow_y)){
            accumulated_arrow_x += mouse_report.x;
            accumulated_arrow_y = 0;
        }
        if (fabs(average_arrow_y) > fabs(average_arrow_x)){
            accumulated_arrow_x = 0;
            accumulated_arrow_y += mouse_report.y;
        }
    
        // process queued clicks
        if (accumulated_arrow_x <= -ARROW_STEP){
            tap_code(KC_LEFT);
            accumulated_arrow_x += ARROW_STEP;
        }
        if (accumulated_arrow_x >= ARROW_STEP) {
            tap_code(KC_RIGHT);
            accumulated_arrow_x -= ARROW_STEP;
        }
        if (accumulated_arrow_y <= -ARROW_STEP){
            tap_code(KC_UP);
            accumulated_arrow_y += ARROW_STEP;
        }
        if (accumulated_arrow_y >= ARROW_STEP) {
            tap_code(KC_DOWN);
            accumulated_arrow_y -= ARROW_STEP;
        }
        
        // return a null report
        mouse_report.x = 0;
        mouse_report.y = 0;
        return mouse_report;
    }

    // compute the size of the last mouse movement
    float mouse_length = sqrt(mouse_report.x*mouse_report.x + mouse_report.y*mouse_report.y);

    // compute an instantaneous scaling factor and update exponential moving average
    float factor =  GROWTH_FACTOR*mouse_length+ MIN_SCALE;
    accumulated_factor = accumulated_factor*(1-MOMENTUM) + factor*MOMENTUM;

    if (accumulated_factor > MAX_SCALE) {
        // clamp the scaling factor to avoid overflowing mouse_report
        mouse_report.x *= MAX_SCALE;
        mouse_report.y *= MAX_SCALE;
    }
    else {
        // scale up the mouse movement by the average factor
        mouse_report.x = (int16_t)(mouse_report.x * accumulated_factor);
        mouse_report.y = (int16_t)(mouse_report.y * accumulated_factor);
    }

    // switch to scrolling on every layer but 2 (where my mousekeys live)
    if (!layer_state_is(2)) {
        // Calculate and accumulate scroll values based on mouse movement and divisors
        scroll_accumulated_h += (float)mouse_report.x / SCROLL_DIVISOR_H;
        scroll_accumulated_v += (float)mouse_report.y / SCROLL_DIVISOR_V;

        // Assign integer parts of accumulated scroll values to the mouse report
        mouse_report.h = -(int16_t)scroll_accumulated_h;
        mouse_report.v = (int16_t)scroll_accumulated_v;

        // Update accumulated scroll values by subtracting the integer parts
        scroll_accumulated_h -= (int16_t)scroll_accumulated_h;
        scroll_accumulated_v -= (int16_t)scroll_accumulated_v;

        // Clear the X and Y values of the mouse report
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case 0:
        pimoroni_trackball_set_rgbw(0,0,0,255);
        break;
    case 1:
        pimoroni_trackball_set_rgbw(192,0,64,0);
        break;
    case 2:
        pimoroni_trackball_set_rgbw(0,192,128,0);
        break;
    case 3:
        pimoroni_trackball_set_rgbw(153,113,0,0);
        break;
    }
  return state;
}

void keyboard_post_init_user(void) {
    // default trackball to white
	pimoroni_trackball_set_rgbw(0,0,0,255);
}

void suspend_power_down_user(void) {
	pimoroni_trackball_set_rgbw(0,0,0,0);
}

void suspend_wakeup_init_user(void) {
    pimoroni_trackball_set_rgbw(0,0,0,255);
}
