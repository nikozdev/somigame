#ifndef SOMIGAME_GFIX_FUNC_QUIT_CXX
#define SOMIGAME_GFIX_FUNC_QUIT_CXX

#include "../head.hxx"
#include "../gfix/type_drawn.hxx"
#include "../gfix/type_layer.hxx"
#include "../gfix/type_image.hxx"
#include "../gfix/type_label.hxx"
#include "../gfix/type_fonts.hxx"
#include "../gfix/type_antor.hxx"
#include "../gfix/type_shape.hxx"
#include "../gfix/unit_guiman.hxx"
#include "../gfix/unit_camera.hxx"
#include "../gfix/func_init.hxx"

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
    _EFNOT(gfix::quit_type_shape(), return FALSE, "quit type shape failed");
    _EFNOT(gfix::quit_type_fonts(), return FALSE, "quit type fonts failed");
    _EFNOT(gfix::quit_type_label(), return FALSE, "quit type label failed");
    _EFNOT(gfix::quit_type_image(), return FALSE, "quit type image failed");
    _EFNOT(gfix::quit_type_layer(), return FALSE, "quit type layer failed");
    _EFNOT(gfix::quit_type_drawn(), return FALSE, "quit type drawn failed");
    // final
    gfix::quit_bot_signal.holder.publish();
    return TRUTH;
} // quit

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_FUNC_QUIT_CXX
