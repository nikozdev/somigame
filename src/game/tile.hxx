#ifndef SOMIGAME_GAME_TILE_HXX
#define SOMIGAME_GAME_TILE_HXX 1

#include "../head.hxx"

/* headers */

#include "../geom/sizes.hxx"
#include "../geom/coord.hxx"
#include "../geom/shape.hxx"

namespace somigame { /* content */

namespace game { /* consdef */

constexpr auto TCELL_IMAGE_COUNT = 0b1'000'00'000;

constexpr auto TCELL_CFULL_X = 0x80;
constexpr auto TCELL_CFULL_Y = 0x40;
constexpr auto TCELL_CFULL_Z = 0x20;
constexpr auto TCELL_CFULL = TCELL_CFULL_X * TCELL_CFULL_Y * TCELL_CFULL_Z;

constexpr auto TCELL_CHALF_X = TCELL_CFULL_X / 2;
constexpr auto TCELL_CHALF_Y = TCELL_CFULL_Y / 2;
constexpr auto TCELL_CHALF_Z = TCELL_CFULL_Z / 2;
constexpr auto TCELL_CHALF = TCELL_CHALF_X * TCELL_CHALF_Y * TCELL_CHALF_Z;

constexpr auto TCELL_ASIZE_X = 0x10;
constexpr auto TCELL_ASIZE_Y = 0x10;
constexpr auto TCELL_ASIZE_Z = 0x01;

constexpr auto TCELL_AHALF_X = TCELL_ASIZE_X / 2;
constexpr auto TCELL_AHALF_Y = TCELL_ASIZE_Y / 2;

constexpr auto TGRID_AHALF_X = TCELL_CHALF_X * TCELL_ASIZE_X;
constexpr auto TGRID_AHALF_Y = TCELL_CHALF_Y * TCELL_ASIZE_Y;
constexpr auto TGRID_AHALF_Z = TCELL_CHALF_Z * TCELL_ASIZE_Z;

} /* consdef */

namespace game { /* datadef */

extern entity_t tgrid[TCELL_CFULL];

} /* datadef */

namespace game { /* enumdef */

typedef enum tilet_e {
    _TILET_NONE = 0x0,
    _TILET_TEST_FLOOR,
    _TILET_TEST_BLOCK,
    _TILET_COUNT
} tilet_e; /* tile type enumeration */

} /* enumdef */

namespace game { /* typedef */

typedef struct tpos3_t {
    v1s_t x = 0, y = 0, z = 0;
} tpos3_t, com_tpos3_t; /* tilegrid position 3d */
typedef msize_t tkey1_t; /* tilegrid key 1d */
typedef struct tkey3_t {
    tkey1_t x = 0, y = 0, z = 0;
} tkey3_t; /* tilegrid key 3d */

typedef struct tsize_t {
    v1s_t x = 0, y = 0;
} tsize_t, com_tsize_t; /* tilegrid size */

typedef struct trect_t {
    struct x_t {
        v1s_t l = 0, r = 0;
    } x;
    struct y_t {
        v1s_t b = 0, t = 0;
    } y;
} trect_t, com_trect_t; /* tilegrid rectangle */

typedef struct tcell_t {
    tilet_e tilet = _TILET_NONE; /* the tile type */
} tcell_t, com_tcell_t; /* tile cell */

} /* typedef */

namespace game { /* getters */

[[nodiscard]] inline
game::tpos3_t get_tpos3_from_gpos3(geom::gpos3_t gpos3)
{
    return game::tpos3_t{
        .x = gpos3.x/TCELL_ASIZE_X,
        .y = gpos3.y/TCELL_ASIZE_Y,
        .z = gpos3.z/TCELL_ASIZE_Z,
    };
}
[[nodiscard]] inline
geom::gpos3_t get_gpos3_from_tpos3(game::tpos3_t tpos3)
{
    return geom::gpos3_t{
        .x = tpos3.x * TCELL_ASIZE_X,
        .y = tpos3.y * TCELL_ASIZE_Y,
        .z = tpos3.z * TCELL_ASIZE_Z,
    };
}
[[nodiscard]] inline
game::tkey3_t get_tkey3_from_tpos3(game::tpos3_t tpos3)
{
    return game::tkey3_t{
        .x = static_cast<v1u_t>(tpos3.x + TCELL_CHALF_X),
        .y = static_cast<v1u_t>(tpos3.y + TCELL_CHALF_Y),
        .z = static_cast<v1u_t>(tpos3.z + TCELL_CHALF_Z),
    };
}
[[nodiscard]] inline
game::tkey1_t get_tkey1_from_tkey3(game::tkey3_t tkey3)
{
    return (tkey3.z * TCELL_CFULL_Y * TCELL_CFULL_X)
        + (tkey3.y * TCELL_CFULL_X) + tkey3.x;
}

[[nodiscard]] extern
ent_t get_tcell_from_tpos3(game::tpos3_t tpos3);
[[nodiscard]] inline
ent_t get_tcell_from_gpos3(geom::gpos3_t gpos3)
{ return get_tcell_from_tpos3(get_tpos3_from_gpos3(gpos3)); }

} /* getters */

namespace game { /* vetters */

[[nodiscard]] extern
bool vet_tcell_from_tpos3(game::tpos3_t tpos3);
[[nodiscard]] inline
bool vet_tcell_from_apos3(geom::gpos3_t gpos3)
{ return game::vet_tcell_from_tpos3(game::get_tpos3_from_gpos3(gpos3)); }

[[nodiscard]] extern
bool vet_floor_from_tpos3(game::tpos3_t tpos3);
inline
bool vet_floor_from_gpos3(geom::gpos3_t gpos3)
{ return game::vet_floor_from_tpos3(game::get_tpos3_from_gpos3(gpos3)); }

[[nodiscard]] extern
bool vet_block_from_tpos3(game::tpos3_t tpos3);
[[nodiscard]] inline
bool vet_block_from_gpos3(geom::gpos3_t gpos3)
{ return game::vet_block_from_tpos3(game::get_tpos3_from_gpos3(gpos3)); }

} /* vetters */

namespace game { /* setters */

extern
ent_t set_tcell_into_tpos3(
    game::tpos3_t tpos3,
    game::tcell_t tcell = { .tilet = _TILET_TEST_FLOOR }
);

} /* setters */

} /* content */

#endif/*SOMIGAME_GAME_TILE_HXX*/
