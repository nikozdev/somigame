#pragma once

/* headers */

#include "head.hxx"
#include "ecos.hxx"
#include "fsix.hxx"
#include "gfix.hxx"

/* content */

_NAMESPACE_ENTER

/** consdef **/

constexpr int TILE_IMAGE_COUNT = 0b1'000'00'000;

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

typedef struct game_t {
    ent_t ent_hero = entt::null;
    ent_t ent_somi = entt::null;
    ent_t tile_map[UNIT_COUNT];
} game_t;


/** datadef **/

extern game_t game;

/** actions **/

extern void game_init();

extern void game_loop();

/*** hero ***/

extern void hero_goto(const coord_t&coord);
extern void hero_goto_x(v1s_t gotox = 0);
extern void hero_goto_y(v1s_t gotoy = 0);
extern void hero_goto_z(v1s_t gotoz = 0);
extern void hero_move(const mover_t&mover);
extern void hero_move_x(v1s_t movex = 0);
extern void hero_move_y(v1s_t movey = 0);
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

/** setters **/

ent_t set_tile(coord_t coord, tile_t tile);

_NAMESPACE_LEAVE
