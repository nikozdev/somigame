/* headers */

#include "iput.hxx"
#include "memo.hxx"
#include "gfix.hxx"
#include "ecos.hxx"
#include "game.hxx"

#include <GL/glut.h>

/* content */

_NAMESPACE_ENTER

/** datadef **/

key_mode_set_signal_t key_mode_set_signal;

std::string key_line = "";
int key_narg_sign = +1;
int key_narg = 0;

key_mode_e key_mode = _KEY_MODE_COUNT;
key_list_t key_list_table[_KEY_MODE_COUNT] = { };
key_list_t*key_list = _NULL;
key_bind_t*key_bind_used = _NULL;

/** getters **/

std::string&get_key_line() { return key_line; }
const char*get_key_mode_name()
{
    return key_mode == _KEY_MODE_COUNT ? "??"
        : key_mode == _KEY_MODE_MAIN ? "main"
        : key_mode == _KEY_MODE_HERO ? "hero"
        : key_mode == _KEY_MODE_VIEW ? "view"
        : key_mode == _KEY_MODE_PICK ? "pick"
        : "none";
}
key_mode_e get_key_mode() { return key_mode; }
int get_key_narg_sign() { return key_narg_sign; }
int get_key_narg() { return key_narg; }

/** setters **/

bool key_mode_set(key_mode_e mode)
{
    if (mode < _KEY_MODE_COUNT && mode != key_mode)
    {
        key_mode = mode;
        key_list =&key_list_table[mode];
        key_mode_set_signal.call(mode);
        return _TRUTH;
    }
    else { return _FALSE; }
}

bool key_bind_set(key_mode_e mode, key_path_t path, key_func_t func)
{
    size_t path_size = std::strlen(path);
    if (path_size > 0)
    {
        key_code_t code = path[0];
        key_list_t*list =&key_list_table[mode];
        key_bind_t*bind = (*list)[code];
        for (auto iter = 0; iter < path_size; iter += 1)
        {
            code = path[iter];
            if (list->empty())
            {
                *list = key_list_t(KEY_COUNT);
            }
            bind = (*list)[code];
            if (bind)
            {
            }
            else
            {
                memo::owner->new_one<key_bind_t>(bind );
                (*list)[code] = bind;
            }
            list =&bind->list;
        }
        bind->func = func;
        return _TRUTH;
    }
    else
    {
        return _FALSE;
    }
}

/** actions **/

static void key_line_reset()
{
    key_bind_used = _NULL;
    key_narg = 0;
}

static void key_line_apply()
{
    if (key_bind_used)
    {
        if (key_bind_used->func == _NULL)
        {
            key_line = "\"" + key_line + "\"" + ": is not a valid keybind";
        }
        else
        {
            key_bind_used->func(key_narg * key_narg_sign);
            key_line += "[done]";
        }
    }
    else
    {
        key_line = "no keybind to apply";
    }
    key_line_reset();
}

static void key_line_insert(key_code_t code)
{
    key_bind_t* bind;
    if (key_bind_used)
    {
        if (code == '\r' || code == '\n') { return key_line_apply(); }
        bind = key_bind_used->list[code];
    }
    else
    {
        key_line = "";
        bind =(*key_list)[code];
    }
    key_line += code;
    if (bind)
    {
        key_bind_used = bind;
        if (bind->list.empty())
        {
            key_line_apply();
        }
        else
        {
        }
    }
    else
    {
        key_line = "\"" + key_line + "\"" + ": is not a valid keybind";
        key_line_reset();
    }
}

extern void proc_key_board(unsigned char key, int _mouse_x, int _mouse_y);
void iput_init()
{
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    glutKeyboardFunc(proc_key_board);
    /* keybinds */
    for (index_t iter = _KEY_MODE_MAIN; iter < _KEY_MODE_COUNT; iter++)
    {
        auto mode = static_cast<key_mode_e>(iter);
        key_list_table[mode] = key_list_t(KEY_COUNT);
        /** main, meta, mode, menu... **/
        /*** main mode ***/
        key_bind_set(mode, "mm", [](int narg){ key_mode_set(_KEY_MODE_MAIN); });
        /*** hero mode ***/
        key_bind_set(mode, "mh", [](int narg){ key_mode_set(_KEY_MODE_HERO); });
        /*** pick mode ***/
        key_bind_set(mode, "mp", [](int narg){ key_mode_set(_KEY_MODE_PICK); });
        /*** view mode ***/
        key_bind_set(mode, "mv", [](int narg){ key_mode_set(_KEY_MODE_VIEW); });
        /*** quit ***/
        key_bind_set(mode, "mq", [](int narg){ exit(_ERROR_NONE); });
    }
    key_mode_set(_KEY_MODE_MAIN);
    if constexpr (_TRUTH)
    { /* keybinds */
        if constexpr (_TRUTH)
        { /** view **/
            /*** goto ***/
            key_bind_set(_KEY_MODE_VIEW, "gx", [](int narg){ view_goto_x(narg); });
            key_bind_set(_KEY_MODE_VIEW, "gy", [](int narg){ view_goto_y(narg); });
            key_bind_set(_KEY_MODE_VIEW, "gz", [](int narg){ view_goto_z(narg); });
            /*** move ***/
            key_bind_set(_KEY_MODE_VIEW, "a", [](int narg){ view_move({.x=(-1)*(get_num_sign(narg)+narg),.y=0}); });
            key_bind_set(_KEY_MODE_VIEW, "d", [](int narg){ view_move({.x=(+1)*(get_num_sign(narg)+narg),.y=0}); });
            key_bind_set(_KEY_MODE_VIEW, "s", [](int narg){ view_move({.x=0,.y=(-1)*(get_num_sign(narg)+narg)}); });
            key_bind_set(_KEY_MODE_VIEW, "w", [](int narg){ view_move({.x=0,.y=(+1)*(get_num_sign(narg)+narg)}); });
            key_bind_set(_KEY_MODE_VIEW, "e", [](int narg){ view_turn(_FALSE); });
            key_bind_set(_KEY_MODE_VIEW, "q", [](int narg){ view_turn(_TRUTH); });
            /*** zoom ***/
            key_bind_set(_KEY_MODE_VIEW, "z", [](int narg){ view_zoom({.x=narg,.y=narg}); });
        }
        if constexpr (_TRUTH)
        { /** hero **/
            /*** step ***/
            key_bind_set(_KEY_MODE_HERO, "a", [](int narg){ hero_step_x(-1*(get_num_sign(narg)+narg)); });
            key_bind_set(_KEY_MODE_HERO, "d", [](int narg){ hero_step_x(+1*(get_num_sign(narg)+narg)); });
            key_bind_set(_KEY_MODE_HERO, "s", [](int narg){ hero_step_y(-1*(get_num_sign(narg)+narg)); });
            key_bind_set(_KEY_MODE_HERO, "w", [](int narg){ hero_step_y(+1*(get_num_sign(narg)+narg)); });
        } /* pick */
        if constexpr (_TRUTH)
        { /** pick **/
            /*** step ***/
            key_bind_set(_KEY_MODE_PICK, "a", [](int narg){ pick_step_x(-1*(get_num_sign(narg)+narg)); });
            key_bind_set(_KEY_MODE_PICK, "d", [](int narg){ pick_step_x(+1*(get_num_sign(narg)+narg)); });
            key_bind_set(_KEY_MODE_PICK, "s", [](int narg){ pick_step_y(-1*(get_num_sign(narg)+narg)); });
            key_bind_set(_KEY_MODE_PICK, "w", [](int narg){ pick_step_y(+1*(get_num_sign(narg)+narg)); });
            key_bind_set(_KEY_MODE_PICK, "g", [](int narg){
                auto&coord_p = ecos.get<com_coord_t>(get_by_ename({_ENAME_GAME_PICK}));
                auto&coord_h = ecos.get<com_coord_t>(get_by_ename({_ENAME_GAME_HERO}));
                if constexpr (_FALSE)
                {
                    auto stepx = coord_p.x / UNIT_SCALE_X;
                    auto stepy = coord_p.y / UNIT_SCALE_Y;
                    hero_step_x(stepx); hero_step_y(stepy);
                }
                else
                {
                    auto gotox = coord_h.x + coord_p.x;
                    auto gotoy = coord_h.y + coord_p.y;
                    hero_goto_x(gotox); hero_goto_y(gotoy);
                }
                coord_p.x = 0;
                coord_p.y = 0;
                coord_p.z = 0;
            });
        } /* pick */
    } /* keybinds */
}

void proc_key_board(unsigned char key, int _mouse_x, int _mouse_y)
{
    if (std::isdigit(key))
    {
        key_narg = key_narg * 10 + (key - '0');
    }
    else if (key == '-')
    {
        key_narg_sign *= -1;
    }
    else
    {
        key_line_insert(key);
    }
}

_NAMESPACE_LEAVE
