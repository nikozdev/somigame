#ifndef SOMIGAME_GFIX_FUNC_WORK_CXX
#define SOMIGAME_GFIX_FUNC_WORK_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_rects.hxx"
#include "../gfix/type_image.hxx"
#include "../gfix/type_label.hxx"
#include "../gfix/type_fonts.hxx"
#include "../gfix/type_layer.hxx"
#include "../gfix/type_visual.hxx"
#include "../gfix/unit_camera.hxx"
#include "../gfix/unit_guiman.hxx"
#include "../game/type_sizes.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_rects.hxx"
#include "../game/type_tcell.hxx"

#undef _HELP_LINE

namespace somigame { namespace gfix {

// signals

_DEFN_DATA signal_t<void(void)>work_top_signal;
_DEFN_DATA signal_t<void(void)>work_bot_signal;

// actions

_DEFN_FUNC bool work()
{
    gfix::work_top_signal.holder.publish();
    // clear
    ::glutSwapBuffers();
    ::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ::glClear(GL_COLOR_BUFFER_BIT);
    // prepare the camera
    auto camera_entity = gfix::get_camera_ent();
    auto camera_grect = ecos::reg.get<geom::com_grect_t>(camera_entity);
    // count all visible components
    count_t count = 0;
    for (auto&&
        [entity, visual]
        : ecos::reg.view<
        com_visual_t
        >().each()
    ) { count += 1; }
    // make a list of drawable entities
    typedef struct drawable_t {
        ecos::ent_t entity;
        gfix::glayer_t glayer;
        geom::gposi_t gposi;
        geom::grect_t grect;
    } drawable_t;
    typedef struct drawlist_t {
        drawable_t*head,*back,*tail;
    } drawlist_t;
    drawlist_t drawlist;
    memo::pager_t::get()->new_mul(drawlist.head, count);
    drawlist.back = drawlist.head;
    drawlist.tail = drawlist.head + count;
    for (auto
        [entity, visual, glayer, gposi_e, tposi_e, grect_e]
        : ecos::reg.view<
        gfix::com_visual_t,gfix::com_glayer_t,
        geom::com_gposi_t, game::com_tposi_t, geom::com_grect_t
        >().each()
    ) { // fill the drawlist
        if (visual.vet() && ((FALSE
        ) || (TRUTH
        && ((grect_e.x.l <= camera_grect.x.r) && (grect_e.y.b <= camera_grect.y.t))
        && ((grect_e.x.l >= camera_grect.x.l) && (grect_e.y.b >= camera_grect.y.b))
        ) || (TRUTH
        && ((grect_e.x.l <= camera_grect.x.r) && (grect_e.y.t <= camera_grect.y.t))
        && ((grect_e.x.l >= camera_grect.x.l) && (grect_e.y.t >= camera_grect.y.b))
        ) || (TRUTH
        && ((grect_e.x.r <= camera_grect.x.r) && (grect_e.y.b <= camera_grect.y.t))
        && ((grect_e.x.r >= camera_grect.x.l) && (grect_e.y.b >= camera_grect.y.b))
        ) || (TRUTH
        && ((grect_e.x.r <= camera_grect.x.r) && (grect_e.y.t <= camera_grect.y.t))
        && ((grect_e.x.r >= camera_grect.x.l) && (grect_e.y.t >= camera_grect.y.b))
        ) || (TRUTH
        && ((grect_e.x.l <= camera_grect.x.l) && (grect_e.x.r >= camera_grect.x.r))
        && ((grect_e.y.b >= camera_grect.y.b) || (grect_e.y.t >= camera_grect.y.t))
        ) || (TRUTH
        && ((grect_e.y.b <= camera_grect.y.b) && (grect_e.y.t >= camera_grect.y.t))
        && ((grect_e.x.l >= camera_grect.x.l) || (grect_e.x.r <= camera_grect.x.r))
        )) && ((grect_e.x.l != grect_e.x.r) || (grect_e.y.b != grect_e.y.t))) {
            geom::gposi_t gposi = gposi_e;
            geom::grect_t grect = grect_e;
            *drawlist.back++ = drawable_t{
                .entity = entity,
                .glayer = glayer,
                .gposi = gposi,
                .grect = grect,
            };
        }
    } // fill the drawlist
    // sort all drawable entities by layers
    static constexpr auto sortfunc = [](
        const drawable_t&sortl,
        const drawable_t&sortr
    ) { return sortl.glayer.layer < sortr.glayer.layer; };
    drawlist.tail = drawlist.back + 1;
    std::sort(drawlist.head, drawlist.tail, sortfunc);
    for (auto*drawable = drawlist.head;
        drawable < drawlist.tail; drawable++
    ) { // draw all entities from the list
        ecos::ent_t entity = drawable->entity;
        geom::grect_t grect = drawable->grect;
        gfix::color_t color = { 0xff };
        if (auto*tcomp = ecos::reg.try_get<gfix::com_color_t>(entity))
        { color =*tcomp; }
        ::glColor3ub(color.v, color.v, color.v); 
        if (auto*imreg = ecos::reg.try_get<gfix::com_imreg_t>(entity);
            auto*irect = ecos::reg.try_get<gfix::com_irect_t>(entity)
        ) { // quads
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
            if (auto*label = ecos::reg.try_get<com_label_t>(entity);
                auto*fonts = ecos::reg.try_get<com_fonts_t>(entity)
            ) { // string
                // image values
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
                // string values
                auto msize = label->string.size();
                auto*mdata = label->string.data();
                struct { v1s_t x = 1, y = 1; } schar_count;
                auto schar_count_x = 0;
                for (auto index = 0;
                    (mdata[index] > 0)
                    && (index < msize);
                    index = index + 1
                ) { // count characters
                    if (mdata[index] == '\n')
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
                } // count characters
                // calculate sizes
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
                // calculare coord
                geom::gposi_t label_gposi = {
                    .x = grect.x.l - schar_gsize.x,
                    .y = grect.y.t,
                };
                geom::gposi_t schar_gposi = {
                    .x = label_gposi.x,
                    .y = label_gposi.y,
                };
                // render the text
                for (auto index = 0;
                    (mdata[index] > 0)
                    && (index < msize);
                    index = index + 1
                ) { // text rendering
                    v1s_t mbyte = mdata[index];
                    // schar grect
                    if (mbyte == text::ENDL_CHAR)
                    {
                        schar_gposi.y -= schar_gsize.y;
                        schar_gposi.x = label_gposi.x;
                    }
                    else
                    {
                        schar_gposi.x += schar_gsize.x;
                    }
                    geom::grect_t schar_grect = {
                        .x = {
                            .l = schar_gposi.x,
                            .r = schar_gposi.x + schar_gsize.x,
                        },
                        .y = {
                            .b = schar_gposi.y - schar_gsize.y,
                            .t = schar_gposi.y,
                        },
                    };
                    mbyte = nums::get_max(
                        mbyte, fonts->first
                    ) - fonts->first;
                    /* glyph frect */
                    iposi_t glyph_iposi = {
                        .x = (mbyte % glyph_count.x)
                            * glyph_isize.x,
                        .y = (glyph_count.y - (mbyte / glyph_count.x))
                            * glyph_isize.y,
                    };
                    struct { v1f_t x, y; } glyph_fposi;
                    glyph_fposi = {
                        .x = static_cast<v1f_t>(glyph_iposi.x)
                            /static_cast<v1f_t>(imreg_isize.x),
                        .y = static_cast<v1f_t>(glyph_iposi.y)
                            /static_cast<v1f_t>(imreg_isize.y),
                    };
                    struct{struct{v1f_t l,r;}x;struct{v1f_t b,t;}y;}glyph_frect;
                    glyph_frect = {
                        .x = {
                            .l = glyph_fposi.x,
                            .r = glyph_fposi.x + glyph_fsize.x,
                        },
                        .y = {
                            .b = glyph_fposi.y - glyph_fsize.y,
                            .t = glyph_fposi.y,
                        },
                    };
                    // drawing
                    ::glTexCoord2f(glyph_frect.x.l, glyph_frect.y.b);
                    ::glVertex2i(schar_grect.x.l, schar_grect.y.b);
                    ::glTexCoord2f(glyph_frect.x.r, glyph_frect.y.b);
                    ::glVertex2i(schar_grect.x.r, schar_grect.y.b);
                    ::glTexCoord2f(glyph_frect.x.r, glyph_frect.y.t);
                    ::glVertex2i(schar_grect.x.r, schar_grect.y.t);
                    ::glTexCoord2f(glyph_frect.x.l, glyph_frect.y.t);
                    ::glVertex2i(schar_grect.x.l, schar_grect.y.t);
                } // text rendering
            } // string
            else
            { // sprite
                ::glTexCoord2f(frect.x.l, frect.y.b);
                ::glVertex2i(grect.x.l, grect.y.b);
                ::glTexCoord2f(frect.x.r, frect.y.b);
                ::glVertex2i(grect.x.r, grect.y.b);
                ::glTexCoord2f(frect.x.r, frect.y.t);
                ::glVertex2i(grect.x.r, grect.y.t);
                ::glTexCoord2f(frect.x.l, frect.y.t);
                ::glVertex2i(grect.x.l, grect.y.t);
            } // sprite
            ::glEnd();
            ::glBindTexture(GL_TEXTURE_2D, 0);
        } // quads
        if (auto*lgrid = ecos::reg.try_get<gfix::com_lgrid_t>(entity))
        { // lgrid
            ::glBegin(GL_QUADS);
            const auto&cells = lgrid->cells;
            const auto&tails = lgrid->tails;
            const auto&lsize = lgrid->lsize;
            const auto fromy = grect.y.b - tails.y;
            const auto intoy = grect.y.t + tails.y;
            for (auto stepx = grect.x.l; stepx <= grect.x.r; stepx += cells.x)
            { // l-to-r
                ::glVertex2i(stepx - lsize.x, fromy);
                ::glVertex2i(stepx + lsize.x, fromy);
                ::glVertex2i(stepx + lsize.x, intoy);
                ::glVertex2i(stepx - lsize.x, intoy);
            } // l-to-r
            const auto fromx = grect.x.l - tails.x;
            const auto intox = grect.x.r + tails.x;
            for (auto stepy = grect.y.b; stepy <= grect.y.t; stepy += cells.x)
            { // b-to-t
                ::glVertex2i(fromx, stepy + lsize.y);
                ::glVertex2i(fromx, stepy - lsize.y);
                ::glVertex2i(intox, stepy - lsize.y);
                ::glVertex2i(intox, stepy + lsize.y);
            } // b-to-t
            ::glEnd();
        } // lgrid
#if defined(_CONF_WORK) && defined(_HELP_LINE)
        ::glColor3ub(0x00, 0xff, 0x00);
        ::glBegin(GL_LINES);
        // bside
        ::glVertex2i(grect.x.l, grect.y.b);
        ::glVertex2i(grect.x.r, grect.y.b);
        // rside
        ::glVertex2i(grect.x.r, grect.y.b);
        ::glVertex2i(grect.x.r, grect.y.t);
        // tside
        ::glVertex2i(grect.x.r, grect.y.t);
        ::glVertex2i(grect.x.l, grect.y.t);
        // lside
        ::glVertex2i(grect.x.l, grect.y.t);
        ::glVertex2i(grect.x.l, grect.y.b);
        ::glEnd();
        if (auto*temp = ecos::reg.try_get<geom::com_gposi_t>(entity))
        {
            auto gposi =*temp;
            ::glBegin(GL_QUADS);
            ::glVertex2i(grect.x.l, gposi.y-1);
            ::glVertex2i(grect.x.r, gposi.y-1);
            ::glVertex2i(grect.x.r, gposi.y+1);
            ::glVertex2i(grect.x.l, gposi.y+1);
            ::glVertex2i(gposi.x-1, grect.y.b);
            ::glVertex2i(gposi.x-1, grect.y.t);
            ::glVertex2i(gposi.x+1, grect.y.t);
            ::glVertex2i(gposi.x+1, grect.y.b);
            ::glEnd();
        }
#endif
    } // draw all entities from the list
    memo::pager_t::get()->del_mul(drawlist.head, count);
    gfix::work_bot_signal.holder.publish();
    return TRUTH;
} // work

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_FUNC_WORK_CXX
