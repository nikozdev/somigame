#include "head.hxx"

/* headers */

#include "util.hxx"
#include "ecos.hxx"

/* defines */

/* content */

_NAMESPACE_ENTER

/** typedef **/

/** datadef **/

timer_t timer;

using ename_table_t = std::array<ent_t, _ENAME_COUNT>;
static ename_table_t ename_table;

/** actions **/

void util_init()
{
    /* signals */
    /** entity **/
    entt::sigh_helper{ecos}
    .with<entity_t>().on_update<[](ecos_t&ecos, ent_t ent){
        _ELOG("entity update: %u", ent);
    }>();
    /** strbuf **/
    entt::sigh_helper{ecos}
    .with<strbuf_t>().on_update<[](ecos_t&ecos, ent_t ent){
        auto&strbuf = ecos.get<com_strbuf_t>(ent);
    }>().on_construct<[](ecos_t&ecos, ent_t ent){
        auto&strbuf = ecos.get<com_strbuf_t>(ent);
        strbuf.msize = std::min(strbuf.msize, STRBUF_MSIZE);
        memset(strbuf.mdata, '\0', strbuf.msize);
    }>().on_destroy<[](ecos_t&ecos, ent_t ent){
        auto&strbuf = ecos.get<com_strbuf_t>(ent);
        memset(strbuf.mdata, '\0', strbuf.msize);
    }>();
    /** names **/
    for (auto&ent:ename_table) { ent = entt::null; }
    entt::sigh_helper{ecos}
    .with<com_ename_t>().on_update<[](ecos_t&ecos, ent_t ent){
        auto&ename = ecos.get<com_ename_t>(ent);
        for (auto iter = 0; iter < _ENAME_COUNT; iter++)
        {
            if (ename_table[iter] == ent)
            { ename_table[iter] = entt::null; break; }
        }
        ename_table[ename.e] = ent;
    }>().on_construct<[](ecos_t&ecos, ent_t ent){
        auto&ename = ecos.get<com_ename_t>(ent);
        ename_table[ename.e] = ent;
    }>().on_destroy<[](ecos_t&ecos, ent_t ent){
        auto&ename = ecos.get<com_ename_t>(ent);
        ename_table[ename.e] = entt::null;
    }>();
    /** gfix **/
    struct gfix_listener_t
    {
        /* geometry */
        void update_grect_trect(ecos_t&ecos, ent_t ent) {
            /* sizes */
            gsize_t gsize_e = { 0, 0 };
            if (auto*gsize = ecos.try_get<com_gsize_t>(ent))
            { gsize_e =*gsize; }
            /* coord */
            gpos3_t gpos3_e = { 0, 0 };
            if (auto*gpos3 = ecos.try_get<com_gpos3_t>(ent))
            { gpos3_e =*gpos3; }
            /* pivot */
            pivot_t pivot_e = { PIVOT_MID, PIVOT_MID };
            if (auto*pivot = ecos.try_get<com_pivot_t>(ent))
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
            ecos.emplace_or_replace<com_grect_t>(ent, grect_e);
            ecos.emplace_or_replace<com_trect_t>(ent, trect_t{
                .x = {
                    .l = grect_e.x.l / TCELL_ASIZE_X,
                    .r = grect_e.x.r / TCELL_ASIZE_X,
                },
                .y = {
                    .b = grect_e.y.b / TCELL_ASIZE_Y,
                    .t = grect_e.y.t / TCELL_ASIZE_Y,
                },
            });
        }
        /* sizes */
        void update_gsize_tsize(ecos_t&ecos, ent_t ent)
        {
            /* sizes */
            gsize_t gsize_e = { 0, 0 };
            if (auto*asize_e = ecos.try_get<com_asize_t>(ent))
            { gsize_e.x = asize_e->x; gsize_e.y = asize_e->y; }
            if (auto*family_e = ecos.try_get<com_family_t>(ent))
            { /* family */
                auto ancestor = family_e->ancestor;
                if (ecos.valid(ancestor))
                { /* ancestor */
                    if (auto*gsize_a = ecos.try_get<com_gsize_t>(ancestor))
                    { /* ancestor global size */
                        if (auto*rsize_e = ecos.try_get<com_rsize_t>(ent))
                        { /* relative size */
                            gsize_e.x += gsize_a->x * rsize_e->x / RSIZE_DIV;
                            gsize_e.y += gsize_a->y * rsize_e->y / RSIZE_DIV;
                        } /* relative size */
                    } /* ancestor global size */
                } /* ancestor */
                /* update sizes */
                ecos.emplace_or_replace<com_gsize_t>(ent, gsize_e);
                ecos.emplace_or_replace<com_tsize_t>(ent, tsize_t{
                    .x = gsize_e.x / TCELL_ASIZE_X,
                    .y = gsize_e.y / TCELL_ASIZE_Y,
                });
                if (ecos.valid(family_e->follower))
                { /* update followers */
                    auto follower = family_e->follower;
                    update(ecos, follower);
                    auto*family_f =&ecos.get<com_family_t>(family_e->follower);
                    update_gsize_tsize(ecos, follower);
                    /* siblingl */
                    auto siblingl = follower;
                    auto*family_l = family_f;
                    while (ecos.valid(family_l->siblingl))
                    {
                        siblingl = family_l->siblingl;
                        family_l =&ecos.get<com_family_t>(siblingl);
                        update_gsize_tsize(ecos, siblingl);
                    }
                    /* siblingr */
                    auto siblingr = follower;
                    auto*family_r = family_f;
                    while (ecos.valid(family_r->siblingr))
                    {
                        siblingr = family_r->siblingr;
                        family_r =&ecos.get<com_family_t>(siblingr);
                        update_gsize_tsize(ecos, siblingr);
                    }
                } /* update family */
            } /* family */
            else
            { /* update sizes */
                ecos.emplace_or_replace<com_gsize_t>(ent, gsize_e);
                ecos.emplace_or_replace<com_tsize_t>(ent, tsize_t{
                    .x = gsize_e.x / TCELL_ASIZE_X,
                    .y = gsize_e.y / TCELL_ASIZE_Y,
                });
            } /* update sizes */
            update_grect_trect(ecos, ent);
        }
        /* coord */
        void update_gpos3_tpos3(ecos_t&ecos, ent_t ent)
        {
            /* coord */
            gpos3_t gpos3_e = { 0, 0 };
            if (auto*apos2_e = ecos.try_get<com_apos2_t>(ent))
            { gpos3_e.x = apos2_e->x; gpos3_e.y = apos2_e->y; }
            if (auto*zpos1_e = ecos.try_get<com_zpos1_t>(ent))
            { gpos3_e.x = zpos1_e->z; }
            if (auto*family_e = ecos.try_get<com_family_t>(ent))
            { /* family */
                auto ancestor = family_e->ancestor;
                if (ecos.valid(ancestor))
                { /* ancestor */
                    if (auto*gsize_a = ecos.try_get<com_gsize_t>(ancestor))
                    { /* ancestor global size */
                        if (auto*rpos2_e = ecos.try_get<com_rpos2_t>(ent))
                        { /* relative size */
                            gpos3_e.x += gsize_a->x * rpos2_e->x / RPOS2_DIV;
                            gpos3_e.y += gsize_a->y * rpos2_e->y / RPOS2_DIV;
                        } /* relative size */
                    } /* ancestor global size */
                } /* ancestor */
                /* update sizes */
                ecos.emplace_or_replace<com_gpos3_t>(ent, gpos3_e);
                ecos.emplace_or_replace<com_tpos3_t>(ent, tpos3_t{
                    .x = gpos3_e.x / TCELL_ASIZE_X,
                    .y = gpos3_e.y / TCELL_ASIZE_Y,
                });
                if (ecos.valid(family_e->follower))
                { /* update followers */
                    auto follower = family_e->follower;
                    update(ecos, follower);
                    auto*family_f =&ecos.get<com_family_t>(family_e->follower);
                    update_gpos3_tpos3(ecos, follower);
                    /* siblingl */
                    auto siblingl = follower;
                    auto*family_l = family_f;
                    while (ecos.valid(family_l->siblingl))
                    {
                        siblingl = family_l->siblingl;
                        family_l =&ecos.get<com_family_t>(siblingl);
                        update_gpos3_tpos3(ecos, siblingl);
                    }
                    /* siblingr */
                    auto siblingr = follower;
                    auto*family_r = family_f;
                    while (ecos.valid(family_r->siblingr))
                    {
                        siblingr = family_r->siblingr;
                        family_r =&ecos.get<com_family_t>(siblingr);
                        update_gpos3_tpos3(ecos, siblingr);
                    }
                } /* update family */
            } /* family */
            else
            { /* update coord */
                ecos.emplace_or_replace<com_gpos3_t>(ent, gpos3_e);
                ecos.emplace_or_replace<com_tpos3_t>(ent, tpos3_t{
                    .x = gpos3_e.x / TCELL_ASIZE_X,
                    .y = gpos3_e.y / TCELL_ASIZE_Y,
                });
            } /* update coord */
            update_grect_trect(ecos, ent);
        }
        /* visual */
        void update_layer(ecos_t&ecos, ent_t ent)
        {
            /* layer */
            glayer_t glayer_e = { 0 };
            if (auto*rlayer_e = ecos.try_get<com_rlayer_t>(ent))
            { glayer_e.layer = rlayer_e->layer; }
            if (auto*family_e = ecos.try_get<com_family_t>(ent))
            { /* family */
                auto ancestor = family_e->ancestor;
                if (ecos.valid(ancestor))
                { /* ancestor layer */
                    if (auto*glayer_a = ecos.try_get<com_glayer_t>(ancestor))
                    { glayer_e.layer += glayer_a->layer; }
                } /* ancestor */
                /* update */
                ecos.emplace_or_replace<com_glayer_t>(ent, glayer_e);
                if (ecos.valid(family_e->follower))
                { /* update followers */
                    auto follower = family_e->follower;
                    update(ecos, follower);
                    auto*family_f =&ecos.get<com_family_t>(family_e->follower);
                    update_layer(ecos, follower);
                    /* siblingl */
                    auto siblingl = follower;
                    auto*family_l = family_f;
                    while (ecos.valid(family_l->siblingl))
                    {
                        siblingl = family_l->siblingl;
                        family_l =&ecos.get<com_family_t>(siblingl);
                        update_layer(ecos, siblingl);
                    }
                    /* siblingr */
                    auto siblingr = follower;
                    auto*family_r = family_f;
                    while (ecos.valid(family_r->siblingr))
                    {
                        siblingr = family_r->siblingr;
                        family_r =&ecos.get<com_family_t>(siblingr);
                        update_layer(ecos, siblingr);
                    }
                } /* update followers */
            } /* family */
            else
            { /* update layer */
                ecos.emplace_or_replace<com_glayer_t>(ent, glayer_e);
            } /* update layer */
        }
        void update_imreg(ecos_t&ecos, ent_t ent)
        {
            /* sizes */
            isize_t isize = { 0, 0 };
            /* coord */
            ipos2_t ipos2 = { 0, 0 };
            /* irect */
            irect_t irect = { { 0, 0 }, { 0, 0 } };
            /* imreg */
            if (auto*imreg = ecos.try_get<com_imreg_t>(ent))
            { /* imreg */
                auto*imori = get_imori_from_index(imreg->index);
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
                    + ((isize.x * (PIVOT_MIN - imreg->pivot.x))
                        / PIVOT_DIV);
                irect.x.r = ipos2.x
                    + ((isize.x * (PIVOT_MAX - imreg->pivot.x))
                        / PIVOT_DIV);
                irect.y.b = ipos2.y
                    + ((isize.y * (PIVOT_MIN - imreg->pivot.y))
                        / PIVOT_DIV);
                irect.y.t = ipos2.y
                    + ((isize.y * (PIVOT_MAX - imreg->pivot.y))
                        / PIVOT_DIV);
            } /* imreg */
            /* update sizes */
            ecos.emplace_or_replace<com_isize_t>(ent, isize);
            /* update coord */
            ecos.emplace_or_replace<com_ipos2_t>(ent, ipos2);
            /* update irect */
            ecos.emplace_or_replace<com_irect_t>(ent, irect);
        }
        void update(ecos_t&ecos, ent_t ent)
        {
            /* sizes */
            gsize_t gsize_e = { 0, 0 };
            if (auto*asize_e = ecos.try_get<com_asize_t>(ent))
            { gsize_e.x = asize_e->x; gsize_e.y = asize_e->y; }
            /* coord */
            gpos3_t gpos3_e = { 0, 0, 0 };
            if (auto*apos2_e = ecos.try_get<com_apos2_t>(ent))
            { gpos3_e.x = apos2_e->x; gpos3_e.y = apos2_e->y; }
            if (auto*zpos1_e = ecos.try_get<com_zpos1_t>(ent))
            { gpos3_e.z = zpos1_e->z; }
            /* layer */
            glayer_t glayer_e = { 0 };
            if (auto*rlayer_e = ecos.try_get<com_rlayer_t>(ent))
            { glayer_e.layer = rlayer_e->layer; }
            if (auto*family_e = ecos.try_get<com_family_t>(ent))
            { /* entity family */
                auto ancestor = family_e->ancestor;
                if (ecos.valid(ancestor))
                { /* ancestor */
                    if (auto*gsize_a = ecos.try_get<com_gsize_t>(ancestor))
                    { /* ancestor global size */
                        if (auto*rsize_e = ecos.try_get<com_rsize_t>(ent))
                        { /* entity relative size */
                            gsize_e.x += gsize_a->x * rsize_e->x / RSIZE_DIV;
                            gsize_e.y += gsize_a->y * rsize_e->y / RSIZE_DIV;
                        } /* entity relative size */
                        if (auto*rpos2_e = ecos.try_get<com_rpos2_t>(ent))
                        { /* entity relative pos2 */
                            gpos3_e.x += gsize_a->x * rpos2_e->x / RPOS2_DIV;
                            gpos3_e.y += gsize_a->y * rpos2_e->y / RPOS2_DIV;
                        } /* entity relative pos2 */
                    } /* ancestor global size */
                    if (auto*gpos3_a = ecos.try_get<com_gpos3_t>(ancestor))
                    { /* ancestor global pos3 */
                        gpos3_e.x += gpos3_a->x;
                        gpos3_e.y += gpos3_a->y;
                        gpos3_e.z += gpos3_a->z;
                    } /* ancestor global pos3 */
                    /* ancestor global layer */
                    if (auto*glayer_a = ecos.try_get<com_glayer_t>(ancestor))
                    { glayer_e.layer += glayer_a->layer; }
                } /* ancestor */
                /* update sizes */
                ecos.emplace_or_replace<com_gsize_t>(ent, gsize_e);
                ecos.emplace_or_replace<com_tsize_t>(ent, tsize_t{
                    .x = gsize_e.x / TCELL_ASIZE_X,
                    .y = gsize_e.y / TCELL_ASIZE_Y,
                });
                /* update coord */
                ecos.emplace_or_replace<com_gpos3_t>(ent, gpos3_e);
                ecos.emplace_or_replace<com_tpos3_t>(ent, tpos3_t{
                    .x = gpos3_e.x / TCELL_ASIZE_X,
                    .y = gpos3_e.y / TCELL_ASIZE_Y,
                    .z = gpos3_e.z / TCELL_ASIZE_Z,
                });
                /* update layer */
                ecos.emplace_or_replace<com_glayer_t>(ent, glayer_e);
                if (ecos.valid(family_e->follower))
                { /* update followers */
                    auto follower = family_e->follower;
                    auto*family_f =&ecos.get<com_family_t>(family_e->follower);
                    update(ecos, follower);
                    /* siblingl */
                    auto siblingl = follower;
                    auto*family_l = family_f;
                    while (ecos.valid(family_l->siblingl))
                    {
                        siblingl = family_l->siblingl;
                        family_l =&ecos.get<com_family_t>(siblingl);
                        update(ecos, siblingl);
                    }
                    /* siblingr */
                    auto siblingr = follower;
                    auto*family_r = family_f;
                    while (ecos.valid(family_r->siblingr))
                    {
                        siblingr = family_r->siblingr;
                        family_r =&ecos.get<com_family_t>(siblingr);
                        update(ecos, siblingr);
                    }
                } /* update followers */
            } /* family */
            else
            { /* update */
                /* sizes */
                ecos.emplace_or_replace<com_gsize_t>(ent, gsize_e);
                ecos.emplace_or_replace<com_tsize_t>(ent, tsize_t{
                    .x = gsize_e.x / TCELL_ASIZE_X,
                    .y = gsize_e.y / TCELL_ASIZE_Y,
                });
                /* coord */
                ecos.emplace_or_replace<com_gpos3_t>(ent, gpos3_e);
                ecos.emplace_or_replace<com_tpos3_t>(ent, tpos3_t{
                    .x = gpos3_e.x / TCELL_ASIZE_X,
                    .y = gpos3_e.y / TCELL_ASIZE_Y,
                    .z = gpos3_e.z / TCELL_ASIZE_Z,
                });
                /* layer */
                ecos.emplace_or_replace<com_glayer_t>(ent, glayer_e);
            } /* update */
            /* pivot */
            pivot_t pivot_e = { PIVOT_MID, PIVOT_MID };
            if (auto*pivot = ecos.try_get<com_pivot_t>(ent))
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
            ecos.emplace_or_replace<com_grect_t>(ent, grect_e);
            ecos.emplace_or_replace<com_trect_t>(ent, trect_t{
                .x = {
                    .l = grect_e.x.l / TCELL_ASIZE_X,
                    .r = grect_e.x.r / TCELL_ASIZE_X,
                },
                .y = {
                    .b = grect_e.y.b / TCELL_ASIZE_Y,
                    .t = grect_e.y.t / TCELL_ASIZE_Y,
                },
            });
        }
    } gfix_listener;
    /** sizes **/
    entt::sigh_helper{ecos}.with<com_asize_t>()
        .on_update<&gfix_listener_t::update_gsize_tsize>(gfix_listener)
        .on_destroy<&gfix_listener_t::update_gsize_tsize>(gfix_listener)
        .on_construct<&gfix_listener_t::update_gsize_tsize>(gfix_listener);
    entt::sigh_helper{ecos}.with<com_rsize_t>()
        .on_update<&gfix_listener_t::update_gsize_tsize>(gfix_listener)
        .on_destroy<&gfix_listener_t::update_gsize_tsize>(gfix_listener)
        .on_construct<&gfix_listener_t::update_gsize_tsize>(gfix_listener);
    /** coord **/
    entt::sigh_helper{ecos}.with<com_apos2_t>()
        .on_update<&gfix_listener_t::update_gpos3_tpos3>(gfix_listener)
        .on_destroy<&gfix_listener_t::update_gpos3_tpos3>(gfix_listener)
        .on_construct<&gfix_listener_t::update_gpos3_tpos3>(gfix_listener);
    entt::sigh_helper{ecos}.with<com_rpos2_t>()
        .on_update<&gfix_listener_t::update_gpos3_tpos3>(gfix_listener)
        .on_destroy<&gfix_listener_t::update_gpos3_tpos3>(gfix_listener)
        .on_construct<&gfix_listener_t::update_gpos3_tpos3>(gfix_listener);
    entt::sigh_helper{ecos}.with<com_zpos1_t>()
        .on_update<&gfix_listener_t::update_gpos3_tpos3>(gfix_listener)
        .on_destroy<&gfix_listener_t::update_gpos3_tpos3>(gfix_listener)
        .on_construct<&gfix_listener_t::update_gpos3_tpos3>(gfix_listener);
    entt::sigh_helper{ecos}.with<com_pivot_t>()
        .on_update<&gfix_listener_t::update_grect_trect>(gfix_listener)
        .on_destroy<&gfix_listener_t::update_grect_trect>(gfix_listener)
        .on_construct<&gfix_listener_t::update_grect_trect>(gfix_listener);
    /** visual **/
    entt::sigh_helper{ecos}.with<com_rlayer_t>()
        .on_update<&gfix_listener_t::update_layer>(gfix_listener)
    .on_destroy<&gfix_listener_t::update_layer>(gfix_listener)
    .on_construct<&gfix_listener_t::update_layer>(gfix_listener);
    entt::sigh_helper{ecos}.with<com_imreg_t>()
        .on_update<&gfix_listener_t::update_imreg>(gfix_listener)
    .on_destroy<&gfix_listener_t::update_imreg>(gfix_listener)
    .on_construct<&gfix_listener_t::update_imreg>(gfix_listener);
    /** family **/
    entt::sigh_helper{ecos}.with<com_family_t>()
        .on_update<&gfix_listener_t::update>(gfix_listener)
    .on_destroy<&gfix_listener_t::update>(gfix_listener)
    .on_construct<&gfix_listener_t::update>(gfix_listener);
}
void util_loop()
{
    timer.was_mil = timer.now_mil;
    timer.now_mil = ::glutGet(GLUT_ELAPSED_TIME);
    timer.dif_mil = timer.now_mil - timer.was_mil;
    if (timer.dif_mil == 0) { timer.dif_mil = 1; }
    else if (timer.dif_mil < 0) { timer.dif_mil = -timer.dif_mil; }
    call_on_sec([](int was_sec, int now_sec, int dif_sec){
        if (timer.dif_mil > 0)
        {
            timer.fps_num = 1'000 / timer.dif_mil;
            timer.dispatcher.trigger<newsec_event_t>( { .sec = timer.now_mil / 1'000 } );
            timer.dispatcher.update<newsec_event_t>();
        }
    });
    timer.dispatcher.trigger<update_event_t>( { .mil = timer.now_mil } );
    timer.dispatcher.update<update_event_t>();
}

/** getters **/

ent_t get_by_ename(const ename_t&ename)
{
    return ename_table[ename.e];
}
ent_t get_by_iname(const iname_t&iname)
{
    for (auto&&[ent,iname_e] : ecos.view<com_iname_t>().each())
    { if (iname_e.i == iname.i) { return ent; } }
    return entt::null;
}
ent_t get_by_sname(const sname_t&sname)
{
    for (auto&&[ent,sname] : ecos.view<com_sname_t>().each())
    { if (std::strcmp(sname.s, sname.s) == 0) { return ent; } }
    return entt::null;
}

_NAMESPACE_LEAVE
