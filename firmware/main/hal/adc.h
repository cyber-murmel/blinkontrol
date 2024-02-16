#ifndef _ADC_H
#define _ADC_H

#include <stdint.h>

void adc_init();
uint32_t adc_read_mv();

#endif // _ADC_H
