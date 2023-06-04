/* headers */

#include "gfix.hxx"
#include "main.hxx"
#include "iput.hxx"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

/* content */

_NAMESPACE_ENTER

/** datadef **/

gfix_t gfix;

/** actions **/

void gfix_init()
{
    std::clog << "[opengl-version] " << glGetString(GL_VERSION) << std::endl;
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_TEXTURE_3D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_TEXTURE_1D);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    stbi_set_flip_vertically_on_load(true);
    load_image_index("rsc/logo16x16.png", 0);
}

static void draw_init()
{
    /* screen */
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, gfix.window.sizes.w, gfix.window.sizes.h);
    /* camera */
    gfix.camera.sizes.w = gfix.window.sizes.w;
    gfix.camera.sizes.h = gfix.window.sizes.h;
    auto xl = gfix.camera.coord.x - gfix.camera.sizes.w*gfix.camera.scale/2;
    auto xr = gfix.camera.coord.x + gfix.camera.sizes.w*gfix.camera.scale/2;
    auto yb = gfix.camera.coord.y - gfix.camera.sizes.h*gfix.camera.scale/2;
    auto yt = gfix.camera.coord.y + gfix.camera.sizes.h*gfix.camera.scale/2;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xl, xr, yb, yt);
}

static void draw_proc()
{
    glBindTexture(GL_TEXTURE_2D, gfix.image_list[0].index);
    draw_global_quad({
        .scale = 1,
        .coord = {
            .x = 256,
            .y = 0,
        },
        .sizes = {
            .w = 256,
            .h = 256,
        },
        .color = {
            .r = 255,
            .g = 255,
            .b = 255,
        },
    });
    glBindTexture(GL_TEXTURE_2D, 0);
    {
        glPointSize(10);
        glBegin(GL_POINTS);
        auto*image = &gfix.image_list[0];
        auto*idata = image->mdata;
        auto nchan = image->nchan;
        auto scale = ((1 << 8) >> 4);
        auto sizes_w = image->sizes.w;
        auto sizes_h = image->sizes.h;
        auto coord_x = -sizes_w/2;
        auto coord_y = -sizes_h/2;
        for (auto itery = 0; itery < sizes_h; itery++)
        {
            for (auto iterx = 0; iterx < sizes_w; iterx++)
            {
                glColor3ubv(&idata[((sizes_w * itery) + iterx) * nchan]);
                glVertex2i((coord_x + iterx) * scale, (coord_y + itery) * scale);
            }
        }
        glEnd();
    }
    /* gui */
    draw_screen_quad({
        .scale = 1,
        .coord = {
            .x = 0,
            .y = 0,
        },
        .sizes = {
            .w = gfix.camera.sizes.w,
            .h = 32,
        },
        .color = {
            .r = 16,
            .g = 16,
            .b = 16,
        },
    }, _SCREEN_FROM_LB);
    constexpr auto msize = 128;
    char mdata[msize];
    memset(mdata, '\0', 128);
    std::sprintf(
        mdata,
        "[ now=%03zu.%03zu; fps=%05zu ] %s",
        timer.now_mil/1000,
        timer.now_mil%1000,
        timer.fps_num,
        &key_line[0]
    );
    draw_screen_text({
        .mdata = mdata,
        .msize = msize,
        .scale = 1,
        .coord = {
            .x = 12,
            .y = 12,
        },
        .sizes = {
            .w = 16,
            .h = 16,
        },
        .color = {
            .r = 240,
            .g = 240,
            .b = 240,
        },
    }, _SCREEN_FROM_LB);
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

void draw_global_quad(quad_t quad)
{
    auto xl = quad.coord.x - quad.sizes.w*quad.scale/2;
    auto xr = quad.coord.x + quad.sizes.w*quad.scale/2;
    auto yb = quad.coord.y - quad.sizes.h*quad.scale/2;
    auto yt = quad.coord.y + quad.sizes.h*quad.scale/2;
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

void draw_screen_quad(quad_t quad, screen_from_e from)
{
    int xl = quad.coord.x;
    int xr = quad.coord.x;
    int yb = quad.coord.y;
    int yt = quad.coord.y;
    switch (from)
    {
        case _SCREEN_FROM_C: {
            xl -= quad.sizes.w*quad.scale/2;
            xr += quad.sizes.w*quad.scale/2;
            yb -= quad.sizes.h*quad.scale/2;
            yt += quad.sizes.h*quad.scale/2;
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
            xr += quad.sizes.w*quad.scale;
            xr -= gfix.camera.sizes.w / 2;
            yb -= gfix.camera.sizes.h / 2;
            yt += quad.sizes.h*quad.scale;
            yt -= gfix.camera.sizes.h / 2;
        } break;
        case _SCREEN_FROM_RB: {
        } break;
        case _SCREEN_FROM_LT: {
        } break;
        case _SCREEN_FROM_RT: {
        } break;
    }
    xl *= gfix.camera.scale;
    xl += gfix.camera.coord.x;
    xr *= gfix.camera.scale;
    xr += gfix.camera.coord.x;
    yb *= gfix.camera.scale;
    yb += gfix.camera.coord.y;
    yt *= gfix.camera.scale;
    yt += gfix.camera.coord.y;
    glBegin(GL_QUADS);
    glColor3ubv(&quad.color.r);
    glVertex2i(xl, yb);
    glVertex2i(xr, yb);
    glVertex2i(xr, yt);
    glVertex2i(xl, yt);
    glEnd();
}

void draw_screen_text(text_t text, screen_from_e from)
{
    glColor3ubv(&text.color.r);
    auto font = GLUT_BITMAP_HELVETICA_12;
    int xl = text.coord.x;
    int yb = text.coord.y;
    int xs = text.sizes.w * text.scale;
    int ys = text.sizes.h * text.scale;
    switch (from)
    {
        case _SCREEN_FROM_C: {
            xl *= gfix.camera.scale;
            xl -= glutBitmapLength(font, (unsigned char*)text.mdata)/2;
            yb *= gfix.camera.scale;
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
            xl *= gfix.camera.scale;
            yb -= gfix.camera.sizes.h/2;
            yb *= gfix.camera.scale;
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
    xs *= gfix.camera.scale;
    ys *= gfix.camera.scale;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(xl, yb, 0);
    glScalef(0.005*xs, 0.005*ys, 1);
    glLineWidth(text.scale);
    for (auto iter = 0; iter < text.msize; iter++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, text.mdata[iter]);
    }
    glPopMatrix();
}

void load_image(std::string path, image_t*image)
{
    image->mdata = stbi_load(&path[0], &image->sizes.w, &image->sizes.h, &image->nchan, 0);
    if (image->mdata)
    {
        image->msize = image->sizes.w * image->sizes.h * image->nchan;
        glGenTextures(1, &image->index);
        glBindTexture(GL_TEXTURE_2D, image->index);
        glTexImage2D(
            GL_TEXTURE_2D, 0,
            image->nchan, image->sizes.w, image->sizes.h, 0,
            image->nchan == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
            image->mdata
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        std::clog << "--[[load_image]]--" << std::endl;
        std::clog << "[path]=" << path << std::endl;
        std::clog << "[sizes]=" << image->sizes.h << "x" << image->sizes.h << std::endl;
        std::clog << "[nchan]=" << image->nchan << std::endl;
        std::clog << "[msize]=" << image->msize << std::endl;
        std::clog << "[index]=" << image->index << std::endl;
    }
    else
    {
        std::cerr << "failed image loading: " << path << std::endl;
    }
}
void load_image_index(std::string path, size_t index)
{
    if (gfix.image_list.size() <= index)
    {
        gfix.image_list.resize(index + 1);
    }
    return load_image(path, &gfix.image_list[index]);
}

void view_move(int stepx, int stepy)
{
    gfix.camera.coord.x += stepx * (1 << 4 << gfix.camera.scale);
    gfix.camera.coord.y += stepy * (1 << 4 << gfix.camera.scale);
}

void view_scale(int scale)
{
    if (scale < 0)
    {
        gfix.camera.scale >>= (-scale);
    }
    else
    {
        gfix.camera.scale <<= (+scale);
    }
    gfix.camera.scale = std::clamp(gfix.camera.scale, 1, 2 << 16);
}

_NAMESPACE_LEAVE
