#ifndef SOMIGAME_GAME_UNIT_WORLD_CXX
#define SOMIGAME_GAME_UNIT_WORLD_CXX

#include "../head.hxx"
#include "../main.hxx"
#include "../gfix.hxx"
#include "../game.hxx"
#include "unit_world.hxx"

namespace somigame { namespace game {

// datadef

static ecos::ent_t world_ent = entt::null;

// getters

_DEFN_FUNC ecos::ent_t get_world_ent()
{ return world_ent; }

// actions

_DEFN_FUNC v1b_t init_unit_world()
{
    _EFNOT(
        ecos::reg.valid(get_world_ent()) == FALSE,
        return FALSE,
        "world init was called twice !? [world_ent]={:d}",
        get_world_ent()
    );
    // entity
    world_ent = ecos::reg.create();
    // family
    ecos::reg.emplace<ecos::com_family_t>(world_ent);
    // final
    return TRUTH;
}
_DEFN_FUNC v1b_t quit_unit_world()
{
    _EFNOT(
        ecos::reg.valid(get_world_ent()) == TRUTH,
        return FALSE,
        "world quit was called twice !? [wolrd_ent]={:d}",
        get_world_ent()
    );
    ecos::reg.destroy(world_ent); world_ent = entt::null;
    return TRUTH;
}

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_UNIT_WORLD_CXX
