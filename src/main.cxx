/* headers*/

#include "head.hxx"

#include "main.hxx"

/* content */

_NAMESPACE_ENTER

/** typedef **/

/*** graphics ***/

struct com_gfix_sprite_t
{
    struct { float x = 0.0, y = 0.0; } sizes;
    struct { float r = 0.0, g = 0.0, b = 0.0, a = 0.0; } color;
};

/*** physics ***/

struct com_fsix_move_t
{
    struct { float x = 0.0, y = 0.0; } position;
    struct { float x = 0.0, y = 0.0; } velocity;
};

/*** keyboard ***/

typedef void(*key_func_t)();

typedef int key_code_t;

struct key_bind_t;
using key_list_t = std::vector<key_bind_t*>;
struct key_bind_t
{
    key_func_t func;
    key_list_t list;
};

/** datadef **/

struct window_t
{
    struct { size_t x = 0, y = 0; } coord;
    struct { size_t w = 256, h = 256; } sizes;
} window;

struct camera_t
{
    float scale = 1.0;
    struct { float x = 0.0, y = 0.0; } coord;
    struct { float w = 1.0, h = 1.0; } sizes;
} camera;

struct timer_t
{
    float delta = 0.0;
    float msnow = 0.1;
    float mswas = 0.0;
} timer;

struct gfix_t
{
} gfix;

struct fsix_t
{
} fsix;

key_list_t key_list;

std::string key_line = "";
key_bind_t* key_bind_used = _NULL;

/** actions **/

/*** application ***/

int main(int argc, char** argv)
{
    std::clog << "hello world !" << std::endl;
    /* framework */
    std::clog << "init graphics library utility" << std::endl;
    glutInit(&argc, argv);
    /* window */
    std::clog << "init window" << std::endl;
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(window.coord.x, window.coord.y);
    glutInitWindowSize(window.sizes.w, window.sizes.h);
    glutCreateWindow(_NAME_STR);
    /* gfix */
    glViewport(0, 0, window.sizes.w, window.sizes.h);
    /** gray bg color **/
    glClearColor(0.25, 0.25, 0.25, 1.0);
    /** white drawing **/
    glColor3d(1.0, 1.0, 1.0);
    /** vertex width **/
    glPointSize(1.0);
    /** camera setup **/
    /* keyboard */
    auto key_count = 2 << (sizeof(char) * 8);
    key_list = key_list_t(key_count);
    /** menu **/
    key_list['m'] = new key_bind_t;
    key_list['m']->list = key_list_t(key_count);
    key_list['m']->list['q'] = new key_bind_t;
    key_list['m']->list['q']->func = [](){
        exit(_ERROR_NONE);
    };
    key_list['m']->list['r'] = new key_bind_t;
    key_list['m']->list['r']->func = [](){
        camera = camera_t();
    };
    /** view **/
    key_list['v'] = new key_bind_t;
    key_list['v']->list = key_list_t(key_count);
    key_list['v']->list['a'] = new key_bind_t;
    key_list['v']->list['a']->func = [](){
        camera.coord.x -= camera.scale;
    };
    key_list['v']->list['d'] = new key_bind_t;
    key_list['v']->list['d']->func = [](){
        camera.coord.x += camera.scale;
    };
    key_list['v']->list['s'] = new key_bind_t;
    key_list['v']->list['s']->func = [](){
        camera.coord.y -= camera.scale;
    };
    key_list['v']->list['w'] = new key_bind_t;
    key_list['v']->list['w']->func = [](){
        camera.coord.y += camera.scale;
    };
    key_list['v']->list['z'] = new key_bind_t;
    key_list['v']->list['z']->list = key_list_t(key_count);
    key_list['v']->list['z']->list['i'] = new key_bind_t;
    key_list['v']->list['z']->list['i']->func = [](){
        camera.scale /= 2.0;
    };
    key_list['v']->list['z']->list['o'] = new key_bind_t;
    key_list['v']->list['z']->list['o']->func = [](){
        camera.scale *= 2.0;
    };
    /* camera */
    glutKeyboardFunc(proc_key_board);
    glutSpecialFunc(proc_key_board_special);
    /* oput */
    glutDisplayFunc(draw);
    glutReshapeFunc(proc_resize);
    /* work */
    glutIdleFunc(work);
    glutMainLoop();
    /* final */
    std::clog << "good day, sir !" << std::endl;
    return _ERROR_NONE;
}

void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor4d(0.5, 0.5, 0.5, 1.0);
    glVertex2d(+1.0, +0.0);
    glVertex2d(+0.0, +1.0);
    glVertex2d(-1.0, -0.0);
    glVertex2d(-0.0, -1.0);
    glEnd();
    /* bottom bar */
    auto xl = -camera.sizes.w/2.0;
    auto xr = +camera.sizes.w/2.0;
    auto yb = -camera.sizes.h/2.0;
    glBegin(GL_QUADS);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    glVertex2d(xl, yb);
    glVertex2d(xl, yb + camera.scale * 0.1);
    glVertex2d(xr, yb + camera.scale * 0.1);
    glVertex2d(xr, yb);
    glEnd();
    glColor4d(1.0, 1.0, 1.0, 1.0);
    glRasterPos2d(xl + camera.scale * 0.025, yb + camera.scale * 0.025);
    for (auto&key:key_line)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, key);
    }
    glutSwapBuffers();
}

void work(void)
{
    /* time */
    timer.delta = timer.mswas - timer.msnow;
    timer.mswas = timer.msnow;
    timer.msnow = glutGet(GLUT_ELAPSED_TIME);
    /* view */
    auto ratio = static_cast<float>(window.sizes.w) / static_cast<float>(window.sizes.h);
    camera.sizes.w = camera.scale * ratio;
    camera.sizes.h = camera.scale;
    glViewport(0, 0, window.sizes.w, window.sizes.h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(
        camera.coord.x - camera.sizes.w / 2.0,
        camera.coord.x + camera.sizes.w / 2.0,
        camera.coord.y - camera.sizes.h / 2.0,
        camera.coord.y + camera.sizes.h / 2.0
    );
    /* draw */
    glutPostRedisplay();
}

/*** callbacks ***/

void proc_resize(int sizew, int sizeh)
{
    window.sizes.w = sizew;
    window.sizes.h = sizeh;
}
void proc_key_board(unsigned char key, int _mouse_x, int _mouse_y)
{
    key_line_insert(key);
}

void proc_key_board_special(int key, int _mouse_x, int _mouse_y)
{
}

/*** keyboard ***/

void key_line_reset()
{
    key_bind_used = _NULL;
}

void key_line_apply()
{
    if (key_bind_used->func)
    {
        key_bind_used->func();
    }
    key_line_reset();
}

void key_line_insert(unsigned char key)
{
    key_bind_t* bind;
    if (key_bind_used)
    {
        bind = key_bind_used->list[key];
    }
    else
    {
        key_line = "";
        bind = key_list[key];
    }
    key_line += key;
    if (bind)
    {
        key_bind_used = bind;
        if (bind->list.empty())
        {
            key_line_apply();
        }
        else
        {
        }
    }
    else
    {
        key_line += ": is not a valid keybind";
        key_line_reset();
    }
}

_NAMESPACE_LEAVE

#ifdef _TYPE_RUN

int main(int argc, char** argv)
{
    return _NAMESPACE::main(argc, argv);
}

#endif
