#pragma once

/* headers */

#include <cstdio>
#include <iostream>

#include <algorithm>
#include <functional>

#include <array>
#include <vector>

#include <string>
#include <cstring>

#include <cmath>
#include <cstdint>

#include "../lib/entt/entt.hpp"

/* defines */

#define _GET_STR(str) #str

#ifndef _NAME
#   define _NAME nameless
#endif /* _NAME */
#ifndef _NAME_STR
#   define _NAME_STR _GET_STR(_NAME)
#endif /* _NAME_STR */

#ifndef _VNUM
#   define _VNUM nonevnum
#endif /* _VNUM */
#ifndef _VNUM_STR
#   define _VNUM_STR _GET_STR(_VNUM)
#endif /* _VNUM_STR */

#define _NAMESPACE _NAME
#define _NAMESPACE_ENTER namespace _NAMESPACE {
#define _NAMESPACE_LEAVE }

/** numbers **/

#define _ZERO 0
#define _UNIT 1

#define _FNUM_SIZE 1'000'000

/** logic **/

#define _FALSE false
#define _TRUTH true

/** memory **/

#define _NULL nullptr

/** debug **/

#define _ERROR_NONE 0

/* content */

_NAMESPACE_ENTER

/** typedef **/

using bool_t = bool;
using flag_t = bool_t;

using schar_t = signed char;
using uchar_t = unsigned char;
using cchar_t = char;

using v1u08_t = uint8_t;
using v1u16_t = uint16_t;
using v1u32_t = uint32_t;
using v1u_t = unsigned;

using v1s08_t = int8_t;
using v1s16_t = int16_t;
using v1s32_t = int32_t;
using v1s_t = signed;

using index_t = v1s_t;
using count_t = v1u_t;

using sbyte_t = signed char;
using ubyte_t = unsigned char;
using mbyte_t = char;
using sdata_t = sbyte_t*;
using udata_t = ubyte_t*;
using mdata_t = mbyte_t*;
using msize_t = size_t;

/** consdef **/

constexpr v1s_t UNIT_SCALE_X = 0x010;
constexpr v1s_t UNIT_SCALE_Y = 0x010;
constexpr v1s_t UNIT_COUNT_X = 0x100;
constexpr v1s_t UNIT_COUNT_Y = 0x100;
constexpr v1s_t UNIT_COUNT_Z = 0x100;

const size_t CSTRING_MSIZE = 0x100;

constexpr v1s_t RELPOS_DIV = 128;
constexpr v1s_t RELPOS_MID = 0x0;
constexpr v1s_t RELPOS_MIN = -64;
constexpr v1s_t RELPOS_MAX = +64;

_NAMESPACE_LEAVE
