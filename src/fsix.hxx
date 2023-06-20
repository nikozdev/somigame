#pragma once

/* headers */

#include "head.hxx"

/* content */

_NAMESPACE_ENTER

/** consdef **/

constexpr auto FRICT_MIN = 0x00;
constexpr auto FRICT_MID = 0x80;
constexpr auto FRICT_MAX = 0xff;
constexpr auto FRICT_DIV = 0xff;

constexpr auto WEIGH_MIN = 0x02;
constexpr auto WEIGH_MAX = 0xff;

constexpr auto FORCE_SCALE = 0x888;

/** typedef **/

typedef struct frict_t {
    v1s_t value = FRICT_MIN;
} frict_t;

typedef struct weigh_t {
    v1s_t value = WEIGH_MIN;
} weigh_t;

typedef struct force_t {
    v1s_t x = 0, y = 0;
} force_t;

typedef struct mover_t {
    v1s_t x = 0, y = 0;
} mover_t;

/** datadef **/

/** actions **/

extern void fsix_init();

extern void fsix_loop();

_NAMESPACE_LEAVE
