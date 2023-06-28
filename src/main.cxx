#include "head.hxx"

/* headers*/

#include "main.hxx"
#include "memo.hxx"
#include "util.hxx"
#include "oput.hxx"
#include "iput.hxx"
#include "ecos.hxx"
#include "fsys.hxx"
#include "gfix.hxx"
#include "game.hxx"

#include <GL/glut.h>

/* content */

_NAMESPACE_ENTER

/** datadef **/

/** actions **/

void main_loop()
{
    util_loop();
    gfix_loop();
}

void main_quit()
{
    ecos_quit();
    exit(_ERROR_NONE);
}

int main(int argc, char** argv)
{
    if constexpr (_TRUTH)
    {
        ::glutInit(&argc, argv);
        memo::init();
        ecos_init();
        util_init();
        gfix_init();
        iput_init();
        game_init();
        ::glutIdleFunc(main_loop);
        ::glutMainLoop();
    }
    else if constexpr(_FALSE)
    {
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
    }
    else if constexpr (_FALSE)
    {
        std::string s1 = "hello";
        std::string s2 = "world";
        std::string s3 = s1 + s2;
    }
    else if constexpr (_FALSE)
    {
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
    }
    else if constexpr (_FALSE)
    {
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
    }
    return _ERROR_NONE;
}

_NAMESPACE_LEAVE

#ifdef _TYPE_RUN

int main(int argc, char** argv)
{
    return _NAMESPACE::main(argc, argv);
}

#endif
