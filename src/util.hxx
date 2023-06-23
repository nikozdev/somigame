#pragma once

/* headers */

#include "head.hxx"

/* defines */

#define _FUNC_NAME __FUNCTION__
#define _FUNC_SIGN __PRETTY_FUNCTION__

#define _OLOG_NAME olog
#define _OLOG_STR _GET_STR( _OLOG_NAME )
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
#define _ELOG_STR _GET_STR( _ELOG_NAME )
#define _EPUT( args... ) fprintf( stderr, ##args );
#define _ELOG( mesg, args... ) ({ \
    _EPUT( \
        "from: " _ELOG_STR " | " \
        "file: " _FILE_STR " | " \
        "line: " "%d" " | " \
        "mesg: " _ENDL_STR \
        "" mesg "" _ENDL_STR \
        , _LINE_NUM, ##args ) \
})

#if (defined SIGINT)
#   define _BREAK() ({ raise(SIGINT); })
#else
#   define _BREAK() ({ _EPUT( _BELL_STR ); system("pause"); })
#endif   /* SIGINT */

#define _ERROR( mesg, code, actn ) \
    ({ _ELOG( mesg ); _BREAK(); ecode = code; actn; })
#define _EFNOT( expr, mesg, actn ) \
    ({ if ( (expr) == _FALSE ) { _ERROR( mesg, 1, actn ); } })
#define _PCALL( exec, mesg, actn ) \
    ({ exec; _EFNOT( ecode == _ZERO, mesg, ({ actn; ecode = _ZERO; }) ); })

/* content */

_NAMESPACE_ENTER

/** predecl **/

extern index_t ecode;

/** typedef **/

/*** enums ***/

typedef enum ename_e {
    _ENAME_META_NONE = 0x0,
    _ENAME_GAME,
    _ENAME_GAME_HERO,
    _ENAME_GAME_PICK,
    _ENAME_GFIX_GRID,
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
    _ENAME_COUNT
} ename_e;

/*** classes ***/

template <typename...args_t>
class t_signal_t
{
public: /* typedef */
    using signal_t = t_signal_t<args_t...>;
    using func_t = std::function<void(args_t...)>;
    class link_t
    {
    public: /* codetor */
        inline link_t(signal_t&signal, index_t index, const func_t&func)
            : signal(&signal), index(index), func(func) {}
        inline link_t(const link_t&copy)
            : signal(copy.signal), index(copy.index), func(copy.func) {}
        inline link_t(link_t&&copy)
            : signal(copy.signal), index(copy.index), func(copy.func) {}
        inline ~link_t() = default;
    public: /* vetters */
        inline bool vet() const { return this->signal; }
    public: /* actions */
        inline bool quit()
        {
            _EFNOT(this->vet(), "cannot quit signal - invalid link", return _FALSE);
            _EFNOT(this->signal->drop(this->index), "signal drop error", return _FALSE);
            return _TRUTH;
        }
    public: /* symbols */
        inline link_t&operator=(link_t&&copy)
        { std::memmove(this, &copy, sizeof(link_t)); return *this; }
        inline link_t&operator=(const link_t&copy)
        { std::memcpy(this, &copy, sizeof(link_t)); return *this; }
    private: /* datadef */
        signal_t*signal;
        index_t index;
        const func_t func;
    private: /* friends */
        friend signal_t;
    };
    using link_list_t = t_array_t<link_t*>;
public: /* codetor */
    t_signal_t() {}
    ~t_signal_t()
    { _EFNOT(this->quit(), "signal-dtor error", return); }
public: /* vetters */
    inline bool_t vet_link(index_t index)
    { return this->link_list.size() > index; }
public: /* actions */
    inline link_t*bind(const func_t&func)
    {
        auto index = this->link_list.size();
        this->link_list.push_back(new link_t(*this, index, func));
        return link_list.back();
    }
    inline bool_t quit()
    {
        while (this->link_list.size() > 0)
        {
            _EFNOT(
                this->link_list.back()->quit(),
                "signal-quit-link error",
                return _FALSE
            );
        }
        return _TRUTH;
    }
    inline bool_t drop(index_t index)
    {
        _EFNOT(vet_link(index), "signal-drop error: no link found", return _FALSE);
        delete this->link_list[index];
        this->link_list.erase(this->link_list.begin() + index);
        for (auto iter = index; iter < this->link_list.size(); iter++)
        { this->link_list[iter]->index = iter; }
        return _TRUTH;
    }
    inline void call(args_t...args)
    {
        for (auto link : this->link_list)
        { link->func(std::forward<args_t&&...>(args)...); }
    }
private:
    link_list_t link_list;
};

typedef struct timer_t {
    size_t dif_mil = 0; /* miliseconds between current and last frames */
    size_t was_mil = 0; /* last frame milisecond */
    size_t now_mil = 0; /* current frame milisecond */
    size_t fps_num = 0; /* frames per second number */
    using signal_t = t_signal_t<>;
    signal_t sig_sec; /* call on every second */
    signal_t sig_upd; /* call on every update */
} timer_t;

/*** struct ***/

typedef struct iname_t {
    const index_t i = 0;
} iname_t;

typedef struct sname_t {
    const msize_t l = CSTRING_MSIZE - 1;
    const mbyte_t s[CSTRING_MSIZE];
} sname_t;

typedef struct ename_t {
    const ename_e e = _ENAME_META_NONE;
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

extern index_t ecode;

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
