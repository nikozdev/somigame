#pragma once

/* headers */

#include "head.hxx"

/* content */

_NAMESPACE_ENTER

/** typedef **/

typedef struct timer_t {
    size_t dif_mil = 0; /* miliseconds between current and last frames */
    size_t was_mil = 0; /* last frame milisecond */
    size_t now_mil = 0; /* current frame milisecond */
    size_t fps_num = 0; /* frames per second number */
    /* TODO: implement signal class
    signal_t sig_sec;
    signal_t sig_upd;
     */
} timer_t;

/** datadef **/

extern timer_t timer;

/** actions **/

extern int main(int, char**);

_NAMESPACE_LEAVE
