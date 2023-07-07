#ifndef SOMIGAME_GEOM_TYPE_RECTS_HXX
#define SOMIGAME_GEOM_TYPE_RECTS_HXX

#include "../head.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"

namespace somigame { namespace geom {

// typedef

typedef struct grect_t {
    struct x_t {
        v1s_t l = 0, r = 0;
    } x;
    struct y_t {
        v1s_t b = 0, t = 0;
    } y;
} grect_t, com_grect_t; // global rectangle

// actions

_DECL_FUNC v1b_t init_type_rects();
_DECL_FUNC v1b_t quit_type_rects();

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_TYPE_RECTS_HXX
