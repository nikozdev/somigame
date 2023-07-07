#ifndef SOMIGAME_MAIN_FUNC_INIT_CXX
#define SOMIGAME_MAIN_FUNC_INIT_CXX

#include "../head.hxx"
#include "../ecos/func_init.hxx"
#include "../fsys/func_init.hxx"
#include "../geom/func_init.hxx"
#include "../gfix/func_init.hxx"
#include "../game/func_init.hxx"
#include "../main/func_init.hxx"
#include "../main/unit_window.hxx"
#include "../main/unit_keyman.hxx"
#include "../main/unit_ticker.hxx"

namespace somigame { namespace main {

// signals

_DEFN_DATA signal_t<void(void)>init_top_signal;
_DEFN_DATA signal_t<void(void)>init_bot_signal;

// actions

_DEFN_FUNC bool init()
{
    main::init_top_signal.holder.publish();
    _EFNOT(memo::init(), return FALSE, "memo init failed");
    _EFNOT(main::init_unit_window(), return FALSE, "main::window init failed");
    _EFNOT(main::init_unit_keyman(), return FALSE, "main::keyman init failed");
    _EFNOT(main::init_unit_ticker(), return FALSE, "main::ticker init failed");
    _EFNOT(ecos::init(), return FALSE, "ecos init failed");
    _EFNOT(fsys::init(), return FALSE, "fsys init failed");
    _EFNOT(geom::init(), return FALSE, "geom init failed");
    _EFNOT(gfix::init(), return FALSE, "gfix init failed");
    _EFNOT(game::init(), return FALSE, "game init failed");
    main::init_bot_signal.holder.publish();
    return TRUTH;
} // init

} } // namespace somigame { namespace main {

#endif//SOMIGAME_MAIN_FUNC_INIT_CXX
