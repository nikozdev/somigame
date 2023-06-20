#pragma once

/* headers */

#include "head.hxx"
#include "util.hxx"
#include "gfix.hxx"
#include "fsix.hxx"

/* defines */

/* content */

_NAMESPACE_ENTER

/** typedef **/

/*** entity ***/

typedef entt::registry ecos_t;
typedef entt::entity ent_t;
typedef ent_t entity_t;

/*** component ***/

typedef struct family_t {
    ent_t ancestor = entt::null;
    ent_t follower = entt::null;
    ent_t siblingl = entt::null;
    ent_t siblingr = entt::null;
} family_t, com_family_t;

using com_iname_t = iname_t;
using com_sname_t = sname_t;
using com_ename_t = ename_t;

using com_cstring_t = cstring_t;

using com_coord_t = coord_t;
using com_force_t = force_t;
using com_frict_t = frict_t;
using com_weigh_t = weigh_t;
using com_mover_t = mover_t;
using com_direc_t = direc_t;

using com_sizes_t = sizes_t;
using com_scale_t = scale_t;
using com_ratio_t = ratio_t;
using com_recta_t = recta_t;

using com_relpos_t = relpos_t;
using com_anchor_t = anchor_t;

using com_visual_t = visual_t;
using com_color_t = color_t;
using com_image_t = image_region_t;
using com_faces_t = faces_t;
using com_fonts_t = fonts_t;

/** datadef**/

extern ecos_t ecos;

/** actions **/

extern void ecos_init();

template<typename func_t, typename...args_t>
void call_for_ancestor(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto*family_e =&ecos.get<com_family_t>(ent);
    if (ecos.valid(family_e->ancestor))
    { func(family_e->ancestor, std::forward<args_t>(args)...); }
}
template<typename func_t, typename...args_t>
void call_for_ancestry(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto ancestor = ent;
    auto*family_a =&ecos.get<com_family_t>(ancestor);
    while (ecos.valid(family_a->ancestor))
    {
        ancestor = family_a->ancestor;
        family_a =&ecos.get<com_family_t>(ancestor);
        func(ancestor, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void call_for_children(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto*family_e =&ecos.get<com_family_t>(ent);
    auto follower = family_e->follower;
    if (ecos.valid(follower))
    {
        func(follower, std::forward<args_t>(args)...);
        auto siblingl = follower;
        auto*family_l =&ecos.get<com_family_t>(siblingl);
        while (ecos.valid(family_l->siblingl))
        {
            siblingl = family_l->siblingl;
            family_l =&ecos.get<com_family_t>(siblingl);
            func(siblingl, std::forward<args_t>(args)...);
        }
        auto siblingr = follower;
        auto*family_r =&ecos.get<com_family_t>(siblingr);
        while (ecos.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r =&ecos.get<com_family_t>(siblingr);
            func(siblingr, std::forward<args_t>(args)...);
        }
    }
}
template<typename func_t, typename...args_t>
void call_for_inherit(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto follower = ent;
    auto*family_f =&ecos.get<com_family_t>(follower);
    while (ecos.valid(family_f->follower))
    {
        follower = family_f->follower;
        family_f = &ecos.get<com_family_t>(follower);
        func(follower, std::forward<args_t>(args)...);
        auto siblingl = follower;
        auto*family_l =&ecos.get<com_family_t>(siblingl);
        while (ecos.valid(family_l->siblingl))
        {
            siblingl = family_l->siblingl;
            family_l =&ecos.get<com_family_t>(siblingl);
            func(siblingl, std::forward<args_t>(args)...);
        }
        auto siblingr = follower;
        auto*family_r =&ecos.get<com_family_t>(siblingr);
        while (ecos.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r =&ecos.get<com_family_t>(siblingr);
            func(siblingr, std::forward<args_t>(args)...);
        }
    }
}
template<typename func_t, typename...args_t>
void call_for_siblingl(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto siblingl = ent;
    auto*family_l =&ecos.get<com_family_t>(siblingl);
    while (ecos.valid(family_l->siblingr))
    {
        siblingl = family_l->siblingr;
        family_l =&ecos.get<com_family_t>(siblingl);
        func(siblingl, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void call_for_siblingr(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto siblingr = ent;
    auto*family_r =&ecos.get<com_family_t>(siblingr);
    while (ecos.valid(family_r->siblingr))
    {
        siblingr = family_r->siblingr;
        family_r =&ecos.get<com_family_t>(siblingr);
        func(siblingr, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void call_for_siblings(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto siblingl = ent;
    auto*family_l =&ecos.get<com_family_t>(siblingl);
    while (ecos.valid(family_l->siblingl))
    {
        siblingl = family_l->siblingr;
        family_l =&ecos.get<com_family_t>(siblingl);
        func(siblingl, std::forward<args_t>(args)...);
    }
    auto siblingr = ent;
    auto*family_r =&ecos.get<com_family_t>(siblingr);
    while (ecos.valid(family_r->siblingr))
    {
        siblingr = family_r->siblingr;
        family_r =&ecos.get<com_family_t>(siblingr);
        func(siblingr, std::forward<args_t>(args)...);
    }
}

/*** getters ***/

extern ent_t get_by_iname(const iname_t&value);
extern ent_t get_by_ename(const ename_t&value);
extern ent_t get_by_sname(const sname_t&value);

extern ent_t get_ancestor(ent_t ent);
extern ent_t get_follower(ent_t ent);
extern ent_t get_siblingl(ent_t ent);
extern ent_t get_siblingr(ent_t ent);

/*** vetters ***/

extern bool vet_ancestry(ent_t ent, ent_t ancestry);
extern bool vet_ancestor(ent_t ent, ent_t ancestor = entt::null);

extern bool vet_follower(ent_t ent, ent_t follower = entt::null);
extern bool vet_children(ent_t ent, ent_t children = entt::null);
extern bool vet_inherits(ent_t ent, ent_t inherits = entt::null);

extern bool vet_siblings(ent_t ent, ent_t siblings);
extern bool vet_siblings(ent_t ent, ent_t siblingl, ent_t siblingr);
extern bool vet_siblingl(ent_t ent, ent_t siblingl = entt::null);
extern bool vet_siblingr(ent_t ent, ent_t siblingr = entt::null);

/*** setters ***/

extern bool set_ancestor(ent_t ent, ent_t ancestor = entt::null);
extern bool set_follower(ent_t ent, ent_t follower = entt::null);

_NAMESPACE_LEAVE
