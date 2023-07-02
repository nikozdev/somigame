#ifndef SOMIGAME_GFIX_GUIS_HXX
#define SOMIGAME_GFIX_GUIS_HXX 1

#include "../head.hxx"

/* headers */

#include "view.hxx"
#include "../ecos.hxx"
#include "../geom/sizes.hxx"

/* content */

namespace somigame {

namespace gfix { /* consdef */

constexpr auto GUIS_ASIZE_X = UNIT_ASIZE_X * 2;
constexpr auto GUIS_ASIZE_Y = UNIT_ASIZE_Y;
constexpr auto GUIS_RSIZE_X = geom::RSIZE_MAX * 4 / VIEW_RATIO_X;
constexpr auto GUIS_RSIZE_Y = geom::RSIZE_MAX / VIEW_RATIO_Y;
constexpr auto GUIS_LAYER = 0xff;

} /* consdef */

namespace gfix { /* typedef */

/* guis_t
 * graphical user interface entities
*/
class guis_t : public meta::single_t<guis_t>
{
public: /* codetor */
    inline guis_t() = default;
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

#endif/*SOMIGAME_GFIX_GUIS_HXX*/
