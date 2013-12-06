#ifndef _MATH_FUNCS_H_
#define _MATH_FUNCS_H_

#include "global-defines.h"
#include "math.h"

inline uint _round(double x)
{
	double tmp = (double) ( (uint) x );
	return (x-tmp >= 0.5) ? ((uint) x) : (((uint) x) + 1);
}

inline bool isNotNull(double x)
{
	if (fabs(x) < DOUBLE_NULL) return false;
		return true;
}

#endif
