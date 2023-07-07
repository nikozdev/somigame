#ifndef SOMIGAME_GAME_ALIVE_HXX
#define SOMIGAME_GAME_ALIVE_HXX 1

#include "../head.hxx"

namespace somigame { namespace game {

// typedef

typedef struct alive_t {
    v1bit_t valid = TRUTH; /* dead or live */
    msize_t timep = 0; /* the last state change timepoint */
} alive_t, com_alive_t; /* anything living or diying */

// actions

_DECL_FUNC v1b_t init_type_alive();
_DECL_FUNC v1b_t quit_type_alive();

} } // namespace somigame { namespace game {

#endif/*SOMIGAME_GAME_ALIVE_HXX*/
