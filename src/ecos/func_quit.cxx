#ifndef SOMIGAME_ECOS_FUNC_QUIT_CXX
#define SOMIGAME_ECOS_FUNC_QUIT_CXX

#include "../head.hxx"
#include "../ecos.hxx"

namespace somigame { namespace ecos {

// signals

_DEFN_DATA signal_t<void(void)>quit_top_signal;
_DEFN_DATA signal_t<void(void)>quit_bot_signal;

// actions

_DEFN_FUNC bool quit()
{
    _EFNOT(
        ecos::reg.storage<ent_t>().in_use() != 0,
        return FALSE,
        "ecos quit is not allowed; ents in use: %d",
        ecos::reg.storage<ent_t>().in_use()
    );
    ecos::quit_top_signal.holder.publish();
    ecos::quit_type_family();
    ecos::reg.clear();
    ecos::quit_bot_signal.holder.publish();
    return TRUTH;
} // quit

} } // namespace somigame { namespace ecos {

#endif//SOMIGAME_ECOS_FUNC_QUIT_CXX
