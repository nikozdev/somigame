#pragma once

/* headers */

#include "head.hxx"

/* defines */

#define _KEY_COUNT (2 << (sizeof(char) * 8))

/* content */

_NAMESPACE_ENTER

/** typedef **/

typedef void(*key_func_t)();

typedef char key_code_t;
typedef char* key_mesg_t;
typedef struct key_bind_t key_bind_t;
typedef std::vector<key_bind_t*> key_list_t;
typedef const char* key_path_t;

typedef struct key_bind_t {
    key_func_t func;
    key_list_t list;
    key_mesg_t mesg;
} key_bind_t;

/** datadef **/

extern std::string key_line;

/** actions **/

extern void key_board_init();

extern void key_bind_set(key_path_t, key_func_t);

extern void proc_key_board(unsigned char, int, int);
extern void proc_key_board_special(int, int, int);

_NAMESPACE_LEAVE
