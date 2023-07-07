#ifndef SOMIGAME_GEOM_FUNC_INIT_HXX
#define SOMIGAME_GEOM_FUNC_INIT_HXX

#include "../head.hxx"

namespace somigame { namespace geom {

// signals

_DECL_DATA signal_t<void(void)>init_top_signal;
_DECL_DATA signal_t<void(void)>init_bot_signal;

// actions

_DECL_FUNC bool init();

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_FUNC_INIT_HXX
