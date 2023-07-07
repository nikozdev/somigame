#ifndef SOMIGAME_FSYS_FUNC_INIT_CXX
#define SOMIGAME_FSYS_FUNC_INIT_CXX

#include "../head.hxx"
#include "../fsys.hxx"

namespace somigame { namespace fsys {

// signals

_DEFN_DATA signal_t<void(void)>init_top_signal;
_DEFN_DATA signal_t<void(void)>init_bot_signal;

// actions

_DEFN_FUNC bool init()
{
    fsys::init_top_signal.holder.publish();
    fsys::init_bot_signal.holder.publish();
    return TRUTH;
} // init

} } // namespace somigame { namespace fsys {

#endif//SOMIGAME_FSYS_FUNC_INIT_CXX
