#ifndef SOMIGAME_GEOM_TYPE_DIREC_HXX
#define SOMIGAME_GEOM_TYPE_DIREC_HXX

#include "../head.hxx"

/* headers */

namespace somigame { namespace geom {

// typedef

typedef struct direc_t {
    v1s_t x = 0, y = 1;
} direc_t, com_direc_t; // direction vector

} } // namespace somigame { namespace geom {

#endif//SOMIGAME_GEOM_TYPE_DIREC_HXX
