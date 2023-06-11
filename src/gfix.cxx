/* headers */

#include "gfix.hxx"
#include "main.hxx"
#include "iput.hxx"

#include "../lib/stbl/src/stbi.hxx"

/* content */

_NAMESPACE_ENTER

/** datadef **/

gfix_t gfix;

/*** meta ***/

struct sort_by_layer_t {
    bool operator() (const ent_t& lent, const ent_t& rent)
    {
        auto lent_layer = ecos.reg.get<com_layer_t>(lent);
        auto rent_layer = ecos.reg.get<com_layer_t>(rent);
        return lent_layer.index < rent_layer.index;
    }
} sort_by_layer;

/** actions **/

void gfix_init()
{
    std::clog << "[opengl-version] " << glGetString(GL_VERSION) << std::endl;
    /* settings */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_TEXTURE_1D);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_3D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);
    /* images */
    stbi_set_flip_vertically_on_load(true);
    load_image_index("rsc/gfix/meta-logo.png", 1);
    load_image_index("rsc/gfix/entt-main-ff-ai-f1.png", 2);
    /* keybinds */
    /** view **/
    /*** goto ***/
    key_bind_set("vgx", [](int narg){ view_goto({ narg, gfix.camera.coord.y }); });
    key_bind_set("vgy", [](int narg){ view_goto({ gfix.camera.coord.x, narg }); });
    /*** move ***/
    key_bind_set("va", [](int narg){ view_move({ .x = (-1) * (1+narg), .y = 0 }); });
    key_bind_set("vd", [](int narg){ view_move({ .x = (+1) * (1+narg), .y = 0 }); });
    key_bind_set("vs", [](int narg){ view_move({ .x = 0, .y = (-1) * (1+narg) }); });
    key_bind_set("vw", [](int narg){ view_move({ .x = 0, .y = (+1) * (1+narg) }); });
    /*** zoom ***/
    key_bind_set("vz", [](int narg){ view_zoom({ .x = narg, .y = narg }); });
    /** graphics **/
    /*** mode ***/
    /**** wire ****/
    key_bind_set("gmw", [](int narg){ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); });
    /**** fill ****/
    key_bind_set("gmf", [](int narg){ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); });
    /* entity component system */
    { /* main entity */
        ent_t ent = ecos.reg.create();
        ecos.reg.emplace<com_sname_t>(ent, "entt-main");
        ecos.reg.emplace<com_layer_t>(ent, _LAYER_MG + 0);
        ecos.reg.emplace<com_coord_t>(ent, 0, 0);
        ecos.reg.emplace<com_sizes_t>(ent, 256, 256);
        ecos.reg.emplace<com_scale_t>(ent, 1, 1);
        ecos.reg.emplace<com_color_t>(ent, 255, 255, 255);
        ecos.reg.emplace<com_image_t>(ent, 2);
    }
    { /* gui-status-quad */
        ent_t ent = ecos.reg.create();
        ecos.reg.emplace<com_sname_t>(ent, "gui-status-quad");
        ecos.reg.emplace<com_layer_t>(ent, _LAYER_UI + 0);
        ecos.reg.emplace<com_coord_t>(ent, 0, 0);
        ecos.reg.emplace<com_sizes_t>(ent, gfix.camera.sizes.w, 32);
        ecos.reg.emplace<com_scale_t>(ent, 1, 1);
        ecos.reg.emplace<com_color_t>(ent, 64, 64, 64);
        ecos.reg.emplace<com_screen_from_t>(ent, _SCREEN_FROM_LB);
    }
    { /* gui-status-text */
        auto ent = ecos.reg.create();
        ecos.reg.emplace<com_sname_t>(ent, "gui-status-text");
        ecos.reg.emplace<com_layer_t>(ent, _LAYER_UI + 1);
        ecos.reg.emplace<com_string_t>(ent, new char[256], 256);
        ecos.reg.emplace<com_coord_t>(ent, 12, 12);
        ecos.reg.emplace<com_sizes_t>(ent, 16, 16);
        ecos.reg.emplace<com_scale_t>(ent, 1, 1);
        ecos.reg.emplace<com_color_t>(ent, 240, 240, 240);
        ecos.reg.emplace<com_screen_from_t>(ent, _SCREEN_FROM_LB);
    }
    { /* logo */
        ent_t ent = ecos.reg.create();
        ecos.reg.emplace<com_sname_t>(ent, "gui-status-logo");
        ecos.reg.emplace<com_layer_t>(ent, _LAYER_UI + 2);
        ecos.reg.emplace<com_coord_t>(ent, 0, 0);
        ecos.reg.emplace<com_sizes_t>(ent, 32, 32);
        ecos.reg.emplace<com_scale_t>(ent, 1, 1);
        ecos.reg.emplace<com_color_t>(ent, 255, 255, 255);
        ecos.reg.emplace<com_image_t>(ent, 1);
        ecos.reg.emplace<com_screen_from_t>(ent, _SCREEN_FROM_RB);
    }
}

/*** drawing ***/

static void draw_init()
{
    /* screen */
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, gfix.window.sizes.w, gfix.window.sizes.h);
    /* camera */
    gfix.camera.sizes.w = gfix.window.sizes.w;
    gfix.camera.sizes.h = gfix.window.sizes.h;
    auto xl = gfix.camera.coord.x - gfix.camera.sizes.w * gfix.camera.scale.x / 2;
    auto xr = gfix.camera.coord.x + gfix.camera.sizes.w * gfix.camera.scale.x / 2;
    auto yb = gfix.camera.coord.y - gfix.camera.sizes.h * gfix.camera.scale.y / 2;
    auto yt = gfix.camera.coord.y + gfix.camera.sizes.h * gfix.camera.scale.y / 2;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xl, xr, yb, yt);
}

static void draw_proc()
{
    /* entity component system */
    { /* change unique ones */
        auto view = ecos.reg.view<com_sname_t>();
        for (auto [ent, sname] : view.each())
        {
            if (sname.title == "gui-status-quad")
            {
                auto&sizes = ecos.reg.get<com_sizes_t>(ent);
                sizes.w = gfix.camera.sizes.w;
            }
            else if (sname.title == "gui-status-text")
            {
                auto&string = ecos.reg.get<com_string_t>(ent);
                memset(string.mdata, '\0', 256);
                std::sprintf(
                    string.mdata,
                    "[ now=%03zu.%03zu; fps=%05zu; pos=%ix%i; scale=%ix%i; ] %s(%+dx%d)",
                    timer.now_mil / 1000, timer.now_mil % 1000, timer.fps_num,
                    gfix.camera.coord.x, gfix.camera.coord.y,
                    gfix.camera.scale.x, gfix.camera.scale.y,
                    &key_line[0], key_narg_sign, key_narg
                );
            }
        }
    }
    std::vector<ent_t> draw_list;
    {
        auto view = ecos.reg.view<com_layer_t>();
        for (auto [ent, layer] : view.each()) { draw_list.push_back(ent); }
        std::sort(draw_list.begin(), draw_list.end(), sort_by_layer);
    }
    {
        for (auto ent : draw_list)
        {
            if (ecos.reg.all_of<com_coord_t, com_sizes_t, com_scale_t, com_color_t>(ent))
            {
                /* props */
                auto coord = ecos.reg.get<com_coord_t>(ent);
                auto sizes = ecos.reg.get<com_sizes_t>(ent);
                auto scale = ecos.reg.get<com_scale_t>(ent);
                auto color = ecos.reg.get<com_color_t>(ent);
                /* image */
                index_t image_index = _ZERO;
                if (ecos.reg.any_of<com_image_t>(ent))
                {
                    image_index = ecos.reg.get<com_image_t>(ent).index;
                }
                if (image_index > 0) { glBindTexture(GL_TEXTURE_2D, image_index); }
                /* screen or global ? */
                if (ecos.reg.any_of<com_screen_from_t>(ent))
                {
                    auto from = ecos.reg.get<com_screen_from_t>(ent).from;
                    /* text or quad ? */
                    if (ecos.reg.any_of<com_string_t>(ent))
                    {
                        auto string = ecos.reg.get<com_string_t>(ent);
                        auto text = text_t{ string, coord, sizes, scale, color };
                        draw_screen_text(text, from);
                    }
                    else
                    {
                        auto quad = quad_t{ coord, sizes, scale, color };
                        draw_screen_quad(quad, from);
                    } /* text or quad */
                }
                else
                {
                    /* text or quad ? */
                    if (ecos.reg.any_of<com_string_t>(ent))
                    {
                        auto string = ecos.reg.get<com_string_t>(ent);
                        auto text = text_t{ string, coord, sizes, scale, color };
                        draw_global_text(text);
                    }
                    else
                    {
                        auto quad = quad_t{ coord, sizes, scale, color };
                        draw_global_quad(quad);
                    } /* text or quad */
                } /* screen or global */
                if (image_index) { glBindTexture(GL_TEXTURE_2D, _ZERO); }
            }
        }
    }
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

void draw_global_quad(const quad_t&quad)
{
    auto xl = quad.coord.x - quad.sizes.w * quad.scale.x / 2;
    auto xr = quad.coord.x + quad.sizes.w * quad.scale.x / 2;
    auto yb = quad.coord.y - quad.sizes.h * quad.scale.y / 2;
    auto yt = quad.coord.y + quad.sizes.h * quad.scale.y / 2;
    glBegin(GL_QUADS);
    glColor3ubv(&quad.color.r);
    glTexCoord2i(0, 0);
    glVertex2i(xl, yb);
    glTexCoord2i(1, 0);
    glVertex2i(xr, yb);
    glTexCoord2i(1, 1);
    glVertex2i(xr, yt);
    glTexCoord2i(0, 1);
    glVertex2i(xl, yt);
    glEnd();
}

void draw_global_text(const text_t&text)
{
    glColor3ubv(&text.color.r);
    auto font = GLUT_BITMAP_HELVETICA_12;
    int xl = text.coord.x;
    int yb = text.coord.y;
    int xs = text.sizes.w * text.scale.x;
    int ys = text.sizes.h * text.scale.y;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(xl, yb, 0);
    glScalef(0.005*xs, 0.005*ys, 1);
    glLineWidth(text.scale.x);
    for (auto iter = 0; iter < text.string.msize; iter++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, text.string.mdata[iter]);
    }
    glPopMatrix();
}

void draw_screen_quad(const quad_t&quad, screen_from_e from)
{
    int xl = quad.coord.x;
    int xr = quad.coord.x;
    int yb = quad.coord.y;
    int yt = quad.coord.y;
    switch (from)
    {
        case _SCREEN_FROM_C: {
            xl -= quad.sizes.w * quad.scale.x / 2;
            xr += quad.sizes.w * quad.scale.x / 2;
            yb -= quad.sizes.h * quad.scale.y / 2;
            yt += quad.sizes.h * quad.scale.y / 2;
        } break;
        case _SCREEN_FROM_L: {
        } break;
        case _SCREEN_FROM_R: {
        } break;
        case _SCREEN_FROM_B: {
        } break;
        case _SCREEN_FROM_T: {
        } break;
        case _SCREEN_FROM_LB: {
            xl -= gfix.camera.sizes.w / 2;
            xr += quad.sizes.w * quad.scale.x;
            xr -= gfix.camera.sizes.w / 2;
            yb -= gfix.camera.sizes.h / 2;
            yt += quad.sizes.h * quad.scale.y;
            yt -= gfix.camera.sizes.h / 2;
        } break;
        case _SCREEN_FROM_RB: {
            xl -= quad.sizes.w * quad.scale.x;
            xl += gfix.camera.sizes.w / 2;
            xr += gfix.camera.sizes.w / 2;
            yb -= gfix.camera.sizes.h / 2;
            yt += quad.sizes.h * quad.scale.y;
            yt -= gfix.camera.sizes.h / 2;
        } break;
        case _SCREEN_FROM_LT: {
        } break;
        case _SCREEN_FROM_RT: {
        } break;
    }
    xl *= gfix.camera.scale.x;
    xl += gfix.camera.coord.x;
    xr *= gfix.camera.scale.x;
    xr += gfix.camera.coord.x;
    yb *= gfix.camera.scale.y;
    yb += gfix.camera.coord.y;
    yt *= gfix.camera.scale.y;
    yt += gfix.camera.coord.y;
    glBegin(GL_QUADS);
    glColor3ubv(&quad.color.r);
    glTexCoord2i(0, 0);
    glVertex2i(xl, yb);
    glTexCoord2i(1, 0);
    glVertex2i(xr, yb);
    glTexCoord2i(1, 1);
    glVertex2i(xr, yt);
    glTexCoord2i(0, 1);
    glVertex2i(xl, yt);
    glEnd();
}

void draw_screen_text(const text_t&text, screen_from_e from)
{
    glColor3ubv(&text.color.r);
    auto font = GLUT_BITMAP_HELVETICA_12;
    int xl = text.coord.x;
    int yb = text.coord.y;
    int xs = text.sizes.w * text.scale.x;
    int ys = text.sizes.h * text.scale.y;
    switch (from)
    {
        case _SCREEN_FROM_C: {
            xl *= gfix.camera.scale.x;
            xl -= glutBitmapLength(font, (unsigned char*)text.string.mdata)/2;
            yb *= gfix.camera.scale.y;
        } break;
        case _SCREEN_FROM_L: {
        } break;
        case _SCREEN_FROM_R: {
        } break;
        case _SCREEN_FROM_B: {
        } break;
        case _SCREEN_FROM_T: {
        } break;
        case _SCREEN_FROM_LB: {
            xl -= gfix.camera.sizes.w/2;
            xl *= gfix.camera.scale.x;
            yb -= gfix.camera.sizes.h/2;
            yb *= gfix.camera.scale.y;
        } break;
        case _SCREEN_FROM_RB: {
        } break;
        case _SCREEN_FROM_LT: {
        } break;
        case _SCREEN_FROM_RT: {
        } break;
    }
    xl += gfix.camera.coord.x;
    yb += gfix.camera.coord.y;
    xs *= gfix.camera.scale.x;
    ys *= gfix.camera.scale.y;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(xl, yb, 0);
    glScalef(0.005*xs, 0.005*ys, 1);
    glLineWidth(text.scale.x);
    for (auto iter = 0; iter < text.string.msize; iter++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, text.string.mdata[iter]);
    }
    glPopMatrix();
}

/*** files ***/

void load_image(const std::string&path, image_t*image)
{
    image->pixel_data.mdata = stbi_load(&path[0], &image->sizes.w, &image->sizes.h, &image->pixel_size.value, 0);
    if (image->pixel_data.mdata)
    {
        image->pixel_data.msize = image->sizes.w * image->sizes.h * image->pixel_size.value;
        glGenTextures(1, &image->index);
        glBindTexture(GL_TEXTURE_2D, image->index);
        glTexImage2D(
            GL_TEXTURE_2D, 0,
            image->pixel_size.value, image->sizes.w, image->sizes.h, 0,
            image->pixel_size.value == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
            image->pixel_data.mdata
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        std::clog << "--[[load_image]]--" << std::endl;
        std::clog << "[path]=" << path << std::endl;
        std::clog << "[sizes]=" << image->sizes.h << "x" << image->sizes.h << std::endl;
        std::clog << "[nchan]=" << image->pixel_size.value << std::endl;
        std::clog << "[msize]=" << image->pixel_data.msize << std::endl;
        std::clog << "[index]=" << image->index << std::endl;
    }
    else
    {
        std::cerr << "failed image loading: " << path << std::endl;
    }
}

void load_image_index(const std::string&path, size_t index)
{
    if (gfix.image_list.size() <= index)
    {
        gfix.image_list.resize(index + 1);
    }
    return load_image(path, &gfix.image_list[index]);
}

/*** view ***/

void view_goto(const com_coord_t&coord)
{
    gfix.camera.coord.x = coord.x;
    gfix.camera.coord.y = coord.y;
}

void view_move(const com_coord_t&coord)
{
    gfix.camera.coord.x += coord.x * (1 << 4 << gfix.camera.scale.x);
    gfix.camera.coord.y += coord.y * (1 << 4 << gfix.camera.scale.y);
}

void view_zoom(const com_scale_t&scale)
{
    if (scale.x < 0)
    {
        gfix.camera.scale.x >>= (-scale.x);
    }
    else
    {
        gfix.camera.scale.x <<= (+scale.x);
    }
    gfix.camera.scale.x = std::clamp(gfix.camera.scale.x, 1, 2 << 16);
    if (scale.y < 0)
    {
        gfix.camera.scale.y >>= (-scale.y);
    }
    else
    {
        gfix.camera.scale.y <<= (+scale.y);
    }
    gfix.camera.scale.y = std::clamp(gfix.camera.scale.y, 1, 2 << 16);
}

_NAMESPACE_LEAVE
