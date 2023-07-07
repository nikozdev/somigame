#ifndef SOMIGAME_GFIX_FUNC_WORK_HXX
#define SOMIGAME_GFIX_FUNC_WORK_HXX

#include "../head.hxx"

namespace somigame { namespace gfix {

// signals

_DECL_DATA signal_t<void(void)>work_top_signal;
_DECL_DATA signal_t<void(void)>work_bot_signal;

// actions

_DECL_FUNC bool work();

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_FUNC_WORK_HXX
