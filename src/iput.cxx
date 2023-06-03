/* headers */

#include "iput.hxx"

/* content */

_NAMESPACE_ENTER

/** datadef **/

std::string key_line = "";

key_list_t key_list = key_list_t(_KEY_COUNT);
key_bind_t* key_bind_used = _NULL;

/** actions **/

static void key_line_reset()
{
    key_bind_used = _NULL;
}

static void key_line_apply()
{
    if (key_bind_used->func)
    {
        key_bind_used->func();
    }
    key_line_reset();
}

static void key_line_insert(key_code_t code)
{
    key_bind_t* bind;
    if (key_bind_used)
    {
        bind = key_bind_used->list[code];
    }
    else
    {
        key_line = "";
        bind = key_list[code];
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

void key_board_init()
{
}

void key_bind_set(key_path_t path, key_func_t func)
{
    size_t path_size = std::strlen(path);
    if (path_size > 0)
    {
        key_code_t code = path[0];
        key_list_t*list =&key_list;
        key_bind_t*bind = (*list)[code];
        for (auto iter = 0; iter < path_size; iter += 1)
        {
            code = path[iter];
            if (list->empty())
            {
                *list = key_list_t(_KEY_COUNT);
            }
            bind = (*list)[code];
            if (bind)
            {
            }
            else
            {
                bind = new key_bind_t;
                (*list)[code] = bind;
            }
            list =&bind->list;
        }
        bind->func = func;
    }
    else
    {
    }
}

void proc_key_board(unsigned char key, int _mouse_x, int _mouse_y)
{
    key_line_insert(key);
}

void proc_key_board_special(int key, int _mouse_x, int _mouse_y)
{
    key_line_insert(key);
}

_NAMESPACE_LEAVE
