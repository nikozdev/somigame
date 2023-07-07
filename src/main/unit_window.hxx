#ifndef SOMIGAME_MAIN_UNIT_WINDOW_HXX
#define SOMIGAME_MAIN_UNIT_WINDOW_HXX

#include "../head.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_sizes.hxx"

namespace somigame { namespace main {

// typedef

typedef struct window_t {
    geom::ratio_t ratio;
    geom::aposi_t aposi;
    geom::asize_t asize;
} window_t;

// signals

_DECL_DATA signal_t<void(const window_t&)>window_sized_start_signal;
_DECL_DATA signal_t<void(const window_t&)>window_sized_final_signal;

// getters

_DECL_FUNC const window_t&get_window();

// actions

_DECL_FUNC bool init_unit_window();
_DECL_FUNC bool quit_unit_window();

} } // namespace somigame { namespace main {

#endif//SOMIGAME_MAIN_UNIT_WINDOW_HXX
