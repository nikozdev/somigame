#pragma once

/* headers */

#include "head.hxx"
#include "ecos.hxx"

/* content */

_NAMESPACE_ENTER

/** consdef **/

constexpr int RATIO_X = 16;
constexpr int RATIO_Y = 9;

constexpr int CAMERA_SIZES_W = UNIT_SCALE_X * RATIO_X;
constexpr int CAMERA_SIZES_H = UNIT_SCALE_Y * RATIO_Y;

constexpr int GUIS_SIZES_X = UNIT_SCALE_X;
constexpr int GUIS_SIZES_Y = UNIT_SCALE_X;
constexpr int GUIS_LAYER = UNIT_COUNT_Z/2+1;

/** enumdef **/

/*** entity face ***/
typedef enum eface_e : index_t {
    _EFACE_F = 0x0,
    _EFACE_L,
    _EFACE_B,
    _EFACE_R,
    _EFACE_COUNT,
} eface_e;

typedef enum image_e : index_t {
    _IMAGE_META_NONE = 0x0,
    _IMAGE_META_TEST = 0x1,
    _IMAGE_META_LOGO,
    _IMAGE_ENTT_HERO,
    _IMAGE_TILE_TEST,
    _IMAGE_FONT_TEST,
    _IMAGE_FONT_MAIN,
    _IMAGE_COUNT,
} image_e;

/** typedef **/

typedef struct color_t {
    unsigned char value = 0u;
} color_t, com_color_t;

typedef struct image_origin_t {
    index_t glint = 0;
    com_sizes_t sizes = { .w = 0, .h = 0 };
    int mstep = 0;
    size_t msize = 0;
    unsigned char*mdata = _NULL;
} image_t, image_origin_t;

typedef struct image_region_t {
    index_t index = _IMAGE_META_TEST;
    pos2d_t coord = { 0, 0 };
    sizes_t sizes = { UNIT_SCALE_X, UNIT_SCALE_Y };
} image_region_t, com_image_t;

typedef struct faces_t {
    image_region_t ilist[_EFACE_COUNT];
} faces_t, com_faces_t;

typedef struct image_holder_t {
    image_region_t faces[_EFACE_COUNT] = {
        [_EFACE_F] = {
            .index = _IMAGE_META_NONE,
            .coord = pos2d_t { .x = 0, .y = 0 },
            .sizes = sizes_t { .w = 0, .h = 0 },
        },
        [_EFACE_L] = {
            .index = _IMAGE_META_NONE,
            .coord = pos2d_t { .x = 0, .y = 0 },
            .sizes = sizes_t { .w = 0, .h = 0 },
        },
        [_EFACE_B] = {
            .index = _IMAGE_META_NONE,
            .coord = pos2d_t { .x = 0, .y = 0 },
            .sizes = sizes_t { .w = 0, .h = 0 },
        },
        [_EFACE_R] = {
            .index = _IMAGE_META_NONE,
            .coord = pos2d_t { .x = 0, .y = 0 },
            .sizes = sizes_t { .w = 0, .h = 0 },
        },
    };
} image_holder_t;

typedef struct visual_t {
    bool_t valid = _TRUTH;
    index_t layer = 0;
} visual_t, com_visual_t;

/*** system ***/

typedef struct gfix_t {
    ent_t ent_guis = entt::null;
} gfix_t;

/** datadef **/

extern gfix_t gfix;

/** getters **/

inline auto get_anchor_coord(
    const auto anchor_x = RELPOS_MID,
    const auto sizes_w = 0,
    const auto scale_x = 1
) { return (sizes_w * scale_x * anchor_x) / RELPOS_DIV; }
inline auto get_anchor_coord(
    const anchor_t&anchor = { RELPOS_MID, RELPOS_MID },
    const sizes_t&sizes = {0,0},
    const scale_t&scale = {1,1}
) {
    return coord_t{
        get_anchor_coord(anchor.x, sizes.w, scale.x),
        get_anchor_coord(anchor.y, sizes.h, scale.y),
        0
    };
}

extern const direc_t&get_camera_direc();

/** actions **/

extern void gfix_init();

extern void gfix_loop();

extern void load_image_from_fpath_into_value(const std::string&fpath, image_t&image);
extern void load_image_from_fpath_into_index(const std::string&fpath, index_t index);
extern void load_image_from_value_into_index(const image_t&image, index_t index);

extern void view_move(const pos3d_t&coord = {0,0,0});
extern void view_goto(const pos3d_t&coord = {0,0,0});
extern void view_zoom(const scale_t&scale);
extern void view_turn(bool lside = _TRUTH);

_NAMESPACE_LEAVE
