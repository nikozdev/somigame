#ifndef SOMIGAME_GEOM_TYPE_RECTS_CXX
#define SOMIGAME_GEOM_TYPE_RECTS_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_rects.hxx"

namespace somigame { namespace geom {

// actions

static void on_proc(ecos::reg_t&reg, ecos::ent_t ent)
{
    // sizes
    geom::gsize_t gsize_e = { 0, 0 };
    if (auto*gsize = reg.try_get<geom::com_gsize_t>(ent))
    { gsize_e =*gsize; }
    // coord
    geom::gposi_t gposi_e = { 0, 0 };
    if (auto*gposi = reg.try_get<geom::com_gposi_t>(ent))
    { gposi_e =*gposi; }
    // pivot
    geom::pivot_t pivot_e = { PIVOT_MID, PIVOT_MID };
    if (auto*pivot = reg.try_get<geom::com_pivot_t>(ent))
    { pivot_e =*pivot; }
    // grect
    geom::grect_t grect_e = {
        .x = { gposi_e.x, gposi_e.x },
        .y = { gposi_e.y, gposi_e.y },
    };
    grect_e.x.l += gsize_e.x * (PIVOT_MIN - pivot_e.x) / PIVOT_DIV;
    grect_e.x.r += gsize_e.x * (PIVOT_MAX - pivot_e.x) / PIVOT_DIV;
    grect_e.y.b += gsize_e.y * (PIVOT_MIN - pivot_e.y) / PIVOT_DIV;
    grect_e.y.t += gsize_e.y * (PIVOT_MAX - pivot_e.y) / PIVOT_DIV;
    // update
    reg.emplace_or_replace<geom::com_grect_t>(ent, grect_e);
} // on_proc
static void on_ctor(ecos::reg_t&reg, ecos::ent_t ent)
{ return on_proc(reg, ent); }
static void on_dtor(ecos::reg_t&reg, ecos::ent_t ent)
{}

_DEFN_FUNC v1b_t init_type_rects()
{
    // sizes
    ecos::reg.on_ctor<geom::com_gsize_t>().connect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_gsize_t>().connect<&on_dtor>();
    ecos::reg.on_proc<geom::com_gsize_t>().connect<&on_proc>();
    // coord
    ecos::reg.on_ctor<geom::com_gposi_t>().connect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_gposi_t>().connect<&on_dtor>();
    ecos::reg.on_proc<geom::com_gposi_t>().connect<&on_proc>();
    // pivot
    ecos::reg.on_ctor<geom::com_pivot_t>().connect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_pivot_t>().connect<&on_dtor>();
    ecos::reg.on_proc<geom::com_pivot_t>().connect<&on_proc>();
    // final
    return TRUTH;
} // init_type_rects
_DEFN_FUNC v1b_t quit_type_rects()
{
    // sizes
    ecos::reg.on_ctor<geom::com_gsize_t>().disconnect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_gsize_t>().disconnect<&on_dtor>();
    ecos::reg.on_proc<geom::com_gsize_t>().disconnect<&on_proc>();
    // coord
    ecos::reg.on_ctor<geom::com_gposi_t>().disconnect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_gposi_t>().disconnect<&on_dtor>();
    ecos::reg.on_proc<geom::com_gposi_t>().disconnect<&on_proc>();
    // pivot
    ecos::reg.on_ctor<geom::com_pivot_t>().disconnect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_pivot_t>().disconnect<&on_dtor>();
    ecos::reg.on_proc<geom::com_pivot_t>().disconnect<&on_proc>();
    // final
    return TRUTH;
} // quit_type_rects

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_TYPE_RECTS_CXX
