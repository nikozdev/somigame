#ifndef SOMIGAME_GAME_TYPE_TCELL_HXX
#define SOMIGAME_GAME_TYPE_TCELL_HXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_rects.hxx"
#include "../game/type_sizes.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_rects.hxx"

namespace somigame { namespace game {

// consdef

constexpr auto TCELL_IMAGE_COUNT = 0b1'000'00'000;

// enumdef

typedef enum tilet_e {
    _TILET_NONE = 0x0,
    _TILET_TEST_FLOOR,
    _TILET_TEST_BLOCK,
    _TILET_COUNT
} tilet_e; // tile type enumeration

// typedef

typedef struct tcell_t {
    tilet_e tilet = _TILET_NONE; // the tile type
} tcell_t, com_tcell_t; // tile cell

// getters

[[nodiscard]] extern
ecos::ent_t get_tcell_from_tposi(game::tposi_t tposi);
[[nodiscard]] inline
ecos::ent_t get_tcell_from_gposi(geom::gposi_t gposi)
{ return get_tcell_from_tposi(get_tposi_from_gposi(gposi)); }

// vetters

[[nodiscard]] extern
bool vet_tcell_from_tposi(game::tposi_t tposi);
[[nodiscard]] inline
bool vet_tcell_from_aposi(geom::gposi_t gposi)
{ return game::vet_tcell_from_tposi(game::get_tposi_from_gposi(gposi)); }

[[nodiscard]] extern
bool vet_floor_from_tposi(game::tposi_t tposi);
inline
bool vet_floor_from_gposi(geom::gposi_t gposi)
{ return game::vet_floor_from_tposi(game::get_tposi_from_gposi(gposi)); }

[[nodiscard]] extern
bool vet_block_from_tposi(game::tposi_t tposi);
[[nodiscard]] inline
bool vet_block_from_gposi(geom::gposi_t gposi)
{ return game::vet_block_from_tposi(game::get_tposi_from_gposi(gposi)); }

// getters

_DECL_FUNC ecos::ent_t set_tcell_into_tposi(
    game::tposi_t tposi,
    game::tcell_t tcell = { .tilet = _TILET_TEST_FLOOR }
);

// actions

_DECL_FUNC v1b_t init_type_tcell();
_DECL_FUNC v1b_t quit_type_tcell();

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_TYPE_TCELL_HXX
