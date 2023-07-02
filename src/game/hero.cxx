#ifndef SOMIGAME_HERO_CXX
#define SOMIGAME_HERO_CXX 1

#include "../head.hxx"

/* headers */

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

bool hero_t::init()
{
    _EFNOT(this->vet_ent(FALSE), return FALSE, "hero entity is already set");
    /* entity */
    this->ent = ecos::ecs.ctx()
        .emplace_as<entity_t>("hero_entity"_shstr, ecos::ecs.create());
    /* logic */
    ecos::ecs.emplace<game::com_alive_t>(this->ent, game::alive_t{
        .valid = TRUTH,
        .timep = main::timer.now_mil,
    });
    ecos::ecs.emplace<game::com_actor_t>(this->ent, game::actor_t{
        .count = 0,
        .timep = main::timer.now_mil,
    });
    ecos::ecs.emplace<game::com_mover_t>(this->ent, game::mover_t{
        .fall = TRUTH,
        .move = geom::apos2_t{ 0, 0 },
    });
    /* sizes */
    ecos::ecs.emplace<geom::com_asize_t>(this->ent, geom::asize_t{
        TCELL_ASIZE_X, TCELL_ASIZE_Y
    });
    /* coord */
    ecos::ecs.emplace<geom::com_apos2_t>(this->ent);
    ecos::ecs.emplace<geom::com_zpos1_t>(this->ent);
    ecos::ecs.emplace<geom::com_pivot_t>(this->ent, geom::pivot_t{
        .x = geom::PIVOT_MID,
        .y = geom::PIVOT_MID,
    });
    /* geometry */
    ecos::ecs.emplace<geom::com_direc_t>(this->ent, +0, -1);
    /* visual */
    ecos::ecs.emplace<gfix::com_visual_t>(this->ent);
    ecos::ecs.emplace<gfix::com_rlayer_t>(this->ent);
    ecos::ecs.emplace<gfix::com_vrange_t>(this->ent, 2);
    ecos::ecs.emplace<gfix::com_color_t>(this->ent, 0xff);
    ecos::ecs.emplace<gfix::com_imreg_t>(this->ent, gfix::com_imreg_t{
        .index = gfix::_IMORI_GAME_HERO,
        .rsize = geom::rsize_t{
            .x = geom::RSIZE_MAX/4,
            .y = geom::RSIZE_MAX,
        },
    });
    ecos::ecs.emplace<gfix::com_faces_t>(this->ent, gfix::com_faces_t{
        .ilist = {
            gfix::imreg_t{
                .index = gfix::_IMORI_GAME_HERO,
                .rpos2 = geom::rpos2_t{
                    .x = geom::RPOS2_MAX*0/4,
                    .y = geom::RPOS2_MIN,
                },
                .rsize = geom::rsize_t{
                    .x = geom::RSIZE_MAX/4,
                    .y = geom::RSIZE_MAX,
                },
            },
            gfix::imreg_t{
                .index = gfix::_IMORI_GAME_HERO,
                .rpos2 = geom::rpos2_t{
                    .x = geom::RPOS2_MAX*1/4,
                    .y = geom::RPOS2_MIN,
                },
                .rsize = geom::rsize_t{
                    .x = geom::RSIZE_MAX/4,
                    .y = geom::RSIZE_MIN,
                },
            },
            gfix::imreg_t{
                .index = gfix::_IMORI_GAME_HERO,
                .rpos2 = geom::rpos2_t{
                    .x = geom::RPOS2_MAX*2/4,
                    .y = geom::RPOS2_MIN,
                },
                .rsize = geom::rsize_t{
                    .x = geom::RSIZE_MAX/4,
                    .y = geom::RSIZE_MIN,
                },
            },
            gfix::imreg_t{
                .index = gfix::_IMORI_GAME_HERO,
                .rpos2 = geom::rpos2_t{
                    .x = geom::RPOS2_MAX*3/4,
                    .y = geom::RPOS2_MIN,
                },
                .rsize = geom::rsize_t{
                    .x = geom::RSIZE_MAX/4,
                    .y = geom::RSIZE_MAX,
                },
            },
        },
    });
    /* signal */
    ecos::ecs.emplace<ecos::com_update_t<geom::com_gpos3_t>>(this->ent)
        .binder.connect<[](ecs_t&ecs, ent_t ent) {
            if (iput::get_key_mode() == iput::_KEY_MODE_VIEW) { return; }
            auto view_entity = ecs.ctx()
                .get<ent_t>("view_entity"_shstr);
            auto&gpos3 = ecs.get<geom::com_gpos3_t>(ent);
            ecs.replace<geom::com_apos2_t>(view_entity, geom::apos2_t{
                gpos3.x,
                gpos3.y,
            });
            /*
            ecs.replace<com_zpos1_t>(view_entity, zpos1_t{
                gpos3.z,
            });
            */
        }>();
    /* signal */
    struct hero_listener_t
    {
        void update_timer_alive(main::update_event_t&update)
        {
            auto&alive = ecos::ecs.get<com_alive_t>(hero_entity);
            if (!alive.valid && ((main::timer.now_mil - alive.timep) > 3'000))
            {
                alive.valid = TRUTH;
                ecos::ecs.patch<game::com_alive_t>(hero_entity);
            }
        }
        void update_alive(ecs_t&ecs, ent_t ent)
        {
            auto&alive = ecs.get<game::com_alive_t>(ent);
            if (alive.valid)
            {
                ecs.replace<geom::com_apos2_t>(hero_entity);
                iput::key_mode_set(iput::_KEY_MODE_MAIN);
                main::timer.dispatcher.sink<main::update_event_t>().disconnect<
                    &hero_listener_t::update_timer_alive
                    >(*this);
            }
            else
            {
                iput::key_mode_set(iput::_KEY_MODE_DEAD);
                main::timer.dispatcher.sink<main::update_event_t>().connect<
                    &hero_listener_t::update_timer_alive
                    >(*this);
            }
        }
        void update_gfix()
        {
        }
        entity_t hero_entity;
    } static hero_listener{ this->ent };
    ecos::ecs.emplace<ecos::com_update_t<com_alive_t>>(this->ent).binder.connect<
        &hero_listener_t::update_alive
        >(hero_listener);
    gfix::work_top_signal.binder.connect<
        &hero_listener_t::update_gfix
        >(hero_listener);
    /** family **/
    ecos::ecs.emplace<ecos::com_family_t>(this->ent, somi_t::get().get_ent());
    /* final */
    return TRUTH;
}
bool hero_t::quit()
{
    _EFNOT(this->vet_ent(TRUTH), return FALSE, "hero entity has to be set");
    /* entity */
    this->ent = entt::null;
    ecos::ecs.ctx().erase<entity_t>("hero_entity"_shstr);
    /* final */
    return TRUTH;
}

} /* typedef */

} /* content */

#endif/*SOMIGAME_HERO_CXX*/
