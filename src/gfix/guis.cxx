#ifndef SOMIGAME_GFIX_GUIS_CXX
#define SOMIGAME_GFIX_GUIS_CXX 1

#include "../head.hxx"

/* headers */

#include "guis.hxx"
#include "view.hxx"

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
#include "../gfix/text.hxx"
#include "../game.hxx"
#include "../game/somi.hxx"
#include "../game/hero.hxx"
#include "../game/picktile.hxx"
#include "../iput.hxx"
#include "../main.hxx"

namespace somigame { /* content */

namespace gfix { /* typedef */

constexpr const geom::asize_t IMORI_FONT_ASIZE{ 8, 8 };

bool guis_t::init()
{
    _EFNOT(this->vet_ent(FALSE), return FALSE, "guis entity is already set");
    /* entity */
    this->ent = ecos::ecs.ctx()
        .emplace_as<entity_t>("guis_entity"_shstr, ecos::ecs.create());
    /* coord */
    ecos::ecs.emplace<geom::com_rpos2_t>(this->ent, geom::RPOS2_MID, geom::RPOS2_MID);
    /* sizes */
    ecos::ecs.emplace<geom::com_rsize_t>(this->ent, geom::RSIZE_MAX, geom::RSIZE_MAX);
    /* family */
    ecos::ecs.emplace<ecos::com_family_t>(this->ent, view_t::get().get_ent());
    /* gui-mm */
    auto quad_mm_entity = ecos::ecs.create();
    /** sizes **/
    ecos::ecs.emplace<geom::com_rsize_t>(quad_mm_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX - gfix::GUIS_RSIZE_X*4,
        .y = geom::RSIZE_MAX - gfix::GUIS_RSIZE_Y*2,
    });
    /** visual **/
    ecos::ecs.emplace<gfix::com_visual_t>(quad_mm_entity);
    ecos::ecs.emplace<gfix::com_rlayer_t>(quad_mm_entity);
    /** family **/
    ecos::ecs.emplace<ecos::com_family_t>(quad_mm_entity, this->ent);
    /*** died ***/
    entity_t text_died_entity = ecos::ecs.create();
    /*** sizes ***/
    ecos::ecs.emplace<geom::com_rsize_t>(text_died_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = gfix::GUIS_RSIZE_Y,
    });
    /*** visual ***/
    auto&visual= ecos::ecs.emplace<gfix::com_visual_t>(text_died_entity);
    ecos::ecs.emplace<gfix::com_rlayer_t>(text_died_entity);
    ecos::ecs.emplace<gfix::com_color_t>(text_died_entity, 0xe0);
    ecos::ecs.emplace<gfix::com_imreg_t>(text_died_entity, _IMORI_FONT_MAIN);
    ecos::ecs.emplace<gfix::com_fonts_t>(text_died_entity, gfix::fonts_t{
        .first = ' ',
        .glyph = {
            .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y },
        },
    });
    ecos::ecs.emplace<gfix::com_label_t>(text_died_entity, "YouDied"_sdstr);
    /*** family ***/
    ecos::ecs.emplace<ecos::com_family_t>(text_died_entity, quad_mm_entity);
    /*** signal ***/
    struct text_died_listener_t {
        void on_init_game()
        {
            ecos::ecs.get<ecos::com_update_t<game::com_alive_t>>(
                ecos::ecs.ctx().get<ent_t>("hero_entity"_shstr)
            ).binder.connect<&text_died_listener_t::on_update_alive>(*this);
        }
        void on_update_alive(ecs_t&ecs, ent_t ent)
        {
            const auto&alive = ecs.get<game::com_alive_t>(ent);
            this->visual->set(
                visual_t::HARD_HIDE * (nums::get_sign(-1*alive.valid))
            );
            this->visual =&ecs.patch<com_visual_t>(this->entity);
        }
        entity_t entity;
        visual_t*visual;
    } static text_died_listener{text_died_entity,&visual};
    game::init_bot_signal.binder.connect<
        &text_died_listener_t::on_init_game
        >(text_died_listener);
    /** gui-mb **/
    entity_t quad_mb_entity = ecos::ecs.create();
    /** sizes **/
    ecos::ecs.emplace<geom::com_rsize_t>(quad_mb_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = gfix::GUIS_RSIZE_Y,
    });
    /** coord **/
    ecos::ecs.emplace<geom::com_rpos2_t>(quad_mb_entity, geom::rpos2_t{
        .x = geom::RPOS2_MID,
        .y = geom::RPOS2_MIN,
    });
    ecos::ecs.emplace<geom::com_pivot_t>(quad_mb_entity, geom::pivot_t{
        .x = geom::PIVOT_MID,
        .y = geom::PIVOT_MIN,
    });
    /** visual **/
    ecos::ecs.emplace<com_visual_t>(quad_mb_entity);
    ecos::ecs.emplace<com_rlayer_t>(quad_mb_entity);
    ecos::ecs.emplace<com_color_t>(quad_mb_entity, 0x40);
    ecos::ecs.emplace<com_imreg_t>(quad_mb_entity, _IMORI_META_NONE);
    /** family **/
    ecos::ecs.emplace<ecos::com_family_t>(quad_mb_entity, this->ent);
    /*** label-mb ***/
    auto text_mb_entity = ecos::ecs.create();
    /*** sizes ***/
    ecos::ecs.emplace<geom::com_rsize_t>(text_mb_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = geom::RSIZE_MAX,
    });
    /*** visual ***/
    ecos::ecs.emplace<gfix::com_visual_t>(text_mb_entity);
    ecos::ecs.emplace<gfix::com_rlayer_t>(text_mb_entity);
    ecos::ecs.emplace<gfix::com_color_t>(text_mb_entity, 0xe0);
    ecos::ecs.emplace<gfix::com_imreg_t>(text_mb_entity, gfix::_IMORI_FONT_MAIN);
    ecos::ecs.emplace<gfix::com_fonts_t>(text_mb_entity, gfix::fonts_t{
        .first = ' ',
        .glyph = {
            .asize = {
                .x = IMORI_FONT_ASIZE.x,
                .y = IMORI_FONT_ASIZE.y,
            },
        },
    });
    ecos::ecs.emplace<gfix::com_label_t>(text_mb_entity);
    /*** family ***/
    ecos::ecs.emplace<ecos::com_family_t>(text_mb_entity, quad_mb_entity);
    /*** signal ***/
    struct text_mb_listener_t {
        void on_game_init()
        {
            this->hero_entity = ecos::ecs.ctx()
                .get<ent_t>("hero_entity"_shstr);
            main::timer.dispatcher.sink<main::update_event_t>().connect<
                &text_mb_listener_t::on_update>(*this);
        }
        void on_update(main::update_event_t event)
        {
            label.string = std::format(
                "mode={:4s};acts={:08d};"
                "{:c}"
                "{:s}({:+1d})*(:d)"
                "{:c}",
                iput::get_key_mode_name(),
                ecos::ecs.get<game::com_actor_t>(hero_entity).count,
                text::ENDL_CHAR,
                &iput::get_key_line()[0],
                iput::get_key_narg_sign(),
                iput::get_key_narg(),
                text::ENDS_CHAR
            );
        }
        label_t&label;
        entity_t hero_entity;
    } static text_mb_listener{ ecos::ecs.get<com_label_t>(text_mb_entity) };
    game::init_bot_signal
        .binder.connect<&text_mb_listener_t::on_game_init>(text_mb_listener);
    /** gui-mt **/
    entity_t quad_mt_entity = ecos::ecs.create();
    /** sizes **/
    ecos::ecs.emplace<geom::com_rsize_t>(quad_mt_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = gfix::GUIS_RSIZE_Y,
    });
    /** coord **/
    ecos::ecs.emplace<geom::com_rpos2_t>(quad_mt_entity, geom::rpos2_t{
        .x = geom::RPOS2_MID,
        .y = geom::RPOS2_MAX,
    });
    ecos::ecs.emplace<geom::com_pivot_t>(quad_mt_entity, geom::pivot_t{
        .x = geom::PIVOT_MID,
        .y = geom::PIVOT_MAX,
    });
    /** visual **/
    ecos::ecs.emplace<gfix::com_visual_t>(quad_mt_entity);
    ecos::ecs.emplace<gfix::com_rlayer_t>(quad_mt_entity);
    ecos::ecs.emplace<gfix::com_color_t>(quad_mt_entity, 0x40);
    ecos::ecs.emplace<gfix::com_imreg_t>(quad_mt_entity, gfix::_IMORI_META_NONE);
    /** family **/
    ecos::ecs.emplace<ecos::com_family_t>(quad_mt_entity, this->ent);
    /*** label-mt ***/
    auto text_mt_entity = ecos::ecs.create();
    /*** sizes ***/
    ecos::ecs.emplace<geom::com_rsize_t>(text_mt_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = geom::RSIZE_MAX,
    });
    /*** visual ***/
    ecos::ecs.emplace<gfix::com_visual_t>(text_mt_entity);
    ecos::ecs.emplace<gfix::com_rlayer_t>(text_mt_entity);
    ecos::ecs.emplace<gfix::com_color_t>(text_mt_entity, 0xe0);
    ecos::ecs.emplace<gfix::com_imreg_t>(text_mt_entity, gfix::_IMORI_FONT_MAIN);
    ecos::ecs.emplace<gfix::com_fonts_t>(text_mt_entity, gfix::fonts_t{
        .first = ' ',
        .glyph = {
            .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y, },
        },
    });
    ecos::ecs.emplace<gfix::com_label_t>(text_mt_entity);
    /*** family ***/
    ecos::ecs.emplace<ecos::com_family_t>(text_mt_entity, quad_mt_entity);
    /*** signal ***/
    struct text_mt_listener_t
    {
        /*
        void on_update(ecs_t&ecs, ent_t ent)
        */
        void on_update(main::update_event_t event)
        {
            auto view_entity = ecos::ecs.ctx().get<ent_t>("view_entity"_shstr);
            auto view_gpos3 = ecos::ecs.get<geom::com_gpos3_t>(view_entity);
            auto view_direc = ecos::ecs.get<geom::com_direc_t>(view_entity);
            label.string = std::format(
                "now={:03d}.{:03d};fps={:05d};"
                "{:c}"
                "g(x{:+04d}y{:+04d}z{:+04d})d(x{:+1d}y{:+1d})"
                "{:c}",
                (main::timer.now_mil / 1000) % 1000,
                main::timer.now_mil % 1000,
                main::timer.fps_num,
                text::ENDL_CHAR,
                view_gpos3.x, view_gpos3.y, view_gpos3.z,
                view_direc.x, view_direc.y,
                text::ENDS_CHAR
            );
        }
        gfix::label_t&label;
    } static text_mt_listener{ ecos::ecs.get<com_label_t>(text_mt_entity) };
    /*
    ecs.get<com_update_t<com_grect_t>>(view_t::get().get_ent()).binder.connect<
        &text_mt_listener_t::on_update
        >(text_mt_listener);
    */
    main::timer.dispatcher.sink<main::update_event_t>()
    .connect<&text_mt_listener_t::on_update>(text_mt_listener);
    /** gui-lm **/
    entity_t quad_lm_entity = ecos::ecs.create();
    /** sizes **/
    ecos::ecs.emplace<geom::com_rsize_t>(quad_lm_entity, geom::rsize_t{
        .x = gfix::GUIS_RSIZE_X,
        .y = geom::RSIZE_MAX - gfix::GUIS_RSIZE_Y*2,
    });
    /** visual **/
    ecos::ecs.emplace<com_visual_t>(quad_lm_entity);
    ecos::ecs.emplace<com_rlayer_t>(quad_lm_entity);
    ecos::ecs.emplace<com_color_t>(quad_lm_entity, 0x40);
    ecos::ecs.emplace<com_imreg_t>(quad_lm_entity, gfix::_IMORI_META_NONE);
    /** coord **/
    ecos::ecs.emplace<geom::com_rpos2_t>(quad_lm_entity, geom::rpos2_t{
        .x = geom::RPOS2_MIN,
        .y = geom::RPOS2_MID,
    });
    ecos::ecs.emplace<geom::com_pivot_t>(quad_lm_entity, geom::pivot_t{
        .x = geom::PIVOT_MIN,
        .y = geom::PIVOT_MID,
    });
    /** family **/
    ecos::ecs.emplace<ecos::com_family_t>(quad_lm_entity, this->ent);
    /*** label-lm ***/
    entity_t text_lm_entity = ecos::ecs.create();
    /*** sizes ***/
    ecos::ecs.emplace<geom::com_rsize_t>(text_lm_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = geom::RSIZE_MAX,
    });
    /*** coord ***/
    ecos::ecs.emplace<geom::com_rpos2_t>(text_lm_entity, geom::rpos2_t{
        .x = geom::RPOS2_MIN,
        .y = geom::RPOS2_MIN,
    });
    ecos::ecs.emplace<geom::com_pivot_t>(text_lm_entity, geom::pivot_t{
        .x = geom::PIVOT_MIN,
        .y = geom::PIVOT_MIN,
    });
    /*** visual ***/
    ecos::ecs.emplace<gfix::com_visual_t>(text_lm_entity);
    ecos::ecs.emplace<gfix::com_rlayer_t>(text_lm_entity);
    ecos::ecs.emplace<gfix::com_color_t>(text_lm_entity, 0xe0);
    ecos::ecs.emplace<gfix::com_imreg_t>(text_lm_entity, _IMORI_FONT_MAIN);
    ecos::ecs.emplace<gfix::com_fonts_t>(text_lm_entity, fonts_t{
        .first = ' ',
        .glyph = {
            .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y, },
        },
    });
    ecos::ecs.emplace<gfix::com_label_t>(text_lm_entity);
    /*** family ***/
    ecos::ecs.emplace<ecos::com_family_t>(text_lm_entity, quad_lm_entity);
    /** gui-rm **/
    auto quad_rm_entity = ecos::ecs.create();
    /** coord **/
    ecos::ecs.emplace<geom::com_rpos2_t>(quad_rm_entity, geom::rpos2_t{
        .x = geom::RPOS2_MAX,
        .y = geom::RPOS2_MID,
    });
    ecos::ecs.emplace<geom::com_pivot_t>(quad_rm_entity, geom::pivot_t{
        .x = geom::PIVOT_MAX,
        .y = geom::PIVOT_MID,
    });
    /** sizes **/
    ecos::ecs.emplace<geom::com_rsize_t>(quad_rm_entity, geom::rsize_t{
        .x = gfix::GUIS_RSIZE_X,
        .y = geom::RSIZE_MAX - gfix::GUIS_RSIZE_Y*2,
    });
    /** visual **/
    ecos::ecs.emplace<gfix::com_visual_t>(quad_rm_entity);
    ecos::ecs.emplace<gfix::com_rlayer_t>(quad_rm_entity);
    ecos::ecs.emplace<gfix::com_color_t>(quad_rm_entity, 0x40);
    ecos::ecs.emplace<gfix::com_imreg_t>(quad_rm_entity, _IMORI_META_NONE);
    /** family **/
    ecos::ecs.emplace<ecos::com_family_t>(quad_rm_entity, this->ent);
    /*** label-rm ***/
    auto text_rm_entity = ecos::ecs.create();
    /*** sizes ***/
    ecos::ecs.emplace<geom::com_rsize_t>(text_rm_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = geom::RSIZE_MAX,
    });
    /*** coord ***/
    ecos::ecs.emplace<geom::com_rpos2_t>(text_rm_entity, geom::rpos2_t{
        .x = geom::RPOS2_MIN,
        .y = geom::RPOS2_MIN,
    });
    ecos::ecs.emplace<geom::com_pivot_t>(text_rm_entity, geom::pivot_t{
        .x = geom::PIVOT_MIN,
        .y = geom::PIVOT_MIN,
    });
    /*** visual ***/
    ecos::ecs.emplace<gfix::com_visual_t>(text_rm_entity);
    ecos::ecs.emplace<gfix::com_rlayer_t>(text_rm_entity);
    ecos::ecs.emplace<gfix::com_color_t>(text_rm_entity, 0xe0);
    ecos::ecs.emplace<gfix::com_imreg_t>(text_rm_entity, imreg_t{
        .index = _IMORI_FONT_MAIN,
        .rsize = geom::rsize_t{ geom::RSIZE_MAX, geom::RSIZE_MAX },
    });
    ecos::ecs.emplace<gfix::com_fonts_t>(text_rm_entity, gfix::fonts_t{
        .first = ' ',
        .glyph = {
            .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y, },
        },
    });
    ecos::ecs.emplace<gfix::com_label_t>(text_rm_entity, "pick info");
    /*** family ***/
    ecos::ecs.emplace<ecos::com_family_t>(text_rm_entity, quad_rm_entity);
    /*** signal ***/
    struct text_rm_listener_t {
        void on_game_init()
        {
            this->entity = game::picktile_t::get().get_ent();
            ecos::ecs.get<ecos::com_update_t<game::com_tpos3_t>>(entity)
            .binder.connect<
            &text_rm_listener_t::on_update_tpos3
            >(*this);
            this->on_update_tpos3(ecos::ecs, this->entity);
        }
        void on_update_tpos3(ecs_t&ecs, ent_t ent)
        {
            auto&tpos3 = ecs.get<game::com_tpos3_t>(ent);
            label.string = std::format(
                "pick\n"
                "========{:c}"
                "tpose\n"
                "x{:+04d}\n"
                "y{:+04d}\n"
                "z{:+04d}\n"
                "========{:c}",
                text::ENDL_CHAR,
                tpos3.x,
                tpos3.y,
                tpos3.z,
                text::ENDS_CHAR
            );
        }
        gfix::label_t&label;
        entity_t entity;
    } static text_rm_listener{ ecos::ecs.get<gfix::com_label_t>(text_rm_entity) };
    game::init_bot_signal.binder.connect<
    &text_rm_listener_t::on_game_init
    >(text_rm_listener);
    /* final */
    return TRUTH;
}
bool guis_t::quit()
{
    _EFNOT(this->vet_ent(TRUTH), return FALSE, "guis entity has to be set");
    /* entity */
    this->ent = entt::null;
    ecos::ecs.ctx().erase<entity_t>("guis_entity"_shstr);
    /* final */
    return TRUTH;
}

} /* typedef */

} /* content */

#endif/*SOMIGAME_GFIX_GUIS_CXX*/
