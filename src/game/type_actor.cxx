#ifndef SOMIGAME_GAME_TYPE_ACTOR_CXX
#define SOMIGAME_GAME_TYPE_ACTOR_CXX

#include "../head.hxx"
#include "../ecos.hxx"
#include "../game.hxx"
#include "../main.hxx"

namespace somigame { namespace game {

_DEFN_FUNC v1b_t init_type_actor()
{
    /* alive component signals */
    entt::sigh_helper{ ecos::reg }
    .with<game::com_alive_t>().on_update<[](ecos::reg_t&reg, ecos::ent_t ent){
        auto&alive = reg.get<game::com_alive_t>(ent);
        alive.timep = main::get_ticker().now_mil;
    }>().on_construct<[](ecos::reg_t&reg, ecos::ent_t ent){
    }>().on_destroy<[](ecos::reg_t&reg, ecos::ent_t ent){
    }>();
    return TRUTH;
}
_DEFN_FUNC v1b_t quit_type_actor()
{
    return TRUTH;
}

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_TYPE_ACTOR_CXX
