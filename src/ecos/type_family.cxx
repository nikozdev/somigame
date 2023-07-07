#ifndef SOMIGAME_ECOS_TYPE_FAMILY_CXX
#define SOMIGAME_ECOS_TYPE_FAMILY_CXX

#include "../head.hxx"

#include "type_ent.hxx"
#include "type_reg.hxx"
#include "type_family.hxx"

namespace somigame { namespace ecos {

// actions

_DEFN_FUNC bool init_type_family()
{
    /* signals */
    entt::sigh_helper{reg}
    .with<com_family_t>().on_update<[](reg_t&reg, ent_t ent){
    }>().on_construct<[](reg_t&reg, ent_t ent){
        auto*family_e =&reg.get<com_family_t>(ent);
        auto ancestor = family_e->ancestor;
        family_e->ancestor = entt::null;
        set_ancestor(ent, ancestor);
        auto follower = family_e->follower;
        family_e->follower = entt::null;
        set_follower(ent, follower);
    }>().on_destroy<[](reg_t&reg, ent_t ent){
        auto*family_e =&reg.get<com_family_t>(ent);
        set_ancestor(ent);
        set_follower(ent);
    }>();
    return TRUTH;
} // init_type_family
_DEFN_FUNC bool quit_type_family()
{
    return TRUTH;
} // quit_type_family

// getters

_DEFN_FUNC ent_t get_ancestor(ent_t ent)
{
    if (reg.valid(ent) == FALSE) { return entt::null; }
    auto family_e = reg.get<com_family_t>(ent);
    return family_e.ancestor;
} // get_ancestor
_DEFN_FUNC ent_t get_follower(ent_t ent)
{
    if (reg.valid(ent) == FALSE) { return entt::null; }
    if (reg.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = reg.get<com_family_t>(ent);
    return family_e.follower;
} // get_follower
_DEFN_FUNC ent_t get_siblingl(ent_t ent)
{
    if (reg.valid(ent) == FALSE) { return entt::null; }
    if (reg.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = reg.get<com_family_t>(ent);
    return family_e.siblingl;
} // get_siblingl
_DEFN_FUNC ent_t get_siblingr(ent_t ent)
{
    if (reg.valid(ent) == FALSE) { return entt::null; }
    if (reg.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = reg.get<com_family_t>(ent);
    return family_e.siblingr;
} // get_siblingr

// vetters

_DEFN_FUNC bool vet_ancestor(ent_t ent, ent_t ancestor)
{
    if (reg.valid(ent) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    return (family_e->ancestor == ancestor);
} // vet_ancestor
_DEFN_FUNC bool vet_ancestry(ent_t ent, ent_t ancestry)
{
    if (reg.valid(ent) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    if (reg.valid(ancestry) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(ancestry) == NULL) { return FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    auto ancestor = ancestry;
    while (reg.valid(ancestor))
    {
        if (family_e->ancestor == ancestor) { return TRUTH; }
        else { ancestor = reg.get<com_family_t>(ancestor).ancestor; }
    }
    return FALSE;
} // vet_ancestry

_DEFN_FUNC bool vet_siblings(ent_t ent, ent_t siblings)
{
    return vet_siblingl(ent, siblings) || vet_siblingr(siblings);
} // vet_siblings
_DEFN_FUNC bool vet_siblings(ent_t ent, ent_t siblingl, ent_t siblingr)
{
    return vet_siblingl(ent, siblingl) && vet_siblingr(siblingr);
} // vet_siblings
_DEFN_FUNC bool vet_siblingl(ent_t ent, ent_t siblingl)
{
    if (reg.valid(ent) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    if (reg.valid(siblingl) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(siblingl) == NULL) { return FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (family_e->siblingl == siblingl) { return TRUTH; }
    auto temp = family_e->siblingl;
    while (reg.valid(temp))
    {
        if (siblingl == temp) { return TRUTH; }
        else { temp = reg.get<com_family_t>(temp).siblingl; }
    }
    return FALSE;
} // vet_siblingl
_DEFN_FUNC bool vet_siblingr(ent_t ent, ent_t siblingr)
{
    if (reg.valid(ent) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    if (reg.valid(siblingr) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(siblingr) == NULL) { return FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (family_e->siblingr == siblingr) { return TRUTH; }
    auto temp = family_e->siblingr;
    while (reg.valid(temp))
    {
        if (siblingr == temp) { return TRUTH; }
        else { temp = reg.get<com_family_t>(temp).siblingr; }
    }
    return FALSE;
} // vet_siblingr

_DEFN_FUNC bool vet_follower(ent_t ent, ent_t follower)
{
    if (reg.valid(ent) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    return reg.get<com_family_t>(ent).follower == follower;
} // vet_follower
_DEFN_FUNC bool vet_children(ent_t ent, ent_t children)
{
    if (reg.valid(ent) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    if (reg.valid(children) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(children) == NULL) { return FALSE; }
    if (vet_follower(ent, children)) { return TRUTH; }
    else { return vet_siblings(reg.get<com_family_t>(ent).follower, children); }
} // vet_children
_DEFN_FUNC bool vet_inherits(ent_t ent, ent_t inherits)
{
    if (reg.valid(ent) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    if (reg.valid(inherits) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(inherits) == NULL) { return FALSE; }
    auto follower = ent;
    auto*family_f =&reg.get<com_family_t>(follower);
    while(reg.valid(family_f->follower))
    {
        if (vet_children(follower, inherits)) { return TRUTH; }
        follower = family_f->follower;
        family_f =&reg.get<com_family_t>(family_f->follower);
    }
    return FALSE;
} // vet_inherits

// setters

_DEFN_FUNC bool set_ancestor(ent_t ent, ent_t ancestor)
{
    if (ent == ancestor) { return FALSE; }
    if (vet_ancestor(ent, ancestor)) { return FALSE; }
    if (vet_follower(ent, ancestor)) { return FALSE; }
    if (reg.valid(ent) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (reg.valid(family_e->ancestor))
    {
        auto*family_a =&reg.get<com_family_t>(family_e->ancestor);
        auto follower = family_a->follower;
        if (follower == ent) { family_a->follower = entt::null; }
        family_e->ancestor = entt::null;
    }
    auto siblingl = family_e->siblingl;
    if (reg.valid(siblingl))
    {
        reg.get<com_family_t>(siblingl).siblingr = family_e->siblingr;
        family_e->siblingl = entt::null;
    }
    auto siblingr = family_e->siblingr;
    if (reg.valid(siblingr))
    {
        reg.get<com_family_t>(siblingr).siblingl = family_e->siblingl;
        family_e->siblingr = entt::null;
    }
    if (reg.valid(ancestor))
    {
        auto*family_a =&reg.get<com_family_t>(ancestor);
        auto follower = family_a->follower;
        if (reg.valid(follower))
        {
            auto siblingr = follower;
            auto*family_r =&reg.get<com_family_t>(siblingr);
            while (reg.valid(family_r->siblingr))
            {
                siblingr = family_r->siblingr;
                family_r =&reg.get<com_family_t>(siblingr);
            }
            family_e->siblingl = siblingr;
            family_r->siblingr = ent;
        }
        else { family_a->follower = ent; }
    }
    family_e->ancestor = ancestor;
    return TRUTH;
} // set_ancestor
_DEFN_FUNC bool set_follower(ent_t ent, ent_t follower)
{
    if (ent == follower) { return FALSE; }
    if (vet_ancestry(ent, follower)) { return FALSE; }
    if (reg.valid(ent) == FALSE) { return FALSE; }
    if (reg.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (reg.valid(family_e->follower))
    {
        auto follower = family_e->follower;
        auto*family_f =&reg.get<com_family_t>(follower);
        family_f->ancestor = entt::null;
        auto siblingl = follower;
        auto*family_l =&reg.get<com_family_t>(siblingl);
        while (reg.valid(family_l->siblingl))
        {
            siblingl = family_l->siblingl;
            family_l = reg.try_get<com_family_t>(siblingl);
            family_l->siblingr = entt::null;
            family_l->ancestor = entt::null;
        }
        family_f->siblingl = entt::null;
        auto siblingr = follower;
        auto*family_r =&reg.get<com_family_t>(siblingr);
        while (reg.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r = reg.try_get<com_family_t>(siblingr);
            family_r->siblingl = entt::null;
            family_r->ancestor = entt::null;
        }
        family_f->siblingr = entt::null;
    }
    family_e->follower = follower;
    if (reg.valid(follower))
    {
        auto*family_f =&reg.get<com_family_t>(follower);
        auto siblingr = family_f->siblingr;
        if (reg.valid(siblingr))
        {
            auto*family_r =&reg.get<com_family_t>(siblingr);
            if (reg.valid(family_f->siblingl))
            {
                family_r->siblingl = family_f->siblingl;
            }
            else
            {
                family_r->siblingl = follower;
            }
            while (reg.valid(family_r->siblingl))
            {
                family_r->ancestor = ent;
                siblingr = family_r->siblingl;
                family_r =&reg.get<com_family_t>(siblingr);
            }
        }
        auto siblingl = family_f->siblingl;
        if (reg.valid(siblingl))
        {
            auto*family_l =&reg.get<com_family_t>(siblingl);
            while (reg.valid(family_l->siblingl))
            {
                family_l->ancestor = ent;
                siblingl = family_l->siblingl;
                family_l =&reg.get<com_family_t>(siblingl);
            }
            family_l->siblingl = follower;
            family_f->siblingr = siblingl;
        }
    }
    return TRUTH;
} // set_follower

} } // namespace somigame { namespace ecos {

#endif//SOMIGAME_ECOS_FAMILY_CXX
