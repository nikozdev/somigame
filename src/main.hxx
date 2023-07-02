#ifndef SOMIGAME_MAIN_HXX
#define SOMIGAME_MAIN_HXX 1

#include "head.hxx"

namespace somigame { /* content */

namespace main { /* typedef */

typedef struct update_event_t {
    msize_t mil;
} timer_mil_event_t; /* event sent on every update */
typedef struct newsec_event_t {
    msize_t sec;
} timer_sec_event_t; /* event sent on every second */

typedef struct timer_t {
    msize_t dif_mil = 0; /* miliseconds between current and last frames */
    msize_t was_mil = 0; /* last frame milisecond */
    msize_t now_mil = 0; /* current frame milisecond */
    msize_t fps_num = 0; /* frames per second number */
    dispatcher_t dispatcher;
} timer_t; /* timing values and signals holder */

} /* typedef */

namespace main { /* signals */

extern signal_t<void(void)> init_top_signal;
extern signal_t<void(void)> init_bot_signal;
extern signal_t<void(void)> quit_top_signal;
extern signal_t<void(void)> quit_bot_signal;
extern signal_t<void(void)> work_top_signal;
extern signal_t<void(void)> work_bot_signal;

} /* signals */

namespace main { /* datadef */

extern timer_t timer;

} /* datadef */

namespace main { /* actions */

extern void init();
extern void quit();
extern void work();

extern int main(int, char**);

template<typename func_t>
void call_on_sec(func_t func, count_t sec = 1)
{
    auto was = timer.was_mil / (sec * 1'000);
    auto now = timer.now_mil / (sec * 1'000);
    auto dif = now - was;
    if (dif > 0) { func(was, now, dif); }
}

template<typename func_t>
void call_on_mil(func_t func, count_t mil = 1)
{
    auto was = timer.was_mil / mil;
    auto now = timer.now_mil / mil;
    auto dif = now - was;
    if (dif > 0) { func(was, now, dif); }
}

} /* actions */

} /* content */

#endif/*SOMIGAME_MAIN_HXX*/
