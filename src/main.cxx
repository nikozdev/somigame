#ifndef SOMIGAME_MAIN_CXX
#define SOMIGAME_MAIN_CXX 1

#include "head.hxx"

/* headers*/

#include "main.hxx"
#include "iput.hxx"
#include "ecos.hxx"
#include "geom.hxx"
#include "fsys.hxx"
#include "gfix.hxx"
#include "game.hxx"

/* content */

#if defined(_TYPE_EXE)
int main(int argc, char** argv)
{ return somigame::main::main(argc, argv); }
#endif/*ifd(_TYPE_EXE)*/

namespace somigame { namespace main { /* signals */

signal_t<void(void)> init_top_signal;
signal_t<void(void)> init_bot_signal;
signal_t<void(void)> quit_top_signal;
signal_t<void(void)> quit_bot_signal;
signal_t<void(void)> work_top_signal;
signal_t<void(void)> work_bot_signal;

} } /* signals */

namespace somigame { namespace main { /* datadef */

timer_t timer;

} } /* datadef */

namespace somigame { namespace main { /* actions */

void init()
{
    main::init_top_signal.holder.publish(); memo::init();
    memo::init();
    ecos::init();
    geom::init();
    gfix::init();
    iput::init();
    fsys::init();
    game::init();
    main::init_bot_signal.holder.publish();
}
void quit()
{
    main::quit_top_signal.holder.publish();
    game::quit();
    fsys::quit();
    iput::quit();
    gfix::quit();
    geom::quit();
    ecos::quit();
    memo::quit();
    main::quit_bot_signal.holder.publish();
    exit(error_none);
}
void work()
{
    main::work_top_signal.holder.publish();
    timer.was_mil = timer.now_mil;
    timer.now_mil = ::glutGet(GLUT_ELAPSED_TIME);
    timer.dif_mil = timer.now_mil - timer.was_mil;
    if (timer.dif_mil == 0) { timer.dif_mil = 1; }
    else if (timer.dif_mil < 0) { timer.dif_mil = -timer.dif_mil; }
    call_on_sec([](int was_sec, int now_sec, int dif_sec){
        if (timer.dif_mil > 0)
        {
            timer.fps_num = 1'000 / timer.dif_mil;
            timer.dispatcher.trigger<newsec_event_t>( { .sec = timer.now_mil / 1'000 } );
            timer.dispatcher.update<newsec_event_t>();
        }
    });
    timer.dispatcher.trigger<update_event_t>( { .mil = timer.now_mil } );
    timer.dispatcher.update<update_event_t>();
    ::glutPostRedisplay();
    main::work_bot_signal.holder.publish();
}

int main(signed argc, char**argv)
{
    if constexpr (TRUTH)
    { /* the main execution */
        ::glutInit(&argc, argv);
        init();
        ::glutIdleFunc(main::work);
        ::glutDisplayFunc([](){ gfix::work(); });
        ::glutMainLoop();
    } /* the main execution */
    else if constexpr(FALSE)
    { /* memory benchmark #1 */
        constexpr msize_t REP = 4;
        constexpr msize_t DIV = 4;
        constexpr msize_t COUNT = memo::arena_t::MSIZE
            / nums::get_aligned<geom::gpos3_t>()
            / DIV;
        geom::gpos3_t**table;
        static_cast<memo::giver_t*>(memo::heapy_t::get())
        ->give(table, COUNT);
        msize_t was_mil, now_mil;
        if constexpr (TRUTH)
        { /* heapy */
            was_mil = std::clock();
            for (auto rep = 0; rep < REP; rep++)
            {
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::heapy_t::get()->new_mul(table[iter], DIV); }
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::heapy_t::get()->del_mul(table[iter], DIV); }
            }
            now_mil = std::clock();
            _OLOG("[heapy time]: %zu;%c", now_mil - was_mil, '\n');
        } /* heapy */
        if constexpr (TRUTH)
        { /* stack */
            was_mil = std::clock();
            for (auto rep = 0; rep < REP; rep++)
            {
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::stack_t::get()->new_mul(table[iter], DIV); }
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::stack_t::get()->del_mul(table[iter], DIV); }
            }
            now_mil = std::clock();
            _OLOG("[stack time]: %zu;%c", now_mil - was_mil, '\n');
        } /* stack */
        if constexpr (TRUTH)
        { /* arena */
            was_mil = std::clock();
            for (auto rep = 0; rep < REP; rep++)
            {
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::arena_t::get()->new_mul(table[iter], DIV); }
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::arena_t::get()->del_mul(table[iter], DIV); }
            }
            now_mil = std::clock();
            _OLOG("[arena time]: %zu;%c", now_mil - was_mil, '\n');
        } /* arena */
        if constexpr (TRUTH)
        { /* pager */
            was_mil = std::clock();
            for (auto rep = 0; rep < REP; rep++)
            {
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::pager_t::get()->new_mul(table[iter], DIV); }
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::pager_t::get()->del_mul(table[iter], DIV); }
            }
            now_mil = std::clock();
            _OLOG("[pager time]: %zu;%c", now_mil - was_mil, '\n');
        } /* pager */
    } /* memory benchmark #1 */
    else if constexpr (FALSE)
    { /* new/delete override test */
        std::string s1 = "hello";
        std::string s2 = "world";
        std::string s3 = s1 + s2;
    } /* new/delete override test */
    else if constexpr (FALSE)
    { /* vector benchmark time */
        constexpr msize_t COUNT = 0xf00;
        constexpr msize_t REP = 0x3;
        msize_t was_mil, now_mil;
        for (auto iter = 0; iter < REP; iter++)
        {
            was_mil = std::clock();
            for (auto iter = 0; iter < COUNT; iter++)
            {
                std::vector<std::string> sv = {""};
                for (auto iter = 0; iter < COUNT; iter++)
                {
                    sv.push_back(std::to_string(iter%128));
                }
            }
            now_mil = std::clock();
            _OLOG("[string vector alloc time]: %zu;%c", now_mil - was_mil, '\n');
        }
    } /* vector benchmark time */
    else if constexpr (FALSE)
    { /* member meta getter test */
        struct s1_t {
            bool b = 1;
            struct s2_t {
                unsigned u = 42;
                struct s3_t {
                    char c = 'c';
                } s3;
            } s2;
        } s1;
        _ELOG(
            "s1.b={:d};"
            "s1.s2.u={:d};"
            "s1.s2.s3.c={:c};"
            "{:c}",
            get_member(s1,&s1_t::b),
            get_member(s1,&s1_t::s2,&s1_t::s2_t::u),
            get_member(s1,&s1_t::s2,&s1_t::s2_t::s3,&s1_t::s2_t::s3_t::c),
            text::ENDL_CHAR
        );
    } /* member meta getter test */
    return error_none;
}

} } /* actions */

#endif/*SOMIGAME_MAIN_CXX*/
