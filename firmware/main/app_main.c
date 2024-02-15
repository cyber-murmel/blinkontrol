#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "sdkconfig.h"

#include "hal/led.h"

static const char *TAG = "main";

void app_main(void)
{
    ESP_LOGI(TAG, "started");

    /* Configure the peripheral according to the LED type */
    led_init();

    while (1) {
        led_toggle();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}