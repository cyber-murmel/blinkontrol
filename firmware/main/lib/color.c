/**
 * @file color.c
 * @author marble (contact@computer-in.love)
 * @brief Implementation of the color module
 * @date 2024-02-19
 *
 */

#include "lib/color.h"

#include "lib/utils.h"

#define HUE_SLOPE (((6 * INTENSITY_STEPS) / HUE_STEPS))

typedef enum {
    CH_GREEN = 0,
    CH_RED = 1,
    CH_BLUE = 2,
} color_channel_t;

/**
 * @brief Calculate the intensity of a color channel for a give hue
 *
 * @details Creates a linear fade, with red at hue = 0, green at hue = (HUE_STEPS / 3)
 *          and blue at hue = 2*(HUE_STEPS / 3)
 *
 * @param hue
 * @param color_channel
 * @return uint8_t
 */
static uint8_t value(uint16_t hue, color_channel_t color_channel)
{
    int32_t value;

    // split hue range into 6 slices, each with a linear function for a color channel
    static const uint8_t hue_slope = (((6 * INTENSITY_STEPS) / HUE_STEPS));

    // color channels are realized with a hue shift
    hue += (color_channel * HUE_STEPS) / 3;
    hue %= HUE_STEPS;

    // this function creates two linear functions
    // one intersecting [0, 0] the other [2*(HUE_STEPS / 3), 0]
    // both starting at [(HUE_STEPS / 3), 2*INTENSITY_STEPS], making x = (HUE_STEPS / 3) the mirror
    // line this is the peak of the green channel
    value = (2 * INTENSITY_STEPS) - abs((hue_slope * hue) - (HUE_STEPS / 3));

    // limit the value to INTENSITY_STEPS
    value = (CLAMP(0, (value), (INTENSITY_STEPS - 1)));

    return value;
}

static uint8_t saturate(uint8_t value, uint8_t saturation)
{
    value = (value * saturation) / (INTENSITY_STEPS - 1);
    value += (INTENSITY_STEPS - 1) - saturation;
    return value;
}

static uint8_t brighten(uint8_t value, uint8_t brightness) { return (value * brightness) / 255; }

color_t hsb2rgb(uint16_t hue, uint8_t saturation, uint8_t brightness)
{
    color_t color = {
        .r = brighten(saturate(value(hue, CH_RED), saturation), brightness),
        .g = brighten(saturate(value(hue, CH_GREEN), saturation), brightness),
        .b = brighten(saturate(value(hue, CH_BLUE), saturation), brightness),
    };

    return color;
}
