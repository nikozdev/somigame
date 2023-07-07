#ifndef SOMIGAME_GFIX_TYPE_VISUAL_HXX
#define SOMIGAME_GFIX_TYPE_VISUAL_HXX

#include "../head.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../game/type_sizes.hxx"

namespace somigame { namespace gfix {

// typedef

typedef struct color_t {
    ubyte_t v = 0u;
} color_t, com_color_t;

typedef class visual_t
{
public: // getters
    v1s_t get() const { return this->value; }
public: // vetters
    v1s_t vet() const { return this->value >= 0; }
public: // setters
    v1s_t set(v1s_t value);
public: // actions
    v1s_t rev() { return this->set(-this->get()); }
public: // consdef
    constexpr static v1s_t WEAK_HIDE = -0x01;
    constexpr static v1s_t WEAK_SHOW = +0x01;
    constexpr static v1s_t SOFT_HIDE = -0x10;
    constexpr static v1s_t SOFT_SHOW = +0x10;
    constexpr static v1s_t FIRM_HIDE = -0xf0;
    constexpr static v1s_t FIRM_SHOW = +0xf0;
    constexpr static v1s_t HARD_HIDE = -0xff;
    constexpr static v1s_t HARD_SHOW = +0xff;
private: // datadef
    v1s_t value = WEAK_SHOW; // (>=0)show or (<0)hide
} visual_t, com_visual_t; // any visible entity

typedef struct lgrid_t {
    geom::asize_t cells{ game::TCELL_ASIZE_X, game::TCELL_ASIZE_X };
    geom::asize_t tails{ 0, 0 }; // trailing lines from both sides
    geom::asize_t lsize{ 1, 1 }; // lines sizes in both axes
} lgrid_t, com_lgrid_t; // line grid

// actions

_DECL_FUNC v1b_t init_type_visual();
_DECL_FUNC v1b_t quit_type_visual();

} } // namespace somigame { namespace gfix {

#endif/*SOMIGAME_GFIX_VISUAL_HXX*/
