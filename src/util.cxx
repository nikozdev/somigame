/* headers */

#include "util.hxx"

#include <GL/glut.h>

/* defines */

/* content */

_NAMESPACE_ENTER

/** typedef **/

/** datadef **/

timer_t timer;

/** actions **/

void util_loop()
{
    timer.was_mil = timer.now_mil;
    timer.now_mil = glutGet(GLUT_ELAPSED_TIME);
    timer.dif_mil = timer.now_mil - timer.was_mil;
    if (timer.dif_mil == 0) { timer.dif_mil = 1; }
    else if (timer.dif_mil < 0) { timer.dif_mil = -timer.dif_mil; }
    call_on_sec([](int was_sec, int now_sec, int dif_sec){
        if (timer.dif_mil > 0)
        {
            timer.fps_num = 1'000 / timer.dif_mil;
            timer.sig_sec.call();
        }
    });
    timer.sig_upd.call();
}

_NAMESPACE_LEAVE
