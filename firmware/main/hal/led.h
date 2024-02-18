/**
 * @file led.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _LED_H
#define _LED_H

#include <stdint.h>

typedef enum {
    LED_POWER_OFF = 0,
    LED_POWER_ON = 1,
} led_power_t;

void led_init(void);
void led_power_set(led_power_t led_power);
void led_color_set(uint8_t r, uint8_t g, uint8_t b);
void led_toggle();

#endif // _LED_H
