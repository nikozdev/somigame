#pragma once

/* headers */

#include "head.hxx"

#include <GL/glut.h>

/* content */

_NAMESPACE_ENTER

/** typedef **/

typedef enum screen_from_e {
    _SCREEN_FROM_C,
    _SCREEN_FROM_L, _SCREEN_FROM_R, _SCREEN_FROM_B, _SCREEN_FROM_T,
    _SCREEN_FROM_LB, _SCREEN_FROM_RB, _SCREEN_FROM_LT, _SCREEN_FROM_RT,
} screen_from_e;

typedef struct window_t
{
    struct { int x = 0, y = 0; } coord;
    struct { int w = 256, h = 256; } sizes;
} window_t;

typedef struct camera_t
{
    int scale = 1;
    struct { int x = 0, y = 0; } coord;
    struct { int w = 256, h = 256; } sizes;
} camera_t;

typedef struct image_t {
    GLuint index;
    unsigned char*mdata = _NULL;
    size_t msize = 0;
    int nchan = 3;
    struct { int w = 0, h = 0; } sizes;
} image_t;

typedef struct quad_t {
    int scale = 1;
    struct { int x = 0, y = 0; } coord;
    struct { int w = 0, h = 0; } sizes;
    struct { unsigned char r = 0, g = 0, b = 0; } color;
    image_t image;
} quad_t;

typedef struct text_t {
    char*mdata;
    size_t msize;
    int scale = 1;
    struct { int x = 0, y = 0; } coord;
    struct { int w = 8, h = 8; } sizes;
    struct { unsigned char r = 0, g = 0, b = 0; } color;
} text_t;

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

extern void draw_global_quad(quad_t quad);
extern void draw_screen_quad(quad_t quad, screen_from_e from = _SCREEN_FROM_C);
extern void draw_screen_text(text_t text, screen_from_e from = _SCREEN_FROM_C);

extern void load_image(std::string path, image_t*image);
extern void load_image_index(std::string path, size_t index);

extern void view_move(int stepx, int stepy);
extern void view_scale(int scale);

_NAMESPACE_LEAVE
