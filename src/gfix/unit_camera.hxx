#ifndef SOMIGAME_GFIX_UNIT_CAMERA_HXX
#define SOMIGAME_GFIX_UNIT_CAMERA_HXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"

namespace somigame { namespace gfix {

// consdef

constexpr auto UNIT_ASIZE_X = 0x10; /* screen ratio width unit */
constexpr auto UNIT_ASIZE_Y = 0x10; /* screen ratio height unit */

constexpr auto CAMERA_RATIO_X = 0x10;
constexpr auto CAMERA_RATIO_Y = 0x09;

constexpr auto CAMERA_ASIZE_X = UNIT_ASIZE_X * CAMERA_RATIO_X * 2;
constexpr auto CAMERA_ASIZE_Y = UNIT_ASIZE_Y * CAMERA_RATIO_Y * 2;

// getters

_DECL_FUNC ecos::ent_t get_camera_ent();

// actions

_DECL_FUNC v1b_t init_unit_camera();
_DECL_FUNC v1b_t quit_unit_camera();

} } // namespac somigame { namespace gfix {

#endif//SOMIGAME_GFIX_UNIT_CAMERA_HXX
