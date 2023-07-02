#ifndef SOMIGAME_GFIX_CXX
#define SOMIGAME_GFIX_CXX 1

#include "head.hxx"

/* headers */

#include "gfix.hxx"
#include "gfix/view.hxx"
#include "gfix/guis.hxx"
#include "gfix/text.hxx"
#include "gfix/layer.hxx"
#include "gfix/color.hxx"
#include "gfix/image.hxx"
#include "gfix/visual.hxx"
#include "geom.hxx"
#include "geom/sizes.hxx"
#include "geom/coord.hxx"
#include "geom/direc.hxx"
#include "geom/shape.hxx"
#include "iput.hxx"
#include "ecos.hxx"
#include "game.hxx"
#include "game/tile.hxx"
#include "game/hero.hxx"
#include "game/somi.hxx"
#include "main.hxx"

/* defines */

#undef _HELP_LINE

namespace somigame { /* content */

namespace gfix { /* signals */

signal_t<void(void)> init_top_signal;
signal_t<void(void)> init_bot_signal;
signal_t<void(void)> quit_top_signal;
signal_t<void(void)> quit_bot_signal;
signal_t<void(void)> work_top_signal;
signal_t<void(void)> work_bot_signal;

} /* signals */

namespace gfix { /* datadef */

struct window_t {
    geom::ratio_t ratio = { .x = gfix::VIEW_RATIO_X, .y = gfix::VIEW_RATIO_Y };
    geom::apos2_t apos2 = { .x = 0, .y = 0 };
    geom::asize_t asize = { .x = gfix::VIEW_ASIZE_X, .y = gfix::VIEW_ASIZE_Y };
} window;

} /* datadef */

namespace gfix { /* actions */

bool init()
{
    gfix::init_top_signal.holder.publish();
    /* glut setup */
    ::glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    ::glutInitWindowPosition(window.apos2.x, window.apos2.y);
    ::glutInitWindowSize(window.asize.x, window.asize.y);
    ::glutCreateWindow(_NAME_STR);
    ::glutReshapeFunc([](int sizex, int sizey) {
        auto winratx = window.ratio.x;
        auto winraty = window.ratio.y;
        auto winrelw = sizex / winratx;
        auto winrelh = sizey / winraty;
        auto winreld = winrelw - winrelh;
        if (winreld == 0)
        {
            window.apos2.x = 0;
            window.apos2.y = 0;
        }
        else if (winreld < 0)
        {
            window.apos2.x = 0;
            window.apos2.y = -winreld * winraty / 2;
            sizey -= (window.apos2.y * 2);
        }
        else if (winreld > 0)
        {
            window.apos2.x = +winreld * winratx / 2;
            window.apos2.y = 0;
            sizex -= (window.apos2.x * 2);
        }
        window.asize.x = sizex;
        window.asize.y = sizey;
        ::glViewport(window.apos2.x, window.apos2.y, window.asize.x, window.asize.y);
    });
    /* opengl setup */
    _ELOG("[opengl-version] {:s}\n", (char*)::glGetString(GL_VERSION));
    ::glDisable(GL_DEPTH_TEST);
    ::glDisable(GL_STENCIL_TEST);
    ::glEnable(GL_TEXTURE_1D);
    ::glEnable(GL_TEXTURE_2D);
    ::glDisable(GL_TEXTURE_3D);
    ::glEnable(GL_BLEND);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef _CONF_WORK
    ::glEnable(GL_CULL_FACE);
    ::glCullFace(GL_BACK);
    ::glFrontFace(GL_CCW);
#else
    ::glDisable(GL_CULL_FACE);
#endif
    ::glShadeModel(GL_FLAT);
    /* imori load */
    init_image();
    //init_layer();
    /* layer component signal */
#if 0
    struct gfix_listener_t
    {
        void update_glayer(ecos::ecs_t&ecos::ecs, ent_t ent)
        {
            if (ecos::ecs.valid(ent) == FALSE) { return; }
            /* layer */
            gfix::glayer_t glayer_e = { 0 };
            if (auto*rlayer_e = ecos::ecs.try_get<gfix::com_rlayer_t>(ent))
            { glayer_e.layer = rlayer_e->layer; }
            if (auto*family_e = ecos::ecs.try_get<ecos::com_family_t>(ent))
            { /* family */
                auto ancestor = family_e->ancestor;
                if (ecos::ecs.valid(ancestor))
                { /* ancestor layer */
                    if (auto*glayer_a = ecos::ecs.try_get<gfix::com_glayer_t>(ancestor))
                    { glayer_e.layer += glayer_a->layer; }
                } /* ancestor */
                /* update */
                ecos::ecs.emplace_or_replace<gfix::com_glayer_t>(ent, glayer_e);
                if (ecos::ecs.valid(family_e->follower))
                { /* update followers */
                    auto follower = family_e->follower;
                    auto*family_f =&ecos::ecs.get<ecos::com_family_t>(family_e->follower);
                    update_glayer(ecos::ecs, follower);
                    /* siblingl */
                    auto siblingl = follower;
                    auto*family_l = family_f;
                    while (ecos::ecs.valid(family_l->siblingl))
                    {
                        siblingl = family_l->siblingl;
                        family_l =&ecos::ecs.get<ecos::com_family_t>(siblingl);
                        update_glayer(ecos::ecs, siblingl);
                    }
                    /* siblingr */
                    auto siblingr = follower;
                    auto*family_r = family_f;
                    while (ecos::ecs.valid(family_r->siblingr))
                    {
                        siblingr = family_r->siblingr;
                        family_r =&ecos::ecs.get<ecos::com_family_t>(siblingr);
                        update_glayer(ecos::ecs, siblingr);
                    }
                } /* update followers */
            } /* family */
            else
            { /* update layer */
                ecos::ecs.emplace_or_replace<gfix::com_glayer_t>(ent, glayer_e);
            } /* update layer */
        }
    } static gfix_listener;
    entt::sigh_helper{ecos::ecs}.with<gfix::com_rlayer_t>()
    .on_construct<&gfix_listener_t::update_glayer>(gfix_listener)
    .on_destroy<&gfix_listener_t::update_glayer>(gfix_listener)
    .on_update<&gfix_listener_t::update_glayer>(gfix_listener);
#endif
    /* vrange */
#if FALSE
    struct vrange_listener_t
    {
        void update_vrange(ecos::ecs_t&ecos::ecs, ent_t ent)
        {
            auto vrange = ecos::ecs.get<com_vrange_t>(ent);
            auto range1 = vrange.range;
            auto range2 = range1*range1;
            zpos1_t zpos1 = ecos::ecs.get<com_zpos1_t>(ent);
            for (auto&&[entity, visual, tcell, tpos3]
                : ecos::ecs.view<
                com_visual_t, com_tcell_t, com_tpos3_t
                >().each()
            ) {
                if (tpos3.z > zpos1.z
                    && ((tpos3.x*tpos3.x + tpos3.y*tpos3.y
                    ) <= range2)
                ) {
                    ecos::ecs.replace<com_visual_t>(entity, -1);
                }
                else
                {
                    ecos::ecs.replace<com_visual_t>(entity, +1);
                }
            }
        } /* update */
        void update_tcell(ecos::ecs_t&ecos::ecs, ent_t ent)
        {
            for (auto entity : ecos::ecs.view<com_vrange_t>())
            { ecos::ecs.patch<com_vrange_t>(entity); }
        }
    } static vrange_listener;
    entt::sigh_helper{ ecos::ecs }
    .with<com_vrange_t>().on_update<
        &vrange_listener_t::update_vrange
        >(vrange_listener).on_construct<
        &vrange_listener_t::update_vrange
        >(vrange_listener);
    entt::sigh_helper{ ecos::ecs }
    .with<com_tcell_t>().on_construct<
        &vrange_listener_t::update_tcell
        >(vrange_listener);
#endif
    _EFNOT(view_t::get().init(), return FALSE, "");
    _EFNOT(guis_t::get().init(), return FALSE, "");
    /* init is done */
    gfix::init_bot_signal.holder.publish();
    return TRUTH;
}
bool quit()
{
    gfix::quit_top_signal.holder.publish();
    quit_image();
    gfix::quit_bot_signal.holder.publish();
    return TRUTH;
}
bool work()
{
    gfix::work_top_signal.holder.publish();
    /* clear */
    ::glutSwapBuffers();
    ::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ::glClear(GL_COLOR_BUFFER_BIT);
    /* prepare the view */
    auto view_entity = ecos::ecs.ctx().get<ent_t>("view_entity"_shstr);
    auto view_grect = ecos::ecs.get<geom::com_grect_t>(view_entity);
    /* count all visible components */
    count_t count = 0;
    for (auto&&
        [entity, visual]
        : ecos::ecs.view<
        com_visual_t
        >().each()
    ) { count++; }
    /* make a list sorted by depth and layers */
    typedef struct drawable_t {
        entity_t entity;
        gfix::glayer_t glayer;
        geom::gpos3_t gpos3;
        geom::grect_t grect;
    } drawable_t;
    typedef struct drawlist_t {
        drawable_t*head,*back,*tail;
    } drawlist_t;
    drawlist_t drawlist;
    memo::pager_t::get()->new_mul(drawlist.head, count);
    drawlist.back = drawlist.head;
    drawlist.tail = drawlist.head + count;
    auto hero_entity = ecos::ecs.ctx().get<entity_t>("hero_entity"_shstr);
    auto hero_tpos3 = ecos::ecs.get<game::com_tpos3_t>(hero_entity);
    for (auto
        [entity, visual, glayer, gpos3_e, tpos3_e, grect_e]
        : ecos::ecs.view<
        gfix::com_visual_t,gfix::com_glayer_t,
        geom::com_gpos3_t, game::com_tpos3_t, geom::com_grect_t
        >().each()
    ) {
        if constexpr (TRUTH)
        { /* relative to every vrange owner */
            for (auto&&
                [entity, vrange, tpos3_v]
                : ecos::ecs.view<
                gfix::com_vrange_t, game::com_tpos3_t
                >().each()
            ) {
                game::tpos3_t tdiff = {
                    .x = tpos3_e.x - tpos3_v.x,
                    .y = tpos3_e.y - tpos3_v.y,
                    .z = tpos3_e.z - tpos3_v.z,
                };
                tdiff.y += tdiff.z;
                auto range2 = vrange.range * vrange.range + vrange.range;
                if ((tdiff.z > 0)
                    && ((tdiff.x*tdiff.x + tdiff.y*tdiff.y)
                        <= range2)
                ) { visual.set(visual_t::SOFT_HIDE); break; }
                else {  visual.set(visual_t::SOFT_SHOW); }
            }
        } /* relative to every vrange owner */
        if (visual.vet() && ((FALSE
        ) || (TRUTH
        && ((grect_e.x.l <= view_grect.x.r) && (grect_e.y.b <= view_grect.y.t))
        && ((grect_e.x.l >= view_grect.x.l) && (grect_e.y.b >= view_grect.y.b))
        ) || (TRUTH
        && ((grect_e.x.l <= view_grect.x.r) && (grect_e.y.t <= view_grect.y.t))
        && ((grect_e.x.l >= view_grect.x.l) && (grect_e.y.t >= view_grect.y.b))
        ) || (TRUTH
        && ((grect_e.x.r <= view_grect.x.r) && (grect_e.y.b <= view_grect.y.t))
        && ((grect_e.x.r >= view_grect.x.l) && (grect_e.y.b >= view_grect.y.b))
        ) || (TRUTH
        && ((grect_e.x.r <= view_grect.x.r) && (grect_e.y.t <= view_grect.y.t))
        && ((grect_e.x.r >= view_grect.x.l) && (grect_e.y.t >= view_grect.y.b))
        ) || (TRUTH
        && ((grect_e.x.l <= view_grect.x.l) && (grect_e.x.r >= view_grect.x.r))
        && ((grect_e.y.b >= view_grect.y.b) || (grect_e.y.t >= view_grect.y.t))
        ) || (TRUTH
        && ((grect_e.y.b <= view_grect.y.b) && (grect_e.y.t >= view_grect.y.t))
        && ((grect_e.x.l >= view_grect.x.l) || (grect_e.x.r <= view_grect.x.r))
        )) && ((grect_e.x.l != grect_e.x.r) || (grect_e.y.b != grect_e.y.t))) {
            geom::gpos3_t gpos3 = gpos3_e;
            geom::grect_t grect = grect_e;
            if constexpr (TRUTH)
            { /* relative to hero */
                if (ecos::ecs.try_get<geom::com_zpos1_t>(entity))
                {
                    game::tpos3_t tdiff = {
                        .x = tpos3_e.x - hero_tpos3.x,
                        .y = tpos3_e.y - hero_tpos3.y,
                        .z = tpos3_e.z - hero_tpos3.z,
                    };
                    if (auto*color = ecos::ecs.try_get<com_color_t>(entity))
                    //{ color->v = 0xff - (std::abs(tdiff.z) * 0x40); }
                    { color->v = 0x80 + (tdiff.z * 0x40); }
                    grect.y.b += tdiff.z * game::TCELL_ASIZE_Y;
                    grect.y.t += tdiff.z * game::TCELL_ASIZE_Y;
                }
            } /* relative to hero */
            *drawlist.back++ = drawable_t{
                .entity = entity,
                .glayer = glayer,
                .gpos3 = gpos3,
                .grect = grect,
            };
        }
    } /* fill the drawlist */
    static constexpr auto sortfunc = [](
        const drawable_t&sortl,
        const drawable_t&sortr
    ) {
        if (sortl.gpos3.z == sortr.gpos3.z)
        { return sortl.glayer.layer < sortr.glayer.layer; }
        else
        { return sortl.gpos3.z < sortr.gpos3.z; }
    };
    drawlist.tail = drawlist.back + 1;
    std::sort(drawlist.head, drawlist.tail, sortfunc);
    for (auto*drawable = drawlist.head;
        drawable < drawlist.tail; drawable++
    ) {
        entity_t entity = drawable->entity;
        geom::grect_t grect = drawable->grect;
        gfix::color_t color = { 0xff };
        if (auto*tcomp = ecos::ecs.try_get<gfix::com_color_t>(entity))
        { color =*tcomp; }
        ::glColor3ub(color.v, color.v, color.v); 
        if (auto*imreg = ecos::ecs.try_get<gfix::com_imreg_t>(entity);
            auto*irect = ecos::ecs.try_get<gfix::com_irect_t>(entity)
        ) { /* quads */
            gfix::imori_t*imori = get_imori_from_index(imreg->index);
            gfix::isize_t imori_isize = imori->isize;
            struct{struct{v1f_t l,r;}x;struct{v1f_t b,t;}y;}frect;
            frect = {
                .x = {
                    .l = static_cast<v1f_t>(irect->x.l)
                    /static_cast<v1f_t>(imori_isize.x),
                    .r = static_cast<v1f_t>(irect->x.r)
                    /static_cast<v1f_t>(imori_isize.x),
                },
                .y = {
                    .b = static_cast<v1f_t>(irect->y.b)
                    /static_cast<v1f_t>(imori_isize.y),
                    .t = static_cast<v1f_t>(irect->y.t)
                    /static_cast<v1f_t>(imori_isize.y),
                },
            };
            ::glBindTexture(GL_TEXTURE_2D, imori->glint);
            ::glBegin(GL_QUADS);
            if (auto*label = ecos::ecs.try_get<com_label_t>(entity);
                auto*fonts = ecos::ecs.try_get<com_fonts_t>(entity)
            ) { /* string */
                /* image values */
                isize_t imreg_isize = {
                    .x = irect->x.r - irect->x.l,
                    .y = irect->y.t - irect->y.b,
                };
                struct { v1f_t x, y; } imreg_fsize;
                imreg_fsize = {
                    .x = static_cast<v1f_t>(imreg_isize.x)
                        /static_cast<v1f_t>(imori_isize.x),
                    .y = static_cast<v1f_t>(imreg_isize.y)
                        /static_cast<v1f_t>(imori_isize.y),
                };
                gfix::isize_t glyph_isize = {
                    .x = fonts->glyph.asize.x
                        + ((imreg_isize.x * fonts->glyph.rsize.x)
                            / geom::RSIZE_DIV),
                    .y = fonts->glyph.asize.y
                        + ((imreg_isize.y * fonts->glyph.rsize.y)
                            / geom::RSIZE_DIV),
                };
                struct{ v1f_t x, y; } glyph_fsize;
                glyph_fsize = {
                    .x = static_cast<v1f_t>(glyph_isize.x)
                        /static_cast<v1f_t>(imreg_isize.x),
                    .y = static_cast<v1f_t>(glyph_isize.y)
                        /static_cast<v1f_t>(imreg_isize.y),
                };
                isize_t glyph_count = {
                    .x = imreg_isize.x / glyph_isize.x,
                    .y = imreg_isize.y / glyph_isize.y,
                };
                /* grect values */
                auto msize = label->string.size();
                auto*mdata = label->string.data();
                struct { v1s_t x = 1, y = 1; } schar_count;
                auto schar_count_x = 0;
                for (auto index = 0;
                    (mdata[index] > 0)
                    && (index < msize);
                    index = index + 1
                ) { /* count characters */
                    if (label->string[index] == '\n')
                    {
                        schar_count.y += 1;
                        schar_count_x = 0;
                    }
                    else
                    {
                        schar_count_x++;
                        schar_count.x = nums::get_max(
                            schar_count.x,
                            schar_count_x
                        );
                    }
                } /* count characters */
                geom::gsize_t label_gsize = {
                    .x = grect.x.r - grect.x.l,
                    .y = grect.y.t - grect.y.b,
                };
                geom::gsize_t schar_gsize = {
                    .x = label_gsize.x / schar_count.x,
                    .y = label_gsize.y / schar_count.y,
                };
                schar_gsize.x = nums::get_min(schar_gsize.x, schar_gsize.y);
                schar_gsize.y = nums::get_min(schar_gsize.y, schar_gsize.x);
                geom::gpos3_t label_gpos2 = {
                    .x = grect.x.l - schar_gsize.x,
                    .y = grect.y.t,
                };
                geom::gpos3_t schar_gpos2 = {
                    .x = label_gpos2.x,
                    .y = label_gpos2.y,
                };
                for (auto index = 0;
                    (mdata[index] > 0)
                    && (index < msize);
                    index = index + 1
                ) { /* text rendering */
                    v1s_t mbyte = label->string[index];
                    /* schar grect */
                    if (mbyte == text::ENDL_CHAR)
                    {
                        schar_gpos2.y -= schar_gsize.y;
                        schar_gpos2.x = label_gpos2.x;
                    }
                    else
                    {
                        schar_gpos2.x += schar_gsize.x;
                    }
                    geom::grect_t schar_grect = {
                        .x = {
                            .l = schar_gpos2.x,
                            .r = schar_gpos2.x + schar_gsize.x,
                        },
                        .y = {
                            .b = schar_gpos2.y - schar_gsize.y,
                            .t = schar_gpos2.y,
                        },
                    };
                    mbyte = nums::get_max(
                        mbyte, fonts->first
                    ) - fonts->first;
                    /* glyph frect */
                    ipos2_t glyph_ipos2 = {
                        .x = (mbyte % glyph_count.x)
                            * glyph_isize.x,
                        .y = (glyph_count.y - (mbyte / glyph_count.x))
                            * glyph_isize.y,
                    };
                    struct { v1f_t x, y; } glyph_fpos2;
                    glyph_fpos2 = {
                        .x = static_cast<v1f_t>(glyph_ipos2.x)
                            /static_cast<v1f_t>(imreg_isize.x),
                        .y = static_cast<v1f_t>(glyph_ipos2.y)
                            /static_cast<v1f_t>(imreg_isize.y),
                    };
                    struct{struct{v1f_t l,r;}x;struct{v1f_t b,t;}y;}glyph_frect;
                    glyph_frect = {
                        .x = {
                            .l = glyph_fpos2.x,
                            .r = glyph_fpos2.x + glyph_fsize.x,
                        },
                        .y = {
                            .b = glyph_fpos2.y - glyph_fsize.y,
                            .t = glyph_fpos2.y,
                        },
                    };
                    /* drawing */
                    ::glTexCoord2f(glyph_frect.x.l, glyph_frect.y.b);
                    ::glVertex2i(schar_grect.x.l, schar_grect.y.b);
                    ::glTexCoord2f(glyph_frect.x.r, glyph_frect.y.b);
                    ::glVertex2i(schar_grect.x.r, schar_grect.y.b);
                    ::glTexCoord2f(glyph_frect.x.r, glyph_frect.y.t);
                    ::glVertex2i(schar_grect.x.r, schar_grect.y.t);
                    ::glTexCoord2f(glyph_frect.x.l, glyph_frect.y.t);
                    ::glVertex2i(schar_grect.x.l, schar_grect.y.t);
                } /* text rendering */
            } /* string */
            else
            { /* sprite */
                ::glTexCoord2f(frect.x.l, frect.y.b);
                ::glVertex2i(grect.x.l, grect.y.b);
                ::glTexCoord2f(frect.x.r, frect.y.b);
                ::glVertex2i(grect.x.r, grect.y.b);
                ::glTexCoord2f(frect.x.r, frect.y.t);
                ::glVertex2i(grect.x.r, grect.y.t);
                ::glTexCoord2f(frect.x.l, frect.y.t);
                ::glVertex2i(grect.x.l, grect.y.t);
            } /* sprite */
            ::glEnd();
            ::glBindTexture(GL_TEXTURE_2D, 0);
        } /* quads */
        if (auto*lgrid = ecos::ecs.try_get<geom::com_lgrid_t>(entity))
        { /* lgrid */
            ::glBegin(GL_QUADS);
            const auto&cells = lgrid->cells;
            const auto&tails = lgrid->tails;
            const auto&lsize = lgrid->lsize;
            const auto fromy = grect.y.b - tails.y;
            const auto intoy = grect.y.t + tails.y;
            for (auto stepx = grect.x.l; stepx <= grect.x.r; stepx += cells.x)
            { /* l-to-r */
                ::glVertex2i(stepx - lsize.x, fromy);
                ::glVertex2i(stepx + lsize.x, fromy);
                ::glVertex2i(stepx + lsize.x, intoy);
                ::glVertex2i(stepx - lsize.x, intoy);
            } /* l-to-r */
            const auto fromx = grect.x.l - tails.x;
            const auto intox = grect.x.r + tails.x;
            for (auto stepy = grect.y.b; stepy <= grect.y.t; stepy += cells.x)
            { /* b-to-t */
                ::glVertex2i(fromx, stepy + lsize.y);
                ::glVertex2i(fromx, stepy - lsize.y);
                ::glVertex2i(intox, stepy - lsize.y);
                ::glVertex2i(intox, stepy + lsize.y);
            } /* b-to-t */
            ::glEnd();
        } /* lgrid */
#if defined(_CONF_WORK) && defined(_HELP_LINE)
        ::glColor3ub(0x00, 0xff, 0x00);
        ::glBegin(GL_LINE_STRIP);
        ::glVertex2i(grect.x.l, grect.y.b);
        ::glVertex2i(grect.x.r, grect.y.b);
        ::glVertex2i(grect.x.r, grect.y.t);
        ::glVertex2i(grect.x.l, grect.y.t);
        ::glEnd();
        if (gpos3_t*gpos3 = ecos::ecs.try_get<com_gpos3_t>(entity))
        {
            auto gpos2 =*gpos3;
            ::glBegin(GL_QUADS);
            ::glVertex2i(grect.x.l, gpos2.y-1);
            ::glVertex2i(grect.x.r, gpos2.y-1);
            ::glVertex2i(grect.x.r, gpos2.y+1);
            ::glVertex2i(grect.x.l, gpos2.y+1);
            ::glVertex2i(gpos2.x-1, grect.y.b);
            ::glVertex2i(gpos2.x-1, grect.y.t);
            ::glVertex2i(gpos2.x+1, grect.y.t);
            ::glVertex2i(gpos2.x+1, grect.y.b);
            ::glEnd();
        }
#endif
    }
    memo::pager_t::get()->del_mul(drawlist.head, count);
    gfix::work_bot_signal.holder.publish();
    return TRUTH;
}

} /* actions */

} /* content */

#endif/*SOMIGAME_GFIX_CXX*/
