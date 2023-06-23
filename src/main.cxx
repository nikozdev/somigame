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
    game_loop();
}

int main(int argc, char** argv)
{
    if constexpr (_FALSE)
    {
        glutInit(&argc, argv);
        ecos_init();
        gfix_init();
        iput_init();
        game_init();
        glutIdleFunc(main_loop);
        glutMainLoop();
    }
    else
    {
        constexpr msize_t DIV = 4;
        constexpr msize_t COUNT = (memo::OWNER_MSIZE >> 4)/DIV;
        constexpr msize_t REP = 4;
        coord_t**table;
        static_cast<memo::owner_t*>(memo::heapy)->give<coord_t*>(table, COUNT);
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
    return _ERROR_NONE;
}

_NAMESPACE_LEAVE

#ifdef _TYPE_RUN

int main(int argc, char** argv)
{
    return _NAMESPACE::main(argc, argv);
}

#endif
