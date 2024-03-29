/**
 * @file utils.h
 * @author marble (contact@computer-in.love)
 * @brief Utilities
 * @date 2024-02-18
 *
 */

#ifndef _UTILS_H
#define _UTILS_H

#include <math.h>

/**
 * @brief Return the lower number
 *
 */
#define MIN(A, B) ((A < B) ? A : B)

/**
 * @brief Return the higher number
 *
 */
#define MAX(A, B) ((A > B) ? A : B)

/**
 * @brief Limit the value X to the range [A .. B]
 *
 */
#define CLAMP(A, X, B) (MIN(MAX(A, X), B))

/**
 * @brief Map the value VAL from the range [INL .. INH] to the range [OUTL .. OUTH]
 *
 */
#define MAP(VAL, INL, INH, OUTL, OUTH) (OUTL + (((OUTH - OUTL) * (VAL - INL)) / (INH - INL)))

/**
 * @brief Calculate floored division module of A for the divisor N
 *
 */
#define FLOOR_DIV_MOD(A, N) (A - (N * floor(A / N)))

#endif // _UTILS_H
