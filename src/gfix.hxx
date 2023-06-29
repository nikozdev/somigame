#pragma once

/* headers */

#include "head.hxx"
#include "util.hxx"

/* content */

_NAMESPACE_ENTER

/** consdef **/

constexpr auto VIEW_RATIO_X = 16;
constexpr auto VIEW_RATIO_Y = 9;

constexpr auto VIEW_ASIZE_X = UNIT_ASIZE_X * VIEW_RATIO_X;
constexpr auto VIEW_ASIZE_Y = UNIT_ASIZE_Y * VIEW_RATIO_Y;

constexpr auto GUIS_ASIZE_X = UNIT_ASIZE_X * 2;
constexpr auto GUIS_ASIZE_Y = UNIT_ASIZE_Y;
constexpr auto GUIS_RSIZE_X = RSIZE_MAX * 4 / VIEW_RATIO_X;
constexpr auto GUIS_RSIZE_Y = RSIZE_MAX / VIEW_RATIO_Y;
constexpr auto GUIS_LAYER = 0xff;

/** enumdef **/

typedef enum eface_e : index_t {
    _EFACE_F = 0x0,
    _EFACE_L,
    _EFACE_B,
    _EFACE_R,
    _EFACE_COUNT,
} eface_e; /*** entity face ***/

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
} imori_e; /* image origins */

/** forward **/

typedef struct family_t family_t;

/** typedef **/

typedef struct color_t {
    ubyte_t v = 0u;
} color_t;

typedef struct imori_t {
    index_t glint = 0;
    isize_t isize = { 0, 0 };
    index_t mstep = 0;
    msize_t msize = 0;
    udata_t mdata = _NULL;
} imori_t;

typedef struct imreg_t {
    index_t index = _IMORI_META_TEST;
    pivot_t pivot = { PIVOT_MIN, PIVOT_MIN };
    apos2_t apos2 = { 0, 0 };
    rpos2_t rpos2 = { RPOS2_MIN, RPOS2_MIN };
    asize_t asize = { 0, 0 };
    rsize_t rsize = { RSIZE_MAX, RSIZE_MAX };
} imreg_t;

typedef struct faces_t {
    imreg_t ilist[_EFACE_COUNT];
} faces_t;

typedef class visual_t
{
public: /* getters */
    v1s_t get() const { return this->value; }
public: /* vetters */
    v1s_t vet() const { return this->value >= 0; }
public: /* setters */
    v1s_t set(v1s_t value)
    {
        if (std::abs(value) >= std::abs(this->value))
        { this->value = value; }
        return this->value;
    }
public: /* consdef */
    constexpr static v1s_t WEAK_HIDE = -0x01;
    constexpr static v1s_t WEAK_SHOW = +0x01;
    constexpr static v1s_t SOFT_HIDE = -0x10;
    constexpr static v1s_t SOFT_SHOW = +0x10;
    constexpr static v1s_t FIRM_HIDE = -0xf0;
    constexpr static v1s_t FIRM_SHOW = +0xf0;
    constexpr static v1s_t HARD_HIDE = -0xff;
    constexpr static v1s_t HARD_SHOW = +0xff;
private: /* datadef */
    v1s_t value = 0; /* (>=0)show or (<0)hide */
} visual_t; /* any visible entity */

typedef struct rlayer_t {
    index_t layer = 1;
} rlayer_t; /* relative layer */
typedef struct glayer_t {
    index_t layer = 1;
} glayer_t; /* global layer */
typedef struct vrange_t {
    v1s_t range = 2;
} vrange_t; /* how much do we see around it ? */

typedef struct ratio_t {
    v1s08_t x = VIEW_RATIO_X, y = VIEW_RATIO_Y;
} ratio_t; /* limit of x-y sizes relative to each other */

typedef struct fonts_t {
    cchar_t first = ' '; /* the first character in the image */
    struct glyph_t {
        /* offsets from glyph to glyph */
        apos2_t apos2 = { 0, 0 };
        rpos2_t rpos2 = { RPOS2_MID, RPOS2_MID };
        /* glyph size */
        asize_t asize = { 8, 8 };
        rsize_t rsize = { RSIZE_MIN, RSIZE_MIN };
    } glyph; /* settings for every character */
} fonts_t; /* font settings */

typedef struct lgrid_t {
    asize_t cells{
        .x = UNIT_ASIZE_X,
        .y = UNIT_ASIZE_Y,
    }; /* sizes of cells */
    asize_t tails{
        .x = 0,
        .y = 0
    }; /* trailing lines from both sides */
    asize_t lsize = {
        .x = 1,
        .y = 1,
    }; /* lines sizes in both axes */
} lgrid_t; /* line grid */

/** actions **/

extern void gfix_init();
extern void gfix_quit();
extern void gfix_work();

extern void load_imori_from_fpath_into_value(const std::string&fpath, imori_t&imori);
extern void load_imori_from_fpath_into_index(const std::string&fpath, index_t index);
extern void load_imori_from_value_into_index(const imori_t&imori_t, index_t index);

extern void view_turn(const bool_t lside = _TRUTH);

/** getters **/

extern imori_t*get_imori_from_index(index_t index);

_NAMESPACE_LEAVE
