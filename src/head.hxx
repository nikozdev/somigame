#pragma once

/* headers */

#include <cstdio>
#include <iostream>

#include <algorithm>
#include <functional>

#include <array>
#include <vector>
#include <initializer_list>

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

#define _LINE_NUM __LINE__

#define _NUM_MAX( l, r ) ( l > r ? l : r )
#define _NUM_MIN( l, r ) ( l < r ? l : r )

/** strings **/

#define _FILE_STR __FILE__

#define _ENDS_SYM '\0'
#define _ENDS_STR "\0"

#define _ENDL_SYM '\n'
#define _ENDL_STR "\n"

#define _BELL_SYM '\a'
#define _BELL_STR "\a"

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

using void_t = void;

using bool_t = bool;
using flag_t = bool_t;

using schar_t = signed char;
using uchar_t = unsigned char;
using cchar_t = char;

template<typename t_num_t>
struct v2n_t { t_num_t x = 0, y = 0; };
template<typename t_num_t>
struct v3n_t { t_num_t x = 0, y = 0, z = 0; };

using v1bit_t = bool_t;
using v1b_t = v1bit_t;

using v1u08_t = uint8_t;
using v1u16_t = uint16_t;
using v1u32_t = uint32_t;
using v1u64_t = uint64_t;
using v1u_t = unsigned;
using v2u_t = v2n_t<v1u_t>;
using v3u_t = v3n_t<v1u_t>;

using v1s08_t = int8_t;
using v1s16_t = int16_t;
using v1s32_t = int32_t;
using v1s64_t = int64_t;
using v1s_t = signed;
using v2s_t = v2n_t<v1s_t>;
using v3s_t = v3n_t<v1s_t>;

using v1f32_t = float;
using v1f64_t = float;
using v1f_t = float;
using v2f_t = v2n_t<v1f_t>;
using v3f_t = v3n_t<v1f_t>;

using index_t = v1s_t;
using count_t = v1u_t;

using sbyte_t = signed char;
using ubyte_t = unsigned char;
using mbyte_t = char;
using sdata_t = sbyte_t*;
using udata_t = ubyte_t*;
using mdata_t = void*;
using msize_t = size_t;

template <typename t_val_t>
using t_array_t = std::vector<t_val_t>;
template <typename t_val_t>
using t_list2_t = std::list<t_val_t>;
template <typename t_val_t>
using t_ilist_t = std::initializer_list<t_val_t>;

template <typename t_call_t>
using t_func_t = std::function<t_call_t>;

/** consdef **/

constexpr v1s_t UNIT_SCALE_X = 0x010;
constexpr v1s_t UNIT_SCALE_Y = 0x010;

const size_t CSTRING_MSIZE = 0x100;

constexpr v1s_t RELPOS_DIV = 128;
constexpr v1s_t RELPOS_MID = 0x0;
constexpr v1s_t RELPOS_MIN = -64;
constexpr v1s_t RELPOS_MAX = +64;

constexpr v1s_t SCALE_DIV = 64;
constexpr v1s_t SCALE_MID = 0x0;
constexpr v1s_t SCALE_MIN = -64;
constexpr v1s_t SCALE_MAX = +64;

_NAMESPACE_LEAVE
