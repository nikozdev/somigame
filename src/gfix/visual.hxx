#ifndef SOMIGAME_GFIX_VISUAL_HXX
#define SOMIGAME_GFIX_VISUAL_HXX 1

#include "../head.hxx"

/* headers */

namespace somigame { /* content */

namespace gfix { /* enumdef */

} /* enumdef */

namespace gfix { /* typedef */

typedef class visual_t
{
public: /* getters */
    v1s_t get() const { return this->value; }
public: /* vetters */
    v1s_t vet() const { return this->value >= 0; }
public: /* setters */
    v1s_t set(v1s_t value)
    {
        if (std::abs(value) >= std::abs(this->value))
        { this->value = value; }
        return this->value;
    }
public: /* consdef */
    constexpr static v1s_t WEAK_HIDE = -0x01;
    constexpr static v1s_t WEAK_SHOW = +0x01;
    constexpr static v1s_t SOFT_HIDE = -0x10;
    constexpr static v1s_t SOFT_SHOW = +0x10;
    constexpr static v1s_t FIRM_HIDE = -0xf0;
    constexpr static v1s_t FIRM_SHOW = +0xf0;
    constexpr static v1s_t HARD_HIDE = -0xff;
    constexpr static v1s_t HARD_SHOW = +0xff;
private: /* datadef */
    v1s_t value = 0; /* (>=0)show or (<0)hide */
} visual_t, com_visual_t; /* any visible entity */

typedef struct vrange_t {
    v1s_t range = 2;
} vrange_t, com_vrange_t; /* how much do we see around it ? */

} /* typedef */

} /* content */

#endif/*SOMIGAME_GFIX_VISUAL_HXX*/
