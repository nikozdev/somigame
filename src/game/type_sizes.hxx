#ifndef SOMIGAME_GAME_TYPE_SIZES_HXX
#define SOMIGAME_GAME_TYPE_SIZES_HXX

#include "../head.hxx"
#include "../geom/type_sizes.hxx"

namespace somigame { namespace game {

// consdef

constexpr auto TCELL_CFULL_X = 0x20;
constexpr auto TCELL_CFULL_Y = 0x20;
constexpr auto TCELL_CFULL = TCELL_CFULL_X * TCELL_CFULL_Y;

constexpr auto TCELL_CHALF_X = TCELL_CFULL_X / 2;
constexpr auto TCELL_CHALF_Y = TCELL_CFULL_Y / 2;
constexpr auto TCELL_CHALF = TCELL_CHALF_X * TCELL_CHALF_Y;

constexpr auto TCELL_ASIZE_X = 0x10;
constexpr auto TCELL_ASIZE_Y = 0x10;

constexpr auto TCELL_AHALF_X = TCELL_ASIZE_X / 2;
constexpr auto TCELL_AHALF_Y = TCELL_ASIZE_Y / 2;

constexpr auto TGRID_AHALF_X = TCELL_CHALF_X * TCELL_ASIZE_X;
constexpr auto TGRID_AHALF_Y = TCELL_CHALF_Y * TCELL_ASIZE_Y;

// typedef

typedef struct tsize_t {
    v1s_t x = 0, y = 0;
} tsize_t, com_tsize_t; // tilegrid size

// getters

[[nodiscard]] inline constexpr
game::tsize_t get_tsize_from_gsize(geom::gsize_t gsize)
{ return game::tsize_t{ gsize.x/TCELL_ASIZE_X, gsize.y/TCELL_ASIZE_Y }; }
[[nodiscard]] inline constexpr
geom::gsize_t get_tsize_from_gsize(game::tsize_t tsize)
{ return geom::gsize_t{ tsize.x*TCELL_ASIZE_X, tsize.y*TCELL_ASIZE_Y }; }

// actions

_DECL_FUNC v1b_t init_type_sizes();
_DECL_FUNC v1b_t quit_type_sizes();

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_TYPE_SIZES_HXX
