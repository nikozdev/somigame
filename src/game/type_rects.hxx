#ifndef SOMIGAME_GAME_TYPE_RECTS_HXX
#define SOMIGAME_GAME_TYPE_RECTS_HXX

#include "../head.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_sizes.hxx"
#include "../geom/type_rects.hxx"

namespace somigame { namespace game {

// typedef

typedef struct trect_t {
    struct x_t {
        v1s_t l = 0, r = 0;
    } x;
    struct y_t {
        v1s_t b = 0, t = 0;
    } y;
} trect_t, com_trect_t; // tilegrid rectangle

// getters

[[nodiscard]] inline constexpr
game::trect_t get_trect_from_grect(geom::grect_t grect)
{
    return game::trect_t{
        { grect.x.l/TCELL_ASIZE_X, grect.x.r/TCELL_ASIZE_X },
        { grect.y.b/TCELL_ASIZE_Y, grect.y.t/TCELL_ASIZE_Y },
    };
}
[[nodiscard]] inline constexpr
geom::grect_t get_trect_from_grect(game::trect_t trect)
{
    return geom::grect_t{
        { trect.x.l*TCELL_ASIZE_X, trect.x.r*TCELL_ASIZE_X },
        { trect.y.b*TCELL_ASIZE_Y, trect.y.t*TCELL_ASIZE_Y },
    };
}

// actions

_DECL_FUNC v1b_t init_type_rects();
_DECL_FUNC v1b_t quit_type_rects();

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_TYPE_RECTS_HXX
