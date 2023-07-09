#ifndef SOMIGAME_GFIX_TYPE_SHAPE_HXX
#define SOMIGAME_GFIX_TYPE_SHAPE_HXX

#include "../head.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../game/type_sizes.hxx"

namespace somigame { namespace gfix {

// typedef

typedef struct lgrid_t {
    geom::asize_t cells{ game::TCELL_ASIZE_X, game::TCELL_ASIZE_X };
    geom::asize_t tails{ 0, 0 }; // trailing lines from both sides
    geom::asize_t lsize{ 1, 1 }; // lines sizes in both axes
} lgrid_t, com_lgrid_t; // line grid

// actions

_DECL_FUNC v1b_t init_type_shape();
_DECL_FUNC v1b_t quit_type_shape();

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_SHAPE_HXX
