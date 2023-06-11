/* headers */

#include "ecos.hxx"

/* content */

_NAMESPACE_ENTER

/** typedef **/

/** datadef **/

ecos_t ecos;

/** actions **/

static void ecos_com_family_ctor(entt::registry&reg, entt::entity ent)
{
    auto*family_e =&reg.get<com_family_t>(ent);
    auto ancestor = family_e->ancestor;
    family_e->ancestor = entt::null;
    ecos_set_ancestor(ent, ancestor);
    auto children = family_e->children;
    family_e->children = entt::null;
    ecos_set_children(ent, children);
    std::printf("com_family_ctor:%+i:%+i;%+i;%+i;%+i;%c",
        ent,
        family_e->ancestor,
        family_e->children,
        family_e->siblingl,
        family_e->siblingr,
        '\n'
    );
}
static void ecos_com_family_dtor(entt::registry&reg, entt::entity ent)
{
    auto*family_e =&reg.get<com_family_t>(ent);
    std::printf("com_family_dtor:%+i:%+i;%+i;%+i;%+i;%c",
        ent,
        family_e->ancestor,
        family_e->children,
        family_e->siblingl,
        family_e->siblingr,
        '\n'
    );
    ecos_set_ancestor(ent);
    ecos_set_children(ent);
}

void ecos_init()
{
    auto&reg = ecos.reg;
    reg.on_construct<com_family_t>().connect<&ecos_com_family_ctor>();
    reg.on_destroy<com_family_t>().connect<&ecos_com_family_dtor>();
    auto null = reg.create();
}

/*** family ***/

bool ecos_vet_ancestor(ent_t ent, ent_t ancestor)
{
    auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    if (reg.valid(ancestor) == _FALSE) { return _FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    return (family_e->ancestor == ancestor);
}
bool ecos_vet_ancestry(ent_t ent, ent_t ancestry)
{
    auto&reg = ecos.reg;
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
bool ecos_set_ancestor(ent_t ent, ent_t ancestor)
{
    if (ent == ancestor) { return _FALSE; }
    if (ecos_vet_ancestor(ent, ancestor)) { return _FALSE; }
    if (ecos_vet_children(ent, ancestor)) { return _FALSE; }
    auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (reg.valid(family_e->ancestor))
    {
        auto*family_a =&reg.get<com_family_t>(family_e->ancestor);
        auto children = family_a->children;
        if (children == ent) { family_a->children = entt::null; }
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
        auto children = family_a->children;
        if (reg.valid(children))
        {
            auto siblingr = children;
            auto*family_r =&reg.get<com_family_t>(siblingr);
            while (reg.valid(family_r->siblingr))
            {
                siblingr = family_r->siblingr;
                family_r =&reg.get<com_family_t>(siblingr);
            }
            family_e->siblingl = siblingr;
            family_r->siblingr = ent;
        }
        else { family_a->children = ent; }
    }
    family_e->ancestor = ancestor;
    return _TRUTH;
}

bool ecos_vet_siblings(ent_t ent, ent_t siblings)
{ return ecos_vet_siblingl(ent, siblings) || ecos_vet_siblingr(siblings); }
bool ecos_vet_siblings(ent_t ent, ent_t siblingl, ent_t siblingr)
{ return ecos_vet_siblingl(ent, siblingl) && ecos_vet_siblingr(siblingr); }
bool ecos_vet_siblingl(ent_t ent, ent_t siblingl)
{
    auto&reg = ecos.reg;
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
    auto&reg = ecos.reg;
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
bool ecos_set_siblingl(ent_t ent, ent_t siblingl)
{
    if (ent == siblingl) { return _FALSE; }
    if (ecos_vet_siblingr(ent, siblingl)) { return _FALSE; }
    auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (reg.valid(family_e->siblingl))
    {
        auto*family_l =&reg.get<com_family_t>(family_e->siblingl);
        family_l->siblingr = siblingl;
    }
    auto*family_l =&reg.get<com_family_t>(siblingl);
    family_l->siblingr = ent;
    family_e->siblingl = siblingl;
    return _TRUTH;
}
bool ecos_set_siblingr(ent_t ent, ent_t siblingr)
{
    if (ent == siblingr) { return _FALSE; }
    if (ecos_vet_siblingl(ent, siblingr)) { return _FALSE; }
    auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (reg.valid(family_e->siblingr))
    {
        auto*family_r =&reg.get<com_family_t>(family_e->siblingr);
        family_r->siblingl = siblingr;
    }
    auto*family_r = &reg.get<com_family_t>(siblingr);
    family_r->siblingl = ent;
    family_e->siblingr = siblingr;
    return _TRUTH;
}
bool ecos_set_siblings(ent_t ent, ent_t siblingl, ent_t siblingr)
{
    if (ecos_vet_siblings(ent, siblingl, siblingr)) { return _FALSE; }
    return ecos_set_siblingl(siblingl) && ecos_set_siblingr(siblingr);
}

bool ecos_vet_children(ent_t ent, ent_t children)
{
    auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    if (reg.valid(children) == _FALSE) { return _FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (family_e->children == children) { return _TRUTH; }
    else { return ecos_vet_siblings(family_e->children, children); }
}
bool ecos_set_children(ent_t ent, ent_t children)
{
    if (ent == children) { return _FALSE; }
    if (ecos_vet_children(ent, children)) { return _FALSE; }
    if (ecos_vet_ancestry(ent, children)) { return _FALSE; }
    auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (reg.valid(family_e->children))
    {
        auto children = family_e->children;
        auto*family_c =&reg.get<com_family_t>(children);
        family_c->ancestor = entt::null;
        auto siblingl = family_c->siblingl;
        auto*family_l =&reg.get<com_family_t>(siblingl);
        while (reg.valid(family_l->siblingl))
        {
            family_l->siblingr = entt::null;
            family_l->ancestor = entt::null;
        }
        auto siblingr = family_c->siblingr;
        auto*family_r =&reg.get<com_family_t>(siblingr);
        while (reg.valid(family_r->siblingr))
        {
            family_r->siblingl = entt::null;
            family_r->ancestor = entt::null;
        }
    }
    family_e->children = children;
    if (reg.valid(children))
    {
        auto*family_c =&reg.get<com_family_t>(children);
        auto siblingr = family_c->siblingr;
        if (reg.valid(siblingr))
        {
            auto*family_r =&reg.get<com_family_t>(siblingr);
            if (reg.valid(family_c->siblingl))
            {
                family_r->siblingl = family_c->siblingl;
            }
            else
            {
                family_r->siblingl = children;
            }
            while (reg.valid(family_r->siblingl))
            {
                family_r->ancestor = ent;
                siblingr = family_r->siblingl;
                family_r =&reg.get<com_family_t>(siblingr);
            }
        }
        auto siblingl = family_c->siblingl;
        if (reg.valid(siblingl))
        {
            auto*family_l =&reg.get<com_family_t>(siblingl);
            while (reg.valid(family_l->siblingl))
            {
                family_l->ancestor = ent;
                siblingl = family_l->siblingl;
                family_l =&reg.get<com_family_t>(siblingl);
            }
            family_l->siblingl = children;
            family_c->siblingr = siblingl;
        }
    }
    return _TRUTH;
}
bool ecos_insert_child(ent_t ent, ent_t child)
{
    if (ent == child) { return _FALSE; }
    if (ecos_vet_children(ent, child) == _TRUTH) { return _FALSE; }
    auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    if (reg.valid(child) == _FALSE) { return _FALSE; }
    auto*family_c =&reg.get<com_family_t>(child);
    if (reg.valid(family_c->ancestor))
    {
        auto*family_a =&reg.get<com_family_t>(family_c->ancestor);
        if (family_a->children == child)
        {
            family_a->children = entt::null;
        }
    }
    family_c->ancestor = ent;
    if (reg.valid(family_c->siblingl))
    {
        reg.get<com_family_t>(family_c->siblingl).siblingr = family_c->siblingr;
        family_c->siblingl = entt::null;
    }
    if (reg.valid(family_c->siblingr))
    {
        reg.get<com_family_t>(family_c->siblingr).siblingl = family_c->siblingl;
        family_c->siblingr = entt::null;
    }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (reg.valid(family_e->children))
    {
        auto siblingr = family_e->children;
        auto*family_r =&reg.get<com_family_t>(siblingr);
        while (reg.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r =&reg.get<com_family_t>(siblingr);
        }
        family_r->siblingr = child;
        family_c->siblingl = siblingr;
    }
    else { family_e->children = child; }
    return _TRUTH;
}
bool ecos_remove_child(ent_t ent, ent_t child)
{
    if (ent == child) { return _FALSE; }
    if (ecos_vet_children(ent, child) == _FALSE) { return _FALSE; }
    auto&reg = ecos.reg;
    if (reg.valid(ent) == _FALSE) { return _FALSE; }
    if (reg.valid(child) == _FALSE) { return _FALSE; }
    auto*family_c =&reg.get<com_family_t>(child);
    if (reg.valid(family_c->siblingl))
    {
        reg.get<com_family_t>(family_c->siblingl).siblingr = family_c->siblingr;
        family_c->siblingl = entt::null;
    }
    if (reg.valid(family_c->siblingr))
    {
        reg.get<com_family_t>(family_c->siblingr).siblingl = family_c->siblingl;
        family_c->siblingr = entt::null;
    }
    auto*family_e =&reg.get<com_family_t>(ent);
    if (family_e->children == child) { family_e->children = entt::null; }
    return _TRUTH;
}

_NAMESPACE_LEAVE
