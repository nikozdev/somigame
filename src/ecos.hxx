#ifndef SOMIGAME_ECOS_HXX
#define SOMIGAME_ECOS_HXX 1

#include "head.hxx"

/* headers */

//#include "ecos/core.hxx"

/* defines */

/* content */

namespace somigame { namespace ecos { /* datadef */

extern ecs_t ecs;

} } /* datadef */

namespace somigame { namespace ecos { /* typedef */

/* family component
 * this is the base for the hierarchy of entities
*/
typedef struct family_t {
    ent_t ancestor = entt::null;
    ent_t follower = entt::null;
    ent_t siblingl = entt::null;
    ent_t siblingr = entt::null;
} family_t, com_family_t;

/* update component
 * the signal bound to updates of a specific component type
*/
template<typename com_t>
struct com_update_t
{
public: /* typedef */
    using holder_t = sigholder_t<void(ecs_t&, ent_t)>;
    using binder_t = sigbinder_t<holder_t>;
public: /* codetor */
    com_update_t() : entity(entt::to_entity(ecs, *this))
    {
        ecs.on_update<com_t>().template connect
            <&com_update_t::update>(*this);
    }
    ~com_update_t()
    {
        ecs.on_update<com_t>().template disconnect
            <&com_update_t::update>(*this);
    }
public: /* actions */
    void update(ecs_t&ecs, ent_t ent)
    {
        if (this->entity == ent)
        { this->holder.publish(ecs, ent); }
    }
public: /* datadef */
    holder_t holder;
    binder_t binder{holder};
private:
    entity_t entity;
}; /* templated signal */

} } /* typedef */

namespace somigame { namespace ecos { /* actions */

extern void init();
extern void quit();

template<typename func_t, typename...args_t>
void call_for_ancestor(ent_t ent, func_t func, args_t&&...args)
{
    if (ecs.valid(ent) == FALSE) { return; }
    if (ecs.any_of<com_family_t>(ent) == FALSE) { return; }
    auto*family_e =&ecs.get<com_family_t>(ent);
    if (ecs.valid(family_e->ancestor))
    { func(family_e->ancestor, std::forward<args_t>(args)...); }
}
template<typename func_t, typename...args_t>
void call_for_ancestry(ent_t ent, func_t func, args_t&&...args)
{
    if (ecs.valid(ent) == FALSE) { return; }
    if (ecs.any_of<com_family_t>(ent) == FALSE) { return; }
    auto ancestor = ent;
    auto*family_a =&ecs.get<com_family_t>(ancestor);
    while (ecs.valid(family_a->ancestor))
    {
        ancestor = family_a->ancestor;
        family_a =&ecs.get<com_family_t>(ancestor);
        func(ancestor, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void call_for_children(ent_t ent, func_t func, args_t&&...args)
{
    if (ecs.valid(ent) == FALSE) { return; }
    if (ecs.any_of<com_family_t>(ent) == FALSE) { return; }
    auto*family_e =&ecs.get<com_family_t>(ent);
    auto follower = family_e->follower;
    if (ecs.valid(follower))
    {
        func(follower, std::forward<args_t>(args)...);
        auto siblingl = follower;
        auto*family_l =&ecs.get<com_family_t>(siblingl);
        while (ecs.valid(family_l->siblingl))
        {
            siblingl = family_l->siblingl;
            family_l =&ecs.get<com_family_t>(siblingl);
            func(siblingl, std::forward<args_t>(args)...);
        }
        auto siblingr = follower;
        auto*family_r =&ecs.get<com_family_t>(siblingr);
        while (ecs.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r =&ecs.get<com_family_t>(siblingr);
            func(siblingr, std::forward<args_t>(args)...);
        }
    }
}
template<typename func_t, typename...args_t>
void call_for_inherit(ent_t ent, func_t func, args_t&&...args)
{
    if (ecs.valid(ent) == FALSE) { return; }
    if (ecs.any_of<com_family_t>(ent) == FALSE) { return; }
    auto follower = ent;
    auto*family_f =&ecs.get<com_family_t>(follower);
    while (ecs.valid(family_f->follower))
    {
        follower = family_f->follower;
        family_f = &ecs.get<com_family_t>(follower);
        func(follower, std::forward<args_t>(args)...);
        auto siblingl = follower;
        auto*family_l =&ecs.get<com_family_t>(siblingl);
        while (ecs.valid(family_l->siblingl))
        {
            siblingl = family_l->siblingl;
            family_l =&ecs.get<com_family_t>(siblingl);
            func(siblingl, std::forward<args_t>(args)...);
        }
        auto siblingr = follower;
        auto*family_r =&ecs.get<com_family_t>(siblingr);
        while (ecs.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r =&ecs.get<com_family_t>(siblingr);
            func(siblingr, std::forward<args_t>(args)...);
        }
    }
}
template<typename func_t, typename...args_t>
void call_for_siblingl(ent_t ent, func_t func, args_t&&...args)
{
    if (ecs.valid(ent) == FALSE) { return; }
    if (ecs.any_of<com_family_t>(ent) == FALSE) { return; }
    auto siblingl = ent;
    auto*family_l =&ecs.get<com_family_t>(siblingl);
    while (ecs.valid(family_l->siblingr))
    {
        siblingl = family_l->siblingr;
        family_l =&ecs.get<com_family_t>(siblingl);
        func(siblingl, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void call_for_siblingr(ent_t ent, func_t func, args_t&&...args)
{
    if (ecs.valid(ent) == FALSE) { return; }
    if (ecs.any_of<com_family_t>(ent) == FALSE) { return; }
    auto siblingr = ent;
    auto*family_r =&ecs.get<com_family_t>(siblingr);
    while (ecs.valid(family_r->siblingr))
    {
        siblingr = family_r->siblingr;
        family_r =&ecs.get<com_family_t>(siblingr);
        func(siblingr, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void call_for_siblings(ent_t ent, func_t func, args_t&&...args)
{
    if (ecs.valid(ent) == FALSE) { return; }
    if (ecs.any_of<com_family_t>(ent) == FALSE) { return; }
    auto siblingl = ent;
    auto*family_l =&ecs.get<com_family_t>(siblingl);
    while (ecs.valid(family_l->siblingl))
    {
        siblingl = family_l->siblingr;
        family_l =&ecs.get<com_family_t>(siblingl);
        func(siblingl, std::forward<args_t>(args)...);
    }
    auto siblingr = ent;
    auto*family_r =&ecs.get<com_family_t>(siblingr);
    while (ecs.valid(family_r->siblingr))
    {
        siblingr = family_r->siblingr;
        family_r =&ecs.get<com_family_t>(siblingr);
        func(siblingr, std::forward<args_t>(args)...);
    }
}

} } /* actions */

namespace somigame { namespace ecos { /* getters */

extern ent_t get_ancestor(ent_t ent);
extern ent_t get_follower(ent_t ent);
extern ent_t get_siblingl(ent_t ent);
extern ent_t get_siblingr(ent_t ent);

} } /* getters */

namespace somigame { namespace ecos { /* vetters */

extern bool vet_ancestry(ent_t ent, ent_t ancestry);
extern bool vet_ancestor(ent_t ent, ent_t ancestor = entt::null);

extern bool vet_follower(ent_t ent, ent_t follower = entt::null);
extern bool vet_children(ent_t ent, ent_t children = entt::null);
extern bool vet_inherits(ent_t ent, ent_t inherits = entt::null);

extern bool vet_siblings(ent_t ent, ent_t siblings);
extern bool vet_siblings(ent_t ent, ent_t siblingl, ent_t siblingr);
extern bool vet_siblingl(ent_t ent, ent_t siblingl = entt::null);
extern bool vet_siblingr(ent_t ent, ent_t siblingr = entt::null);

} } /* vetters */

namespace somigame { namespace ecos { /* setters */

extern bool set_ancestor(ent_t ent, ent_t ancestor = entt::null);
extern bool set_follower(ent_t ent, ent_t follower = entt::null);

} } /* setters */

#endif/*SOMIGAME_ECOS_HXX*/
