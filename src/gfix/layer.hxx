#ifndef SOMIGAME_GFIX_LAYER_HXX
#define SOMIGAME_GFIX_LAYER_HXX 1

#include "../head.hxx"

namespace somigame { /* content */

namespace gfix { /* enumdef */

} /* enumdef */

namespace gfix { /* typedef */

typedef struct rlayer_t {
    index_t layer = 1;
} rlayer_t, com_rlayer_t; /* relative layer */
typedef struct glayer_t {
    index_t layer = 1;
} glayer_t, com_glayer_t; /* global layer */

} /* typedef */

} /* content */

#endif/*SOMIGAME_GFIX_LAYER_HXX*/
