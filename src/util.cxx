/* headers */

#include "util.hxx"
#include "gfix.hxx"

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
    util.timer.was_mil = util.timer.now_mil;
    auto was_sec = util.timer.was_mil / 1000;
    util.timer.now_mil = glutGet(GLUT_ELAPSED_TIME) % (1000 * 1000);
    auto now_sec = util.timer.now_mil/1000;
    util.timer.dif_mil = util.timer.now_mil - util.timer.was_mil;
    if ((now_sec - was_sec) > 0)
    {
        if (util.timer.dif_mil > 0)
        {
            util.timer.fps_num = 60'000 / util.timer.dif_mil;
            util.timer.sig_sec.call();
        }
    }
    util.timer.sig_upd.call();
}

_NAMESPACE_LEAVE
