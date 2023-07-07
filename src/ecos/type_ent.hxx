#ifndef SOMIGAME_ECOS_TYPE_ENT_HXX
#define SOMIGAME_ECOS_TYPE_ENT_HXX

#include "../head.hxx"

namespace somigame { namespace ecos {

// typedef

typedef entt::entity ent_t;

} } // namespace somigame { namespace ecos {

template<>
struct std::formatter<somigame::ecos::ent_t>
: public std::formatter<entt::id_type> {
    inline
    auto format(const auto&val, auto&con) const
    { return std::format_to(con.out(),
        "([eid]={:d})", static_cast<entt::id_type>(val)
    ); }

};

#endif//SOMIGAME_ECOS_TYPE_ENT_HXX
