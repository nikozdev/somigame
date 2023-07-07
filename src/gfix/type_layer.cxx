#ifndef SOMIGAME_GFIX_TYPE_LAYER_CXX
#define SOMIGAME_GFIX_TYPE_LAYER_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../gfix/type_layer.hxx"

namespace somigame { namespace gfix {

static void on_proc(ecos::reg_t&reg, ecos::ent_t ent)
{
    if (reg.valid(ent) == FALSE) { return; }
    // layer
    gfix::glayer_t glayer_e = { 0 };
    if (auto*rlayer_e = reg.try_get<gfix::com_rlayer_t>(ent))
    { glayer_e.layer = rlayer_e->layer; }
    if (auto*family_e = reg.try_get<ecos::com_family_t>(ent))
    { // family
        // ancestor
        auto ancestor = family_e->ancestor;
        if (reg.valid(ancestor))
        { // ancestor
            if (auto*glayer_a = reg.try_get<gfix::com_glayer_t>(ancestor))
            { glayer_e.layer += glayer_a->layer; }
        } // ancestor
        // update
        reg.emplace_or_replace<gfix::com_glayer_t>(ent, glayer_e);
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
    else
    { // update layer
        reg.emplace_or_replace<gfix::com_glayer_t>(ent, glayer_e);
    } // update layer
} // on_proc
static void on_ctor(ecos::reg_t&reg, ecos::ent_t ent)
{ return on_proc(reg, ent); }

// actions

_DEFN_FUNC v1b_t init_type_layer()
{
    // rlayer
    ecos::reg.on_ctor<gfix::com_rlayer_t>().connect<&on_ctor>();
    ecos::reg.on_proc<gfix::com_rlayer_t>().connect<&on_proc>();
    // family
    ecos::reg.on_ctor<ecos::com_family_t>().connect<&on_ctor>();
    ecos::reg.on_proc<ecos::com_family_t>().connect<&on_proc>();
    // final
    return TRUTH;
} // init_type_layer
_DEFN_FUNC v1b_t quit_type_layer()
{
    // rlayer
    ecos::reg.on_ctor<gfix::com_rlayer_t>().disconnect<&on_ctor>();
    ecos::reg.on_proc<gfix::com_rlayer_t>().disconnect<&on_proc>();
    // family
    ecos::reg.on_ctor<ecos::com_family_t>().disconnect<&on_ctor>();
    ecos::reg.on_proc<ecos::com_family_t>().disconnect<&on_proc>();
    // final
    return TRUTH;
} // quit_type_layer

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_LAYER_CXX
