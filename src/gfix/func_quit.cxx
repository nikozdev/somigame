#ifndef SOMIGAME_GFIX_FUNC_QUIT_CXX
#define SOMIGAME_GFIX_FUNC_QUIT_CXX

#include "../head.hxx"
#include "../gfix.hxx"

namespace somigame { namespace gfix {

// signals

_DEFN_DATA signal_t<void(void)>quit_top_signal;
_DEFN_DATA signal_t<void(void)>quit_bot_signal;

// actions

_DEFN_FUNC bool quit()
{
    gfix::quit_top_signal.holder.publish();
    // modules
    _EFNOT(gfix::quit_unit_guiman(), return FALSE, "quit unit guiman failed");
    _EFNOT(gfix::quit_unit_camera(), return FALSE, "quit unit camera failed");
    _EFNOT(gfix::quit_type_visual(), return FALSE, "quit type visual failed");
    _EFNOT(gfix::quit_type_fonts(), return FALSE, "quit type fonts failed");
    _EFNOT(gfix::quit_type_label(), return FALSE, "quit type label failed");
    _EFNOT(gfix::quit_type_image(), return FALSE, "quit type image failed");
    _EFNOT(gfix::quit_type_layer(), return FALSE, "quit type layer failed");
    // final
    gfix::quit_bot_signal.holder.publish();
    return TRUTH;
} // quit

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_FUNC_QUIT_CXX
