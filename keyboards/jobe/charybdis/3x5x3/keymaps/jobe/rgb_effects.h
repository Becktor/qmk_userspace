/* Copyright 2024 Jobe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include QMK_KEYBOARD_H
#include "sm_td.h"

// Flag to indicate if a homerow modifier is currently active
// Remove static and declare as extern, definition will be in keymap.c
extern bool homerow_mod_active;

// Forward declaration of layer state functions
#ifndef LAYER_STATE_H_
extern layer_state_t layer_state;
layer_state_t layer_state_set_user(layer_state_t state);
#endif

// Charybdis 3x5x3 LED layout
// The split keyboard has LEDs arranged in a matrix where:
// Left side LEDs typically have indices 0 to (RGBLED_NUM/2-1)
// Right side LEDs typically have indices (RGBLED_NUM/2) to (RGBLED_NUM-1)

#ifdef RGB_MATRIX_ENABLE
// Function to set color for only the left side of the keyboard
void rgb_matrix_set_left_side(uint8_t hue, uint8_t sat, uint8_t val) {
    // Force RGB matrix into solid color mode
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    
    // Convert HSV to RGB
    HSV hsv = {hue, sat, val};
    RGB rgb = hsv_to_rgb(hsv);
    
    // Set only left side LEDs (0-17)
    for (uint8_t i = 0; i <= 17; i++) {
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    
    // Force update of the LED buffers
    rgb_matrix_update_pwm_buffers();
}

// Function to set color for only the right side of the keyboard
void rgb_matrix_set_right_side(uint8_t hue, uint8_t sat, uint8_t val) {
    // Force RGB matrix into solid color mode
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    
    // Convert HSV to RGB
    HSV hsv = {hue, sat, val};
    RGB rgb = hsv_to_rgb(hsv);
    
    // Set only right side LEDs (18-35)
    for (uint8_t i = 18; i < RGB_MATRIX_LED_COUNT; i++) {
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    
    // Force update of the LED buffers
    rgb_matrix_update_pwm_buffers();
}

// Function to handle RGB color changes for homerow mods
void update_rgb_for_homerow_mods(uint16_t keycode, smtd_action action) {
    // Change RGB color based on homerow mod activation
    if (action == SMTD_ACTION_HOLD) {
        homerow_mod_active = true; // Set the flag
        rgb_matrix_set_suspend_state(true); // Suspend main RGB task
        
        // Use the actual modifier keycode for the switch
        uint16_t mod_keycode = 0;
        // Map HRM_ keycode back to the modifier it represents
        switch (keycode) {
            case HRM_A: mod_keycode = KC_LGUI; break;
            case HRM_S: mod_keycode = KC_LALT; break;
            case HRM_D: mod_keycode = KC_LCTL; break;
            case HRM_F: mod_keycode = KC_LSFT; break;
            case HRM_J: mod_keycode = KC_RSFT; break;
            case HRM_K: mod_keycode = KC_RCTL; break;
            case HRM_L: mod_keycode = KC_LALT; break; // Note: Keymap uses LALT for HRM_L
            case HRM_QUOT: mod_keycode = KC_RGUI; break;
        }

        if (mod_keycode != 0) { // Only proceed if a valid mod was found
            switch (mod_keycode) {
                // Left-hand mods - only light up left side
                case KC_LGUI: // Pink
                    rgb_matrix_set_left_side(HSV_PINK);
                    break;
                case KC_LALT: // Green (Used by S and L)
                    if (keycode == HRM_S) { // Check original keycode for S
                        rgb_matrix_set_left_side(HSV_GREEN);
                    } else { // Must be L
                         rgb_matrix_set_right_side(HSV_GREEN);
                    }
                    break;
                case KC_LCTL: // Blue
                    rgb_matrix_set_left_side(HSV_BLUE);
                    break;
                case KC_LSFT: // Yellow
                    rgb_matrix_set_left_side(HSV_YELLOW);
                    break;

                // Right-hand mods - only light up right side
                case KC_RSFT: // Yellow
                    rgb_matrix_set_right_side(HSV_YELLOW);
                    break;
                case KC_RCTL: // Blue
                    rgb_matrix_set_right_side(HSV_BLUE);
                    break;
                // KC_LALT handled above for HRM_L
                case KC_RGUI: // Pink
                    rgb_matrix_set_right_side(HSV_PINK);
                    break;
            }
        }
    } 
    // Reset flag and resume RGB task when modifier is released
    else if (action == SMTD_ACTION_RELEASE) {
        homerow_mod_active = false; // Clear the flag
        rgb_matrix_set_suspend_state(false); // Resume main RGB task
        // Layer color should update naturally.
    }
}

// Layer RGB color settings function
void update_rgb_for_layer(layer_state_t state) {
    // Don't update layer color if a homerow mod is active
    if (homerow_mod_active) {
        return;
    }
    
    switch (get_highest_layer(state)) {
        case LAYER_BASE:
            // Set color for base layer (e.g., white)
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            //rgb_matrix_sethsv_noeeprom(HSV_WHITE);
            rgb_matrix_sethsv_noeeprom(0, 0, 0); // Off
            break;
        case LAYER_FUNCTION:
            // Set color for function layer (e.g., blue)
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(HSV_BLUE);
            break;
        case LAYER_NAVIGATION:
            // Set color for navigation layer (e.g., green)
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
            break;
        case LAYER_MEDIA:
            // Set color for media layer (e.g., yellow)
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(HSV_YELLOW);
            break;
        case LAYER_POINTER:
            // Set color for pointer layer (e.g., cyan)
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(HSV_CYAN);
            break;
        case LAYER_NUMERAL:
            // Set color for numeral layer (e.g., orange)
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(HSV_ORANGE);
            break;
        case LAYER_SYMBOLS:
            // Set color for symbols layer (e.g., magenta)
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(HSV_MAGENTA);
            break;
        default:
            // Optional: Set a default color or turn off LEDs for other layers
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(0, 0, 0); // Off
            break;
    }
} 
#endif // RGB_MATRIX_ENABLE 