#ifndef SOMIGAME_GFIX_UNIT_GUIMAN_CXX
#define SOMIGAME_GFIX_UNIT_GUIMAN_CXX

#include "../head.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_rects.hxx"
#include "../geom/type_direc.hxx"
#include "../gfix/type_image.hxx"
#include "../gfix/type_fonts.hxx"
#include "../gfix/type_label.hxx"
#include "../gfix/type_layer.hxx"
#include "../gfix/type_visual.hxx"
#include "../gfix/unit_camera.hxx"
#include "../gfix/unit_guiman.hxx"
#include "../game/unit_gamer.hxx"
#include "../game/type_sizes.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_rects.hxx"
#include "../game/type_tcell.hxx"
#include "../game/type_actor.hxx"
#include "../game/type_alive.hxx"
#include "../game/func_init.hxx"
#include "../main/unit_keyman.hxx"
#include "../main/unit_ticker.hxx"

namespace somigame { namespace gfix {

// consdef

constexpr const geom::asize_t IMORI_FONT_ASIZE{ 8, 8 };

// datadef

static ecos::ent_t guiman_ent = entt::null;

// getters

_DEFN_FUNC ecos::ent_t get_guiman_ent()
{ return guiman_ent; }

// actions

_DEFN_FUNC v1b_t init_unit_guiman()
{
    _EFNOT(
        ecos::reg.valid(get_guiman_ent()) == FALSE,
        return FALSE,
        "guiman init was called twice !? [guiman_ent]={}",
        get_camera_ent()
    );
    // entity
    guiman_ent = ecos::reg.create();
    // coord
    ecos::reg.emplace<geom::com_rposi_t>(guiman_ent, geom::RPOSI_MID, geom::RPOSI_MID);
    // sizes
    ecos::reg.emplace<geom::com_rsize_t>(guiman_ent, geom::RSIZE_MAX, geom::RSIZE_MAX);
    // visual
    ecos::reg.emplace<gfix::com_visual_t>(guiman_ent).set(gfix::visual_t::WEAK_SHOW);
    ecos::reg.emplace<com_rlayer_t>(guiman_ent, gfix::GUIS_LAYER);
    // family
    ecos::reg.emplace<ecos::com_family_t>(guiman_ent, get_camera_ent());
    // // gui-mm
    auto quad_mm_entity = ecos::reg.create();
    // // sizes
    ecos::reg.emplace<geom::com_rsize_t>(quad_mm_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX - gfix::GUIS_RSIZE_X*2,
        .y = geom::RSIZE_MAX - gfix::GUIS_RSIZE_Y*2,
    });
    // // visual
    ecos::reg.emplace<gfix::com_visual_t>(quad_mm_entity);
    ecos::reg.emplace<gfix::com_rlayer_t>(quad_mm_entity);
    // // family
    ecos::reg.emplace<ecos::com_family_t>(quad_mm_entity, guiman_ent);
    // // // died
    ecos::ent_t text_died_entity = ecos::reg.create();
    // // // sizes
    ecos::reg.emplace<geom::com_rsize_t>(text_died_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = gfix::GUIS_RSIZE_Y,
    });
    // // // visual
    auto&text_died_visual= ecos::reg.emplace<gfix::com_visual_t>(text_died_entity);
    text_died_visual.set(gfix::com_visual_t::HARD_HIDE);
    ecos::reg.emplace<gfix::com_rlayer_t>(text_died_entity);
    ecos::reg.emplace<gfix::com_color_t>(text_died_entity, 0xe0);
    ecos::reg.emplace<gfix::com_imreg_t>(text_died_entity, gfix::_IMORI_FONT_MAIN);
    ecos::reg.emplace<gfix::com_fonts_t>(text_died_entity, gfix::fonts_t{
        .first = ' ',
        .glyph = { .asize = IMORI_FONT_ASIZE },
    });
    ecos::reg.emplace<gfix::com_label_t>(text_died_entity, "YouDied");
    // // // family
    ecos::reg.emplace<ecos::com_family_t>(text_died_entity, quad_mm_entity);
    // // // signal
    struct text_died_listener_t {
        void on_init_game()
        {
            ecos::reg.get<ecos::com_update_t<game::com_alive_t>>(
                game::get_gamer_ent()
            ).binder.connect<&text_died_listener_t::on_update_alive>(*this);
        }
        void on_update_alive(ecos::reg_t&reg, ecos::ent_t ent)
        {
            const auto&alive = reg.get<game::com_alive_t>(ent);
            this->visual->set(
                visual_t::HARD_HIDE * (nums::get_sign(-1*alive.valid))
            );
            this->visual =&reg.patch<com_visual_t>(this->entity);
        }
        ecos::ent_t entity;
        visual_t*visual;
    } static text_died_listener{text_died_entity,&text_died_visual};
    game::init_bot_signal.binder
    .connect<&text_died_listener_t::on_init_game>(text_died_listener);
    // // gui-mb
    ecos::ent_t quad_mb_entity = ecos::reg.create();
    // // sizes
    ecos::reg.emplace<geom::com_rsize_t>(quad_mb_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = gfix::GUIS_RSIZE_Y,
    });
    // // coord
    ecos::reg.emplace<geom::com_rposi_t>(quad_mb_entity, geom::rposi_t{
        .x = geom::RPOSI_MID,
        .y = geom::RPOSI_MIN,
    });
    ecos::reg.emplace<geom::com_pivot_t>(quad_mb_entity, geom::pivot_t{
        .x = geom::PIVOT_MID,
        .y = geom::PIVOT_MIN,
    });
    // // visual
    ecos::reg.emplace<com_visual_t>(quad_mb_entity);
    ecos::reg.emplace<com_rlayer_t>(quad_mb_entity);
    ecos::reg.emplace<com_color_t>(quad_mb_entity, 0x40);
    ecos::reg.emplace<com_imreg_t>(quad_mb_entity, gfix::_IMORI_META_NONE);
    // // family
    ecos::reg.emplace<ecos::com_family_t>(quad_mb_entity, guiman_ent);
    // // // label-mb
    auto text_mb_entity = ecos::reg.create();
    // // // sizes
    ecos::reg.emplace<geom::com_rsize_t>(text_mb_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = geom::RSIZE_MAX,
    });
    // // // visual
    ecos::reg.emplace<gfix::com_visual_t>(text_mb_entity);
    ecos::reg.emplace<gfix::com_rlayer_t>(text_mb_entity);
    ecos::reg.emplace<gfix::com_color_t>(text_mb_entity, 0xe0);
    ecos::reg.emplace<gfix::com_imreg_t>(text_mb_entity, gfix::_IMORI_FONT_MAIN);
    ecos::reg.emplace<gfix::com_fonts_t>(text_mb_entity, gfix::fonts_t{
        .first = ' ',
        .glyph = { .asize = IMORI_FONT_ASIZE },
    });
    ecos::reg.emplace<gfix::com_label_t>(text_mb_entity);
    // // // family
    ecos::reg.emplace<ecos::com_family_t>(text_mb_entity, quad_mb_entity);
    // // // signal
    struct text_mb_listener_t {
        void on_game_init()
        {
            this->gamer_ent = game::get_gamer_ent();
            main::ticker_update_signal.binder
            .connect<&text_mb_listener_t::on_update>(*this);
        }
        void on_update(const main::ticker_t&ticker)
        {
            label.string = std::format(
                "mode={:4s};acts={:08d};"
                "{:c}"
                "{:s}({:+1d})*(:d)"
                "{:c}",
                main::get_key_mode_name(),
                ecos::reg.get<game::com_actor_t>(this->gamer_ent).count,
                text::ENDL_CHAR,
                &main::get_key_line()[0],
                main::get_key_narg_sign(),
                main::get_key_narg(),
                text::ENDS_CHAR
            );
        }
        gfix::label_t&label;
        ecos::ent_t gamer_ent;
    } static text_mb_listener{ ecos::reg.get<com_label_t>(text_mb_entity) };
    game::init_bot_signal.binder
    .connect<&text_mb_listener_t::on_game_init>(text_mb_listener);
    // // gui-mt
    ecos::ent_t quad_mt_entity = ecos::reg.create();
    // // sizes
    ecos::reg.emplace<geom::com_rsize_t>(quad_mt_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = gfix::GUIS_RSIZE_Y,
    });
    // // coord
    ecos::reg.emplace<geom::com_rposi_t>(quad_mt_entity, geom::rposi_t{
        .x = geom::RPOSI_MID,
        .y = geom::RPOSI_MAX,
    });
    ecos::reg.emplace<geom::com_pivot_t>(quad_mt_entity, geom::pivot_t{
        .x = geom::PIVOT_MID,
        .y = geom::PIVOT_MAX,
    });
    // // visual
    ecos::reg.emplace<gfix::com_visual_t>(quad_mt_entity);
    ecos::reg.emplace<gfix::com_rlayer_t>(quad_mt_entity);
    ecos::reg.emplace<gfix::com_color_t>(quad_mt_entity, 0x40);
    ecos::reg.emplace<gfix::com_imreg_t>(quad_mt_entity, gfix::_IMORI_META_NONE);
    // // family
    ecos::reg.emplace<ecos::com_family_t>(quad_mt_entity, guiman_ent);
    // // // label-mt
    auto text_mt_entity = ecos::reg.create();
    // // // sizes
    ecos::reg.emplace<geom::com_rsize_t>(text_mt_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = geom::RSIZE_MAX,
    });
    // // // visual
    ecos::reg.emplace<gfix::com_visual_t>(text_mt_entity);
    ecos::reg.emplace<gfix::com_rlayer_t>(text_mt_entity);
    ecos::reg.emplace<gfix::com_color_t>(text_mt_entity, 0xe0);
    ecos::reg.emplace<gfix::com_imreg_t>(text_mt_entity, gfix::_IMORI_FONT_MAIN);
    ecos::reg.emplace<gfix::com_fonts_t>(text_mt_entity, gfix::fonts_t{
        .first = ' ',
        .glyph = { .asize = IMORI_FONT_ASIZE },
    });
    ecos::reg.emplace<gfix::com_label_t>(text_mt_entity);
    // // // family
    ecos::reg.emplace<ecos::com_family_t>(text_mt_entity, quad_mt_entity);
    // // // signal
    struct text_mt_listener_t
    {
        void on_update(const main::ticker_t&ticker)
        {
            auto camera_entity = get_camera_ent();
            auto camera_gposi = ecos::reg.get<geom::com_gposi_t>(camera_entity);
            auto camera_direc = ecos::reg.get<geom::com_direc_t>(camera_entity);
            label.string = std::format(
                "now={:03d}.{:03d};fps={:05d};"
                "{:c}"
                "g(x{:+04d}y{:+04d}d(x{:+1d}y{:+1d})"
                "{:c}",
                (ticker.now_mil / 1000) % 1000,
                (ticker.now_mil) % 1000,
                (ticker.fps_num),
                text::ENDL_CHAR,
                camera_gposi.x, camera_gposi.y,
                camera_direc.x, camera_direc.y,
                text::ENDS_CHAR
            );
        }
        gfix::label_t&label;
    } static text_mt_listener{ ecos::reg.get<com_label_t>(text_mt_entity) };
    main::ticker_update_signal.binder
    .connect<&text_mt_listener_t::on_update>(text_mt_listener);
    // // gui-lm
    ecos::ent_t quad_lm_entity = ecos::reg.create();
    // // sizes
    ecos::reg.emplace<geom::com_rsize_t>(quad_lm_entity, geom::rsize_t{
        .x = gfix::GUIS_RSIZE_X,
        .y = geom::RSIZE_MAX - gfix::GUIS_RSIZE_Y*2,
    });
    // // visual
    ecos::reg.emplace<com_visual_t>(quad_lm_entity);
    ecos::reg.emplace<com_rlayer_t>(quad_lm_entity);
    ecos::reg.emplace<com_color_t>(quad_lm_entity, 0x40);
    ecos::reg.emplace<com_imreg_t>(quad_lm_entity, gfix::_IMORI_META_NONE);
    // // coord
    ecos::reg.emplace<geom::com_rposi_t>(quad_lm_entity, geom::rposi_t{
        .x = geom::RPOSI_MIN,
        .y = geom::RPOSI_MID,
    });
    ecos::reg.emplace<geom::com_pivot_t>(quad_lm_entity, geom::pivot_t{
        .x = geom::PIVOT_MIN,
        .y = geom::PIVOT_MID,
    });
    // // family
    ecos::reg.emplace<ecos::com_family_t>(quad_lm_entity, guiman_ent);
    // // // label-lm
    ecos::ent_t text_lm_entity = ecos::reg.create();
    // // // sizes
    ecos::reg.emplace<geom::com_rsize_t>(text_lm_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = geom::RSIZE_MAX,
    });
    // // // coord
    ecos::reg.emplace<geom::com_rposi_t>(text_lm_entity, geom::rposi_t{
        .x = geom::RPOSI_MIN,
        .y = geom::RPOSI_MIN,
    });
    ecos::reg.emplace<geom::com_pivot_t>(text_lm_entity, geom::pivot_t{
        .x = geom::PIVOT_MIN,
        .y = geom::PIVOT_MIN,
    });
    // // // visual
    ecos::reg.emplace<gfix::com_visual_t>(text_lm_entity);
    ecos::reg.emplace<gfix::com_rlayer_t>(text_lm_entity);
    ecos::reg.emplace<gfix::com_color_t>(text_lm_entity, 0xe0);
    ecos::reg.emplace<gfix::com_imreg_t>(text_lm_entity, gfix::_IMORI_FONT_MAIN);
    ecos::reg.emplace<gfix::com_fonts_t>(text_lm_entity, gfix::fonts_t{
        .first = ' ',
        .glyph = { .asize = IMORI_FONT_ASIZE },
    });
    ecos::reg.emplace<gfix::com_label_t>(text_lm_entity);
    // // // family
    ecos::reg.emplace<ecos::com_family_t>(text_lm_entity, quad_lm_entity);
    // // gui-rm
    auto quad_rm_entity = ecos::reg.create();
    // // coord
    ecos::reg.emplace<geom::com_rposi_t>(quad_rm_entity, geom::rposi_t{
        .x = geom::RPOSI_MAX,
        .y = geom::RPOSI_MID,
    });
    ecos::reg.emplace<geom::com_pivot_t>(quad_rm_entity, geom::pivot_t{
        .x = geom::PIVOT_MAX,
        .y = geom::PIVOT_MID,
    });
    // // sizes
    ecos::reg.emplace<geom::com_rsize_t>(quad_rm_entity, geom::rsize_t{
        .x = gfix::GUIS_RSIZE_X,
        .y = geom::RSIZE_MAX - gfix::GUIS_RSIZE_Y*2,
    });
    // // visual
    ecos::reg.emplace<gfix::com_visual_t>(quad_rm_entity);
    ecos::reg.emplace<gfix::com_rlayer_t>(quad_rm_entity);
    ecos::reg.emplace<gfix::com_color_t>(quad_rm_entity, 0x40);
    ecos::reg.emplace<gfix::com_imreg_t>(quad_rm_entity, _IMORI_META_NONE);
    // // family
    ecos::reg.emplace<ecos::com_family_t>(quad_rm_entity, guiman_ent);
    // // // label-rm
    auto text_rm_entity = ecos::reg.create();
    // // // sizes
    ecos::reg.emplace<geom::com_rsize_t>(text_rm_entity, geom::rsize_t{
        .x = geom::RSIZE_MAX,
        .y = geom::RSIZE_MAX,
    });
    // // // coord
    ecos::reg.emplace<geom::com_rposi_t>(text_rm_entity, geom::rposi_t{
        .x = geom::RPOSI_MIN,
        .y = geom::RPOSI_MIN,
    });
    ecos::reg.emplace<geom::com_pivot_t>(text_rm_entity, geom::pivot_t{
        .x = geom::PIVOT_MIN,
        .y = geom::PIVOT_MIN,
    });
    // // // visual
    ecos::reg.emplace<gfix::com_visual_t>(text_rm_entity);
    ecos::reg.emplace<gfix::com_rlayer_t>(text_rm_entity);
    ecos::reg.emplace<gfix::com_color_t>(text_rm_entity, 0xe0);
    ecos::reg.emplace<gfix::com_imreg_t>(text_rm_entity, imreg_t{
        .index = _IMORI_FONT_MAIN,
        .rsize = geom::rsize_t{ geom::RSIZE_MAX, geom::RSIZE_MAX },
    });
    ecos::reg.emplace<gfix::com_fonts_t>(text_rm_entity, gfix::fonts_t{
        .first = ' ',
        .glyph = { .asize = IMORI_FONT_ASIZE },
    });
    ecos::reg.emplace<gfix::com_label_t>(text_rm_entity, "pick info");
    // // // family
    ecos::reg.emplace<ecos::com_family_t>(text_rm_entity, quad_rm_entity);
    // // // signal
    struct text_rm_listener_t {
        void on_game_init()
        {
            this->entity = game::get_picker_ent();
            ecos::reg.get<ecos::com_update_t<game::com_tposi_t>>(this->entity)
            .binder.connect<
            &text_rm_listener_t::on_update_tposi
            >(*this);
            this->on_update_tposi(ecos::reg, this->entity);
        }
        void on_update_tposi(ecos::reg_t&reg, ecos::ent_t ent)
        {
            auto&tposi = reg.get<game::com_tposi_t>(ent);
            label.string = std::format(
                "pick\n"
                "========{:c}"
                "tposi\n"
                "x{:+04d}\n"
                "y{:+04d}\n"
                "========{:c}",
                text::ENDL_CHAR,
                tposi.x,
                tposi.y,
                text::ENDS_CHAR
            );
        }
        gfix::label_t&label;
        ecos::ent_t entity;
    } static text_rm_listener{ ecos::reg.get<gfix::com_label_t>(text_rm_entity) };
    game::init_bot_signal.binder
    .connect<&text_rm_listener_t::on_game_init>(text_rm_listener);
    // keyman
    main::key_bind_set(main::_KEY_MODE_MAIN, "igt", [](v1s_t narg){
        auto&visual = ecos::reg.get<gfix::com_visual_t>(guiman_ent);
        visual.set(visual.vet() ? gfix::visual_t::HARD_HIDE : gfix::visual_t::HARD_SHOW);
        ecos::reg.patch<gfix::com_visual_t>(guiman_ent);
    });
    // final
    return TRUTH;
} // init_unit_guiman
_DEFN_FUNC v1b_t quit_unit_guiman()
{
    _EFNOT(
        ecos::reg.valid(get_guiman_ent()) == TRUTH,
        return FALSE,
        "guiman quit was called twice !? [guiman_ent]={}",
        get_camera_ent()
    );
    // entity
    ecos::reg.destroy(guiman_ent); guiman_ent = entt::null;
    // final
    return TRUTH;
} // quit_unit_guiman

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_UNIT_GUIMAN_CXX
