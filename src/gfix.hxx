#pragma once

/* headers */

#include "head.hxx"

#include <GL/glut.h>

/* content */

_NAMESPACE_ENTER

/** typedef **/

typedef struct window_t
{
    struct { size_t x = 0, y = 0; } coord;
    struct { size_t w = 256, h = 256; } sizes;
} window_t;

typedef struct camera_t
{
    float scale = 1.0;
    struct { float x = 0.0, y = 0.0; } coord;
    struct { float w = 1.0, h = 1.0; } sizes;
} camera_t;

typedef struct com_gfix_sprite_t
{
    struct { float x = 0.0, y = 0.0; } sizes;
    struct { float r = 0.0, g = 0.0, b = 0.0, a = 0.0; } color;
} com_gfix_sprite_t;

typedef struct gfix_t
{
    window_t window;
    camera_t camera;
    std::vector<com_gfix_sprite_t> com_list;
} gfix_t;

/** datadef **/

extern gfix_t gfix;

_NAMESPACE_LEAVE
