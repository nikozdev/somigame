#ifndef SOMIGAME_GFIX_FUNC_INIT_HXX
#define SOMIGAME_GFIX_FUNC_INIT_HXX

#include "../head.hxx"

namespace somigame { namespace gfix {

// signals

_DECL_DATA signal_t<void(void)>init_top_signal;
_DECL_DATA signal_t<void(void)>init_bot_signal;

// actions

_DECL_FUNC bool init();

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_FUNC_INIT_HXX
