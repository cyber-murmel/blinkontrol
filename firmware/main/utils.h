#ifndef _UTILS_H
#define _UTILS_H

#define MIN(A, B) ((A < B) ? A : B)
#define MAX(A, B) ((A > B) ? A : B)
#define CLAMP(A, X, B) (MIN(MAX(A, X), B))

#define MAP(IN, INL, INH, OUTL, OUTH) (OUTL + (((OUTH-OUTL)*(IN - INL))/(INH-INL)))

#endif // _UTILS_H
