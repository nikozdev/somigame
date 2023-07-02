#ifndef SOMIGAME_GFIX_IMAGE_CXX
#define SOMIGAME_GFIX_IMAGE_CXX 1

#include "../head.hxx"

/* headers */

#include "image.hxx"
#include "../ecos.hxx"
#include "../geom/sizes.hxx"
#include "../geom/coord.hxx"
#include "../geom/shape.hxx"

namespace somigame { /* content */

namespace gfix { /* consdef */

constexpr const v1s08_t IMORI_NONE_MSTEP = 4;
constexpr const gfix::isize_t IMORI_NONE_ISIZE = { .x = 1, .y = 1, };
ubyte_t IMORI_NONE_MDATA[] = { 0xff,0xff,0xff,0xff };

constexpr const v1s08_t IMORI_TEST_MSTEP = 4;
constexpr const gfix::isize_t IMORI_TEST_ISIZE = { .x = 4, .y = 4, };
ubyte_t IMORI_TEST_MDATA[] = {
    0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa,
    0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55,
    0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa,
    0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55, 0x55,0x55,0x55,0xaa, 0xaa,0xaa,0xaa,0x55,
};

} /* consdef */

namespace gfix { /* datadef */

gfix::imori_t imori_list[gfix::_IMORI_COUNT];

} /* datadef */

namespace gfix { /* actions */

bool init_image()
{
    stbi_set_flip_vertically_on_load(TRUTH);
    load_imori_from_value_into_index({
        .glint = 0,
        .isize = IMORI_NONE_ISIZE,
        .mstep = IMORI_NONE_MSTEP,
        .msize = IMORI_NONE_MSTEP * IMORI_NONE_ISIZE.x * IMORI_NONE_ISIZE.y,
        .mdata = &IMORI_NONE_MDATA[0]
    }, _IMORI_META_NONE);
    load_imori_from_value_into_index(imori_t{
        .glint = 0,
        .isize = IMORI_TEST_ISIZE,
        .mstep = IMORI_TEST_MSTEP,
        .msize = IMORI_TEST_MSTEP * IMORI_TEST_ISIZE.x * IMORI_TEST_ISIZE.y,
        .mdata = &IMORI_TEST_MDATA[0]
    }, _IMORI_META_TEST);
    load_imori_from_fpath_into_index("rsc/gfix/meta-logo.png", _IMORI_META_LOGO);
    load_imori_from_fpath_into_index("rsc/gfix/game-pick.png", _IMORI_GAME_PICK);
    load_imori_from_fpath_into_index("rsc/gfix/game-hero-ai-f1.png", _IMORI_GAME_HERO);
    load_imori_from_fpath_into_index("rsc/gfix/tile-test-st-tmm.png", _IMORI_TILE_TEST);
    load_imori_from_fpath_into_index("rsc/gfix/font-main-8x8.png", _IMORI_FONT_MAIN);
    /* imreg component signal */
#if 0
    entt::sigh_helper{ ecos::ecs }
    .with<com_imreg_t>().on_update<[](ecos::ecs_t&ecos::ecs, ent_t ent){
        auto&imreg = ecos::ecs.get<com_imreg_t>(ent);
        if (imreg.index < 0 || imreg.index >= _IMORI_COUNT)
        { imreg.index = _IMORI_META_TEST; }
        if (ecos::ecs.valid(ent) == FALSE) { return; }
        /* sizes */
        isize_t isize = { 0, 0 };
        /* coord */
        ipos2_t ipos2 = { 0, 0 };
        /* irect */
        irect_t irect = { { 0, 0 }, { 0, 0 } };
        /* imreg */
        if (auto*imreg = ecos::ecs.try_get<gfix::com_imreg_t>(ent))
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
        ecos::ecs.emplace_or_replace<com_isize_t>(ent, isize);
        /* update coord */
        ecos::ecs.emplace_or_replace<com_ipos2_t>(ent, ipos2);
        /* update irect */
        ecos::ecs.emplace_or_replace<com_irect_t>(ent, irect);
    }>().on_construct<[](ecos::ecs_t&ecos::ecs, ent_t ent){
        auto&imreg = ecos::ecs.get<com_imreg_t>(ent);
        if (imreg.index < 0 || imreg.index >= _IMORI_COUNT)
        { imreg.index = _IMORI_META_TEST; }
    }>().on_destroy<[](ecos::ecs_t&ecos::ecs, ent_t ent){
        auto&imreg = ecos::ecs.get<com_imreg_t>(ent);
    }>();
#endif
    /* faces component signal */
    entt::sigh_helper{ ecos::ecs }
    .with<gfix::com_faces_t>().on_update<[](ecs_t&ecs, ent_t ent){
    }>().on_construct<[](ecs_t&ecs, ent_t ent){
        auto&faces = ecs.get<com_faces_t>(ent);
    }>().on_destroy<[](ecs_t&ecs, ent_t ent){
        auto&faces = ecs.get<com_faces_t>(ent);
    }>();
    return TRUTH;
}
bool quit_image()
{
    return TRUTH;
}

static bool proc_imori(imori_t&imori)
{
    imori.msize = imori.isize.x * imori.isize.y * imori.mstep;
    GLuint glint = 0;
    ::glGenTextures(1, &glint);
    ::glBindTexture(GL_TEXTURE_2D, glint);
    ::glTexImage2D(
        GL_TEXTURE_2D, 0,
        imori.mstep, imori.isize.x, imori.isize.y, 0,
        imori.mstep == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
        imori.mdata
    );
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    ::glBindTexture(GL_TEXTURE_2D, 0);
    imori.glint = glint;
    return TRUTH;
}

bool load_imori_from_fpath_into_value(const std::string&fpath, imori_t&imori)
{
    imori.mdata = stbi_load(&fpath[0], &imori.isize.x, &imori.isize.y, &imori.mstep, 0);
    if (imori.mdata) { return proc_imori(imori); }
    else
    { std::cerr << "failed imori loading: " << fpath << std::endl; return FALSE; }
}

bool load_imori_from_fpath_into_index(const std::string&fpath, index_t index)
{
    if (index < _IMORI_COUNT)
    {
        return load_imori_from_fpath_into_value(fpath, imori_list[index]);
    }
    else
    {
        return FALSE;
    }
}

bool load_imori_from_value_into_index(const imori_t&imori, index_t index)
{
    if (index < _IMORI_COUNT)
    {
        imori_list[index] = imori;
        return proc_imori(imori_list[index]);
    }
    else
    {
        return FALSE;
    }
}

} /* actions */

namespace gfix { /* getters */

imori_t*get_imori_from_index(index_t index)
{ return&imori_list[index % _IMORI_COUNT]; }

} /* getters */

} /* content */

#endif/*SOMIGAME_GFIX_IMAGE_CXX*/
