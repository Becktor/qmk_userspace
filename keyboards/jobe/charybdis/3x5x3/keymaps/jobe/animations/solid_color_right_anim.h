RGB_MATRIX_EFFECT(SOLID_COLOR_RIGHT)
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool SOLID_COLOR_RIGHT(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    rgb_t rgb = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        // Only set color for the right side (RGBLED_NUM/2 to RGBLED_NUM-1)
        if (i >= RGB_MATRIX_LED_COUNT / 2) {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        } 
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS 