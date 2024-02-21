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
 * @brief Number of steps in a color channel
 *
 * @details Each channel is defined by an 8-bit number
 */
#define INTENSITY_STEPS (1 << 8)

/**
 * @brief Turn hue, value and saturation to an RGB color
 *
 * @param hue [0.0 .. 1.0]
 * @param saturation [0.0 .. 1.0]
 * @param value [0.0 .. 1.0]
 * @return color_t
 */
color_t hsb2rgb(float hue, float saturation, float brightness);

#endif // _COLOR_H
