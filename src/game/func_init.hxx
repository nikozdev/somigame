#ifndef SOMIGAME_GAME_FUNC_INIT_HXX
#define SOMIGAME_GAME_FUNC_INIT_HXX

#include "../head.hxx"

namespace somigame { namespace game {

// signals

_DECL_DATA signal_t<void(void)>init_top_signal;
_DECL_DATA signal_t<void(void)>init_bot_signal;

// actions

_DECL_FUNC bool init();

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_FUNC_INIT_HXX
