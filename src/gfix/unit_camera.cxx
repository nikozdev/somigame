#ifndef SOMIGAME_GFIX_UNIT_CAMERA_CXX
#define SOMIGAME_GFIX_UNIT_CAMERA_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../ecos/type_update.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_rects.hxx"
#include "../geom/type_direc.hxx"
#include "../gfix/type_layer.hxx"
#include "../gfix/type_drawn.hxx"
#include "../gfix/unit_camera.hxx"
#include "../game/type_sizes.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_tcell.hxx"

namespace somigame { namespace gfix {

// datadef

static ecos::ent_t camera_ent = entt::null;

// getters

_DEFN_FUNC ecos::ent_t get_camera_ent()
{ return camera_ent; }

// actions

static void on_proc_gposi(ecos::reg_t&reg, ecos::ent_t ent)
{
}
static void on_proc_grect(ecos::reg_t&reg, ecos::ent_t ent)
{
    auto&grect = reg.get<geom::com_grect_t>(ent);
    ::glMatrixMode(GL_PROJECTION);
    ::glLoadIdentity();
    ::gluOrtho2D(
        grect.x.l, grect.x.r,
        grect.y.b, grect.y.t
    );
}

_DEFN_FUNC v1b_t init_unit_camera()
{
    _EFNOT(
        ecos::reg.valid(get_camera_ent()) == FALSE,
        return FALSE,
        "camera init was called twice !? [camera_ent]={:d}",
        get_camera_ent()
    );
    // entity
    camera_ent = ecos::reg.create();
    // sizes
    ecos::reg.emplace<geom::com_asize_t>(camera_ent, geom::asize_t{
        .x = gfix::CAMERA_ASIZE_X,
        .y = gfix::CAMERA_ASIZE_Y,
    });
    ecos::reg.emplace<geom::com_ratio_t>(camera_ent);
    // coord
    ecos::reg.emplace<geom::com_aposi_t>(camera_ent);
    ecos::reg.emplace<geom::com_aplim_t>(camera_ent, geom::aplim_t{
        {
            -game::TGRID_AHALF_X + gfix::CAMERA_ASIZE_X/2,
            +game::TGRID_AHALF_X - gfix::CAMERA_ASIZE_X/2,
        },
        {
            -game::TGRID_AHALF_Y + gfix::CAMERA_ASIZE_Y/2,
            +game::TGRID_AHALF_Y - gfix::CAMERA_ASIZE_Y/2,
        },
    });
    // direc
    ecos::reg.emplace<geom::com_direc_t>(camera_ent, -0, +1);
    // signal
    ecos::reg.emplace<ecos::com_update_t<geom::com_gposi_t>>(camera_ent)
        .binder.connect<&on_proc_gposi>();
    ecos::reg.emplace<ecos::com_update_t<geom::com_grect_t>>(camera_ent)
        .binder.connect<&on_proc_grect>();
    // family
    ecos::reg.emplace<ecos::com_family_t>(camera_ent);
    // final
    return TRUTH;
}
_DEFN_FUNC v1b_t quit_unit_camera()
{
    _EFNOT(
        ecos::reg.valid(get_camera_ent()) == TRUTH,
        return FALSE,
        "camera quit was called twice !? [camera_ent]={:d}",
        get_camera_ent()
    );
    /* entity */
    camera_ent = entt::null;
    /* final */
    return TRUTH;
} // quit_unit_camera

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_UNIT_CAMERA_CXX
