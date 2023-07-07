#ifndef SOMIGAME_GEOM_TYPE_MOVER_CXX
#define SOMIGAME_GEOM_TYPE_MOVER_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_mover.hxx"
#include "../game/type_sizes.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_rects.hxx"
#include "../game/type_tcell.hxx"

namespace somigame { namespace geom {

_DEFN_FUNC bool init_type_mover()
{
    entt::sigh_helper{ ecos::reg }
    .with<geom::com_mover_t>().on_update<[](ecos::reg_t&reg, ecos::ent_t ent){
        auto&mover = reg.get<geom::com_mover_t>(ent);
        auto&aposi = reg.get<geom::com_aposi_t>(ent);
        aposi.x += mover.move.x * game::TCELL_ASIZE_X;
        aposi.y += mover.move.y * game::TCELL_ASIZE_Y;
        reg.replace<geom::com_aposi_t>(ent, aposi.x, aposi.y);
        mover.move.x = 0;
        mover.move.y = 0;
    }>().on_construct<[](ecos::reg_t&reg, ecos::ent_t ent){
    }>().on_destroy<[](ecos::reg_t&reg, ecos::ent_t ent){
    }>();
    return TRUTH;
} // init_type_mover

_DEFN_FUNC bool quit_type_mover()
{
    return TRUTH;
} // quit_type_mover

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_geom_TYPE_MOVER_CXX
