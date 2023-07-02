#ifndef SOMIGAME_PICKTILE_CXX
#define SOMIGAME_PICKTILE_CXX 1

#include "../head.hxx"

/* headers */

#include "picktile.hxx"
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
#include "../game/tile.hxx"

namespace somigame { /* content */

namespace game { /* typedef */

bool picktile_t::init()
{
    _EFNOT(this->vet_ent(FALSE), return FALSE, "picktile entity is already set");
    /* entity */
    this->ent = ecos::ecs.ctx()
        .emplace_as<entity_t>("picktile_entity"_shstr, ecos::ecs.create());
    /* logic */
    ecos::ecs.emplace<game::com_mover_t>(this->ent, FALSE);
    /* coord */
    ecos::ecs.emplace<geom::com_apos2_t>(this->ent);
    /* sizes */
    ecos::ecs.emplace<geom::com_asize_t>(this->ent, geom::asize_t{
        .x = game::TCELL_ASIZE_X,
        .y = game::TCELL_ASIZE_Y,
    });
    /* visual */
    ecos::ecs.emplace<gfix::com_visual_t>(this->ent).set(gfix::visual_t::HARD_HIDE);
    ecos::ecs.emplace<gfix::com_rlayer_t>(this->ent, +3);
    ecos::ecs.emplace<gfix::com_color_t>(this->ent, 0xff);
    ecos::ecs.emplace<gfix::com_imreg_t>(this->ent, gfix::imreg_t{
        .index = gfix::_IMORI_GAME_PICK,
        .rsize = geom::rsize_t{
            .x = geom::RSIZE_MAX,
            .y = geom::RSIZE_MAX,
        },
    });
    /* signal */
    struct listener_t {
        void on_key_mode(iput::key_mode_e mode)
        {
            ecos::ecs.replace<geom::com_apos2_t>(entity);
            ecos::ecs.get<gfix::com_visual_t>(entity).set(
                gfix::visual_t::HARD_SHOW*(mode==iput::_KEY_MODE_PICK) +
                + gfix::visual_t::HARD_HIDE*(mode!=iput::_KEY_MODE_PICK)
            );
            ecos::ecs.patch<gfix::com_visual_t>(entity);
        }
        entity_t entity;
    } static listener{ this->ent };
    iput::key_mode_set_signal.binder.connect<
        &listener_t::on_key_mode
        >(listener);
    ecos::ecs.emplace<ecos::com_update_t<com_tpos3_t>>(this->ent);
    /* family */
    ecos::ecs.emplace<ecos::com_family_t>(this->ent, hero_t::get().get_ent());
    /* final */
    return TRUTH;
}
bool picktile_t::quit()
{
    _EFNOT(this->vet_ent(TRUTH), return FALSE, "picktile entity has to be set");
    /* entity */
    this->ent = entt::null;
    ecos::ecs.ctx().erase<entity_t>("picktile_entity"_shstr);
    /* final */
    return TRUTH;
}

} /* typedef */

} /* content */

#endif/*SOMIGAME_PICKTILE_CXX*/
