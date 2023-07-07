#ifndef SOMIGAME_MAIN_FUNC_QUIT_CXX
#define SOMIGAME_MAIN_FUNC_QUIT_CXX

#include "../head.hxx"
#include "../ecos/func_quit.hxx"
#include "../fsys/func_quit.hxx"
#include "../geom/func_quit.hxx"
#include "../gfix/func_quit.hxx"
#include "../game/func_quit.hxx"
#include "../main/unit_window.hxx"
#include "../main/unit_keyman.hxx"
#include "../main/unit_ticker.hxx"
#include "../main/func_quit.hxx"

namespace somigame { namespace main {

// signals

_DEFN_DATA signal_t<void(void)>quit_top_signal;
_DEFN_DATA signal_t<void(void)>quit_bot_signal;

// actions

_DEFN_FUNC bool quit()
{
    main::quit_top_signal.holder.publish();
    _EFNOT(game::quit(), return FALSE, "gfix quit failed");
    _EFNOT(gfix::quit(), return FALSE, "gfix quit failed");
    _EFNOT(geom::quit(), return FALSE, "geom quit failed");
    _EFNOT(fsys::quit(), return FALSE, "fsys quit failed");
    _EFNOT(ecos::quit(), return FALSE, "ecos quit failed");
    _EFNOT(main::quit_unit_ticker(), return FALSE, "main::ticker quit failed");
    _EFNOT(main::quit_unit_keyman(), return FALSE, "main::keyman quit failed");
    _EFNOT(main::quit_unit_window(), return FALSE, "main::window quit failed");
    _EFNOT(memo::quit(), return FALSE, "memo quit failed");
    main::quit_bot_signal.holder.publish();
    ::exit(error_none);
    return TRUTH;
} // quit

} } // namespace somigame { namespace main {

#endif//SOMIGAME_MAIN_FUNC_QUIT_CXX
