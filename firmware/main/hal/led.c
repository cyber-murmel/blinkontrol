/**
 * @file led.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "led.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"


/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink */
#define BLINK_GPIO CONFIG_BLINK_GPIO

static const char *TAG = "led";

static led_power_t s_led_power = LED_POWER_OFF;
static struct {
    uint8_t r, g, b;
} s_led_color = {
    .r = 16,
    .g = 16,
    .b = 16,
};

#ifdef CONFIG_BLINK_LED_RMT

static led_strip_handle_t led_strip;

void led_init(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1, // at least one LED on board
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

void led_power_set(led_power_t led_power) {
    s_led_power = led_power;
    switch (s_led_power) {
        case LED_POWER_ON: {
            led_strip_set_pixel(led_strip, 0,
                s_led_color.r,
                s_led_color.g,
                s_led_color.b);
            led_strip_refresh(led_strip);
        } break;
        case LED_POWER_OFF: // deliberate fallthrough
        default: {
            led_strip_clear(led_strip);
        } break;
    }
}

void led_color_set(uint8_t r, uint8_t g, uint8_t b) {
    s_led_color.r = r;
    s_led_color.g = g;
    s_led_color.b = b;
    
    switch (s_led_power) {
        case LED_POWER_ON: {
            led_strip_set_pixel(led_strip, 0,
                s_led_color.r,
                s_led_color.g,
                s_led_color.b);
            led_strip_refresh(led_strip);
        } break;
        case LED_POWER_OFF: // deliberate fallthrough
        default: {
            led_strip_clear(led_strip);
        } break;
    }
}


#elif CONFIG_BLINK_LED_GPIO

void led_init(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

void led_power_set(led_power_t led_power) {
    s_led_power = led_power;
    switch (s_led_power) {
        case LED_POWER_ON: {
            gpio_set_level(BLINK_GPIO, 1);
        } break;
        case LED_POWER_OFF: // deliberate fallthrough
        default: {
            gpio_set_level(BLINK_GPIO, 0);
        } break;
    }
}

#endif

void led_toggle(void) {
    if (LED_POWER_OFF == s_led_power) {
        led_power_set(LED_POWER_ON);
    }
    else {
        led_power_set(LED_POWER_OFF);
    }
}
