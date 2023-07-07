#ifndef SOMIGAME_GAME_TYPE_POWER_HXX
#define SOMIGAME_GAME_TYPE_POWER_HXX

#include "../head.hxx"

namespace somigame { namespace game {

// typedef

typedef struct power_t {
    v1u_t value;
} power_t, com_power_t; // energy for actions

// actions

_DECL_FUNC bool init_type_power();
_DECL_FUNC bool quit_type_power();

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_UNIT_POWER_HXX
