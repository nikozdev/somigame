#ifndef SOMIGAME_MAIN_UNIT_TICKER_HXX
#define SOMIGAME_MAIN_UNIT_TICKER_HXX

#include "../head.hxx"

namespace somigame { namespace main {

// typedef

using timen_t = msize_t;

typedef struct ticker_t {
    timen_t dif_mil = 0; /* miliseconds between current and last frames */
    timen_t was_mil = 0; /* last frame milisecond */
    timen_t now_mil = 0; /* current frame milisecond */
} ticker_t;

// signals

_DECL_DATA signal_t<void(const ticker_t&)>ticker_update_signal; // on each update
_DECL_DATA signal_t<void(const ticker_t&)>ticker_second_signal; // on each second

// getters

_DECL_FUNC const ticker_t&get_ticker();
_DECL_FUNC const count_t get_fps();

// actions

_DECL_FUNC bool init_unit_ticker();
_DECL_FUNC bool quit_unit_ticker();
_DECL_FUNC bool proc_unit_ticker();

template<typename func_t, typename...args_t>
bool call_on_mil(timen_t mil, func_t func, args_t&&...args)
{
    const auto ticker = get_ticker();
    const auto was = ticker.was_mil / mil;
    const auto now = ticker.now_mil / mil;
    const auto dif = now - was;
    if (dif > 0)
    {
        func(ticker, std::forward<args_t>(args)...);
        return TRUTH;
    }
    else { return FALSE; }
} // call_on_mil
template<typename func_t, typename...args_t>
bool call_on_sec(timen_t sec, func_t func, args_t&&...args)
{
    return call_on_mil(sec * 1000, func, std::forward<args_t>(args)...);
} // call_on_sec

} } // namespace somigame { namespace main {

#endif//SOMIGAME_MAIN_UNIT_TICKER_HXX
