/**
 * @file adc.h
 * @author marble (contact@computer-in.love)
 * @brief Definition of the ADC interface
 * @date 2024-02-18
 *
 */

#ifndef _ADC_H
#define _ADC_H

#include <stdint.h>
#include "esp_adc/adc_continuous.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Initialize the ADC
 *
 */
void adc_init(void);
void adc_start(void);
uint32_t adc_get_data(void);
float adc_get_value(void);

#endif // _ADC_H
