#pragma once

/* headers */

#include "head.hxx"

/* defines */
/* content */

_NAMESPACE_ENTER

/** typedef **/

/*** enums ***/

typedef enum ename_e {
    _ENAME_META_NONE = 0x0,
    _ENAME_ENTT_HERO = 0x1,
    _ENAME_GFIX_VIEW,
    _ENAME_GUIS,
    _ENAME_GUIS_MM_QUAD,
    _ENAME_GUIS_LM_QUAD, _ENAME_GUIS_LM_TEXT,
    _ENAME_GUIS_RM_QUAD, _ENAME_GUIS_RM_TEXT,
    _ENAME_GUIS_MB_QUAD, _ENAME_GUIS_MB_TEXT,
    _ENAME_GUIS_MT_QUAD, _ENAME_GUIS_MT_TEXT,
    _ENAME_GUIS_LB_QUAD, _ENAME_GUIS_LB_TEXT, _ENAME_GUIS_LB_LOGO,
    _ENAME_GUIS_RB_QUAD, _ENAME_GUIS_RB_TEXT, _ENAME_GUIS_RB_LOGO,
    _ENAME_GUIS_LT_QUAD, _ENAME_GUIS_LT_TEXT, _ENAME_GUIS_LT_LOGO,
    _ENAME_GUIS_RT_QUAD, _ENAME_GUIS_RT_TEXT, _ENAME_GUIS_RT_LOGO,
} ename_e;

/*** classes ***/

class signal_t
{
public: /* typedef */
    using this_t = signal_t;
    using func_t = std::function<void(void)>;
    using func_list_t = std::vector<func_t*>;
    using link_t = func_t*;
public: /* codetor */
    signal_t();
    ~signal_t();
public: /* setters */
    index_t listen(func_t);
    bool_t forget(index_t);
public: /* actions */
    void call();
private:
    func_list_t func_list;
};

typedef struct timer_t {
    size_t dif_mil = 0; /* miliseconds between current and last frames */
    size_t was_mil = 0; /* last frame milisecond */
    size_t now_mil = 0; /* current frame milisecond */
    size_t fps_num = 0; /* frames per second number */
    signal_t sig_sec; /* call on every second */
    signal_t sig_upd; /* call on every update */
} timer_t;

/*** struct ***/

typedef struct iname_t {
    const index_t value = 0;
} iname_t;

typedef struct sname_t {
    const msize_t msize = CSTRING_MSIZE;
    const mbyte_t value[CSTRING_MSIZE];
} sname_t;

typedef struct ename_t {
    const ename_e value = _ENAME_META_NONE;
} ename_t;

typedef struct cstring_t {
    msize_t msize = CSTRING_MSIZE;
    mbyte_t mdata[CSTRING_MSIZE];
} cstring_t;


typedef struct pos2d_t {
    v1s_t x = 0, y = 0;
} pos2d_t, vec2d_t;

typedef struct pos3d_t {
    v1s_t x = 0, y = 0, z = 0;
} pos3d_t, vec3d_t;
using coord_t = pos3d_t;

typedef struct direc_t {
    v1s_t x = 0, y = 0;
} direc_t;

typedef struct sizes_t {
    v1s_t w = 0, h = 0;
} sizes_t;

typedef struct scale_t {
    v1s_t x = SCALE_MID, y = SCALE_MID;
} scale_t;

typedef struct relpos_t {
    v1s_t x = RELPOS_MID, y = RELPOS_MID;
} relpos_t;

typedef struct anchor_t {
    v1s_t x = RELPOS_MID, y = RELPOS_MID;
} anchor_t;

typedef struct recta_t {
    v1s_t sl = 0, sr = 0, sb = 0, st = 0;
} recta_t;

/** datadef **/

extern timer_t timer;

/** actions **/

extern void util_loop();

template<typename func_t>
void call_on_sec(func_t func, count_t sec = 1)
{
    auto was = timer.was_mil / (sec * 1'000);
    auto now = timer.now_mil / (sec * 1'000);
    auto dif = now - was;
    if (dif > 0) { func(was, now, dif); }
}

template<typename func_t>
void call_on_mil(func_t func, count_t mil = 1)
{
    auto was = timer.was_mil / mil;
    auto now = timer.now_mil / mil;
    auto dif = now - was;
    if (dif > 0) { func(was, now, dif); }
}

/** getters **/

/*** numbers ***/

template <typename t_num_t>
constexpr inline int get_num_sign(t_num_t n) { return n < 0 ? -1 : +1; }

template <typename t_vec_t = direc_t>
constexpr inline t_vec_t get_vec_turn(t_vec_t vec, bool lside = _TRUTH)
{
    const auto rside = !lside;
    return { vec.y * (-1*lside+rside), vec.x * (-1*rside+lside), };
}

/** symbols **/
/*** char ***/
/*** string ***/
inline const char* operator ""_cstr(const char* mdata, size_t msize) { return mdata; }
inline std::string operator ""_dstr(const char* mdata, size_t msize) { return std::string(mdata, msize); }
/*** int ***/
/**** signed int ****/
inline signed int operator ""_si(long double f) { return static_cast<signed int>(f); }
/**** unsigned int ****/
inline unsigned int operator ""_ui(long double f) { return static_cast<unsigned int>(f); }
/*** float ***/
/**** enable ****/
inline int operator ""_fe(long double n) { return (n * _FNUM_SIZE); }
/**** disable ****/
inline int operator ""_fd(long double n) { return (n / _FNUM_SIZE); }
/*** screen ***/
/**** unit ****/
inline int operator ""_su(unsigned long long int u) { return u * 1; }
inline int operator ""_su(long double u) { return u * 1; }
/*** global ***/
/**** unit ****/
inline int operator ""_gu(unsigned long long int u) { return u * 1_su; }
/**** meter ****/
inline int operator ""_gm(unsigned long long int u) { return u * 16_gu; }

_NAMESPACE_LEAVE
