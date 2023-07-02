#ifndef SOMIGAME_IPUT_HXX
#define SOMIGAME_IPUT_HXX 1

#include "head.hxx"

/* headers */

/* defines */

/* content */

namespace somigame { namespace iput { /* enumdef */

typedef enum key_mode_e {
    _KEY_MODE_MAIN = 0x0,
    _KEY_MODE_VIEW,
    _KEY_MODE_DEAD,
    _KEY_MODE_HERO,
    _KEY_MODE_PICK,
    _KEY_MODE_COUNT
} key_mode_e;

} } /* enumdef */

namespace somigame { namespace iput { /* typedef */

typedef void(*key_func_t)(int narg);

typedef char key_code_t;
typedef const char* key_mesg_t;
typedef struct key_bind_t key_bind_t;
typedef darray_t<key_bind_t*> key_list_t;
typedef const char* key_path_t;

typedef struct key_bind_t {
    key_func_t func = NULL;
    key_list_t list;
    key_mesg_t mesg;
} key_bind_t;

} } /* typedef */

namespace somigame { namespace iput { /* datadef */

extern signal_t<void(key_mode_e)>key_mode_set_signal;
extern signal_t<void(key_path_t)>key_line_apply_signal;
extern signal_t<void(key_path_t)>key_line_insert_signal;
extern signal_t<void(key_code_t)>key_down_signal;

} } /* datadef */

namespace somigame { namespace iput { /* consdef */

constexpr auto KEY_COUNT = 1 << 8;

} } /* consdef */

namespace somigame { namespace iput { /* getters */

extern std::string&get_key_line();
extern const char*get_key_mode_name();
extern key_mode_e get_key_mode();
extern int get_key_narg_sign();
extern int get_key_narg();

} } /* getters */

namespace somigame { namespace iput { /* setters */

extern bool key_mode_set(key_mode_e mode = _KEY_MODE_MAIN);
extern bool key_bind_set(key_mode_e mode, key_path_t path, key_func_t func);

} } /* setters */

namespace somigame { namespace iput { /* actions */

extern void init();
extern void quit();

} } /* actions */

#endif/*SOMIGAME_IPUT_HXX*/
