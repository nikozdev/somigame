/* headers*/

#include "main.hxx"
#include "gfix.hxx"
#include "fsix.hxx"
#include "oput.hxx"
#include "iput.hxx"

/* content */

_NAMESPACE_ENTER

/** datadef **/

timer_t timer;

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
    /* graphics */
    gfix_init();
    /* keyboard */
    key_board_init();
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    /** menu **/
    key_bind_set("mq", [](){ exit(_ERROR_NONE); });
    key_bind_set("mr", [](){ gfix.camera = camera_t(); });
    /** view **/
    key_bind_set("vma", [](){ view_move(-1, 0); });
    key_bind_set("vmd", [](){ view_move(+1, 0); });
    key_bind_set("vms", [](){ view_move(0, -1); });
    key_bind_set("vmw", [](){ view_move(0, +1); });
    key_bind_set("vsi", [](){ view_scale(-1); });
    key_bind_set("vso", [](){ view_scale(+1); });
    glutKeyboardFunc(proc_key_board);
    glutSpecialFunc(proc_key_board_special);
    /* oput */
    glutDisplayFunc(draw_loop);
    glutReshapeFunc([](int sizew, int sizeh) {
        gfix.window.sizes.w = sizew;
        gfix.window.sizes.h = sizeh;
    });
    /* work */
    glutIdleFunc([](){
        /* time */
        timer.was_mil = timer.now_mil;
        auto was_sec = timer.was_mil/1000;
        timer.now_mil = glutGet(GLUT_ELAPSED_TIME)%(1000*1000);
        auto now_sec = timer.now_mil/1000;
        timer.dif_mil = timer.now_mil - timer.was_mil;
        if ((now_sec - was_sec) > 0)
        {
            if (timer.dif_mil > 0)
            {
                timer.fps_num = 60'000 / timer.dif_mil;
                // timer.sig_sec.call(); TODO
            }
        }
        // timer.sig_upd.call(); TODO
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
