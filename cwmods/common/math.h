#ifndef COMMON_MATH_H
#define COMMON_MATH_H
#if defined(__clang__) || defined(__GNUC__)
#include_next <math.h>
#elif defined(_MSC_VER)
#include <corecrt_math.h>
#endif

#include "../common/types.h"

// C++'s mod and div operators are stupid
i64 pymod(i64 a, i64 b);
i64 pydiv(i64 a, i64 b);

#endif
