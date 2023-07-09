#ifndef SOMIGAME_MAIN_UNIT_KEYMAN_HXX
#define SOMIGAME_MAIN_UNIT_KEYMAN_HXX

#include "../head.hxx"

namespace somigame { namespace main {

// enumdef

typedef enum key_mode_e {
    _KEY_MODE_MAIN = 0x0,
    _KEY_MODE_VIEW,
    _KEY_MODE_DEAD,
    _KEY_MODE_HERO,
    _KEY_MODE_PICK,
    _KEY_MODE_COUNT
} key_mode_e;

// typedef

typedef void(*key_func_t)(int narg);

typedef char key_code_t;
typedef const char* key_mesg_t;
typedef struct key_bind_t key_bind_t;
typedef data::dpack_t<key_bind_t*> key_list_t;
typedef const char* key_path_t;

typedef struct key_bind_t {
    key_func_t func = NULL;
    key_list_t list;
    key_mesg_t mesg;
} key_bind_t;

// signals

_DECL_DATA signal_t<void(key_mode_e)>key_mode_set_signal;
_DECL_DATA signal_t<void(key_path_t)>key_line_apply_signal;
_DECL_DATA signal_t<void(key_path_t)>key_line_insert_signal;
_DECL_DATA signal_t<void(key_code_t)>key_down_signal;

// consdef

constexpr auto KEY_COUNT = 1 << 8;

// getters

_DECL_FUNC svstr_t get_key_line();
_DECL_FUNC svstr_t get_key_mode_name();
_DECL_FUNC key_mode_e get_key_mode();
_DECL_FUNC int get_key_narg_sign();
_DECL_FUNC int get_key_narg();

// setters

_DECL_FUNC bool key_mode_set(key_mode_e mode = _KEY_MODE_MAIN);
_DECL_FUNC bool key_bind_set(key_mode_e mode, key_path_t path, key_func_t func);

// actions

_DECL_FUNC bool init_unit_keyman();
_DECL_FUNC bool quit_unit_keyman();

} } // namespace somigame { namespace main {

#endif//SOMIGAME_MAIN_UNIT_KEYMAN_HXX
