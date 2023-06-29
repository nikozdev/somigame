#include "head.hxx"

/* headers*/

#include "main.hxx"
#include "memo.hxx"
#include "util.hxx"
#include "iput.hxx"
#include "ecos.hxx"
#include "fsys.hxx"
#include "gfix.hxx"
#include "game.hxx"

/* content */

_NAMESPACE_ENTER

/** datadef **/

t_signal_t<void(void)> main_init_top_signal;
t_signal_t<void(void)> main_init_bot_signal;
t_signal_t<void(void)> main_quit_top_signal;
t_signal_t<void(void)> main_quit_bot_signal;

/** actions **/

void main_init()
{
    main_init_top_signal.holder.publish();
    memo::init();
    ecos_init();
    util_init();
    gfix_init();
    iput_init();
    game_init();
    main_init_bot_signal.holder.publish();
}
void main_quit()
{
    main_quit_top_signal.holder.publish();
    game_quit();
    iput_quit();
    gfix_quit();
    util_quit();
    ecos_quit();
    memo::quit();
    main_quit_bot_signal.holder.publish();
    exit(_ERROR_NONE);
}
void main_work()
{
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
}

int main(int argc, char** argv)
{
    if constexpr (_TRUTH)
    { /* the main execution */
        ::glutInit(&argc, argv);
        main_init();
        ::glutIdleFunc(main_work);
        ::glutDisplayFunc(gfix_work);
        ::glutMainLoop();
    } /* the main execution */
    else if constexpr(_FALSE)
    { /* memory benchmark #1 */
        constexpr msize_t REP = 4;
        constexpr msize_t DIV = 4;
        constexpr msize_t COUNT = memo::OWNER_MSIZE
            / _ALIGN(sizeof(gpos3_t), alignof(gpos3_t))
            / DIV;
        gpos3_t**table;
        static_cast<memo::owner_t*>(memo::heapy)->give(table, COUNT);
        msize_t was_mil, now_mil;
        if constexpr (_TRUTH)
        { /* heapy */
            was_mil = std::clock();
            for (auto rep = 0; rep < REP; rep++)
            {
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::heapy->new_mul(table[iter], DIV); }
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::heapy->del_mul(table[iter], DIV); }
            }
            now_mil = std::clock();
            _OLOG("[heapy time]: %zu;%c", now_mil - was_mil, '\n');
        } /* heapy */
        if constexpr (_TRUTH)
        { /* stack */
            was_mil = std::clock();
            for (auto rep = 0; rep < REP; rep++)
            {
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::stack->new_mul(table[iter], DIV); }
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::stack->del_mul(table[iter], DIV); }
            }
            now_mil = std::clock();
            _OLOG("[stack time]: %zu;%c", now_mil - was_mil, '\n');
        } /* stack */
        if constexpr (_TRUTH)
        { /* arena */
            was_mil = std::clock();
            for (auto rep = 0; rep < REP; rep++)
            {
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::arena->new_mul(table[iter], DIV); }
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::arena->del_mul(table[iter], DIV); }
            }
            now_mil = std::clock();
            _OLOG("[arena time]: %zu;%c", now_mil - was_mil, '\n');
        } /* arena */
        if constexpr (_TRUTH)
        { /* pager */
            was_mil = std::clock();
            for (auto rep = 0; rep < REP; rep++)
            {
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::pager->new_mul(table[iter], DIV); }
                for (auto iter = 0; iter < COUNT; iter++)
                { memo::pager->del_mul(table[iter], DIV); }
            }
            now_mil = std::clock();
            _OLOG("[pager time]: %zu;%c", now_mil - was_mil, '\n');
        } /* pager */
    } /* memory benchmark #1 */
    else if constexpr (_FALSE)
    { /* new/delete override test */
        std::string s1 = "hello";
        std::string s2 = "world";
        std::string s3 = s1 + s2;
    } /* new/delete override test */
    else if constexpr (_FALSE)
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
    else if constexpr (_FALSE)
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
            "s1.b=%d;\n"
            "s1.s2.u=%u;\n"
            "s1.s2.s3.c=%c;\n"
            "%c",
            get_member(s1,&s1_t::b),
            get_member(s1,&s1_t::s2,&s1_t::s2_t::u),
            get_member(s1,&s1_t::s2,&s1_t::s2_t::s3,&s1_t::s2_t::s3_t::c),
            '\n'
        );
    } /* member meta getter test */
    return _ERROR_NONE;
}

_NAMESPACE_LEAVE

#ifdef _TYPE_RUN

int main(int argc, char** argv)
{
    return _NAMESPACE::main(argc, argv);
}

#endif
