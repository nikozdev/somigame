#ifndef SOMIGAME_ECOS_FUNC_INIT_CXX
#define SOMIGAME_ECOS_FUNC_INIT_CXX

#include "../head.hxx"
#include "../ecos.hxx"

namespace somigame { namespace ecos {

// signals

_DEFN_DATA signal_t<void(void)>init_top_signal;
_DEFN_DATA signal_t<void(void)>init_bot_signal;

// actions

_DEFN_FUNC bool init()
{
    _EFNOT(
        ecos::reg.storage<ent_t>().in_use() == 0,
        return FALSE,
        "ecos init is not allowed; ents in use: %d",
        ecos::reg.storage<ent_t>().in_use()
    );
    ecos::init_top_signal.holder.publish();
    auto ent0 = ecos::reg.create(); // take the 0th index
    ecos::init_type_family();
    ecos::init_bot_signal.holder.publish();
    return TRUTH;
} // init

} } // namespace somigame { namespace ecos {

#endif//SOMIGAME_ECOS_FUNC_INIT_CXX
