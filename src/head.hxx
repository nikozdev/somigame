#ifndef SOMIGAME_HEAD_HXX
#define SOMIGAME_HEAD_HXX

/* headers */

/** standard **/

#include <cstdlib>

/*** numbers ***/

#include <cmath>
#include <numeric>
#include <cstdint>

/*** strings ***/

#include <string>
#include <cstring>
#include <sstream>

/*** time ***/

#include <ctime>

/*** input-output ***/

#include <cstdio>
#include <iostream>

/*** filesystem ***/

#include <fstream>
#include <experimental/bits/fs_fwd.h>
#include <experimental/bits/fs_path.h>
#include <experimental/bits/fs_dir.h>
#include <experimental/bits/fs_ops.h>

/** memory **/

#include <new>
#include <memory>

/*** program execution ***/

#include <csignal>
#include <thread>

/** general purpose utilities **/

#include <algorithm>

#include <list>
#include <array>
#include <vector>
#include <initializer_list>

/** 3rd party **/

#include <GL/glut.h>

#include "../lib/stbl/src/stbi.hxx"
#include "../lib/entt/src/all.hpp"

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

#define _FUNC_NAME __FUNCTION__
#define _FUNC_SIGN __PRETTY_FUNCTION__

#define _OLOG_NAME olog
#define _OLOG_STR "olog"
#define _OPUT( args... ) fprintf( stdout, ##args );
#define _OLOG( mesg, args... ) ({ \
    _OPUT( \
        "from: " _OLOG_STR " | " \
        "file: " _FILE_STR " | " \
        "line: " "%d" " | " \
        "mesg: " _ENDL_STR \
        "" mesg "" _ENDL_STR \
        , __LINE__, ##args ) \
})

#define _ELOG_NAME elog
#define _ELOG_STR "elog"
#define _EPUT( args... ) fprintf( stderr, ##args );
#define _ELOG( mesg, args... ) { \
    _EPUT( \
        "from: " _ELOG_STR " | " \
        "file: " _FILE_STR " | " \
        "line: " "%d" " | " \
        "mesg: " _ENDL_STR \
        "" mesg "" _ENDL_STR \
        , _LINE_NUM, ##args ) \
}

#ifdef _CONF_WORK
#   ifdef SIGINT
#       define _BREAK() ({ raise(SIGINT); })
#   else
#       define _BREAK() ({ _EPUT( _BELL_STR ); /* system("pause"); */ })
#   endif /* SIGINT */
#else /* CONF_WORK */
#   define _BREAK() ({})
#endif /* CONF_PLAY */

#define _ERROR( code, actn, ... ) \
    ({ _ELOG( __VA_ARGS__ ); _BREAK(); _ecode = code; actn; })
#define _EFNOT( expr, actn, ... ) \
    ({ if ( (expr) == 0 ) { _ERROR( 1, actn, __VA_ARGS__ ); } })
#define _PCALL( exec, actn, ... ) \
    ({ exec; _EFNOT( _ecode == _ZERO, ({ actn; _ecode = _ZERO; }), __VA_ARGS__ ); })

/** meta **/

#define _SIGNALDEC(name, ...) \
    using name##_sigholder_t = t_sigholder_t<__VA_ARGS__>; \
    using name##_sigbinder_t = t_sigbinder_t<name##_sigholder_t>; \
    extern name##_sigbinder_t name##_sigbinder; \
    /* _SIGNALDEC */
#define _SIGNALDEF(name) \
    name##_sigholder_t name##_sigholder; \
    name##_sigbinder_t name##_sigbinder{name##_sigholder};
    /* _SIGNALDEF */

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
using mstep_t = msize_t;

/*** standard ***/

template <typename t_val_t>
using t_array_t = std::vector<t_val_t>;
template <typename t_val_t>
using t_list2_t = std::list<t_val_t>;
template <typename t_val_t>
using t_ilist_t = std::initializer_list<t_val_t>;

/*** entt ***/

using ent_t = entt::entity;
using entity_t = entt::entity;

template<typename t_iface_t>
using t_delegator_t = entt::delegate<t_iface_t>;

template<typename t_iface_t>
using t_sigholder_t = entt::sigh<t_iface_t>;
template<typename t_iface_t>
using t_sigbinder_t = entt::sink<t_iface_t>;

template<typename t_event_t>
using t_evemitter_t = entt::emitter<t_event_t>;

using dispatcher_t = entt::dispatcher;

/*** standard ***/

/** consdef **/

constexpr v1s_t UNIT_ASIZE_X = 0x010;
constexpr v1s_t UNIT_ASIZE_Y = 0x010;

/** datadef **/

static inline size_t _ecode = 0;

/** getters **/

template <typename t_num_t>
[[nodiscard]] constexpr inline
auto get_num_sign(t_num_t n)
{ return n < 0 ? -1 : +1; }

/*** meta ***/

template<typename t_com_t, typename t_mem_t>
[[nodiscard]] constexpr
decltype(auto)get_member(t_com_t&com, t_mem_t t_com_t::*mem)
{ return com.*mem; }
/* i did not know it for a while...
 * nodiscard attribute means that you should use returned value
 * that can also include [[nodiscard("comment-why-string")]] */
template<typename t_com_t, typename t_mem_t, typename...t_rem_t>
[[nodiscard]] constexpr
decltype(auto)get_member(t_com_t&com, t_mem_t t_com_t::*mem, t_rem_t&&... rem)
{ return get_member(com.*mem, std::forward<t_rem_t>(rem)...); }
/* we can keep a tuple of varargs and pass it here */
/*
template<typename t_com_t, typename t_mem_t, typename...t_rem_t>
[[nodiscard]]
decltype(auto)get_member(t_com_t&com, std::tuple<t_rem_t...>&&rem)
{ return get_member(com, std::apply(rem)); }
*/
template<typename t_com_t, typename t_tup_t, size_t...t_index>
[[nodiscard]]
decltype(auto)get_member(t_com_t&com, t_tup_t&tup, std::index_sequence<t_index...>)
{ return get_member(com, std::get<t_index>(tup)...); }
template<typename t_com_t, typename t_tup_t>
[[nodiscard]]
decltype(auto)get_member(t_com_t&com, t_tup_t&tup)
{ return get_member(com, tup, std::make_index_sequence<std::tuple_size<t_tup_t>::value>()); }

/** symbols **/

constexpr inline
const char* operator ""_cstr(const char* mdata, size_t msize)
{ return mdata; }
constexpr inline
std::string operator ""_dstr(const char* mdata, size_t msize)
{ return std::string(mdata, msize); }
constexpr inline
entt::hashed_string operator ""_hstr(const char* mdata, size_t msize)
{ return entt::hashed_string{mdata}; }

_NAMESPACE_LEAVE

#endif /* SOMIGAME_HEAD_HXX */
