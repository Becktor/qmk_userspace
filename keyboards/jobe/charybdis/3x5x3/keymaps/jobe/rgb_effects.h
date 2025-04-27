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

// Save the default RGB matrix mode
static uint8_t saved_rgb_matrix_mode = RGB_MATRIX_SOLID_REACTIVE_SIMPLE; // Default fallback
static hsv_t saved_rgb_matrix_hsv = {HSV_BLUE}; // Properly initialize with braces
static bool mode_saved = false;

// Charybdis 3x5x3 LED layout
// The split keyboard has LEDs arranged in a matrix where:
// Left side LEDs typically have indices 0 to (RGBLED_NUM/2-1)
// Right side LEDs typically have indices (RGBLED_NUM/2) to (RGBLED_NUM-1)

#ifdef RGB_MATRIX_ENABLE
// Forward declarations for functions
void save_rgb_matrix_mode(void);
void restore_rgb_matrix_mode(void);
void update_rgb_for_layer(layer_state_t state);

// Function to save the current RGB matrix mode
void save_rgb_matrix_mode(void) {
    if (!mode_saved) {
        saved_rgb_matrix_mode = rgb_matrix_get_mode();
        saved_rgb_matrix_hsv = rgb_matrix_get_hsv();
        mode_saved = true;
    }
}

// Function to restore the saved RGB matrix mode
void restore_rgb_matrix_mode(void) {
    if (mode_saved) {
        rgb_matrix_mode_noeeprom(saved_rgb_matrix_mode);
        rgb_matrix_sethsv_noeeprom(saved_rgb_matrix_hsv.h, saved_rgb_matrix_hsv.s, saved_rgb_matrix_hsv.v);
        mode_saved = false;
    }
}

// Function to handle RGB color changes for homerow mods
void update_rgb_for_homerow_mods(uint16_t keycode, smtd_action action) {
    // Change RGB color based on homerow mod activation
    if (action == SMTD_ACTION_HOLD) {
        save_rgb_matrix_mode(); // Save current mode before changing
        homerow_mod_active = true; // Set the flag
        
        switch (keycode) {
            // Left-hand home row mods
            case HRM_A:  // GUI - Pink
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LEFT);
                rgb_matrix_sethsv_noeeprom(HSV_PINK);
                break;
            case HRM_S:  // ALT - Green
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LEFT);
                rgb_matrix_sethsv_noeeprom(HSV_GREEN);
                break;
            case HRM_D:  // CTRL - Blue
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LEFT);
                rgb_matrix_sethsv_noeeprom(HSV_BLUE);
                break;
            case HRM_F:  // SHIFT - Yellow
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LEFT);
                rgb_matrix_sethsv_noeeprom(HSV_YELLOW);
                break;
            
            // Right-hand home row mods
            case HRM_J:  // SHIFT - Yellow
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_RIGHT);
                rgb_matrix_sethsv_noeeprom(HSV_YELLOW);
                break;
            case HRM_K:  // CTRL - Blue
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_RIGHT);
                rgb_matrix_sethsv_noeeprom(HSV_BLUE);
                break;
            case HRM_L:  // ALT - Green
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_RIGHT);
                rgb_matrix_sethsv_noeeprom(HSV_GREEN);
                break;
            case HRM_QUOT:  // GUI - Pink
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_RIGHT);
                rgb_matrix_sethsv_noeeprom(HSV_PINK);
                break;
        }
    } 
    // Reset flag and resume RGB task when modifier is released
    else if (action == SMTD_ACTION_RELEASE) {
        homerow_mod_active = false; // Clear the flag
        restore_rgb_matrix_mode(); // Restore the saved mode
        update_rgb_for_layer(layer_state); // Update for current layer
    }
}

// Layer RGB color settings function
void update_rgb_for_layer(layer_state_t state) {
    // Don't update layer color if a homerow mod is active
    if (homerow_mod_active) {
        return;
    }
    
    // Base layer restores the saved mode, all others set specific colors
    uint8_t highest_layer = get_highest_layer(state);
    if (highest_layer == LAYER_BASE) {
        restore_rgb_matrix_mode();
    } else {
        save_rgb_matrix_mode();
        
        switch (highest_layer) {
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
                // For any other layers, restore the default mode
                restore_rgb_matrix_mode();
                break;
        }
    }
} 
#endif // RGB_MATRIX_ENABLE 

