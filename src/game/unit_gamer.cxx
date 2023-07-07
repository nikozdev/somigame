#ifndef SOMIGAME_GAME_UNIT_GAMER_CXX
#define SOMIGAME_GAME_UNIT_GAMER_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../ecos/type_update.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_rects.hxx"
#include "../geom/type_direc.hxx"
#include "../geom/type_mover.hxx"
#include "../gfix/type_image.hxx"
#include "../gfix/type_layer.hxx"
#include "../gfix/type_visual.hxx"
#include "../gfix/unit_camera.hxx"
#include "../game/type_sizes.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_rects.hxx"
#include "../game/type_alive.hxx"
#include "../game/type_actor.hxx"
#include "../game/unit_gamer.hxx"
#include "../game/unit_world.hxx"
#include "../main/unit_ticker.hxx"
#include "../main/unit_keyman.hxx"

namespace somigame { namespace game {

// datadef

static ecos::ent_t gamer_ent = entt::null;
static ecos::ent_t picker_ent = entt::null;
static ecos::ent_t grider_ent = entt::null;

// getters

_DEFN_FUNC ecos::ent_t get_gamer_ent()
{ return gamer_ent; }
_DEFN_FUNC ecos::ent_t get_picker_ent()
{ return picker_ent; }

// actions

static void on_proc_alive_ticker(const main::ticker_t&ticker)
{
    auto&alive = ecos::reg.get<com_alive_t>(gamer_ent);
    if (!alive.valid && ((ticker.now_mil - alive.timep) > 3'000))
    {
        alive.valid = TRUTH;
        ecos::reg.patch<game::com_alive_t>(gamer_ent);
    }
}
static void on_proc_alive(ecos::reg_t&reg, ecos::ent_t ent)
{
    auto&alive = reg.get<game::com_alive_t>(ent);
    if (alive.valid)
    {
        reg.replace<geom::com_aposi_t>(gamer_ent);
        main::key_mode_set(main::_KEY_MODE_MAIN);
        main::ticker_update_signal.binder
        .disconnect<&on_proc_alive_ticker>();
    }
    else
    {
        main::key_mode_set(main::_KEY_MODE_DEAD);
        main::ticker_update_signal.binder
        .connect<&on_proc_alive_ticker>();
    }
}
static void on_proc_gposi(ecos::reg_t&reg, ecos::ent_t ent)
{
    auto camera_entity = gfix::get_camera_ent();
    auto&aposi = reg.get<geom::com_aposi_t>(ent);
    reg.replace<geom::com_aposi_t>(camera_entity, geom::aposi_t{
        aposi.x,
        aposi.y,
    });
}

_DEFN_FUNC bool init_unit_gamer()
{
    _EFNOT(
        ecos::reg.valid(get_gamer_ent()) == FALSE,
        return FALSE,
        "gamer init was called twice !? [gamer_ent]={:d}",
        get_gamer_ent()
    );
    // entity
    gamer_ent = ecos::reg.create();
    // logic
    ecos::reg.emplace<game::com_alive_t>(gamer_ent, game::alive_t{
        .valid = TRUTH,
        .timep = main::get_ticker().now_mil,
    });
    ecos::reg.emplace<game::com_actor_t>(gamer_ent, game::actor_t{
        .count = 0,
        .timep = main::get_ticker().now_mil,
    });
    ecos::reg.emplace<geom::com_mover_t>(gamer_ent, geom::mover_t{
        .fall = TRUTH,
        .move = geom::aposi_t{ 0, 0 },
    });
    // sizes
    ecos::reg.emplace<geom::com_asize_t>(gamer_ent, geom::asize_t{
        TCELL_ASIZE_X, TCELL_ASIZE_Y
    });
    // coord
    ecos::reg.emplace<geom::com_aposi_t>(gamer_ent);
    ecos::reg.emplace<geom::com_aplim_t>(gamer_ent, geom::aplim_t{
        { -game::TGRID_AHALF_X, +game::TGRID_AHALF_X, },
        { -game::TGRID_AHALF_Y, +game::TGRID_AHALF_Y, },
    });
    ecos::reg.emplace<geom::com_pivot_t>(gamer_ent, geom::pivot_t{
        .x = geom::PIVOT_MID,
        .y = geom::PIVOT_MID,
    });
    // direc
    ecos::reg.emplace<geom::com_direc_t>(gamer_ent, +0, -1);
    // visual
    ecos::reg.emplace<gfix::com_visual_t>(gamer_ent);
    ecos::reg.emplace<gfix::com_rlayer_t>(gamer_ent, 2);
    ecos::reg.emplace<gfix::com_color_t>(gamer_ent, 0xff);
    ecos::reg.emplace<gfix::com_imreg_t>(gamer_ent, gfix::com_imreg_t{
        .index = gfix::_IMORI_GAME_HERO,
        .rsize = geom::rsize_t{
            .x = geom::RSIZE_MAX/4,
            .y = geom::RSIZE_MAX,
        },
    });
    ecos::reg.emplace<gfix::com_faces_t>(gamer_ent, gfix::com_faces_t{
        .ilist = {
            gfix::imreg_t{
                .index = gfix::_IMORI_GAME_HERO,
                .rposi = geom::rposi_t{
                    .x = geom::RPOSI_MAX*0/4,
                    .y = geom::RPOSI_MIN,
                },
                .rsize = geom::rsize_t{
                    .x = geom::RSIZE_MAX/4,
                    .y = geom::RSIZE_MAX,
                },
            },
            gfix::imreg_t{
                .index = gfix::_IMORI_GAME_HERO,
                .rposi = geom::rposi_t{
                    .x = geom::RPOSI_MAX*1/4,
                    .y = geom::RPOSI_MIN,
                },
                .rsize = geom::rsize_t{
                    .x = geom::RSIZE_MAX/4,
                    .y = geom::RSIZE_MIN,
                },
            },
            gfix::imreg_t{
                .index = gfix::_IMORI_GAME_HERO,
                .rposi = geom::rposi_t{
                    .x = geom::RPOSI_MAX*2/4,
                    .y = geom::RPOSI_MIN,
                },
                .rsize = geom::rsize_t{
                    .x = geom::RSIZE_MAX/4,
                    .y = geom::RSIZE_MIN,
                },
            },
            gfix::imreg_t{
                .index = gfix::_IMORI_GAME_HERO,
                .rposi = geom::rposi_t{
                    .x = geom::RPOSI_MAX*3/4,
                    .y = geom::RPOSI_MIN,
                },
                .rsize = geom::rsize_t{
                    .x = geom::RSIZE_MAX/4,
                    .y = geom::RSIZE_MAX,
                },
            },
        },
    });
    // signal
    ecos::reg.emplace<ecos::com_update_t<geom::com_gposi_t>>(gamer_ent)
    .binder.connect<on_proc_gposi>();
    ecos::reg.emplace<ecos::com_update_t<com_alive_t>>(gamer_ent).binder
    .connect<&on_proc_alive>();
    // family
    ecos::reg.emplace<ecos::com_family_t>(gamer_ent, game::get_world_ent());
    { // picker
        // entity
        picker_ent = ecos::reg.create();
        // logic
        ecos::reg.emplace<geom::com_mover_t>(picker_ent, FALSE);
        // coord
        ecos::reg.emplace<geom::com_aposi_t>(picker_ent);
        // sizes
        ecos::reg.emplace<geom::com_asize_t>(picker_ent, geom::asize_t{
            .x = game::TCELL_ASIZE_X,
            .y = game::TCELL_ASIZE_Y,
        });
        // visual
        ecos::reg.emplace<gfix::com_visual_t>(picker_ent).set(gfix::visual_t::HARD_HIDE);
        ecos::reg.emplace<gfix::com_rlayer_t>(picker_ent, +3);
        ecos::reg.emplace<gfix::com_color_t>(picker_ent, 0xff);
        ecos::reg.emplace<gfix::com_imreg_t>(picker_ent, gfix::imreg_t{
            .index = gfix::_IMORI_GAME_PICK,
            .rsize = geom::rsize_t{
                .x = geom::RSIZE_MAX,
                .y = geom::RSIZE_MAX,
            },
        });
        // signal
        struct listener_t {
            void on_key_mode(main::key_mode_e mode)
            {
                ecos::reg.replace<geom::com_aposi_t>(entity);
                ecos::reg.get<gfix::com_visual_t>(entity).set(
                    gfix::visual_t::HARD_SHOW*(mode==main::_KEY_MODE_PICK) +
                    + gfix::visual_t::HARD_HIDE*(mode!=main::_KEY_MODE_PICK)
                );
                ecos::reg.patch<gfix::com_visual_t>(entity);
            }
            ecos::ent_t entity;
        } static listener{ picker_ent };
        main::key_mode_set_signal.binder.connect<
        &listener_t::on_key_mode
        >(listener);
        ecos::reg.emplace<ecos::com_update_t<com_tposi_t>>(picker_ent);
        // family
        ecos::reg.emplace<ecos::com_family_t>(picker_ent, gamer_ent);
    } // picker
    { // grider
        // entity
        grider_ent = ecos::reg.create();
        // sizes
        ecos::reg.emplace<geom::com_asize_t>(grider_ent, geom::asize_t{
            .x = game::TCELL_ASIZE_X * 3,
            .y = game::TCELL_ASIZE_Y * 3,
        });
        // visual
        ecos::reg.emplace<gfix::com_visual_t>(grider_ent);
        ecos::reg.emplace<gfix::com_rlayer_t>(grider_ent, 1);
        ecos::reg.emplace<gfix::com_color_t>(grider_ent, 0x20);
        ecos::reg.emplace<gfix::com_lgrid_t>(grider_ent, gfix::lgrid_t{
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
        // family
        ecos::reg.emplace<ecos::com_family_t>(grider_ent, gamer_ent);
    } // grider
    return TRUTH;
}
_DEFN_FUNC bool quit_unit_gamer()
{
    _EFNOT(
        ecos::reg.valid(get_gamer_ent()) == TRUTH, return FALSE, "gamer quit was called twice !?"
    );
    ecos::reg.destroy(picker_ent); picker_ent = entt::null;
    ecos::reg.destroy(grider_ent); grider_ent = entt::null;
    ecos::reg.destroy(gamer_ent); gamer_ent = entt::null;
    return TRUTH;
}

} } // namespace somigame { namespace game {

#endif//SOMIGAME_GAME_UNIT_GAMER_CXX
