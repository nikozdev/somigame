#ifndef SOMIGAME_GEOM_SIZES_HXX
#define SOMIGAME_GEOM_SIZES_HXX 1

#include "../head.hxx"

/* headers */

namespace somigame { /* content */

namespace geom { /* consdef */

constexpr v1s_t RSIZE_DIV = 0x80; /* normalize dividing */
constexpr v1s_t RSIZE_MID = 0x40; /* half of the parent */
constexpr v1s_t RSIZE_MIN = 0x00; /* no relative effect */
constexpr v1s_t RSIZE_MAX = 0x80; /* entire parent size */

} /* consdef */

namespace geom { /* typedef */

typedef struct asize_t {
    v1s_t x = 0, y = 0;
} asize_t, com_asize_t; /* absolute size */
typedef struct rsize_t {
    v1s_t x = RSIZE_MIN, y = RSIZE_MIN;
} rsize_t, com_rsize_t; /* relative size */
typedef struct gsize_t {
    v1s_t x = 0, y = 0;
} gsize_t, com_gsize_t; /* global size */
typedef struct fsize_t {
    v1s_t x = 0, y = 0;
} fsize_t; /* family size */

typedef struct ratio_t {
    v1s08_t x = 1, y = 1;
} ratio_t, com_ratio_t; /* limit of x-y sizes relative to each other */

} /* typedef */

} /* content */

#endif/*SOMIGAME_GEOM_SIZES_HXX*/
