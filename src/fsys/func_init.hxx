#ifndef SOMIGAME_FSYS_FUNC_INIT_HXX
#define SOMIGAME_FSYS_FUNC_INIT_HXX

#include "../head.hxx"

namespace somigame { namespace fsys {

// signals

_DECL_DATA signal_t<void(void)>init_top_signal;
_DECL_DATA signal_t<void(void)>init_bot_signal;

// actions

_DECL_FUNC bool init();

} } // namespace somigame { namespace fsys {

#endif//SOMIGAME_FSYS_FUNC_INIT_HXX
