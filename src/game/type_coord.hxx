#ifndef SOMIGAME_GAME_TYPE_COORD_HXX
#define SOMIGAME_GAME_TYPE_COORD_HXX

#include "../head.hxx"
#include "../geom/type_coord.hxx"
#include "../game/type_sizes.hxx"

namespace somigame { namespace game {

// typedef

typedef struct tposi_t {
    v1s_t x = 0, y = 0;
} tposi_t, com_tposi_t; // tilegrid position
typedef struct tkeys_t {
    mstep_t x = 0, y = 0;
} tkeys_t; // tilegrid keys

// getters

[[nodiscard]] inline
game::tposi_t get_tposi_from_gposi(geom::gposi_t gposi)
{ return game::tposi_t{ gposi.x/TCELL_ASIZE_X, gposi.y/TCELL_ASIZE_Y };
}
[[nodiscard]] inline
geom::gposi_t get_gposi_from_tposi(game::tposi_t tposi)
{ return geom::gposi_t{ tposi.x*TCELL_ASIZE_X, tposi.y*TCELL_ASIZE_Y, }; }
[[nodiscard]] inline
game::tkeys_t get_tkeys_from_tposi(game::tposi_t tposi)
{
    return game::tkeys_t{
        .x = static_cast<v1u_t>(tposi.x + TCELL_CHALF_X),
        .y = static_cast<v1u_t>(tposi.y + TCELL_CHALF_Y),
    };
}
[[nodiscard]] inline
mstep_t get_mstep_from_tkeys(game::tkeys_t tkeys)
{ return (tkeys.y * game::TCELL_CFULL_X) + tkeys.x; }

// vetters

[[nodiscard]] inline
bool vet_tkeys_oside(tkeys_t tkeys)
{ return FALSE
    || (tkeys.x < 0 || tkeys.x >= game::TCELL_CFULL_X)
    || (tkeys.y < 0 || tkeys.y >= game::TCELL_CFULL_Y);
}
[[nodiscard]] inline
bool vet_tkeys_iside(tkeys_t tkeys)
{ return TRUTH
    && (tkeys.x >= 0 && tkeys.x < game::TCELL_CFULL_X)
    && (tkeys.y >= 0 && tkeys.y < game::TCELL_CFULL_Y);
}

// actions

_DECL_FUNC v1b_t init_type_coord();
_DECL_FUNC v1b_t quit_type_coord();

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_TYPE_COORD_HXX
