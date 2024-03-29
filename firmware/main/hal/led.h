/**
 * @file led.h
 * @author marble (contact@computer-in.love)
 * @brief Definition of the LED interface
 * @date 2024-02-18
 *
 */

#ifndef _LED_H
#define _LED_H

#include <stdint.h>

#include "lib/color.h"

typedef enum {
    LED_POWER_OFF = 0,
    LED_POWER_ON = 1,
} led_power_t;

/**
 * @brief Initialize the LED
 *
 */
void led_init(void);

/**
 * @brief Set power of the LED
 *
 * @param led_power
 */
void led_power_set(led_power_t led_power);

/**
 * @brief Toggle power of the LED
 *
 */
void led_toggle();

/**
 * @brief Set color of the LED
 *
 * @param color
 */
void led_color_set(color_t color);

#endif // _LED_H
