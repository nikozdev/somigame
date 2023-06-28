#include "head.hxx"

/* headers */

#include "memo.hxx"

/* defines */

/* 5-10% performance difference for me */
#define _MSIZE_SET _FALSE
/* 10-20% performance difference for me */
#define _MSIZE_VET _FALSE
#define _MDATA_VET _FALSE

/* most of the times this is gonna break memory allocations */
#define _NEWDELDEF _FALSE
#define _NEWDELDEF_ELOG _FALSE

/* content */

_NAMESPACE_ENTER

_NAMESPACE_ENTER_MEMO

/** typedef **/

/*** owner_t ***/

/**** datadef ****/

owner_t*owner_t::global = _NULL;

/*** heapy_t ***/

/**** codetor ****/

heapy_t::heapy_t() :
    msize_used(_ZERO), msize_umax(_ZERO),
    malig{_ALIGN(sizeof(mdata_t), alignof(mdata_t))}
{}

heapy_t::~heapy_t()
{}

/**** actions ****/

v1bit_t heapy_t::give(mdata_t&mdata, msize_t msize, msize_t malig)
{
#if _MSIZE_VET
    _EFNOT(msize > _ZERO, "cannot give 0 bytes!", return _FALSE);
#endif
    malig = _ALIGN(malig, this->malig);
    msize = _ALIGN(msize, malig);
#if _NEWDELDEF
    mdata = std::malloc(msize);
#else
    mdata = new mbyte_t[msize];
#endif
#if _MSIZE_SET
    this->msize_used += msize;
    this->msize_umax = _NUM_MAX(this->msize_umax, this->msize_used);
#endif
    return _TRUTH;
}

v1bit_t heapy_t::take(mdata_t&mdata, msize_t msize, msize_t malig)
{
#if _MSIZE_VET
    _EFNOT(msize > _ZERO, "cannot give 0 bytes!", return _FALSE);
#endif
    malig = _ALIGN(malig, this->malig);
    msize = _ALIGN(msize, malig);
#if _NEWDELDEF
    std::free(mdata);
#else
    delete[]static_cast<mbyte_t*>(mdata);
#endif
#if _MSIZE_SET
    this->msize_used -= msize;
#endif
    return _TRUTH;
}

/**** datadef ****/

heapy_t*heapy_t::global = _NULL;

/*** stack_t ***/

/**** codetor ****/

stack_t::stack_t(msize_t msize, owner_t*owner) :
    owner_t(),
    owner(owner ? owner : heapy),
    mdata_head(_NULL), mdata_tail(_NULL), mdata_back(_NULL),
    msize_full(msize), msize_umax(_ZERO), msize_used(_ZERO),
    malig(_ALIGN(sizeof(mdata_t), alignof(mdata_t)))
{
    this->msize_full = _ALIGN(this->msize_full, this->malig);
    this->owner->give(this->mdata_head, this->msize_full, this->malig);
    this->mdata_back = this->mdata_head;
    this->mdata_tail = this->mdata_head + this->msize_full;
}
stack_t::~stack_t()
{
    this->owner->take(this->mdata_head, this->msize_full, this->malig);
}

/**** actions ****/

v1bit_t stack_t::give(mdata_t&mdata, msize_t msize, msize_t malig)
{
#if _MSIZE_VET
    _EFNOT(msize > _ZERO, "cannot give 0 bytes!", return _FALSE);
#endif
    mdata = _NULL;
    malig = _ALIGN(malig, this->malig);
    msize = _ALIGN(msize, malig);
#if _MSIZE_VET
    _EFNOT(this->vet_msize_free(msize), "vet_msize_free error!", {
        this->elog();
        return _FALSE;
    });
#endif
    mdata = this->mdata_back;
    this->mdata_back += msize;
#if _MSIZE_SET
    this->msize_umax += msize;
    this->msize_used += msize;
#endif
    return _TRUTH;
}

v1bit_t stack_t::take(mdata_t&mdata, msize_t msize, msize_t malig)
{
#if _MSIZE_VET
    _EFNOT(mdata != _NULL, "cannot deallocate by null!", return _FALSE);
    _EFNOT(msize != _ZERO, "cannot deallocate 0 bytes!", return _FALSE);
#endif
#if _MDATA_VET
    _EFNOT(this->vet_mdata(mdata), "mdata does not belong this arena!", return _FALSE);
#endif
    malig = _ALIGN(malig, this->malig);
    msize = _ALIGN(msize, malig);
#if _MSIZE_VET
    _EFNOT(this->vet_msize_used(msize), "too many bytes to delete!", return _FALSE);
#endif
#if _MDATA_VET
    _EFNOT(
        this->vet_mdata(mdata, msize),
        "mdata has to be at the top of the stack!",
        return _FALSE
    );
#endif
    this->mdata_back -= msize;
#if _MSIZE_SET
	this->msize_used -= msize;
#endif
    return _TRUTH;
}

/**** datadef ****/

stack_t*stack_t::global = _NULL;

/*** arena_t ***/

/**** codetor ****/

arena_t::arena_t(msize_t msize, owner_t*owner) :
    owner_t(),
    owner(owner ? owner : heapy),
    mdata_head(_NULL), mdata_tail(_NULL), mdata_back(_NULL),
    msize_full(msize), msize_umax(_ZERO), msize_used(_ZERO),
    malig(_ALIGN(sizeof(mlink_t), alignof(mlink_t))),
    mlink(_NULL)
{
    this->msize_full = _ALIGN(this->msize_full, this->malig);
    this->owner->give(this->mdata_head, this->msize_full, this->malig);
    this->mdata_back = this->mdata_head;
    this->mdata_tail = this->mdata_head + this->msize_full;
}
arena_t::~arena_t()
{
    this->owner->take(this->mdata_head, this->msize_full, this->malig);
}

/**** actions ****/

v1bit_t arena_t::give(mdata_t&mdata, msize_t msize, msize_t malig)
{
#if _MSIZE_VET
    _EFNOT(msize > _ZERO, "cannot give 0 bytes!", return _FALSE);
#endif
    mdata = _NULL;
    malig = _ALIGN(malig, this->malig);
    msize = _ALIGN(msize, malig);
#if _MSIZE_VET
    _EFNOT(this->vet_msize_free(msize), "vet_msize_free error!", {
        this->elog();
        return _FALSE;
    });
#endif
    mlink_t*plink = _NULL;
    mlink_t*nlink = this->mlink;
    while (nlink)
    {
        if (nlink->has_msize(msize))
        {
            if (plink) { plink->nlink = nlink->nlink; }
            else { this->mlink = nlink->nlink; }
            mdata = nlink;
#if _MSIZE_SET
            this->msize_used += msize;
#endif
            return _TRUTH;
        }
        plink = nlink;
        nlink = nlink->nlink;
    }
    /* failed to get a free-list block, advance the back */
    mdata = this->mdata_back;
    this->mdata_back += msize;
#if _MSIZE_SET
    this->msize_umax += msize;
    this->msize_used += msize;
#endif
    return _TRUTH;
}

v1bit_t arena_t::take(mdata_t&mdata, msize_t msize, msize_t malig)
{
#if _MSIZE_VET
    _EFNOT(mdata != _NULL, "cannot deallocate by null!", return _FALSE);
    _EFNOT(msize != _ZERO, "cannot deallocate 0 bytes!", return _FALSE);
#endif
#if _MDATA_VET
    _EFNOT(this->vet_mdata(mdata), "mdata does not belong this arena!", return _FALSE);
#endif
    malig = _ALIGN(malig, this->malig);
    msize = _ALIGN(msize, malig);
#if _MSIZE_VET
    _EFNOT(this->vet_msize_used(msize), "too many bytes to delete!", return _FALSE);
#endif
    mlink_t*mlink = new(mdata)mlink_t();
    mlink->nlink = this->mlink;
    mlink->msize = msize;
    this->mlink = mlink;
	/* mdata_back and msize_umax should not be changed back! */
#if _MSIZE_SET
	this->msize_used -= msize;
#endif
    return _TRUTH;
}

/**** datadef ****/

arena_t*arena_t::global = _NULL;

/*** pager_t ***/

/**** codetor ****/

pager_t::pager_t(msize_t psize, owner_t*owner) :
    owner_t(),
    owner(owner ? owner : heapy),
    hpage(_NULL), bpage(_NULL), tpage(_NULL),
    mlink(_NULL),
    malig(_ALIGN(sizeof(mlink_t), alignof(mlink_t)))
{
    this->owner->new_one(this->hpage),
    this->owner->give(this->hpage->hdata, psize, this->malig),
    this->hpage->npage = _NULL;
    this->hpage->tdata = this->hpage->hdata + psize;
    this->hpage->bdata = this->hpage->hdata;
    this->bpage = this->hpage;
    this->tpage = this->hpage;
}
pager_t::~pager_t()
{
    auto ipage = this->hpage;
    while (ipage)
    {
        this->hpage = ipage->npage;
        this->owner->take(
            ipage->hdata,
            (ipage->tdata - ipage->hdata),
            this->malig
        ),
        this->owner->del_one(ipage),
        ipage = this->hpage;
    }
    this->hpage = _NULL; this->bpage = _NULL; this->tpage = _NULL;
}

/**** actions ****/

v1bit_t pager_t::give(mdata_t&mdata, msize_t msize, msize_t malig)
{
    /* argument adjustment */
    mdata = _NULL;
    malig = _ALIGN(malig, this->malig);
    msize = _ALIGN(msize, malig);
    /* memory link */
    mlink_t*plink = _NULL;
    mlink_t*nlink = this->mlink;
    while (nlink)
    {
        if (nlink->has_msize(msize))
        {
            if (plink) { plink->nlink = nlink->nlink; }
            else { this->mlink = nlink->nlink; }
            mdata = nlink;
            return _TRUTH;
        }
        plink = nlink;
        nlink = nlink->nlink;
    }
    /* the default page advancement */
    if ((this->bpage->tdata - this->bpage->bdata) < msize)
    { /* not enough at the back - move to the next page */
        this->owner->new_one(this->tpage->npage);
        this->tpage = this->tpage->npage;
        this->tpage->npage = _NULL;
        auto nsize = _NUM_MAX(PAGER_PSIZE, msize);
        this->owner->give(this->tpage->hdata, nsize, malig);
        this->tpage->bdata = this->tpage->hdata;
        this->tpage->tdata = this->tpage->hdata + nsize;
        this->bpage->npage = this->tpage;
        this->bpage = this->bpage->npage;
    }
    mdata = this->bpage->bdata;
    this->bpage->bdata += msize;
    return _TRUTH;
}

v1bit_t pager_t::take(mdata_t&mdata, msize_t msize, msize_t malig)
{
    malig = _ALIGN(malig, this->malig);
    msize = _ALIGN(msize, malig);
    mlink_t*mlink = new(mdata)mlink_t();
    mlink->nlink = this->mlink;
    mlink->msize = msize;
    this->mlink = mlink;
    return _TRUTH;
}

/**** datadef ****/

pager_t*pager_t::global = _NULL;

/** datadef **/

heapy_t*heapy;
stack_t*stack;
arena_t*arena;
pager_t*pager;
owner_t*owner;

/** actions **/

bool_t init()
{
    if constexpr (_FALSE)
    {
        heapy_t::global = new(
            std::malloc( _ALIGN(sizeof(heapy_t), alignof(heapy_t)) )
        )heapy_t();
        stack_t::global = new(
            std::malloc( _ALIGN(sizeof(stack_t), alignof(stack_t)) )
        )stack_t(OWNER_MSIZE, heapy_t::global);
        arena_t::global = new(
            std::malloc( _ALIGN(sizeof(arena_t), alignof(arena_t)) )
        )arena_t(OWNER_MSIZE, heapy_t::global);
        pager_t::global = new(
            std::malloc( _ALIGN(sizeof(pager_t), alignof(pager_t)) )
        )pager_t(PAGER_PSIZE, heapy_t::global);
    }
    else
    {
        static heapy_t heapy = heapy_t();
        heapy_t::global =&heapy;
        static stack_t stack = stack_t(OWNER_MSIZE,&heapy);
        stack_t::global =&stack;
        static arena_t arena = arena_t(OWNER_MSIZE,&heapy);
        arena_t::global =&arena;
        static pager_t pager = pager_t(PAGER_PSIZE,&heapy);
        pager_t::global =&pager;
        owner_t::global =&arena;
    }
    heapy = heapy_t::global;
    stack = stack_t::global;
    arena = arena_t::global;
    pager = pager_t::global;
    owner = pager_t::global;
    return _TRUTH;
}

_NAMESPACE_LEAVE_MEMO

_NAMESPACE_LEAVE

/* symbols */

#if _NEWDELDEF
/** new **/
void* operator new(size_t msize)
{
    void*mdata = _NULL;
    size_t malig = alignof(void*);
    if (_NAMESPACE::memo::arena)
    { _NAMESPACE::memo::arena->give(mdata, msize, malig); }
    else { mdata = std::malloc(_ALIGN(msize, malig)); }
    auto func_name = _FUNC_NAME, func_sign = _FUNC_SIGN;
#if _NEWDELDEF_ELOG
    _ELOG("[func_name]=%s;[func_sign]=%s;[msize]=%zu;[malig]=%zu;"
        "%c",
        func_name, func_sign,
        msize, malig,
        _ENDL_SYM
    );
#endif /* _NEWDELDEF_ELOG */
    return mdata;
}
void* operator new(size_t msize, size_t malig)
{
    void*mdata = _NULL;
    if (_NAMESPACE::memo::arena)
    { _NAMESPACE::memo::arena->give(mdata, msize, malig); }
    else { mdata = std::malloc(_ALIGN(msize, malig)); }
    auto func_name = _FUNC_NAME, func_sign = _FUNC_SIGN;
#if _NEWDELDEF_ELOG
    _ELOG("[func_name]=%s;[func_sign]=%s;[msize]=%zu;[malig]=%zu;"
        "%c",
        func_name, func_sign,
        msize, malig,
        _ENDL_SYM
    );
#endif /* _NEWDELDEF_ELOG */
    return mdata;
}
void* operator new[](size_t msize)
{
    void*mdata = _NULL;
    size_t malig = alignof(void*);
    if (_NAMESPACE::memo::arena)
    { _NAMESPACE::memo::arena->give(mdata, msize, malig); }
    else
    { mdata = std::malloc(_ALIGN(msize, malig)); }
    auto func_name = _FUNC_NAME, func_sign = _FUNC_SIGN;
#if _NEWDELDEF_ELOG
    _ELOG("[func_name]=%s;[func_sign]=%s;[msize]=%zu;[malig]=%zu;"
        "%c",
        func_name, func_sign,
        msize, malig,
        _ENDL_SYM
    );
#endif /* _NEWDELDEF_ELOG */
    return mdata;
}
void* operator new[](size_t msize, size_t malig)
{
    void*mdata = _NULL;
    if (_NAMESPACE::memo::arena)
    { _NAMESPACE::memo::arena->give(mdata, msize, malig); }
    else
    { mdata = std::malloc(_ALIGN(msize, malig)); }
    auto func_name = _FUNC_NAME, func_sign = _FUNC_SIGN;
#if _NEWDELDEF_ELOG
    _ELOG("[func_name]=%s;[func_sign]=%s;[msize]=%zu;[malig]=%zu;"
        "%c",
        func_name, func_sign,
        msize, malig,
        _ENDL_SYM
    );
#endif /* _NEWDELDEF_ELOG */
    return mdata;
}
/** delete **/
void operator delete(void*mdata, size_t msize) noexcept
{
    size_t malig = alignof(void*);
    if (_NAMESPACE::memo::arena)
    { _NAMESPACE::memo::arena->take(mdata, msize, malig); }
    else { std::free(mdata); }
    auto func_name = _FUNC_NAME, func_sign = _FUNC_SIGN;
#if _NEWDELDEF_ELOG
    _ELOG("[func_name]=%s;[func_sign]=%s;[msize]=%zu;[malig]=%zu;"
        "%c",
        func_name, func_sign,
        msize, malig,
        _ENDL_SYM
    );
#endif /* _NEWDELDEF_ELOG */
}
void operator delete(void*mdata, size_t msize, size_t malig) noexcept
{
    if (_NAMESPACE::memo::arena)
    { _NAMESPACE::memo::arena->take(mdata, msize, malig); }
    else
    { std::free(mdata); }
    auto func_name = _FUNC_NAME, func_sign = _FUNC_SIGN;
#if _NEWDELDEF_ELOG
    _ELOG("[func_name]=%s;[func_sign]=%s;[msize]=%zu;[malig]=%zu;"
        "%c",
        func_name, func_sign,
        msize, malig,
        _ENDL_SYM
    );
#endif /* _NEWDELDEF_ELOG */
}
void operator delete[](void*mdata, size_t msize) noexcept
{
    size_t malig = alignof(void*);
    if (_NAMESPACE::memo::arena)
    { _NAMESPACE::memo::arena->take(mdata, msize, malig); }
    else
    { std::free(mdata); }
    auto func_name = _FUNC_NAME, func_sign = _FUNC_SIGN;
#if _NEWDELDEF_ELOG
    _ELOG("[func_name]=%s;[func_sign]=%s;[msize]=%zu;[malig]=%zu;"
        "%c",
        func_name, func_sign,
        msize, malig,
        _ENDL_SYM
    );
#endif /* _NEWDELDEF_ELOG */
}
void operator delete[](void*mdata, size_t msize, size_t malig) noexcept
{
    if (_NAMESPACE::memo::arena)
    { _NAMESPACE::memo::arena->take(mdata, msize, malig); }
    else
    { std::free(mdata); }
    auto func_name = _FUNC_NAME, func_sign = _FUNC_SIGN;
#if _NEWDELDEF_ELOG
    _ELOG("[func_name]=%s;[func_sign]=%s;[msize]=%zu;[malig]=%zu;"
        "%c",
        func_name, func_sign,
        msize, malig,
        _ENDL_SYM
    );
#endif /* _NEWDELDEF_ELOG */
}
#endif /* _NEWDELDEF */
