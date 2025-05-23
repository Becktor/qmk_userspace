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
#include QMK_KEYBOARD_H
#include "utils.h" // Include custom definitions

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_FUNCTION,
    LAYER_NAVIGATION,
    LAYER_MEDIA,
    LAYER_POINTER,
    LAYER_NUMERAL,
    LAYER_SYMBOLS,
};

// Update the existing custom_keycodes enum to include our SMTD keycodes
enum custom_keycodes {
    SAFE_RANGE_START = SAFE_RANGE,
    SMTD_KEYCODES_BEGIN, // Begin of SM Tap Dance keycodes
    // Left-hand home row mods
    HRM_A,  // GUI
    HRM_S,  // ALT
    HRM_D,  // CTRL
    HRM_F,  // SHIFT
    // Right-hand home row mods
    HRM_J,  // SHIFT
    HRM_K,  // CTRL
    HRM_L,  // ALT
    HRM_QUOT, // GUI
    SMTD_KEYCODES_END,   // End of SM Tap Dance keycodes
};

// Include sm_td.h AFTER the enum with SMTD_KEYCODES_BEGIN and SMTD_KEYCODES_END
#include "sm_td.h"
#include "rgb_effects.h" // Include the RGB effects

// Define the global flag used by rgb_effects.h
bool homerow_mod_active = false;
// Define flag for layer-tap detection
bool layer_tap_active = false;

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Original SMTD processing (keep this active)
    if (!process_smtd(keycode, record)) {
        return false;
    }
    
    // Check for layer-tap keycodes
#ifdef RGB_MATRIX_ENABLE
    // Detect layer-tap keycodes
    // We can't directly match the macros, so we need to check for the LT() expanded values
    switch (keycode) {
        case LT(LAYER_MEDIA, KC_ESC):      // ESC_MED
        case LT(LAYER_NAVIGATION, KC_SPC): // SPC_NAV
        case LT(LAYER_FUNCTION, KC_TAB):   // TAB_FUN
        case LT(LAYER_SYMBOLS, KC_ENT):    // ENT_SYM
        case LT(LAYER_NUMERAL, KC_BSPC):   // BSP_NUM
        case LT(LAYER_NUMERAL, KC_DEL):    // DEL_NUM
            update_rgb_for_layer_tap(keycode, record->event.pressed);
            break;
        default:
            // Check if it's a pointer layer key (wrapped in _L_PTR macro)
            if ((keycode & 0xFF00) == (LT(LAYER_POINTER, 0) & 0xFF00)) {
                update_rgb_for_layer_tap(keycode, record->event.pressed);
            }
            break;
    }
#endif
    
    // Add any other custom keycode handling here if needed
    
    return true;
}

// SM Tap Dance action handler
void on_smtd_action(uint16_t keycode, smtd_action action, uint8_t tap_count) {
    switch (keycode) {
        // Left-hand home row mods
        SMTD_MT(HRM_A, KC_A, KC_LGUI, 1000)  // Regular mod-tap behavior with longer threshold
        SMTD_MT(HRM_S, KC_S, KC_LALT, 1000)  // Regular mod-tap behavior
        SMTD_MT(HRM_D, KC_D, KC_LCTL, 1000)  // Regular mod-tap behavior
        SMTD_MT(HRM_F, KC_F, KC_LSFT, 1000)  // Regular mod-tap behavior
        
        // Right-hand home row mods
        SMTD_MT(HRM_J, KC_J, KC_RSFT, 1000)  // Regular mod-tap behavior
        SMTD_MT(HRM_K, KC_K, KC_RCTL, 1000)  // Regular mod-tap behavior
        SMTD_MT(HRM_L, KC_L, KC_LALT, 1000)  // Regular mod-tap behavior
        SMTD_MT(HRM_QUOT, KC_QUOT, KC_RGUI, 1000)  // Regular mod-tap behavior with longer threshold
    }

// --- RE-ENABLE homerow RGB call --- 
#ifdef RGB_MATRIX_ENABLE
    // Call the RGB handling function for homerow mods
    update_rgb_for_homerow_mods(keycode, action);
#endif
}

// Automatically enable sniping-mode on the pointer layer.
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define ESC_MED LT(LAYER_MEDIA, KC_ESC)
#define SPC_NAV LT(LAYER_NAVIGATION, KC_SPC)
#define TAB_FUN LT(LAYER_FUNCTION, KC_TAB)
#define ENT_SYM LT(LAYER_SYMBOLS, KC_ENT)
#define BSP_NUM LT(LAYER_NUMERAL, KC_BSPC)
#define DEL_NUM LT(LAYER_NUMERAL, KC_DEL)
#define _L_PTR(KC) LT(LAYER_POINTER, KC)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// clang-format off
/** \brief QWERTY layout (3 rows, 10 columns). */
#define LAYOUT_LAYER_BASE                                                                      \
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, \
    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L, KC_QUOT, \
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT, KC_SLSH, \
                      BSP_NUM, SPC_NAV, TAB_FUN,     ESC_MED, DEL_NUM, ENT_SYM 


/** Convenience row shorthands. */
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX
#define ______________HOME_ROW_GACS_R______________ XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI

/*
 * Layers used on the Charybdis Nano.
 *
 * These layers started off heavily inspired by the Miryoku layout, but trimmed
 * down and tailored for a stock experience that is meant to be fundation for
 * further personalization.
 *
 * See https://github.com/manna-harbour/miryoku for the original layout.
 */

/**
 * \brief Function layer.
 *
 * Secondary right-hand layer has function keys mirroring the numerals on the
 * primary layer with extras on the pinkie column, plus system keys on the inner
 * column. App is on the tertiary thumb key and other thumb keys are duplicated
 * from the base layer to enable auto-repeat.
 */
#define LAYOUT_LAYER_FUNCTION                                                                    \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_PSCR, KC_F7,   KC_F8,   KC_F9,  KC_F12,  \
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,     KC_SCRL, KC_F4,   KC_F5,   KC_F6,  KC_F11,  \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_PAUS, KC_F1,   KC_F2,   KC_F3,  KC_F10,  \
                      KC_SPC, KC_BSPC, XXXXXXX,      KC_ENT, KC_DEL, KC_MPLY

/**
 * \brief Media layer.
 *
 * Tertiary left- and right-hand layer is media and RGB control.  This layer is
 * symmetrical to accomodate the left- and right-hand trackball.
 */
#define LAYOUT_LAYER_MEDIA                                                                        \
    XXXXXXX,RGB_RMOD, RGB_TOG, RGB_MOD, XXXXXXX,     XXXXXXX,RGB_RMOD, RGB_TOG, RGB_MOD, XXXXXXX, \
    KC_MPRV, KC_VOLD, KC_MUTE, KC_VOLU, KC_MNXT,     KC_MPRV, KC_VOLD, KC_MUTE, KC_VOLU, KC_MNXT, \
    XXXXXXX, XXXXXXX, XXXXXXX,  EE_CLR, QK_BOOT,     QK_BOOT,  EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, \
             KC_MPLY, KC_MSTP, KC_MSTP,      KC_MSTP, KC_MPLY, KC_MPLY

/** \brief Mouse emulation and pointer functions. */
#define LAYOUT_LAYER_POINTER                                                                      \
    QK_BOOT,  EE_CLR, XXXXXXX, DPI_MOD, S_D_MOD,     S_D_MOD, DPI_MOD, XXXXXXX,  EE_CLR, QK_BOOT, \
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,     XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, \
    XXXXXXX, DRGSCRL, SNIPING, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, XXXXXXX, \
             KC_BTN2, KC_BTN1, KC_BTN3,      KC_BTN3, KC_BTN1, XXXXXXX

/**
 * \brief Navigation layer.
 *
 * Primary right-hand layer (left home thumb) is navigation and editing. Cursor
 * keys are on the home position, line and page movement below, clipboard above,
 * caps lock and insert on the inner column. Thumb keys are duplicated from the
 * base layer to avoid having to layer change mid edit and to enable auto-repeat.
 */
#define LAYOUT_LAYER_NAVIGATION                                                                     \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   \
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,      KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_CAPS,  \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      KC_HOME, KC_PGDN, KC_PGUP,  KC_END, KC_INS,   \
                      XXXXXXX, KC_BSPC, KC_TAB,      KC_ENT, KC_DEL, XXXXXXX

/**
 * \brief Numeral layout.
 *
 * Primary left-hand layer (right home thumb) is numerals and symbols. Numerals
 * are in the standard numpad locations with symbols in the remaining positions.
 * `KC_DOT` is duplicated from the base layer.
 */
#define LAYOUT_LAYER_NUMERAL                                                                      \
    KC_LBRC,    KC_7,    KC_8,    KC_9, KC_RBRC,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    KC_SCLN,    KC_4,    KC_5,    KC_6,  KC_EQL,     XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, \
     KC_GRV,    KC_1,    KC_2,    KC_3, KC_BSLS,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                         KC_DOT,    KC_0, KC_MINS,   KC_ENT, XXXXXXX, KC_MPLY

/**
 * \brief Symbols layer.
 *
 * Secondary left-hand layer has shifted symbols in the same locations to reduce
 * chording when using mods with shifted symbols. `KC_LPRN` is duplicated next to
 * `KC_RPRN`.
 */
#define LAYOUT_LAYER_SYMBOLS                                                                      \
    KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    KC_COLN,  KC_DLR, KC_PERC, KC_CIRC, KC_PLUS,     XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, \
    KC_TILD, KC_EXLM,   KC_AT, KC_HASH, KC_PIPE,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                      KC_LPRN, KC_RPRN, KC_UNDS,     XXXXXXX, KC_DEL, KC_MPLY

/**
 * \brief Add Home Row mod to a layout.
 *
 * Expects a 10-key per row layout.  Adds support for GACS (Gui, Alt, Ctl, Shift)
 * home row.  The layout passed in parameter must contain at least 20 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     HOME_ROW_MOD_GACS(LAYER_ALPHAS_QWERTY)
 */
#define _HOME_ROW_MOD_GACS(                                            \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
           HRM_A,       HRM_S,       HRM_D,       HRM_F,         L14,   \
             R15,      HRM_J,       HRM_K,       HRM_L,     HRM_QUOT,  \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)

/**
 * \brief Add pointer layer keys to a layout.
 *
 * Expects a 10-key per row layout.  The layout passed in parameter must contain
 * at least 30 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     POINTER_MOD(LAYER_ALPHAS_QWERTY)
 */
#define _POINTER_MOD(                                                  \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    L20, L21, L22, L23, L24, R25, R26, R27, R28, R29,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
             L10,         L11,         L12,         L13,         L14,  \
             R15,         R16,         R17,         R18,         R19,  \
      _L_PTR(L20),        L21,         L22,         L23,         L24,  \
             R25,         R26,         R27,         R28,  _L_PTR(R29), \
      __VA_ARGS__
#define POINTER_MOD(...) _POINTER_MOD(__VA_ARGS__)

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(
    POINTER_MOD(HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE))
  ),
  [LAYER_FUNCTION] = LAYOUT_wrapper(LAYOUT_LAYER_FUNCTION),
  [LAYER_NAVIGATION] = LAYOUT_wrapper(LAYOUT_LAYER_NAVIGATION),
  [LAYER_MEDIA] = LAYOUT_wrapper(LAYOUT_LAYER_MEDIA),
  [LAYER_NUMERAL] = LAYOUT_wrapper(LAYOUT_LAYER_NUMERAL),
  [LAYER_POINTER] = LAYOUT_wrapper(LAYOUT_LAYER_POINTER),
  [LAYER_SYMBOLS] = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        layer_state_set_user(layer_state);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    // Original auto-sniping logic
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));

    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in
// rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif
