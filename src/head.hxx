#pragma once

/* headers */

#include <cstdio>
#include <iostream>

#include <vector>

#include <string>
#include <cstring>

#include <cmath>

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

_NAMESPACE_LEAVE
