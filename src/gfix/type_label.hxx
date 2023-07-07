#ifndef SOMIGAME_GFIX_TYPE_LABEL_HXX
#define SOMIGAME_GFIX_TYPE_LABEL_HXX

#include "../head.hxx"

namespace somigame { namespace gfix {

// typedef

typedef struct label_t {
    sdstr_t string;
} label_t, com_label_t; // text label

// actions

_DECL_FUNC v1b_t init_type_label();
_DECL_FUNC v1b_t quit_type_label();

} } // namespace somigame { namespace gfix {

#endif/*SOMIGAME_GFIX_TYPE_LABEL_HXX*/
