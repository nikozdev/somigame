#include "head.hxx"

/* headers */

#include "gfix.hxx"
#include "util.hxx"
#include "iput.hxx"
#include "ecos.hxx"
#include "game.hxx"

/* defines */

#undef _HELP_LINE

/* content */

_NAMESPACE_ENTER

/** typedef **/

/** datadef **/

/*** signal ***/

t_signal_t<void(void)> gfix_init_top_signal;
t_signal_t<void(void)> gfix_init_bot_signal;
t_signal_t<void(void)> gfix_work_top_signal;
t_signal_t<void(void)> gfix_work_bot_signal;
t_signal_t<void(void)> gfix_quit_top_signal;
t_signal_t<void(void)> gfix_quit_bot_signal;

extern t_signal_t<void(void)> game_init_bot_signal;

/*** system ***/

struct window_t {
    ratio_t ratio = { .x = VIEW_RATIO_X, .y = VIEW_RATIO_Y };
    apos2_t apos2 = { .x = 0, .y = 0 };
    asize_t asize = { .x = VIEW_ASIZE_X, .y = VIEW_ASIZE_Y };
} window;

/*** assets ***/

imori_t imori_list[_IMORI_COUNT];

/*** meta ***/

constexpr const v1s08_t IMORI_NONE_MSTEP = 4;
constexpr const isize_t IMORI_NONE_ISIZE = { .x = 1, .y = 1, };
ubyte_t IMORI_NONE_MDATA[] = { 0xff,0xff,0xff,0xff };

constexpr const v1s08_t IMORI_TEST_MSTEP = 4;
constexpr const isize_t IMORI_TEST_ISIZE = { .x = 4, .y = 4, };
ubyte_t IMORI_TEST_MDATA[] = {
    0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa,
    0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55,
    0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa,
    0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55,
};

constexpr const asize_t IMORI_FONT_ASIZE{ 8, 8 };

/** actions **/

void gfix_init()
{
    gfix_init_top_signal.holder.publish();
    /* glut setup */
    ::glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    ::glutInitWindowPosition(window.apos2.x, window.apos2.y);
    ::glutInitWindowSize(window.asize.x, window.asize.y);
    ::glutCreateWindow(_NAME_STR);
    /* callbacks */
    ::glutReshapeFunc([](int sizex, int sizey) {
        auto winratx = window.ratio.x;
        auto winraty = window.ratio.y;
        auto winrelw = sizex / winratx;
        auto winrelh = sizey / winraty;
        auto winreld = winrelw - winrelh;
        if (winreld == 0)
        {
            window.apos2.x = 0;
            window.apos2.y = 0;
        }
        else if (winreld < 0)
        {
            window.apos2.x = 0;
            window.apos2.y = -winreld * winraty / 2;
            sizey -= (window.apos2.y * 2);
        }
        else if (winreld > 0)
        {
            window.apos2.x = +winreld * winratx / 2;
            window.apos2.y = 0;
            sizex -= (window.apos2.x * 2);
        }
        window.asize.x = sizex;
        window.asize.y = sizey;
        ::glViewport(window.apos2.x, window.apos2.y, window.asize.x, window.asize.y);
    });
    /* opengl setup */
    _ELOG("[opengl-version] %s\n", ::glGetString(GL_VERSION));
    ::glDisable(GL_DEPTH_TEST);
    ::glDisable(GL_STENCIL_TEST);
    ::glEnable(GL_TEXTURE_1D);
    ::glEnable(GL_TEXTURE_2D);
    ::glDisable(GL_TEXTURE_3D);
    ::glEnable(GL_BLEND);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef _CONF_PLAY
    ::glEnable(GL_CULL_FACE);
    ::glCullFace(GL_BACK);
    ::glFrontFace(GL_CCW);
#else
    ::glDisable(GL_CULL_FACE);
#endif
    ::glShadeModel(GL_FLAT);
    /* imori load */
    stbi_set_flip_vertically_on_load(_TRUTH);
    load_imori_from_value_into_index(imori_t{
        .glint = 0,
        .isize = IMORI_NONE_ISIZE,
        .mstep = IMORI_NONE_MSTEP,
        .msize = IMORI_NONE_MSTEP * IMORI_NONE_ISIZE.x * IMORI_NONE_ISIZE.y,
        .mdata = &IMORI_NONE_MDATA[0]
    }, _IMORI_META_NONE);
    load_imori_from_value_into_index(imori_t{
        .glint = 0,
        .isize = IMORI_TEST_ISIZE,
        .mstep = IMORI_TEST_MSTEP,
        .msize = IMORI_TEST_MSTEP * IMORI_TEST_ISIZE.x * IMORI_TEST_ISIZE.y,
        .mdata = &IMORI_TEST_MDATA[0]
    }, _IMORI_META_TEST);
    load_imori_from_fpath_into_index("rsc/gfix/meta-logo.png", _IMORI_META_LOGO);
    load_imori_from_fpath_into_index("rsc/gfix/game-pick.png", _IMORI_GAME_PICK);
    load_imori_from_fpath_into_index("rsc/gfix/game-hero-ai-f1.png", _IMORI_GAME_HERO);
    load_imori_from_fpath_into_index("rsc/gfix/tile-test-st-tmm.png", _IMORI_TILE_TEST);
    load_imori_from_fpath_into_index("rsc/gfix/font-main-8x8.png", _IMORI_FONT_MAIN);
    /* imreg component signal */
    entt::sigh_helper{ ecos }
    .with<com_imreg_t>().on_update<[](ecos_t&ecos, ent_t ent){
        auto&imreg = ecos.get<com_imreg_t>(ent);
        if (imreg.index < 0 || imreg.index >= _IMORI_COUNT)
        { imreg.index = _IMORI_META_TEST; }
    }>().on_construct<[](ecos_t&ecos, ent_t ent){
        auto&imreg = ecos.get<com_imreg_t>(ent);
        if (imreg.index < 0 || imreg.index >= _IMORI_COUNT)
        { imreg.index = _IMORI_META_TEST; }
        if (ecos.try_get<com_faces_t>(ent) != _NULL)
        { ecos.remove<com_faces_t>(ent); }
    }>().on_destroy<[](ecos_t&ecos, ent_t ent){
        auto&imreg = ecos.get<com_imreg_t>(ent);
    }>();
    /* faces component signal */
    entt::sigh_helper{ ecos }
    .with<com_faces_t>().on_update<[](ecos_t&ecos, ent_t ent){
    }>().on_construct<[](ecos_t&ecos, ent_t ent){
        auto&faces = ecos.get<com_faces_t>(ent);
    }>().on_destroy<[](ecos_t&ecos, ent_t ent){
        auto&faces = ecos.get<com_faces_t>(ent);
    }>();
    /* vrange */
#if _FALSE
    struct vrange_listener_t
    {
        void update_vrange(ecos_t&ecos, ent_t ent)
        {
            auto vrange = ecos.get<com_vrange_t>(ent);
            auto range1 = vrange.range;
            auto range2 = range1*range1;
            zpos1_t zpos1 = ecos.get<com_zpos1_t>(ent);
            for (auto&&[entity, visual, tcell, tpos3]
                : ecos.view<
                com_visual_t, com_tcell_t, com_tpos3_t
                >().each()
            ) {
                if (tpos3.z > zpos1.z
                    && ((tpos3.x*tpos3.x + tpos3.y*tpos3.y
                    ) <= range2)
                ) {
                    ecos.replace<com_visual_t>(entity, -1);
                }
                else
                {
                    ecos.replace<com_visual_t>(entity, +1);
                }
            }
        } /* update */
        void update_tcell(ecos_t&ecos, ent_t ent)
        {
            for (auto entity : ecos.view<com_vrange_t>())
            { ecos.patch<com_vrange_t>(entity); }
        }
    } static vrange_listener;
    entt::sigh_helper{ ecos }
    .with<com_vrange_t>().on_update<
        &vrange_listener_t::update_vrange
        >(vrange_listener).on_construct<
        &vrange_listener_t::update_vrange
        >(vrange_listener);
    entt::sigh_helper{ ecos }
    .with<com_tcell_t>().on_construct<
        &vrange_listener_t::update_tcell
        >(vrange_listener);
#endif
    /* view */
    auto view_entity= ecos.ctx()
        .emplace_as<entity_t>("view_entity"_hstr, ecos.create());
    /* sizes */
    ecos.emplace<com_asize_t>(view_entity, asize_t{
        .x = VIEW_ASIZE_X,
        .y = VIEW_ASIZE_Y,
    });
    ecos.emplace<com_ratio_t>(view_entity);
    /* coord */
    ecos.emplace<com_apos2_t>(view_entity);
    ecos.emplace<com_zpos1_t>(view_entity, TCELL_CFULL_Z);
    /* geometry */
    ecos.emplace<com_direc_t>(view_entity, -0, +1);
    /* signal */
    struct view_listener_t {
        void on_grect_update(ecos_t&ecos, ent_t ent)
        {
            auto&grect = ecos.get<com_grect_t>(ent);
            ::glMatrixMode(GL_PROJECTION);
            ::glLoadIdentity();
            ::gluOrtho2D(
                grect.x.l, grect.x.r,
                grect.y.b, grect.y.t
            );
        }
        entity_t entity = entt::null;
    } static view_listener;
    view_listener.entity = view_entity;
    ecos.emplace<com_update_t<com_grect_t>>(view_entity)
        .binder.connect<
        &view_listener_t::on_grect_update
        >(view_listener);
    /* family */
    ecos.emplace<com_family_t>(view_entity);
    /** gui-mm **/
    auto quad_mm_entity = ecos.create();
    /** sizes **/
    ecos.emplace<com_rsize_t>(quad_mm_entity, rsize_t{
        .x = RSIZE_MAX - GUIS_RSIZE_X*4,
        .y = RSIZE_MAX - GUIS_RSIZE_Y*2,
    });
    /** visual **/
    ecos.emplace<com_visual_t>(quad_mm_entity);
    ecos.emplace<com_rlayer_t>(quad_mm_entity);
    /** family **/
    ecos.emplace<com_family_t>(quad_mm_entity, view_entity);
    /*** died ***/
    entity_t text_died_entity = ecos.create();
    /*** sizes ***/
    ecos.emplace<com_rsize_t>(text_died_entity, rsize_t{
        .x = RSIZE_MAX,
        .y = GUIS_RSIZE_Y,
    });
    /*** visual ***/
    auto&visual= ecos.emplace<com_visual_t>(text_died_entity);
    ecos.emplace<com_rlayer_t>(text_died_entity);
    ecos.emplace<com_color_t>(text_died_entity, 0xe0);
    ecos.emplace<com_imreg_t>(text_died_entity, _IMORI_FONT_MAIN);
    ecos.emplace<com_fonts_t>(text_died_entity, fonts_t{
        .first = ' ',
        .glyph = {
            .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y },
        },
    });
    ecos.emplace<com_strbuf_t>(text_died_entity, strbuf_t{
        .msize = 8,
        .mdata = "YouDied",
    });
    /*** family ***/
    ecos.emplace<com_family_t>(text_died_entity, quad_mm_entity);
    /*** signal ***/
    struct text_died_listener_t {
        void on_init_game()
        {
            ecos.get<com_update_t<com_alive_t>>(
                ecos.ctx().get<ent_t>("hero_entity"_hstr)
            ).binder.connect<&text_died_listener_t::on_update_alive>(*this);
        }
        void on_update_alive(ecos_t&ecos, ent_t ent)
        {
            const auto&alive = ecos.get<com_alive_t>(ent);
            this->visual->set(
                visual_t::HARD_HIDE * (get_num_sign(-1*alive.valid))
            );
            this->visual =&ecos.patch<com_visual_t>(this->entity);
        }
        entity_t entity;
        visual_t*visual;
    } static text_died_listener{text_died_entity,&visual};
    game_init_bot_signal.binder.connect<
        &text_died_listener_t::on_init_game
        >(text_died_listener);
    /** gui-mb **/
    entity_t quad_mb_entity = ecos.create();
    /** sizes **/
    ecos.emplace<com_rsize_t>(quad_mb_entity, rsize_t{
        .x = RSIZE_MAX,
        .y = GUIS_RSIZE_Y,
    });
    /** coord **/
    ecos.emplace<com_rpos2_t>(quad_mb_entity, rpos2_t{
        .x = RPOS2_MID,
        .y = RPOS2_MIN,
    });
    ecos.emplace<com_pivot_t>(quad_mb_entity, pivot_t{
        .x = PIVOT_MID,
        .y = PIVOT_MIN,
    });
    /** visual **/
    ecos.emplace<com_visual_t>(quad_mb_entity);
    ecos.emplace<com_rlayer_t>(quad_mb_entity);
    ecos.emplace<com_color_t>(quad_mb_entity, 0x40);
    ecos.emplace<com_imreg_t>(quad_mb_entity, _IMORI_META_NONE);
    /** family **/
    ecos.emplace<com_family_t>(quad_mb_entity, view_entity);
    /*** label-mb ***/
    auto text_mb_entity = ecos.create();
    /*** sizes ***/
    ecos.emplace<com_rsize_t>(text_mb_entity, rsize_t{
        .x = RSIZE_MAX,
        .y = RSIZE_MAX,
    });
    /*** visual ***/
    ecos.emplace<com_visual_t>(text_mb_entity);
    ecos.emplace<com_rlayer_t>(text_mb_entity);
    ecos.emplace<com_color_t>(text_mb_entity, 0xe0);
    ecos.emplace<com_imreg_t>(text_mb_entity, _IMORI_FONT_MAIN);
    ecos.emplace<com_fonts_t>(text_mb_entity, fonts_t{
        .first = ' ',
        .glyph = {
            .asize = asize_t{
                .x = IMORI_FONT_ASIZE.x,
                .y = IMORI_FONT_ASIZE.y,
            },
        },
    });
    ecos.emplace<com_strbuf_t>(text_mb_entity, strbuf_t{
        .msize = 128,
        .mdata = "",
    });
    /*** family ***/
    ecos.emplace<com_family_t>(text_mb_entity, quad_mb_entity);
    /*** signal ***/
    struct text_mb_listener_t {
        void on_game_init()
        {
            this->hero_entity = ecos.ctx()
                .get<ent_t>("hero_entity"_hstr);
            timer.dispatcher.sink<update_event_t>().connect<
                &text_mb_listener_t::on_update>(*this);
        }
        void on_update(update_event_t event)
        {
            memset(strbuf.mdata, '\0', strbuf.msize);
            std::snprintf(
                strbuf.mdata, strbuf.msize,
                "mode=%s;acts=%08d;"
                "%c"
                "%s(%+d)x(%d)",
                get_key_mode_name(),
                ecos.get<com_actor_t>(hero_entity).count,
                '\n',
                &get_key_line()[0],
                get_key_narg_sign(),
                get_key_narg()
            );
        }
        strbuf_t&strbuf;
        entity_t hero_entity;
    } static text_mb_listener{ ecos.get<com_strbuf_t>(text_mb_entity) };
    game_init_bot_signal
        .binder.connect<&text_mb_listener_t::on_game_init>(text_mb_listener);
    /** gui-mt **/
    entity_t quad_mt_entity = ecos.create();
    /** sizes **/
    ecos.emplace<com_rsize_t>(quad_mt_entity, rsize_t{
        .x = RSIZE_MAX,
        .y = GUIS_RSIZE_Y,
    });
    /** coord **/
    ecos.emplace<com_rpos2_t>(quad_mt_entity, rpos2_t{
        .x = RPOS2_MID,
        .y = RPOS2_MAX,
    });
    ecos.emplace<com_pivot_t>(quad_mt_entity, pivot_t{
        .x = PIVOT_MID,
        .y = PIVOT_MAX,
    });
    /** visual **/
    ecos.emplace<com_visual_t>(quad_mt_entity);
    ecos.emplace<com_rlayer_t>(quad_mt_entity);
    ecos.emplace<com_color_t>(quad_mt_entity, color_t{
        .v = 0x40,
    });
    ecos.emplace<com_imreg_t>(quad_mt_entity, _IMORI_META_NONE);
    /** family **/
    ecos.emplace<com_family_t>(quad_mt_entity, view_entity);
    /*** label-mt ***/
    auto text_mt_entity = ecos.create();
    /*** sizes ***/
    ecos.emplace<com_rsize_t>(text_mt_entity, rsize_t{
        .x = RSIZE_MAX,
        .y = RSIZE_MAX,
    });
    /*** visual ***/
    ecos.emplace<com_visual_t>(text_mt_entity);
    ecos.emplace<com_rlayer_t>(text_mt_entity);
    ecos.emplace<com_color_t>(text_mt_entity, 0xe0);
    ecos.emplace<com_imreg_t>(text_mt_entity, _IMORI_FONT_MAIN);
    ecos.emplace<com_fonts_t>(text_mt_entity, fonts_t{
        .first = ' ',
        .glyph = {
            .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y, },
        },
    });
    ecos.emplace<com_strbuf_t>(text_mt_entity, 128);
    /*** family ***/
    ecos.emplace<com_family_t>(text_mt_entity, quad_mt_entity);
    /*** signal ***/
    struct text_mt_listener_t
    {
        void on_update(ecos_t&ecos, ent_t ent) {
            memset(strbuf.mdata, '\0', strbuf.msize);
            auto view_entity = ecos.ctx().get<ent_t>("view_entity"_hstr);
            auto view_gpos3 = ecos.get<com_gpos3_t>(view_entity);
            auto view_direc = ecos.get<com_direc_t>(view_entity);
            std::snprintf(
                strbuf.mdata, strbuf.msize,
                "now=%03zu.%03zu;fps=%05zu;"
                "%c"
                "g(x%+04iy%+04iz%+04i)d(x%+1dy%+1d)"
                "%c",
                (timer.now_mil / 1000) % 1000, timer.now_mil % 1000, timer.fps_num,
                '\n',
                view_gpos3.x, view_gpos3.y, view_gpos3.z,
                view_direc.x, view_direc.y,
                '\0'
            );
        }
        strbuf_t&strbuf;
    } static text_mt_listener{ ecos.get<com_strbuf_t>(text_mt_entity) };
    ecos.get<com_update_t<com_grect_t>>(view_entity).binder.connect<
        &text_mt_listener_t::on_update
        >(text_mt_listener);
    /** gui-lm **/
    entity_t quad_lm_entity = ecos.create();
    /** sizes **/
    ecos.emplace<com_rsize_t>(quad_lm_entity, rsize_t{
        .x = GUIS_RSIZE_X,
        .y = RSIZE_MAX - GUIS_RSIZE_Y*2,
    });
    /** visual **/
    ecos.emplace<com_visual_t>(quad_lm_entity);
    ecos.emplace<com_rlayer_t>(quad_lm_entity);
    ecos.emplace<com_color_t>(quad_lm_entity, 0x40);
    ecos.emplace<com_imreg_t>(quad_lm_entity, _IMORI_META_NONE);
    /** coord **/
    ecos.emplace<com_rpos2_t>(quad_lm_entity, rpos2_t{
        .x = RPOS2_MIN,
        .y = RPOS2_MID,
    });
    ecos.emplace<com_pivot_t>(quad_lm_entity, pivot_t{
        .x = PIVOT_MIN,
        .y = PIVOT_MID,
    });
    /** family **/
    ecos.emplace<com_family_t>(quad_lm_entity, view_entity);
    /*** label-lm ***/
    entity_t text_lm_entity = ecos.create();
    /*** sizes ***/
    ecos.emplace<com_rsize_t>(text_lm_entity, rsize_t{
        .x = RSIZE_MAX,
        .y = RSIZE_MAX,
    });
    /*** coord ***/
    ecos.emplace<com_rpos2_t>(text_lm_entity, rpos2_t{
        .x = RPOS2_MIN,
        .y = RPOS2_MIN,
    });
    ecos.emplace<com_pivot_t>(text_lm_entity, pivot_t{
        .x = PIVOT_MIN,
        .y = PIVOT_MIN,
    });
    /*** visual ***/
    ecos.emplace<com_visual_t>(text_lm_entity);
    ecos.emplace<com_rlayer_t>(text_lm_entity);
    ecos.emplace<com_color_t>(text_lm_entity, 0xe0);
    ecos.emplace<com_imreg_t>(text_lm_entity, _IMORI_FONT_MAIN);
    ecos.emplace<com_fonts_t>(text_lm_entity, fonts_t{
        .first = ' ',
        .glyph = {
            .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y, },
        },
    });
    ecos.emplace<com_strbuf_t>(text_lm_entity, strbuf_t{
        .msize = 128,
        .mdata = "",
    });
    /*** family ***/
    ecos.emplace<com_family_t>(text_lm_entity, quad_lm_entity);
    /** gui-rm **/
    auto quad_rm_entity = ecos.create();
    /** coord **/
    ecos.emplace<com_rpos2_t>(quad_rm_entity, rpos2_t{
        .x = RPOS2_MAX,
        .y = RPOS2_MID,
    });
    ecos.emplace<com_pivot_t>(quad_rm_entity, pivot_t{
        .x = PIVOT_MAX,
        .y = PIVOT_MID,
    });
    /** sizes **/
    ecos.emplace<com_rsize_t>(quad_rm_entity, rsize_t{
        .x = GUIS_RSIZE_X,
        .y = RSIZE_MAX - GUIS_RSIZE_Y*2,
    });
    /** visual **/
    ecos.emplace<com_visual_t>(quad_rm_entity);
    ecos.emplace<com_rlayer_t>(quad_rm_entity);
    ecos.emplace<com_color_t>(quad_rm_entity, 0x40);
    ecos.emplace<com_imreg_t>(quad_rm_entity, _IMORI_META_NONE);
    /** family **/
    ecos.emplace<com_family_t>(quad_rm_entity, view_entity);
    /*** label-rm ***/
    auto text_rm_entity = ecos.create();
    /*** sizes ***/
    ecos.emplace<com_rsize_t>(text_rm_entity, rsize_t{
        .x = RSIZE_MAX,
        .y = RSIZE_MAX,
    });
    /*** coord ***/
    ecos.emplace<com_rpos2_t>(text_rm_entity, rpos2_t{
        .x = RPOS2_MIN,
        .y = RPOS2_MIN,
    });
    ecos.emplace<com_pivot_t>(text_rm_entity, pivot_t{
        .x = PIVOT_MIN,
        .y = PIVOT_MIN,
    });
    /*** visual ***/
    ecos.emplace<com_visual_t>(text_rm_entity);
    ecos.emplace<com_rlayer_t>(text_rm_entity);
    ecos.emplace<com_color_t>(text_rm_entity, 0xe0);
    ecos.emplace<com_imreg_t>(text_rm_entity, imreg_t{
        .index = _IMORI_FONT_MAIN,
        .rsize = rsize_t{ RSIZE_MAX, RSIZE_MAX },
    });
    ecos.emplace<com_fonts_t>(text_rm_entity, fonts_t{
        .first = ' ',
        .glyph = {
            .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y, },
        },
    });
    ecos.emplace<com_strbuf_t>(text_rm_entity, strbuf_t{
        .msize = 128,
        .mdata = "pick info",
    });
    /*** family ***/
    ecos.emplace<com_family_t>(text_rm_entity, quad_rm_entity);
    /*** signal ***/
    struct text_rm_listener_t {
        void on_game_init()
        {
            this->entity = ecos.ctx().get<ent_t>("pick_entity"_hstr);
            ecos.get<com_update_t<com_tpos3_t>>(entity).binder.connect<
                &text_rm_listener_t::on_update_tpos3
                >(*this);
            this->on_update_tpos3(ecos, this->entity);
        }
        void on_update_tpos3(ecos_t&ecos, ent_t ent)
        {
            auto&tpos3 = ecos.get<com_tpos3_t>(ent);
            std::snprintf(
                strbuf.mdata, strbuf.msize,
                "pick\n"
                "========%c"
                "tpose\n"
                "x%+04i\n"
                "y%+04i\n"
                "z%+04i\n"
                "========%c",
                '\n',
                tpos3.x,
                tpos3.y,
                tpos3.z,
                '\0'
            );
        }
        strbuf_t&strbuf;
        entity_t entity;
    } static text_rm_listener{ ecos.get<com_strbuf_t>(text_rm_entity) };
    game_init_bot_signal.binder.connect<
        &text_rm_listener_t::on_game_init
        >(text_rm_listener);
    /* init is done */
    gfix_init_bot_signal.holder.publish();
}
void gfix_quit()
{
    gfix_quit_top_signal.holder.publish();
    gfix_quit_bot_signal.holder.publish();
}
void gfix_work()
{
    gfix_work_top_signal.holder.publish();
    /* clear */
    ::glutSwapBuffers();
    ::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ::glClear(GL_COLOR_BUFFER_BIT);
    /* prepare the view */
    auto view_entity = ecos.ctx().get<ent_t>("view_entity"_hstr);
    auto view_grect = ecos.get<com_grect_t>(view_entity);
    /* count all visible components */
    count_t count = 0;
    for (auto&&
        [entity, visual]
        : ecos.view<
        com_visual_t
        >().each()
    ) { count++; }
    /* make a list sorted by depth and layers */
    typedef struct drawable_t {
        entity_t entity;
        glayer_t glayer;
        gpos3_t gpos3;
        grect_t grect;
    } drawable_t;
    typedef struct drawlist_t {
        drawable_t*head,*back,*tail;
    } drawlist_t;
    drawlist_t drawlist;
    memo::owner->new_mul(drawlist.head, count);
    drawlist.back = drawlist.head;
    drawlist.tail = drawlist.head + count;
    auto hero_entity = ecos.ctx().get<entity_t>("hero_entity"_hstr);
    auto hero_tpos3 = ecos.get<com_tpos3_t>(hero_entity);
    for (auto
        [entity, visual, glayer, gpos3_e, tpos3_e, grect_e]
        : ecos.view<
        com_visual_t, com_glayer_t, com_gpos3_t, com_tpos3_t, com_grect_t
        >().each()
    ) {
        if constexpr (_TRUTH)
        { /* relative to every vrange owner */
            for (auto&&
                [entity, vrange, tpos3_v]
                : ecos.view<com_vrange_t, com_tpos3_t>().each()
            ) {
                tpos3_t tdiff = {
                    .x = tpos3_e.x - tpos3_v.x,
                    .y = tpos3_e.y - tpos3_v.y,
                    .z = tpos3_e.z - tpos3_v.z,
                };
                auto range2 = vrange.range * vrange.range;
                if ((tdiff.z > 0)
                    && ((tdiff.x*tdiff.x + tdiff.y*tdiff.y)
                        <= range2)
                ) { visual.set(visual_t::SOFT_HIDE); break; }
                else {  visual.set(visual_t::SOFT_SHOW); }
            }
        } /* relative to every vrange owner */
        if (visual.vet() && ((_FALSE
        ) || (_TRUTH
        && ((grect_e.x.l <= view_grect.x.r) && (grect_e.y.b <= view_grect.y.t))
        && ((grect_e.x.l >= view_grect.x.l) && (grect_e.y.b >= view_grect.y.b))
        ) || (_TRUTH
        && ((grect_e.x.l <= view_grect.x.r) && (grect_e.y.t <= view_grect.y.t))
        && ((grect_e.x.l >= view_grect.x.l) && (grect_e.y.t >= view_grect.y.b))
        ) || (_TRUTH
        && ((grect_e.x.r <= view_grect.x.r) && (grect_e.y.b <= view_grect.y.t))
        && ((grect_e.x.r >= view_grect.x.l) && (grect_e.y.b >= view_grect.y.b))
        ) || (_TRUTH
        && ((grect_e.x.r <= view_grect.x.r) && (grect_e.y.t <= view_grect.y.t))
        && ((grect_e.x.r >= view_grect.x.l) && (grect_e.y.t >= view_grect.y.b))
        ) || (_TRUTH
        && ((grect_e.x.l <= view_grect.x.l) && (grect_e.x.r >= view_grect.x.r))
        && ((grect_e.y.b >= view_grect.y.b) || (grect_e.y.t >= view_grect.y.t))
        ) || (_TRUTH
        && ((grect_e.y.b <= view_grect.y.b) && (grect_e.y.t >= view_grect.y.t))
        && ((grect_e.x.l >= view_grect.x.l) || (grect_e.x.r <= view_grect.x.r))
        )) && ((grect_e.x.l != grect_e.x.r) || (grect_e.y.b != grect_e.y.t))) {
            gpos3_t gpos3 = gpos3_e;
            grect_t grect = grect_e;
            if constexpr (_TRUTH)
            { /* relative to hero */
                if (ecos.try_get<com_zpos1_t>(entity))
                {
                    tpos3_t tdiff = {
                        .x = tpos3_e.x - hero_tpos3.x,
                        .y = tpos3_e.y - hero_tpos3.y,
                        .z = tpos3_e.z - hero_tpos3.z,
                    };
                    if (auto*color = ecos.try_get<com_color_t>(entity))
                    { color->v = 0xff - (std::abs(tdiff.z) * 0x40); }
                    grect.y.b += tdiff.z * TCELL_ASIZE_Y;
                    grect.y.t += tdiff.z * TCELL_ASIZE_Y;
                }
            } /* relative to hero */
            *drawlist.back++ = drawable_t{
                .entity = entity,
                .glayer = glayer,
                .gpos3 = gpos3,
                .grect = grect,
            };
        }
    } /* fill the drawlist */
    static constexpr auto sortfunc = [](
        const drawable_t&sortl,
        const drawable_t&sortr
    ) {
        if (sortl.gpos3.z == sortr.gpos3.z)
        { return sortl.glayer.layer < sortr.glayer.layer; }
        else
        { return sortl.gpos3.z < sortr.gpos3.z; }
    };
    drawlist.tail = drawlist.back + 1;
    std::sort(drawlist.head, drawlist.tail, sortfunc);
    for (auto*drawable = drawlist.head;
        drawable < drawlist.tail; drawable++
    ) {
        entity_t entity = drawable->entity;
        grect_t grect = drawable->grect;
        color_t color = { 0xff };
        if (auto*tcomp = ecos.try_get<com_color_t>(entity))
        { color =*tcomp; }
        ::glColor3ub(color.v, color.v, color.v); 
        if (auto*imreg = ecos.try_get<com_imreg_t>(entity);
            auto*irect = ecos.try_get<com_irect_t>(entity)
        ) { /* quads */
            imori_t*imori = get_imori_from_index(imreg->index);
            isize_t imori_isize = imori->isize;
            frect_t frect = {
                .x = {
                    .l = static_cast<v1f_t>(irect->x.l)
                        /static_cast<v1f_t>(imori_isize.x),
                    .r = static_cast<v1f_t>(irect->x.r)
                        /static_cast<v1f_t>(imori_isize.x),
                },
                .y = {
                    .b = static_cast<v1f_t>(irect->y.b)
                        /static_cast<v1f_t>(imori_isize.y),
                    .t = static_cast<v1f_t>(irect->y.t)
                        /static_cast<v1f_t>(imori_isize.y),
                },
            };
            ::glBindTexture(GL_TEXTURE_2D, imori->glint);
            ::glBegin(GL_QUADS);
            if (auto*strbuf = ecos.try_get<com_strbuf_t>(entity);
                auto*fonts = ecos.try_get<com_fonts_t>(entity)
            ) { /* string */
                /* image values */
                isize_t imreg_isize = {
                    .x = irect->x.r - irect->x.l,
                    .y = irect->y.t - irect->y.b,
                };
                fsize_t imreg_fsize = {
                    .x = static_cast<v1f_t>(imreg_isize.x)
                        /static_cast<v1f_t>(imori_isize.x),
                    .y = static_cast<v1f_t>(imreg_isize.y)
                        /static_cast<v1f_t>(imori_isize.y),
                };
                isize_t glyph_isize = {
                    .x = fonts->glyph.asize.x
                        + ((imreg_isize.x * fonts->glyph.rsize.x)
                            / RSIZE_DIV),
                    .y = fonts->glyph.asize.y
                        + ((imreg_isize.y * fonts->glyph.rsize.y)
                            / RSIZE_DIV),
                };
                fsize_t glyph_fsize = {
                    .x = static_cast<v1f_t>(glyph_isize.x)
                        /static_cast<v1f_t>(imreg_isize.x),
                    .y = static_cast<v1f_t>(glyph_isize.y)
                        /static_cast<v1f_t>(imreg_isize.y),
                };
                isize_t glyph_count = {
                    .x = imreg_isize.x / glyph_isize.x,
                    .y = imreg_isize.y / glyph_isize.y,
                };
                /* grect values */
                msize_t msize = strbuf->msize;
                mbyte_t*mdata = strbuf->mdata;
                asize_t schar_count = { 1, 1 };
                count_t schar_count_x = 0;
                for (auto index = 0;
                    (mdata[index] > 0)
                    && (index < msize);
                    index = index + 1
                ) { /* count characters */
                    if (strbuf->mdata[index] == '\n')
                    {
                        schar_count.y += 1;
                        schar_count_x = 0;
                    }
                    else
                    {
                        schar_count_x++;
                        schar_count.x = _NUM_MAX(
                            schar_count.x,
                            schar_count_x
                        );
                    }
                } /* count characters */
                gsize_t label_gsize = {
                    .x = grect.x.r - grect.x.l,
                    .y = grect.y.t - grect.y.b,
                };
                gsize_t schar_gsize = {
                    .x = label_gsize.x / schar_count.x,
                    .y = label_gsize.y / schar_count.y,
                };
                schar_gsize.x = _NUM_MIN(schar_gsize.x, schar_gsize.y);
                schar_gsize.y = _NUM_MIN(schar_gsize.y, schar_gsize.x);
                gpos3_t label_gpos2 = {
                    .x = grect.x.l - schar_gsize.x,
                    .y = grect.y.t,
                };
                gpos3_t schar_gpos2 = {
                    .x = label_gpos2.x,
                    .y = label_gpos2.y,
                };
                for (auto index = 0;
                    (mdata[index] > 0)
                    && (index < msize);
                    index = index + 1
                ) { /* text rendering */
                    v1s_t mbyte = strbuf->mdata[index];
                    /* schar grect */
                    if (mbyte == _ENDL_SYM)
                    {
                        schar_gpos2.y -= schar_gsize.y;
                        schar_gpos2.x = label_gpos2.x;
                    }
                    else
                    {
                        schar_gpos2.x += schar_gsize.x;
                    }
                    grect_t schar_grect = {
                        .x = {
                            .l = schar_gpos2.x,
                            .r = schar_gpos2.x + schar_gsize.x,
                        },
                        .y = {
                            .b = schar_gpos2.y - schar_gsize.y,
                            .t = schar_gpos2.y,
                        },
                    };
                    mbyte = _NUM_MAX(
                        mbyte, fonts->first
                    ) - fonts->first;
                    /* glyph frect */
                    ipos2_t glyph_ipos2 = {
                        .x = (mbyte % glyph_count.x)
                            * glyph_isize.x,
                        .y = (glyph_count.y - (mbyte / glyph_count.x))
                            * glyph_isize.y,
                    };
                    fpos2_t glyph_fpos2 = {
                        .x = static_cast<v1f_t>(glyph_ipos2.x)
                            /static_cast<v1f_t>(imreg_isize.x),
                        .y = static_cast<v1f_t>(glyph_ipos2.y)
                            /static_cast<v1f_t>(imreg_isize.y),
                    };
                    frect_t glyph_frect = {
                        .x = {
                            .l = glyph_fpos2.x,
                            .r = glyph_fpos2.x + glyph_fsize.x,
                        },
                        .y = {
                            .b = glyph_fpos2.y - glyph_fsize.y,
                            .t = glyph_fpos2.y,
                        },
                    };
                    /* drawing */
                    ::glTexCoord2f(glyph_frect.x.l, glyph_frect.y.b);
                    ::glVertex2i(schar_grect.x.l, schar_grect.y.b);
                    ::glTexCoord2f(glyph_frect.x.r, glyph_frect.y.b);
                    ::glVertex2i(schar_grect.x.r, schar_grect.y.b);
                    ::glTexCoord2f(glyph_frect.x.r, glyph_frect.y.t);
                    ::glVertex2i(schar_grect.x.r, schar_grect.y.t);
                    ::glTexCoord2f(glyph_frect.x.l, glyph_frect.y.t);
                    ::glVertex2i(schar_grect.x.l, schar_grect.y.t);
                } /* text rendering */
            } /* string */
            else
            { /* sprite */
                ::glTexCoord2f(frect.x.l, frect.y.b);
                ::glVertex2i(grect.x.l, grect.y.b);
                ::glTexCoord2f(frect.x.r, frect.y.b);
                ::glVertex2i(grect.x.r, grect.y.b);
                ::glTexCoord2f(frect.x.r, frect.y.t);
                ::glVertex2i(grect.x.r, grect.y.t);
                ::glTexCoord2f(frect.x.l, frect.y.t);
                ::glVertex2i(grect.x.l, grect.y.t);
            } /* sprite */
            ::glEnd();
            ::glBindTexture(GL_TEXTURE_2D, 0);
        } /* quads */
        if (auto*lgrid = ecos.try_get<com_lgrid_t>(entity))
        { /* lgrid */
            ::glBegin(GL_QUADS);
            const auto&cells = lgrid->cells;
            const auto&tails = lgrid->tails;
            const auto&lsize = lgrid->lsize;
            const auto fromy = grect.y.b - tails.y;
            const auto intoy = grect.y.t + tails.y;
            for (auto stepx = grect.x.l; stepx <= grect.x.r; stepx += cells.x)
            { /* l-to-r */
                ::glVertex2i(stepx - lsize.x, fromy);
                ::glVertex2i(stepx + lsize.x, fromy);
                ::glVertex2i(stepx + lsize.x, intoy);
                ::glVertex2i(stepx - lsize.x, intoy);
            } /* l-to-r */
            const auto fromx = grect.x.l - tails.x;
            const auto intox = grect.x.r + tails.x;
            for (auto stepy = grect.y.b; stepy <= grect.y.t; stepy += cells.x)
            { /* b-to-t */
                ::glVertex2i(fromx, stepy - lsize.y);
                ::glVertex2i(fromx, stepy + lsize.y);
                ::glVertex2i(intox, stepy + lsize.y);
                ::glVertex2i(intox, stepy - lsize.y);
            } /* b-to-t */
            ::glEnd();
        } /* lgrid */
#if defined(_CONF_WORK) && defined(_HELP_LINE)
        ::glColor3ub(0x00, 0xff, 0x00);
        ::glBegin(GL_LINE_STRIP);
        ::glVertex2i(grect.x.l, grect.y.b);
        ::glVertex2i(grect.x.r, grect.y.b);
        ::glVertex2i(grect.x.r, grect.y.t);
        ::glVertex2i(grect.x.l, grect.y.t);
        ::glEnd();
        if (gpos3_t*gpos3 = ecos.try_get<com_gpos3_t>(entity))
        {
            auto gpos2 =*gpos3;
            ::glBegin(GL_QUADS);
            ::glVertex2i(grect.x.l, gpos2.y-1);
            ::glVertex2i(grect.x.r, gpos2.y-1);
            ::glVertex2i(grect.x.r, gpos2.y+1);
            ::glVertex2i(grect.x.l, gpos2.y+1);
            ::glVertex2i(gpos2.x-1, grect.y.b);
            ::glVertex2i(gpos2.x-1, grect.y.t);
            ::glVertex2i(gpos2.x+1, grect.y.t);
            ::glVertex2i(gpos2.x+1, grect.y.b);
            ::glEnd();
        }
#endif
    }
    memo::owner->del_mul(drawlist.head, count);
    gfix_work_bot_signal.holder.publish();
}

/*** files ***/

static void proc_imori(imori_t&imori)
{
    imori.msize = imori.isize.x * imori.isize.y * imori.mstep;
    GLuint glint = 0;
    ::glGenTextures(1, &glint);
    ::glBindTexture(GL_TEXTURE_2D, glint);
    ::glTexImage2D(
        GL_TEXTURE_2D, 0,
        imori.mstep, imori.isize.x, imori.isize.y, 0,
        imori.mstep == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
        imori.mdata
    );
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    ::glBindTexture(GL_TEXTURE_2D, 0);
    imori.glint = glint;
}

void load_imori_from_fpath_into_value(const std::string&fpath, imori_t&imori)
{
    imori.mdata = stbi_load(&fpath[0], &imori.isize.x, &imori.isize.y, &imori.mstep, 0);
    if (imori.mdata) { proc_imori(imori); }
    else { std::cerr << "failed imori loading: " << fpath << std::endl; }
}

void load_imori_from_fpath_into_index(const std::string&fpath, index_t index)
{
    if (index < _IMORI_COUNT)
    {
        load_imori_from_fpath_into_value(fpath, imori_list[index]);
    }
    else
    {
    }
}

void load_imori_from_value_into_index(const imori_t&imori, index_t index)
{
    if (index < _IMORI_COUNT)
    {
        imori_list[index] = imori;
        proc_imori(imori_list[index]);
    }
    else
    {
    }
}

/*** view ***/

void view_turn(const bool_t lside)
{
    auto view_entity = ecos.ctx().get<ent_t>("view_entity"_hstr);
    auto view_direc = ecos.get<com_direc_t>(view_entity);
    ecos.replace<com_direc_t>(view_entity,
        get_vec_turn(view_direc, lside)
    );
}

/** getters **/

imori_t*get_imori_from_index(index_t index)
{ return&imori_list[index % _IMORI_COUNT]; }

_NAMESPACE_LEAVE
