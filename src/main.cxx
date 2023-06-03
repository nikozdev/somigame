/* headers*/

#include "main.hxx"

/* content */

_NAMESPACE_ENTER

/** datadef **/

timer_t timer;

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
    glutInitWindowPosition(gfix.window.coord.x, gfix.window.coord.y);
    glutInitWindowSize(gfix.window.sizes.w, gfix.window.sizes.h);
    glutCreateWindow(_NAME_STR);
    /* gfix */
    glViewport(0, 0, gfix.window.sizes.w, gfix.window.sizes.h);
    /** gray bg color **/
    glClearColor(0.25, 0.25, 0.25, 1.0);
    /** white drawing **/
    glColor3f(1.0, 1.0, 1.0);
    /** vertex width **/
    glPointSize(1.0);
    /* keyboard */
    key_board_init();
    /** menu **/
    key_bind_set("mq", [](){
        std::clog << "mq" << std::endl;
        exit(_ERROR_NONE);
    });
    key_bind_set("mr", [](){
        gfix.camera = camera_t();
    });
    /** view **/
    key_bind_set("vma", [](){
        //gfix.camera.coord.x -= gfix.camera.scale;
        gfix.camera.coord.x -= 1.0;
    });
    key_bind_set("vmd", [](){
        //gfix.camera.coord.x += gfix.camera.scale;
        gfix.camera.coord.x += 1.0;
    });
    key_bind_set("vms", [](){
        //gfix.camera.coord.y -= gfix.camera.scale;
        gfix.camera.coord.y -= 1.0;
    });
    key_bind_set("vmw", [](){
        //gfix.camera.coord.y += gfix.camera.scale;
        gfix.camera.coord.y += 1.0;
    });
    key_bind_set("vsi", [](){
        gfix.camera.scale /= 2.0;
    });
    key_bind_set("vso", [](){
        gfix.camera.scale *= 2.0;
    });
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
    glColor4f(0.5, 0.5, 0.5, 1.0);
    glVertex2f(+1.0, +0.0);
    glVertex2f(+0.0, +1.0);
    glVertex2f(-1.0, -0.0);
    glVertex2f(-0.0, -1.0);
    glEnd();
    /* bottom bar */
    auto xl = gfix.camera.coord.x-gfix.camera.sizes.w/2.0;
    auto xr = gfix.camera.coord.x+gfix.camera.sizes.w/2.0;
    auto yb = gfix.camera.coord.y-gfix.camera.sizes.h/2.0;
    auto yt = gfix.camera.coord.y+gfix.camera.sizes.h/2.0;
    glBegin(GL_QUADS);
    glColor4f(0.0, 0.0, 0.0, 1.0);
    glVertex2f(xl, yb);
    glVertex2f(xl, yb + gfix.camera.scale * 0.1);
    glVertex2f(xr, yb + gfix.camera.scale * 0.1);
    glVertex2f(xr, yb);
    glEnd();
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glRasterPos2f(xl + gfix.camera.scale * 0.025, yb + gfix.camera.scale * 0.025);
    for (auto&iter:key_line)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, iter);
    }
    glRasterPos2f(xl + gfix.camera.scale * 0.025, yt - gfix.camera.scale * 0.025);
    glutSwapBuffers();
}

void work(void)
{
    /* time */
    timer.delta = timer.mswas - timer.msnow;
    timer.mswas = timer.msnow;
    timer.msnow = glutGet(GLUT_ELAPSED_TIME);
    /* view */
    auto ratio = static_cast<float>(gfix.window.sizes.w) / static_cast<float>(gfix.window.sizes.h);
    gfix.camera.sizes.w = gfix.camera.scale * ratio;
    gfix.camera.sizes.h = gfix.camera.scale;
    glViewport(0, 0, gfix.window.sizes.w, gfix.window.sizes.h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(
        gfix.camera.coord.x - gfix.camera.sizes.w / 2.0,
        gfix.camera.coord.x + gfix.camera.sizes.w / 2.0,
        gfix.camera.coord.y - gfix.camera.sizes.h / 2.0,
        gfix.camera.coord.y + gfix.camera.sizes.h / 2.0
    );
    /* draw */
    glutPostRedisplay();
}

/*** callbacks ***/

void proc_resize(int sizew, int sizeh)
{
    gfix.window.sizes.w = sizew;
    gfix.window.sizes.h = sizeh;
}

_NAMESPACE_LEAVE

#ifdef _TYPE_RUN

int main(int argc, char** argv)
{
    return _NAMESPACE::main(argc, argv);
}

#endif
