#ifndef SOMIGAME_MAIN_UNIT_WINDOW_CXX
#define SOMIGAME_MAIN_UNIT_WINDOW_CXX

#include "../head.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_sizes.hxx"
#include "../gfix/unit_camera.hxx"
#include "../main/unit_window.hxx"

namespace somigame { namespace main {

// datadef

static window_t window;

// signals

_DEFN_DATA signal_t<void(const window_t&)>window_sized_start_signal;
_DEFN_DATA signal_t<void(const window_t&)>window_sized_final_signal;

// getters

_DEFN_FUNC const main::window_t&get_window()
{ return main::window; }

// actions

_DEFN_FUNC bool init_unit_window()
{
    window.ratio = { .x = gfix::CAMERA_RATIO_X, .y = gfix::CAMERA_RATIO_Y };
    window.aposi = { .x = 0, .y = 0 };
    window.asize = { .x = gfix::CAMERA_ASIZE_X, .y = gfix::CAMERA_ASIZE_Y };
    ::glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    ::glutInitWindowPosition(window.aposi.x, window.aposi.y);
    ::glutInitWindowSize(window.asize.x, window.asize.y);
    ::glutCreateWindow(_NAME_STR);
    ::glutReshapeFunc([](int sizex, int sizey) {
        main::window_sized_start_signal.holder.publish(main::get_window());
        auto&aposi = window.aposi;
        auto&asize = window.asize;
        auto ratx = window.ratio.x;
        auto raty = window.ratio.y;
        auto relw = sizex / ratx;
        auto relh = sizey / raty;
        auto reld = relw - relh;
        if (reld == 0) { aposi = { 0, 0 }; }
        else if (reld < 0)
        {
            aposi = { 0, -reld * (raty >> 1) };
            sizey -= (aposi.y << 1);
        }
        else if (reld > 0)
        {
            aposi = { +reld * (ratx >> 1), 0 };
            sizex -= (aposi.x << 1);
        }
        asize = { sizex, sizey };
        ::glViewport( aposi.x,aposi.y, asize.x,asize.y );
        main::window_sized_final_signal.holder.publish(main::get_window());
    });
    return TRUTH;
} // init_unit_window
_DEFN_FUNC bool quit_unit_window()
{
    window.ratio = { .x = gfix::CAMERA_RATIO_X, .y = gfix::CAMERA_RATIO_Y };
    window.aposi = { .x = 0, .y = 0 };
    window.asize = { .x = gfix::CAMERA_ASIZE_X, .y = gfix::CAMERA_ASIZE_Y };
    return TRUTH;
}

} } // namespace somigame { namespace main {

#endif//SOMIGAME_MAIN_UNIT_WINDOW_CXX
