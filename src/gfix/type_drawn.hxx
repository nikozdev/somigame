#ifndef SOMIGAME_GFIX_TYPE_DRAWN_HXX
#define SOMIGAME_GFIX_TYPE_DRAWN_HXX

#include "../head.hxx"

namespace somigame { namespace gfix {

// typedef

typedef struct color_t {
    ubyte_t v = 0u;
} color_t, com_color_t;

typedef struct rdrawn_t {
    v1bit_t valid = TRUTH;
} rdrawn_t, com_rdrawn_t; // relative drawn flag
typedef struct gdrawn_t {
    v1bit_t valid = TRUTH;
} gdrawn_t, com_gdrawn_t; // global drawn flag

// actions

_DECL_FUNC v1b_t init_type_drawn();
_DECL_FUNC v1b_t quit_type_drawn();

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_DRAWN_HXX
