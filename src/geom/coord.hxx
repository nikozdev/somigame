#ifndef SOMIGAME_GEOM_COORD_HXX
#define SOMIGAME_GEOM_COORD_HXX 1

#include "../head.hxx"

/* headers */

namespace somigame { /* content */

namespace geom { /* consdef */

constexpr v1s_t RPOS2_DIV = 128; /* normalize dividing */
constexpr v1s_t RPOS2_MID = 0x0; /* mm of parent */
constexpr v1s_t RPOS2_MIN = -64; /* lb of parent */
constexpr v1s_t RPOS2_MAX = +64; /* rt of parent */

constexpr v1s_t PIVOT_DIV = 128; /* normalize dividing */
constexpr v1s_t PIVOT_MID = 0x0; /* mm of itself */
constexpr v1s_t PIVOT_MIN = -64; /* lb of itself */
constexpr v1s_t PIVOT_MAX = +64; /* rt of itself */

} /* consdef */

namespace geom { /* typedef */

typedef struct apos2_t {
    v1s_t x = 0, y = 0;
} apos2_t, com_apos2_t; /* absolute position 2d */
typedef struct rpos2_t {
    v1s_t x = RPOS2_MID, y = RPOS2_MID;
} rpos2_t, com_rpos2_t; /* relative position 2d */
typedef struct zpos1_t {
    v1s_t z = 0;
} zpos1_t, com_zpos1_t; /* depth */
typedef struct gpos3_t {
    v1s_t x = 0, y = 0, z = 0;
} gpos3_t, com_gpos3_t; /* global position 3d */
typedef struct fpos2_t {
    v1s_t x = 0, y = 0;
} fpos2_t, com_fpos2_t; /* family position */

typedef struct pivot_t {
    v1s_t x = PIVOT_MID, y = PIVOT_MID;
} pivot_t, com_pivot_t; /* pivot point - the centre of a quad */

} /* typedef */

} /* content */

#endif/*SOMIGAME_GEOM_COORD_HXX*/
