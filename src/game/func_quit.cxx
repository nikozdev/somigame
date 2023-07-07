#ifndef SOMIGAME_GAME_FUNC_QUIT_CXX
#define SOMIGAME_GAME_FUNC_QUIT_CXX

#include "../head.hxx"
#include "../game.hxx"
#include "../game/type_alive.hxx"
#include "../game/type_actor.hxx"
#include "../game/type_power.hxx"
#include "../game/type_sizes.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_rects.hxx"
#include "../game/unit_world.hxx"
#include "../game/unit_gamer.hxx"

namespace somigame { namespace game {

// signals

_DEFN_DATA signal_t<void(void)>quit_top_signal;
_DEFN_DATA signal_t<void(void)>quit_bot_signal;

// actions

_DEFN_FUNC bool quit()
{
    game::quit_top_signal.holder.publish();
    // units
    _EFNOT(game::quit_unit_gamer(), return FALSE, "game quit unit gamer failed");
    _EFNOT(game::quit_unit_world(), return FALSE, "game quit unit world failed");
    // tiles
    _EFNOT(game::quit_type_tcell(), return FALSE, "game quit type tcell failed");
    _EFNOT(game::quit_type_rects(), return FALSE, "game quit type rects failed");
    _EFNOT(game::quit_type_coord(), return FALSE, "game quit type coord failed");
    _EFNOT(game::quit_type_sizes(), return FALSE, "game quit type sizes failed");
    // logic
    _EFNOT(game::quit_type_power(), return FALSE, "game quit type power failed");
    _EFNOT(game::quit_type_actor(), return FALSE, "game quit type actor failed");
    _EFNOT(game::quit_type_alive(), return FALSE, "game quit type alive failed");
    // final
    game::quit_bot_signal.holder.publish();
    return TRUTH;
} // quit

} } // namespace somigame { namespace ecos {

#endif//SOMIGAME_GAME_FUNC_QUIT_CXX
