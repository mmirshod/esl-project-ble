#include "esl_gpio.h"

// Configure pins for LEDs and switch
void esl_cfg_pins() {
    nrf_gpio_cfg_output(ESL_IO_PIN_LED1);
    nrf_gpio_cfg_output(ESL_IO_PIN_LED_R);
    nrf_gpio_cfg_output(ESL_IO_PIN_LED_G);
    nrf_gpio_cfg_output(ESL_IO_PIN_LED_B);
    nrf_gpio_cfg_input(ESL_IO_PIN_SW1, NRF_GPIO_PIN_PULLUP);
}

// Turn off all LEDs
void esl_led_off_all() {
    nrf_gpio_pin_write(ESL_IO_PIN_LED1, 1);
    nrf_gpio_pin_write(ESL_IO_PIN_LED_R, 1);
    nrf_gpio_pin_write(ESL_IO_PIN_LED_G, 1);
    nrf_gpio_pin_write(ESL_IO_PIN_LED_B, 1);
}

// Turn on RGB LEDs
void esl_led_on_rgb() {
    nrf_gpio_pin_write(ESL_IO_PIN_LED_R, 0);
    nrf_gpio_pin_write(ESL_IO_PIN_LED_G, 0);
    nrf_gpio_pin_write(ESL_IO_PIN_LED_B, 0);
}

// Turn off RGB LEDs
void esl_led_off_rgb() {
    nrf_gpio_pin_write(ESL_IO_PIN_LED_R, 1);
    nrf_gpio_pin_write(ESL_IO_PIN_LED_G, 1);
    nrf_gpio_pin_write(ESL_IO_PIN_LED_B, 1);
}

// Turn on a specified LED
void esl_led_on(esl_io_pin_t pin) {
    nrf_gpio_pin_write(pin, 0);
}

// Turn off a specified LED
void esl_led_off(esl_io_pin_t pin) {
    nrf_gpio_pin_write(pin, 1);
}

bool esl_btn_is_pressed(void) {
    return !nrf_gpio_pin_read(ESL_IO_PIN_SW1);
}
