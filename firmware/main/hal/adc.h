/**
 * @file adc.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _ADC_H
#define _ADC_H

#include <stdint.h>

void adc_init();
uint32_t adc_read_mv();

#endif // _ADC_H
