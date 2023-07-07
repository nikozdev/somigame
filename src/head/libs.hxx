#ifndef SOMIGAME_HEAD_LIBS_HXX
#define SOMIGAME_HEAD_LIBS_HXX

#include "../../lib/libasist/src/main.hxx"
#include "../../lib/stbl/src/stbi.hxx"
#include "../../lib/entt/src/all.hpp"

// TODO: glut is a bad thing for winos and us
#include <GL/glut.h>

namespace somigame {

using namespace libasist;

template<typename t_iface_t>
using delegator_t = entt::delegate<t_iface_t>;

template<typename delegator_t>
using sigholder_t = entt::sigh<delegator_t>;
template<typename sigholder_t>
using sigbinder_t = entt::sink<sigholder_t>;
template<typename delegator_t>
struct signal_t
{
public: // typedef
    using sigholder_t = sigholder_t<delegator_t>;
    using sigbinder_t = sigbinder_t<sigholder_t>;
public: /* datadef */
    sigholder_t holder;
    sigbinder_t binder{holder};
}; // templated signal

template<typename t_event_t>
using evemitter_t = entt::emitter<t_event_t>;

using dispatcher_t = entt::dispatcher;

} // namespace somigame {

#endif//SOMIGAME_HEAD_LIBS_HXX
