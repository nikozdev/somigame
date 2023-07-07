#ifndef SOMIGAME_GEOM_FUNC_QUIT_HXX
#define SOMIGAME_GEOM_FUNC_QUIT_HXX

#include "../head.hxx"

namespace somigame { namespace geom {

// signals

_DECL_DATA signal_t<void(void)>quit_top_signal;
_DECL_DATA signal_t<void(void)>quit_bot_signal;

_DECL_FUNC bool quit();

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_FUNC_QUIT_HXX
