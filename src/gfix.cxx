/* headers */

#include "gfix.hxx"
#include "util.hxx"
#include "iput.hxx"
#include "fsix.hxx"
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
using drawlist_t = std::vector<drawable_t>;

/** datadef **/

/*** system ***/

gfix_t gfix;

struct window_t {
    scale_t ratio = { .x = RATIO_X, .y = RATIO_Y };
    pos2d_t coord = { .x = 0, .y = 0 };
    sizes_t sizes = { .w = CAMERA_SIZES_W, .h = CAMERA_SIZES_H };
} window;

/*** vision ***/

struct camera_t {
    scale_t scale = { .x = 1, .y = 1 };
    scale_t ratio = { .x = RATIO_X, .y = RATIO_Y };
    coord_t coord = { .x = 0, .y = 0, .z = 0 };
    sizes_t sizes = { .w = CAMERA_SIZES_W, .h = CAMERA_SIZES_H };
    direc_t direc = { +0, +1 };
    struct { v1s_t sl = 0, sr = 0, sb = 0, st = 0; } ortho;
} camera;

/*** assets ***/

image_origin_t image_list[_IMAGE_COUNT];

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

/** getters **/

const direc_t&get_camera_direc() { return camera.direc; }

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
    load_image_from_fpath_into_index("rsc/gfix/entt-hero-ai-f1.png", _IMAGE_ENTT_HERO);
    load_image_from_fpath_into_index("rsc/gfix/tile-test-st-tmm.png", _IMAGE_TILE_TEST);
    load_image_from_fpath_into_index("rsc/gfix/font-main-8x8.png", _IMAGE_FONT_MAIN);
    /* keybinds */
    /** view **/
    /*** goto ***/
    key_bind_set(_KEY_MODE_VIEW, "gx", [](int narg){ view_goto({narg,camera.coord.y}); });
    key_bind_set(_KEY_MODE_VIEW, "gy", [](int narg){ view_goto({camera.coord.x,narg}); });
    /*** move ***/
    key_bind_set(_KEY_MODE_VIEW, "a", [](int narg){ view_move({.x=(-1)*(get_num_sign(narg)+narg),.y=0}); });
    key_bind_set(_KEY_MODE_VIEW, "d", [](int narg){ view_move({.x=(+1)*(get_num_sign(narg)+narg),.y=0}); });
    key_bind_set(_KEY_MODE_VIEW, "s", [](int narg){ view_move({.x=0,.y=(-1)*(get_num_sign(narg)+narg)}); });
    key_bind_set(_KEY_MODE_VIEW, "w", [](int narg){ view_move({.x=0,.y=(+1)*(get_num_sign(narg)+narg)}); });
    key_bind_set(_KEY_MODE_VIEW, "e", [](int narg){ view_turn(_FALSE); });
    key_bind_set(_KEY_MODE_VIEW, "q", [](int narg){ view_turn(_TRUTH); });
    /*** zoom ***/
    key_bind_set(_KEY_MODE_VIEW, "z", [](int narg){ view_zoom({.x=narg,.y=narg}); });
    /** graphics **/
    /*** mode ***/
    /**** wire ****/
    key_bind_set(_KEY_MODE_MAIN, "gmw", [](int narg){ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); });
    /**** fill ****/
    key_bind_set(_KEY_MODE_MAIN, "gmf", [](int narg){ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); });
    /* entity component system */
    using reg_t = entt::registry;
    constexpr auto&reg = ecos.reg;
    reg.on_construct<com_image_t>().connect<[](reg_t&reg, ent_t ent)
    {
        auto&image = reg.get<com_image_t>(ent);
        if (image.index >= _IMAGE_COUNT) { image.index = _IMAGE_META_TEST; }
        if (image.sizes.w <= 0) { image.sizes.w = image_list[image.index].sizes.w; }
        if (image.sizes.h <= 0) { image.sizes.h = image_list[image.index].sizes.h; }
        if (reg.try_get<com_faces_t>(ent) != _NULL)
        { reg.remove<com_faces_t>(ent); }
    }>();
    reg.on_destroy<com_image_t>().connect<[](reg_t&reg, ent_t ent)
    {
        auto&image = reg.get<com_image_t>(ent);
    }>();
    reg.on_construct<com_faces_t>().connect<[](reg_t&reg, ent_t ent)
    {
        auto&faces = reg.get<com_faces_t>(ent);
        if (reg.try_get<com_image_t>(ent) != _NULL)
        { reg.remove<com_image_t>(ent); }
        if (reg.try_get<com_direc_t>(ent) == _NULL)
        { reg.remove<com_direc_t>(ent); }
    }>();
    reg.on_destroy<com_faces_t>().connect<[](reg_t&reg, ent_t ent)
    {
        auto&faces = reg.get<com_faces_t>(ent);
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
        reg.emplace<com_coord_t>(ent_guis, 0, 0, GUIS_LAYER);
        reg.emplace<com_sizes_t>(ent_guis, camera.sizes.w, camera.sizes.h);
        reg.emplace<com_scale_t>(ent_guis, 1, 1);
        if constexpr (_TRUTH)
        { /* middle screen */
            ent_t ent_quad = reg.create();
            reg.emplace<com_family_t>(ent_quad, ent_guis);
            reg.emplace<com_visual_t>(ent_quad, _TRUTH, 0);
            reg.emplace<com_relpos_t>(ent_quad, RELPOS_MID, RELPOS_MID);
            reg.emplace<com_anchor_t>(ent_quad, RELPOS_MID, RELPOS_MID);
            reg.emplace<com_coord_t>(ent_quad, 0, 0, 0);
            reg.emplace<com_sizes_t>(ent_quad, camera.sizes.w - GUIS_SIZES_X*2, camera.sizes.h - GUIS_SIZES_Y*2);
            reg.emplace<com_scale_t>(ent_quad, 1, 1);
            reg.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_MM_QUAD);
            /*
            reg.emplace<com_color_t>(ent_quad, 128);
            reg.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, pos2d_t{0,0}, sizes_t{0,0});
            */
            if constexpr (_FALSE)
            { /* testing text */
                auto ent_text = reg.create();
                reg.emplace<com_family_t>(ent_text, ent_quad);
                reg.emplace<com_visual_t>(ent_text, _TRUTH, 1);
                reg.emplace<com_relpos_t>(ent_text, RELPOS_MID, RELPOS_MAX/2);
                reg.emplace<com_anchor_t>(ent_text, RELPOS_MID, RELPOS_MAX);
                reg.emplace<com_coord_t>(ent_text, 0, 0, 2);
                reg.emplace<com_sizes_t>(ent_text, IMAGE_FONT_SIZES_X*5, IMAGE_FONT_SIZES_Y*2);
                reg.emplace<com_scale_t>(ent_text, 1, 1);
                reg.emplace<com_color_t>(ent_text, 240);
                reg.emplace<com_cstring_t>(ent_text, 11);
                snprintf(reg.get<com_cstring_t>(ent_text).mdata, 11, "Hello\nWorld");
            }
        }
        if constexpr (_TRUTH)
        { /* gui-mb */
            ent_t ent_quad = reg.create();
            reg.emplace<com_family_t>(ent_quad, ent_guis);
            reg.emplace<com_visual_t>(ent_quad, _TRUTH, 1);
            reg.emplace<com_relpos_t>(ent_quad, RELPOS_MID, RELPOS_MIN);
            reg.emplace<com_anchor_t>(ent_quad, RELPOS_MID, RELPOS_MIN);
            reg.emplace<com_coord_t>(ent_quad, 0, 0, 0);
            reg.emplace<com_sizes_t>(ent_quad, camera.sizes.w, GUIS_SIZES_Y);
            reg.emplace<com_scale_t>(ent_quad, 1, 1);
            reg.emplace<com_color_t>(ent_quad, 64);
            reg.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_MB_QUAD);
            reg.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, pos2d_t{0,0}, sizes_t{0,0});
            if constexpr (_TRUTH)
            { /* text-mb */
                auto ent_text = reg.create();
                reg.emplace<com_family_t>(ent_text, ent_quad);
                reg.emplace<com_visual_t>(ent_text, _TRUTH, 1);
                reg.emplace<com_relpos_t>(ent_text, RELPOS_MID, RELPOS_MID);
                reg.emplace<com_anchor_t>(ent_text, RELPOS_MID, RELPOS_MID);
                reg.emplace<com_coord_t>(ent_text, 0, 0, 1);
                reg.emplace<com_sizes_t>(ent_text, camera.sizes.w, GUIS_SIZES_Y);
                reg.emplace<com_scale_t>(ent_text, 1, 1);
                reg.emplace<com_color_t>(ent_text, 240);
                reg.emplace<com_ename_t>(ent_text, _ENAME_GUIS_MB_TEXT);
                reg.emplace<com_cstring_t>(ent_text, 128);
            }
            if constexpr (_FALSE)
            { /* logo-mb */
                ent_t ent_logo = reg.create();
                reg.emplace<com_family_t>(ent_logo, ent_quad);
                reg.emplace<com_visual_t>(ent_logo, _TRUTH, 0);
                reg.emplace<com_relpos_t>(ent_logo, RELPOS_MAX, RELPOS_MID);
                reg.emplace<com_anchor_t>(ent_logo, RELPOS_MIN, RELPOS_MID);
                reg.emplace<com_coord_t>(ent_logo, 0, 0, 3);
                reg.emplace<com_sizes_t>(ent_logo, UNIT_SCALE_X, UNIT_SCALE_Y);
                reg.emplace<com_scale_t>(ent_logo, 1, 1);
                reg.emplace<com_color_t>(ent_logo, 255);
                reg.emplace<com_image_t>(ent_logo, _IMAGE_META_LOGO, pos2d_t{0,0}, sizes_t{0,0});
            }
        }
        if constexpr (_TRUTH)
        { /* gui-mt */
            ent_t ent_quad = reg.create();
            reg.emplace<com_family_t>(ent_quad, ent_guis);
            reg.emplace<com_visual_t>(ent_quad, _TRUTH, 1);
            reg.emplace<com_relpos_t>(ent_quad, RELPOS_MID, RELPOS_MAX);
            reg.emplace<com_anchor_t>(ent_quad, RELPOS_MID, RELPOS_MAX);
            reg.emplace<com_coord_t>(ent_quad, 0, 0, 0);
            reg.emplace<com_color_t>(ent_quad, 64);
            reg.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, pos2d_t{0,0}, sizes_t{0,0});
            reg.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_MT_QUAD);
            reg.emplace<com_sizes_t>(ent_quad, camera.sizes.w, GUIS_SIZES_Y);
            reg.emplace<com_scale_t>(ent_quad, 1, 1);
            if constexpr (_TRUTH)
            { /* text-mt */
                auto ent_text = reg.create();
                reg.emplace<com_family_t>(ent_text, ent_quad);
                reg.emplace<com_visual_t>(ent_text, _TRUTH, 1);
                reg.emplace<com_relpos_t>(ent_text, RELPOS_MID, RELPOS_MID);
                reg.emplace<com_anchor_t>(ent_text, RELPOS_MID, RELPOS_MID);
                reg.emplace<com_coord_t>(ent_text, 0, 0, 1);
                reg.emplace<com_sizes_t>(ent_text, camera.sizes.w, GUIS_SIZES_Y);
                reg.emplace<com_scale_t>(ent_text, 1, 1);
                reg.emplace<com_color_t>(ent_text, 240);
                reg.emplace<com_ename_t>(ent_text, _ENAME_GUIS_MT_TEXT);
                reg.emplace<com_cstring_t>(ent_text, 128);
            }
        }
        if constexpr (_TRUTH)
        { /* gui-lm */
            ent_t ent_quad = reg.create();
            reg.emplace<com_family_t>(ent_quad, ent_guis);
            reg.emplace<com_visual_t>(ent_quad, _TRUTH, 1);
            reg.emplace<com_relpos_t>(ent_quad, RELPOS_MIN, RELPOS_MID);
            reg.emplace<com_anchor_t>(ent_quad, RELPOS_MIN, RELPOS_MID);
            reg.emplace<com_coord_t>(ent_quad, 0, 0, 0);
            reg.emplace<com_sizes_t>(ent_quad, GUIS_SIZES_X, camera.sizes.h - GUIS_SIZES_X*2);
            reg.emplace<com_scale_t>(ent_quad, 1, 1);
            reg.emplace<com_color_t>(ent_quad, 64);
            reg.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_LM_QUAD);
            reg.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, pos2d_t{0,0}, sizes_t{0,0});
        }
        if constexpr (_TRUTH)
        { /* gui-rm */
            ent_t ent_quad = reg.create();
            reg.emplace<com_family_t>(ent_quad, ent_guis);
            reg.emplace<com_visual_t>(ent_quad, _TRUTH, 1);
            reg.emplace<com_relpos_t>(ent_quad, RELPOS_MAX, RELPOS_MID);
            reg.emplace<com_anchor_t>(ent_quad, RELPOS_MAX, RELPOS_MID);
            reg.emplace<com_coord_t>(ent_quad, 0, 0, 0);
            reg.emplace<com_sizes_t>(ent_quad, GUIS_SIZES_X, camera.sizes.h - GUIS_SIZES_Y*2);
            reg.emplace<com_scale_t>(ent_quad, 1, 1);
            reg.emplace<com_color_t>(ent_quad, 64);
            reg.emplace<com_image_t>(ent_quad, _IMAGE_META_NONE, pos2d_t{0,0}, sizes_t{0,0});
            reg.emplace<com_ename_t>(ent_quad, _ENAME_GUIS_RM_QUAD);
        }
    }
}

/*** drawing ***/

static void draw_init()
{
}

extern void proc_list(drawlist_t&drawlist);
void draw_unit(drawable_t&drawable)
{
    constexpr auto&reg = ecos.reg;
    auto&entity = drawable.entity;
    auto&heritage = drawable.heritage;
    /* props */
    auto coord_e = drawable.coord;
    auto&coord_a = heritage.coord;
    if (reg.try_get<com_tile_t>(entity))
    { coord_e.y += UNIT_SCALE_Y * (coord_e.z - camera.coord.z); }
    auto color_e = color_t{ .value = 255u };
    if (auto*temp = reg.try_get<com_color_t>(entity)) { color_e =*temp; }
    auto&sizes_a = heritage.sizes;
    auto sizes_e = sizes_t{ .w = UNIT_SCALE_X, .h = UNIT_SCALE_Y };
    if (auto*temp = reg.try_get<com_sizes_t>(entity)) { sizes_e =*temp; }
    auto&scale_a = heritage.scale;
    auto scale_e = scale_t{ .x = 1, .y = 1 };
    if (auto*temp = reg.try_get<com_scale_t>(entity)) { scale_e =*temp; }
    scale_e.x *= scale_a.x;
    scale_e.y *= scale_a.y;
    auto anchor_e = anchor_t{ RELPOS_MID, RELPOS_MID };
    if (auto*temp = reg.try_get<com_anchor_t>(entity)) { anchor_e =*temp; }
    auto relpos_e = relpos_t{ RELPOS_MID, RELPOS_MID };
    if (auto*temp = reg.try_get<com_relpos_t>(entity)) { relpos_e =*temp; }
    /* direc */
    auto coord_u = coord_e;
    if constexpr (_FALSE)
    {
        auto direc_c = camera.direc;
        while (direc_c.y != +1)
        {
            direc_c = get_vec_turn(direc_c);
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
    constexpr auto&ortho = camera.ortho; 
    if ((_FALSE
    ) || (_TRUTH
    && ((coord_sl <= ortho.sr) && (coord_sb <= ortho.st))
    && ((coord_sl >= ortho.sl) && (coord_sb >= ortho.sb))
    ) || (_TRUTH
    && ((coord_sl <= ortho.sr) && (coord_st <= ortho.st))
    && ((coord_sl >= ortho.sl) && (coord_st >= ortho.sb))
    ) || (_TRUTH
    && ((coord_sr <= ortho.sr) && (coord_sb <= ortho.st))
    && ((coord_sr >= ortho.sl) && (coord_sb >= ortho.sb))
    ) || (_TRUTH
    && ((coord_sr <= ortho.sr) && (coord_st <= ortho.st))
    && ((coord_sr >= ortho.sl) && (coord_st >= ortho.sb))
    ) || (_TRUTH
    && ((coord_sl <= ortho.sl) && (coord_sr >= ortho.sr))
    && ((coord_sb >= ortho.sb) || (coord_st >= ortho.st))
    ) || (_TRUTH
    && ((coord_sb <= ortho.sb) && (coord_st >= ortho.st))
    && ((coord_sl >= ortho.sl) || (coord_sr <= ortho.sr))
    )) { /* draw */
        glColor3ub(color_e.value, color_e.value, color_e.value);
        if (auto*cstring = reg.try_get<com_cstring_t>(entity))
        {
            const auto&image = image_list[_IMAGE_FONT_MAIN];
            auto image_uw = static_cast<v1u_t>(image.sizes.w);
            auto image_uh = static_cast<v1u_t>(image.sizes.h);
            auto image_fw = static_cast<v1f_t>(image.sizes.w);
            auto image_fh = static_cast<v1f_t>(image.sizes.h);
            /* glyph is a single chracter from the image */
            auto glyph_count_uw = static_cast<v1u_t>(image_uw / IMAGE_FONT_SIZES_X);
            auto glyph_count_uh = static_cast<v1u_t>(image_uh / IMAGE_FONT_SIZES_Y);
            auto glyph_uw = static_cast<v1u_t>(image_uw / glyph_count_uw);
            auto glyph_uh = static_cast<v1u_t>(image_uh / glyph_count_uh);
            auto glyph_fw = static_cast<v1f_t>(IMAGE_FONT_SIZES_X) / image_fw;
            auto glyph_fh = static_cast<v1f_t>(IMAGE_FONT_SIZES_Y) / image_fh;
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
            auto label_uw = label_sr - label_sl;
            auto label_uh = label_st - label_sb;
            /* sizes */
            auto sizes_uw = label_uw / count_uw;
            auto sizes_uh = label_uh / count_uh;
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
                else if (mbyte == '\0') { mbyte = '!'; }
                mbyte = std::max(mdata[iter], ' ') - ' ';
                coord_sl += sizes_uw;
                coord_sr += sizes_uw;
                moved_sx += sizes_uw;
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
        auto*image = reg.try_get<com_image_t>(entity);
        if (auto*faces = reg.try_get<com_faces_t>(entity))
        {
            auto direc_e = direc_t{+0,-1};
            auto direc_c = camera.direc;
            if (auto*temp = reg.try_get<com_direc_t>(entity)) { direc_e =*temp; }
            auto eface = _EFACE_F;
            auto dotec = direc_c.x * direc_e.x + direc_c.y * direc_e.y;
            if (dotec == 0)
            {
                if (direc_c.x == 0)
                {
                    if (direc_c.y == direc_e.x) { eface = _EFACE_R; }
                    else { eface = _EFACE_L; }
                }
                else
                {
                    if (direc_c.x == direc_e.y) { eface = _EFACE_R; }
                    else { eface = _EFACE_L; }
                }
            }
            else if (dotec == -1) { eface = _EFACE_F; }
            else if (dotec == +1) { eface = _EFACE_B; }
            /*
            if (direc_c.x == +0 && direc_e.x == +0)
            {
                if (direc_c.y == direc_e.y) { eface = _EFACE_B; }
                else { eface = _EFACE_F; }
            }
            else if (direc_c.x == direc_e.x) { eface = _EFACE_B; }
            */
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
    }
    /* hierarchy */
    if (auto*family = reg.try_get<com_family_t>(drawable.entity))
    {
        drawlist_t drawlist;
        ecos_call_for_children(entity, [&](ent_t children){
            if (auto*visual = reg.try_get<com_visual_t>(children))
            {
                if (visual->valid)
                {
                    if (auto*coord = reg.try_get<com_coord_t>(children))
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
    constexpr auto&reg = ecos.reg;
    auto head = drawlist.begin();
    auto tail = drawlist.end();
    std::sort(head, tail, [](const drawable_t& dl, const drawable_t& dr) {
        constexpr auto&reg = ecos.reg;
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
    camera.sizes.w = CAMERA_SIZES_W;
    camera.sizes.h = CAMERA_SIZES_H;
    camera.ortho.sl = camera.coord.x - camera.sizes.w * camera.scale.x / 2;
    camera.ortho.sr = camera.coord.x + camera.sizes.w * camera.scale.x / 2;
    camera.ortho.sb = camera.coord.y - camera.sizes.h * camera.scale.y / 2;
    camera.ortho.st = camera.coord.y + camera.sizes.h * camera.scale.y / 2;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(camera.ortho.sl, camera.ortho.sr, camera.ortho.sb, camera.ortho.st);
    /* entity component system */
    constexpr auto&reg = ecos.reg;
    if constexpr(_TRUTH)
    { /* middle bot text */
        auto&cstring = reg.get<com_cstring_t>(ecos_get_by_ename(ename_t{_ENAME_GUIS_MB_TEXT}));
        memset(cstring.mdata, '\0', cstring.msize);
        std::snprintf(
            cstring.mdata, cstring.msize - 1,
            "mode=%s;%c%s(%+d)x(%d)",
            get_key_mode_name(),
            '\n',
            &get_key_line()[0], get_key_narg_sign(), get_key_narg()
        );
    }
    if constexpr(_TRUTH)
    { /* middle top text */
        auto&cstring = reg.get<com_cstring_t>(ecos_get_by_ename(ename_t{_ENAME_GUIS_MT_TEXT}));
        memset(cstring.mdata, '\0', cstring.msize);
        std::snprintf(
            cstring.mdata, cstring.msize - 1,
            "now=%03zu.%03zu;fps=%05zu;"
            "%c"
            "p(x%+04iy%+04i)s(x%+04iy%+04i)d(x%+1dy%+1d)"
            "%c",
            util.timer.now_mil / 1000, util.timer.now_mil % 1000, util.timer.fps_num,
            '\n',
            camera.coord.x, camera.coord.y,
            camera.scale.x, camera.scale.y,
            camera.direc.x, camera.direc.y,
            '\0'
        );
    }
    if constexpr (_TRUTH)
    { /* draw families from top to bottom */
        drawlist_t drawlist;
        for (auto&&[entity, visual, coord] : reg.view<com_visual_t, com_coord_t>().each())
        {
            auto ent = entity;
            if (visual.valid == _FALSE) { continue; }
            if (auto*family = reg.try_get<com_family_t>(entity))
            { if (reg.valid(family->ancestor)) { continue; } }
            auto heritage = heritage_t{
                .coord = coord_t{0,0,0},
                .sizes = sizes_t{0,0},
                .scale = scale_t{1,1},
            };
            if (auto*relpos = reg.try_get<com_relpos_t>(entity))
            {
                heritage.coord = camera.coord;
                heritage.sizes = camera.sizes;
                heritage.scale = camera.scale;
            }
            drawlist.push_back({entity,visual,coord,heritage});
        }
        proc_list(drawlist);
    }
    if constexpr (_TRUTH)
    { /* drawing the tile grid */
        glColor3ub(128, 128, 128);
        glBegin(GL_LINES);
        auto anchor_e = reg.get<com_anchor_t>(game.ent_hero);
        auto coord_e = reg.get<com_coord_t>(game.ent_hero);
        auto sizes_e = reg.get<com_sizes_t>(game.ent_hero);
        auto coord_a = get_anchor_coord(anchor_e, sizes_e);
        coord_e.x += get_anchor_coord(anchor_e.x, sizes_e.w, 1);
        coord_e.y += get_anchor_coord(anchor_e.y, sizes_e.h, 1);
        auto halfx = RATIO_X / 4;
        auto halfy = RATIO_Y / 4;
        halfx = std::min(halfx, halfy);
        halfy = std::min(halfy, halfx);
        auto stepx = coord_e.x / UNIT_SCALE_X;
        auto stepy = coord_e.y / UNIT_SCALE_Y;
        for (auto ix = -halfx; ix <= +halfx; ix++)
        {
            auto fromx = (stepx + ix) * UNIT_SCALE_X - coord_a.x;
            glVertex2i(fromx, (stepy - halfy) * UNIT_SCALE_Y - coord_a.y);
            glVertex2i(fromx, (stepy + halfy) * UNIT_SCALE_Y - coord_a.y);
        }
        for (auto iy = -halfy; iy <= +halfy; iy++)
        {
            auto fromy = (stepy + iy) * UNIT_SCALE_Y - coord_a.y;
            glVertex2i((stepx - halfx) * UNIT_SCALE_X, fromy);
            glVertex2i((stepx + halfx) * UNIT_SCALE_X, fromy);
        }
        glEnd();
    }
    if constexpr (_TRUTH)
    { /* finish the frame */
        glutPostRedisplay();
    }
    if constexpr (_TRUTH)
    { /* camera-to-hero adjustment */
        auto&&anchor = reg.get<com_anchor_t>(game.ent_hero);
        auto&&coord = reg.get<com_coord_t>(game.ent_hero);
        auto&&force = reg.get<com_force_t>(game.ent_hero);
        auto&&sizes = reg.get<com_sizes_t>(game.ent_hero);
        auto&&scale = reg.get<com_scale_t>(game.ent_hero);
        camera.coord = coord;
        camera.coord.x += get_anchor_coord(anchor.x, sizes.w, scale.x);
        camera.coord.y += get_anchor_coord(anchor.y, sizes.h, scale.y);
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
    camera.coord.x = (coord.x + UNIT_COUNT_X/2) % UNIT_COUNT_X;
    camera.coord.y = (coord.y + UNIT_COUNT_Y/2) % UNIT_COUNT_Y;
    camera.coord.z = (coord.z + UNIT_COUNT_Z/2) % UNIT_COUNT_Z;
}

void view_move(const coord_t&coord)
{
    view_goto({
        .x = coord.x * (UNIT_SCALE_X * camera.scale.x),
        .y = coord.y * (UNIT_SCALE_Y * camera.scale.y),
        .z = coord.z,
    });
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

void view_turn(bool_t lside)
{
    camera.direc = get_vec_turn(camera.direc, lside);
}

_NAMESPACE_LEAVE
