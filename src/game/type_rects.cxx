#ifndef SOMIGAME_GAME_TYPE_RECTS_CXX
#define SOMIGAME_GAME_TYPE_RECTS_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../game/type_sizes.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_rects.hxx"

namespace somigame { namespace game {

// actions

static void on_proc(ecos::reg_t&reg, ecos::ent_t ent)
{
    reg.emplace_or_replace<game::com_trect_t>(ent,
        game::get_trect_from_grect(reg.get<geom::com_grect_t>(ent))
    );
}
static void on_ctor(ecos::reg_t&reg, ecos::ent_t ent)
{ return on_proc(reg, ent); }
static void on_dtor(ecos::reg_t&reg, ecos::ent_t ent)
{}

_DEFN_FUNC v1b_t init_type_rects()
{
    // grect
    ecos::reg.on_ctor<geom::com_grect_t>().connect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_grect_t>().connect<&on_dtor>();
    ecos::reg.on_proc<geom::com_grect_t>().connect<&on_proc>();
    // final
    return TRUTH;
} // init_type_rects
_DEFN_FUNC v1b_t quit_type_rects()
{
    // grect
    ecos::reg.on_ctor<geom::com_grect_t>().disconnect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_grect_t>().disconnect<&on_dtor>();
    ecos::reg.on_proc<geom::com_grect_t>().disconnect<&on_proc>();
    // final
    return TRUTH;
} // quit_type_rects

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_TYPE_RECTS_CXX
