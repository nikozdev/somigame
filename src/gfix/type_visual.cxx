#ifndef SOMIGAME_GFIX_TYPE_VISUAL_CXX
#define SOMIGAME_GFIX_TYPE_VISUAL_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../gfix/type_visual.hxx"

namespace somigame { namespace gfix {

// typedef

v1s_t gfix::visual_t::set(v1s_t value)
{
#if 0
    auto vsign = nums::get_sign(value);
    auto tsign = nums::get_sign(this->value);
    if ( vsign == tsign )
    { this->value = value; }
    else
    {
        if ( (value * vsign) > (this->value * tsign) )
        { this->value = value; }
    }
#endif
    if ( std::abs(this->value) <= std::abs(value) )
    { this->value = value; }
    return this->value;
} // visual_t::set

// actions

static void on_proc(ecos::reg_t&reg, ecos::ent_t ent)
{
    // visual
    gfix::visual_t visual_v;
    gfix::visual_t*visual_e =&visual_v;
    if (auto*pcomp = reg.try_get<com_visual_t>(ent))
    { visual_e = pcomp; }
    if (auto*family_e = reg.try_get<ecos::com_family_t>(ent))
    { // family
        // ancestor
        auto ancestor = family_e->ancestor;
        if (reg.valid(ancestor))
        { // ancestor
            if (auto*visual_a = reg.try_get<gfix::com_visual_t>(ancestor))
            { visual_e->set(visual_a->get()); }
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

_DEFN_FUNC v1b_t init_type_visual()
{
    // gposi
    ecos::reg.on_ctor<gfix::com_visual_t>().connect<&on_ctor>();
    ecos::reg.on_proc<gfix::com_visual_t>().connect<&on_proc>();
    // family
    ecos::reg.on_ctor<ecos::com_family_t>().connect<&on_ctor>();
    ecos::reg.on_proc<ecos::com_family_t>().connect<&on_proc>();
    // final
    return TRUTH;
} // init_type_visual
_DEFN_FUNC v1b_t quit_type_visual()
{
    // gposi
    ecos::reg.on_ctor<gfix::com_visual_t>().disconnect<&on_ctor>();
    ecos::reg.on_proc<gfix::com_visual_t>().disconnect<&on_proc>();
    // family
    ecos::reg.on_ctor<ecos::com_family_t>().disconnect<&on_ctor>();
    ecos::reg.on_proc<ecos::com_family_t>().disconnect<&on_proc>();
    // final
    return TRUTH;
} // quit_type_visual

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_VISUAL_CXX
