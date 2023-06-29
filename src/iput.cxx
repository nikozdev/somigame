#include "head.hxx"

/* headers */

#include "iput.hxx"
#include "memo.hxx"
#include "gfix.hxx"
#include "ecos.hxx"
#include "game.hxx"
#include "main.hxx"

/* defines */

/* content */

_NAMESPACE_ENTER

/** datadef **/

t_signal_t<void(key_mode_e)>key_mode_set_signal;
t_signal_t<void(key_path_t)>key_line_apply_signal;
t_signal_t<void(key_path_t)>key_line_insert_signal;
t_signal_t<void(key_code_t)>key_down_signal;

extern t_signal_t<void(void)>game_init_bot_signal;

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
        key_mode_set_signal.holder.publish(mode);
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
                memo::owner->new_one<key_bind_t>(bind);
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
            key_line_apply_signal.holder.publish(&key_line[0]);
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
    key_line_insert_signal.holder.publish(&key_line[0]);
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

void iput_init()
{
    ::glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    ::glutKeyboardFunc([](unsigned char key, int _mx, int _my) {
        key_down_signal.holder.publish(key);
        if (std::isdigit(key))
        { key_narg = key_narg * 10 + (key - '0'); }
        else if (key == '-')
        { key_narg_sign *= -1; }
        else
        { key_line_insert(key); }
    });
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
        key_bind_set(mode, "mq", [](int narg){ main_quit(); });
    }
    key_mode_set(_KEY_MODE_MAIN);
    if constexpr (_TRUTH)
    { /* keybinds */
        if constexpr (_TRUTH)
        { /** view **/
            key_bind_set(_KEY_MODE_VIEW, "e", [](int narg){
                view_turn(_FALSE);
            });
            key_bind_set(_KEY_MODE_VIEW, "q", [](int narg){
                view_turn(_TRUTH);
            });
            /*** zoom ***/
            key_bind_set(_KEY_MODE_VIEW, "z", [](int narg){
                auto view_entity = ecos.ctx().get<entity_t>("view_entity"_hstr);
                auto&view_asize = ecos.get<com_asize_t>(view_entity);
                view_asize.x = std::max(view_asize.x << narg, VIEW_ASIZE_X/2);
                view_asize.y = std::max(view_asize.y << narg, VIEW_ASIZE_Y/2);
                ecos.replace<com_asize_t>(view_entity, view_asize);
            });
        }
        if constexpr (_TRUTH)
        { /** hero **/
            struct hero_listener_t {
                auto&get_mover()
                { return ecos.get<com_mover_t>(this->hero_entity); }
                void on_game_init()
                { this->hero_entity = ecos.ctx().get<ent_t>("hero_entity"_hstr); }
                void on_key_a(int narg)
                {
                    auto&mover = this->get_mover();
                    mover.move.x -= 1*(get_num_sign(narg)+narg);
                    ecos.replace<com_mover_t>(this->hero_entity, mover);
                }
                void on_key_d(int narg)
                {
                    auto&mover = this->get_mover();
                    mover.move.x += 1*(get_num_sign(narg)+narg);
                    ecos.replace<com_mover_t>(this->hero_entity, mover);
                }
                void on_key_s(int narg)
                {
                    auto&mover = this->get_mover();
                    mover.move.y -= 1*(get_num_sign(narg)+narg);
                    ecos.replace<com_mover_t>(this->hero_entity, mover);
                }
                void on_key_w(int narg)
                {
                    auto&mover = this->get_mover();
                    mover.move.y += 1*(get_num_sign(narg)+narg);
                    ecos.replace<com_mover_t>(this->hero_entity, mover);
                }
                entity_t hero_entity;
            } static hero_listener{};
            game_init_bot_signal.binder.connect<
                &hero_listener_t::on_game_init
                >(hero_listener);
            key_bind_set(_KEY_MODE_HERO, "a", [](int narg){
                hero_listener.on_key_a(narg);
            });
            key_bind_set(_KEY_MODE_HERO, "d", [](int narg){
                hero_listener.on_key_d(narg);
            });
            key_bind_set(_KEY_MODE_HERO, "s", [](int narg){
                hero_listener.on_key_s(narg);
            });
            key_bind_set(_KEY_MODE_HERO, "w", [](int narg){
                hero_listener.on_key_w(narg);
            });
        } /* pick */
        if constexpr (_TRUTH)
        { /** pick **/
            struct pick_listener_t
            {
                void on_game_init()
                {
                    this->pick_entity = ecos.ctx().get<ent_t>("pick_entity"_hstr);
                    this->hero_entity = ecos.ctx().get<ent_t>("hero_entity"_hstr);
                }
                void on_key_a(int narg)
                {
                    auto&mover = ecos.get<com_mover_t>(pick_entity);
                    mover.move.x -= 1*(get_num_sign(narg)+narg);
                    ecos.replace<com_mover_t>(this->pick_entity, mover);
                }
                void on_key_d(int narg)
                {
                    auto&mover = ecos.get<com_mover_t>(pick_entity);
                    mover.move.x += 1*(get_num_sign(narg)+narg);
                    ecos.replace<com_mover_t>(this->pick_entity, mover);
                }
                void on_key_s(int narg)
                {
                    auto&mover = ecos.get<com_mover_t>(pick_entity);
                    mover.move.y -= 1*(get_num_sign(narg)+narg);
                    ecos.replace<com_mover_t>(this->pick_entity, mover);
                }
                void on_key_w(int narg)
                {
                    auto&mover = ecos.get<com_mover_t>(pick_entity);
                    mover.move.y += 1*(get_num_sign(narg)+narg);
                    ecos.replace<com_mover_t>(this->pick_entity, mover);
                }
                void on_key_g(int narg)
                {
                    tpos3_t hero_tpos3 = ecos.get<com_tpos3_t>(this->hero_entity);
                    tpos3_t pick_tpos3 = ecos.get<com_tpos3_t>(this->pick_entity);
                    mover_t&hero_mover = ecos.get<com_mover_t>(this->hero_entity);
                    hero_mover.move = {
                        .x = pick_tpos3.x - hero_tpos3.x,
                        .y = pick_tpos3.y - hero_tpos3.y,
                    };
                    ecos.patch<com_mover_t>(this->hero_entity);
                    ecos.replace<com_apos2_t>(this->pick_entity);
                }
                entity_t pick_entity;
                entity_t hero_entity;
            } static pick_listener{};
            game_init_bot_signal.binder.connect<
                &pick_listener_t::on_game_init
                >(pick_listener);
            key_bind_set(_KEY_MODE_PICK, "a", [](int narg){
                pick_listener.on_key_a(narg);
            });
            key_bind_set(_KEY_MODE_PICK, "d", [](int narg){
                pick_listener.on_key_d(narg);
            });
            key_bind_set(_KEY_MODE_PICK, "s", [](int narg){
                pick_listener.on_key_s(narg);
            });
            key_bind_set(_KEY_MODE_PICK, "w", [](int narg){
                pick_listener.on_key_w(narg);
            });
            key_bind_set(_KEY_MODE_PICK, "g", [](int narg){
                pick_listener.on_key_g(narg);
            });
        } /* pick */
    } /* keybinds */
}
void iput_quit()
{
}

_NAMESPACE_LEAVE
