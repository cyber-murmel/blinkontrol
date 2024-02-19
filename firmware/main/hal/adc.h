/**
 * @file adc.h
 * @author marble (contact@computer-in.love)
 * @brief Definition of the ADC interface
 * @version 0.1
 * @date 2024-02-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _ADC_H
#define _ADC_H

#include <stdint.h>

/**
 * @brief Initialize the ADC
 *
 */
void adc_init();

/**
 * @brief Read the ADC
 *
 * @return uint32_t Voltage in mV
 */
uint32_t adc_read_mv();

#endif // _ADC_H
