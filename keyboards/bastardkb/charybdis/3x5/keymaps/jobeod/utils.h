#pragma once

#include QMK_KEYBOARD_H // Needed for types in function signature

// Custom keycodes for DPI control
enum custom_keycodes {
    DPI_INC = SAFE_RANGE,
    DPI_DEC,
};

// Declaration for the DPI adjustment function
bool process_record_user(uint16_t keycode, keyrecord_t *record); 