#ifndef SOMIGAME_HEAD_HXX
#define SOMIGAME_HEAD_HXX 1

/* headers */

#include "../lib/libasist/src/main.hxx"
#include "../lib/stbl/src/stbi.hxx"
#include "../lib/entt/src/all.hpp"

/* external */

#include <GL/glut.h> /* TODO: this is a bad thing for winos */

/* defines */

#ifndef SOMIGAME_NAME
#   define SOMIGAME_NAME somigame
#endif/*SOMIGAME_NAME*/
#ifndef SOMIGAME_NAME_STR
#   define SOMIGAME_NAME_STR _GET_STR(SOMIGAME_NAME)
#endif/*SOMIGAME_NAME_STR*/

#ifndef SOMIGAME_VNUM
#   define SOMIGAME_VNUM 0xa0a3a0
#endif/*SOMIGAME_VNUM*/
#ifndef SOMIGAME_VNUM_STR
#   define SOMIGAME_VNUM_STR _GET_STR(SOMIGAME_VNUM)
#endif/*SOMIGAME_VNUM_STR*/

/* content */

namespace somigame { /* imports */

using namespace libasist;

} /* imports */

namespace somigame { /** consdef **/

constexpr v1s_t UNIT_ASIZE_X = 0x010; /* screen ratio width unit */
constexpr v1s_t UNIT_ASIZE_Y = 0x010; /* screen ratio height unit */

} /* consdef */

namespace somigame { /* typedef */

/* entt */

typedef entt::registry ecs_t;
typedef entt::entity ent_t;
typedef ent_t entity_t;

template<typename t_iface_t>
using delegator_t = entt::delegate<t_iface_t>;

template<typename funcsign_t>
using sigholder_t = entt::sigh<funcsign_t>;
template<typename holder_t>
using sigbinder_t = entt::sink<holder_t>;
template<typename funcsign_t>
struct signal_t
{
public: /* typedef */
    using sigholder_t = sigholder_t<funcsign_t>;
    using sigbinder_t = sigbinder_t<sigholder_t>;
public: /* datadef */
    sigholder_t holder;
    sigbinder_t binder{holder};
}; /* templated signal */

template<typename t_event_t>
using t_evemitter_t = entt::emitter<t_event_t>;

using dispatcher_t = entt::dispatcher;

} /* typedef */

#endif/*SOMIGAME_HEAD_HXX*/
