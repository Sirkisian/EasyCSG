#pragma once

#include <array>

#define ARRAY2REF(X, Y) std::array<X, 2> & Y
#define ARRAY3REF(X, Y) std::array<X, 3> & Y
#define ARRAY4REF(X, Y) std::array<X, 4> & Y

#define _IN_(parameter) const parameter
#define _OUT_(parameter) parameter
#define _INOUT_(parameter) parameter
//-----
#define GLUE(x, y) x y

#define _ZERO1(A1) A1 = 0
#define _ZERO2(A1, A2) _ZERO1(A1); A2 = 0
#define _ZERO3(A1, A2, A3) _ZERO2(A1, A2); A3 = 0
#define _ZERO4(A1, A2, A3, A4) _ZERO3(A1, A2, A3); A4 = 0
#define _ZERO5(A1, A2, A3, A4, A5) _ZERO4(A1, A2, A3, A4); A5 = 0

#define RETURN_ARG_COUNT(_1, _2, _3, _4, _5, count, ...) count
#define EXPAND_ARGS(args) RETURN_ARG_COUNT args
#define COUNT_ARGS_MAX5(...) EXPAND_ARGS((__VA_ARGS__, 5, 4, 3, 2, 1, 0))

#define MACRO2(count) _ZERO##count
#define MACRO1(count) MACRO2(count)
#define MACRO(count) MACRO1(count)

#define _ZERO_OUT_ARGS(...) GLUE(MACRO(COUNT_ARGS_MAX5(__VA_ARGS__)), (__VA_ARGS__))
//-----
enum mCOORDINATE {X, Y, Z};
enum mSIZE {WIDTH, HEIGHT};
enum mCOLOR {R, G, B, A};