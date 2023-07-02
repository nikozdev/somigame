#ifndef SOMIGAME_HELPGRID_HXX
#define SOMIGAME_HELPGRID_HXX 1

#include "../head.hxx"

/* headers */

#include "../ecos.hxx"

/* content */

namespace somigame { namespace game { /* typedef */

/* helpgrid
 * the grid around the hero to distictly see possible moved
*/
class helpgrid_t : public meta::single_t<helpgrid_t>
{
public: /* codetor */
    inline helpgrid_t() = default;
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

#endif/*SOMIGAME_HELPGRID_HXX*/
