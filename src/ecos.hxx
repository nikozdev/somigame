#pragma once

/* headers */

#include "head.hxx"
#include "util.hxx"
#include "gfix.hxx"
#include "game.hxx"

/* defines */

/* content */

_NAMESPACE_ENTER

/** forward **/

typedef entt::registry ecos_t;
typedef entt::entity ent_t;
typedef ent_t entity_t;

/** datadef**/

extern ecos_t ecos;

/** typedef **/

/*** classes ***/

class animator_t
{
public: /* typedef */
    using this_t = animator_t;
    using len_t = msize_t;
    class a_step_t
    {
    public: /* typedef */
        using this_t = a_step_t;
    public: /* codetor */
        a_step_t(len_t since, len_t until) :
            since(since), until(until) {}
        virtual ~a_step_t() = default;
    public: /* actions */
        virtual bool play(len_t pas) = 0;
    public: /* datadef */
        const len_t since;
        const len_t until;
    }; /* a_step_t */
    template<typename t_com_t, typename t_mem_t, typename...t_paf_t>
    class t_step_t : public a_step_t
    {
    public: /* typedef */
        using this_t = t_step_t<t_com_t, t_mem_t, t_paf_t...>;
        using com_t = t_com_t;
        using mem_t = t_mem_t;
    public: /* codetor */
        t_step_t(len_t since, len_t until, t_mem_t tar, ent_t ent, t_paf_t&&...paf) :
            a_step_t(since, until),
            ent(ent), paf{paf...},
            was(get_member(
                ecos.get<t_com_t>(ent),
                std::forward<t_paf_t>(paf)...
            )), tar(tar)
        { }
    public: /* actions */
        virtual bool play(len_t pas) override
        {
            if (!ecos.valid(this->ent)) { return _FALSE; }
            if (auto*com = ecos.try_get<t_com_t>(this->ent))
            {
                if (pas >= since && pas <= until)
                {
                    auto&mem = get_member(*com, this->paf);
                    mem = was + ((tar-was) * (pas-since) / (until-since));
                    return _TRUTH;
                }
                else { return _FALSE; }
                return _TRUTH;
            }
            else
            { return _FALSE; }
        }
    private: /* datadef */
        ent_t ent;
        const std::tuple<t_paf_t...> paf;
    public: /* datadef */
        const mem_t was;
        const mem_t tar;
    }; /* t_step_t */
public: /* codetor */
    animator_t(ent_t ent) :
        ent(ent), start{ timer.now_mil }, step_array{}
    { }
public: /* actions */
    template<typename t_com_t, typename t_tar_t, typename...t_paf_t>
    bool_t insert(len_t since, len_t until, t_tar_t tar, t_paf_t&&...paf)
    {
        auto*step = new t_step_t<t_com_t, t_tar_t, t_paf_t...>(
            since, until, tar, this->ent, std::forward<t_paf_t>(paf)...
        );
        step_array.push_back(step);
        return _TRUTH;
    }
    void proc()
    {
        auto done = 0;
        for (auto*step : this->step_array)
        {
            auto diff = timer.now_mil - this->start;
            done += diff > step->until;
            step->play(diff);
        }
        if (done)
        {
            timer.dispatcher.sink<update_event_t>().disconnect<&this_t::proc>(*this);
            this->start = timer.now_mil;
        }
    }
    bool_t play()
    {
        this->start = timer.now_mil;
        timer.dispatcher.sink<update_event_t>().connect<&this_t::proc>(*this);
        return _TRUTH;
    }
public: /* datadef */
    ent_t ent;
    len_t start;
    t_array_t<a_step_t*>step_array;
}; /* animator_t */

/*** structs ***/

typedef struct family_t {
    ent_t ancestor = entt::null;
    ent_t follower = entt::null;
    ent_t siblingl = entt::null;
    ent_t siblingr = entt::null;
} family_t, com_family_t;

using com_strbuf_t = strbuf_t;

using com_clock_t = clock_t;

template<typename t_com_t>
struct com_update_t
{
public: /* typedef */
    using holder_t = t_sigholder_t<void(ecos_t&, ent_t)>;
    using binder_t = t_sigbinder_t<holder_t>;
public: /* codetor */
    com_update_t() : entity(entt::to_entity(ecos, *this))
    {
        ecos.on_update<t_com_t>().template connect
            <&com_update_t::update>(*this);
    }
    ~com_update_t()
    {
        ecos.on_update<t_com_t>().template disconnect
            <&com_update_t::update>(*this);
    }
public: /* actions */
    void update(ecos_t&ecos, ent_t ent)
    {
        if (this->entity == ent)
        { this->holder.publish(ecos, ent); }
    }
public: /* datadef */
    holder_t holder;
    binder_t binder{holder};
private:
    entity_t entity;
}; /* templated signal */

/**** names ****/

using com_iname_t = iname_t;
using com_sname_t = sname_t;
using com_ename_t = ename_t;

/**** sizes ****/

using com_asize_t = asize_t;
using com_rsize_t = rsize_t;
using com_gsize_t = gsize_t;
using com_tsize_t = tsize_t;
using com_isize_t = isize_t;

using com_ratio_t = ratio_t;

/**** coord ****/

using com_apos2_t = apos2_t;
using com_rpos2_t = rpos2_t;
using com_zpos1_t = zpos1_t;
using com_gpos3_t = gpos3_t;
using com_tpos3_t = tpos3_t;
using com_ipos2_t = ipos2_t;

using com_pivot_t = pivot_t;

/**** geometry ****/

using com_direc_t = direc_t;

using com_grect_t = grect_t;
using com_trect_t = trect_t;
using com_irect_t = irect_t;

/**** visual ****/

using com_visual_t= visual_t;
using com_rlayer_t= rlayer_t;
using com_glayer_t= glayer_t;
using com_vrange_t= vrange_t;

using com_color_t = color_t;
using com_imreg_t = imreg_t;
using com_faces_t = faces_t;
using com_fonts_t = fonts_t;

using com_lgrid_t = lgrid_t;

/**** logic ****/

using com_actor_t = actor_t;
using com_alive_t = alive_t;
using com_mover_t = mover_t;

/**** tilegrid ****/

using com_tcell_t = tcell_t;

/** actions **/

extern void ecos_init();
extern void ecos_quit();

template<typename func_t, typename...args_t>
void call_for_ancestor(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto*family_e =&ecos.get<com_family_t>(ent);
    if (ecos.valid(family_e->ancestor))
    { func(family_e->ancestor, std::forward<args_t>(args)...); }
}
template<typename func_t, typename...args_t>
void call_for_ancestry(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto ancestor = ent;
    auto*family_a =&ecos.get<com_family_t>(ancestor);
    while (ecos.valid(family_a->ancestor))
    {
        ancestor = family_a->ancestor;
        family_a =&ecos.get<com_family_t>(ancestor);
        func(ancestor, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void call_for_children(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto*family_e =&ecos.get<com_family_t>(ent);
    auto follower = family_e->follower;
    if (ecos.valid(follower))
    {
        func(follower, std::forward<args_t>(args)...);
        auto siblingl = follower;
        auto*family_l =&ecos.get<com_family_t>(siblingl);
        while (ecos.valid(family_l->siblingl))
        {
            siblingl = family_l->siblingl;
            family_l =&ecos.get<com_family_t>(siblingl);
            func(siblingl, std::forward<args_t>(args)...);
        }
        auto siblingr = follower;
        auto*family_r =&ecos.get<com_family_t>(siblingr);
        while (ecos.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r =&ecos.get<com_family_t>(siblingr);
            func(siblingr, std::forward<args_t>(args)...);
        }
    }
}
template<typename func_t, typename...args_t>
void call_for_inherit(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto follower = ent;
    auto*family_f =&ecos.get<com_family_t>(follower);
    while (ecos.valid(family_f->follower))
    {
        follower = family_f->follower;
        family_f = &ecos.get<com_family_t>(follower);
        func(follower, std::forward<args_t>(args)...);
        auto siblingl = follower;
        auto*family_l =&ecos.get<com_family_t>(siblingl);
        while (ecos.valid(family_l->siblingl))
        {
            siblingl = family_l->siblingl;
            family_l =&ecos.get<com_family_t>(siblingl);
            func(siblingl, std::forward<args_t>(args)...);
        }
        auto siblingr = follower;
        auto*family_r =&ecos.get<com_family_t>(siblingr);
        while (ecos.valid(family_r->siblingr))
        {
            siblingr = family_r->siblingr;
            family_r =&ecos.get<com_family_t>(siblingr);
            func(siblingr, std::forward<args_t>(args)...);
        }
    }
}
template<typename func_t, typename...args_t>
void call_for_siblingl(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto siblingl = ent;
    auto*family_l =&ecos.get<com_family_t>(siblingl);
    while (ecos.valid(family_l->siblingr))
    {
        siblingl = family_l->siblingr;
        family_l =&ecos.get<com_family_t>(siblingl);
        func(siblingl, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void call_for_siblingr(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto siblingr = ent;
    auto*family_r =&ecos.get<com_family_t>(siblingr);
    while (ecos.valid(family_r->siblingr))
    {
        siblingr = family_r->siblingr;
        family_r =&ecos.get<com_family_t>(siblingr);
        func(siblingr, std::forward<args_t>(args)...);
    }
}
template<typename func_t, typename...args_t>
void call_for_siblings(ent_t ent, func_t func, args_t&&...args)
{
    if (ecos.valid(ent) == _FALSE) { return; }
    if (ecos.any_of<com_family_t>(ent) == _FALSE) { return; }
    auto siblingl = ent;
    auto*family_l =&ecos.get<com_family_t>(siblingl);
    while (ecos.valid(family_l->siblingl))
    {
        siblingl = family_l->siblingr;
        family_l =&ecos.get<com_family_t>(siblingl);
        func(siblingl, std::forward<args_t>(args)...);
    }
    auto siblingr = ent;
    auto*family_r =&ecos.get<com_family_t>(siblingr);
    while (ecos.valid(family_r->siblingr))
    {
        siblingr = family_r->siblingr;
        family_r =&ecos.get<com_family_t>(siblingr);
        func(siblingr, std::forward<args_t>(args)...);
    }
}

/*** getters ***/

extern ent_t get_ancestor(ent_t ent);
extern ent_t get_follower(ent_t ent);
extern ent_t get_siblingl(ent_t ent);
extern ent_t get_siblingr(ent_t ent);

/*** vetters ***/

extern bool vet_ancestry(ent_t ent, ent_t ancestry);
extern bool vet_ancestor(ent_t ent, ent_t ancestor = entt::null);

extern bool vet_follower(ent_t ent, ent_t follower = entt::null);
extern bool vet_children(ent_t ent, ent_t children = entt::null);
extern bool vet_inherits(ent_t ent, ent_t inherits = entt::null);

extern bool vet_siblings(ent_t ent, ent_t siblings);
extern bool vet_siblings(ent_t ent, ent_t siblingl, ent_t siblingr);
extern bool vet_siblingl(ent_t ent, ent_t siblingl = entt::null);
extern bool vet_siblingr(ent_t ent, ent_t siblingr = entt::null);

/*** setters ***/

extern bool set_ancestor(ent_t ent, ent_t ancestor = entt::null);
extern bool set_follower(ent_t ent, ent_t follower = entt::null);

_NAMESPACE_LEAVE
