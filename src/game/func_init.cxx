#ifndef SOMIGAME_GAME_FUNC_INIT_CXX
#define SOMIGAME_GAME_FUNC_INIT_CXX

#include "../head.hxx"
#include "../game/type_alive.hxx"
#include "../game/type_actor.hxx"
#include "../game/type_power.hxx"
#include "../game/type_sizes.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_rects.hxx"
#include "../game/type_tcell.hxx"
#include "../game/unit_world.hxx"
#include "../game/unit_gamer.hxx"

namespace somigame { namespace game {

// signals

_DEFN_DATA signal_t<void(void)>init_top_signal;
_DEFN_DATA signal_t<void(void)>init_bot_signal;

// actions

_DEFN_FUNC bool init()
{
    game::init_top_signal.holder.publish();
    // logic
    _EFNOT(game::init_type_alive(), return FALSE, "game init type alive failed");
    _EFNOT(game::init_type_actor(), return FALSE, "game init type actor failed");
    _EFNOT(game::init_type_power(), return FALSE, "game init type power failed");
    // tiles
    _EFNOT(game::init_type_sizes(), return FALSE, "game init type sizes failed");
    _EFNOT(game::init_type_coord(), return FALSE, "game init type coord failed");
    _EFNOT(game::init_type_rects(), return FALSE, "game init type rects failed");
    _EFNOT(game::init_type_tcell(), return FALSE, "game init type tcell failed");
    // units
    _EFNOT(game::init_unit_world(), return FALSE, "game init unit world failed");
    _EFNOT(game::init_unit_gamer(), return FALSE, "game init unit gamer failed");
    game::init_bot_signal.holder.publish();
    return TRUTH;
} // init

} } // namespace somigame { namespace ecos {

#endif//SOMIGAME_GAME_FUNC_INIT_CXX
