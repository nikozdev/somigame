#pragma once

/* headers */

#include "head.hxx"
#include "ecos.hxx"
#include "gfix.hxx"

/* content */

_NAMESPACE_ENTER

/** consdef **/

constexpr int TILE_IMAGE_COUNT = 0b1'000'00'000;

/** typedef **/

typedef struct tile_t {
    index_t index;
} tile_t, com_tile_t;

typedef struct game_t {
    ent_t ent_main = entt::null;
    ent_t tile_map[UNIT_COUNT_X * UNIT_COUNT_Y * UNIT_COUNT_Z];
} game_t;


/** datadef **/

extern game_t game;

/** actions **/

extern void game_init();

/** getters **/

ent_t game_get_tile(coord_t coord, index_t layer);

/** vetters **/

bool game_vet_tile(coord_t coord, index_t layer);

/** setters **/

bool game_set_tile(coord_t coord, index_t layer, tile_t tile);

_NAMESPACE_LEAVE
