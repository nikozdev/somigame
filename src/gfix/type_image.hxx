#ifndef SOMIGAME_GFIX_TYPE_IMAGE_HXX
#define SOMIGAME_GFIX_TYPE_IMAGE_HXX

#include "../head.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"

namespace somigame { namespace gfix {

// enumdef

typedef enum faces_e : index_t {
    _FACES_F = 0x0,
    _FACES_L,
    _FACES_B,
    _FACES_R,
    _FACES_COUNT,
} faces_e; // entity faces

typedef enum imori_e : index_t {
    _IMORI_META_NONE = 0x0,
    _IMORI_META_TEST = 0x1,
    _IMORI_META_LOGO,
    _IMORI_GAME_PICK,
    _IMORI_GAME_HERO,
    _IMORI_TILE_TEST,
    _IMORI_FONT_TEST,
    _IMORI_FONT_MAIN,
    _IMORI_COUNT,
} imori_e; // image origins

// typedef

typedef struct isize_t {
    v1s_t x = 0, y = 0;
} isize_t, com_isize_t; /* image size */
typedef struct iposi_t {
    v1s_t x = 0, y = 0;
} iposi_t, com_iposi_t; /* image position */
typedef struct irect_t {
    struct x_t {
        v1s_t l = 0, r = 0;
    } x;
    struct y_t {
        v1s_t b = 0, t = 0;
    } y;
} irect_t, com_irect_t; /* image rectangle */

typedef struct imori_t {
    v1u32_t glint = 0;
    isize_t isize = { 0, 0 };
    v1s32_t mstep = 0;
    msize_t msize = 0;
    udata_t mdata = NULL;
} imori_t;
typedef struct imreg_t {
    index_t index = _IMORI_META_TEST;
    geom::pivot_t pivot = { geom::PIVOT_MIN, geom::PIVOT_MIN };
    geom::aposi_t aposi = { 0, 0 };
    geom::rposi_t rposi = { geom::RPOSI_MIN, geom::RPOSI_MIN };
    geom::asize_t asize = { 0, 0 };
    geom::rsize_t rsize = { geom::RSIZE_MAX, geom::RSIZE_MAX };
} imreg_t, com_imreg_t;

typedef struct faces_t {
    gfix::imreg_t ilist[_FACES_COUNT];
} faces_t, com_faces_t;

// actions

_DECL_FUNC v1bit_t init_type_image();
_DECL_FUNC v1bit_t quit_type_image();

_DECL_FUNC v1bit_t load_imori_from_fpath_into_value(const sdstr_t&fpath, imori_t&imori);
_DECL_FUNC v1bit_t load_imori_from_fpath_into_index(const sdstr_t&fpath, index_t index);
_DECL_FUNC v1bit_t load_imori_from_value_into_index(const imori_t&imori, index_t index);

// getters

_DECL_FUNC imori_t*get_imori_from_index(index_t index);

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_IMAGE_HXX
