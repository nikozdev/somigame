#ifndef SOMIGAME_MAIN_FUNC_WORK_HXX
#define SOMIGAME_MAIN_FUNC_WORK_HXX

#include "../head.hxx"

namespace somigame { namespace main {

// signals

_DECL_DATA signal_t<void(void)>work_top_signal;
_DECL_DATA signal_t<void(void)>work_bot_signal;

// actions

_DECL_FUNC bool work();

} } // namespace somigame { namespace main {

#endif//SOMIGAME_MAIN_FUNC_WORK_HXX
