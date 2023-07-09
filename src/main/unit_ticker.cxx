#ifndef SOMIGAME_MAIN_UNIT_TICKER_CXX
#define SOMIGAME_MAIN_UNIT_TICKER_CXX

#include "../head.hxx"
#include "unit_ticker.hxx"

namespace somigame { namespace main {

// signals

_DEFN_DATA signal_t<void(const ticker_t&)>ticker_update_signal; // on each update
_DEFN_DATA signal_t<void(const ticker_t&)>ticker_second_signal; // on each second

// datadef

static ticker_t ticker;
count_t fps = 0; // frames per second number

// getters

_DEFN_FUNC const ticker_t&get_ticker()
{ return ticker; }
_DEFN_FUNC const count_t get_fps()
{ return fps; }

// actions

_DEFN_FUNC bool init_unit_ticker()
{
    ticker.was_mil = 0;
    ticker.now_mil = 0;
    ticker.dif_mil = 0;
    return TRUTH;
} // init_unit_ticker
_DEFN_FUNC bool quit_unit_ticker()
{
    ticker.was_mil = 0;
    ticker.now_mil = 0;
    ticker.dif_mil = 0;
    return TRUTH;
} // quit_unit_ticker
_DEFN_FUNC bool proc_unit_ticker()
{
    ticker.was_mil = ticker.now_mil;
    ticker.now_mil = ::glutGet(GLUT_ELAPSED_TIME);
    ticker.dif_mil = ticker.now_mil - ticker.was_mil;
    if (ticker.dif_mil == 0) { ticker.dif_mil = 1; }
    else if (ticker.dif_mil < 0) { ticker.dif_mil = -ticker.dif_mil; }
    call_on_sec(1, [&](const ticker_t&temp){
        if (ticker.dif_mil > 0)
        {
            fps = 1'000 / ticker.dif_mil;
            ticker_second_signal.holder.publish(ticker);
        }
    });
    ticker_update_signal.holder.publish(ticker);
    return TRUTH;
} // proc_unit_ticker

} } // namespace somigame { namespace main {

#endif//SOMIGAME_MAIN_UNIT_TICKER_CXX
