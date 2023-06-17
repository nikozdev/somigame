/* headers */

#include "gfix.hxx"
#include "main.hxx"
#include "util.hxx"
#include "iput.hxx"
#include "game.hxx"

#include "../lib/stbl/src/stbi.hxx"

/* content */

_NAMESPACE_ENTER

/** typedef **/

struct heritage_t
{
    com_coord_t coord = { 0, 0 };
    com_sizes_t sizes = { 0, 0 };
    com_scale_t scale = { 0, 0 };
};
struct drawable_t
{
    ent_t entity;
    com_visual_t visual;
    heritage_t heritage;
};
using drawlist_t = std::vector<drawable_t>;

/** datadef **/

/*** system ***/

gfix_t gfix;

struct window_t {
    scale_t ratio = { .x = RATIO_X, .y = RATIO_Y };
    coord_t coord = { .x = 0, .y = 0 };
    sizes_t sizes = { .w = CAMERA_SIZES_W, .h = CAMERA_SIZES_H };
} window;

/*** vision ***/

struct camera_t {
    index_t layer = 0;
    scale_t scale = { .x = 1, .y = 1 };
    scale_t ratio = { .x = RATIO_X, .y = RATIO_Y };
    coord_t coord = { .x = 0, .y = 0 };
    sizes_t sizes = { .w = CAMERA_SIZES_W, .h = CAMERA_SIZES_H };
    index_t vfdir = _VFDIR_N;
} camera;

/*** assets ***/

image_t image_list[_IMAGE_COUNT];

/*** meta ***/

constexpr const int IMAGE_NONE_MSTEP = 4;
constexpr const com_sizes_t IMAGE_NONE_SIZES = { .w = 1, .h = 1, };
unsigned char IMAGE_NONE_MDATA[] = { 0xff,0xff,0xff,0xff };

constexpr const int IMAGE_TEST_MSTEP = 4;
constexpr const com_sizes_t IMAGE_TEST_SIZES = { .w = 4, .h = 4, };
unsigned char IMAGE_TEST_MDATA[] = {
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
    load_image_from_fpath_into_index("rsc/gfix/entt-main-ff-ai-f1.png", _IMAGE_ENTT_MAIN);
    load_image_from_fpath_into_index("rsc/gfix/tile-test-st-tmm.png", _IMAGE_TILE_TEST);
    load_image_from_fpath_into_index("rsc/gfix/font-main-8x8.png", _IMAGE_FONT_MAIN);
    /* keybinds */
    /** view **/
    /*** goto ***/
    key_bind_set("vgx", [](int narg){ view_goto({narg,camera.coord.y}); });
    key_bind_set("vgy", [](int narg){ view_goto({camera.coord.x,narg}); });
    /*** move ***/
    key_bind_set("va", [](int narg){ view_move({.x=(-1)*(get_num_sign(narg)+narg),.y=0}); });
    key_bind_set("vd", [](int narg){ view_move({.x=(+1)*(get_num_sign(narg)+narg),.y=0}); });
    key_bind_set("vs", [](int narg){ view_move({.x=0,.y=(-1)*(get_num_sign(narg)+narg)}); });
    key_bind_set("vw", [](int narg){ view_move({.x=0,.y=(+1)*(get_num_sign(narg)+narg)}); });
    key_bind_set("ve", [](int narg){ view_turn(_TRUTH); });
    key_bind_set("vq", [](int narg){ view_turn(_FALSE); });
    /*** zoom ***/
    key_bind_set("vz", [](int narg){ view_zoom({.x=narg,.y=narg}); });
    /** graphics **/
    /*** mode ***/
    /**** wire ****/
    key_bind_set("gmw", [](int narg){ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); });
    /**** fill ****/
    key_bind_set("gmf", [](int narg){ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); });
    /* entity component system */
    using reg_t = entt::registry;
    constexpr auto&reg = ecos.reg;
    reg.on_construct<com_image_t>().connect<[](reg_t&reg, ent_t ent)
    {
        auto&image = reg.get<com_image_t>(ent);
        if (image.index >= _IMAGE_COUNT) { image.index = _IMAGE_META_TEST; }
        if (image.sizes.w <= 0) { image.sizes.w = image_list[image.index].sizes.w; }
        if (image.sizes.h <= 0) { image.sizes.h = image_list[image.index].sizes.h; }
    }>();
    reg.on_destroy<com_image_t>().connect<[](reg_t&reg, ent_t ent)
    {
        auto&image = reg.get<com_image_t>(ent);
    }>();
    if constexpr (_TRUTH)
    { /* guis entity */
        ent_t ent_guis = reg.create();
        gfix.ent_guis = ent_guis;
        reg.emplace<com_family_t>(ent_guis, ent_guis);
        reg.emplace<com_visual_t>(ent_guis, _TRUTH, GUIS_LAYER);
        reg.emplace<com_relpos_t>(ent_guis, RELPOS_MID, RELPOS_MID);
        reg.emplace<com_anchor_t>(ent_guis, RELPOS_MID, RELPOS_MID);
        reg.emplace<com_ename_t>(ent_guis, _ENAME_GUIS);
        reg.emplace<com_coord_t>(ent_guis, 0, 0);
        reg.emplace<com_sizes_t>(ent_guis,
            camera.sizes.w,
            camera.sizes.h
        );
        if constexpr (_TRUTH)
        { /* middle screen */
            ent_t ent_quad = reg.create();
            reg.emplace<com_family_t>(ent_quad, ent_guis);
            reg.emplace<com_visual_t>(ent_quad, _TRUTH, 0);
            reg.emplace<com_relpos_t>(ent_quad, RELPOS_MID, RELPOS_MID);
            reg.emplace<com_anchor_t>(ent_quad, RELPOS_MID, RELPOS_MID);
            reg.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_MM_QUAD);
            reg.emplace<com_coord_t>(ent_quad, 0, 0);
            reg.emplace<com_color_t>(ent_quad, 255);
            reg.emplace<com_sizes_t>(ent_quad,
                camera.sizes.w - GUIS_SIZES_X*2,
                camera.sizes.h - GUIS_SIZES_Y*2
            );
            reg.emplace<com_image_t>(ent_quad,
                _IMAGE_META_NONE,
                coord_t{0,0},
                sizes_t{0,0}
            );
            if constexpr(_TRUTH)
            { /* font image */
                ent_t ent_font = reg.create();
                reg.emplace<com_family_t>(ent_font, ent_quad);
                reg.emplace<com_visual_t>(ent_font, _TRUTH, 2);
                reg.emplace<com_relpos_t>(ent_font, RELPOS_MIN, RELPOS_MID);
                reg.emplace<com_anchor_t>(ent_font, RELPOS_MIN, RELPOS_MID);
                reg.emplace<com_coord_t>(ent_font, 0, 0);
                reg.emplace<com_sizes_t>(ent_font, 128/2, 48/2);
                /*
                reg.emplace<com_color_t>(ent_font, 255);
                reg.emplace<com_image_t>(ent_font,
                    _IMAGE_FONT_MAIN,
                    coord_t{0,0},
                    sizes_t{0,0}
                );
                */
            }
            if constexpr (_FALSE)
            { /* testing text */
                auto ent_text = reg.create();
                reg.emplace<com_family_t>(ent_text, ent_quad);
                reg.emplace<com_visual_t>(ent_text, _TRUTH, 5);
                reg.emplace<com_relpos_t>(ent_text, RELPOS_MID, RELPOS_MID);
                reg.emplace<com_anchor_t>(ent_text, RELPOS_MID, RELPOS_MID);
                reg.emplace<com_coord_t>(ent_text, 0, 0);
                /*
                reg.emplace<com_sizes_t>(ent_text, IMAGE_FONT_SIZES_X * 5, IMAGE_FONT_SIZES_Y * 2);
                */
                reg.emplace<com_sizes_t>(ent_text, IMAGE_FONT_SIZES_X, IMAGE_FONT_SIZES_Y);
                reg.emplace<com_color_t>(ent_text, 240);
                reg.emplace<com_cstring_t>(ent_text);
                sprintf(reg.get<com_cstring_t>(ent_text).mdata, "Hello\nWorld");
            }
            if constexpr (_FALSE)
            { /* test-mm */
                ent_t ent_test = reg.create();
                reg.emplace<com_family_t>(ent_test, ent_quad);
                reg.emplace<com_visual_t>(ent_test, _TRUTH, 3);
                reg.emplace<com_anchor_t>(ent_test, RELPOS_MID, RELPOS_MID);
                reg.emplace<com_relpos_t>(ent_test, RELPOS_MID, RELPOS_MID);
                reg.emplace<com_coord_t>(ent_test, 0, 0);
                reg.emplace<com_sizes_t>(ent_test, UNIT_SCALE_X, UNIT_SCALE_Y);
                reg.emplace<com_color_t>(ent_test, 196);
                reg.emplace<com_image_t>(ent_test,
                    _IMAGE_META_TEST,
                    coord_t{0,0},
                    sizes_t{0,0}
                );
                if constexpr (_TRUTH)
                { /* test-mb */
                    ent_t ent_test_mb = reg.create();
                    reg.emplace<com_family_t>(ent_test_mb, ent_test);
                    reg.emplace<com_visual_t>(ent_test_mb, _TRUTH, 0);
                    reg.emplace<com_relpos_t>(ent_test_mb, RELPOS_MID, RELPOS_MIN);
                    reg.emplace<com_anchor_t>(ent_test_mb, RELPOS_MID, RELPOS_MID);
                    reg.emplace<com_coord_t>(ent_test_mb, 0, 0);
                    reg.emplace<com_sizes_t>(ent_test_mb, UNIT_SCALE_X, UNIT_SCALE_Y);
                    reg.emplace<com_color_t>(ent_test_mb, 128);
                    reg.emplace<com_image_t>(ent_test_mb,
                        _IMAGE_META_TEST,
                        coord_t{0,0},
                        sizes_t{0,0}
                    );
                    if constexpr (_TRUTH)
                    { /* test-mb-rt */
                        ent_t ent_test_mb_rt = reg.create();
                        reg.emplace<com_family_t>(ent_test_mb_rt, ent_test_mb);
                        reg.emplace<com_visual_t>(ent_test_mb_rt, _TRUTH, 0);
                        reg.emplace<com_relpos_t>(ent_test_mb_rt, RELPOS_MAX, RELPOS_MAX);
                        reg.emplace<com_anchor_t>(ent_test_mb_rt, RELPOS_MIN, RELPOS_MAX);
                        reg.emplace<com_coord_t>(ent_test_mb_rt, 0, 0);
                        reg.emplace<com_sizes_t>(ent_test_mb_rt, UNIT_SCALE_X, UNIT_SCALE_Y);
                        reg.emplace<com_color_t>(ent_test_mb_rt, 32);
                        reg.emplace<com_image_t>(ent_test_mb_rt,
                            _IMAGE_META_TEST,
                            coord_t{0,0},
                            sizes_t{0,0}
                        );
                    }
                }
                if constexpr (_TRUTH)
                { /* test-mt */
                    ent_t ent_test_mt = reg.create();
                    reg.emplace<com_family_t>(ent_test_mt, ent_test);
                    reg.emplace<com_visual_t>(ent_test_mt, _TRUTH, 0);
                    reg.emplace<com_relpos_t>(ent_test_mt, RELPOS_MID, RELPOS_MAX);
                    reg.emplace<com_anchor_t>(ent_test_mt, RELPOS_MID, RELPOS_MID);
                    reg.emplace<com_coord_t>(ent_test_mt, 0, 0);
                    reg.emplace<com_sizes_t>(ent_test_mt, UNIT_SCALE_X, UNIT_SCALE_Y);
                    reg.emplace<com_color_t>(ent_test_mt, 128);
                    reg.emplace<com_image_t>(ent_test_mt,
                        _IMAGE_META_TEST,
                        coord_t{0,0},
                        sizes_t{0,0}
                    );
                }
            }
            if constexpr (_FALSE)
            { /* test-lb */
                ent_t ent_test = reg.create();
                reg.emplace<com_family_t>(ent_test, ent_quad);
                reg.emplace<com_visual_t>(ent_test, _TRUTH, 3);
                reg.emplace<com_relpos_t>(ent_test, RELPOS_MIN, RELPOS_MIN);
                reg.emplace<com_anchor_t>(ent_test, RELPOS_MIN, RELPOS_MIN);
                reg.emplace<com_coord_t>(ent_test, 0, 0);
                reg.emplace<com_sizes_t>(ent_test, UNIT_SCALE_X, UNIT_SCALE_Y);
                reg.emplace<com_color_t>(ent_test, 128);
                reg.emplace<com_image_t>(ent_test,
                    _IMAGE_META_TEST,
                    coord_t{0,0},
                    sizes_t{0,0}
                );
            }
            if constexpr (_FALSE)
            { /* test-rt */
                ent_t ent_test = reg.create();
                reg.emplace<com_family_t>(ent_test, ent_quad);
                reg.emplace<com_visual_t>(ent_test, _TRUTH, 3);
                reg.emplace<com_relpos_t>(ent_test, RELPOS_MAX, RELPOS_MAX);
                reg.emplace<com_anchor_t>(ent_test, RELPOS_MAX, RELPOS_MAX);
                reg.emplace<com_coord_t>(ent_test, 0, 0);
                reg.emplace<com_sizes_t>(ent_test, UNIT_SCALE_X, UNIT_SCALE_Y);
                reg.emplace<com_color_t>(ent_test, 240);
                reg.emplace<com_image_t>(ent_test,
                    _IMAGE_META_TEST,
                    coord_t{0,0},
                    sizes_t{0,0}
                );
            }
        }
        if constexpr (_TRUTH)
        { /* gui-mb */
            ent_t ent_quad = reg.create();
            reg.emplace<com_family_t>(ent_quad, ent_guis);
            reg.emplace<com_visual_t>(ent_quad, _TRUTH, 0);
            reg.emplace<com_relpos_t>(ent_quad, RELPOS_MID, RELPOS_MIN);
            reg.emplace<com_anchor_t>(ent_quad, RELPOS_MID, RELPOS_MIN);
            reg.emplace<com_coord_t>(ent_quad, 0, 0);
            reg.emplace<com_sizes_t>(ent_quad, camera.sizes.w - GUIS_SIZES_X*2, GUIS_SIZES_Y);
            reg.emplace<com_color_t>(ent_quad, 64);
            reg.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_MB_QUAD);
            reg.emplace<com_image_t>(ent_quad,
                _IMAGE_META_NONE,
                coord_t{0,0},
                sizes_t{0,0}
            );
            if constexpr (_TRUTH)
            { /* text-mb */
                auto ent_text = reg.create();
                reg.emplace<com_family_t>(ent_text, ent_quad);
                reg.emplace<com_visual_t>(ent_text, _TRUTH, 1);
                reg.emplace<com_relpos_t>(ent_text, RELPOS_MIN, RELPOS_MAX);
                reg.emplace<com_anchor_t>(ent_text, RELPOS_MIN, RELPOS_MAX);
                reg.emplace<com_coord_t>(ent_text, 0, 0);
                reg.emplace<com_sizes_t>(ent_text, IMAGE_FONT_SIZES_X, IMAGE_FONT_SIZES_Y);
                reg.emplace<com_color_t>(ent_text, 240);
                reg.emplace<com_ename_t>(ent_text, _ENAME_GUIS_MB_TEXT);
                reg.emplace<com_cstring_t>(ent_text);
            }
            if constexpr (_TRUTH)
            { /* logo-mb */
                ent_t ent_logo = reg.create();
                reg.emplace<com_family_t>(ent_logo, ent_quad);
                reg.emplace<com_visual_t>(ent_logo, _TRUTH, 3);
                reg.emplace<com_relpos_t>(ent_logo, RELPOS_MAX, RELPOS_MID);
                reg.emplace<com_anchor_t>(ent_logo, RELPOS_MIN, RELPOS_MID);
                reg.emplace<com_coord_t>(ent_logo, 0, 0);
                reg.emplace<com_sizes_t>(ent_logo, UNIT_SCALE_X, UNIT_SCALE_Y);
                reg.emplace<com_color_t>(ent_logo, 255);
                reg.emplace<com_image_t>(ent_logo,
                    _IMAGE_META_LOGO,
                    coord_t{0,0},
                    sizes_t{0,0}
                );
            }
            if constexpr (_FALSE)
            { /* logo-mm */
                ent_t ent_logo = reg.create();
                reg.emplace<com_family_t>(ent_logo, ent_quad);
                reg.emplace<com_visual_t>(ent_logo, _TRUTH, 3);
                reg.emplace<com_relpos_t>(ent_logo, RELPOS_MID, RELPOS_MID);
                reg.emplace<com_anchor_t>(ent_logo, RELPOS_MID, RELPOS_MID);
                reg.emplace<com_coord_t>(ent_logo, 0, 0);
                reg.emplace<com_sizes_t>(ent_logo, UNIT_SCALE_X, UNIT_SCALE_Y);
                reg.emplace<com_color_t>(ent_logo, 255);
                reg.emplace<com_image_t>(ent_logo,
                    _IMAGE_META_LOGO,
                    coord_t{0,0},
                    sizes_t{0,0}
                );
            }
        }
        if constexpr (_TRUTH)
        { /* gui-mt */
            ent_t ent_quad = reg.create();
            reg.emplace<com_family_t>(ent_quad, ent_guis);
            reg.emplace<com_visual_t>(ent_quad, _TRUTH, 0);
            reg.emplace<com_relpos_t>(ent_quad, RELPOS_MID, RELPOS_MAX);
            reg.emplace<com_anchor_t>(ent_quad, RELPOS_MID, RELPOS_MAX);
            reg.emplace<com_coord_t>(ent_quad, 0, 0);
            reg.emplace<com_color_t>(ent_quad, 64);
            reg.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, coord_t{0,0}, sizes_t{0,0});
            reg.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_MT_QUAD);
            reg.emplace<com_sizes_t>(ent_quad,
                camera.sizes.w - GUIS_SIZES_X*2,
                GUIS_SIZES_Y
            );
            if constexpr (_TRUTH)
            {
                auto ent_text = reg.create();
                reg.emplace<com_family_t>(ent_text, ent_quad);
                reg.emplace<com_visual_t>(ent_text, _TRUTH, 1);
                reg.emplace<com_relpos_t>(ent_text, RELPOS_MIN, RELPOS_MIN);
                reg.emplace<com_anchor_t>(ent_text, RELPOS_MIN, RELPOS_MIN);
                reg.emplace<com_coord_t>(ent_text, 0, 0);
                reg.emplace<com_sizes_t>(ent_text, IMAGE_FONT_SIZES_X, IMAGE_FONT_SIZES_Y);
                reg.emplace<com_color_t>(ent_text, 240);
                reg.emplace<com_ename_t>(ent_text, _ENAME_GUIS_MT_TEXT);
                reg.emplace<com_cstring_t>(ent_text);
            }
        }
        if constexpr (_TRUTH)
        { /* gui-lm */
            ent_t ent_quad = reg.create();
            reg.emplace<com_family_t>(ent_quad, ent_guis);
            reg.emplace<com_visual_t>(ent_quad, _TRUTH, 0);
            reg.emplace<com_relpos_t>(ent_quad, RELPOS_MIN, RELPOS_MID);
            reg.emplace<com_anchor_t>(ent_quad, RELPOS_MIN, RELPOS_MID);
            reg.emplace<com_coord_t>(ent_quad, 0, 0);
            reg.emplace<com_sizes_t>(ent_quad,
                GUIS_SIZES_X,
                camera.sizes.h - GUIS_SIZES_X*2
            );
            reg.emplace<com_color_t>(ent_quad, 64);
            reg.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_LM_QUAD);
            reg.emplace<com_image_t>(ent_quad,
                _IMAGE_META_NONE,
                coord_t{0,0},
                sizes_t{0,0}
            );
        }
        if constexpr (_TRUTH)
        { /* gui-rm */
            ent_t ent_quad = reg.create();
            reg.emplace<com_family_t>(ent_quad, ent_guis);
            reg.emplace<com_visual_t>(ent_quad, _TRUTH, 0);
            reg.emplace<com_relpos_t>(ent_quad, RELPOS_MAX, RELPOS_MID);
            reg.emplace<com_anchor_t>(ent_quad, RELPOS_MAX, RELPOS_MID);
            reg.emplace<com_coord_t>(ent_quad, 0, 0);
            reg.emplace<com_sizes_t>(ent_quad, GUIS_SIZES_X, camera.sizes.h - GUIS_SIZES_Y*2);
            reg.emplace<com_color_t>(ent_quad, 64);
            reg.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, coord_t{0,0}, sizes_t{0,0});
            reg.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_RM_QUAD);
        }
    }
}

/*** drawing ***/

static void draw_init()
{
    /* window */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(window.coord.x, window.coord.y, window.sizes.w, window.sizes.h);
    /* camera */
    camera.sizes.w = CAMERA_SIZES_W;
    camera.sizes.h = CAMERA_SIZES_H;
    auto xl = camera.coord.x - camera.sizes.w * camera.scale.x / 2;
    auto xr = camera.coord.x + camera.sizes.w * camera.scale.x / 2;
    auto yb = camera.coord.y - camera.sizes.h * camera.scale.y / 2;
    auto yt = camera.coord.y + camera.sizes.h * camera.scale.y / 2;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xl, xr, yb, yt);
}

extern void on_list(drawlist_t&drawlist);
void on_draw(drawable_t&drawable)
{
    constexpr auto&reg = ecos.reg;
    auto&entity = drawable.entity;
    auto&visual_e = drawable.visual;
    auto&heritage = drawable.heritage;
    /* props */
    auto color_e = color_t{ .value = 255u };
    if (auto*temp = reg.try_get<com_color_t>(entity)) { color_e =*temp; }
    auto&coord_a = heritage.coord;
    auto coord_e = coord_t { .x = 0, .y = 0 };
    if (auto*temp = reg.try_get<com_coord_t>(entity)) { coord_e =*temp; }
    auto&sizes_a = heritage.sizes;
    auto sizes_e = sizes_t{ .w = UNIT_SCALE_X, .h = UNIT_SCALE_Y };
    if (auto*temp = reg.try_get<com_sizes_t>(entity)) { sizes_e =*temp; }
    auto&scale_a = heritage.scale;
    auto scale_e = scale_t{ .x = 1, .y = 1 };
    if (auto*temp = reg.try_get<com_scale_t>(entity)) { scale_e =*temp; }
    scale_e.x *= scale_a.x;
    scale_e.y *= scale_a.y;
    /* coord */
    auto coord_xl = coord_e.x;
    auto coord_xr = coord_e.x;
    auto coord_yb = coord_e.y;
    auto coord_yt = coord_e.y;
    if (auto*relpos_e = reg.try_get<com_relpos_t>(entity))
    {
        auto offsetx = (sizes_a.w * relpos_e->x) / RELPOS_DIV;
        auto offsety = (sizes_a.h * relpos_e->y) / RELPOS_DIV;
        coord_xl = (coord_xl + offsetx) * scale_a.x + coord_a.x;
        coord_xr = (coord_xr + offsetx) * scale_a.x + coord_a.x;
        coord_yb = (coord_yb + offsety) * scale_a.y + coord_a.y;
        coord_yt = (coord_yt + offsety) * scale_a.y + coord_a.y;
        coord_e.x = (coord_e.x + offsetx) * scale_a.x + coord_a.x;
        coord_e.y = (coord_e.y + offsety) * scale_a.y + coord_a.y;
    }
    if (auto*anchor_e = reg.try_get<com_anchor_t>(entity))
    {
        coord_xl += (sizes_e.w * scale_e.x * (RELPOS_MIN - anchor_e->x)) / RELPOS_DIV;
        coord_xr += (sizes_e.w * scale_e.x * (RELPOS_MAX - anchor_e->x)) / RELPOS_DIV;
        coord_yb += (sizes_e.h * scale_e.y * (RELPOS_MIN - anchor_e->y)) / RELPOS_DIV;
        coord_yt += (sizes_e.h * scale_e.y * (RELPOS_MAX - anchor_e->y)) / RELPOS_DIV;
        coord_e.x -= (sizes_e.w * scale_e.x * anchor_e->x) / RELPOS_DIV;
        coord_e.y -= (sizes_e.h * scale_e.y * anchor_e->y) / RELPOS_DIV;
    }
    /* draw */
    glColor3ub(color_e.value, color_e.value, color_e.value);
    if (auto*cstring = reg.try_get<com_cstring_t>(entity))
    {
        const auto&image = image_list[_IMAGE_FONT_MAIN];
        auto image_uw = static_cast<v1u_t>(image.sizes.w);
        auto image_uh = static_cast<v1u_t>(image.sizes.h);
        auto image_fw = static_cast<float>(image_uw);
        auto image_fh = static_cast<float>(image_uh);
        auto glyph_count_uw = static_cast<v1u_t>(image_uw / IMAGE_FONT_SIZES_X);
        auto glyph_count_uh = static_cast<v1u_t>(image_uh / IMAGE_FONT_SIZES_Y);
        auto glyph_uw = static_cast<v1u_t>(image_uw / glyph_count_uw);
        auto glyph_uh = static_cast<v1u_t>(image_uh / glyph_count_uh);
        auto glyph_fw = static_cast<float>(IMAGE_FONT_SIZES_X) / image_fw;
        auto glyph_fh = static_cast<float>(IMAGE_FONT_SIZES_Y) / image_fh;
        auto image_glint = image.glint;
        auto image_xl = 0.0f;
        auto image_xr = 0.0f;
        auto image_yb = 0.0f;
        auto image_yt = 0.0f;
        auto offset_x = 0;
        auto offset_y = 0;
        glBindTexture(GL_TEXTURE_2D, image_glint);
        auto mdata = cstring->mdata;
        auto msize = cstring->msize;
        auto schar_count_uw = 0u, schar_count_uw_iter = 0u;
        auto schar_count_uh = 0u;
        for (auto iter = 0; (mdata[iter] > 0) && (iter < msize); iter++)
        {
            int mbyte = std::max(mdata[iter], ' ');
            if (mbyte == '\n')
            { schar_count_uh += 1; schar_count_uw_iter = 1; }
            else
            { schar_count_uw = std::max(schar_count_uw, ++schar_count_uw_iter); }
        }
        auto schar_sizes_fw = static_cast<float>(schar_count_uw) / static_cast<float>(sizes_e.w);
        auto schar_sizes_fh = static_cast<float>(schar_count_uh) / static_cast<float>(sizes_e.h);
        for (auto iter = 0; (mdata[iter] > 0) && (iter < msize); iter++)
        {
            int mbyte = mdata[iter];
            if (mbyte == '\n')
            {
                coord_xl -= offset_x;
                coord_xr -= offset_x;
                offset_x = 0;
                coord_yb -= glyph_uh;
                coord_yt -= glyph_uh;
                offset_y -= glyph_uh;
                continue;
            }
            mbyte = std::max(mdata[iter], ' ') - ' ';
            coord_xl += glyph_uw;
            coord_xr += glyph_uw;
            offset_x += glyph_uw;
            auto stepx = (mbyte % glyph_count_uw);
            image_xl = static_cast<float>(stepx * glyph_uw);
            image_xl = image_xl / image_fw;
            image_xr = image_xl + glyph_fw;
            auto stepy = glyph_count_uh - (mbyte / glyph_count_uw);
            image_yt = static_cast<float>(stepy * glyph_uw);
            image_yt = image_yt / image_fh;
            image_yb = image_yt - glyph_fh;
            glBegin(GL_QUADS);
            glTexCoord2f(image_xl, image_yb);
            glVertex2i(coord_xl, coord_yb);
            glTexCoord2f(image_xr, image_yb);
            glVertex2i(coord_xr, coord_yb);
            glTexCoord2f(image_xr, image_yt);
            glVertex2i(coord_xr, coord_yt);
            glTexCoord2f(image_xl, image_yt);
            glVertex2i(coord_xl, coord_yt);
            glEnd();
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    if (auto*image = reg.try_get<com_image_t>(entity))
    {
        auto image_region = *image;
        auto image_index = image_region.index;
        auto image_glint = 0u;
        auto image_xl = static_cast<float>(image_region.coord.x);
        auto image_xr = image_xl + static_cast<float>(image_region.sizes.w);
        auto image_yb = static_cast<float>(image_region.coord.y);
        auto image_yt = image_yb + static_cast<float>(image_region.sizes.h);
        if (image_index < _IMAGE_COUNT)
        {
            auto&image_origin = image_list[image_index];
            image_glint = image_origin.glint;
            float image_w = static_cast<float>(image_origin.sizes.w);
            float image_h = static_cast<float>(image_origin.sizes.h);
            image_xl /= image_w; image_xr /= image_w;
            image_yb /= image_h; image_yt /= image_h;
        }
        glBindTexture(GL_TEXTURE_2D, image_glint);
        glBegin(GL_QUADS);
        glTexCoord2f(image_xl, image_yb);
        glVertex2i(coord_xl, coord_yb);
        glTexCoord2f(image_xr, image_yb);
        glVertex2i(coord_xr, coord_yb);
        glTexCoord2f(image_xr, image_yt);
        glVertex2i(coord_xr, coord_yt);
        glTexCoord2f(image_xl, image_yt);
        glVertex2i(coord_xl, coord_yt);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    /* hierarchy */
    if (auto*family = reg.try_get<com_family_t>(drawable.entity))
    {
        drawlist_t drawlist;
        ecos_call_for_children(entity, [&](ent_t children){
            if (auto*visual = reg.try_get<com_visual_t>(children))
            {
                if (visual->visible)
                {
                    drawlist.push_back(drawable_t{
                        .entity = children,
                        .visual =*visual,
                        .heritage = {
                            .coord = coord_e,
                            .sizes = sizes_e,
                            .scale = scale_e,
                        }
                    });
                }
            }
        });
        on_list(drawlist);
    }
}
void on_list(drawlist_t&drawlist)
{
    constexpr auto&reg = ecos.reg;
    auto head = drawlist.begin();
    auto tail = drawlist.end();
    std::sort(head, tail, [](const drawable_t& dl, const drawable_t& dr) {
        auto&reg = ecos.reg;
        auto layer_l = dl.visual.layer;
        auto layer_r = dr.visual.layer;
        return layer_l < layer_r;
    });
    for (auto iter = head; iter != tail; iter++)
    {
        if (iter->visual.visible) { on_draw(*iter); }
    }
}

static void draw_proc()
{
    /* entity component system */
    constexpr auto&reg = ecos.reg;
    if constexpr(_TRUTH)
    {
        auto&cstring = reg.get<com_cstring_t>(ecos_get_by_ename(ename_t{_ENAME_GUIS_MB_TEXT}));
        memset(cstring.mdata, '\0', cstring.msize);
        std::sprintf(
            cstring.mdata,
            "[now=%03zu.%03zu;fps=%05zu;]\n[pos=%ix%i;scale=%ix%i;]",
            util.timer.now_mil / 1000, util.timer.now_mil % 1000, util.timer.fps_num,
            camera.coord.x, camera.coord.y,
            camera.scale.x, camera.scale.y
        );
    }
    if constexpr(_TRUTH)
    {
        auto&cstring = reg.get<com_cstring_t>(ecos_get_by_ename(ename_t{_ENAME_GUIS_MT_TEXT}));
        memset(cstring.mdata, '\0', cstring.msize);
        std::sprintf(
            cstring.mdata,
            "%s(%+dx%d)",
            &key_line[0], key_narg_sign, key_narg
        );
    }
    camera.layer = reg.get<com_visual_t>(game.ent_main).layer;
    /* draw hierarchies from top to bottom */
    drawlist_t drawlist;
    for (auto [entity, visual] : reg.view<com_visual_t>().each())
    {
        auto ent = entity;
        if (visual.visible == _FALSE) { continue; }
        if (auto*family = reg.try_get<com_family_t>(entity))
        { if (reg.valid(family->ancestor)) { continue; } }
        auto heritage = heritage_t{};
        if (auto*relpos = reg.try_get<com_relpos_t>(entity))
        {
            heritage.coord = camera.coord;
            heritage.sizes = camera.sizes;
            heritage.scale = camera.scale;
        }
        drawlist.push_back({entity,visual,heritage});
    }
    on_list(drawlist);
}

static void draw_quit()
{
    glutSwapBuffers();
}

void draw_loop()
{
    draw_init();
    draw_proc();
    draw_quit();
}

/*** files ***/

static void proc_image(image_t&image)
{
    image.msize = image.sizes.w * image.sizes.h * image.mstep;
    glGenTextures(1, &image.glint);
    glBindTexture(GL_TEXTURE_2D, image.glint);
    glTexImage2D(
        GL_TEXTURE_2D, 0,
        image.mstep, image.sizes.w, image.sizes.h, 0,
        image.mstep == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
        image.mdata
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
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
    camera.coord.x = coord.x;
    camera.coord.y = coord.y;
}

void view_move(const coord_t&coord)
{
    camera.coord.x += coord.x * (UNIT_SCALE_X * camera.scale.x);
    camera.coord.y += coord.y * (UNIT_SCALE_Y * camera.scale.y);
}

void view_zoom(const scale_t&scale)
{
    if (scale.x < 0)
    {
        camera.scale.x >>= (-scale.x);
    }
    else
    {
        camera.scale.x <<= (+scale.x);
    }
    camera.scale.x = std::clamp(camera.scale.x, 1, 2 << 16);
    if (scale.y < 0)
    {
        camera.scale.y >>= (-scale.y);
    }
    else
    {
        camera.scale.y <<= (+scale.y);
    }
    camera.scale.y = std::clamp(camera.scale.y, 1, 2 << 16);
}

void view_turn(bool_t rside)
{
    camera.vfdir = (camera.vfdir - 1*rside + 1*rside) % _VFDIR_COUNT;
}

_NAMESPACE_LEAVE
