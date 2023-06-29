#pragma once

/* headers */

#include "head.hxx"
#include "memo.hxx"

/* content */

_NAMESPACE_ENTER

/** consdef **/

const msize_t STRBUF_MSIZE = 0x100;

constexpr v1s_t RSIZE_DIV = 0x80; /* normalize dividing */
constexpr v1s_t RSIZE_MID = 0x40; /* half of the parent */
constexpr v1s_t RSIZE_MIN = 0x00; /* no relative effect */
constexpr v1s_t RSIZE_MAX = 0x80; /* entire parent size */

constexpr v1s_t RPOS2_DIV = 128; /* normalize dividing */
constexpr v1s_t RPOS2_MID = 0x0; /* mm of parent */
constexpr v1s_t RPOS2_MIN = -64; /* lb of parent */
constexpr v1s_t RPOS2_MAX = +64; /* rt of parent */

constexpr v1s_t PIVOT_DIV = 128; /* normalize dividing */
constexpr v1s_t PIVOT_MID = 0x0; /* mm of itself */
constexpr v1s_t PIVOT_MIN = -64; /* lb of itself */
constexpr v1s_t PIVOT_MAX = +64; /* rt of itself */

/** enumdef **/

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
} ename_e; /* enumerated entity name */

/** typedef **/

/*** classes ***/

/* template singleton type */
template<typename t_type_t>
class t_single_t
{
public: /* typedef */
	using type_t = t_type_t;
    using this_t = t_single_t<type_t>;
public: /* codetor */
	inline t_single_t(const type_t& copy) = delete;
	inline t_single_t(type_t&& copy) = delete;
	inline t_single_t(const this_t& copy) = delete;
	inline t_single_t(this_t&& copy) = delete;
public: /* getters */
    static inline type_t& get()
    { static type_t single; return single; }
public: /* symbols */
	type_t& operator=(const type_t& copy) = delete;
	type_t& operator=(type_t&& copy) = delete;
	this_t& operator=(const this_t& copy) = delete;
	this_t& operator=(this_t&& copy) = delete;
}; /* t_single_t */

/* template system type */
template<class t_type_t>
class t_system_t : public t_single_t<t_type_t>
{
public: /* typedef */
	using type_t = t_type_t;
    using this_t = t_system_t<type_t>;
public: /* actions */
	inline v1bit_t init() { return _TRUTH; }
	inline v1bit_t quit() { return _TRUTH; }
	inline v1bit_t work() { return _TRUTH; }
}; /* t_system_t */

/* abstract state type */
class a_engine_state_t
{
public: /* typedef */
    using name_t = std::string;
    using this_t = a_engine_state_t;
public: /* codetor */
    virtual ~a_engine_state_t() = default;
public: /* actions */
    virtual inline v1bit_t init() { return _TRUTH; }
    virtual inline v1bit_t quit() { return _TRUTH; }
    virtual inline v1bit_t work() { return _TRUTH; }
}; /* abstract engine state */
/* template engine type */
template<class t_class_t, class t_state_t = a_engine_state_t>
class t_engine_t : public t_single_t<t_class_t>
{
public: /* typedef */
	using work_proc_t = t_delegator_t<bool_t(void_t)>;
	using work_flow_t = std::thread;
	using work_flag_t = v1bit_t;
	using state_t = t_state_t;
	using state_list_t = t_array_t<state_t*>;
    using this_t = t_engine_t<t_class_t>;
public: /* getters */
    inline work_flow_t&get_work_flow()
    { return this->work_flow; }
    inline work_flag_t&get_work_flag()
    { return this->work_flag; }
    inline state_t*get_state(index_t index)
    {
        _EFNOT(this->vet_state(index), return _NULL, "not found!");
        return this->state_list[index];
    }
public: /* setters */
    template<class t_value_t, typename...t_vargs_t>
    this_t&create_state(t_vargs_t&&...vargs)
    {
        this->state_list.push_back(new t_value_t(std::forward<vargs>(vargs)...));
        return *this;
    }
    this_t&delete_state(index_t index)
    {
        _EFNOT(this->vet_state(index), return *this, "index error!");
        delete this->state_list[index];
        this->state_list.erase(this->state_lsit.begin() + index);
        return *this;
    }
    this_t&delete_state()
    {
        return this->state_list.empty()
            ? *this
            : this->delete_state(this->state_list.size() - 1);
    }
public: /* vetters */
    inline v1bit_t vet_work(bool_t work_flag = _TRUTH) const
    { return this->work_flag == work_flag; }
    inline v1bit_t vet_state() const
    { return !this->state_list.empty(); }
    inline v1bit_t vet_state(index_t index) const
    { return this->state_list.size() > index; }
public: /* actions */
    virtual inline v1bit_t init()
    {
        for (auto&iter : this->state_list)
        { _EFNOT(iter->init(), return _FALSE, "state init error!"); }
		return _TRUTH;
	}
    virtual inline v1bit_t quit()
    {
        for (auto&iter : this->state_list)
        { _EFNOT(iter->quit(), return _FALSE, "quit error!"); }
        while (this->vet_state())
        { this->delete_state(); }
		this->state_list.clear();
		return _TRUTH;
	}
    virtual inline v1bit_t work()
    {
        for (auto&iter : this->state_list)
        { _EFNOT(iter->work(), return _FALSE, "work error!"); }
		return _TRUTH;
	}
    inline v1bit_t exec()
    {
        _EFNOT(vet_work(_FALSE), return _FALSE, "already working!");
		this->work_flow = work_flow_t(this->work_proc);
		this->work_flag = _TRUTH;
		return _TRUTH;
	}
    inline v1bit_t stop()
    {
		_EFNOT(vet_work(_TRUTH), return _FALSE, "already quitted!");
		this->work_flag = _FALSE;
		return _TRUTH;
	}
protected: /* datadef */
	work_flow_t work_flow;
	work_flag_t work_flag;
	work_proc_t work_proc = [this]()->v1bit_t {
		_PCALL({
			_EFNOT(init(), return stop(), "init error!");
            while (vet_work())
            { _EFNOT(this->work(), return stop(), "work error!"); }
			_EFNOT(quit(), return stop(), "quit error!");
		}, return _FALSE, "work error!");
		return _TRUTH;
	};
	state_list_t state_list;
}; /* t_engine_t */

template <typename...args_t>
class _t_signal_t
{
public: /* typedef */
    using signal_t = _t_signal_t<args_t...>;
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
            this->signal->drop(this->index);
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
    using relink_t = link_t*;
    using relink_list_t = t_array_t<relink_t>;
public: /* codetor */
    _t_signal_t() {}
    ~_t_signal_t() { this->quit(); }
public: /* actions */
    inline relink_t find(index_t index)
    {
        return this->relink_list.size() > index ?
            this->relink_list[index] : relink_t();
    }
    inline relink_t bind(const func_t&func)
    {
        auto index = this->relink_list.size();
        relink_t relink;
        memo::owner->new_one(relink, *this, index, func);
        this->relink_list.push_back(relink);
        return relink;
    }
    inline bool_t quit()
    {
        while (this->relink_list.size() > 0)
        {
            this->relink_list.back()->quit();
        }
        return _TRUTH;
    }
    inline bool_t drop(index_t index)
    {
        auto liter = this->relink_list.begin() + index;
        memo::owner->del_one(*liter);
        this->relink_list.erase(liter);
        for (auto iter = index; iter < this->relink_list.size(); iter++)
        { this->relink_list[iter]->index = iter; }
        return _TRUTH;
    }
    inline void call(args_t...args)
    {
        for (auto&relink : this->relink_list)
        { relink->func(std::forward<args_t&&...>(args)...); }
    }
private:
    relink_list_t relink_list;
}; /* t_signal_t */

typedef struct update_event_t {
    msize_t mil;
} timer_mil_event_t; /* event sent on every update */
typedef struct newsec_event_t {
    msize_t sec;
} timer_sec_event_t; /* event sent on every second */
typedef struct timer_t {
    msize_t dif_mil = 0; /* miliseconds between current and last frames */
    msize_t was_mil = 0; /* last frame milisecond */
    msize_t now_mil = 0; /* current frame milisecond */
    msize_t fps_num = 0; /* frames per second number */
    dispatcher_t dispatcher;
} timer_t; /* timing values and signals holder */

/*** struct ***/

template<typename t_func_t>
struct t_signal_t
{
public: /* typedef */
    using sigholder_t = t_sigholder_t<t_func_t>;
    using sigbinder_t = t_sigbinder_t<sigholder_t>;
public: /* datadef */
    sigholder_t holder;
    sigbinder_t binder{holder};
}; /* templated signal */

typedef struct strbuf_t {
    msize_t msize = STRBUF_MSIZE;
    mbyte_t mdata[STRBUF_MSIZE];
} strbuf_t; /* string buffer */

/**** names ****/

typedef struct iname_t {
    const index_t i = 0;
} iname_t; /* index name */

typedef struct sname_t {
    const msize_t l = STRBUF_MSIZE - 1;
    const mbyte_t s [STRBUF_MSIZE];
} sname_t; /* string name */

typedef struct ename_t {
    const ename_e e = _ENAME_META_NONE;
} ename_t; /* enumerated entity name */

/**** sizes ****/

typedef struct asize_t {
    v1s_t x = 0, y = 0;
} asize_t; /* absolute size */
typedef struct rsize_t {
    v1s_t x = RSIZE_MIN, y = RSIZE_MIN;
} rsize_t; /* relative size */
typedef struct gsize_t {
    v1s_t x = 0, y = 0;
} gsize_t; /* global size */
typedef struct tsize_t {
    v1s_t x = 0, y = 0;
} tsize_t; /* tilegrid size */
typedef struct isize_t {
    v1s_t x = 0, y = 0;
} isize_t; /* image size */
typedef struct fsize_t {
    v1f_t x = 0, y = 0;
} fsize_t; /* floating point size */

/**** coord ****/

typedef struct apos2_t {
    v1s_t x = 0, y = 0;
} apos2_t; /* absolute position 2d */
typedef struct rpos2_t {
    v1s_t x = RPOS2_MID, y = RPOS2_MID;
} rpos2_t; /* relative position 2d */
typedef struct zpos1_t {
    v1s_t z = 0;
} zpos1_t; /* depth */

typedef struct gpos3_t {
    v1s_t x = 0, y = 0, z = 0;
} gpos3_t; /* global position 3d */
typedef struct tpos3_t {
    v1s_t x = 0, y = 0, z = 0;
} tpos3_t; /* tilegrid position 3d */
typedef msize_t tkey1_t; /* tilegrid key 1d */
typedef struct tkey3_t {
    tkey1_t x = 0, y = 0, z = 0;
} tkey3_t; /* tilegrid key 3d */
typedef struct ipos2_t {
    v1s_t x = 0, y = 0;
} ipos2_t; /* image position 2d */
typedef struct fpos2_t {
    v1f_t x = 0, y = 0;
} fpos2_t; /* floating point position 2d */

typedef struct pivot_t {
    v1s_t x = PIVOT_MID, y = PIVOT_MID;
} pivot_t; /* pivot point - the centre of a quad */

/**** geometry ****/

typedef struct direc_t {
    v1s_t x = 0, y = 1;
} direc_t; /* direction vector */

typedef struct grect_t {
    struct x_t {
        v1s_t l = 0, r = 0;
    } x;
    struct y_t {
        v1s_t b = 0, t = 0;
    } y;
} grect_t; /* global rectangle */
typedef struct trect_t {
    struct x_t {
        v1s_t l = 0, r = 0;
    } x;
    struct y_t {
        v1s_t b = 0, t = 0;
    } y;
} trect_t; /* tilegrid rectangle */
typedef struct irect_t {
    struct x_t {
        v1s_t l = 0, r = 0;
    } x;
    struct y_t {
        v1s_t b = 0, t = 0;
    } y;
} irect_t; /* image rectangle */
typedef struct frect_t {
    struct x_t {
        v1f_t l = 0, r = 0;
    } x;
    struct y_t {
        v1f_t b = 0, t = 0;
    } y;
} frect_t; /* floating point rectangle */

/** datadef **/

extern timer_t timer;

/** actions **/

extern void util_init();
extern void util_quit();

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

extern ent_t get_by_iname(const iname_t&value);
extern ent_t get_by_ename(const ename_t&value);
extern ent_t get_by_sname(const sname_t&value);

/*** numbers ***/

template <typename t_vec_t = direc_t> [[nodiscard]]
constexpr inline t_vec_t get_vec_turn(t_vec_t vec, bool lside = _TRUTH)
{
    const auto rside = !lside;
    return { vec.y * (-1*lside+rside), vec.x * (-1*rside+lside), };
}

_NAMESPACE_LEAVE
