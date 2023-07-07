#ifndef SOMIGAME_GAME_UNIT_WORLD_HXX
#define SOMIGAME_GAME_UNIT_WORLD_HXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"

namespace somigame { namespace game {

// getters

_DECL_FUNC ecos::ent_t get_world_ent();

// actions

_DECL_FUNC v1b_t init_unit_world();
_DECL_FUNC v1b_t quit_unit_world();

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_UNIT_WORLD_HXX
