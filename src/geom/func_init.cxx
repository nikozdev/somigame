#ifndef SOMIGAME_GEOM_FUNC_INIT_CXX
#define SOMIGAME_GEOM_FUNC_INIT_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_rects.hxx"
#include "../geom/type_mover.hxx"

namespace somigame { namespace geom {

// signals

_DEFN_DATA signal_t<void(void)>init_top_signal;
_DEFN_DATA signal_t<void(void)>init_bot_signal;

// actions

_DEFN_FUNC bool init()
{
    // start
    geom::init_top_signal.holder.publish();
    // geometry
    _EFNOT(geom::init_type_sizes(), return FALSE, "geom init type sizes failed");
    _EFNOT(geom::init_type_coord(), return FALSE, "geom init type coord failed");
    _EFNOT(geom::init_type_rects(), return FALSE, "geom init type rects failed");
    // logic
    _EFNOT(geom::init_type_mover(), return FALSE, "geom init type mover failed");
    // final
    geom::init_bot_signal.holder.publish();
    return TRUTH;
} // init

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_FUNC_INIT_CXX
