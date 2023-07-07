#ifndef SOMIGAME_MAIN_FUNC_INIT_HXX
#define SOMIGAME_MAIN_FUNC_INIT_HXX

#include "../head.hxx"

namespace somigame { namespace main {

// signals

_DECL_DATA signal_t<void(void)>init_top_signal;
_DECL_DATA signal_t<void(void)>init_bot_signal;

// actions

_DECL_FUNC bool init();

} } // namespace somigame { namespace main {

#endif//SOMIGAME_MAIN_FUNC_INIT_HXX
