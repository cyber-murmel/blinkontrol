/**
 * @file color.h
 * @author marble (contact@computer-in.love)
 * @brief Interface definition of the color module
 * @date 2024-02-18
 *
 */

#ifndef _COLOR_H
#define _COLOR_H

#include <stdint.h>
#include <stdlib.h>

#include "utils.h"

typedef struct {
    uint8_t r, g, b;
} color_t;

/**
 * @brief Number of hue steps
 *
 */
#define HUE_STEPS (256 * 6)

/**
 * @brief Number of steps in a color channel
 *
 * @details Each channel is defined by an 8-bit number
 */
#define INTENSITY_STEPS (1 << 8)

/**
 * @brief Turn hue, value and saturation to an RGB color
 *
 * @param hue [0 .. HUE_STEPS-1 ]
 * @param saturation [0 .. 255]
 * @param value [0 .. 255]
 * @return color_t
 */
color_t hsb2rgb(uint16_t hue, uint8_t saturation, uint8_t brightness);

#endif // _COLOR_H
