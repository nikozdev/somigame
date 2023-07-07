#ifndef SOMIGAME_GEOM_TYPE_COORD_CXX
#define SOMIGAME_GEOM_TYPE_COORD_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"

namespace somigame { namespace geom {

// actions

static void proc_gposi(ecos::reg_t&reg, ecos::ent_t ent, gposi_t&gposi)
{
    if (geom::gplim_t*gplim = reg.try_get<geom::com_gplim_t>(ent))
    {
        gposi.x = std::clamp(gposi.x, gplim->x.l, gplim->x.r);
        gposi.y = std::clamp(gposi.y, gplim->y.b, gplim->y.t);
    }
    reg.emplace_or_replace<geom::com_gposi_t>(ent, gposi);
}
static void on_proc(ecos::reg_t&reg, ecos::ent_t ent)
{
    // coord
    geom::gposi_t gposi_e = { 0, 0 };
    if (auto*aposi_e = reg.try_get<geom::com_aposi_t>(ent))
    { gposi_e.x = aposi_e->x; gposi_e.y = aposi_e->y; }
    // family
    if (auto*family_e = reg.try_get<ecos::com_family_t>(ent))
    { // family
      // ancestor
        auto ancestor = family_e->ancestor;
        if (reg.valid(ancestor))
        { // ancestor
            if (auto*gsize_a = reg.try_get<geom::com_gsize_t>(ancestor))
            { // ancestor global sizes
                if (auto*rposi_e = reg.try_get<geom::com_rposi_t>(ent))
                { // relative sizes
                    gposi_e.x += gsize_a->x * rposi_e->x / RPOSI_DIV;
                    gposi_e.y += gsize_a->y * rposi_e->y / RPOSI_DIV;
                } // relative sizes
                if (auto*pivot_a = reg.try_get<geom::com_pivot_t>(ancestor))
                { // pivot
                    gposi_e.x -= gsize_a->x * pivot_a->x / PIVOT_DIV;
                    gposi_e.y -= gsize_a->y * pivot_a->y / PIVOT_DIV;
                } // pivot
            } // ancestor global sizes
            if (auto*gposi_a = reg.try_get<geom::com_gposi_t>(ancestor))
            { // ancestor global coord
                gposi_e.x += gposi_a->x;
                gposi_e.y += gposi_a->y;
            } // ancestor global coord
        } // ancestor
        // update coord
        proc_gposi(reg, ent, gposi_e);
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
    { // update coord
        proc_gposi(reg, ent, gposi_e);
    } // update coord
} // on_proc
static void on_proc_aposi(ecos::reg_t&reg, ecos::ent_t ent)
{
    geom::aposi_t&aposi = reg.get<geom::com_aposi_t>(ent);
    if (geom::aplim_t*aplim = reg.try_get<geom::com_aplim_t>(ent))
    {
        aposi.x = std::clamp(aposi.x, aplim->x.l, aplim->x.r);
        aposi.y = std::clamp(aposi.y, aplim->y.b, aplim->y.t);
    }
    return on_proc(reg, ent);
}
static void on_proc_rposi(ecos::reg_t&reg, ecos::ent_t ent)
{
    geom::rposi_t&rposi = reg.get<geom::com_rposi_t>(ent);
    if (geom::rplim_t*rplim = reg.try_get<geom::com_rplim_t>(ent))
    {
        rposi.x = std::clamp(rposi.x, rplim->x.l, rplim->x.r);
        rposi.y = std::clamp(rposi.y, rplim->y.b, rplim->y.t);
    }
    return on_proc(reg, ent);
}
static void on_ctor(ecos::reg_t&reg, ecos::ent_t ent)
{ return on_proc(reg, ent); }
static void on_ctor_aposi(ecos::reg_t&reg, ecos::ent_t ent)
{ return on_proc_aposi(reg, ent); }
static void on_ctor_rposi(ecos::reg_t&reg, ecos::ent_t ent)
{ return on_proc_rposi(reg, ent); }

_DEFN_FUNC v1b_t init_type_coord()
{
    // aposi
    ecos::reg.on_ctor<geom::com_aposi_t>().connect<&on_ctor_aposi>();
    ecos::reg.on_proc<geom::com_aposi_t>().connect<&on_proc_aposi>();
    // rposi
    ecos::reg.on_ctor<geom::com_rposi_t>().connect<&on_ctor_rposi>();
    ecos::reg.on_proc<geom::com_rposi_t>().connect<&on_proc_rposi>();
    // family
    ecos::reg.on_ctor<ecos::com_family_t>().connect<&on_ctor>();
    ecos::reg.on_proc<ecos::com_family_t>().connect<&on_proc>();
    // final
    return TRUTH;
} // init_type_coord
_DEFN_FUNC v1b_t quit_type_coord()
{
    // aposi
    ecos::reg.on_ctor<geom::com_aposi_t>().disconnect<&on_ctor>();
    ecos::reg.on_proc<geom::com_aposi_t>().disconnect<&on_proc>();
    // rposi
    ecos::reg.on_ctor<geom::com_rposi_t>().disconnect<&on_ctor>();
    ecos::reg.on_proc<geom::com_rposi_t>().disconnect<&on_proc>();
    // family
    ecos::reg.on_ctor<ecos::com_family_t>().disconnect<&on_ctor>();
    ecos::reg.on_proc<ecos::com_family_t>().disconnect<&on_proc>();
    // final
    return TRUTH;
} // quit_type_coord

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_TYPE_COORD_CXX
