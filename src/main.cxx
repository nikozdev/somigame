/* headers*/

#include "main.hxx"
#include "util.hxx"
#include "gfix.hxx"
#include "fsix.hxx"
#include "oput.hxx"
#include "iput.hxx"
#include "ecos.hxx"

/* content */

_NAMESPACE_ENTER

/** datadef **/

/** actions **/

int main(int argc, char** argv)
{
    /* framework */
    glutInit(&argc, argv);
    /* window */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(gfix.window.coord.x, gfix.window.coord.y);
    glutInitWindowSize(gfix.window.sizes.w, gfix.window.sizes.h);
    glutCreateWindow(_NAME_STR);
    /* entity component system */
    ecos_init();
    /* graphics */
    gfix_init();
    /* keyboard */
    key_board_init();
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    /** menu **/
    /*** quit ***/
    key_bind_set("mq", [](int narg){ exit(_ERROR_NONE); });
    /*** reset ***/
    key_bind_set("mr", [](int narg){
    });
    /** view **/
    glutKeyboardFunc(proc_key_board);
    /* oput */
    glutDisplayFunc(draw_loop);
    glutReshapeFunc([](int sizew, int sizeh) {
        gfix.window.sizes.w = sizew;
        gfix.window.sizes.h = sizeh;
    });
    /* work */
    glutIdleFunc([](){
        /* util */
        util_loop();
        /* draw */
        glutPostRedisplay();
    });
    glutMainLoop();
    /* final */
    std::clog << "good day, sir !" << std::endl;
    return _ERROR_NONE;
}

_NAMESPACE_LEAVE

#ifdef _TYPE_RUN

int main(int argc, char** argv)
{
    return _NAMESPACE::main(argc, argv);
}

#endif
