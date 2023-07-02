#ifndef SOMIGAME_GFIX_ANTOR_CXX
#define SOMIGAME_GFIX_ANTOR_CXX 1

#include "../head.hxx"

/* headers */

#include "antor.hxx"

#include "../main.hxx"

namespace somigame { /* content */

namespace gfix { /* typedef */

antor_t::antor_t(ent_t ent)
: ent(ent), start{ main::timer.now_mil }, step_array{}
{ }

void antor_t::proc()
{
    auto done = 0;
    for (auto*step : this->step_array)
    {
        auto diff = main::timer.now_mil - this->start;
        done += diff > step->until;
        step->play(diff);
    }
    if (done)
    {
        main::timer.dispatcher.sink<main::update_event_t>()
        .disconnect<&this_t::proc>(*this);
        this->start = main::timer.now_mil;
    }
}
bool_t antor_t::play()
{
    this->start = main::timer.now_mil;
    main::timer.dispatcher.sink<main::update_event_t>()
    .connect<&this_t::proc>(*this);
    return TRUTH;
}

} /* typedef */

} /* content */

#endif/*SOMIGAME_GFIX_ANTOR_CXX*/
