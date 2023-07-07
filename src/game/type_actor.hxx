#ifndef SOMIGAME_GAME_TYPE_ACTOR_HXX
#define SOMIGAME_GAME_TYPE_ACTOR_HXX

#include "../head.hxx"

namespace somigame { namespace game {

// typedef

typedef struct actor_t {
    count_t count = 0; /* action count */
    msize_t timep = 0; /* the last action timepoint */
} actor_t, com_actor_t; /* anything what can perform actions */

// actions

_DECL_FUNC bool init_type_actor();
_DECL_FUNC bool quit_type_actor();

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_TYPE_ACTOR_HXX
