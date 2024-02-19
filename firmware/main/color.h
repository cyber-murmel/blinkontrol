/**
 * @file color.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _COLOR_H
#define _COLOR_H

#include <stdint.h>
#include <stdlib.h>

#include "utils.h"

typedef struct
{
    uint8_t r, g, b;
} color_t;

#define HUE_STEPS (256*6)
#define INTENSITY_STEPS (256)
#define HUE_SLOPE (((6*INTENSITY_STEPS)/HUE_STEPS))

/**
 * @brief Calculates intensity of the green channel for a given hue value
 *
 * Implements a trapezoidal curve like so
 */
#define HUE2GREEN(HUE) (CLAMP(0, ((2*INTENSITY_STEPS) - abs(((HUE_SLOPE*HUE) % (HUE_STEPS)) - (HUE_STEPS/3))), (INTENSITY_STEPS-1)))

/**
 * @brief Calculates intensity of the red channel for a given hue value
 */
#define HUE2RED(HUE) HUE2GREEN(HUE+(HUE_STEPS/3))

/**
 * @brief Calculates intensity of the blue channel for a given hue value
 */
#define HUE2BLUE(HUE) HUE2GREEN(HUE+((2*HUE_STEPS)/3))

/**
 * @brief Turn hue, value and saturation to an RGB color
 * 
 * @param hue [0 .. HUE_STEPS-1 ]
 * @param saturation [0 .. 255]
 * @param value [0 .. 255]
 * @return color_t 
 */
static inline color_t hsv2rgb(uint16_t hue, uint8_t saturation, uint8_t value) {
    color_t color = {
        .r = (value * ((saturation*HUE2RED(hue))/255 + (255-saturation)))/255,
        .g = (value * ((saturation*HUE2GREEN(hue))/255 + (255-saturation)))/255,
        .b = (value * ((saturation*HUE2BLUE(hue))/255 + (255-saturation)))/255,
    };

    return color;
}


#endif // _COLOR_H
