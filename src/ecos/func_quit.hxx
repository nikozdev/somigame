#ifndef SOMIGAME_ECOS_FUNC_QUIT_HXX
#define SOMIGAME_ECOS_FUNC_QUIT_HXX

#include "../head.hxx"

namespace somigame { namespace ecos {

// signals

_DECL_DATA signal_t<void(void)>quit_top_signal;
_DECL_DATA signal_t<void(void)>quit_bot_signal;

// actions

_DECL_FUNC bool quit();

} } // namespace somigame { namespace ecos {

#endif//SOMIGAME_ECOS_FUNC_QUIT_HXX
