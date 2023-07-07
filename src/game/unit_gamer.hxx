#ifndef SOMIGAME_GAME_UNIT_GAMER_HXX
#define SOMIGAME_GAME_UNIT_GAMER_HXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"

namespace somigame { namespace game {

// getters

_DECL_FUNC ecos::ent_t get_gamer_ent();
_DECL_FUNC ecos::ent_t get_picker_ent();

// actions

_DECL_FUNC bool init_unit_gamer();
_DECL_FUNC bool quit_unit_gamer();

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_UNIT_GAMER_HXX
