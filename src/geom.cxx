#ifndef SOMIGAME_GEOM_CXX
#define SOMIGAME_GEOM_CXX 1

#include "head.hxx"

/* headers */

#include "geom.hxx"
#include "geom/sizes.hxx"
#include "geom/coord.hxx"
#include "geom/shape.hxx"
#include "ecos.hxx"
#include "gfix.hxx"
#include "gfix/visual.hxx"
#include "gfix/image.hxx"
#include "gfix/layer.hxx"
#include "game.hxx"
#include "game/tile.hxx"

namespace somigame { /* content */

namespace geom { /* typedef */

struct geom_listener_t
{
    /* geometry */
    void update_grect_trect(ecs_t&ecs, ent_t ent)
    {
        if (ecs.valid(ent) == FALSE) { return; }
        /* sizes */
        gsize_t gsize_e = { 0, 0 };
        if (auto*gsize = ecs.try_get<com_gsize_t>(ent))
        { gsize_e =*gsize; }
        /* coord */
        gpos3_t gpos3_e = { 0, 0 };
        if (auto*gpos3 = ecs.try_get<geom::com_gpos3_t>(ent))
        { gpos3_e =*gpos3; }
        /* pivot */
        pivot_t pivot_e = { PIVOT_MID, PIVOT_MID };
        if (auto*pivot = ecs.try_get<com_pivot_t>(ent))
        { pivot_e =*pivot; }
        /* grect */
        grect_t grect_e = {
            .x = { gpos3_e.x, gpos3_e.x },
            .y = { gpos3_e.y, gpos3_e.y },
        };
        grect_e.x.l += gsize_e.x * (PIVOT_MIN - pivot_e.x) / PIVOT_DIV;
        grect_e.x.r += gsize_e.x * (PIVOT_MAX - pivot_e.x) / PIVOT_DIV;
        grect_e.y.b += gsize_e.y * (PIVOT_MIN - pivot_e.y) / PIVOT_DIV;
        grect_e.y.t += gsize_e.y * (PIVOT_MAX - pivot_e.y) / PIVOT_DIV;
        /* update */
        ecs.emplace_or_replace<geom::com_grect_t>(ent, grect_e);
        ecs.emplace_or_replace<game::com_trect_t>(ent, game::trect_t{
            .x = {
                .l = grect_e.x.l / game::TCELL_ASIZE_X,
                .r = grect_e.x.r / game::TCELL_ASIZE_X,
            },
            .y = {
                .b = grect_e.y.b / game::TCELL_ASIZE_Y,
                .t = grect_e.y.t / game::TCELL_ASIZE_Y,
            },
        });
    }
    /* sizes */
    void update_gsize_tsize(ecs_t&ecs, ent_t ent)
    {
        if (ecs.valid(ent) == FALSE) { return; }
        /* sizes */
        geom::gsize_t gsize_e = { 0, 0 };
        if (auto*asize_e = ecs.try_get<geom::com_asize_t>(ent))
        { gsize_e.x = asize_e->x; gsize_e.y = asize_e->y; }
        if (auto*family_e = ecs.try_get<ecos::com_family_t>(ent))
        { /* family */
            auto ancestor = family_e->ancestor;
            if (ecs.valid(ancestor))
            { /* ancestor */
                if (auto*gsize_a = ecs.try_get<geom::com_gsize_t>(ancestor))
                { /* ancestor global size */
                    if (auto*rsize_e = ecs.try_get<geom::com_rsize_t>(ent))
                    { /* relative size */
                        gsize_e.x += gsize_a->x * rsize_e->x / RSIZE_DIV;
                        gsize_e.y += gsize_a->y * rsize_e->y / RSIZE_DIV;
                    } /* relative size */
                } /* ancestor global size */
            } /* ancestor */
            /* update sizes */
            ecs.emplace_or_replace<geom::com_gsize_t>(ent, gsize_e);
            ecs.emplace_or_replace<game::com_tsize_t>(ent, game::tsize_t{
                .x = gsize_e.x / game::TCELL_ASIZE_X,
                .y = gsize_e.y / game::TCELL_ASIZE_Y,
            });
            if (ecs.valid(family_e->follower))
            { /* update followers */
                auto follower = family_e->follower;
                update(ecs, follower);
                auto*family_f =&ecs.get<ecos::com_family_t>(family_e->follower);
                update_gsize_tsize(ecs, follower);
                /* siblingl */
                auto siblingl = follower;
                auto*family_l = family_f;
                while (ecs.valid(family_l->siblingl))
                {
                    siblingl = family_l->siblingl;
                    family_l =&ecs.get<ecos::com_family_t>(siblingl);
                    update_gsize_tsize(ecs, siblingl);
                }
                /* siblingr */
                auto siblingr = follower;
                auto*family_r = family_f;
                while (ecs.valid(family_r->siblingr))
                {
                    siblingr = family_r->siblingr;
                    family_r =&ecs.get<ecos::com_family_t>(siblingr);
                    update_gsize_tsize(ecs, siblingr);
                }
            } /* update family */
        } /* family */
        else
        { /* update sizes */
            ecs.emplace_or_replace<geom::com_gsize_t>(ent, gsize_e);
            ecs.emplace_or_replace<game::com_tsize_t>(ent, game::tsize_t{
                .x = gsize_e.x / game::TCELL_ASIZE_X,
                .y = gsize_e.y / game::TCELL_ASIZE_Y,
            });
        } /* update sizes */
        update_grect_trect(ecs, ent);
    }
    /* coord */
    void update_gpos3_tpos3(ecs_t&ecs, ent_t ent)
    {
        if (ecs.valid(ent) == FALSE) { return; }
        /* coord */
        geom::gpos3_t gpos3_e = { 0, 0 };
        if (auto*apos2_e = ecs.try_get<geom::com_apos2_t>(ent))
        { gpos3_e.x = apos2_e->x; gpos3_e.y = apos2_e->y; }
        if (auto*zpos1_e = ecs.try_get<geom::com_zpos1_t>(ent))
        { gpos3_e.z = zpos1_e->z; }
        /* family */
        if (auto*family_e = ecs.try_get<ecos::com_family_t>(ent))
        { /* family */
            auto ancestor = family_e->ancestor;
            if (ecs.valid(ancestor))
            { /* ancestor */
                if (auto*gsize_a = ecs.try_get<geom::com_gsize_t>(ancestor))
                { /* ancestor global size */
                    if (auto*rpos2_e = ecs.try_get<geom::com_rpos2_t>(ent))
                    { /* relative size */
                        gpos3_e.x += gsize_a->x * rpos2_e->x / RPOS2_DIV;
                        gpos3_e.y += gsize_a->y * rpos2_e->y / RPOS2_DIV;
                    } /* relative size */
                    if (auto*pivot_a = ecs.try_get<geom::com_pivot_t>(ancestor))
                    { /* pivot */
                        gpos3_e.x -= gsize_a->x * pivot_a->x / PIVOT_DIV;
                        gpos3_e.y -= gsize_a->y * pivot_a->y / PIVOT_DIV;
                    } /* pivot */
                } /* ancestor global size */
                if (auto*gpos3_a = ecs.try_get<geom::com_gpos3_t>(ancestor))
                { /* ancestor global size */
                    gpos3_e.x += gpos3_a->x;
                    gpos3_e.y += gpos3_a->y;
                    gpos3_e.z += gpos3_a->z;
                } /* ancestor global pos3 */
            } /* ancestor */
            /* update sizes */
            ecs.emplace_or_replace<geom::com_gpos3_t>(ent, gpos3_e);
            ecs.emplace_or_replace<game::com_tpos3_t>(ent, game::tpos3_t{
                .x = gpos3_e.x / game::TCELL_ASIZE_X,
                .y = gpos3_e.y / game::TCELL_ASIZE_Y,
            });
            if (ecs.valid(family_e->follower))
            { /* update followers */
                auto follower = family_e->follower;
                auto*family_f =&ecs.get<ecos::com_family_t>(family_e->follower);
                update_gpos3_tpos3(ecs, follower);
                /* siblingl */
                auto siblingl = follower;
                auto*family_l = family_f;
                while (ecs.valid(family_l->siblingl))
                {
                    siblingl = family_l->siblingl;
                    family_l =&ecs.get<ecos::com_family_t>(siblingl);
                    update_gpos3_tpos3(ecs, siblingl);
                }
                /* siblingr */
                auto siblingr = follower;
                auto*family_r = family_f;
                while (ecs.valid(family_r->siblingr))
                {
                    siblingr = family_r->siblingr;
                    family_r =&ecs.get<ecos::com_family_t>(siblingr);
                    update_gpos3_tpos3(ecs, siblingr);
                }
            } /* update family */
        } /* family */
        else
        { /* update coord */
            ecs.emplace_or_replace<geom::com_gpos3_t>(ent, gpos3_e);
            ecs.emplace_or_replace<game::com_tpos3_t>(ent, game::tpos3_t{
                .x = gpos3_e.x / game::TCELL_ASIZE_X,
                .y = gpos3_e.y / game::TCELL_ASIZE_Y,
            });
        } /* update coord */
        /* update global and tilegrid rect */
        update_grect_trect(ecs, ent);
        /* update visual */
        update_visual(ecs, ent);
    }
    /* visual */
    void update_visual(ecs_t&ecs, ent_t ent)
    {
        if (ecs.valid(ent) == FALSE) { return; }
        game::tpos3_t tpos3 = { 0, 0, 0 };
        if (auto*tpos3_e = ecs.try_get<game::com_tpos3_t>(ent))
        { tpos3 =*tpos3_e; }
        else if (auto*gpos3 = ecs.try_get<geom::com_gpos3_t>(ent))
        { tpos3 = game::get_tpos3_from_gpos3(*gpos3); }
    }
    void update_glayer(ecs_t&ecs, ent_t ent)
    {
        if (ecs.valid(ent) == FALSE) { return; }
        /* layer */
        gfix::glayer_t glayer_e = { 0 };
        if (auto*rlayer_e = ecs.try_get<gfix::com_rlayer_t>(ent))
        { glayer_e.layer = rlayer_e->layer; }
        if (auto*family_e = ecs.try_get<ecos::com_family_t>(ent))
        { /* family */
            auto ancestor = family_e->ancestor;
            if (ecs.valid(ancestor))
            { /* ancestor layer */
                if (auto*glayer_a = ecs.try_get<gfix::com_glayer_t>(ancestor))
                { glayer_e.layer += glayer_a->layer; }
            } /* ancestor */
            /* update */
            ecs.emplace_or_replace<gfix::com_glayer_t>(ent, glayer_e);
            if (ecs.valid(family_e->follower))
            { /* update followers */
                auto follower = family_e->follower;
                update(ecs, follower);
                auto*family_f =&ecs.get<ecos::com_family_t>(family_e->follower);
                update_glayer(ecs, follower);
                /* siblingl */
                auto siblingl = follower;
                auto*family_l = family_f;
                while (ecs.valid(family_l->siblingl))
                {
                    siblingl = family_l->siblingl;
                    family_l =&ecs.get<ecos::com_family_t>(siblingl);
                    update_glayer(ecs, siblingl);
                }
                /* siblingr */
                auto siblingr = follower;
                auto*family_r = family_f;
                while (ecs.valid(family_r->siblingr))
                {
                    siblingr = family_r->siblingr;
                    family_r =&ecs.get<ecos::com_family_t>(siblingr);
                    update_glayer(ecs, siblingr);
                }
            } /* update followers */
        } /* family */
        else
        { /* update layer */
            ecs.emplace_or_replace<gfix::com_glayer_t>(ent, glayer_e);
        } /* update layer */
    }
    void update_imreg(ecs_t&ecs, ent_t ent)
    {
        if (ecs.valid(ent) == FALSE) { return; }
        /* sizes */
        gfix::isize_t isize = { 0, 0 };
        /* coord */
        gfix::ipos2_t ipos2 = { 0, 0 };
        /* irect */
        gfix::irect_t irect = { { 0, 0 }, { 0, 0 } };
        /* imreg */
        if (auto*imreg = ecs.try_get<gfix::com_imreg_t>(ent))
        { /* imreg */
            auto*imori = gfix::get_imori_from_index(imreg->index);
            /* ipos2 */
            ipos2.x = imreg->apos2.x
            + (imori->isize.x * (imreg->rpos2.x - RPOS2_MIN) / RPOS2_DIV);
            ipos2.y = imreg->apos2.y
            + (imori->isize.y * (imreg->rpos2.y - RPOS2_MIN) / RPOS2_DIV);
            /* isize */
            isize.x = imreg->asize.x
            + (imreg->rsize.x * imori->isize.x / RSIZE_DIV);
            isize.y = imreg->asize.y
            + (imreg->rsize.y * imori->isize.y / RSIZE_DIV);
            /* irect */
            irect.x.l = ipos2.x
            + ((isize.x * (geom::PIVOT_MIN - imreg->pivot.x))
                / PIVOT_DIV);
            irect.x.r = ipos2.x
            + ((isize.x * (geom::PIVOT_MAX - imreg->pivot.x))
                / PIVOT_DIV);
            irect.y.b = ipos2.y
            + ((isize.y * (geom::PIVOT_MIN - imreg->pivot.y))
                / PIVOT_DIV);
            irect.y.t = ipos2.y
            + ((isize.y * (geom::PIVOT_MAX - imreg->pivot.y))
                / PIVOT_DIV);
        } /* imreg */
        /* update sizes */
        ecs.emplace_or_replace<gfix::com_isize_t>(ent, isize);
        /* update coord */
        ecs.emplace_or_replace<gfix::com_ipos2_t>(ent, ipos2);
        /* update irect */
        ecs.emplace_or_replace<gfix::com_irect_t>(ent, irect);
    }
    void update(ecs_t&ecs, ent_t ent)
    {
        if (ecs.valid(ent) == FALSE) { return; }
        /* sizes */
        gsize_t gsize_e = { 0, 0 };
        if (auto*asize_e = ecs.try_get<com_asize_t>(ent))
        { gsize_e.x = asize_e->x; gsize_e.y = asize_e->y; }
        /* coord */
        gpos3_t gpos3_e = { 0, 0, 0 };
        if (auto*apos2_e = ecs.try_get<com_apos2_t>(ent))
        { gpos3_e.x = apos2_e->x; gpos3_e.y = apos2_e->y; }
        if (auto*zpos1_e = ecs.try_get<com_zpos1_t>(ent))
        { gpos3_e.z = zpos1_e->z; }
        /* layer */
        gfix::glayer_t glayer_e = { 0 };
        if (auto*rlayer_e = ecs.try_get<gfix::com_rlayer_t>(ent))
        { glayer_e.layer = rlayer_e->layer; }
        if (auto*family_e = ecs.try_get<ecos::com_family_t>(ent))
        { /* entity family */
            auto ancestor = family_e->ancestor;
            if (ecs.valid(ancestor))
            { /* ancestor */
                if (auto*gsize_a = ecs.try_get<com_gsize_t>(ancestor))
                { /* ancestor global size */
                    if (auto*rsize_e = ecs.try_get<com_rsize_t>(ent))
                    { /* entity relative size */
                        gsize_e.x += gsize_a->x * rsize_e->x / RSIZE_DIV;
                        gsize_e.y += gsize_a->y * rsize_e->y / RSIZE_DIV;
                    } /* entity relative size */
                    if (auto*rpos2_e = ecs.try_get<com_rpos2_t>(ent))
                    { /* entity relative pos2 */
                        gpos3_e.x += gsize_a->x * rpos2_e->x / RPOS2_DIV;
                        gpos3_e.y += gsize_a->y * rpos2_e->y / RPOS2_DIV;
                    } /* entity relative pos2 */
                    if (auto*pivot_a = ecs.try_get<com_pivot_t>(ancestor))
                    { /* pivot */
                        gpos3_e.x -= gsize_a->x * pivot_a->x / PIVOT_DIV;
                        gpos3_e.y -= gsize_a->y * pivot_a->y / PIVOT_DIV;
                    } /* pivot */
                } /* ancestor global size */
                if (auto*gpos3_a = ecs.try_get<geom::com_gpos3_t>(ancestor))
                { /* ancestor global pos3 */
                    gpos3_e.x += gpos3_a->x;
                    gpos3_e.y += gpos3_a->y;
                    gpos3_e.z += gpos3_a->z;
                } /* ancestor global pos3 */
                /* ancestor global layer */
                if (auto*glayer_a = ecs.try_get<gfix::com_glayer_t>(ancestor))
                { glayer_e.layer += glayer_a->layer; }
            } /* ancestor */
            /* update sizes */
            ecs.emplace_or_replace<geom::com_gsize_t>(ent, gsize_e);
            ecs.emplace_or_replace<game::com_tsize_t>(ent, game::tsize_t{
                .x = gsize_e.x / game::TCELL_ASIZE_X,
                .y = gsize_e.y / game::TCELL_ASIZE_Y,
            });
            /* update coord */
            ecs.emplace_or_replace<geom::com_gpos3_t>(ent, gpos3_e);
            ecs.emplace_or_replace<game::com_tpos3_t>(ent, game::tpos3_t{
                .x = gpos3_e.x / game::TCELL_ASIZE_X,
                .y = gpos3_e.y / game::TCELL_ASIZE_Y,
                .z = gpos3_e.z / game::TCELL_ASIZE_Z,
            });
            /* update layer */
            ecs.emplace_or_replace<gfix::com_glayer_t>(ent, glayer_e);
            if (ecs.valid(family_e->follower))
            { /* update followers */
                auto follower = family_e->follower;
                auto*family_f =&ecs.get<ecos::com_family_t>(family_e->follower);
                update(ecs, follower);
                /* siblingl */
                auto siblingl = follower;
                auto*family_l = family_f;
                while (ecs.valid(family_l->siblingl))
                {
                    siblingl = family_l->siblingl;
                    family_l =&ecs.get<ecos::com_family_t>(siblingl);
                    update(ecs, siblingl);
                }
                /* siblingr */
                auto siblingr = follower;
                auto*family_r = family_f;
                while (ecs.valid(family_r->siblingr))
                {
                    siblingr = family_r->siblingr;
                    family_r =&ecs.get<ecos::com_family_t>(siblingr);
                    update(ecs, siblingr);
                }
            } /* update followers */
        } /* family */
        else
        { /* update */
            /* sizes */
            ecs.emplace_or_replace<geom::com_gsize_t>(ent, gsize_e);
            ecs.emplace_or_replace<game::com_tsize_t>(ent, game::tsize_t{
                .x = gsize_e.x / game::TCELL_ASIZE_X,
                .y = gsize_e.y / game::TCELL_ASIZE_Y,
            });
            /* coord */
            ecs.emplace_or_replace<geom::com_gpos3_t>(ent, gpos3_e);
            ecs.emplace_or_replace<game::com_tpos3_t>(ent, game::tpos3_t{
                .x = gpos3_e.x / game::TCELL_ASIZE_X,
                .y = gpos3_e.y / game::TCELL_ASIZE_Y,
                .z = gpos3_e.z / game::TCELL_ASIZE_Z,
            });
            /* layer */
            ecs.emplace_or_replace<gfix::com_glayer_t>(ent, glayer_e);
        } /* update */
        /* pivot */
        pivot_t pivot_e = { PIVOT_MID, PIVOT_MID };
        if (auto*pivot = ecs.try_get<geom::com_pivot_t>(ent))
        { pivot_e =*pivot; }
        /* grect */
        grect_t grect_e = {
            .x = { gpos3_e.x, gpos3_e.x },
            .y = { gpos3_e.y, gpos3_e.y },
        };
        grect_e.x.l += gsize_e.x * (PIVOT_MIN - pivot_e.x) / PIVOT_DIV;
        grect_e.x.r += gsize_e.x * (PIVOT_MAX - pivot_e.x) / PIVOT_DIV;
        grect_e.y.b += gsize_e.y * (PIVOT_MIN - pivot_e.y) / PIVOT_DIV;
        grect_e.y.t += gsize_e.y * (PIVOT_MAX - pivot_e.y) / PIVOT_DIV;
        /* update */
        ecs.emplace_or_replace<geom::com_grect_t>(ent, grect_e);
        ecs.emplace_or_replace<game::com_trect_t>(ent, game::trect_t{
            .x = {
                .l = grect_e.x.l / game::TCELL_ASIZE_X,
                .r = grect_e.x.r / game::TCELL_ASIZE_X,
            },
            .y = {
                .b = grect_e.y.b / game::TCELL_ASIZE_Y,
                .t = grect_e.y.t / game::TCELL_ASIZE_Y,
            },
        });
    }
} geom_listener;

} /* typedef */

namespace geom { /* actions */

bool init()
{
    /** sizes **/
    entt::sigh_helper{ecos::ecs}.with<geom::com_asize_t>()
    .on_construct<&geom_listener_t::update_gsize_tsize>(geom_listener)
    .on_destroy<&geom_listener_t::update_gsize_tsize>(geom_listener)
    .on_update<&geom_listener_t::update_gsize_tsize>(geom_listener);
    entt::sigh_helper{ecos::ecs}.with<geom::com_rsize_t>()
    .on_construct<&geom_listener_t::update_gsize_tsize>(geom_listener)
    .on_destroy<&geom_listener_t::update_gsize_tsize>(geom_listener)
    .on_update<&geom_listener_t::update_gsize_tsize>(geom_listener);
    /** coord **/
    entt::sigh_helper{ecos::ecs}.with<geom::com_apos2_t>()
    .on_construct<&geom_listener_t::update_gpos3_tpos3>(geom_listener)
    .on_destroy<&geom_listener_t::update_gpos3_tpos3>(geom_listener)
    .on_update<&geom_listener_t::update_gpos3_tpos3>(geom_listener);
    entt::sigh_helper{ecos::ecs}.with<geom::com_rpos2_t>()
    .on_construct<&geom_listener_t::update_gpos3_tpos3>(geom_listener)
    .on_destroy<&geom_listener_t::update_gpos3_tpos3>(geom_listener)
    .on_update<&geom_listener_t::update_gpos3_tpos3>(geom_listener);
    entt::sigh_helper{ecos::ecs}.with<geom::com_zpos1_t>()
    .on_construct<&geom_listener_t::update_gpos3_tpos3>(geom_listener)
    .on_destroy<&geom_listener_t::update_gpos3_tpos3>(geom_listener)
    .on_update<&geom_listener_t::update_gpos3_tpos3>(geom_listener);
    entt::sigh_helper{ecos::ecs}.with<geom::com_pivot_t>()
    .on_construct<&geom_listener_t::update_grect_trect>(geom_listener)
    .on_destroy<&geom_listener_t::update_grect_trect>(geom_listener)
    .on_update<&geom_listener_t::update_grect_trect>(geom_listener);
    /* visual */
    entt::sigh_helper{ecos::ecs}.with<gfix::com_rlayer_t>()
    .on_update<&geom_listener_t::update_glayer>(geom_listener)
    .on_construct<&geom_listener_t::update_glayer>(geom_listener)
    .on_destroy<&geom_listener_t::update_glayer>(geom_listener);
    entt::sigh_helper{ecos::ecs}.with<gfix::com_imreg_t>()
    .on_construct<&geom_listener_t::update_imreg>(geom_listener)
    .on_destroy<&geom_listener_t::update_imreg>(geom_listener)
    .on_update<&geom_listener_t::update_imreg>(geom_listener);
    /** family **/
    entt::sigh_helper{ecos::ecs}.with<ecos::com_family_t>()
    .on_construct<&geom_listener_t::update>(geom_listener)
    .on_destroy<&geom_listener_t::update>(geom_listener)
    .on_update<&geom_listener_t::update>(geom_listener);
    return TRUTH;
}
bool quit()
{
    return TRUTH;
}

} /* actions */

} /* content */

#endif/*SOMIGAME_GEOM_CXX*/
