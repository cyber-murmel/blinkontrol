/**
 * @file adc.h
 * @author marble (contact@computer-in.love)
 * @brief Definition of the ADC interface
 * @date 2024-02-18
 *
 */

#ifndef _ADC_H
#define _ADC_H

#include "esp_adc/adc_continuous.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdint.h>

/**
 * @brief Initialize the ADC
 *
 */
void adc_init(void);
void adc_start(void);
float adc_get_value(void);

#endif // _ADC_H
