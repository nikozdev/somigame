#pragma once

/* headers */

#include "head.hxx"

#include <new>

/* defines */

#define _NAMESPACE_ENTER_MEMO namespace memo {
#define _NAMESPACE_LEAVE_MEMO }

/* binary numeric alignment
 * let's break it down into all ops
 * > (msize_t) is a typecast to operate on pointers and lesser types
 * > > so let's ignore it
 * > a-1 gives us all bits that we need to ignore
 * > > ~(a-1) gives us the number without all those bits
 * > > > &~(a-1) removes all those unnecessary bits
 * > m+a is required for upper alignment
 * > m+a-1, or a-1 in the 1st case
 * > > done to prevent excessful addition for already aligned numbers
*/
#define _ALIGN(m,a) (((size_t)(m)+((size_t)(a)-1))&~((size_t)(a)-1))

/* content */

_NAMESPACE_ENTER

_NAMESPACE_ENTER_MEMO

/** consdef **/

constexpr msize_t OWNER_MSIZE = 1 << 20; /* 1mb */
constexpr msize_t PAGER_PSIZE = 1 << 10; /* 1kb */

/** typedef **/

struct mlink_t;

/*** allocators ***/

/* abstract memory owner type */
class a_owner_t
{
public: /* typedef */
    using this_t = a_owner_t;
    using owner_t = a_owner_t;
public: /* codetor */
    a_owner_t() = default;
    a_owner_t(const this_t&copy) = delete;
    a_owner_t(this_t&&copy) = delete;
    virtual ~a_owner_t() = default;
public: /* getters */
public: /* setters */
public: /* vetters */
public: /* actions */
    /** abstract give-take **/
    virtual
    v1bit_t give(mdata_t&mdata, msize_t msize, msize_t malig)
    = 0;
    virtual
    v1bit_t take(mdata_t&mdata, msize_t msize, msize_t malig)
    = 0;
    /** template give-take **/
    template<typename t_mdata_t> constexpr
    v1bit_t give(t_mdata_t&mdata, msize_t msize, msize_t malig)
    { return this->give(reinterpret_cast<mdata_t&>(mdata), msize, malig); }
    template<typename t_vdata_t> constexpr
    v1bit_t give(t_vdata_t*&vdata, count_t count)
    { return this->give(vdata, count * sizeof(t_vdata_t), alignof(t_vdata_t)); }
    template<typename t_mdata_t> constexpr
    v1bit_t take(t_mdata_t&mdata, msize_t msize, msize_t malig)
    { return this->take(reinterpret_cast<mdata_t&>(mdata), msize, malig); }
    template<typename t_vdata_t> constexpr
    v1bit_t take(t_vdata_t*&vdata, count_t count)
    { return this->take(vdata, count * sizeof(t_vdata_t), alignof(t_vdata_t)); } 
    /** template new-del **/
    template<typename t_vdata_t, typename...t_vargs_t> constexpr
    v1bit_t new_one(t_vdata_t*&vdata, t_vargs_t&&...vargs)
    {
        this->give<t_vdata_t>(vdata, 1);
		new(vdata)t_vdata_t(std::forward<t_vargs_t>(vargs)...);
		return _TRUTH;
	}
    template<typename t_vdata_t, typename ... t_vargs_t> constexpr
    v1bit_t new_mul(t_vdata_t*&vdata, count_t count, t_vargs_t&&...vargs)
    {
        this->give<t_vdata_t>(vdata, count);
        for (t_vdata_t*iter = vdata,*tail = iter + count; iter < tail; iter++)
        { new(iter)t_vdata_t(std::forward<t_vargs_t>(vargs)...); }
		return _TRUTH;
	}
    template<typename t_vdata_t> constexpr
    v1bit_t del_one(t_vdata_t*&vdata)
    {
		vdata->~t_vdata_t();
        this->take<t_vdata_t>(vdata, 1);
		return _TRUTH;
	}
    template <typename t_vdata_t> constexpr
    v1bit_t del_mul(t_vdata_t*&vdata, count_t count)
    {
        for (t_vdata_t*iter = vdata,*tail = iter + count; iter < tail; iter++)
        { iter->~t_vdata_t(); }
        this->take<t_vdata_t>(vdata, count);
		return _TRUTH;
	}
    /** edit **/
    inline constexpr
    v1bit_t edit(mdata_t&mdata, msize_t s_old, msize_t s_new, msize_t malig)
    {
        mdata_t older = mdata;
        mdata = _NULL;
        if (s_new > 0) { this->give(mdata, s_new, malig); }
        if (s_old > 0)
        {
            if (mdata)
            {
                s_old = _ALIGN(s_old, malig);
                s_new = _ALIGN(s_new, malig);
                malig = _NUM_MIN(s_old, s_new);
                std::memcpy(mdata, older, s_new);
            }
            this->take(older, s_old, malig);
        }
        return _TRUTH;
    }
    template<typename t_mdata_t> constexpr
    v1bit_t edit(t_mdata_t&mdata, msize_t s_old, msize_t s_new, msize_t malig)
    { return this->edit(reinterpret_cast<mdata_t&>(mdata), s_old, s_new, malig); }
    template<typename t_vdata_t> constexpr
    v1bit_t edit(t_vdata_t*&vdata, count_t c_old, count_t c_new)
    { return this->edit(vdata, c_old * sizeof(t_vdata_t), c_new * sizeof(t_vdata_t), alignof(t_vdata_t)); } 
public: /* symbols */
    inline auto operator=(const this_t& copy) = delete;
    inline auto operator=(this_t&& copy) = delete;
public: /* datadef */
    static this_t*global;
}; /* a_owner_t */
using owner_t = a_owner_t;

/* heap allocator */
class heapy_t : public a_owner_t
{
public: /* typedef */
    using this_t = heapy_t;
public: /* codetor */
    heapy_t();
    heapy_t(const this_t&copy) = delete;
    heapy_t(this_t&&copy) = delete;
    ~heapy_t();
public: /* getters */
    inline msize_t get_msize_umax() const { return this->msize_umax; }
    inline msize_t get_msize_used() const { return this->msize_used; }
public: /* vetters */
    inline v1bit_t vet_msize_used() const
    { return this->msize_used > 0; }
    inline v1bit_t vet_msize_used(msize_t msize) const
    { return this->msize_used >= msize; }
    inline v1bit_t vet_msize_umax() const
    { return this->msize_umax > 0; }
    inline v1bit_t vet_msize_umax(msize_t msize) const
    { return this->msize_umax >= msize; }
public: /* actions */
    virtual
    v1bit_t give(mdata_t&mdata, msize_t msize, msize_t malig)
    override;
    virtual
    v1bit_t take(mdata_t&mdata, msize_t msize, msize_t malig)
    override;
    inline
    void elog(void)
    const
    {
        _ELOG("[memo::heapy->elog]=("
            "[msize_used]=(%zu)[msize_umax]=(%zu)"
            "[malig]=(%zu))",
            this->msize_used, this->msize_umax,
            this->malig
        );
    }
public: /* symbols */
    inline operator owner_t&()
    { return static_cast<owner_t&>(*this); }
    inline operator owner_t*()
    { return static_cast<owner_t*>(this); }
protected: /* datadef */
	msize_t msize_used; /* currently used space */
	msize_t msize_umax; /* maximally used space */
    msize_t malig = _ALIGN(sizeof(mdata_t), alignof(mdata_t));
public: /* datadef */
    static this_t*global;
}; /* heapy_t */

/* stack allocator */
class stack_t : public a_owner_t
{
public: /* typedef */
    using this_t = stack_t;
public: /* codetor */
    stack_t(msize_t msize = _ZERO, owner_t*owner = _NULL);
    stack_t(const this_t&copy) = delete;
    stack_t(this_t&&copy) = delete;
    ~stack_t();
public: /* getters */
    inline mdata_t get_mdata(msize_t mstep)
    { return static_cast<udata_t>(this->mdata_head) + mstep; }
    inline mdata_t get_mdata_head() { return this->mdata_head; }
    inline mdata_t get_mdata_tail() { return this->mdata_tail; }
    inline msize_t get_msize_full() const { return this->msize_full; }
    inline msize_t get_msize_umax() const { return this->msize_umax; }
    inline msize_t get_msize_used() const { return this->msize_used; }
    inline msize_t get_msize_free() const
    { return this->msize_full - this->msize_used; }
public: /* vetters */
    inline v1bit_t vet_mdata(mdata_t mdata) const
    { return (mdata >= this->mdata_head) && (mdata < this->mdata_back); }
    inline v1bit_t vet_mdata(mdata_t mdata, msize_t msize) const
    { return mdata == (this->mdata_back - msize); }
    inline v1bit_t vet_msize_used() const
    { return this->msize_used > 0; }
    inline v1bit_t vet_msize_used(msize_t msize) const
    { return this->msize_used >= msize; }
    inline v1bit_t vet_msize_free() const
    { return this->msize_used < this->msize_full; }
    inline v1bit_t vet_msize_free(msize_t msize) const
    { return this->get_msize_free() >= msize; }
    inline v1bit_t vet_msize_umax(msize_t msize) const
    { return this->msize_umax >= msize; }
public: /* actions */
    virtual
    v1bit_t give(mdata_t&mdata, msize_t msize, msize_t malig)
    override;
    virtual
    v1bit_t take(mdata_t&mdata, msize_t msize, msize_t malig)
    override;
    inline
    void elog(void)
    const
    {
        _ELOG("[memo::stack->elog]=("
            "[owner]=(%p)"
            "[mdata_head]=(%p)[mdata_tail]=(%p)[mdata_back]=(%p)"
            "[msize_full]=(%zu)[msize_used]=(%zu)[msize_umax]=(%zu)"
            "[malig]=(%zu))",
            this->owner,
            this->mdata_head, this->mdata_tail, this->mdata_back,
            this->msize_full, this->msize_used, this->msize_umax,
            this->malig
        );
    }
protected: /* datadef */
    owner_t*owner;
    udata_t mdata_head, mdata_tail, mdata_back;
	msize_t msize_full, msize_used, msize_umax;
    msize_t malig;
public: /* datadef */
    static this_t*global;
}; /* stack_t */

/* arena allocator */
class arena_t : public a_owner_t
{
public: /* typedef */
    using this_t = arena_t;
public: /* codetor */
    arena_t(msize_t msize = _ZERO, owner_t*owner = _NULL);
    arena_t(const this_t&copy) = delete;
    arena_t(this_t&&copy) = delete;
    ~arena_t();
public: /* getters */
    inline mdata_t get_mdata(msize_t mstep)
    { return static_cast<udata_t>(this->mdata_head) + mstep; }
    inline mdata_t get_mdata_head() { return this->mdata_head; }
    inline mdata_t get_mdata_tail() { return this->mdata_tail; }
    inline msize_t get_msize_full() const { return this->msize_full; }
    inline msize_t get_msize_umax() const { return this->msize_umax; }
    inline msize_t get_msize_used() const { return this->msize_used; }
    inline msize_t get_msize_free() const
    { return this->msize_full - this->msize_used; }
public: /* vetters */
    inline v1bit_t vet_mdata(mdata_t mdata) const
    { return (mdata >= this->mdata_head) && (mdata < this->mdata_back); }
    inline v1bit_t vet_msize_used() const
    { return this->msize_used > 0; }
    inline v1bit_t vet_msize_used(msize_t msize) const
    { return this->msize_used >= msize; }
    inline v1bit_t vet_msize_free() const
    { return this->msize_used < this->msize_full; }
    inline v1bit_t vet_msize_free(msize_t msize) const
    { return this->get_msize_free() >= msize; }
    inline v1bit_t vet_msize_umax(msize_t msize) const
    { return this->msize_umax >= msize; }
public: /* actions */
    virtual
    v1bit_t give(mdata_t&mdata, msize_t msize, msize_t malig)
    override;
    virtual
    v1bit_t take(mdata_t&mdata, msize_t msize, msize_t malig)
    override;
    inline
    void elog(void)
    const
    {
        _ELOG("[memo::arena->elog]=("
            "[owner]=(%p)"
            "[mdata_head]=(%p)[mdata_tail]=(%p)[mdata_back]=(%p)"
            "[msize_full]=(%zu)[msize_used]=(%zu)[msize_umax]=(%zu)"
            "[mlink]=(%p)"
            "[malig]=(%zu))",
            this->owner,
            this->mdata_head, this->mdata_tail, this->mdata_back,
            this->msize_full, this->msize_used, this->msize_umax,
            this->mlink,
            this->malig
        );
    }
protected: /* datadef */
    owner_t*owner;
    udata_t mdata_head, mdata_tail, mdata_back;
	msize_t msize_full, msize_used, msize_umax;
    mlink_t*mlink;
    msize_t malig;
public: /* datadef */
    static this_t*global;
}; /* arena_t */

/* page based allocator */
class pager_t : public a_owner_t
{
public: /* typedef */
    using this_t = pager_t;
    /*** memory page ***/
    typedef struct mpage_t {
        mbyte_t*hdata = _NULL; /* data head */
        mbyte_t*bdata = _NULL; /* data back */
        mbyte_t*tdata = _NULL; /* data back */
        mpage_t*npage = _NULL; /* next page */
    } mpage_t; /* mpage_t */
public: /* codetor */
    pager_t(msize_t psize = _ZERO, owner_t*owner = _NULL);
    pager_t(const this_t&copy) = delete;
    pager_t(this_t&&copy) = delete;
    ~pager_t();
public: /* getters */
public: /* vetters */
public: /* actions */
    virtual
    v1bit_t give(mdata_t&mdata, msize_t msize, msize_t malig)
    override;
    virtual
    v1bit_t take(mdata_t&mdata, msize_t msize, msize_t malig)
    override;
    inline
    void elog(void)
    const
    {
        _ELOG("[memo::pager->elog]=("
            "[owner]=(%p)"
            "[hpage]=(%p)[bpage]=(%p)[tpage]=(%p)"
            "[malig]=(%zu)"
            ")%c",
            this->hpage, this->bpage, this->tpage,
            this->owner,
            this->malig,
            _ENDL_SYM
        );
    }
protected: /* datadef */
    owner_t*owner;
    mpage_t*hpage,*bpage,*tpage;
    mlink_t*mlink;
    msize_t malig;
public: /* datadef */
    static this_t*global;
}; /* pager_t */

/*** utils ***/

/* memory link type */
struct mlink_t
{
public: /* codetor */
    constexpr inline mlink_t() : nlink(_NULL), msize(_ZERO) { }
public: /* getters */
    inline mdata_t give(msize_t msize)
    {
        mlink_t*plink = _NULL;
        mlink_t*nlink = this;
        while (nlink)
        { /* try the following links */
            if (nlink->has_msize(msize))
            {
                if (plink) { plink->nlink = nlink->nlink; }
                nlink->msize = _ZERO;
                return nlink;
            }
            plink = nlink;
            nlink = nlink->nlink;
        }
        return _NULL;
    }
public: /* vetters */
    inline v1bit_t has_msize(msize_t msize) const
    { return this->msize == msize; }
    inline v1bit_t has_nlink(msize_t msize) const
    {
        if (this->nlink) { return this->nlink->has_nlink(msize); }
        else { return this->has_msize(msize); }
    }
    inline v1bit_t has_nlink() const
    { return this->nlink; }
public: /* symbols */
    template <typename t_vdata_t>
    operator t_vdata_t* ()
    { return reinterpret_cast<t_vdata_t*>(this); }
    template <typename t_vdata_t>
    operator const t_vdata_t* () const
    { return reinterpret_cast<const t_vdata_t*>(this); }
    template <typename t_vdata_t>
    operator t_vdata_t& ()
    { return *reinterpret_cast<t_vdata_t*>(this); }
    template <typename t_vdata_t>
    operator const t_vdata_t& () const
    { return *reinterpret_cast<const t_vdata_t*>(this); }
public: /* datadef */
    mlink_t*nlink;
    msize_t msize;
};

/* reference type
 * description:
 * smart "shared" pointer analog
 * uses the given memory owner
 * it's value gets deleted if there is no any other refs
 * interaction:
 * > create t_ref_t
 * > > new_ref with particular allocator;
 * > > set_ref for other keepers -> use this as a pointer
*/
template <typename t_value_t, class t_owner_t = heapy_t>
class t_refer_t
{
public: /* typedef */
    using owner_t = t_owner_t;
	using value_t = t_value_t;
	using count_t = msize_t;
	using refer_t = t_refer_t<value_t, owner_t>;
public: /* codetor */
    t_refer_t()
        : vdata(_NULL), count(_ZERO)
    { }
    t_refer_t(refer_t&rcopy)
        : refer_t()
    { this->setup(rcopy); }
    t_refer_t(refer_t&&mcopy)
        : refer_t()
    { this->setup(mcopy); mcopy.reset(); }
    template<typename a_t_value_t = value_t>
    t_refer_t(t_refer_t<a_t_value_t, owner_t>&rcopy)
        : t_refer_t()
    { this->setup(rcopy); }
    template<typename a_t_value_t = value_t>
    t_refer_t(t_refer_t<a_t_value_t, owner_t>&&mcopy)
        : t_refer_t()
    { this->setup(mcopy); mcopy.reset(); }
    template<typename a_t_value_t = value_t, typename...t_vargs_t>
    t_refer_t(t_vargs_t&&...vargs)
        : t_refer_t()
    { this->setup<a_t_value_t>(std::forward<t_vargs_t>(vargs)...); }
	~t_refer_t() { this->reset(); }
public: /* getters */
    count_t get_count() const
    { return this->count ? *this->count : 0; }
    /** vdata **/
    template<typename t_vdata_t = t_value_t>
    t_vdata_t*get_vdata()
    { return static_cast<t_vdata_t*>(this->vdata); }
    template<typename t_vdata_t = t_value_t>
    const t_vdata_t*get_vdata() const
    { return static_cast<const t_vdata_t*>(this->vdata); }
    /** value **/
    template<typename a_t_value_t = t_value_t>
    a_t_value_t&get_value()
    { return *static_cast<a_t_value_t*>(this->vdata); }
    template<typename a_t_value_t = t_value_t>
    const a_t_value_t&get_value() const
    { return *static_cast<const a_t_value_t*>(this->vdata); }
public: /* setters */
    template <typename a_t_value_t, typename...t_vargs_t>
    v1bit_t setup(t_vargs_t&&...vargs)
    {
		this->reset();
        owner_t::global->new_one(
            this->vdata,
            std::forward<t_vargs_t>(vargs)...
        );
        owner_t::global->new_one(this->count, 1);
		return _TRUTH;
	}
	template<typename a_t_value_t = value_t>
    v1bit_t setup(t_refer_t<a_t_value_t, owner_t>&refer)
    { /* reference sharing */
		this->reset();
        this->vdata = static_cast<value_t*>(refer.vdata);
        this->count = refer.count;
        if (this->count)
        { *this->count += 1; }
        else if (this->vdata)
        { owner_t::global->new_one(this->count, 1); }
		return _TRUTH;
	}
    v1bit_t reset()
    {
        if (this->count)
        {
            if ((*this->count -= 1) == _ZERO)
            {
                owner_t::global->del_one(this->vdata);
                owner_t::global->del_one(this->count);
			}
		}
		this->vdata = _NULL;
		this->count = _NULL;
		return _TRUTH;
	}
public: /* vetters */
    v1bit_t vet_vdata() const
    { return this->get_vdata() != _NULL; }
    v1bit_t vet_count() const
    { return this->get_count() > _ZERO; }
    v1bit_t vet_count(count_t count) const
    { return this->get_count() >= count; }
    v1bit_t vet() const
    { return this->vet_vdata() && this->vet_count(); }
public: /* symbols */
    refer_t&operator=(const refer_t&rcopy)
    { this->setup(rcopy); return *this; }
    refer_t&operator=(refer_t&&mcopy)
    { this->setup(mcopy) && mcopy.reset(); return *this; }
	template<typename a_t_value_t = value_t>
    refer_t&operator=(t_refer_t<a_t_value_t, owner_t>&rcopy)
    { this->setup(rcopy); return *this; }
	template<typename a_t_value_t = value_t>
    refer_t&operator=(t_refer_t<a_t_value_t, owner_t>&&mcopy)
    { this->setup(mcopy) && mcopy.reset(); return *this; }
	/** referencing  **/
    value_t*operator&()
    { return this->vdata; }
    const value_t*operator&() const
    { return this->vdata; }
    value_t&operator*()
    { return *(this->vdata); }
    const value_t& operator*() const
    { return *(this->vdata); }
    value_t*operator->()
    { return (this->vdata); }
    const value_t*operator->() const
    { return (this->vdata); }
	/** convertion **/
    operator value_t*()
    { return static_cast<value_t*>(this->vdata); }
    operator const value_t*() const
    { return static_cast<const value_t*>(this->vdata); }
    operator value_t&()
    { return static_cast<value_t&>(*this->vdata); }
    operator const value_t&() const
    { return static_cast<const value_t&>(*this->vdata); }
    template<typename a_t_value_t> operator a_t_value_t*()
    { return static_cast<a_t_value_t*>(this->vdata); }
    template<typename a_t_value_t> operator const a_t_value_t*() const
    { return static_cast<const a_t_value_t*>(this->vdata); }
    template<typename a_t_value_t>
    operator t_refer_t<a_t_value_t, owner_t>()
    { return t_refer_t<a_t_value_t, owner_t>(*this); }
    template<typename a_t_value_t>
    operator t_refer_t<a_t_value_t, owner_t>() const
    { return t_refer_t<a_t_value_t, owner_t>(*this); }
    /** logic **/
    operator v1bit_t() const
    { return this->vdata != _NULL; }
    v1bit_t operator==(refer_t&refer) const
    { return this->vdata == refer.get_vdata(); }
    v1bit_t operator!=(refer_t&refer) const
    { return this->vdata != refer.get_vdata(); }
    v1bit_t operator==(mdata_t vdata) const
    { return reinterpret_cast<mdata_t>(this->vdata) == vdata; }
    v1bit_t operator!=(mdata_t mdata) const
    { return reinterpret_cast<mdata_t>(this->vdata) == vdata; }
private: /* datadef */
	mutable value_t*vdata;
	mutable count_t*count;
};

/** datadef **/

extern heapy_t*heapy;
extern stack_t*stack;
extern arena_t*arena;
extern pager_t*pager;
extern owner_t*owner;

/** actions **/

extern bool_t init();
extern bool_t quit();

_NAMESPACE_LEAVE_MEMO

_NAMESPACE_LEAVE
