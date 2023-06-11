#pragma once

/* headers */

#include "head.hxx"
#include "ecos.hxx"

#include <GL/glut.h>

/* content */

_NAMESPACE_ENTER

/** enumdef **/

typedef enum screen_from_e {
    _SCREEN_FROM_C,
    _SCREEN_FROM_L, _SCREEN_FROM_R, _SCREEN_FROM_B, _SCREEN_FROM_T,
    _SCREEN_FROM_LB, _SCREEN_FROM_RB, _SCREEN_FROM_LT, _SCREEN_FROM_RT,
} screen_from_e;

typedef enum layerid_e {
    _LAYERID_BG = 0x0'000,
    _LAYERID_MG = 0x1'000,
    _LAYERID_FG = 0x2'000,
    _LAYERID_UI = 0x3'000,
} layerid_e;

/** typedef **/

/*** entity component system ***/

typedef struct com_screen_from_t {
    screen_from_e from = _SCREEN_FROM_C;
} com_screen_from_t;

typedef struct com_visual_t {
    layerid_e   layerid = _LAYERID_BG;
    bool_t      visible = _TRUTH;
} com_visual_t;

typedef struct com_color_t {
    unsigned char r = _ZERO, g = _ZERO, b = _ZERO;
} com_color_t;

typedef struct com_image_t {
    index_t index = 0;
} com_image_t;

typedef struct com_pixel_data_t {
    unsigned char*mdata = _NULL;
    size_t msize = 0;
} com_pixel_data_t;

typedef struct com_pixel_size_t {
    int value = 3;
} com_pixel_size_t;

/*** vision ***/

typedef struct camera_t {
    com_scale_t scale = { .x = _UNIT, .y = _UNIT };
    com_coord_t coord = { .x = _ZERO, .y = _ZERO };
    com_sizes_t sizes = { .w = 256, .h = 256 };
} camera_t;

/*** visible ***/

typedef struct image_t {
    GLuint index = 0;
    com_pixel_size_t pixel_size = { .value = 3 };
    com_pixel_data_t pixel_data = { .mdata = _NULL, .msize = _ZERO };
    com_sizes_t sizes = { .w = _ZERO, .h = _ZERO };
} image_t;

typedef struct quad_t {
    com_coord_t coord = { .x = _ZERO, .y = _ZERO, };
    com_sizes_t sizes = { .w = _UNIT, .h = _UNIT, };
    com_scale_t scale = { .x = _UNIT, .y = _UNIT, };
    com_color_t color = { .r = 128, .g = 128, .b = 128 };
} quad_t;

typedef struct text_t {
    com_string_t string = { .mdata = _NULL, .msize = _ZERO };
    com_coord_t coord = { .x = _ZERO, .y = _ZERO };
    com_sizes_t sizes = { .w = _UNIT, .h = _UNIT };
    com_scale_t scale = { .x = _UNIT, .y = _UNIT };
    com_color_t color = { .r = 255, .g = 255, .b = 255 };
} text_t;

/*** system ***/

typedef struct window_t {
    com_coord_t coord;
    com_sizes_t sizes;
} window_t;

typedef struct gfix_t {
    window_t window;
    camera_t camera;
    std::vector<image_t> image_list;
} gfix_t;

/** datadef **/

extern gfix_t gfix;

/** actions **/

extern void gfix_init();

extern void draw_loop();

extern void draw_global_quad(const quad_t&quad);
extern void draw_global_text(const text_t&text);
extern void draw_screen_quad(const quad_t&quad, screen_from_e from = _SCREEN_FROM_C);
extern void draw_screen_text(const text_t&text, screen_from_e from = _SCREEN_FROM_C);

extern void load_image(const std::string&path, image_t*image);
extern void load_image_index(const std::string&path, index_t index);

extern void view_move(const com_coord_t&coord);
extern void view_goto(const com_coord_t&coord);
extern void view_zoom(const com_scale_t&scale);

_NAMESPACE_LEAVE
