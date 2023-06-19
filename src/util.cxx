/* headers */

#include "util.hxx"
#include "gfix.hxx"

#include <GL/glut.h>

/* defines */

/* content */

_NAMESPACE_ENTER

/** typedef **/

signal_t::signal_t()
{
}

signal_t::~signal_t()
{
    for (auto&link : this->func_list)
    { if (link) { delete link; } }
}

index_t signal_t::listen(func_t func)
{
    for (auto index = 0; index < func_list.size(); index++)
    {
        auto&link = this->func_list[index]; 
        if (link == _NULL)
        {
            link = new func_t(func);
            return index;
        }
    }
    this->func_list.push_back(new func_t(func));
    return this->func_list.size();
}

bool_t signal_t::forget(index_t index)
{
    if ((this->func_list.size() - 1) == index)
    {
        this->func_list.erase(this->func_list.begin() + index);
        return true;
    }
    auto&link = this->func_list[index];
    if (link == _NULL)
    { return false; }
    else
    {
        delete link;
        link = _NULL;
        return true;
    }
}

void signal_t::call()
{
    for (auto&link : this->func_list)
    { if (link) { (*link)(); } }
}

/** datadef **/

util_t util;

/** actions **/

void util_loop()
{
    constexpr auto&timer = util.timer;
    timer.was_mil = util.timer.now_mil;
    timer.now_mil = glutGet(GLUT_ELAPSED_TIME);
    timer.dif_mil = util.timer.now_mil - util.timer.was_mil;
    if (timer.dif_mil == 0) { timer.dif_mil = 1; }
    else if (timer.dif_mil < 0) { timer.dif_mil = -timer.dif_mil; }
    call_on_sec([](int was_sec, int now_sec, int dif_sec){
        if (util.timer.dif_mil > 0)
        {
            util.timer.fps_num = 60'000 / util.timer.dif_mil;
            util.timer.sig_sec.call();
        }
    });
    util.timer.sig_upd.call();
}

_NAMESPACE_LEAVE
