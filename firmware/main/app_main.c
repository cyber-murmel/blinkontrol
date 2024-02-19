/**
 * @file app_main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-18
 * 
 * @copyright Copyright (c) 2024
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
#include "utils.h"
#include "color.h"

#define POT_V_MIN_MV (30)
#define POT_V_MAX_MV (830)

#define ADC_TIMER_PERIOD_US (10000)
#define LED_TIMER_PERIOD_US (100)

#define LED_MAX_FREQ_HZ (10)
#define LED_COUNTER_RES (1000)

/**
 * @brief value to refill the LED timer counter with
 * 
 * calculated so that at the highest count down speed (LED_COUNTER_RES) the led blinks with LED_MAX_FREQ_HZ
 */
#define LED_COUNTER_REFILL ((1000*1000*LED_COUNTER_RES)/LED_TIMER_PERIOD_US/LED_MAX_FREQ_HZ/2)

static const char *TAG = "main";

static int32_t led_timer_counter = 0;
static volatile uint32_t led_timer_counter_speed = 0;

static void adc_timer_callback(void* arg);
static void led_timer_callback(void* arg);

void app_main(void)
{

    esp_timer_handle_t adc_timer;
    esp_timer_handle_t led_timer;

    const esp_timer_create_args_t adc_timer_args = {
        .callback = &adc_timer_callback,
        .name = "adc_timer"
    };
    const esp_timer_create_args_t led_timer_args = {
        .callback = &led_timer_callback,
        .name = "led_timer"
    };

    ESP_LOGI(TAG, "started");

    led_init();
    adc_init();

    ESP_ERROR_CHECK(esp_timer_create(&adc_timer_args, &adc_timer));
    ESP_ERROR_CHECK(esp_timer_create(&led_timer_args, &led_timer));

    ESP_ERROR_CHECK(esp_timer_start_periodic(adc_timer, ADC_TIMER_PERIOD_US));
    ESP_ERROR_CHECK(esp_timer_start_periodic(led_timer, LED_TIMER_PERIOD_US));


    while (1) {
        // delay to yield the main task
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void adc_timer_callback(void* arg)
{
    uint32_t adc_mv = adc_read_mv();
    int32_t hue; 

    adc_mv = CLAMP(POT_V_MIN_MV, adc_mv, POT_V_MAX_MV);
    led_timer_counter_speed = MAP(adc_mv, POT_V_MIN_MV, POT_V_MAX_MV, 0, LED_COUNTER_RES);
    hue = MAP(adc_mv, POT_V_MIN_MV, POT_V_MAX_MV, 0, 256*6);

    led_color_set(HUE2RED(hue)>>4, HUE2GREEN(hue)>>4, HUE2BLUE(hue)>>4);

    if (0 == led_timer_counter_speed) {
        led_power_set(LED_POWER_ON);
    }
}

static void led_timer_callback(void* arg)
{
    led_timer_counter -= led_timer_counter_speed;

    if (led_timer_counter <= 0) {
        led_toggle();
        led_timer_counter += LED_COUNTER_REFILL;
    }
}
