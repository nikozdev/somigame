/* headers */

#include "gfix.hxx"
#include "util.hxx"
#include "iput.hxx"
#include "game.hxx"

#include <GL/glut.h>

#include "../lib/stbl/src/stbi.hxx"

/* content */

_NAMESPACE_ENTER

/** typedef **/

struct heritage_t
{
    com_coord_t coord = { 0, 0, 0 };
    com_sizes_t sizes = { 0, 0 };
    com_scale_t scale = { 0, 0 };
};
struct drawable_t
{
    entity_t entity;
    visual_t visual;
    coord_t  coord;
    heritage_t heritage;
};
using drawlist_t = t_array_t<drawable_t>;

/** datadef **/

ent_t ent_guis = entt::null;
ent_t ent_view = entt::null;
ent_t ent_grid = entt::null;
com_coord_t*coord_v;
com_direc_t*direc_v;
com_sizes_t*sizes_v;
com_scale_t*scale_v;
com_ratio_t*ratio_v;
com_recta_t*recta_v;

/*** system ***/

struct window_t {
    scale_t ratio = { .x = RATIO_X, .y = RATIO_Y };
    pos2d_t coord = { .x = 0, .y = 0 };
    sizes_t sizes = { .w = VIEW_SIZES_W, .h = VIEW_SIZES_H };
} window;

/*** assets ***/

image_origin_t image_list[_IMAGE_COUNT];

/*** meta ***/

constexpr const int IMAGE_NONE_MSTEP = 4;
constexpr const com_sizes_t IMAGE_NONE_SIZES = { .w = 1, .h = 1, };
ubyte_t IMAGE_NONE_MDATA[] = { 0xff,0xff,0xff,0xff };

constexpr const int IMAGE_TEST_MSTEP = 4;
constexpr const com_sizes_t IMAGE_TEST_SIZES = { .w = 4, .h = 4, };
ubyte_t IMAGE_TEST_MDATA[] = {
    0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa,
    0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55,
    0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa,
    0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55,
};

constexpr const int IMAGE_FONT_SIZES_X = 8;
constexpr const int IMAGE_FONT_SIZES_Y = 8;

/** actions **/

void gfix_init()
{
    if constexpr (_FALSE)
    {
        std::clog << "[opengl-version] " << glGetString(GL_VERSION);
        std::clog << std::endl << std::endl;
    }
    /* window */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(window.coord.x, window.coord.y);
    glutInitWindowSize(window.sizes.w, window.sizes.h);
    glutCreateWindow(_NAME_STR);
    /* callbacks */
    glutDisplayFunc(glutSwapBuffers);
    glutReshapeFunc([](int sizew, int sizeh) {
        int winratx = window.ratio.x;
        int winraty = window.ratio.y;
        int winrelw = sizew / winratx;
        int winrelh = sizeh / winraty;
        int winreld = winrelw - winrelh;
        if (winreld == 0)
        {
            window.coord.x = 0;
            window.coord.y = 0;
        }
        else if (winreld < 0)
        {
            window.coord.x = 0;
            window.coord.y = -winreld * winraty / 2;
            sizeh -= (window.coord.y * 2);
        }
        else if (winreld > 0)
        {
            window.coord.x = +winreld * winratx / 2;
            window.coord.y = 0;
            sizew -= (window.coord.x * 2);
        }
        window.sizes.w = sizew;
        window.sizes.h = sizeh;
    });
    /* settings */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_TEXTURE_1D);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_3D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);
    /* images */
    stbi_set_flip_vertically_on_load(_TRUTH);
    load_image_from_value_into_index(image_t{
        .glint = 0,
        .sizes = { .w = IMAGE_NONE_SIZES.w, .h = IMAGE_NONE_SIZES.h },
        .mstep = IMAGE_NONE_MSTEP,
        .msize = IMAGE_NONE_MSTEP * IMAGE_NONE_SIZES.w * IMAGE_NONE_SIZES.h,
        .mdata = &IMAGE_NONE_MDATA[0]
    }, _IMAGE_META_NONE);
    load_image_from_value_into_index(image_t{
        .glint = 0,
        .sizes = { .w = IMAGE_TEST_SIZES.w, .h = IMAGE_TEST_SIZES.h },
        .mstep = IMAGE_TEST_MSTEP,
        .msize = IMAGE_TEST_MSTEP * IMAGE_TEST_SIZES.w * IMAGE_TEST_SIZES.h,
        .mdata = &IMAGE_TEST_MDATA[0]
    }, _IMAGE_META_TEST);
    load_image_from_fpath_into_index("rsc/gfix/meta-logo.png", _IMAGE_META_LOGO);
    load_image_from_fpath_into_index("rsc/gfix/game-pick.png", _IMAGE_GAME_PICK);
    load_image_from_fpath_into_index("rsc/gfix/game-hero-ai-f1.png", _IMAGE_GAME_HERO);
    load_image_from_fpath_into_index("rsc/gfix/tile-test-st-tmm.png", _IMAGE_TILE_TEST);
    load_image_from_fpath_into_index("rsc/gfix/font-main-8x8.png", _IMAGE_FONT_MAIN);
    if constexpr (_TRUTH)
    { /* view */
        ent_view = ecos.create();
        auto ename_v = ecos.emplace<com_ename_t>(ent_view, _ENAME_GFIX_VIEW);
        scale_v =&ecos.emplace<com_scale_t>(ent_view, 1, 1);
        ratio_v =&ecos.emplace<com_ratio_t>(ent_view, RATIO_X, RATIO_Y);
        coord_v =&ecos.emplace<com_coord_t>(ent_view, 0, 0, 0);
        direc_v =&ecos.emplace<com_direc_t>(ent_view, -0, +1);
        sizes_v =&ecos.emplace<com_sizes_t>(ent_view, VIEW_SIZES_W, VIEW_SIZES_H);
        recta_v =&ecos.emplace<com_recta_t>(ent_view, 0, 0, 0, 0);
    }
    if (_TRUTH)
    { /* images */
        ecos.on_construct<com_image_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            auto&image = ecos.get<com_image_t>(ent);
            if (image.index >= _IMAGE_COUNT) { image.index = _IMAGE_META_TEST; }
            if (image.sizes.w <= 0) { image.sizes.w = image_list[image.index].sizes.w; }
            if (image.sizes.h <= 0) { image.sizes.h = image_list[image.index].sizes.h; }
            if (ecos.try_get<com_faces_t>(ent) != _NULL)
            { ecos.remove<com_faces_t>(ent); }
        }>();
        ecos.on_destroy<com_image_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            auto&image = ecos.get<com_image_t>(ent);
        }>();
        ecos.on_construct<com_faces_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            auto&faces = ecos.get<com_faces_t>(ent);
            if (ecos.try_get<com_image_t>(ent) != _NULL)
            { ecos.remove<com_image_t>(ent); }
            if (ecos.try_get<com_direc_t>(ent) == _NULL)
            { ecos.emplace<com_direc_t>(ent); }
        }>();
        ecos.on_destroy<com_faces_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            auto&faces = ecos.get<com_faces_t>(ent);
        }>();
        ecos.on_construct<com_fonts_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            auto&fonts = ecos.get<com_fonts_t>(ent);
            if (fonts.image.index <= 0 || fonts.image.index >= _IMAGE_COUNT)
            { fonts.image.index = _IMAGE_FONT_MAIN; }
            auto&image = image_list[fonts.image.index];
            if (fonts.image.sizes.w <= 0)
            { fonts.image.sizes.w = image.sizes.w; fonts.image.coord.x = 0; }
            if (fonts.image.sizes.h <= 0)
            { fonts.image.sizes.h = image.sizes.h; fonts.image.coord.y = 0; }
        }>();
        ecos.on_destroy<com_fonts_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            auto&fonts = ecos.get<com_fonts_t>(ent);
        }>();
    }
    if constexpr (_TRUTH)
    { /* guis entity */
        ent_guis = ecos.create();
        ecos.emplace<com_family_t>(ent_guis, ent_guis);
        ecos.emplace<com_visual_t>(ent_guis, _TRUTH, GUIS_LAYER);
        ecos.emplace<com_relpos_t>(ent_guis, RELPOS_MID, RELPOS_MID);
        ecos.emplace<com_anchor_t>(ent_guis, RELPOS_MID, RELPOS_MID);
        ecos.emplace<com_ename_t>(ent_guis, _ENAME_GUIS);
        ecos.emplace<com_coord_t>(ent_guis, 0, 0, GUIS_LAYER);
        ecos.emplace<com_sizes_t>(ent_guis, sizes_v->w, sizes_v->h);
        ecos.emplace<com_scale_t>(ent_guis, 1, 1);
        if constexpr (_TRUTH)
        { /* middle screen */
            ent_t ent_quad = ecos.create();
            ecos.emplace<com_family_t>(ent_quad, ent_guis);
            ecos.emplace<com_visual_t>(ent_quad, _TRUTH, 0);
            ecos.emplace<com_relpos_t>(ent_quad, RELPOS_MID, RELPOS_MID);
            ecos.emplace<com_anchor_t>(ent_quad, RELPOS_MID, RELPOS_MID);
            ecos.emplace<com_coord_t>(ent_quad, 0, 0, 0);
            ecos.emplace<com_sizes_t>(ent_quad, sizes_v->w - GUIS_SIZES_X*2, sizes_v->h - GUIS_SIZES_Y*2);
            ecos.emplace<com_scale_t>(ent_quad, 1, 1);
            ecos.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_MM_QUAD);
            /*
            ecos.emplace<com_color_t>(ent_quad, 0x80);
            ecos.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, pos2d_t{0,0}, sizes_t{0,0});
            */
            if constexpr(_TRUTH)
            { /* died */
                auto ent_died = ecos.create();
                ecos.emplace<com_family_t>(ent_died, ent_quad);
                auto&visual = ecos.emplace<com_visual_t>(ent_died, _FALSE, 0x10);
                ecos.emplace<com_relpos_t>(ent_died, RELPOS_MAX, RELPOS_MID);
                ecos.emplace<com_anchor_t>(ent_died, RELPOS_MAX, RELPOS_MID);
                ecos.emplace<com_coord_t>(ent_died, 0, 0, 0x10);
                ecos.emplace<com_sizes_t>(ent_died, sizes_v->w-GUIS_SIZES_X*2, GUIS_SIZES_Y);
                ecos.emplace<com_scale_t>(ent_died, 1, 1);
                ecos.emplace<com_color_t>(ent_died, 0x20);
                ecos.emplace<com_image_t>(ent_died, _IMAGE_META_NONE, pos2d_t{0,0}, sizes_t{0,0});
                ecos.emplace<com_ename_t>(ent_died, _ENAME_GUIS_MB_TEXT);
                ecos.emplace<com_fonts_t>(ent_died, fonts_t{
                    .color = { .v = 0xe0 },
                    .image = {
                        .index = _IMAGE_FONT_MAIN,
                        .coord = {0,0},
                        .sizes = {0,0},
                    },
                    .first = ' ',
                    .glyph = {
                        .sizes = {8,8},
                        .steps = {0,0},
                    },
                });
                auto&cstring = ecos.emplace<com_cstring_t>(ent_died, 8);
                std::snprintf(cstring.mdata, cstring.msize, "YouDied");
                hero_rise_signal.bind([&](alive_t&alive){ visual.valid = _FALSE; });
                hero_died_signal.bind([&](alive_t&alive){ visual.valid = _TRUTH; });
            } /* died */
        }
        if constexpr (_TRUTH)
        { /* gui-mb */
            ent_t ent_quad = ecos.create();
            ecos.emplace<com_family_t>(ent_quad, ent_guis);
            ecos.emplace<com_visual_t>(ent_quad, _TRUTH, 1);
            ecos.emplace<com_relpos_t>(ent_quad, RELPOS_MID, RELPOS_MIN);
            ecos.emplace<com_anchor_t>(ent_quad, RELPOS_MID, RELPOS_MIN);
            ecos.emplace<com_coord_t>(ent_quad, 0, 0, 0);
            ecos.emplace<com_sizes_t>(ent_quad, sizes_v->w, GUIS_SIZES_Y);
            ecos.emplace<com_scale_t>(ent_quad, 1, 1);
            ecos.emplace<com_color_t>(ent_quad, 0x40);
            ecos.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_MB_QUAD);
            ecos.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, pos2d_t{0,0}, sizes_t{0,0});
            if constexpr (_TRUTH)
            { /* text-mb */
                auto ent_text = ecos.create();
                ecos.emplace<com_family_t>(ent_text, ent_quad);
                ecos.emplace<com_visual_t>(ent_text, _TRUTH, 1);
                ecos.emplace<com_relpos_t>(ent_text, RELPOS_MID, RELPOS_MID);
                ecos.emplace<com_anchor_t>(ent_text, RELPOS_MID, RELPOS_MID);
                ecos.emplace<com_coord_t>(ent_text, 0, 0, 1);
                ecos.emplace<com_sizes_t>(ent_text, sizes_v->w, GUIS_SIZES_Y);
                ecos.emplace<com_scale_t>(ent_text, 1, 1);
                ecos.emplace<com_color_t>(ent_text, 0xe0);
                ecos.emplace<com_ename_t>(ent_text, _ENAME_GUIS_MB_TEXT);
                ecos.emplace<com_fonts_t>(ent_text, fonts_t{
                    .color = { .v = 0xe0 },
                    .image = {
                        .index = _IMAGE_FONT_MAIN,
                        .coord = {0,0},
                        .sizes = {0,0},
                    },
                    .first = ' ',
                    .glyph = {
                        .sizes = {8,8},
                        .steps = {0,0},
                    },
                });
                ecos.emplace<com_cstring_t>(ent_text, 128);
            }
            if constexpr (_FALSE)
            { /* logo-mb */
                ent_t ent_logo = ecos.create();
                ecos.emplace<com_family_t>(ent_logo, ent_quad);
                ecos.emplace<com_visual_t>(ent_logo, _TRUTH, 0);
                ecos.emplace<com_relpos_t>(ent_logo, RELPOS_MAX, RELPOS_MID);
                ecos.emplace<com_anchor_t>(ent_logo, RELPOS_MIN, RELPOS_MID);
                ecos.emplace<com_coord_t>(ent_logo, 0, 0, 3);
                ecos.emplace<com_sizes_t>(ent_logo, UNIT_SCALE_X, UNIT_SCALE_Y);
                ecos.emplace<com_scale_t>(ent_logo, 1, 1);
                ecos.emplace<com_color_t>(ent_logo, 0xff);
                ecos.emplace<com_image_t>(ent_logo, _IMAGE_META_LOGO, pos2d_t{0,0}, sizes_t{0,0});
            }
        }
        if constexpr (_TRUTH)
        { /* gui-mt */
            ent_t ent_quad = ecos.create();
            ecos.emplace<com_family_t>(ent_quad, ent_guis);
            ecos.emplace<com_visual_t>(ent_quad, _TRUTH, 1);
            ecos.emplace<com_relpos_t>(ent_quad, RELPOS_MID, RELPOS_MAX);
            ecos.emplace<com_anchor_t>(ent_quad, RELPOS_MID, RELPOS_MAX);
            ecos.emplace<com_coord_t>(ent_quad, 0, 0, 0);
            ecos.emplace<com_color_t>(ent_quad, 0x40);
            ecos.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, pos2d_t{0,0}, sizes_t{0,0});
            ecos.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_MT_QUAD);
            ecos.emplace<com_sizes_t>(ent_quad, sizes_v->w, GUIS_SIZES_Y);
            ecos.emplace<com_scale_t>(ent_quad, 1, 1);
            if constexpr (_TRUTH)
            { /* text-mt */
                auto ent_text = ecos.create();
                ecos.emplace<com_family_t>(ent_text, ent_quad);
                ecos.emplace<com_visual_t>(ent_text, _TRUTH, 1);
                ecos.emplace<com_relpos_t>(ent_text, RELPOS_MID, RELPOS_MID);
                ecos.emplace<com_anchor_t>(ent_text, RELPOS_MID, RELPOS_MID);
                ecos.emplace<com_coord_t>(ent_text, 0, 0, 1);
                ecos.emplace<com_sizes_t>(ent_text, sizes_v->w, GUIS_SIZES_Y);
                ecos.emplace<com_scale_t>(ent_text, 1, 1);
                ecos.emplace<com_color_t>(ent_text, 0xe0);
                ecos.emplace<com_ename_t>(ent_text, _ENAME_GUIS_MT_TEXT);
                ecos.emplace<com_fonts_t>(ent_text, fonts_t{
                    .color = { .v = 0xe0 },
                    .image = {
                        .index = _IMAGE_FONT_MAIN,
                        .coord = {0,0},
                        .sizes = {0,0},
                    },
                    .first = ' ',
                    .glyph = {
                        .sizes = {8,8},
                        .steps = {0,0},
                    },
                });
                ecos.emplace<com_cstring_t>(ent_text, 128);
            }
        }
        if constexpr (_TRUTH)
        { /* gui-lm */
            ent_t ent_quad = ecos.create();
            ecos.emplace<com_family_t>(ent_quad, ent_guis);
            ecos.emplace<com_visual_t>(ent_quad, _TRUTH, 1);
            ecos.emplace<com_relpos_t>(ent_quad, RELPOS_MIN, RELPOS_MID);
            ecos.emplace<com_anchor_t>(ent_quad, RELPOS_MIN, RELPOS_MID);
            ecos.emplace<com_coord_t>(ent_quad, 0, 0, 0);
            auto&sizes_q = ecos.emplace<com_sizes_t>(ent_quad, GUIS_SIZES_X, sizes_v->h - GUIS_SIZES_Y*2);
            ecos.emplace<com_scale_t>(ent_quad, 1, 1);
            ecos.emplace<com_color_t>(ent_quad, 0x40);
            ecos.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_LM_QUAD);
            ecos.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, pos2d_t{0,0}, sizes_t{0,0});
            if constexpr (_TRUTH)
            { /* text-lm */
                auto ent_text = ecos.create();
                ecos.emplace<com_family_t>(ent_text, ent_quad);
                ecos.emplace<com_visual_t>(ent_text, _TRUTH, 1);
                ecos.emplace<com_relpos_t>(ent_text, RELPOS_MIN, RELPOS_MIN);
                ecos.emplace<com_anchor_t>(ent_text, RELPOS_MIN, RELPOS_MIN);
                ecos.emplace<com_coord_t>(ent_text, 0, 0, 0);
                ecos.emplace<com_sizes_t>(ent_text, sizes_q.w, sizes_q.h);
                ecos.emplace<com_scale_t>(ent_text, 1, 1);
                ecos.emplace<com_color_t>(ent_text, 0xe0);
                ecos.emplace<com_ename_t>(ent_text, _ENAME_GUIS_LM_TEXT);
                ecos.emplace<com_fonts_t>(ent_text, fonts_t{
                    .color = { .v = 0xe0 },
                    .image = {
                        .index = _IMAGE_FONT_MAIN,
                        .coord = {0,0},
                        .sizes = {0,0},
                    },
                    .first = ' ',
                    .glyph = {
                        .sizes = {8,8},
                        .steps = {0,0},
                    },
                });
                auto&cstring = ecos.emplace<com_cstring_t>(ent_text, 128);
            }
        }
        if constexpr (_TRUTH)
        { /* gui-rm */
            ent_t ent_quad = ecos.create();
            ecos.emplace<com_family_t>(ent_quad, ent_guis);
            ecos.emplace<com_visual_t>(ent_quad, _TRUTH, 1);
            ecos.emplace<com_relpos_t>(ent_quad, RELPOS_MAX, RELPOS_MID);
            ecos.emplace<com_anchor_t>(ent_quad, RELPOS_MAX, RELPOS_MID);
            ecos.emplace<com_coord_t>(ent_quad, 0, 0, 0);
            auto&sizes_q = ecos.emplace<com_sizes_t>(ent_quad, GUIS_SIZES_X, sizes_v->h - GUIS_SIZES_Y*2);
            ecos.emplace<com_scale_t>(ent_quad, 1, 1);
            ecos.emplace<com_color_t>(ent_quad, 0x40);
            ecos.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_RM_QUAD);
            ecos.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, pos2d_t{0,0}, sizes_t{0,0});
            if constexpr (_TRUTH)
            { /* text-rm */
                auto ent_text = ecos.create();
                ecos.emplace<com_family_t>(ent_text, ent_quad);
                ecos.emplace<com_visual_t>(ent_text, _TRUTH, 1);
                ecos.emplace<com_relpos_t>(ent_text, RELPOS_MIN, RELPOS_MIN);
                ecos.emplace<com_anchor_t>(ent_text, RELPOS_MIN, RELPOS_MIN);
                ecos.emplace<com_coord_t>(ent_text, 0, 0, 0);
                ecos.emplace<com_sizes_t>(ent_text, sizes_q.w, sizes_q.h);
                ecos.emplace<com_scale_t>(ent_text, 1, 1);
                ecos.emplace<com_color_t>(ent_text, 0xe0);
                ecos.emplace<com_ename_t>(ent_text, _ENAME_GUIS_RM_TEXT);
                ecos.emplace<com_fonts_t>(ent_text, fonts_t{
                    .color = { .v = 0xe0 },
                    .image = {
                        .index = _IMAGE_FONT_MAIN,
                        .coord = {0,0},
                        .sizes = {0,0},
                    },
                    .first = ' ',
                    .glyph = {
                        .sizes = {8,8},
                        .steps = {0,0},
                    },
                });
                auto&cstring = ecos.emplace<com_cstring_t>(ent_text, 128);
                auto update = [&](coord_t coord_p){
                    coord_p = from_coord_into_tile_coord(coord_p);
                    std::snprintf(
                        cstring.mdata, cstring.msize,
                        "pick\n"
                        "========%c"
                        "coord\n"
                        "x%+04i\n"
                        "y%+04i\n"
                        "z%+04i\n"
                        "========%c",
                        '\n',
                        coord_p.x, coord_p.y, coord_p.z,
                        '\0'
                    );
                };
                update({0,0,0});
                pick_step_signal.bind(update);
            }
        }
    }
    if constexpr (_TRUTH)
    { /* grid */
        ent_grid = ecos.create();
        ecos.emplace<com_family_t>(ent_grid);
        ecos.emplace<com_visual_t>(ent_grid, _TRUTH, 0);
        ecos.emplace<com_anchor_t>(ent_grid, RELPOS_MID, RELPOS_MID);
        ecos.emplace<com_coord_t>(ent_grid, 0, 0, 0);
        ecos.emplace<com_sizes_t>(ent_grid, TILE_SCALE_X * 3, TILE_SCALE_Y * 3);
        ecos.emplace<com_scale_t>(ent_grid, 1, 1);
        ecos.emplace<com_color_t>(ent_grid, 0x80);
        ecos.emplace<com_ename_t>(ent_grid, _ENAME_GFIX_GRID);
        ecos.emplace<com_grid_t>(ent_grid,
            grid_t{
                .cells = { TILE_SCALE_X, TILE_SCALE_Y },
                .tails = { TILE_SCALE_X, TILE_SCALE_Y },
                .width = 1,
            }
        );
        if constexpr (_TRUTH)
        { /* hero */
            auto ent_hero = get_by_ename({_ENAME_GAME_HERO});
            if (ecos.valid(ent_hero))
            { set_ancestor(ent_grid, ent_hero); }
        } /* hero */
    } /* grid */
}

/*** drawing ***/

static void draw_init()
{
}

extern void proc_list(drawlist_t&drawlist);
void draw_unit(drawable_t&drawable)
{
    bool can_draw = _TRUTH;
    auto&entity = drawable.entity;
    auto&heritage = drawable.heritage;
    /* props */
    auto coord_e = drawable.coord;
    auto&coord_a = heritage.coord;
    coord_e.z += coord_a.z;
    auto color_e = color_t{ .v = 0xff };
    if (auto*temp = ecos.try_get<com_color_t>(entity)) { color_e =*temp; }
    if (ecos.try_get<com_tile_t>(entity))
    {
        auto dif_z = coord_e.z - coord_v->z;
        auto dif_y = coord_e.y - coord_v->y;
        dif_y += TILE_SCALE_Y * dif_z;
        dif_y = std::abs(dif_y);
        auto dif_x = coord_e.x - coord_v->x;
        dif_x = std::abs(dif_x);
        auto rng_x = TILE_SCALE_X * 2;
        auto rng_y = TILE_SCALE_Y * 2;
        auto coord_f = from_coord_into_tile_coord(coord_e);
        coord_f.y -= 1;
        coord_f.z += 1;
        if (vet_tile_from_tile_coord(coord_f))
        { can_draw = _FALSE; }
        if (dif_z == 0 && ((_FALSE
        ) || (_TRUTH
        && dif_x <= (rng_x)
        && dif_y <= (rng_y - TILE_SCALE_Y)
        ) || (_TRUTH
        && dif_x <= (rng_x - TILE_SCALE_X)
        && dif_y <= (rng_y)
        ))) { can_draw = _TRUTH; }
        else if (dif_z > 0)
        {
            if ((_FALSE
            ) || (_TRUTH
            && dif_x <= (rng_x)
            && dif_y <= (rng_y - TILE_SCALE_Y*1)
            ) || (_TRUTH
            && dif_x <= (rng_x - TILE_SCALE_X*1)
            && dif_y <= (rng_y)
            )) { can_draw = _FALSE; }
        }
        coord_e.y += TILE_SCALE_Y * dif_z;
        color_e.v /= std::abs(get_num_sign(dif_z)+dif_z);
    }
    auto&sizes_a = heritage.sizes;
    auto sizes_e = sizes_t{ .w = UNIT_SCALE_X, .h = UNIT_SCALE_Y };
    if (auto*temp = ecos.try_get<com_sizes_t>(entity)) { sizes_e =*temp; }
    auto&scale_a = heritage.scale;
    auto scale_e = scale_t{ .x = 1, .y = 1 };
    if (auto*temp = ecos.try_get<com_scale_t>(entity)) { scale_e =*temp; }
    scale_e.x *= scale_a.x;
    scale_e.y *= scale_a.y;
    auto anchor_e = anchor_t{ RELPOS_MID, RELPOS_MID };
    if (auto*temp = ecos.try_get<com_anchor_t>(entity)) { anchor_e =*temp; }
    auto relpos_e = relpos_t{ RELPOS_MID, RELPOS_MID };
    if (auto*temp = ecos.try_get<com_relpos_t>(entity)) { relpos_e =*temp; }
    /* direc */
    auto coord_u = coord_e;
    if constexpr (_FALSE)
    {
        while (direc_v->y != +1)
        {
            *direc_v = get_vec_turn(*direc_v);
            coord_u = get_vec_turn(coord_u);
        }
    }
    /* coord */
    auto coord_sl = coord_u.x;
    auto coord_sr = coord_u.x;
    auto coord_sb = coord_u.y;
    auto coord_st = coord_u.y;
    /** relpos **/
    auto moved_sx = (sizes_a.w * relpos_e.x) / RELPOS_DIV;
    auto moved_sy = (sizes_a.h * relpos_e.y) / RELPOS_DIV;
    coord_sl = (coord_sl + moved_sx) * scale_a.x + coord_a.x;
    coord_sr = (coord_sr + moved_sx) * scale_a.x + coord_a.x;
    coord_sb = (coord_sb + moved_sy) * scale_a.y + coord_a.y;
    coord_st = (coord_st + moved_sy) * scale_a.y + coord_a.y;
    coord_e.x = (coord_e.x + moved_sx) * scale_a.x + coord_a.x;
    coord_e.y = (coord_e.y + moved_sy) * scale_a.y + coord_a.y;
    /** anchor **/
    coord_sl += (sizes_e.w * scale_e.x * (RELPOS_MIN - anchor_e.x)) / RELPOS_DIV;
    coord_sr += (sizes_e.w * scale_e.x * (RELPOS_MAX - anchor_e.x)) / RELPOS_DIV;
    coord_sb += (sizes_e.h * scale_e.y * (RELPOS_MIN - anchor_e.y)) / RELPOS_DIV;
    coord_st += (sizes_e.h * scale_e.y * (RELPOS_MAX - anchor_e.y)) / RELPOS_DIV;
    coord_e.x -= (sizes_e.w * scale_e.x * anchor_e.x) / RELPOS_DIV;
    coord_e.y -= (sizes_e.h * scale_e.y * anchor_e.y) / RELPOS_DIV;
    if (can_draw && ((_FALSE
    ) || (_TRUTH
    && ((coord_sl <= recta_v->sr) && (coord_sb <= recta_v->st))
    && ((coord_sl >= recta_v->sl) && (coord_sb >= recta_v->sb))
    ) || (_TRUTH
    && ((coord_sl <= recta_v->sr) && (coord_st <= recta_v->st))
    && ((coord_sl >= recta_v->sl) && (coord_st >= recta_v->sb))
    ) || (_TRUTH
    && ((coord_sr <= recta_v->sr) && (coord_sb <= recta_v->st))
    && ((coord_sr >= recta_v->sl) && (coord_sb >= recta_v->sb))
    ) || (_TRUTH
    && ((coord_sr <= recta_v->sr) && (coord_st <= recta_v->st))
    && ((coord_sr >= recta_v->sl) && (coord_st >= recta_v->sb))
    ) || (_TRUTH
    && ((coord_sl <= recta_v->sl) && (coord_sr >= recta_v->sr))
    && ((coord_sb >= recta_v->sb) || (coord_st >= recta_v->st))
    ) || (_TRUTH
    && ((coord_sb <= recta_v->sb) && (coord_st >= recta_v->st))
    && ((coord_sl >= recta_v->sl) || (coord_sr <= recta_v->sr))
    ))) { /* draw */
        glColor3ub(color_e.v, color_e.v, color_e.v);
        auto*image = ecos.try_get<com_image_t>(entity);
        if (auto*faces = ecos.try_get<com_faces_t>(entity))
        {
            auto direc_e = direc_t{+0,-1};
            if (auto*temp = ecos.try_get<com_direc_t>(entity)) { direc_e =*temp; }
            auto eface = _EFACE_F;
            auto dotec = direc_v->x * direc_e.x + direc_v->y * direc_e.y;
            if (dotec == 0)
            {
                if (direc_v->x == 0)
                {
                    if (direc_v->y == direc_e.x) { eface = _EFACE_R; }
                    else { eface = _EFACE_L; }
                }
                else
                {
                    if (direc_v->x == direc_e.y) { eface = _EFACE_L; }
                    else { eface = _EFACE_R; }
                }
            }
            else if (dotec == -1) { eface = _EFACE_F; }
            else if (dotec == +1) { eface = _EFACE_B; }
            image =&faces->ilist[eface]; 
        }
        if (image)
        {
            auto image_region = *image;
            auto image_index = image_region.index;
            auto image_glint = 0u;
            auto image_sx = image_region.coord.x;
            auto image_sy = image_region.coord.y;
            auto image_uw = image_region.sizes.w;
            auto image_uh = image_region.sizes.h;
            auto image_fl = static_cast<v1f_t>(image_sx);
            auto image_fr = static_cast<v1f_t>(image_uw) + image_fl;
            auto image_fb = static_cast<v1f_t>(image_sy);
            auto image_ft = static_cast<v1f_t>(image_uh) + image_fb;
            if (image_index < _IMAGE_COUNT)
            {
                auto&image_origin = image_list[image_index];
                image_glint = image_origin.glint;
                auto image_fw = static_cast<v1f_t>(image_origin.sizes.w);
                auto image_fh = static_cast<v1f_t>(image_origin.sizes.h);
                image_fl /= image_fw; image_fr /= image_fw;
                image_fb /= image_fh; image_ft /= image_fh;
            }
            glBindTexture(GL_TEXTURE_2D, image_glint);
            glBegin(GL_QUADS);
            glTexCoord2f(image_fl, image_fb);
            glVertex2i(coord_sl, coord_sb);
            glTexCoord2f(image_fr, image_fb);
            glVertex2i(coord_sr, coord_sb);
            glTexCoord2f(image_fr, image_ft);
            glVertex2i(coord_sr, coord_st);
            glTexCoord2f(image_fl, image_ft);
            glVertex2i(coord_sl, coord_st);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        if (auto*grid = ecos.try_get<grid_t>(entity))
        {
            auto cells = grid->cells;
            auto tails = grid->tails;
            auto width = grid->width;
            glBegin(GL_QUADS);
            auto fromy = coord_sb - tails.h;
            auto intoy = coord_st + tails.h;
            for (auto stepx = coord_sl; stepx <= coord_sr; stepx += cells.w)
            {
                glVertex2i(stepx - width, fromy); glVertex2i(stepx + width, fromy);
                glVertex2i(stepx + width, intoy); glVertex2i(stepx - width, intoy);
            }
            auto fromx = coord_sl - tails.w;
            auto intox = coord_sr + tails.w;
            for (auto stepy = coord_sb; stepy <= coord_st; stepy += cells.h)
            {
                glVertex2i(fromx, stepy - width); glVertex2i(fromx, stepy + width);
                glVertex2i(intox, stepy + width); glVertex2i(intox, stepy - width);
            }
            glEnd();
        }
        if (auto*cstring = ecos.try_get<com_cstring_t>(entity))
        {
            auto fonts = fonts_t{};
            if (auto*temp = ecos.try_get<fonts_t>(entity)) { fonts =*temp; }
            glColor3ub(fonts.color.v, fonts.color.v, fonts.color.v);
            const auto&image = image_list[fonts.image.index];
            auto image_uw = static_cast<v1u_t>(fonts.image.sizes.w);
            auto image_uh = static_cast<v1u_t>(fonts.image.sizes.h);
            auto image_fw = static_cast<v1f_t>(fonts.image.sizes.w);
            auto image_fh = static_cast<v1f_t>(fonts.image.sizes.h);
            /* glyph is a single chracter from the image */
            auto glyph_count_uw = static_cast<v1u_t>(image_uw / IMAGE_FONT_SIZES_X);
            auto glyph_count_uh = static_cast<v1u_t>(image_uh / IMAGE_FONT_SIZES_Y);
            auto glyph_uw = static_cast<v1u_t>(fonts.glyph.sizes.w + fonts.glyph.steps.x);
            auto glyph_uh = static_cast<v1u_t>(fonts.glyph.sizes.h + fonts.glyph.steps.y);
            auto glyph_fw = static_cast<v1f_t>(fonts.glyph.sizes.w + fonts.glyph.steps.x) / image_fw;
            auto glyph_fh = static_cast<v1f_t>(fonts.glyph.sizes.h + fonts.glyph.steps.y) / image_fh;
            auto glyph_sl = 0.0f;
            auto glyph_sr = 0.0f;
            auto glyph_sb = 0.0f;
            auto glyph_st = 0.0f;
            auto mdata = cstring->mdata;
            auto msize = cstring->msize;
            auto count_uw = 1, count_uw_iter = 0;
            auto count_uh = 1;
#if _FALSE
            for (auto iter = 0; iter < msize; iter++)
#else
            for (auto iter = 0; iter < msize && mdata[iter] > 0; iter++)
#endif
            {
                int mbyte = mdata[iter];
                if (mbyte == '\n')
                { count_uh += 1; count_uw_iter = 0; }
                else
                { count_uw = std::max(count_uw, ++count_uw_iter); }
            }
            /* label */
            auto label_sl = coord_sl;
            auto label_sr = coord_sr;
            auto label_sb = coord_sb;
            auto label_st = coord_st;
            auto label_uw = (label_sr - label_sl);
            auto label_uh = (label_st - label_sb);
            /* sizes */
            auto sizes_uw = (label_uw / count_uw);
            auto sizes_uh = (label_uh / count_uh);
            sizes_uw = std::min(sizes_uh, sizes_uw);
            sizes_uh = std::min(sizes_uw, sizes_uh);
            /* coord */
#if _TRUTH
            auto coord = pos2d_t{ label_sl, label_st, };
#else
            auto relpos = relpos_t{ RELPOS_MAX - anchor_e.x, RELPOS_MIN - anchor_e.y };
            auto coord = pos2d_t{
                label_sl + (label_uw * relpos.x / RELPOS_DIV),
                label_st + (label_uh * relpos.y / RELPOS_DIV),
            };
#endif
#if _TRUTH
#else
            auto anchor = anchor_t{ RELPOS_MAX - anchor_e.x, RELPOS_MIN - anchor_e.y };
            auto coord_sl = label_sl + (sizes_uw * (RELPOS_MIN - anchor.x)) / RELPOS_DIV;
            auto coord_sr = label_sl + (sizes_uw * (RELPOS_MAX - anchor.x)) / RELPOS_DIV;
            auto coord_sb = label_st + (sizes_uh * (RELPOS_MIN - anchor.y)) / RELPOS_DIV;
            auto coord_st = label_st + (sizes_uh * (RELPOS_MAX - anchor.y)) / RELPOS_DIV;
#endif
            //*/
            auto coord_sl = coord.x;
            auto coord_sr = coord.x + sizes_uw;
            auto coord_sb = coord.y - sizes_uh;
            auto coord_st = coord.y;
            //*/
            /* offset */
            moved_sx = 0;
            moved_sy = 0;
            glBindTexture(GL_TEXTURE_2D, image.glint);
            glBegin(GL_QUADS);
#if _FALSE
            for (auto iter = 0; iter < msize; iter++)
#else
            for (auto iter = 0; iter < msize && mdata[iter] > 0; iter++)
#endif
            {
                int mbyte = mdata[iter];
                if (mbyte == '\n')
                {
                    coord_sl -= moved_sx;
                    coord_sr -= moved_sx;
                    moved_sx = 0;
                    coord_sb -= sizes_uh;
                    coord_st -= sizes_uh;
                    moved_sy -= sizes_uh;
                    continue;
                }
                mbyte = std::max(mdata[iter], fonts.first) - fonts.first;
                auto stepx = (mbyte % glyph_count_uw);
                glyph_sl = static_cast<v1f_t>(stepx * glyph_uw);
                glyph_sl = glyph_sl / image_fw;
                glyph_sr = glyph_sl + glyph_fw;
                auto image_stepy = glyph_count_uh - (mbyte / glyph_count_uw);
                glyph_st = static_cast<v1f_t>(image_stepy * glyph_uw);
                glyph_st = glyph_st / image_fh;
                glyph_sb = glyph_st - glyph_fh;
                glTexCoord2f(glyph_sl, glyph_sb);
                glVertex2i(coord_sl, coord_sb);
                glTexCoord2f(glyph_sr, glyph_sb);
                glVertex2i(coord_sr, coord_sb);
                glTexCoord2f(glyph_sr, glyph_st);
                glVertex2i(coord_sr, coord_st);
                glTexCoord2f(glyph_sl, glyph_st);
                glVertex2i(coord_sl, coord_st);
                coord_sl += sizes_uw;
                coord_sr += sizes_uw;
                moved_sx += sizes_uw;
            }
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
            glBegin(GL_LINE_STRIP);
            glVertex2i(label_sl, label_sb);
            glVertex2i(label_sr, label_sb);
            glVertex2i(label_sr, label_st);
            glVertex2i(label_sl, label_st);
            glEnd();
        }
    }
    /* hierarchy */
    if (auto*family = ecos.try_get<com_family_t>(drawable.entity))
    {
        drawlist_t drawlist;
        call_for_children(entity, [&](ent_t children){
            if (auto*visual = ecos.try_get<com_visual_t>(children))
            {
                if (visual->valid)
                {
                    if (auto*coord = ecos.try_get<com_coord_t>(children))
                    {
                        drawlist.push_back(drawable_t{
                            .entity = children,
                            .visual = *visual,
                            .coord = *coord,
                            .heritage = {
                                .coord = coord_e,
                                .sizes = sizes_e,
                                .scale = scale_e,
                            }
                        });
                    }
                }
            }
        });
        proc_list(drawlist);
    }
}
void proc_list(drawlist_t&drawlist)
{
    auto head = drawlist.begin();
    auto tail = drawlist.end();
    std::sort(head, tail, [](const drawable_t& dl, const drawable_t& dr) {
        if (dl.coord.z == dr.coord.z) { return dl.visual.layer < dr.visual.layer; }
        else { return dl.coord.z < dr.coord.z; }
    });
    for (auto iter = head; iter != tail; iter++)
    {
        if (iter->visual.valid) { draw_unit(*iter); }
    }
}

void gfix_loop()
{
    /* window */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(window.coord.x, window.coord.y, window.sizes.w, window.sizes.h);
    /* camera */
    sizes_v->w = VIEW_SIZES_W;
    sizes_v->h = VIEW_SIZES_H;
    recta_v->sl = coord_v->x - sizes_v->w * scale_v->x / 2;
    recta_v->sr = coord_v->x + sizes_v->w * scale_v->x / 2;
    recta_v->sb = coord_v->y - sizes_v->h * scale_v->y / 2;
    recta_v->st = coord_v->y + sizes_v->h * scale_v->y / 2;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(recta_v->sl, recta_v->sr, recta_v->sb, recta_v->st);
    /* entity component system */
    if constexpr(_TRUTH)
    { /* middle bot text */
        auto&cstring = ecos.get<com_cstring_t>(get_by_ename(ename_t{_ENAME_GUIS_MB_TEXT}));
        memset(cstring.mdata, '\0', cstring.msize);
        std::snprintf(
            cstring.mdata, cstring.msize,
            "mode=%s;acts=%08d;"
            "%c"
            "%s(%+d)x(%d)",
            get_key_mode_name(), get_action_count(),
            '\n',
            &get_key_line()[0], get_key_narg_sign(), get_key_narg()
        );
    }
    if constexpr(_TRUTH)
    { /* middle top text */
        auto&cstring = ecos.get<com_cstring_t>(get_by_ename(ename_t{_ENAME_GUIS_MT_TEXT}));
        memset(cstring.mdata, '\0', cstring.msize);
        std::snprintf(
            cstring.mdata, cstring.msize,
            "now=%03zu.%03zu;fps=%05zu;"
            "%c"
            "p(x%+04iy%+04iz%+04i)s(x%+04iy%+04i)d(x%+1dy%+1d)"
            "%c",
            (timer.now_mil / 1000) % 1000, timer.now_mil % 1000, timer.fps_num,
            '\n',
            coord_v->x, coord_v->y, coord_v->z,
            scale_v->x, scale_v->y,
            direc_v->x, direc_v->y,
            '\0'
        );
    }
    if constexpr (_TRUTH)
    { /* draw families from top to bottom */
        drawlist_t drawlist;
        for (auto&&[entity, visual, coord] : ecos.view<com_visual_t, com_coord_t>().each())
        {
            auto ent = entity;
            if (visual.valid == _FALSE) { continue; }
            if (auto*family = ecos.try_get<com_family_t>(entity))
            { if (ecos.valid(family->ancestor)) { continue; } }
            auto heritage = heritage_t{
                .coord = coord_t{0,0,0},
                .sizes = sizes_t{0,0},
                .scale = scale_t{1,1},
            };
            if (auto*relpos = ecos.try_get<com_relpos_t>(entity))
            {
                heritage.coord =*coord_v;
                heritage.sizes =*sizes_v;
                heritage.scale =*scale_v;
            }
            drawlist.push_back({entity,visual,coord,heritage});
        }
        proc_list(drawlist);
    }
    if constexpr (_TRUTH)
    { /* finish the frame */
        glutPostRedisplay();
    }
    if constexpr (_TRUTH)
    { /* to-hero adjustment */
        auto ent_hero = get_by_ename({_ENAME_GAME_HERO});
        auto anchor_h = ecos.get<com_anchor_t>(ent_hero);
        auto coord_h = ecos.get<com_coord_t>(ent_hero);
        auto sizes_h = ecos.get<com_sizes_t>(ent_hero);
        auto scale_h = ecos.get<com_scale_t>(ent_hero);
        if (get_key_mode() != _KEY_MODE_VIEW)
        {
            *coord_v = coord_h;
            /*
            coord_v->x -= get_anchor_coord(anchor_h.x, sizes_h.w, scale_h.x);
            coord_v->y -= get_anchor_coord(anchor_h.y, sizes_h.h, scale_h.y);
            */
        }
    }
}

/*** files ***/

static void proc_image(image_t&image)
{
    image.msize = image.sizes.w * image.sizes.h * image.mstep;
    GLuint glint = 0;
    glGenTextures(1, &glint);
    glBindTexture(GL_TEXTURE_2D, glint);
    glTexImage2D(
        GL_TEXTURE_2D, 0,
        image.mstep, image.sizes.w, image.sizes.h, 0,
        image.mstep == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
        image.mdata
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    image.glint = glint;
}

void load_image_from_fpath_into_value(const std::string&fpath, image_t&image)
{
    image.mdata = stbi_load(&fpath[0], &image.sizes.w, &image.sizes.h, &image.mstep, 0);
    if (image.mdata) { proc_image(image); }
    else { std::cerr << "failed image loading: " << fpath << std::endl; }
}

void load_image_from_fpath_into_index(const std::string&fpath, index_t index)
{
    if (index < _IMAGE_COUNT)
    {
        load_image_from_fpath_into_value(fpath, image_list[index]);
    }
    else
    {
    }
}

void load_image_from_value_into_index(const image_t&image, index_t index)
{
    if (index < _IMAGE_COUNT)
    {
        image_list[index] = image;
        proc_image(image_list[index]);
    }
    else
    {
    }
}

/*** view ***/

void view_goto(const coord_t&coord)
{
    coord_v->x = (coord.x + TILE_COUNT_X/2) % TILE_COUNT_X;
    coord_v->y = (coord.y + TILE_COUNT_Y/2) % TILE_COUNT_Y;
    coord_v->z = (coord.z + TILE_COUNT_Z/2) % TILE_COUNT_Z;
}
void view_goto_x(v1s_t gotox)
{
    coord_v->x = (gotox + TILE_COUNT_X/2) % TILE_COUNT_X;
}
void view_goto_y(v1s_t gotoy)
{
    coord_v->y = (gotoy + TILE_COUNT_Y/2) % TILE_COUNT_Y;
}
void view_goto_z(v1s_t gotoz)
{
    coord_v->z = (gotoz + TILE_COUNT_Z/2) % TILE_COUNT_Z;
}

void view_move(const coord_t&coord) {
    view_goto({
        .x = coord.x * (TILE_SCALE_X * scale_v->x),
        .y = coord.y * (TILE_SCALE_Y * scale_v->y),
        .z = coord.z,
    });
}

void view_zoom(const scale_t&scale)
{
    if (scale.x < 0)
    {
        scale_v->x >>= (-scale.x);
    }
    else
    {
        scale_v->x <<= (+scale.x);
    }
    scale_v->x = std::clamp(scale_v->x, 1, 2 << 16);
    if (scale.y < 0)
    {
        scale_v->y >>= (-scale.y);
    }
    else
    {
        scale_v->y <<= (+scale.y);
    }
    scale_v->y = std::clamp(scale_v->y, 1, 2 << 16);
}

void view_turn(const bool_t lside)
{
    *direc_v = get_vec_turn(*direc_v, lside);
}

_NAMESPACE_LEAVE
