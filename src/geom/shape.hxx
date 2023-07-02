#ifndef SOMIGAME_GEOM_SHAPE_HXX
#define SOMIGAME_GEOM_SHAPE_HXX 1

#include "../head.hxx"

/* headers */

#include "sizes.hxx"
#include "coord.hxx"
#include "direc.hxx"

namespace somigame { /* content */

namespace geom { /* consdef */

} /* consdef */

namespace geom { /* typedef */

typedef struct grect_t {
    struct x_t {
        v1s_t l = 0, r = 0;
    } x;
    struct y_t {
        v1s_t b = 0, t = 0;
    } y;
} grect_t, com_grect_t; /* global rectangle */
typedef struct frect_t {
    struct x_t {
        v1s_t l = 0, r = 0;
    } x;
    struct y_t {
        v1s_t b = 0, t = 0;
    } y;
} frect_t; /* family rectangle */

typedef struct lgrid_t {
    asize_t cells{
        .x = UNIT_ASIZE_X,
        .y = UNIT_ASIZE_Y,
    }; /* sizes of cells */
    asize_t tails{
        .x = 0,
        .y = 0
    }; /* trailing lines from both sides */
    asize_t lsize = {
        .x = 1,
        .y = 1,
    }; /* lines sizes in both axes */
} lgrid_t, com_lgrid_t; /* line grid */

} /* typedef */

} /* content */

#endif/*SOMIGAME_GEOM_SHAPE_HXX*/
