#ifndef SOMIGAME_ECOS_TYPE_FAMILY_HXX
#define SOMIGAME_ECOS_TYPE_FAMILY_HXX

#include "../head.hxx"
#include "type_ent.hxx"
#include "type_reg.hxx"

namespace somigame { namespace ecos {

// typedef

/* family component
 * this is the base for the hierarchy of entities
*/
typedef struct family_t {
    ecos::ent_t ancestor = entt::null;
    ecos::ent_t follower = entt::null;
    ecos::ent_t siblingl = entt::null;
    ecos::ent_t siblingr = entt::null;
} family_t, com_family_t;

// actions

_DECL_FUNC bool init_type_family();
_DECL_FUNC bool quit_type_family();

template<typename func_t, typename...args_t>
void call_for_ancestor(ecos::ent_t ent, func_t func, args_t&&...args)
{
    if (ecos::reg.valid(ent) == FALSE) { return; }
    if (ecos::reg.any_of<com_family_t>(ent) == FALSE) { return; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (reg.valid(family_e->ancestor))
    { func(family_e->ancestor, std::forward<args_t>(args)...); }
}
template<typename func_t, typename...args_t>
void call_for_ancestry(ent_t ent, func_t func, args_t&&...args)
{
    if (reg.valid(ent) == FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == FALSE) { return; }
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
void call_for_children(ent_t ent, func_t func, args_t&&...args)
{
    if (reg.valid(ent) == FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == FALSE) { return; }
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
void call_for_inherit(ent_t ent, func_t func, args_t&&...args)
{
    if (reg.valid(ent) == FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == FALSE) { return; }
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
void call_for_siblingl(ent_t ent, func_t func, args_t&&...args)
{
    if (reg.valid(ent) == FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == FALSE) { return; }
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
void call_for_siblingr(ent_t ent, func_t func, args_t&&...args)
{
    if (reg.valid(ent) == FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == FALSE) { return; }
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
void call_for_siblings(ent_t ent, func_t func, args_t&&...args)
{
    if (reg.valid(ent) == FALSE) { return; }
    if (reg.any_of<com_family_t>(ent) == FALSE) { return; }
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

// getters

_DECL_FUNC ent_t get_ancestor(ent_t ent);
_DECL_FUNC ent_t get_follower(ent_t ent);
_DECL_FUNC ent_t get_siblingl(ent_t ent);
_DECL_FUNC ent_t get_siblingr(ent_t ent);

// vetters

_DECL_FUNC bool vet_ancestry(ent_t ent, ent_t ancestry);
_DECL_FUNC bool vet_ancestor(ent_t ent, ent_t ancestor = entt::null);

_DECL_FUNC bool vet_follower(ent_t ent, ent_t follower = entt::null);
_DECL_FUNC bool vet_children(ent_t ent, ent_t children = entt::null);
_DECL_FUNC bool vet_inherits(ent_t ent, ent_t inherits = entt::null);

_DECL_FUNC bool vet_siblings(ent_t ent, ent_t siblings);
_DECL_FUNC bool vet_siblings(ent_t ent, ent_t siblingl, ent_t siblingr);
_DECL_FUNC bool vet_siblingl(ent_t ent, ent_t siblingl = entt::null);
_DECL_FUNC bool vet_siblingr(ent_t ent, ent_t siblingr = entt::null);

// setters

_DECL_FUNC bool set_ancestor(ent_t ent, ent_t ancestor = entt::null);
_DECL_FUNC bool set_follower(ent_t ent, ent_t follower = entt::null);

} } // namespace somigame { namespace ecos {

#endif//SOMIGAME_ECOS_TYPE_FAMILY_HXX
