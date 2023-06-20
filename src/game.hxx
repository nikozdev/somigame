#pragma once

/* headers */

#include "head.hxx"
#include "ecos.hxx"
#include "fsix.hxx"
#include "gfix.hxx"

/* content */

_NAMESPACE_ENTER

/** consdef **/

constexpr auto TILE_IMAGE_COUNT = 0b1'000'00'000;
constexpr auto TILE_COUNT_X = 0xff;
constexpr auto TILE_COUNT_Y = 0xff;
constexpr auto TILE_COUNT_Z = 0xff;
constexpr auto TILE_COUNT = TILE_COUNT_X * TILE_COUNT_Y * TILE_COUNT_Z;
constexpr auto TILE_SCALE_X = 0x10;
constexpr auto TILE_SCALE_Y = 0x10;
constexpr auto TILE_SCALE_Z = 0x10;
constexpr auto GAME_HALFS_X = (TILE_COUNT_X * TILE_SCALE_X) / 2;
constexpr auto GAME_HALFS_Y = (TILE_COUNT_Y * TILE_SCALE_Y) / 2;
constexpr auto GAME_HALFS_Z = (TILE_COUNT_Z * TILE_SCALE_Z) / 2;

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


/** datadef **/

/** actions **/

extern void game_init();

extern void game_loop();

/*** hero ***/

extern void hero_goto_x(v1s_t gotox = 0);
extern void hero_goto_y(v1s_t gotoy = 0);
extern void hero_goto_z(v1s_t gotoz = 0);
extern void hero_move_x(v1s_t movex = 0);
extern void hero_move_y(v1s_t movey = 0);
extern void hero_step_x(v1s_t stepx);
extern void hero_step_y(v1s_t stepy);
extern void hero_turn(bool_t lside = _TRUTH);

/** getters **/

inline coord_t get_tile_coord(coord_t coord)
{
    if (coord.x < 0)
    {
        coord.x = (coord.x - UNIT_SCALE_X);
    }
    if (coord.y < 0)
    {
        coord.y = (coord.y - UNIT_SCALE_Y);
    }
    coord.x /= UNIT_SCALE_X;
    coord.y /= UNIT_SCALE_Y;
    return coord;
}
inline coord_t get_tile_steps(coord_t coord)
{
    return {
        coord.x + UNIT_COUNT_X/2,
        coord.y + UNIT_COUNT_Y/2,
        coord.z + UNIT_COUNT_Z/2,
    };
}

ent_t get_tile(coord_t coord);

/** vetters **/

bool vet_tile(coord_t coord);
bool vet_floor(coord_t coord);
bool vet_block(coord_t coord);

/** setters **/

ent_t set_tile(coord_t coord, tile_t tile);

_NAMESPACE_LEAVE
