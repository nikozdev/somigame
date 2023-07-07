#ifndef SOMIGAME_GEOM_TYPE_SIZES_HXX
#define SOMIGAME_GEOM_TYPE_SIZES_HXX

#include "../head.hxx"

namespace somigame { namespace geom {

// consdef

constexpr v1s_t RSIZE_DIV = 0x80; /* normalize dividing */
constexpr v1s_t RSIZE_MID = 0x40; /* half of the parent */
constexpr v1s_t RSIZE_MIN = 0x00; /* no relative effect */
constexpr v1s_t RSIZE_MAX = 0x80; /* entire parent size */

// typedef

typedef struct asize_t {
    v1s_t x = 0, y = 0;
} asize_t, com_asize_t; // absolute size
typedef struct rsize_t {
    v1s_t x = RSIZE_MIN, y = RSIZE_MIN;
} rsize_t, com_rsize_t; // relative size
typedef struct gsize_t {
    v1s_t x = 0, y = 0;
} gsize_t, com_gsize_t; // global size
typedef struct lsize_t {
    struct x_t {
        v1s_t l = std::numeric_limits<v1s_t>::min();
        v1s_t r = std::numeric_limits<v1s_t>::max();
    } x;
    struct y_t {
        v1s_t b = std::numeric_limits<v1s_t>::min();
        v1s_t t = std::numeric_limits<v1s_t>::max();
    } y;
} lsize_t, com_lsize_t; // limited sizes

typedef struct ratio_t {
    v1s_t x = 1, y = 1;
} ratio_t, com_ratio_t; /* limit of x-y sizes relative to each other */

// actions

_DECL_FUNC v1b_t init_type_sizes();
_DECL_FUNC v1b_t quit_type_sizes();

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_TYPE_SIZES_HXX
