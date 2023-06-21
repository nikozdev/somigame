#pragma once

/* headers */

#include "head.hxx"
#include "ecos.hxx"
#include "gfix.hxx"

/* content */

_NAMESPACE_ENTER

/** consdef **/

constexpr auto TILE_IMAGE_COUNT = 0b1'000'00'000;
constexpr auto TILE_COUNT_X = 0x80;
constexpr auto TILE_COUNT_Y = 0x40;
constexpr auto TILE_COUNT_Z = 0x20;
constexpr auto TILE_COUNT = TILE_COUNT_X * TILE_COUNT_Y * TILE_COUNT_Z;
constexpr auto TILE_SCALE_X = 0x10;
constexpr auto TILE_SCALE_Y = 0x10;
constexpr auto TILE_SCALE_Z = 0x01;
constexpr auto TILE_HALFS_X = TILE_SCALE_X / 2;
constexpr auto TILE_HALFS_Y = TILE_SCALE_Y / 2;
constexpr auto GAME_HALFC_X = TILE_COUNT_X / 2;
constexpr auto GAME_HALFC_Y = TILE_COUNT_Y / 2;
constexpr auto GAME_HALFC_Z = TILE_COUNT_Z / 2;
constexpr auto GAME_HALFS_X = GAME_HALFC_X * TILE_SCALE_X;
constexpr auto GAME_HALFS_Y = GAME_HALFC_Y * TILE_SCALE_Y;
constexpr auto GAME_HALFS_Z = GAME_HALFC_Z * TILE_SCALE_Z;

/** typedef **/

typedef struct tile_t {
    bool _;
} tile_t, com_tile_t;

typedef struct floor_t {
    bool _;
} floor_t, com_floor_t;

typedef struct block_t {
    bool _;
} block_t, com_block_t;


/** actions **/

extern void game_init();

extern void game_loop();

extern void inc_action_count(count_t inc = 1);

/*** hero ***/

extern void hero_goto_x(v1s_t gotox = 0);
extern void hero_goto_y(v1s_t gotoy = 0);
extern void hero_goto_z(v1s_t gotoz = 0);
extern void hero_step_x(v1s_t stepx);
extern void hero_step_y(v1s_t stepy);
extern void pick_step_x(v1s_t stepx);
extern void pick_step_y(v1s_t stepy);
extern void hero_turn(bool_t lside = _TRUTH);

/** getters **/

extern count_t get_action_count();

inline coord_t from_tile_coord_into_coord(coord_t coord)
{
    return coord_t{
        .x = (coord.x) * TILE_SCALE_X,
        .y = (coord.y) * TILE_SCALE_Y,
        .z = (coord.z) * TILE_SCALE_Z,
    };
}
inline coord_t from_coord_into_tile_coord(coord_t coord)
{
    coord.x /= (TILE_SCALE_X);
    coord.y /= (TILE_SCALE_Y);
    return coord;
}
inline coord_t from_tile_coord_into_tile_steps(coord_t coord)
{
    return {
        coord.x + GAME_HALFC_X,
        coord.y + GAME_HALFC_Y,
        coord.z + GAME_HALFC_Z,
    };
}
inline msize_t from_tile_steps_into_tile_mstep(coord_t steps)
{
    return (steps.z * TILE_COUNT_Y * TILE_COUNT_X) + (steps.y * TILE_COUNT_X) + steps.x;
}

inline auto from_coord_into_tile_distx(coord_t coord)
{ return std::abs(from_coord_into_tile_coord(coord).x); }
inline auto from_coord_into_tile_disty(coord_t coord)
{ return std::abs(from_coord_into_tile_coord(coord).y); }

ent_t get_tile_from_coord(coord_t coord);
ent_t get_tile_from_tile_coord(coord_t coord);

/** vetters **/

bool vet_tile_from_coord(coord_t coord);
bool vet_tile_from_tile_coord(coord_t coord);

bool vet_floor_from_coord(coord_t coord);
bool vet_floor_from_tile_coord(coord_t coord);

bool vet_block_from_coord(coord_t coord);
bool vet_block_from_tile_coord(coord_t coord);

/** setters **/

ent_t set_tile_into_tile_coord(coord_t coord, tile_t tile);

_NAMESPACE_LEAVE
