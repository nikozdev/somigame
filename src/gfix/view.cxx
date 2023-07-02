#ifndef SOMIGAME_GFIX_VIEW_CXX
#define SOMIGAME_GFIX_VIEW_CXX 1

#include "../head.hxx"

/* headers */

#include "view.hxx"

#include "../geom.hxx"
#include "../geom/sizes.hxx"
#include "../geom/coord.hxx"
#include "../geom/direc.hxx"
#include "../geom/shape.hxx"
#include "../gfix.hxx"
#include "../game.hxx"

namespace somigame { namespace gfix { /* typedef */

bool view_t::init()
{
    _EFNOT(this->vet_ent(FALSE), return FALSE, "view entity is already set");
    /* entity */
    this->ent = ecos::ecs.ctx()
        .emplace_as<entity_t>("view_entity"_shstr, ecos::ecs.create());
    /* sizes */
    ecos::ecs.emplace<geom::com_asize_t>(this->ent, geom::asize_t{
        .x = gfix::VIEW_ASIZE_X,
        .y = gfix::VIEW_ASIZE_Y,
    });
    ecos::ecs.emplace<geom::com_ratio_t>(this->ent);
    /* coord */
    ecos::ecs.emplace<geom::com_apos2_t>(this->ent);
    ecos::ecs.emplace<geom::com_zpos1_t>(this->ent, game::TCELL_CFULL_Z);
    /* geometry */
    ecos::ecs.emplace<geom::com_direc_t>(this->ent, -0, +1);
    /* signal */
    struct listener_t {
        void on_grect_update(ecs_t&ecs, ent_t ent)
        {
            auto&grect = ecs.get<geom::com_grect_t>(ent);
            ::glMatrixMode(GL_PROJECTION);
            ::glLoadIdentity();
            ::gluOrtho2D(
                grect.x.l, grect.x.r,
                grect.y.b, grect.y.t
            );
        }
    } static listener;
    ecos::ecs.emplace<ecos::com_update_t<geom::com_grect_t>>(this->ent)
        .binder.connect<
        &listener_t::on_grect_update
        >(listener);
    /* family */
    ecos::ecs.emplace<ecos::com_family_t>(this->ent);
    /* final */
    return TRUTH;
}
bool view_t::quit()
{
    _EFNOT(this->vet_ent(TRUTH), return FALSE, "view entity has to be set");
    /* entity */
    this->ent = entt::null;
    ecos::ecs.ctx().erase<entity_t>("view_entity"_shstr);
    /* final */
    return FALSE;
}

bool view_t::turn90(bool lside)
{
    auto direc = this->get_com<geom::com_direc_t>();
    ecos::ecs.replace<geom::com_direc_t>(this->ent,
        nums::get_xy90turn(direc, lside)
    );
    return TRUTH;
}

} } /* typedef */

#endif/*SOMIGAME_GFIX_VIEW_CXX*/
