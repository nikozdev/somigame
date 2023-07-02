#ifndef SOMIGAME_PICKTILE_HXX
#define SOMIGAME_PICKTILE_HXX 1

#include "../head.hxx"

/* headers */

#include "../ecos.hxx"

/* content */

namespace somigame { namespace game { /* typedef */

/* picktile_t
 * the tile picker quad
 * part of the interface
*/
class picktile_t : public meta::single_t<picktile_t>
{
public: /* codetor */
    inline picktile_t() = default;
public: /* getters */
    [[nodiscard]] inline constexpr
    auto get_ent() const
    { return this->ent; }
    template<typename...com_t>
    [[nodiscard]] inline constexpr
    auto&&get_com() const
    { return ecos::ecs.get<com_t...>(this->get_ent()); }
public: /* vetters */
    [[nodiscard]] inline
    bool vet_ent(bool valid = TRUTH)
    { return ecos::ecs.valid(this->get_ent()) == valid; }
    template<typename...com_t>
    [[nodiscard]] inline
    bool vet_com() const
    { return ecos::ecs.all_of<com_t...>(this->get_ent()); }
public: /* actions */
    bool init();
    bool quit();
public: /* datadef */
    ent_t ent = entt::null;
};

} } /* typedef */

#endif/*SOMIGAME_PICKTILE_HXX*/
