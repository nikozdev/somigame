#pragma once

/* headers */

#include "head.hxx"

/* defines */
/* content */

_NAMESPACE_ENTER

/** typedef **/

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

typedef struct util_t {
    timer_t timer;
} util_t;

/** datadef **/

extern util_t util;

/** actions **/

extern void util_loop();

template<typename func_t>
void call_on_sec(func_t func)
{
    auto was_sec = util.timer.was_mil / 1000;
    auto now_sec = util.timer.now_mil / 1000;
    auto dif_sec = was_sec - now_sec;
    if (dif_sec > 0) { func(was_sec, now_sec, dif_sec); }
}

/*** numbers ***/

constexpr inline int get_num_sign(int n) { return n < 0 ? -1 : +1; }

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
