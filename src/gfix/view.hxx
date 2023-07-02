#ifndef SOMIGAME_GFIX_VIEW_HXX
#define SOMIGAME_GFIX_VIEW_HXX 1

#include "../head.hxx"

/* headers */

#include "../geom/sizes.hxx"
#include "../geom/shape.hxx"
#include "../ecos.hxx"

namespace somigame { /* content */

namespace gfix { /* consdef */

constexpr auto VIEW_RATIO_X = 16;
constexpr auto VIEW_RATIO_Y = 9;

constexpr auto VIEW_ASIZE_X = UNIT_ASIZE_X * VIEW_RATIO_X;
constexpr auto VIEW_ASIZE_Y = UNIT_ASIZE_Y * VIEW_RATIO_Y;

} /* consdef */

namespace gfix { /* typedef */

/* view_t
 * the camera singleton, point of view of the player
*/
class view_t : public meta::single_t<view_t>
{
public: /* codetor */
    inline view_t() = default;
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
    bool turn90(const bool_t lside = TRUTH);
public: /* datadef */
    ent_t ent = entt::null;
};

} /* typedef */

} /* content */

#endif/*SOMIGAME_GFIX_VIEW_HXX*/
