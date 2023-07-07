#ifndef SOMIGAME_GFIX_TYPE_GUIMAN_HXX
#define SOMIGAME_GFIX_TYPE_GUIMAN_HXX

#include "../head.hxx"
#include "../ecos.hxx"
#include "../geom/type_sizes.hxx"
#include "../gfix/unit_camera.hxx"

namespace somigame { namespace gfix {

// consdef

constexpr auto GUIS_ASIZE_X = UNIT_ASIZE_X * 2;
constexpr auto GUIS_ASIZE_Y = UNIT_ASIZE_Y;
constexpr auto GUIS_RSIZE_X = geom::RSIZE_MAX * 4 / CAMERA_RATIO_X;
constexpr auto GUIS_RSIZE_Y = geom::RSIZE_MAX / CAMERA_RATIO_Y;
constexpr auto GUIS_LAYER = 0xff;

// getters

_DECL_FUNC ecos::ent_t get_guiman_ent();

// actions

_DECL_FUNC v1b_t init_unit_guiman();
_DECL_FUNC v1b_t quit_unit_guiman();

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_GUIMAN_HXX
