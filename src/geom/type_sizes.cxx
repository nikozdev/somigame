#ifndef SOMIGAME_GEOM_TYPE_SIZES_CXX
#define SOMIGAME_GEOM_TYPE_SIZES_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../geom/type_sizes.hxx"

namespace somigame { namespace geom {

// actions

static void on_proc_gsize(ecos::reg_t&reg, ecos::ent_t ent, geom::gsize_t&gsize)
{
    if (auto*lsize = reg.try_get<geom::com_lsize_t>(ent))
    {
        gsize.x = std::clamp(gsize.x, lsize->x.l, lsize->x.r);
        gsize.y = std::clamp(gsize.y, lsize->y.b, lsize->y.t);
    }
    reg.emplace_or_replace<geom::com_gsize_t>(ent, gsize);
}
static void on_proc(ecos::reg_t&reg, ecos::ent_t ent)
{
    // sizes
    geom::gsize_t gsize_e = { 0, 0 };
    if (auto*asize_e = reg.try_get<geom::com_asize_t>(ent))
    { gsize_e.x = asize_e->x; gsize_e.y = asize_e->y; }
    if (auto*family_e = reg.try_get<ecos::com_family_t>(ent))
    { // family
        auto ancestor = family_e->ancestor;
        if (reg.valid(ancestor))
        { // ancestor
            if (auto*gsize_a = reg.try_get<geom::com_gsize_t>(ancestor))
            { // ancestor global size
                if (auto*rsize_e = reg.try_get<geom::com_rsize_t>(ent))
                { // relative size
                    gsize_e.x += gsize_a->x * rsize_e->x / RSIZE_DIV;
                    gsize_e.y += gsize_a->y * rsize_e->y / RSIZE_DIV;
                } // relative size
            } // ancestor global size
        } // ancestor
        // sizes
        on_proc_gsize(reg, ent, gsize_e);
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
        } // update family
    } // family
    else
    { // update sizes
        on_proc_gsize(reg, ent, gsize_e);
    } // update sizes
} // on_proc
static void on_ctor(ecos::reg_t&reg, ecos::ent_t ent)
{ return on_proc(reg, ent); }
static void on_dtor(ecos::reg_t&reg, ecos::ent_t ent)
{}

_DEFN_FUNC v1b_t init_type_sizes()
{
    // asize
    ecos::reg.on_ctor<geom::com_asize_t>().connect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_asize_t>().connect<&on_dtor>();
    ecos::reg.on_proc<geom::com_asize_t>().connect<&on_proc>();
    // rsize
    ecos::reg.on_ctor<geom::com_rsize_t>().connect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_rsize_t>().connect<&on_dtor>();
    ecos::reg.on_proc<geom::com_rsize_t>().connect<&on_proc>();
    // ratio
    ecos::reg.on_ctor<geom::com_ratio_t>().connect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_ratio_t>().connect<&on_dtor>();
    ecos::reg.on_proc<geom::com_ratio_t>().connect<&on_proc>();
    // family
    ecos::reg.on_ctor<ecos::com_family_t>().connect<&on_ctor>();
    ecos::reg.on_dtor<ecos::com_family_t>().connect<&on_dtor>();
    ecos::reg.on_proc<ecos::com_family_t>().connect<&on_proc>();
    // final
    return TRUTH;
} // init_type_sizes
_DEFN_FUNC v1b_t quit_type_sizes()
{
    // asize
    ecos::reg.on_ctor<geom::com_asize_t>().disconnect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_asize_t>().disconnect<&on_dtor>();
    ecos::reg.on_proc<geom::com_asize_t>().disconnect<&on_proc>();
    // rsize
    ecos::reg.on_ctor<geom::com_rsize_t>().disconnect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_rsize_t>().disconnect<&on_dtor>();
    ecos::reg.on_proc<geom::com_rsize_t>().disconnect<&on_proc>();
    // ratio
    ecos::reg.on_ctor<geom::com_ratio_t>().disconnect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_ratio_t>().disconnect<&on_dtor>();
    ecos::reg.on_proc<geom::com_ratio_t>().disconnect<&on_proc>();
    // family
    ecos::reg.on_ctor<ecos::com_family_t>().disconnect<&on_ctor>();
    ecos::reg.on_dtor<ecos::com_family_t>().disconnect<&on_dtor>();
    ecos::reg.on_proc<ecos::com_family_t>().disconnect<&on_proc>();
    // final
    return TRUTH;
} // quit_type_sizes

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_TYPE_SIZES_CXX
