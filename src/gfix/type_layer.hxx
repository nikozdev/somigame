#ifndef SOMIGAME_GFIX_TYPE_LAYER_HXX
#define SOMIGAME_GFIX_TYPE_LAYER_HXX

#include "../head.hxx"

namespace somigame { namespace gfix {

// typedef

typedef struct rlayer_t {
    index_t layer = 1;
} rlayer_t, com_rlayer_t; /* relative layer */
typedef struct glayer_t {
    index_t layer = 1;
} glayer_t, com_glayer_t; /* global layer */

// actions

_DECL_FUNC v1b_t init_type_layer();
_DECL_FUNC v1b_t quit_type_layer();

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_LAYER_HXX
