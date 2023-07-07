#ifndef SOMIGAME_GFIX_TYPE_LABEL_CXX
#define SOMIGAME_GFIX_TYPE_LABEL_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../gfix/type_label.hxx"

namespace somigame { namespace gfix {

// actions

_DEFN_FUNC v1b_t init_type_label()
{
    return TRUTH;
} // init_type_label
_DEFN_FUNC v1b_t quit_type_label()
{
    return TRUTH;
} // quit_type_label

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_LABEL_CXX
