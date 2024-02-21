/**
 * @file app_main.c
 * @author marble (contact@computer-in.love)
 * @brief
 * @date 2024-02-18
 *
 */

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "sdkconfig.h"

#include "hal/adc.h"
#include "hal/led.h"
#include "lib/color.h"
#include "lib/utils.h"

#define LED_TIMER_PERIOD_US (20 * 1000)

#define LED_MAX_FREQ_HZ (10)
#define LED_COUNTER_RES (1000)

#define LED_SATURATION (255)
#define LED_BRIGHTNESS (1.0 / 16)

/**
 * @brief value to refill the LED timer counter with
 *
 * calculated so that at the highest count down speed (LED_COUNTER_RES) the led blinks with
 * LED_MAX_FREQ_HZ
 */
#define LED_COUNTER_REFILL ((1000 * 1000 * LED_COUNTER_RES) / LED_TIMER_PERIOD_US / LED_MAX_FREQ_HZ / 2)

static const char* TAG = "main";

static int32_t led_timer_counter = 0;
static volatile uint32_t led_timer_counter_speed = 0;
static volatile color_t led_color;

static void led_timer_callback(void* arg);

/**
 * @brief Application main task
 *
 */
void app_main(void)
{
    // esp_timer_handle_t adc_timer;
    esp_timer_handle_t led_timer;
    float adv_value;
    float hue;

    const esp_timer_create_args_t led_timer_args = { .callback = &led_timer_callback, .name = "led_timer" };

    ESP_LOGI(TAG, "started");

    led_init();
    adc_init();
    adc_start();

    // ESP_ERROR_CHECK(esp_timer_create(&adc_timer_args, &adc_timer));
    ESP_ERROR_CHECK(esp_timer_create(&led_timer_args, &led_timer));

    // ESP_ERROR_CHECK(esp_timer_start_periodic(adc_timer, ADC_TIMER_PERIOD_US));
    ESP_ERROR_CHECK(esp_timer_start_periodic(led_timer, LED_TIMER_PERIOD_US));

    while (1) {
        adv_value = adc_get_value();
        led_timer_counter_speed = MAP(adv_value, 0.0, 1.0, 0, LED_COUNTER_RES);
        // hue = MAP(adv_value, 0.0, 1.0, 0, (((5 * HUE_STEPS) / 6) - 1));
        hue = (adv_value * 5) / 6;
        led_color = hsb2rgb(hue, 1.0, LED_BRIGHTNESS);

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief LED timer task
 *
 * @param arg unused
 */
static void led_timer_callback(void* arg)
{
    (void)arg; // unused

    led_timer_counter -= led_timer_counter_speed;

    led_color_set(led_color);

    if (0 == led_timer_counter_speed) {
        led_power_set(LED_POWER_ON);
    } else if (led_timer_counter <= 0) {
        led_toggle();
        led_timer_counter += LED_COUNTER_REFILL;
    }
}
