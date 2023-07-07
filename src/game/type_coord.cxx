#ifndef SOMIGAME_GAME_TYPE_COORD_CXX
#define SOMIGAME_GAME_TYPE_COORD_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../geom/type_coord.hxx"
#include "../game/type_coord.hxx"

namespace somigame { namespace game {

// actions

static void on_proc(ecos::reg_t&reg, ecos::ent_t ent)
{
    reg.emplace_or_replace<game::com_tposi_t>(ent,
        game::get_tposi_from_gposi(reg.get<geom::com_gposi_t>(ent))
    );
} // on_proc
static void on_ctor(ecos::reg_t&reg, ecos::ent_t ent)
{ return on_proc(reg, ent); }
static void on_dtor(ecos::reg_t&reg, ecos::ent_t ent)
{ reg.remove<game::com_tposi_t>(ent); }

_DEFN_FUNC v1b_t init_type_coord()
{
    // gposi
    ecos::reg.on_ctor<geom::com_gposi_t>().connect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_gposi_t>().connect<&on_dtor>();
    ecos::reg.on_proc<geom::com_gposi_t>().connect<&on_proc>();
    // final
    return TRUTH;
} // init_type_coord
_DEFN_FUNC v1b_t quit_type_coord()
{
    // gposi
    ecos::reg.on_ctor<geom::com_gposi_t>().disconnect<&on_ctor>();
    ecos::reg.on_dtor<geom::com_gposi_t>().disconnect<&on_dtor>();
    ecos::reg.on_proc<geom::com_gposi_t>().disconnect<&on_proc>();
    // final
    return TRUTH;
} // quit_type_coord

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_TYPE_COORD_CXX
