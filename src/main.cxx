/* headers*/

#include "head.hxx"

#include "main.hxx"

/* content */

_NAMESPACE_ENTER

/** datadef **/

typedef void(*key_func_t)();

typedef int key_code_t;

struct key_bind_t
{
    key_func_t func;
    std::vector<key_bind_t> list;
};

std::vector<key_bind_t> key_list;

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
    glutInitWindowSize(256, 256);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(_NAME_STR);
    /* gfix */
    /** gray bg color **/
    glClearColor(0.25, 0.25, 0.25, 1.0);
    /** white drawing **/
    glColor3d(1.0, 1.0, 1.0);
    /** vertex width **/
    glPointSize(1.0);
    /** camera setup **/
    /* keyboard */
    key_list = std::vector<key_bind_t>(std::pow(2, sizeof(char) * 8));
    key_list['q'] = {
        .func = [](){
            exit(_ERROR_NONE);
        },
        .list = {
        },
    };
    key_list['j'] = {
        .func = [](){
        },
        .list = {
        },
    };
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
    auto time_sec = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    auto window_sizew = glutGet(GLUT_WINDOW_WIDTH);
    auto window_sizeh = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, window_sizew, window_sizeh);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glVertex2d(+1.0, +0.0);
    glVertex2d(+0.0, +1.0);
    glVertex2d(-1.0, -0.0);
    glVertex2d(-0.0, -1.0);
    glEnd();
    glutSwapBuffers();
}

void work(void)
{
    glutPostRedisplay();
}

/*** window ***/

void proc_resize(int sizew, int sizeh)
{
    auto ratio = static_cast<float>(sizew) / static_cast<float>(sizeh);
    auto sidex = 1.0 * ratio;
    auto sidey = 1.0;
    glViewport(0, 0, sizew, sizeh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-sidex, +sidex, -sidey, +sidey);
}

/*** keyboard ***/

void key_line_reset()
{
    key_bind_used = _NULL;
    key_line = "";
}

void key_line_apply()
{
    key_bind_used->func();
    key_line_reset();
}

void key_line_insert(unsigned char key)
{
    key_bind_t* bind;
    if (key_bind_used)
    {
        bind = &key_bind_used->list[key];
    }
    else
    {
        bind = &key_list[key];
    }
    if (bind)
    {
        key_line += key;
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
        key_line_reset();
    }
}

void proc_key_board(unsigned char key, int _mouse_x, int _mouse_y)
{
    key_line_insert(key);
}

void proc_key_board_special(int key, int _mouse_x, int _mouse_y)
{
}

_NAMESPACE_LEAVE

#ifdef _TYPE_RUN

int main(int argc, char** argv)
{
    _NAMESPACE::main(argc, argv);
}

#endif
