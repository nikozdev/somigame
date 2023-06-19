#pragma once

/* headers */

#include "head.hxx"

/* defines */

/* content */

_NAMESPACE_ENTER

/** typedef **/

typedef enum key_mode_e {
    _KEY_MODE_MAIN = 0x0,
    _KEY_MODE_HERO,
    _KEY_MODE_VIEW,
    _KEY_MODE_COUNT
} key_mode_e;

typedef void(*key_func_t)(int narg);

typedef char key_code_t;
typedef const char* key_mesg_t;
typedef struct key_bind_t key_bind_t;
typedef std::vector<key_bind_t*> key_list_t;
typedef const char* key_path_t;

typedef struct key_bind_t {
    key_func_t func = _NULL;
    key_list_t list;
    key_mesg_t mesg;
} key_bind_t;

/** datadef **/

/** consdef **/

constexpr auto KEY_COUNT = 1 << 8;

/** getters **/

extern std::string&get_key_line();
extern const char*get_key_mode_name();
extern key_mode_e get_key_mode();
extern int get_key_narg_sign();
extern int get_key_narg();

/** setters **/

extern bool key_mode_set(key_mode_e mode = _KEY_MODE_MAIN);
extern bool key_bind_set(key_mode_e mode, key_path_t path, key_func_t func);

/** actions **/

extern void iput_init();

extern void proc_key_board(unsigned char code, int mouse_x, int mouse_y);

_NAMESPACE_LEAVE
