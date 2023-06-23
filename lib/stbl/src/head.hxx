#pragma once

/* headers */

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

using v1u_t = unsigned;

using v1s_t = signed;

using index_t = v1s_t;
using count_t = v1u_t;

using sbyte_t = signed char;
using ubyte_t = unsigned char;
using mbyte_t = char;
using sdata_t = sbyte_t*;
using udata_t = ubyte_t*;
using mdata_t = void*;

_NAMESPACE_LEAVE
