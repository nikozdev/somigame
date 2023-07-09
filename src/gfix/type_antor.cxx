#ifndef SOMIGAME_GFIX_TYPE_ANTOR_CXX
#define SOMIGAME_GFIX_TYPE_ANTOR_CXX

#include "../head.hxx"
#include "../main.hxx"
#include "../main/unit_ticker.hxx"
#include "type_antor.hxx"

namespace somigame { namespace gfix {

// typedef

#if 1
#else
antor_t::antor_t(ecos::ent_t ent)
: ent(ent), start{ main::get_ticker().now_mil }, step_array{}
{ }

void antor_t::proc()
{
    auto done = 0;
    for (auto*step : this->step_array)
    {
        auto diff = main::get_ticker().now_mil - this->start;
        done += diff > step->until;
        step->play(diff);
    }
    if (done)
    {
        main::ticker_update_signal.binder.disconnect<&this_t::proc>(*this);
        this->start = main::get_ticker().now_mil;
    }
}
bool_t antor_t::play()
{
    this->start = main::get_ticker().now_mil;
    main::ticker_update_signal.binder.connect<&this_t::proc>(*this);
    return TRUTH;
}
#endif

// actions

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_ANTOR_CXX
