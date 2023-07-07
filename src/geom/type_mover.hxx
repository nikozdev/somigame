#ifndef SOMIGAME_GEOM_TYPE_MOVER_HXX
#define SOMIGAME_GEOM_TYPE_MOVER_HXX

#include "../head.hxx"
#include "../geom/type_coord.hxx"

namespace somigame { namespace geom {

// typedef

typedef struct mover_t {
    v1bit_t fall = TRUTH;
    aposi_t move = { 0, 0 };
} mover_t, com_mover_t; /* anything what can move or be moved */

// actions

_DECL_FUNC bool init_type_mover();
_DECL_FUNC bool quit_type_mover();

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_TYPE_MOVER_HXX
