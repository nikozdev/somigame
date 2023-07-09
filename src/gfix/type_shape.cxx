#ifndef SOMIGAME_GFIX_TYPE_SHAPE_CXX
#define SOMIGAME_GFIX_TYPE_SHAPE_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../gfix/type_shape.hxx"

namespace somigame { namespace gfix {

// actions

static void on_proc(ecos::reg_t&reg, ecos::ent_t ent)
{
    if (auto*family_e = reg.try_get<ecos::com_family_t>(ent))
    { // family
        // ancestor
        auto ancestor = family_e->ancestor;
        if (reg.valid(ancestor))
        { // ancestor
        } // ancestor
        // followers
        if (reg.valid(family_e->follower))
        { // update followers
            auto follower = family_e->follower;
            auto*family_f =&reg.get<ecos::com_family_t>(family_e->follower);
            on_proc(reg, follower);
            // siblingl
            auto siblingl = follower;
            auto*family_l = family_f;
            while (reg.valid(family_l->siblingl))
            {
                siblingl = family_l->siblingl;
                family_l =&reg.get<ecos::com_family_t>(siblingl);
                on_proc(reg, siblingl);
            }
            // siblingr
            auto siblingr = follower;
            auto*family_r = family_f;
            while (reg.valid(family_r->siblingr))
            {
                siblingr = family_r->siblingr;
                family_r =&reg.get<ecos::com_family_t>(siblingr);
                on_proc(reg, siblingr);
            }
        } // update followers
    } // family
} // on_proc
static void on_ctor(ecos::reg_t&reg, ecos::ent_t ent)
{ return on_proc(reg, ent); }

_DEFN_FUNC v1b_t init_type_shape()
{
    // gposi
    ecos::reg.on_ctor<gfix::com_lgrid_t>().connect<&on_ctor>();
    ecos::reg.on_proc<gfix::com_lgrid_t>().connect<&on_proc>();
    // family
    ecos::reg.on_ctor<ecos::com_family_t>().connect<&on_ctor>();
    ecos::reg.on_proc<ecos::com_family_t>().connect<&on_proc>();
    // final
    return TRUTH;
} // init_type_shape
_DEFN_FUNC v1b_t quit_type_shape()
{
    // gposi
    ecos::reg.on_ctor<gfix::com_lgrid_t>().disconnect<&on_ctor>();
    ecos::reg.on_proc<gfix::com_lgrid_t>().disconnect<&on_proc>();
    // family
    ecos::reg.on_ctor<ecos::com_family_t>().disconnect<&on_ctor>();
    ecos::reg.on_proc<ecos::com_family_t>().disconnect<&on_proc>();
    // final
    return TRUTH;
} // quit_type_shape

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_SHAPE_CXX
