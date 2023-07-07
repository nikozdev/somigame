#ifndef SOMIGAME_MAIN_CXX
#define SOMIGAME_MAIN_CXX

#include "head.hxx"
#include "gfix/func_work.hxx"
#include "main/func_init.hxx"
#include "main/func_quit.hxx"
#include "main/func_work.hxx"

namespace somigame { namespace main {

// actions

signed main(signed argc, char**argv)
{
    ::glutInit(&argc, argv);
    ::somigame::main::init();
    ::glutIdleFunc([](){
        _EFNOT(main::work(), main::quit(), "main work failed");
    });
    ::glutDisplayFunc([](){
        _EFNOT(gfix::work(), main::quit(), "gfix work failed");
    });
    ::glutMainLoop();
    return error_none;
} // main

} } // namespace somigame { namespace main

#if defined(_TYPE_EXE)
int main(int argc, char** argv)
{
    return somigame::main::main(argc, argv);
}
#endif/*ifd(_TYPE_EXE)*/

#endif//SOMIGAME_MAIN_CXX
