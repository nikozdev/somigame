#ifndef SOMIGAME_MAIN_FUNC_WORK_CXX
#define SOMIGAME_MAIN_FUNC_WORK_CXX

#include "../head.hxx"
#include "../main.hxx"

namespace somigame { namespace main {

// signals

_DEFN_DATA signal_t<void(void)>work_top_signal;
_DEFN_DATA signal_t<void(void)>work_bot_signal;

// actions

_DEFN_FUNC bool work()
{
    main::work_top_signal.holder.publish();
    main::proc_unit_ticker();
    ::glutPostRedisplay();
    main::work_bot_signal.holder.publish();
    return TRUTH;
} // work

} } // namespace somigame { namespace main {

#endif//SOMIGAME_MAIN_FUNC_WORK_CXX
