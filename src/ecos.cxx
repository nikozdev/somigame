#ifndef SOMIGAME_ECOS_CXX
#define SOMIGAME_ECOS_CXX 1

#include "head.hxx"

/* headers */

#include "ecos.hxx"
#include "game.hxx"
#include "gfix.hxx"

/* content */

namespace somigame { namespace ecos { /* datadef */

ecs_t ecs;

} } /* datadef */


namespace somigame { namespace ecos { /* actions */

void init()
{
    /* signals */
    entt::sigh_helper{ecs}
    .with<com_family_t>().on_update<[](ecs_t&ecs, ent_t ent){
    }>().on_construct<[](ecs_t&ecs, ent_t ent){
        auto*family_e =&ecs.get<com_family_t>(ent);
        auto ancestor = family_e->ancestor;
        family_e->ancestor = entt::null;
        set_ancestor(ent, ancestor);
        auto follower = family_e->follower;
        family_e->follower = entt::null;
        set_follower(ent, follower);
    }>().on_destroy<[](ecs_t&ecs, ent_t ent){
        auto*family_e =&ecs.get<com_family_t>(ent);
        set_ancestor(ent);
        set_follower(ent);
    }>();
    /* others can start from 1 */
    auto ent0 = ecs.create();
}
void quit()
{
    ecs.clear();
}

} } /* actions */

namespace somigame { namespace ecos { /* getters */

ent_t get_ancestor(ent_t ent)
{
    if (ecs.valid(ent) == FALSE) { return entt::null; }
    auto family_e = ecs.get<com_family_t>(ent);
    return family_e.ancestor;
}
ent_t get_follower(ent_t ent)
{
    if (ecs.valid(ent) == FALSE) { return entt::null; }
    if (ecs.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = ecs.get<com_family_t>(ent);
    return family_e.follower;
}
ent_t get_siblingl(ent_t ent)
{
    if (ecs.valid(ent) == FALSE) { return entt::null; }
    if (ecs.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = ecs.get<com_family_t>(ent);
    return family_e.siblingl;
}
ent_t get_siblingr(ent_t ent)
{
    if (ecs.valid(ent) == FALSE) { return entt::null; }
    if (ecs.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = ecs.get<com_family_t>(ent);
    return family_e.siblingr;
}

} } /* getters */

namespace somigame { namespace ecos { /* vetters */

bool vet_ancestor(ent_t ent, ent_t ancestor)
{
    if (ecs.valid(ent) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    auto*family_e =&ecs.get<com_family_t>(ent);
    return (family_e->ancestor == ancestor);
}
bool vet_ancestry(ent_t ent, ent_t ancestry)
{
    if (ecs.valid(ent) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    if (ecs.valid(ancestry) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(ancestry) == NULL) { return FALSE; }
    auto*family_e =&ecs.get<com_family_t>(ent);
    auto ancestor = ancestry;
    while (ecs.valid(ancestor))
    {
        if (family_e->ancestor == ancestor) { return TRUTH; }
        else { ancestor = ecs.get<com_family_t>(ancestor).ancestor; }
    }
    return FALSE;
}

bool vet_siblings(ent_t ent, ent_t siblings)
{ return vet_siblingl(ent, siblings) || vet_siblingr(siblings); }
bool vet_siblings(ent_t ent, ent_t siblingl, ent_t siblingr)
{ return vet_siblingl(ent, siblingl) && vet_siblingr(siblingr); }
bool vet_siblingl(ent_t ent, ent_t siblingl)
{
    if (ecs.valid(ent) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    if (ecs.valid(siblingl) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(siblingl) == NULL) { return FALSE; }
    auto*family_e =&ecs.get<com_family_t>(ent);
    if (family_e->siblingl == siblingl) { return TRUTH; }
    auto temp = family_e->siblingl;
    while (ecs.valid(temp))
    {
        if (siblingl == temp) { return TRUTH; }
        else { temp = ecs.get<com_family_t>(temp).siblingl; }
    }
    return FALSE;
}
bool vet_siblingr(ent_t ent, ent_t siblingr)
{
    if (ecs.valid(ent) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    if (ecs.valid(siblingr) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(siblingr) == NULL) { return FALSE; }
    auto*family_e =&ecs.get<com_family_t>(ent);
    if (family_e->siblingr == siblingr) { return TRUTH; }
    auto temp = family_e->siblingr;
    while (ecs.valid(temp))
    {
        if (siblingr == temp) { return TRUTH; }
        else { temp = ecs.get<com_family_t>(temp).siblingr; }
    }
    return FALSE;
}

bool vet_follower(ent_t ent, ent_t follower)
{
    if (ecs.valid(ent) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    return ecs.get<com_family_t>(ent).follower == follower;
}
bool vet_children(ent_t ent, ent_t children)
{
    if (ecs.valid(ent) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    if (ecs.valid(children) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(children) == NULL) { return FALSE; }
    if (vet_follower(ent, children)) { return TRUTH; }
    else { return vet_siblings(ecs.get<com_family_t>(ent).follower, children); }
}
bool vet_inherits(ent_t ent, ent_t inherits)
{
    if (ecs.valid(ent) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    if (ecs.valid(inherits) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(inherits) == NULL) { return FALSE; }
    auto follower = ent;
    auto*family_f =&ecs.get<com_family_t>(follower);
    while(ecs.valid(family_f->follower))
    {
        if (vet_children(follower, inherits)) { return TRUTH; }
        follower = family_f->follower;
        family_f =&ecs.get<com_family_t>(family_f->follower);
    }
    return FALSE;
}

} } /* vetters */

namespace somigame { namespace ecos { /* setters */

bool set_ancestor(ent_t ent, ent_t ancestor)
{
    if (ent == ancestor) { return FALSE; }
    if (vet_ancestor(ent, ancestor)) { return FALSE; }
    if (vet_follower(ent, ancestor)) { return FALSE; }
    if (ecs.valid(ent) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    auto*family_e =&ecs.get<com_family_t>(ent);
    if (ecs.valid(family_e->ancestor))
    {
        auto*family_a =&ecs.get<com_family_t>(family_e->ancestor);
        auto follower = family_a->follower;
        if (follower == ent) { family_a->follower = entt::null; }
        family_e->ancestor = entt::null;
    }
    auto siblingl = family_e->siblingl;
    if (ecs.valid(siblingl))
    {
        ecs.get<com_family_t>(siblingl).siblingr = family_e->siblingr;
        family_e->siblingl = entt::null;
    }
    auto siblingr = family_e->siblingr;
    if (ecs.valid(siblingr))
    {
        ecs.get<com_family_t>(siblingr).siblingl = family_e->siblingl;
        family_e->siblingr = entt::null;
    }
    if (ecs.valid(ancestor))
    {
        auto*family_a =&ecs.get<com_family_t>(ancestor);
        auto follower = family_a->follower;
        if (ecs.valid(follower))
        {
            auto siblingr = follower;
            auto*family_r =&ecs.get<com_family_t>(siblingr);
            while (ecs.valid(family_r->siblingr))
            {
                siblingr = family_r->siblingr;
                family_r =&ecs.get<com_family_t>(siblingr);
            }
            family_e->siblingl = siblingr;
            family_r->siblingr = ent;
        }
        else { family_a->follower = ent; }
    }
    family_e->ancestor = ancestor;
    return TRUTH;
}
bool set_follower(ent_t ent, ent_t follower)
{
    if (ent == follower) { return FALSE; }
    if (vet_ancestry(ent, follower)) { return FALSE; }
    if (ecs.valid(ent) == FALSE) { return FALSE; }
    if (ecs.try_get<com_family_t>(ent) == NULL) { return FALSE; }
    auto*family_e =&ecs.get<com_family_t>(ent);
    if (ecs.valid(family_e->follower))
    {
        auto follower = family_e->follower;
        auto*family_f =&ecs.get<com_family_t>(follower);
        family_f->ancestor = entt::null;
        auto siblingl = follower;
        auto*family_l =&ecs.get<com_family_t>(siblingl);
        while (ecs.valid(family_l->siblingl))
        {
            siblingl = family_l->siblingl;
            family_l = ecs.try_get<com_family_t>(siblingl);
            family_l->siblingr = entt::null;
            family_l->ancestor = entt::null;
        }
        family_f->siblingl = entt::null;
        auto siblingr = follower;
        auto*family_r =&ecs.get<com_family_t>(siblingr);
        while (ecs.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r = ecs.try_get<com_family_t>(siblingr);
            family_r->siblingl = entt::null;
            family_r->ancestor = entt::null;
        }
        family_f->siblingr = entt::null;
    }
    family_e->follower = follower;
    if (ecs.valid(follower))
    {
        auto*family_f =&ecs.get<com_family_t>(follower);
        auto siblingr = family_f->siblingr;
        if (ecs.valid(siblingr))
        {
            auto*family_r =&ecs.get<com_family_t>(siblingr);
            if (ecs.valid(family_f->siblingl))
            {
                family_r->siblingl = family_f->siblingl;
            }
            else
            {
                family_r->siblingl = follower;
            }
            while (ecs.valid(family_r->siblingl))
            {
                family_r->ancestor = ent;
                siblingr = family_r->siblingl;
                family_r =&ecs.get<com_family_t>(siblingr);
            }
        }
        auto siblingl = family_f->siblingl;
        if (ecs.valid(siblingl))
        {
            auto*family_l =&ecs.get<com_family_t>(siblingl);
            while (ecs.valid(family_l->siblingl))
            {
                family_l->ancestor = ent;
                siblingl = family_l->siblingl;
                family_l =&ecs.get<com_family_t>(siblingl);
            }
            family_l->siblingl = follower;
            family_f->siblingr = siblingl;
        }
    }
    return TRUTH;
}

} } /* setters */

#endif/*SOMIGAME_ECOS_CXX*/
