#ifndef SOMIGAME_FSYS_FUNC_QUIT_CXX
#define SOMIGAME_FSYS_FUNC_QUIT_CXX

#include "../head.hxx"
#include "../fsys.hxx"

namespace somigame { namespace fsys {

// signals

_DEFN_DATA signal_t<void(void)>quit_top_signal;
_DEFN_DATA signal_t<void(void)>quit_bot_signal;

// actions

_DEFN_FUNC bool quit()
{
    fsys::quit_top_signal.holder.publish();
    fsys::quit_bot_signal.holder.publish();
    return TRUTH;
} // quit

} } // namespace somigame { namespace fsys {

#endif//SOMIGAME_FSYS_FUNC_QUIT_CXX
