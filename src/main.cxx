/* headers*/

#include "main.hxx"
#include "util.hxx"
#include "oput.hxx"
#include "iput.hxx"
#include "ecos.hxx"
#include "fsys.hxx"
#include "gfix.hxx"
#include "fsix.hxx"
#include "game.hxx"

#include <GL/glut.h>

/* content */

_NAMESPACE_ENTER

/** datadef **/

/** actions **/

void main_loop()
{
    util_loop();
    fsix_loop();
    call_on_mil([](int, int, int){
        gfix_loop();
    }, 16);
    game_loop();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    ecos_init();
    fsys_init();
    gfix_init();
    fsix_init();
    game_init();
    iput_init();
    glutIdleFunc(main_loop);
    glutMainLoop();
    return _ERROR_NONE;
}

_NAMESPACE_LEAVE

#ifdef _TYPE_RUN

int main(int argc, char** argv)
{
    return _NAMESPACE::main(argc, argv);
}

#endif
