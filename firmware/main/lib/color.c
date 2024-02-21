/**
 * @file color.c
 * @author marble (contact@computer-in.love)
 * @brief Implementation of the color module
 * @date 2024-02-19
 *
 */

#include "lib/color.h"

#include "lib/utils.h"

typedef enum {
    CH_GREEN = 0,
    CH_RED = 1,
    CH_BLUE = 2,
} color_channel_t;

/**
 * @brief Calculate the intensity of a color channel for a give hue
 *
 * @details Creates a linear fade, with red at hue = 0, green at hue = 1/3
 *          and blue at hue = 2/3
 *
 * @param hue
 * @param color_channel
 * @return float
 */
static float value(float hue, color_channel_t color_channel)
{
    float value;

    /* This expression creates two linear functions,
     * one intersecting [0, 0] the other [2/3, 0],
     * both starting at [1/3, 2], making x = 1/3 the mirror axis.
     * This is the peak of the green channel.
     * Other color channels are realized with a hue shift.
     */
    hue += ((float)color_channel / 3.0);
    hue = FLOOR_DIV_MOD(hue, 1.0); // wrap around
    value = 2.0 - fabs(6.0 * (hue - 1.0 / 3.0));
    value = CLAMP(0, (value), 1.0);

    return value;
}

static float saturate(float value, float saturation)
{
    return ((value * saturation) + (1.0 - saturation));
}

static float brighten(float value, float brightness)
{
    return (value * brightness);
}

color_t hsb2rgb(float hue, float saturation, float brightness)
{
    color_t color = {
        .r = (INTENSITY_STEPS - 1) * brighten(saturate(value(hue, CH_RED), saturation), brightness),
        .g = (INTENSITY_STEPS - 1) * brighten(saturate(value(hue, CH_GREEN), saturation), brightness),
        .b = (INTENSITY_STEPS - 1) * brighten(saturate(value(hue, CH_BLUE), saturation), brightness),
    };

    return color;
}
