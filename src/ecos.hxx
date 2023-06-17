#pragma once

/* headers */

#include "head.hxx"
#include "util.hxx"

/* defines */

/* content */

_NAMESPACE_ENTER

/** typedef **/

/*** enums ***/

typedef enum ename_e {
    _ENAME_META_NONE = 0x0,
    _ENAME_ENTT_MAIN = 0x1,
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

/*** entity ***/

typedef entt::entity ent_t;

/*** component ***/

typedef struct family_t {
    ent_t ancestor = entt::null;
    ent_t follower = entt::null;
    ent_t siblingl = entt::null;
    ent_t siblingr = entt::null;
} family_t, com_family_t;

typedef struct iname_t {
    const index_t value = 0;
} iname_t, com_iname_t;

typedef struct sname_t {
    static constexpr const msize_t msize = CSTRING_MSIZE;
    const mbyte_t value[CSTRING_MSIZE];
} sname_t, com_sname_t;

typedef struct ename_t {
    const ename_e value = _ENAME_META_NONE;
} ename_t, com_ename_t;

typedef struct cstring_t {
    static constexpr const msize_t msize = CSTRING_MSIZE;
    mbyte_t mdata[msize];
} cstring_t, com_cstring_t;

typedef struct coord_t {
    v1s32_t x = 0, y = 0;
} coord_t, com_coord_t;

typedef struct relpos_t {
    v1s_t x = RELPOS_MID, y = RELPOS_MID;
} relpos_t, com_relpos_t;

typedef struct anchor_t {
    v1s_t x = RELPOS_MID, y = RELPOS_MID;
} anchor_t, com_anchor_t;

typedef struct sizes_t {
    v1s32_t w = 0, h = 0;
} sizes_t, com_sizes_t;

typedef struct scale_t {
    v1s32_t x = 1, y = 1;
} scale_t, com_scale_t;

/*** system ***/

typedef struct ecos_t {
    entt::registry reg;
} ecos_t;

/** datadef**/

extern ecos_t ecos;

/** actions **/

extern void ecos_init();

template<typename func_t, typename...args_t>
void ecos_call_for_ancestor(ent_t ent, func_t func, args_t&&...args)
{
    constexpr auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (reg.valid(family_e->ancestor))
    { func(family_e->ancestor, std::forward<args_t>(args)...); }
}
template<typename func_t, typename...args_t>
void ecos_call_for_ancestry(ent_t ent, func_t func, args_t&&...args)
{
    constexpr auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto ancestor = ent;
    auto*family_a =&reg.get<com_family_t>(ancestor);
    while (reg.valid(family_a->ancestor))
    {
        ancestor = family_a->ancestor;
        family_a =&reg.get<com_family_t>(ancestor);
        func(ancestor, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void ecos_call_for_children(ent_t ent, func_t func, args_t&&...args)
{
    constexpr auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto*family_e =&reg.get<com_family_t>(ent);
    auto follower = family_e->follower;
    if (reg.valid(follower))
    {
        func(follower, std::forward<args_t>(args)...);
        auto siblingl = follower;
        auto*family_l =&reg.get<com_family_t>(siblingl);
        while (reg.valid(family_l->siblingl))
        {
            siblingl = family_l->siblingl;
            family_l =&reg.get<com_family_t>(siblingl);
            func(siblingl, std::forward<args_t>(args)...);
        }
        auto siblingr = follower;
        auto*family_r =&reg.get<com_family_t>(siblingr);
        while (reg.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r =&reg.get<com_family_t>(siblingr);
            func(siblingr, std::forward<args_t>(args)...);
        }
    }
}
template<typename func_t, typename...args_t>
void ecos_call_for_inherit(ent_t ent, func_t func, args_t&&...args)
{
    constexpr auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto follower = ent;
    auto*family_f =&reg.get<com_family_t>(follower);
    while (reg.valid(family_f->follower))
    {
        follower = family_f->follower;
        family_f = &reg.get<com_family_t>(follower);
        func(follower, std::forward<args_t>(args)...);
        auto siblingl = follower;
        auto*family_l =&reg.get<com_family_t>(siblingl);
        while (reg.valid(family_l->siblingl))
        {
            siblingl = family_l->siblingl;
            family_l =&reg.get<com_family_t>(siblingl);
            func(siblingl, std::forward<args_t>(args)...);
        }
        auto siblingr = follower;
        auto*family_r =&reg.get<com_family_t>(siblingr);
        while (reg.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r =&reg.get<com_family_t>(siblingr);
            func(siblingr, std::forward<args_t>(args)...);
        }
    }
}
template<typename func_t, typename...args_t>
void ecos_call_for_siblingl(ent_t ent, func_t func, args_t&&...args)
{
    constexpr auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto siblingl = ent;
    auto*family_l =&reg.get<com_family_t>(siblingl);
    while (reg.valid(family_l->siblingr))
    {
        siblingl = family_l->siblingr;
        family_l =&reg.get<com_family_t>(siblingl);
        func(siblingl, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void ecos_call_for_siblingr(ent_t ent, func_t func, args_t&&...args)
{
    constexpr auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto siblingr = ent;
    auto*family_r =&reg.get<com_family_t>(siblingr);
    while (reg.valid(family_r->siblingr))
    {
        siblingr = family_r->siblingr;
        family_r =&reg.get<com_family_t>(siblingr);
        func(siblingr, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void ecos_call_for_siblings(ent_t ent, func_t func, args_t&&...args)
{
    constexpr auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto siblingl = ent;
    auto*family_l =&reg.get<com_family_t>(siblingl);
    while (reg.valid(family_l->siblingl))
    {
        siblingl = family_l->siblingr;
        family_l =&reg.get<com_family_t>(siblingl);
        func(siblingl, std::forward<args_t>(args)...);
    }
    auto siblingr = ent;
    auto*family_r =&reg.get<com_family_t>(siblingr);
    while (reg.valid(family_r->siblingr))
    {
        siblingr = family_r->siblingr;
        family_r =&reg.get<com_family_t>(siblingr);
        func(siblingr, std::forward<args_t>(args)...);
    }
}

/*** getters ***/

extern ent_t ecos_get_by_iname(const iname_t&value);
extern ent_t ecos_get_by_ename(const ename_t&value);
extern ent_t ecos_get_by_sname(const sname_t&value);

extern ent_t ecos_get_ancestor(ent_t ent);
extern ent_t ecos_get_follower(ent_t ent);
extern ent_t ecos_get_siblingl(ent_t ent);
extern ent_t ecos_get_siblingr(ent_t ent);

/*** vetters ***/

extern bool ecos_vet_ancestry(ent_t ent, ent_t ancestry);
extern bool ecos_vet_ancestor(ent_t ent, ent_t ancestor = entt::null);

extern bool ecos_vet_follower(ent_t ent, ent_t follower = entt::null);
extern bool ecos_vet_children(ent_t ent, ent_t children = entt::null);
extern bool ecos_vet_inherits(ent_t ent, ent_t inherits = entt::null);

extern bool ecos_vet_siblings(ent_t ent, ent_t siblings);
extern bool ecos_vet_siblings(ent_t ent, ent_t siblingl, ent_t siblingr);
extern bool ecos_vet_siblingl(ent_t ent, ent_t siblingl = entt::null);
extern bool ecos_vet_siblingr(ent_t ent, ent_t siblingr = entt::null);

/*** setters ***/

extern bool ecos_set_ancestor(ent_t ent, ent_t ancestor = entt::null);
extern bool ecos_set_follower(ent_t ent, ent_t follower = entt::null);

_NAMESPACE_LEAVE
