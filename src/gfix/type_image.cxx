#ifndef SOMIGAME_GFIX_TYPE_IMAGE_CXX
#define SOMIGAME_GFIX_TYPE_IMAGE_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../gfix/type_image.hxx"

namespace somigame { namespace gfix {

// consdef

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

// datadef

gfix::imori_t imori_list[gfix::_IMORI_COUNT];

// actions

static void on_proc(ecos::reg_t&reg, ecos::ent_t ent)
{
    auto&imreg = ecos::reg.get<com_imreg_t>(ent);
    if (imreg.index < 0 || imreg.index >= _IMORI_COUNT)
    { imreg.index = _IMORI_META_TEST; }
    if (ecos::reg.valid(ent) == FALSE) { return; }
    // sizes
    gfix::isize_t isize = { 0, 0 };
    // coord
    gfix::iposi_t iposi = { 0, 0 };
    // rects
    gfix::irect_t irect = { { 0, 0 }, { 0, 0 } };
    // imreg
    if (auto*imreg = ecos::reg.try_get<gfix::com_imreg_t>(ent))
    { // imreg
        auto*imori = gfix::get_imori_from_index(imreg->index);
        // coord
        iposi.x = imreg->aposi.x
        + (imori->isize.x * (imreg->rposi.x - geom::RPOSI_MIN) / geom::RPOSI_DIV);
        iposi.y = imreg->aposi.y
        + (imori->isize.y * (imreg->rposi.y - geom::RPOSI_MIN) / geom::RPOSI_DIV);
        // sizes
        isize.x = imreg->asize.x
        + (imreg->rsize.x * imori->isize.x / geom::RSIZE_DIV);
        isize.y = imreg->asize.y
        + (imreg->rsize.y * imori->isize.y / geom::RSIZE_DIV);
        // rects
        irect.x.l = iposi.x
        + ((isize.x * (geom::PIVOT_MIN - imreg->pivot.x))
            / geom::PIVOT_DIV);
        irect.x.r = iposi.x
        + ((isize.x * (geom::PIVOT_MAX - imreg->pivot.x))
            / geom::PIVOT_DIV);
        irect.y.b = iposi.y
        + ((isize.y * (geom::PIVOT_MIN - imreg->pivot.y))
            / geom::PIVOT_DIV);
        irect.y.t = iposi.y
        + ((isize.y * (geom::PIVOT_MAX - imreg->pivot.y))
            / geom::PIVOT_DIV);
    } // imreg
    // update sizes
    ecos::reg.emplace_or_replace<com_isize_t>(ent, isize);
    // update coord
    ecos::reg.emplace_or_replace<com_iposi_t>(ent, iposi);
    // update rects
    ecos::reg.emplace_or_replace<com_irect_t>(ent, irect);
} // on_proc
static void on_ctor(ecos::reg_t&reg, ecos::ent_t ent)
{ return on_proc(reg, ent); }
static void on_dtor(ecos::reg_t&reg, ecos::ent_t ent)
{ }

_DEFN_FUNC v1bit_t init_type_image()
{
    // files
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
    // imreg component signal
    ecos::reg.on_ctor<gfix::com_imreg_t>().connect<on_ctor>();
    ecos::reg.on_dtor<gfix::com_imreg_t>().connect<on_dtor>();
    ecos::reg.on_proc<gfix::com_imreg_t>().connect<on_proc>();
    // final
    return TRUTH;
} // init_type_image
_DEFN_FUNC v1bit_t quit_type_image()
{
    // imreg component signal
    ecos::reg.on_ctor<gfix::com_imreg_t>().disconnect<on_ctor>();
    ecos::reg.on_dtor<gfix::com_imreg_t>().disconnect<on_dtor>();
    ecos::reg.on_proc<gfix::com_imreg_t>().disconnect<on_proc>();
    // final
    return TRUTH;
} // quit_type_image

static v1bit_t proc_imori(imori_t&imori)
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
} // proc_imori

_DEFN_FUNC v1bit_t load_imori_from_fpath_into_value(const std::string&fpath, imori_t&imori)
{
    imori.mdata = stbi_load(&fpath[0], &imori.isize.x, &imori.isize.y, &imori.mstep, 0);
    if (imori.mdata) { return proc_imori(imori); }
    else
    { std::cerr << "failed imori loading: " << fpath << std::endl; return FALSE; }
} // load_imori_from_fpath_into_value

_DEFN_FUNC v1bit_t load_imori_from_fpath_into_index(const std::string&fpath, index_t index)
{
    if (index < _IMORI_COUNT)
    {
        return load_imori_from_fpath_into_value(fpath, imori_list[index]);
    }
    else
    {
        return FALSE;
    }
} // load_imori_from_fpath_into_index

_DEFN_FUNC v1bit_t load_imori_from_value_into_index(const imori_t&imori, index_t index)
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
} // load_imori_from_value_into_index

// getters

_DEFN_FUNC imori_t*get_imori_from_index(index_t index)
{ return&imori_list[index % _IMORI_COUNT]; }

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_IMAGE_CXX
