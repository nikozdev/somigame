/* headers*/

#include "main.hxx"
#include "util.hxx"
#include "ecos.hxx"
#include "gfix.hxx"
#include "fsix.hxx"
#include "oput.hxx"
#include "iput.hxx"
#include "game.hxx"

/* content */

_NAMESPACE_ENTER

/** datadef **/

/** actions **/

int main(int argc, char** argv)
{
    /* framework */
    glutInit(&argc, argv);
    /* entity component system */
    ecos_init();
    /* graphics */
    gfix_init();
    /* physics */
    fsix_init();
    /* gameplay */
    game_init();
    /* keyboard */
    key_board_init();
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    glutKeyboardFunc(proc_key_board);
    /** menu **/
    /*** quit ***/
    key_bind_set("mq", [](int narg){ exit(_ERROR_NONE); });
    /* oput */
    glutDisplayFunc(draw_loop);
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
