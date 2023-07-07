#ifndef SOMIGAME_GFIX_FUNC_QUIT_HXX
#define SOMIGAME_GFIX_FUNC_QUIT_HXX

#include "../head.hxx"

namespace somigame { namespace gfix {

// signals

_DECL_DATA signal_t<void(void)>quit_top_signal;
_DECL_DATA signal_t<void(void)>quit_bot_signal;

// actions

_DECL_FUNC bool quit();

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_FUNC_QUIT_HXX
