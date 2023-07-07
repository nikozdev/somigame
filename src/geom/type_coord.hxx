#ifndef SOMIGAME_GEOM_TYPE_COORD_HXX
#define SOMIGAME_GEOM_TYPE_COORD_HXX

#include "../head.hxx"

namespace somigame { namespace geom {

// consdef

constexpr auto RPOSI_DIV = 128; /* normalize dividing */
constexpr auto RPOSI_MID = 0x0; /* mm of parent */
constexpr auto RPOSI_MIN = -64; /* lb of parent */
constexpr auto RPOSI_MAX = +64; /* rt of parent */

constexpr auto PIVOT_DIV = 128; /* normalize dividing */
constexpr auto PIVOT_MID = 0x0; /* mm of itself */
constexpr auto PIVOT_MIN = -64; /* lb of itself */
constexpr auto PIVOT_MAX = +64; /* rt of itself */

// typedef

typedef struct aposi_t {
    v1s_t x = 0, y = 0;
} aposi_t, com_aposi_t; // absolute position
typedef struct aplim_t {
    struct x_t {
        v1s_t l = std::numeric_limits<v1s_t>::min();
        v1s_t r = std::numeric_limits<v1s_t>::max();
    } x;
    struct y_t {
        v1s_t b = std::numeric_limits<v1s_t>::min();
        v1s_t t = std::numeric_limits<v1s_t>::max();
    } y;
} aplim_t, com_aplim_t; // absolute position limit
typedef struct rposi_t {
    v1s_t x = RPOSI_MID, y = RPOSI_MID;
} rposi_t, com_rposi_t; // relative position
typedef struct rplim_t {
    struct x_t {
        v1s_t l = RPOSI_MIN;
        v1s_t r = RPOSI_MAX;
    } x;
    struct y_t {
        v1s_t b = RPOSI_MIN;
        v1s_t t = RPOSI_MAX;
    } y;
} rplim_t, com_rplim_t; // relative position limit
typedef struct gposi_t {
    v1s_t x = 0, y = 0;
} gposi_t, com_gposi_t; // global position
typedef struct gplim_t {
    struct x_t {
        v1s_t l = std::numeric_limits<v1s_t>::min();
        v1s_t r = std::numeric_limits<v1s_t>::max();
    } x;
    struct y_t {
        v1s_t b = std::numeric_limits<v1s_t>::min();
        v1s_t t = std::numeric_limits<v1s_t>::max();
    } y;
} gplim_t, com_gplim_t; // global position limit

typedef struct pivot_t {
    v1s_t x = PIVOT_MID, y = PIVOT_MID;
} pivot_t, com_pivot_t; /* pivot point - offset from the centre */

// actions

_DECL_FUNC v1b_t init_type_coord();
_DECL_FUNC v1b_t quit_type_coord();

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_TYPE_COORD_HXX
