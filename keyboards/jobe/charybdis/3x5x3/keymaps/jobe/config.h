/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
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

#ifdef VIA_ENABLE
/* VIA configuration. */
#    define DYNAMIC_KEYMAP_LAYER_COUNT 7
#endif // VIA_ENABLE

#ifndef __arm__
/* Disable unused features. */
#    define NO_ACTION_ONESHOT
#endif // __arm__

/* Charybdis-specific features. */
#define MAX_DEFERRED_EXECUTORS 10

#define CHARYBDIS_DRAGSCROLL_REVERSE_X

#ifdef POINTING_DEVICE_ENABLE
// Automatically enable the pointer layer when moving the trackball.  See also:
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS`
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD`
// #define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#endif // POINTING_DEVICE_ENABLE

// Add this line for RGB layer indication
#define RGB_MATRIX_LAYER_INDICATORS

// Make sure RGB Matrix is fully enabled and configured
#ifdef RGB_MATRIX_ENABLE
    // Enable the RGB Matrix effects
    #define RGB_MATRIX_FRAMEBUFFER_EFFECTS
    #define RGB_MATRIX_KEYPRESSES
    
    // Allow RGB Matrix to be used with layers
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 150
    #define RGB_MATRIX_HUE_STEP 8
    #define RGB_MATRIX_SAT_STEP 8
    #define RGB_MATRIX_VAL_STEP 8
    #define RGB_MATRIX_SPD_STEP 10
    
    // Enable solid color effect (needed for layer indicators)
    #define RGB_MATRIX_SOLID_COLOR_ENABLE
#endif

// SM Tap Dance configuration
#ifndef MAX_DEFERRED_EXECUTORS
#define MAX_DEFERRED_EXECUTORS 10
#endif

// Add a delay to avoid immediate activation when pressing multiple keys
#define SMTD_GLOBAL_SIMULTANEOUS_PRESSES_DELAY_MS 100

// Customize SM Tap Dance timing (optional)
#define SMTD_GLOBAL_TAP_TERM 200
#define SMTD_GLOBAL_SEQUENCE_TERM 100
#define SMTD_GLOBAL_FOLLOWING_TAP_TERM 200
#define SMTD_GLOBAL_RELEASE_TERM 50

// Enable mods recall and tap aggregation
#define SMTD_GLOBAL_MODS_RECALL true
#define SMTD_GLOBAL_AGGREGATE_TAPS false
