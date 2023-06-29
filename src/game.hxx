#pragma once

/* headers */

#include "head.hxx"
#include "util.hxx"
#include "gfix.hxx"

/* content */

_NAMESPACE_ENTER

/** consdef **/

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

/** forward **/

typedef struct family_t family_t;

/** enumdef **/

typedef enum tilet_e {
    _TILET_NONE = 0x0,
    _TILET_TEST_FLOOR,
    _TILET_TEST_BLOCK,
    _TILET_COUNT
} tilet_e;

/** typedef **/

/*** structs ***/

typedef struct alive_t {
    v1bit_t valid = _TRUTH; /* dead or live */
    msize_t timep = 0; /* the last state change timepoint */
} alive_t; /* anything living or diying */

typedef struct actor_t {
    count_t count = 0; /* action count */
    msize_t timep = 0; /* the last action timepoint */
} actor_t; /* anything what can perform actions */

typedef struct mover_t {
    v1bit_t fall = _TRUTH;
    apos2_t move = { 0, 0 };
} mover_t; /* anything what can move or be moved */

typedef struct tcell_t {
    tilet_e tilet = _TILET_NONE; /* the tile type */
} tcell_t; /* tile cell */

/** actions **/

extern void game_init();
extern void game_quit();

/** getters **/

inline tpos3_t get_tpos3_from_gpos3(gpos3_t gpos3)
{
    return tpos3_t{
        .x = gpos3.x/TCELL_ASIZE_X,
        .y = gpos3.y/TCELL_ASIZE_Y,
        .z = gpos3.z/TCELL_ASIZE_Z,
    };
}
inline gpos3_t get_gpos3_from_tpos3(tpos3_t tpos3)
{
    return gpos3_t{
        .x = tpos3.x * TCELL_ASIZE_X,
        .y = tpos3.y * TCELL_ASIZE_Y,
        .z = tpos3.z * TCELL_ASIZE_Z,
    };
}
inline tkey3_t get_tkey3_from_tpos3(tpos3_t tpos3)
{
    return tkey3_t{
        .x = static_cast<v1u_t>(tpos3.x + TCELL_CHALF_X),
        .y = static_cast<v1u_t>(tpos3.y + TCELL_CHALF_Y),
        .z = static_cast<v1u_t>(tpos3.z + TCELL_CHALF_Z),
    };
}
inline tkey1_t get_tkey1_from_tkey3(tkey3_t tkey3)
{
    return (tkey3.z * TCELL_CFULL_Y * TCELL_CFULL_X)
        + (tkey3.y * TCELL_CFULL_X) + tkey3.x;
}

ent_t get_tcell_from_tpos3(tpos3_t tpos3);
inline ent_t get_tcell_from_gpos3(gpos3_t gpos3)
{ return get_tcell_from_tpos3(get_tpos3_from_gpos3(gpos3)); }

/** vetters **/

bool vet_tcell_from_tpos3(tpos3_t tpos3);
inline bool vet_tcell_from_apos3(gpos3_t gpos3)
{ return vet_tcell_from_tpos3(get_tpos3_from_gpos3(gpos3)); }

bool vet_floor_from_tpos3(tpos3_t tpos3);
inline bool vet_floor_from_gpos3(gpos3_t gpos3)
{ return vet_floor_from_tpos3(get_tpos3_from_gpos3(gpos3)); }

bool vet_block_from_tpos3(tpos3_t tpos3);
inline bool vet_block_from_gpos3(gpos3_t gpos3)
{ return vet_block_from_tpos3(get_tpos3_from_gpos3(gpos3)); }

/** setters **/

ent_t set_tcell_into_tpos3(
    tpos3_t tpos3,
    tcell_t tcell = { .tilet = _TILET_TEST_FLOOR }
);

_NAMESPACE_LEAVE
