/* headers */

#include "ecos.hxx"

/* content */

_NAMESPACE_ENTER

/** typedef **/

/** datadef **/

ecos_t ecos;

constexpr auto&reg = ecos.reg;

/** actions **/

void ecos_init()
{
    using reg_t = entt::registry;
    reg.on_construct<com_family_t>().connect<[](reg_t&reg, entt::entity ent)
    {
        auto*family_e =&reg.get<com_family_t>(ent);
        auto ancestor = family_e->ancestor;
        family_e->ancestor = entt::null;
        ecos_set_ancestor(ent, ancestor);
        auto follower = family_e->follower;
        family_e->follower = entt::null;
        ecos_set_follower(ent, follower);
    }>();
    reg.on_destroy<com_family_t>().connect<[](reg_t&reg, entt::entity ent)
    {
        auto*family_e =&reg.get<com_family_t>(ent);
        ecos_set_ancestor(ent);
        ecos_set_follower(ent);
    }>();
    reg.on_construct<com_cstring_t>().connect<[](reg_t&reg, ent_t ent)
    {
        auto&cstring = reg.get<com_cstring_t>(ent);
        cstring.msize = std::min(cstring.msize, CSTRING_MSIZE);
        memset(cstring.mdata, '\0', cstring.msize);
    }>();
    reg.on_destroy<com_cstring_t>().connect<[](reg_t&reg, ent_t ent)
    {
        auto&cstring = reg.get<com_cstring_t>(ent);
        memset(cstring.mdata, '\0', cstring.msize);
    }>();
    auto null = reg.create();
}

/*** getters ***/

ent_t ecos_get_by_ename(const ename_t&value)
{
    for (auto&&[ent,ename] : reg.view<com_ename_t>().each())
    { if (ename.value == value.value) { return ent; } }
    return entt::null;
}
ent_t ecos_get_by_iname(const iname_t&value)
{
    for (auto&&[ent,iname] : reg.view<com_iname_t>().each())
    { if (iname.value == value.value) { return ent; } }
    return entt::null;
}
ent_t ecos_get_by_sname(const sname_t&value)
{
    for (auto&&[ent,sname] : reg.view<com_sname_t>().each())
    { if (std::strcmp(sname.value, value.value) == 0) { return ent; } }
    return entt::null;
}

ent_t ecos_get_ancestor(ent_t ent)
{
    if (reg.valid(ent) == _FALSE) { return entt::null; }
    auto family_e = reg.get<com_family_t>(ent);
    return family_e.ancestor;
}
ent_t ecos_get_follower(ent_t ent)
{
    if (reg.valid(ent) == _FALSE) { return entt::null; }
    if (reg.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = reg.get<com_family_t>(ent);
    return family_e.follower;
}
ent_t ecos_get_siblingl(ent_t ent)
{
    if (reg.valid(ent) == _FALSE) { return entt::null; }
    if (reg.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = reg.get<com_family_t>(ent);
    return family_e.siblingl;
}
ent_t ecos_get_siblingr(ent_t ent)
{
    if (reg.valid(ent) == _FALSE) { return entt::null; }
    if (reg.any_of<com_family_t>(ent)) { return entt::null; }
    auto family_e = reg.get<com_family_t>(ent);
    return family_e.siblingr;
}

/*** vetters ***/

bool ecos_vet_ancestor(ent_t ent, ent_t ancestor)
{
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    return (family_e->ancestor == ancestor);
}
bool ecos_vet_ancestry(ent_t ent, ent_t ancestry)
{
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    if (reg.valid(ancestry) == _FALSE) { return _FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    auto ancestor = ancestry;
    while (reg.valid(ancestor))
    {
        if (family_e->ancestor == ancestor) { return _TRUTH; }
        else { ancestor = reg.get<com_family_t>(ancestor).ancestor; }
    }
    return _FALSE;
}

bool ecos_vet_siblings(ent_t ent, ent_t siblings)
{ return ecos_vet_siblingl(ent, siblings) || ecos_vet_siblingr(siblings); }
bool ecos_vet_siblings(ent_t ent, ent_t siblingl, ent_t siblingr)
{ return ecos_vet_siblingl(ent, siblingl) && ecos_vet_siblingr(siblingr); }
bool ecos_vet_siblingl(ent_t ent, ent_t siblingl)
{
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    if (reg.valid(siblingl) == _FALSE) { return _FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (family_e->siblingl == siblingl) { return _TRUTH; }
    auto temp = family_e->siblingl;
    while (reg.valid(temp))
    {
        if (siblingl == temp) { return _TRUTH; }
        else { temp = reg.get<com_family_t>(temp).siblingl; }
    }
    return _FALSE;
}
bool ecos_vet_siblingr(ent_t ent, ent_t siblingr)
{
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    if (reg.valid(siblingr) == _FALSE) { return _FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (family_e->siblingr == siblingr) { return _TRUTH; }
    auto temp = family_e->siblingr;
    while (reg.valid(temp))
    {
        if (siblingr == temp) { return _TRUTH; }
        else { temp = reg.get<com_family_t>(temp).siblingr; }
    }
    return _FALSE;
}

bool ecos_vet_follower(ent_t ent, ent_t follower)
{
    auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    return reg.get<com_family_t>(ent).follower == follower;
}
bool ecos_vet_children(ent_t ent, ent_t children)
{
    auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    if (reg.valid(children) == _FALSE) { return _FALSE; }
    if (ecos_vet_follower(ent, children)) { return _TRUTH; }
    else { return ecos_vet_siblings(reg.get<com_family_t>(ent).follower, children); }
}
bool ecos_vet_inherits(ent_t ent, ent_t inherits)
{
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    if (reg.valid(inherits) == _FALSE) { return _FALSE; }
    auto follower = ent;
    auto*family_f =&reg.get<com_family_t>(follower);
    while(reg.valid(family_f->follower))
    {
        if (ecos_vet_children(follower, inherits)) { return _TRUTH; }
        follower = family_f->follower;
        family_f =&reg.get<com_family_t>(family_f->follower);
    }
    return _FALSE;
}

/*** setters ***/

bool ecos_set_ancestor(ent_t ent, ent_t ancestor)
{
    if (ent == ancestor) { return _FALSE; }
    if (ecos_vet_ancestor(ent, ancestor)) { return _FALSE; }
    if (ecos_vet_follower(ent, ancestor)) { return _FALSE; }
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
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
    return _TRUTH;
}
bool ecos_set_follower(ent_t ent, ent_t follower)
{
    if (ent == follower) { return _FALSE; }
    if (ecos_vet_ancestry(ent, follower)) { return _FALSE; }
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (reg.valid(family_e->follower))
    {
        auto follower = family_e->follower;
        auto*family_f =&reg.get<com_family_t>(follower);
        family_f->ancestor = entt::null;
        auto siblingl = family_f->siblingl;
        auto*family_l =&reg.get<com_family_t>(siblingl);
        while (reg.valid(family_l->siblingl))
        {
            family_l->siblingr = entt::null;
            family_l->ancestor = entt::null;
        }
        auto siblingr = family_f->siblingr;
        auto*family_r =&reg.get<com_family_t>(siblingr);
        while (reg.valid(family_r->siblingr))
        {
            family_r->siblingl = entt::null;
            family_r->ancestor = entt::null;
        }
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
    return _TRUTH;
}

_NAMESPACE_LEAVE
