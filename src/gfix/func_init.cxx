#ifndef SOMIGAME_GFIX_FUNC_INIT_CXX
#define SOMIGAME_GFIX_FUNC_INIT_CXX

#include "../head.hxx"
#include "../gfix.hxx"

namespace somigame { namespace gfix {

// signals

_DEFN_DATA signal_t<void(void)>init_top_signal;
_DEFN_DATA signal_t<void(void)>init_bot_signal;

// actions

_DEFN_FUNC bool init()
{
    gfix::init_top_signal.holder.publish();
    // opengl
    _ELOG("[opengl-version] {:s}\n", (char*)::glGetString(GL_VERSION));
    ::glDisable(GL_DEPTH_TEST);
    ::glDisable(GL_STENCIL_TEST);
    ::glEnable(GL_TEXTURE_1D);
    ::glEnable(GL_TEXTURE_2D);
    ::glDisable(GL_TEXTURE_3D);
    ::glEnable(GL_BLEND);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef _CONF_WORK
    ::glEnable(GL_CULL_FACE);
    ::glCullFace(GL_BACK);
    ::glFrontFace(GL_CCW);
#else
    ::glDisable(GL_CULL_FACE);
#endif
    ::glShadeModel(GL_FLAT);
    // modules
    _EFNOT(gfix::init_type_layer(), return FALSE, "init type layer failed");
    _EFNOT(gfix::init_type_image(), return FALSE, "init type image failed");
    _EFNOT(gfix::init_type_label(), return FALSE, "init type label failed");
    _EFNOT(gfix::init_type_fonts(), return FALSE, "init type fonts failed");
    _EFNOT(gfix::init_type_visual(), return FALSE, "init type visual failed");
    _EFNOT(gfix::init_unit_camera(), return FALSE, "init unit camera failed");
    _EFNOT(gfix::init_unit_guiman(), return FALSE, "init unit guiman failed");
    // final
    gfix::init_bot_signal.holder.publish();
    return TRUTH;
} // init

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_FUNC_INIT_CXX
