#ifndef SOMIGAME_SOMI_CXX
#define SOMIGAME_SOMI_CXX 1

#include "../head.hxx"

/* headers */

#include "somi.hxx"

#include "../main.hxx"
#include "../iput.hxx"
#include "../gfix.hxx"
#include "../game.hxx"

namespace somigame { /* content */

namespace game { /* typedef */

bool somi_t::init()
{
    _EFNOT(this->vet_ent(FALSE), return FALSE, "somi entity is already set");
    /* entity */
    this->ent = ecos::ecs.ctx()
        .emplace_as<entity_t>("somi_entity"_shstr, ecos::ecs.create());
    /* family */
    ecos::ecs.emplace<ecos::com_family_t>(this->ent);
    /* final */
    return TRUTH;
}
bool somi_t::quit()
{
    _EFNOT(this->vet_ent(TRUTH), return FALSE, "somi entity has to be set");
    /* entity */
    this->ent = entt::null;
    ecos::ecs.ctx().erase<entity_t>("somi_entity"_shstr);
    /* final */
    return TRUTH;
}

} /* typedef */

} /* content */

#endif/*SOMIGAME_SOMI_CXX*/
