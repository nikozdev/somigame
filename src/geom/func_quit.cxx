#ifndef SOMIGAME_GEOM_FUNC_QUIT_CXX
#define SOMIGAME_GEOM_FUNC_QUIT_CXX

#include "../head.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_rects.hxx"
#include "../geom/type_mover.hxx"
#include "../geom/func_quit.hxx"

namespace somigame { namespace geom {

// signals

_DEFN_DATA signal_t<void(void)>quit_top_signal;
_DEFN_DATA signal_t<void(void)>quit_bot_signal;

// actions

_DEFN_FUNC bool quit()
{
    quit_top_signal.holder.publish();
    _EFNOT(geom::quit_type_sizes(), return FALSE, "geom quit type sizes failed");
    _EFNOT(geom::quit_type_coord(), return FALSE, "geom quit type coord failed");
    _EFNOT(geom::quit_type_rects(), return FALSE, "geom quit type rects failed");
    _EFNOT(geom::quit_type_mover(), return FALSE, "geom quit type mover failed");
    quit_bot_signal.holder.publish();
    return TRUTH;
} // quit

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_FUNC_QUIT_CXX
