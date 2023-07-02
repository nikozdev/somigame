#ifndef SOMIGAME_HELPGRID_CXX
#define SOMIGAME_HELPGRID_CXX 1

#include "../head.hxx"

/* headers */

#include "helpgrid.hxx"
#include "hero.hxx"

#include "../main.hxx"
#include "../iput.hxx"
#include "../geom.hxx"
#include "../geom/sizes.hxx"
#include "../geom/coord.hxx"
#include "../geom/direc.hxx"
#include "../geom/shape.hxx"
#include "../gfix.hxx"
#include "../gfix/visual.hxx"
#include "../gfix/layer.hxx"
#include "../gfix/color.hxx"
#include "../gfix/image.hxx"
#include "../game.hxx"

namespace somigame { /* content */

namespace game { /* typedef */

bool helpgrid_t::init()
{
    _EFNOT(this->vet_ent(FALSE), return FALSE, "helpgrid entity is already set");
    /* entity */
    this->ent = ecos::ecs.ctx()
        .emplace_as<entity_t>("helpgrid_entity"_shstr, ecos::ecs.create());
    /* sizes */
    ecos::ecs.emplace<geom::com_asize_t>(this->ent, geom::asize_t{
        .x = TCELL_ASIZE_X * 3,
        .y = TCELL_ASIZE_Y * 3,
    });
    /* visual */
    ecos::ecs.emplace<gfix::com_visual_t>(this->ent);
    ecos::ecs.emplace<gfix::com_rlayer_t>(this->ent, 1);
    ecos::ecs.emplace<gfix::com_color_t>(this->ent, 0x20);
    ecos::ecs.emplace<geom::com_lgrid_t>(this->ent, geom::lgrid_t{
        .cells = {
            .x = TCELL_ASIZE_X,
            .y = TCELL_ASIZE_Y,
        },
        .tails = {
            .x = TCELL_ASIZE_X,
            .y = TCELL_ASIZE_Y,
        },
        .lsize = {
            .x = 1,
            .y = 1,
        },
    });
    /* family */
    ecos::ecs.emplace<ecos::com_family_t>(this->ent, hero_t::get().get_ent());
    /* final */
    return TRUTH;
}
bool helpgrid_t::quit()
{
    _EFNOT(this->vet_ent(TRUTH), return FALSE, "helpgrid entity has to be set");
    /* entity */
    this->ent = entt::null;
    ecos::ecs.ctx().erase<entity_t>("helpgrid_entity"_shstr);
    /* final */
    return TRUTH;
}

} /* typedef */

} /* content */

#endif/*SOMIGAME_HELPGRID_CXX*/
