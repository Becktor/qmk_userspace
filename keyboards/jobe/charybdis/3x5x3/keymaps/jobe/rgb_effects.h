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

// Tokyo Night theme HSV color definitions - more deeply saturated and darkened
#define HSV_TOKYO_TEAL     170, 255, 120  // Deeper teal, less whitish
#define HSV_TOKYO_BLUE     215, 255, 120  // Deeper blue, more saturated
#define HSV_TOKYO_PURPLE   280, 255, 120  // Stronger purple
#define HSV_TOKYO_MAGENTA  330, 255, 130  // Rich magenta
#define HSV_TOKYO_PINK     350, 255, 130  // Stronger pink
#define HSV_TOKYO_ORANGE    10, 255, 140  // Deeper orange
#define HSV_TOKYO_YELLOW    60, 255, 130  // Richer yellow
#define HSV_TOKYO_GREEN    120, 255, 100  // More vivid green
#define HSV_TOKYO_SEAFOAM  160, 255, 100  // Deeper seafoam

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
            // Left-hand home row mods - use our custom animation that only affects left side
            case HRM_A:  // GUI - Tokyo Pink
                rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_SOLID_COLOR_LEFT);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_PINK);
                break;
            case HRM_S:  // ALT - Tokyo Green
                rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_SOLID_COLOR_LEFT);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_GREEN);
                break;
            case HRM_D:  // CTRL - Tokyo Blue
                rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_SOLID_COLOR_LEFT);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_BLUE);
                break;
            case HRM_F:  // SHIFT - Tokyo Yellow
                rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_SOLID_COLOR_LEFT);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_YELLOW);
                break;
            
            // Right-hand home row mods - use our custom animation that only affects right side
            case HRM_J:  // SHIFT - Tokyo Yellow
                rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_SOLID_COLOR_RIGHT);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_YELLOW);
                break;
            case HRM_K:  // CTRL - Tokyo Blue
                rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_SOLID_COLOR_RIGHT);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_BLUE);
                break;
            case HRM_L:  // ALT - Tokyo Green
                rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_SOLID_COLOR_RIGHT);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_GREEN);
                break;
            case HRM_QUOT:  // GUI - Tokyo Pink
                rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_SOLID_COLOR_RIGHT);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_PINK);
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
                // Function layer - Tokyo Blue
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_BLUE);
                break;
            case LAYER_NAVIGATION:
                // Navigation layer - Tokyo Teal
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_TEAL);
                break;
            case LAYER_MEDIA:
                // Media layer - Tokyo Yellow
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_YELLOW);
                break;
            case LAYER_POINTER:
                // Pointer layer - Tokyo Seafoam
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_SEAFOAM);
                break;
            case LAYER_NUMERAL:
                // Numeral layer - Tokyo Orange
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_ORANGE);
                break;
            case LAYER_SYMBOLS:
                // Symbols layer - Tokyo Purple
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                rgb_matrix_sethsv_noeeprom(HSV_TOKYO_PURPLE);
                break;
            default:
                // For any other layers, restore the default mode
                restore_rgb_matrix_mode();
                break;
        }
    }
} 
#endif // RGB_MATRIX_ENABLE 

