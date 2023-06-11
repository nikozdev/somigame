#pragma once

/* headers */

#include "head.hxx"

/* defines */
/* content */

_NAMESPACE_ENTER

/** typedef **/

class signal_t
{
public: /* typedef */
    using this_t = signal_t;
    using func_t = std::function<void(void)>;
    using func_list_t = std::vector<func_t*>;
    using link_t = func_t*;
public: /* codetor */
    signal_t();
    ~signal_t();
public: /* setters */
    index_t listen(func_t);
    bool_t forget(index_t);
public: /* actions */
    void call();
private:
    func_list_t func_list;
};

typedef struct timer_t {
    size_t dif_mil = 0; /* miliseconds between current and last frames */
    size_t was_mil = 0; /* last frame milisecond */
    size_t now_mil = 0; /* current frame milisecond */
    size_t fps_num = 0; /* frames per second number */
    signal_t sig_sec; /* call on every second */
    signal_t sig_upd; /* call on every update */
} timer_t;

typedef struct util_t {
    timer_t timer;
} util_t;

/** datadef **/

extern util_t util;

/** actions **/

extern void util_loop();

_NAMESPACE_LEAVE
