#ifndef SOMIGAME_GAME_HXX
#define SOMIGAME_GAME_HXX 1

#include "head.hxx"

/* headers */

#include "gfix.hxx"
#include "geom.hxx"

#include "game/tile.hxx"
#include "game/somi.hxx"
#include "game/hero.hxx"

namespace somigame { /* content */

namespace game { /* signals */

extern signal_t<void(void)> init_top_signal;
extern signal_t<void(void)> init_bot_signal;
extern signal_t<void(void)> quit_top_signal;
extern signal_t<void(void)> quit_bot_signal;

} /* signals */

namespace game { /* typedef */

typedef struct alive_t {
    v1bit_t valid = TRUTH; /* dead or live */
    msize_t timep = 0; /* the last state change timepoint */
} alive_t, com_alive_t; /* anything living or diying */

typedef struct actor_t {
    count_t count = 0; /* action count */
    msize_t timep = 0; /* the last action timepoint */
} actor_t, com_actor_t; /* anything what can perform actions */

typedef struct mover_t {
    v1bit_t fall = TRUTH;
    geom::apos2_t move = { 0, 0 };
} mover_t, com_mover_t; /* anything what can move or be moved */

typedef struct magic_t {
} magic_t, com_magic_t; /* anything what can move or be moved */

} /* typedef */

namespace game { /* actions */

extern bool init();
extern bool quit();

} /* actions */

} /* content */

#endif/*SOMIGAME_GAME_HXX*/
