/**
 * @file utils.h
 * @author marble (contact@computer-in.love)
 * @brief Utilities
 * @version 0.1
 * @date 2024-02-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _UTILS_H
#define _UTILS_H

#define MIN(A, B) ((A < B) ? A : B)
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
#define MAP(VAL, INL, INH, OUTL, OUTH) (OUTL + (((OUTH-OUTL)*(VAL - INL))/(INH-INL)))

#endif // _UTILS_H
