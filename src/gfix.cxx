#include "head.hxx"

/* headers */

#include "gfix.hxx"
#include "util.hxx"
#include "iput.hxx"
#include "ecos.hxx"
#include "game.hxx"

/* content */

_NAMESPACE_ENTER

/** typedef **/

/** datadef **/

struct view_t view = {
    .entity= entt::null,
    /* sizes */
    .asize = _NULL,
    .gsize = _NULL,
    .tsize = _NULL,
    .ratio = _NULL,
    /* coord */
    .apos2 = _NULL,
    .zpos1 = _NULL,
    .gpos3 = _NULL,
    .tpos3 = _NULL,
    /* geometry */
    .direc = _NULL,
    .grect = _NULL,
    .trect = _NULL,
    /* visual */
    .visual= _NULL,
    /* family */
    .family= _NULL,
};

struct helpgrid_t helpgrid = {
    .entity= entt::null,
    /* sizes */
    .asize = _NULL,
    .gsize = _NULL,
    .tsize = _NULL,
    /* coord */
    .gpos3 = _NULL,
    .tpos3 = _NULL,
    /* visual */
    .visual= _NULL,
    .lgrid = _NULL,
    /* family */
    .family= _NULL,
};

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
    if constexpr (_TRUTH)
    { /* glut setup */
        ::glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
        ::glutInitWindowPosition(window.apos2.x, window.apos2.y);
        ::glutInitWindowSize(window.asize.x, window.asize.y);
        ::glutCreateWindow(_NAME_STR);
        /* callbacks */
        ::glutDisplayFunc(::glutSwapBuffers);
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
            /* camera */
            view.asize->x = VIEW_ASIZE_X;
            view.asize->y = VIEW_ASIZE_Y;
            ecos.patch<com_asize_t>(view.entity);
            view.grect->x.l = -view.asize->x / 2 + view.apos2->x;
            view.grect->x.r = +view.asize->x / 2 + view.apos2->x;
            view.grect->y.b = -view.asize->y / 2 + view.apos2->y;
            view.grect->y.t = +view.asize->y / 2 + view.apos2->y;
            ecos.patch<com_grect_t>(view.entity);
            view.trect->x.l = view.grect->x.l / TCELL_ASIZE_X;
            view.trect->x.r = view.grect->x.r / TCELL_ASIZE_X;
            view.trect->y.b = view.grect->y.b / TCELL_ASIZE_Y;
            view.trect->y.t = view.grect->y.t / TCELL_ASIZE_Y;
            ecos.patch<com_trect_t>(view.entity);
            ::glMatrixMode(GL_PROJECTION);
            ::glLoadIdentity();
            ::gluOrtho2D(
                view.grect->x.l, view.grect->x.r,
                view.grect->y.b, view.grect->y.t
            );
        });
    } /* glut setup */
    if constexpr (_TRUTH)
    { /* opengl setup */
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
    } /* opengl setup */
    if constexpr (_TRUTH)
    { /* imori load */
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
    } /* imori load */
    if constexpr (_TRUTH)
    { /* imreg */
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
        entt::sigh_helper{ ecos }
        .with<com_faces_t>().on_update<[](ecos_t&ecos, ent_t ent){
        }>().on_construct<[](ecos_t&ecos, ent_t ent){
            auto&faces = ecos.get<com_faces_t>(ent);
        }>().on_destroy<[](ecos_t&ecos, ent_t ent){
            auto&faces = ecos.get<com_faces_t>(ent);
        }>();
    } /* imreg */
    if constexpr (_TRUTH)
    { /* view */
        view.entity= ecos.create();
        /* sizes */
        view.asize =&ecos.emplace<com_asize_t>(view.entity, asize_t{
            .x = VIEW_ASIZE_X,
            .y = VIEW_ASIZE_Y,
        });
        view.gsize = ecos.try_get<com_gsize_t>(view.entity);
        view.tsize = ecos.try_get<com_tsize_t>(view.entity);
        view.ratio =&ecos.emplace<com_ratio_t>(view.entity);
        /* coord */
        view.apos2 =&ecos.emplace<com_apos2_t>(view.entity);
        view.zpos1 =&ecos.emplace<com_zpos1_t>(view.entity);
        view.gpos3 = ecos.try_get<com_gpos3_t>(view.entity);
        view.tpos3 = ecos.try_get<com_tpos3_t>(view.entity);
        /* geometry */
        view.direc =&ecos.emplace<com_direc_t>(view.entity, direc_t{
            .x = -0,
            .y = +1,
        });
        view.grect = ecos.try_get<com_grect_t>(view.entity);
        view.trect = ecos.try_get<com_trect_t>(view.entity);
        /* family */
        view.family=&ecos.emplace<com_family_t>(view.entity);
        if constexpr (_TRUTH)
        { /* gui-mm */
            auto entity_q= ecos.create();
            /* sizes */
            ecos.emplace<com_rsize_t>(entity_q, rsize_t{
                .x = RSIZE_MAX - GUIS_RSIZE_X*2,
                .y = RSIZE_MAX - GUIS_RSIZE_Y*2,
            });
            /* visual */
            ecos.emplace<com_visual_t>(entity_q, _TRUTH);
            ecos.emplace<com_rlayer_t>(entity_q, 1);
            /* family */
            ecos.emplace<com_family_t>(entity_q, family_t{
                .ancestor = view.entity,
            });
            if constexpr(_TRUTH)
            { /* died */
                auto entity_d = ecos.create();
                /* sizes */
                ecos.emplace<com_rsize_t>(entity_d, rsize_t{
                    .x = RSIZE_MAX - GUIS_RSIZE_X*2,
                    .y = GUIS_RSIZE_Y,
                });
                /* visual */
                auto&visual= ecos.emplace<com_visual_t>(entity_d, _FALSE);
                ecos.emplace<com_rlayer_t>(entity_d, 1);
                ecos.emplace<com_color_t>(entity_d, 0xe0);
                ecos.emplace<com_imreg_t>(entity_d, _IMORI_FONT_MAIN);
                ecos.emplace<com_fonts_t>(entity_d, fonts_t{
                    .first = ' ',
                    .glyph = {
                        .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y },
                    },
                });
                auto&strbuf = ecos.emplace<com_strbuf_t>(entity_d, strbuf_t{
                    .msize = 8,
                    .mdata = "YouDied",
                });
                /* family */
                ecos.emplace<com_family_t>(entity_d, family_t{
                    .ancestor = entity_q,
                });
                /* signal */
                struct listener_t {
                    inline void update(const alive_t&alive)
                    {
                        this->visual->valid = !alive.valid;
                        this->visual =&ecos.patch<com_visual_t>(this->entity);
                    }
                    entity_t entity;
                    visual_t*visual;
                } static listener{entity_d,&visual};
                hero_rise_sigbinder.connect<&listener_t::update>(listener);
                hero_died_sigbinder.connect<&listener_t::update>(listener);
            } /* died */
        } /* gui-mm */
        if constexpr (_TRUTH)
        { /* gui-mb */
            auto entity_q= ecos.create();
            /* sizes */
            ecos.emplace<com_rsize_t>(entity_q, rsize_t{
                .x = RSIZE_MAX,
                .y = GUIS_RSIZE_Y,
            });
            /* coord */
            ecos.emplace<com_rpos2_t>(entity_q, rpos2_t{
                .x = RPOS2_MID,
                .y = RPOS2_MIN,
            });
            ecos.emplace<com_pivot_t>(entity_q, pivot_t{
                .x = PIVOT_MID,
                .y = PIVOT_MIN,
            });
            /* visual */
            ecos.emplace<com_visual_t>(entity_q, _TRUTH);
            ecos.emplace<com_rlayer_t>(entity_q, 1);
            ecos.emplace<com_color_t>(entity_q, 0x40);
            ecos.emplace<com_imreg_t>(entity_q, _IMORI_META_NONE);
            /* family */
            ecos.emplace<com_family_t>(entity_q, family_t{
                .ancestor = view.entity
            });
            if constexpr (_TRUTH)
            { /* label-mb */
                auto entity_l = ecos.create();
                /* sizes */
                ecos.emplace<com_rsize_t>(entity_l, rsize_t{
                    .x = RSIZE_MAX,
                    .y = RSIZE_MAX,
                });
                /* visual */
                ecos.emplace<com_visual_t>(entity_l, _TRUTH);
                ecos.emplace<com_rlayer_t>(entity_l, 1);
                ecos.emplace<com_color_t>(entity_l, 0xe0);
                ecos.emplace<com_imreg_t>(entity_l, _IMORI_FONT_MAIN);
                ecos.emplace<com_fonts_t>(entity_l, fonts_t{
                    .first = ' ',
                    .glyph = {
                        .asize = asize_t{
                            .x = IMORI_FONT_ASIZE.x,
                            .y = IMORI_FONT_ASIZE.y,
                        },
                    },
                });
                auto&strbuf = ecos.emplace<com_strbuf_t>(entity_l, strbuf_t{
                    .msize = 128,
                    .mdata = "",
                });
                if constexpr(_TRUTH)
                { /* signal */
                    struct listener_t { void on_update(update_event_t& event) {
                        memset(strbuf.mdata, '\0', strbuf.msize);
                        std::snprintf(
                            strbuf.mdata, strbuf.msize,
                            "mode=%s;acts=%08d;"
                            "%c"
                            "%s(%+d)x(%d)",
                            get_key_mode_name(), hero.actor->count,
                            '\n',
                            &get_key_line()[0], get_key_narg_sign(), get_key_narg()
                        );
                    } strbuf_t&strbuf; } static listener{ strbuf };
                    timer.dispatcher
                        .sink<update_event_t>()
                        .connect<&listener_t::on_update>(listener);
                } /* signal */
                /* family */
                ecos.emplace<com_family_t>(entity_l, family_t{
                    .ancestor = entity_q,
                });
            } /* label-mb */
        } /* gui-mb */
        if constexpr (_TRUTH)
        { /* gui-mt */
            auto entity_q= ecos.create();
            /* sizes */
            ecos.emplace<com_rsize_t>(entity_q, rsize_t{
                .x = RSIZE_MAX,
                .y = GUIS_RSIZE_Y,
            });
            /* coord */
            ecos.emplace<com_rpos2_t>(entity_q, rpos2_t{
                .x = RPOS2_MID,
                .y = RPOS2_MAX,
            });
            ecos.emplace<com_pivot_t>(entity_q, pivot_t{
                .x = PIVOT_MID,
                .y = PIVOT_MAX,
            });
            /* visual */
            ecos.emplace<com_visual_t>(entity_q, _TRUTH);
            ecos.emplace<com_rlayer_t>(entity_q, 1);
            ecos.emplace<com_color_t>(entity_q, color_t{
                .v = 0x40,
            });
            ecos.emplace<com_imreg_t>(entity_q, _IMORI_META_NONE);
            /* family */
            ecos.emplace<com_family_t>(entity_q, family_t{
                .ancestor = view.entity,
            });
            if constexpr (_TRUTH)
            { /* label-mt */
                auto entity_l = ecos.create();
                /* sizes */
                ecos.emplace<com_rsize_t>(entity_l, rsize_t{
                    .x = RSIZE_MAX,
                    .y = RSIZE_MAX,
                });
                /* visual */
                ecos.emplace<com_visual_t>(entity_l, _TRUTH);
                ecos.emplace<com_rlayer_t>(entity_l, 1);
                ecos.emplace<com_color_t>(entity_l, 0xe0);
                ecos.emplace<com_imreg_t>(entity_l, _IMORI_FONT_MAIN);
                ecos.emplace<com_fonts_t>(entity_l, fonts_t{
                    .first = ' ',
                    .glyph = {
                        .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y, },
                    },
                });
                auto&strbuf = ecos.emplace<com_strbuf_t>(entity_l, 128);
                /* signal */
                struct listener_t { void on_update(update_event_t& event) {
                    memset(strbuf.mdata, '\0', strbuf.msize);
                    std::snprintf(
                        strbuf.mdata, strbuf.msize,
                        "now=%03zu.%03zu;fps=%05zu;"
                        "%c"
                        "g(x%+04iy%+04iz%+04i)d(x%+1dy%+1d)"
                        "%c",
                        (timer.now_mil / 1000) % 1000, timer.now_mil % 1000, timer.fps_num,
                        '\n',
                        view.gpos3->x, view.gpos3->y, view.gpos3->z,
                        view.direc->x, view.direc->y,
                        '\0'
                    );
                } strbuf_t&strbuf; } static listener{ strbuf };
                /* family */
                ecos.emplace<com_family_t>(entity_l, family_t{
                    .ancestor = entity_q,
                });
            } /* label-mt */
        } /* gui-mt */
        if constexpr (_TRUTH)
        { /* gui-lm */
            ent_t entity_q = ecos.create();
            /* sizes */
            ecos.emplace<com_rsize_t>(entity_q, rsize_t{
                .x = GUIS_RSIZE_X,
                .y = RSIZE_MAX - GUIS_RSIZE_Y*2,
            });
            /* visual */
            ecos.emplace<com_visual_t>(entity_q, _TRUTH);
            ecos.emplace<com_rlayer_t>(entity_q, 1);
            ecos.emplace<com_color_t>(entity_q, color_t{
                .v = 0x40,
            });
            ecos.emplace<com_imreg_t>(entity_q, _IMORI_META_NONE);
            /* coord */
            ecos.emplace<com_rpos2_t>(entity_q, rpos2_t{
                .x = RPOS2_MIN,
                .y = RPOS2_MID,
            });
            ecos.emplace<com_pivot_t>(entity_q, pivot_t{
                .x = PIVOT_MIN,
                .y = PIVOT_MID,
            });
            /* family */
            ecos.emplace<com_family_t>(entity_q, family_t{
                .ancestor = view.entity,
            });
            if constexpr (_TRUTH)
            { /* label-lm */
                auto entity_l = ecos.create();
                /* sizes */
                ecos.emplace<com_rsize_t>(entity_l, rsize_t{
                    .x = RSIZE_MAX,
                    .y = RSIZE_MAX,
                });
                /* coord */
                ecos.emplace<com_rpos2_t>(entity_l, rpos2_t{
                    .x = RPOS2_MIN,
                    .y = RPOS2_MIN,
                });
                ecos.emplace<com_pivot_t>(entity_l, pivot_t{
                    .x = PIVOT_MIN,
                    .y = PIVOT_MIN,
                });
                /* visual */
                ecos.emplace<com_visual_t>(entity_l, _TRUTH);
                ecos.emplace<com_rlayer_t>(entity_l, 1);
                ecos.emplace<com_color_t>(entity_l, 0xe0);
                ecos.emplace<com_imreg_t>(entity_l, _IMORI_FONT_MAIN);
                ecos.emplace<com_fonts_t>(entity_l, fonts_t{
                    .first = ' ',
                    .glyph = {
                        .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y, },
                    },
                });
                auto&strbuf = ecos.emplace<com_strbuf_t>(entity_l, strbuf_t{
                    .msize = 128,
                    .mdata = "",
                });
                /* family */
                ecos.emplace<com_family_t>(entity_l, family_t{
                    .ancestor = entity_q,
                });
            } /* label-lm */
        } /* gui-lm */
        if constexpr (_TRUTH)
        { /* gui-rm */
            auto entity_q = ecos.create();
            /* coord */
            ecos.emplace<com_rpos2_t>(entity_q, rpos2_t{
                .x = RPOS2_MAX,
                .y = RPOS2_MID,
            });
            ecos.emplace<com_pivot_t>(entity_q, pivot_t{
                .x = PIVOT_MAX,
                .y = PIVOT_MID,
            });
            /* sizes */
            ecos.emplace<com_rsize_t>(entity_q, rsize_t{
                .x = GUIS_RSIZE_X,
                .y = RSIZE_MAX - GUIS_RSIZE_Y*2,
            });
            /* visual */
            ecos.emplace<com_visual_t>(entity_q, _TRUTH);
            ecos.emplace<com_rlayer_t>(entity_q, 1);
            ecos.emplace<com_color_t>(entity_q, color_t{
                .v = 0x40,
            });
            ecos.emplace<com_imreg_t>(entity_q, _IMORI_META_NONE);
            /* family */
            ecos.emplace<com_family_t>(entity_q, family_t{
                .ancestor = view.entity,
            });
            if constexpr (_TRUTH)
            { /* label-rm */
                auto entity_l = ecos.create();
                /* sizes */
                ecos.emplace<com_rsize_t>(entity_l, rsize_t{
                    .x = RSIZE_MAX,
                    .y = RSIZE_MAX,
                });
                /* coord */
                ecos.emplace<com_rpos2_t>(entity_l, rpos2_t{
                    .x = RPOS2_MIN,
                    .y = RPOS2_MIN,
                });
                ecos.emplace<com_pivot_t>(entity_l, pivot_t{
                    .x = PIVOT_MIN,
                    .y = PIVOT_MIN,
                });
                /* visual */
                ecos.emplace<com_visual_t>(entity_l, _TRUTH);
                ecos.emplace<com_rlayer_t>(entity_l, 1);
                ecos.emplace<com_color_t>(entity_l, 0xe0);
                ecos.emplace<com_imreg_t>(entity_l, imreg_t{
                    .index = _IMORI_FONT_MAIN,
                    .rsize = rsize_t{ RSIZE_MAX, RSIZE_MAX },
                });
                ecos.emplace<com_fonts_t>(entity_l, fonts_t{
                    .first = ' ',
                    .glyph = {
                        .asize = { IMORI_FONT_ASIZE.x, IMORI_FONT_ASIZE.y, },
                    },
                });
                auto&strbuf = ecos.emplace<com_strbuf_t>(entity_l, strbuf_t{
                    .msize = 128,
                    .mdata = "pick info",
                });
                /* family */
                ecos.emplace<com_family_t>(entity_l, entity_q);
                /* signal */
                struct listener_t {
                    inline void update()
                    {
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
                            pick.tpos3->x,
                            pick.tpos3->y,
                            pick.tpos3->z,
                            '\0'
                        );
                    }
                    strbuf_t&strbuf;
                } static listener{strbuf};
                pick_step_sigbinder.connect<&listener_t::update>(listener);
            } /* label-rm */
        } /* gui-rm */
    }
    if constexpr (_TRUTH)
    { /* helpgrid */
        helpgrid.entity= ecos.create();
        /* sizes */
        helpgrid.asize =&ecos.emplace<com_asize_t>(helpgrid.entity, asize_t{
            .x = TCELL_ASIZE_X * 3,
            .y = TCELL_ASIZE_Y * 3,
        });
        helpgrid.gsize = ecos.try_get<com_gsize_t>(helpgrid.entity);
        helpgrid.tsize = ecos.try_get<com_tsize_t>(helpgrid.entity);
        /* coord */
        helpgrid.gpos3 = ecos.try_get<com_gpos3_t>(helpgrid.entity);
        helpgrid.tpos3 = ecos.try_get<com_tpos3_t>(helpgrid.entity);
        /* visual */
        helpgrid.visual=&ecos.emplace<com_visual_t>(helpgrid.entity, _TRUTH);
        ecos.emplace<com_rlayer_t>(helpgrid.entity, 1);
        ecos.emplace<com_color_t>(helpgrid.entity, color_t{
            .v = 0x20,
        });
        helpgrid.lgrid =&ecos.emplace<com_lgrid_t>(helpgrid.entity, lgrid_t{
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
        helpgrid.family=&ecos.emplace<com_family_t>(helpgrid.entity, family_t{
            .ancestor = hero.entity,
        });
        if (ecos.valid(hero.entity))
        {
            if (!vet_ancestor(helpgrid.entity, hero.entity))
            {
                set_ancestor(helpgrid.entity, hero.entity);
            }
        }
    } /* helpgrid */
}

void gfix_loop()
{
    if constexpr (_TRUTH)
    { /* redraw */
        ::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        ::glClear(GL_COLOR_BUFFER_BIT);
    }
    if constexpr (_TRUTH)
    { /* drawing */
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
        for (auto&&
            [entity, visual, glayer, gpos3, grect]
            : ecos.view<
            com_visual_t, com_glayer_t, com_gpos3_t, com_grect_t
            >().each()
        ) {
            if (visual.valid && ((_FALSE
            ) || (_TRUTH
            && ((grect.x.l <= view.grect->x.r) && (grect.y.b <= view.grect->y.t))
            && ((grect.x.l >= view.grect->x.l) && (grect.y.b >= view.grect->y.b))
            ) || (_TRUTH
            && ((grect.x.l <= view.grect->x.r) && (grect.y.t <= view.grect->y.t))
            && ((grect.x.l >= view.grect->x.l) && (grect.y.t >= view.grect->y.b))
            ) || (_TRUTH
            && ((grect.x.r <= view.grect->x.r) && (grect.y.b <= view.grect->y.t))
            && ((grect.x.r >= view.grect->x.l) && (grect.y.b >= view.grect->y.b))
            ) || (_TRUTH
            && ((grect.x.r <= view.grect->x.r) && (grect.y.t <= view.grect->y.t))
            && ((grect.x.r >= view.grect->x.l) && (grect.y.t >= view.grect->y.b))
            ) || (_TRUTH
            && ((grect.x.l <= view.grect->x.l) && (grect.x.r >= view.grect->x.r))
            && ((grect.y.b >= view.grect->y.b) || (grect.y.t >= view.grect->y.t))
            ) || (_TRUTH
            && ((grect.y.b <= view.grect->y.b) && (grect.y.t >= view.grect->y.t))
            && ((grect.x.l >= view.grect->x.l) || (grect.x.r <= view.grect->x.r))
            )) && ((grect.x.l != grect.x.r) || (grect.y.b != grect.y.t))) {
                /*
                *drawlist.back = drawable_t{
                */
                new ( drawlist.back ) drawable_t{
                    .entity = entity,
                    .glayer = glayer,
                    .gpos3 = gpos3,
                    .grect = grect,
                };
                drawlist.back++;
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
            ) {
                auto imori = get_imori_from_index(imreg->index);
                auto frect = frect_t{
                    .x = {
                        .l = static_cast<v1f_t>(irect->x.l)
                            / static_cast<v1f_t>(imori->isize.x),
                        .r = static_cast<v1f_t>(irect->x.r)
                            / static_cast<v1f_t>(imori->isize.x),
                    },
                    .y = {
                        .b = static_cast<v1f_t>(irect->y.b)
                            / static_cast<v1f_t>(imori->isize.y),
                        .t = static_cast<v1f_t>(irect->y.t)
                            / static_cast<v1f_t>(imori->isize.y),
                    },
                };
                ::glBindTexture(GL_TEXTURE_2D, imori->glint);
                ::glBegin(GL_QUADS);
                if (auto*fonts = ecos.try_get<com_fonts_t>(entity))
                {
                }
                else
                {
                    ::glTexCoord2f(frect.x.l, frect.y.b);
                    ::glVertex2i(grect.x.l, grect.y.b);
                    ::glTexCoord2f(frect.x.r, frect.y.b);
                    ::glVertex2i(grect.x.r, grect.y.b);
                    ::glTexCoord2f(frect.x.r, frect.y.t);
                    ::glVertex2i(grect.x.r, grect.y.t);
                    ::glTexCoord2f(frect.x.l, frect.y.t);
                    ::glVertex2i(grect.x.l, grect.y.t);
                }
                ::glEnd();
                ::glBindTexture(GL_TEXTURE_2D, 0);
            } /* quad */
            ::glBegin(GL_QUADS);
            if (auto*lgrid = ecos.try_get<com_lgrid_t>(entity))
            { /* lgrid */
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
            } /* lgrid */
            ::glEnd();
        }
        memo::owner->del_mul(drawlist.head, count);
    } /* TODO: implement batch rendering, decrease draw calls */
    if constexpr (_TRUTH)
    { /* finish the frame */
        ::glutPostRedisplay();
    }
    if constexpr (_TRUTH)
    { /* to-hero adjustment */
        if (get_key_mode() != _KEY_MODE_VIEW)
        {
            ecos.replace<com_gpos3_t>(view.entity, *hero.gpos3);
        }
    }
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
    view.direc =&ecos.replace<com_direc_t>(
        view.entity,
        get_vec_turn(*view.direc, lside)
    );
    ecos.patch<com_direc_t>(view.entity);
}

/** getters **/

imori_t*get_imori_from_index(index_t index)
{ return&imori_list[index % _IMORI_COUNT]; }

_NAMESPACE_LEAVE
