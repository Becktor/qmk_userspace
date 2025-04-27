#include "utils.h" // Include custom definitions and declarations

// Handle custom keycodes for DPI adjustment
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef POINTING_DEVICE_ENABLE
    if (record->event.pressed) { // Only process on key press
        uint16_t current_cpi = pointing_device_get_cpi();
        uint16_t new_cpi = current_cpi;
        uint16_t cpi_step = 100; // How much to change DPI by each step
        // Define sensor limits (adjust if needed for your specific sensor)
        uint16_t min_cpi = 100;
        uint16_t max_cpi = 12000;

        switch (keycode) {
            case DPI_INC:
                new_cpi = current_cpi + cpi_step;
                if (new_cpi > max_cpi) {
                    new_cpi = max_cpi; // Clamp to max
                }
                if (new_cpi != current_cpi) {
                    pointing_device_set_cpi(new_cpi);
                    // Optional: print CPI to console for debugging
                    // printf("CPI set to: %u\n", new_cpi);
                }
                return false; // Skip default processing

            case DPI_DEC:
                if (current_cpi > min_cpi + cpi_step) {
                     new_cpi = current_cpi - cpi_step;
                } else {
                     new_cpi = min_cpi; // Clamp to min
                }
                if (new_cpi != current_cpi) {
                    pointing_device_set_cpi(new_cpi);
                    // Optional: print CPI to console for debugging
                    // printf("CPI set to: %u\n", new_cpi);
                }
                return false; // Skip default processing
        }
    }
#endif // POINTING_DEVICE_ENABLE
    return true; // Process other keycodes normally
} 