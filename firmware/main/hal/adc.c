/**
 * @file adc.c
 * @author marble (contact@computer-in.love)
 * @brief Implementation of the ADC interface
 * @date 2024-02-16
 *
 */

#include "adc.h"

#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_continuous.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <string.h>

#define COUNT_OF(LIST)                                                                             \
    ((sizeof(LIST) / sizeof(0 [LIST])) / ((size_t)(!(sizeof(LIST) % sizeof(0 [LIST])))))

#define ADC_READ_LEN (256)
#define ADC_MAX_STORE_BUF_SIZE (1024)
#define ADC_SAMPLE_FREQUENCY_HZ (2 * 1000)
#define ADC_TASK_STACK_SIZE (2 * 1024)

#define ADC_UNIT (ADC_UNIT_1)
#define _ADC_UNIT_STR(unit) #unit
#define ADC_UNIT_STR(unit) _ADC_UNIT_STR(unit)
#define ADC_CONV_MODE (ADC_CONV_SINGLE_UNIT_1)
#define ADC_ATTEN (ADC_ATTEN_DB_0)
#define ADC_BIT_WIDTH (SOC_ADC_DIGI_MAX_BITWIDTH)
#define ADC_MAX_VALUE ((1 << SOC_ADC_DIGI_MAX_BITWIDTH) - 1)

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#define ADC_OUTPUT_TYPE ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define ADC_GET_CHANNEL(p_data) ((p_data)->type1.channel)
#define ADC_GET_DATA(p_data) ((p_data)->type1.data)
#else
#define ADC_OUTPUT_TYPE ADC_DIGI_OUTPUT_FORMAT_TYPE2
#define ADC_GET_CHANNEL(p_data) ((p_data)->type2.channel)
#define ADC_GET_DATA(p_data) ((p_data)->type2.data)
#endif

static adc_continuous_handle_t adc_continuous_handle = NULL;
static adc_cali_handle_t adc_cali_handle = NULL;
#if CONFIG_IDF_TARGET_ESP32
static adc_channel_t channel[] = { ADC_CHANNEL_6 };
#else
static adc_channel_t channel[] = { ADC_CHANNEL_2 };
#endif
static TaskHandle_t s_task_handle;
static uint32_t data;

static const char* TAG = "ADC";

static bool IRAM_ATTR s_conv_done_cb(
    adc_continuous_handle_t handle, const adc_continuous_evt_data_t* edata, void* user_data)
{
    BaseType_t mustYield = pdFALSE;
    // Notify that ADC continuous driver has done enough number of conversions
    vTaskNotifyGiveFromISR(s_task_handle, &mustYield);

    return (mustYield == pdTRUE);
}

static void continuous_adc_init(
    adc_channel_t* channel, uint8_t channel_num, adc_continuous_handle_t* out_handle)
{
    adc_continuous_handle_t handle = NULL;

    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = ADC_MAX_STORE_BUF_SIZE,
        .conv_frame_size = ADC_READ_LEN,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));

    adc_continuous_config_t dig_cfg = {
        // .sample_freq_hz = 20 * 1000,
        .sample_freq_hz = ADC_SAMPLE_FREQUENCY_HZ,
        .conv_mode = ADC_CONV_MODE,
        .format = ADC_OUTPUT_TYPE,
        .pattern_num = channel_num,
    };

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = { 0 };
    // dig_cfg.pattern_num = channel_num;
    for (int i = 0; i < channel_num; i++) {
        adc_pattern[i].atten = ADC_ATTEN;
        adc_pattern[i].channel = channel[i] & 0x7;
        adc_pattern[i].unit = ADC_UNIT;
        adc_pattern[i].bit_width = ADC_BIT_WIDTH;

        ESP_LOGI(TAG, "adc_pattern[%d].atten is :%" PRIx8, i, adc_pattern[i].atten);
        ESP_LOGI(TAG, "adc_pattern[%d].channel is :%" PRIx8, i, adc_pattern[i].channel);
        ESP_LOGI(TAG, "adc_pattern[%d].unit is :%" PRIx8, i, adc_pattern[i].unit);
    }
    dig_cfg.adc_pattern = adc_pattern;
    ESP_ERROR_CHECK(adc_continuous_config(handle, &dig_cfg));

    *out_handle = handle;
}

static void adc_task(void* pvParameters)
{
    esp_err_t ret;
    uint32_t ret_num = 0;
    uint8_t result[ADC_READ_LEN] = { 0 };
    memset(result, 0xcc, ADC_READ_LEN);

    (void)pvParameters; // unused

    while (1) {

        /**
         * This is to show you the way to use the ADC continuous mode driver event callback.
         * This `ulTaskNotifyTake` will block when the data processing in the task is fast.
         * However in this example, the data processing (print) is slow, so you barely block here.
         *
         * Without using this event callback (to notify this task), you can still just call
         * `adc_continuous_read()` here in a loop, with/without a certain block timeout.
         */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        char unit[] = ADC_UNIT_STR(ADC_UNIT);

        while (1) {
            ret = adc_continuous_read(adc_continuous_handle, result, ADC_READ_LEN, &ret_num, 0);
            if (ret == ESP_OK) {
                // ESP_LOGI("TASK", "ret is %x, ret_num is %" PRIu32 " bytes", ret, ret_num);
                for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES) {
                    adc_digi_output_data_t* p = (void*)&result[i];
                    uint32_t chan_num = ADC_GET_CHANNEL(p);
                    data = ADC_GET_DATA(p);
                    /* Check the channel number validation, the data is invalid if the channel num
                     * exceed the maximum channel */
                    if (chan_num < SOC_ADC_CHANNEL_NUM(ADC_UNIT)) {
                        // ESP_LOGI(TAG, "Unit: %s, Channel: %" PRIu32 ", Value: %" PRIx32, unit,
                        //     chan_num, data);
                    } else {
                        ESP_LOGW(
                            TAG, "Invalid data [%s_%" PRIu32 "_%" PRIx32 "]", unit, chan_num, data);
                    }
                }
                /**
                 * Because printing is slow, so every time you call `ulTaskNotifyTake`, it will
                 * immediately return. To avoid a task watchdog timeout, add a delay here. When you
                 * replace the way you process the data, usually you don't need this delay (as this
                 * task will block for a while).
                 */
                vTaskDelay(1);
            } else if (ret == ESP_ERR_TIMEOUT) {
                // We try to read `EXAMPLE_READ_LEN` until API returns timeout, which means there's
                // no available data
                break;
            }
        }
    }
}

void adc_init(void)
{
    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = s_conv_done_cb,
    };

    continuous_adc_init(channel, COUNT_OF(channel), &adc_continuous_handle);

    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(adc_continuous_handle, &cbs, NULL));
}

void adc_start(void)
{
    // s_task_handle = task_handle;
    xTaskCreate(adc_task, "ADC task", ADC_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, &s_task_handle);
    configASSERT(s_task_handle);

    ESP_ERROR_CHECK(adc_continuous_start(adc_continuous_handle));
}

uint32_t adc_get_data(void) { return data; }

int adc_get_voltage_mV(void)
{
    int voltage;
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_cali_handle, data, &voltage));
    return voltage;
}

float adc_get_value(void)
{
    float value = ((float)data) / ADC_MAX_VALUE;
    return value;
}
