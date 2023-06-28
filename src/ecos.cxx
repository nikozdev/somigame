#include "head.hxx"

/* headers */

#include "ecos.hxx"

/* content */

_NAMESPACE_ENTER

/** typedef **/

/** datadef **/

ecos_t ecos;

/** actions **/

void ecos_init()
{
    /* signals */
    entt::sigh_helper{ecos}
    .with<com_family_t>().on_update<[](ecos_t&ecos, ent_t ent){
    }>().on_construct<[](ecos_t&ecos, ent_t ent){
        auto*family_e =&ecos.get<com_family_t>(ent);
        auto ancestor = family_e->ancestor;
        family_e->ancestor = entt::null;
        set_ancestor(ent, ancestor);
        auto follower = family_e->follower;
        family_e->follower = entt::null;
        set_follower(ent, follower);
    }>().on_destroy<[](ecos_t&ecos, ent_t ent){
        auto*family_e =&ecos.get<com_family_t>(ent);
        set_ancestor(ent);
        set_follower(ent);
    }>();
    /* others can start from 1 */
    auto ent0 = ecos.create();
}
void ecos_quit()
{
    ecos.clear();
}

/** getters **/

ent_t get_ancestor(ent_t ent)
{
    if (ecos.valid(ent) == _FALSE) { return entt::null; }
    auto family_e = ecos.get<com_family_t>(ent);
    return family_e.ancestor;
}
ent_t get_follower(ent_t ent)
{
    if (ecos.valid(ent) == _FALSE) { return entt::null; }
    if (ecos.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = ecos.get<com_family_t>(ent);
    return family_e.follower;
}
ent_t get_siblingl(ent_t ent)
{
    if (ecos.valid(ent) == _FALSE) { return entt::null; }
    if (ecos.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = ecos.get<com_family_t>(ent);
    return family_e.siblingl;
}
ent_t get_siblingr(ent_t ent)
{
    if (ecos.valid(ent) == _FALSE) { return entt::null; }
    if (ecos.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = ecos.get<com_family_t>(ent);
    return family_e.siblingr;
}

/** vetters **/

bool vet_ancestor(ent_t ent, ent_t ancestor)
{
    if (ecos.valid(ent) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(ent) == _NULL) { return _FALSE; }
    auto*family_e =&ecos.get<com_family_t>(ent);
    return (family_e->ancestor == ancestor);
}
bool vet_ancestry(ent_t ent, ent_t ancestry)
{
    if (ecos.valid(ent) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(ent) == _NULL) { return _FALSE; }
    if (ecos.valid(ancestry) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(ancestry) == _NULL) { return _FALSE; }
    auto*family_e =&ecos.get<com_family_t>(ent);
    auto ancestor = ancestry;
    while (ecos.valid(ancestor))
    {
        if (family_e->ancestor == ancestor) { return _TRUTH; }
        else { ancestor = ecos.get<com_family_t>(ancestor).ancestor; }
    }
    return _FALSE;
}

bool vet_siblings(ent_t ent, ent_t siblings)
{ return vet_siblingl(ent, siblings) || vet_siblingr(siblings); }
bool vet_siblings(ent_t ent, ent_t siblingl, ent_t siblingr)
{ return vet_siblingl(ent, siblingl) && vet_siblingr(siblingr); }
bool vet_siblingl(ent_t ent, ent_t siblingl)
{
    if (ecos.valid(ent) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(ent) == _NULL) { return _FALSE; }
    if (ecos.valid(siblingl) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(siblingl) == _NULL) { return _FALSE; }
    auto*family_e =&ecos.get<com_family_t>(ent);
    if (family_e->siblingl == siblingl) { return _TRUTH; }
    auto temp = family_e->siblingl;
    while (ecos.valid(temp))
    {
        if (siblingl == temp) { return _TRUTH; }
        else { temp = ecos.get<com_family_t>(temp).siblingl; }
    }
    return _FALSE;
}
bool vet_siblingr(ent_t ent, ent_t siblingr)
{
    if (ecos.valid(ent) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(ent) == _NULL) { return _FALSE; }
    if (ecos.valid(siblingr) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(siblingr) == _NULL) { return _FALSE; }
    auto*family_e =&ecos.get<com_family_t>(ent);
    if (family_e->siblingr == siblingr) { return _TRUTH; }
    auto temp = family_e->siblingr;
    while (ecos.valid(temp))
    {
        if (siblingr == temp) { return _TRUTH; }
        else { temp = ecos.get<com_family_t>(temp).siblingr; }
    }
    return _FALSE;
}

bool vet_follower(ent_t ent, ent_t follower)
{
    if (ecos.valid(ent) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(ent) == _NULL) { return _FALSE; }
    return ecos.get<com_family_t>(ent).follower == follower;
}
bool vet_children(ent_t ent, ent_t children)
{
    if (ecos.valid(ent) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(ent) == _NULL) { return _FALSE; }
    if (ecos.valid(children) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(children) == _NULL) { return _FALSE; }
    if (vet_follower(ent, children)) { return _TRUTH; }
    else { return vet_siblings(ecos.get<com_family_t>(ent).follower, children); }
}
bool vet_inherits(ent_t ent, ent_t inherits)
{
    if (ecos.valid(ent) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(ent) == _NULL) { return _FALSE; }
    if (ecos.valid(inherits) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(inherits) == _NULL) { return _FALSE; }
    auto follower = ent;
    auto*family_f =&ecos.get<com_family_t>(follower);
    while(ecos.valid(family_f->follower))
    {
        if (vet_children(follower, inherits)) { return _TRUTH; }
        follower = family_f->follower;
        family_f =&ecos.get<com_family_t>(family_f->follower);
    }
    return _FALSE;
}

/** setters **/

bool set_ancestor(ent_t ent, ent_t ancestor)
{
    if (ent == ancestor) { return _FALSE; }
    if (vet_ancestor(ent, ancestor)) { return _FALSE; }
    if (vet_follower(ent, ancestor)) { return _FALSE; }
    if (ecos.valid(ent) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(ent) == _NULL) { return _FALSE; }
    auto*family_e =&ecos.get<com_family_t>(ent);
    if (ecos.valid(family_e->ancestor))
    {
        auto*family_a =&ecos.get<com_family_t>(family_e->ancestor);
        auto follower = family_a->follower;
        if (follower == ent) { family_a->follower = entt::null; }
        family_e->ancestor = entt::null;
    }
    auto siblingl = family_e->siblingl;
    if (ecos.valid(siblingl))
    {
        ecos.get<com_family_t>(siblingl).siblingr = family_e->siblingr;
        family_e->siblingl = entt::null;
    }
    auto siblingr = family_e->siblingr;
    if (ecos.valid(siblingr))
    {
        ecos.get<com_family_t>(siblingr).siblingl = family_e->siblingl;
        family_e->siblingr = entt::null;
    }
    if (ecos.valid(ancestor))
    {
        auto*family_a =&ecos.get<com_family_t>(ancestor);
        auto follower = family_a->follower;
        if (ecos.valid(follower))
        {
            auto siblingr = follower;
            auto*family_r =&ecos.get<com_family_t>(siblingr);
            while (ecos.valid(family_r->siblingr))
            {
                siblingr = family_r->siblingr;
                family_r =&ecos.get<com_family_t>(siblingr);
            }
            family_e->siblingl = siblingr;
            family_r->siblingr = ent;
        }
        else { family_a->follower = ent; }
    }
    family_e->ancestor = ancestor;
    return _TRUTH;
}
bool set_follower(ent_t ent, ent_t follower)
{
    if (ent == follower) { return _FALSE; }
    if (vet_ancestry(ent, follower)) { return _FALSE; }
    if (ecos.valid(ent) == _FALSE) { return _FALSE; }
    if (ecos.try_get<com_family_t>(ent) == _NULL) { return _FALSE; }
    auto*family_e =&ecos.get<com_family_t>(ent);
    if (ecos.valid(family_e->follower))
    {
        auto follower = family_e->follower;
        auto*family_f =&ecos.get<com_family_t>(follower);
        family_f->ancestor = entt::null;
        auto siblingl = follower;
        auto*family_l =&ecos.get<com_family_t>(siblingl);
        while (ecos.valid(family_l->siblingl))
        {
            siblingl = family_l->siblingl;
            family_l = ecos.try_get<com_family_t>(siblingl);
            family_l->siblingr = entt::null;
            family_l->ancestor = entt::null;
        }
        family_f->siblingl = entt::null;
        auto siblingr = follower;
        auto*family_r =&ecos.get<com_family_t>(siblingr);
        while (ecos.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r = ecos.try_get<com_family_t>(siblingr);
            family_r->siblingl = entt::null;
            family_r->ancestor = entt::null;
        }
        family_f->siblingr = entt::null;
    }
    family_e->follower = follower;
    if (ecos.valid(follower))
    {
        auto*family_f =&ecos.get<com_family_t>(follower);
        auto siblingr = family_f->siblingr;
        if (ecos.valid(siblingr))
        {
            auto*family_r =&ecos.get<com_family_t>(siblingr);
            if (ecos.valid(family_f->siblingl))
            {
                family_r->siblingl = family_f->siblingl;
            }
            else
            {
                family_r->siblingl = follower;
            }
            while (ecos.valid(family_r->siblingl))
            {
                family_r->ancestor = ent;
                siblingr = family_r->siblingl;
                family_r =&ecos.get<com_family_t>(siblingr);
            }
        }
        auto siblingl = family_f->siblingl;
        if (ecos.valid(siblingl))
        {
            auto*family_l =&ecos.get<com_family_t>(siblingl);
            while (ecos.valid(family_l->siblingl))
            {
                family_l->ancestor = ent;
                siblingl = family_l->siblingl;
                family_l =&ecos.get<com_family_t>(siblingl);
            }
            family_l->siblingl = follower;
            family_f->siblingr = siblingl;
        }
    }
    return _TRUTH;
}

_NAMESPACE_LEAVE
