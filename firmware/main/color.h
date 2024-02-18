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

#include "utils.h"

typedef struct
{
    uint8_t r, g, b;
} color_t;


#define HUE2GREEN(H) (CLAMP(0, (256*2 - abs(((H) % (256*6)) - 256*2)), 255))
#define HUE2BLUE(H) HUE2GREEN(H+256*4)
#define HUE2RED(H) HUE2GREEN(H+256*2)


#endif // _COLOR_H
