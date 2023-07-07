#ifndef SOMIGAME_MAIN_UNIT_KEYMAN_CXX
#define SOMIGAME_MAIN_UNIT_KEYMAN_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_rects.hxx"
#include "../geom/type_mover.hxx"
#include "../gfix/unit_camera.hxx"
#include "../gfix/unit_guiman.hxx"
#include "../game/type_sizes.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_rects.hxx"
#include "../game/unit_gamer.hxx"
#include "../game/func_init.hxx"
#include "../main/func_quit.hxx"
#include "../main/unit_ticker.hxx"
#include "../main/unit_keyman.hxx"

namespace somigame { namespace main {

// signals

_DEFN_DATA signal_t<void(key_mode_e)>key_mode_set_signal;
_DEFN_DATA signal_t<void(key_path_t)>key_line_apply_signal;
_DEFN_DATA signal_t<void(key_path_t)>key_line_insert_signal;
_DEFN_DATA signal_t<void(key_code_t)>key_down_signal;

// datadef

_DEFN_DATA sdstr_t key_line = "";
_DEFN_DATA int key_narg_sign = +1;
_DEFN_DATA int key_narg = 0;

_DEFN_DATA key_mode_e key_mode = _KEY_MODE_COUNT;
_DEFN_DATA key_list_t key_list_table[_KEY_MODE_COUNT] = { };
_DEFN_DATA key_list_t*key_list = NULL;
_DEFN_DATA key_bind_t*key_bind_used = NULL;

// getters

_DEFN_FUNC svstr_t get_key_line() { return key_line; }
_DEFN_FUNC svstr_t get_key_mode_name()
{
    return key_mode == _KEY_MODE_COUNT ? "??"
        : key_mode == _KEY_MODE_MAIN ? "main"
        : key_mode == _KEY_MODE_HERO ? "hero"
        : key_mode == _KEY_MODE_VIEW ? "view"
        : key_mode == _KEY_MODE_PICK ? "pick"
        : "none";
}
_DEFN_FUNC key_mode_e get_key_mode() { return key_mode; }
_DEFN_FUNC int get_key_narg_sign() { return key_narg_sign; }
_DEFN_FUNC int get_key_narg() { return key_narg; }

// setters

_DEFN_FUNC bool key_mode_set(key_mode_e mode)
{
    if (mode < _KEY_MODE_COUNT && mode != key_mode)
    {
        key_mode = mode;
        key_list =&key_list_table[mode];
        key_mode_set_signal.holder.publish(mode);
        return TRUTH;
    }
    else { return FALSE; }
}

_DEFN_FUNC bool key_bind_set(key_mode_e mode, key_path_t path, key_func_t func)
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
                memo::pager_t::get()->new_one<key_bind_t>(bind);
                (*list)[code] = bind;
            }
            list =&bind->list;
        }
        bind->func = func;
        return TRUTH;
    }
    else
    {
        return FALSE;
    }
} // key_bind_set

// actions

static void key_line_reset()
{
    key_bind_used = NULL;
    key_narg = 0;
} // key_line_reset

static void key_line_apply()
{
    if (key_bind_used)
    {
        if (key_bind_used->func == NULL)
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
} // key_line_apply

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
} // key_line_insert

_DEFN_FUNC bool init_unit_keyman()
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
        key_bind_set(mode, "mq", [](int narg){ main::quit(); });
    }
    key_mode_set(_KEY_MODE_MAIN);
    if constexpr (TRUTH)
    { /* keybinds */
        if constexpr (TRUTH)
        { /** view **/
            key_bind_set(_KEY_MODE_VIEW, "e", [](int narg){
            });
            key_bind_set(_KEY_MODE_VIEW, "q", [](int narg){
            });
            /*** zoom ***/
            key_bind_set(_KEY_MODE_VIEW, "z", [](int narg){
                auto camera_entity = gfix::get_camera_ent();
                auto&camera_asize = ecos::reg.get<geom::com_asize_t>(camera_entity);
                camera_asize.x = std::max(camera_asize.x << narg, gfix::CAMERA_ASIZE_X<<1);
                camera_asize.y = std::max(camera_asize.y << narg, gfix::CAMERA_ASIZE_Y<<1);
                ecos::reg.replace<geom::com_asize_t>(camera_entity, camera_asize);
            });
        }
        if constexpr (TRUTH)
        { /** hero **/
            struct hero_listener_t {
                auto&get_mover()
                { return ecos::reg.get<geom::com_mover_t>(this->hero_entity); }
                void on_game_init()
                { this->hero_entity = game::get_gamer_ent(); }
                void on_key_a(int narg)
                {
                    auto&mover = this->get_mover();
                    mover.move.x -= 1*(nums::get_sign(narg)+narg);
                    ecos::reg.replace<geom::com_mover_t>(this->hero_entity, mover);
                }
                void on_key_d(int narg)
                {
                    auto&mover = this->get_mover();
                    mover.move.x += 1*(nums::get_sign(narg)+narg);
                    ecos::reg.replace<geom::com_mover_t>(this->hero_entity, mover);
                }
                void on_key_s(int narg)
                {
                    auto&mover = this->get_mover();
                    mover.move.y -= 1*(nums::get_sign(narg)+narg);
                    ecos::reg.replace<geom::com_mover_t>(this->hero_entity, mover);
                }
                void on_key_w(int narg)
                {
                    auto&mover = this->get_mover();
                    mover.move.y += 1*(nums::get_sign(narg)+narg);
                    ecos::reg.replace<geom::com_mover_t>(this->hero_entity, mover);
                }
                ecos::ent_t hero_entity;
            } static hero_listener{};
            game::init_bot_signal.binder.connect<
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
        if constexpr (TRUTH)
        { /** pick **/
            struct pick_listener_t
            {
                void on_game_init()
                {
                    this->picker_entity = game::get_picker_ent();
                    this->gamer_entity = game::get_gamer_ent();
                }
                void on_key_a(int narg)
                {
                    auto&mover = ecos::reg.get<geom::com_mover_t>(picker_entity);
                    mover.move.x -= 1*(nums::get_sign(narg)+narg);
                    ecos::reg.replace<geom::com_mover_t>(this->picker_entity, mover);
                }
                void on_key_d(int narg)
                {
                    auto&mover = ecos::reg.get<geom::com_mover_t>(picker_entity);
                    mover.move.x += 1*(nums::get_sign(narg)+narg);
                    ecos::reg.replace<geom::com_mover_t>(this->picker_entity, mover);
                }
                void on_key_s(int narg)
                {
                    auto&mover = ecos::reg.get<geom::com_mover_t>(picker_entity);
                    mover.move.y -= 1*(nums::get_sign(narg)+narg);
                    ecos::reg.replace<geom::com_mover_t>(this->picker_entity, mover);
                }
                void on_key_w(int narg)
                {
                    auto&mover = ecos::reg.get<geom::com_mover_t>(picker_entity);
                    mover.move.y += 1*(nums::get_sign(narg)+narg);
                    ecos::reg.replace<geom::com_mover_t>(this->picker_entity, mover);
                }
                void on_key_g(int narg)
                {
                    game::tposi_t hero_tposi = ecos::reg.get<game::com_tposi_t>(this->gamer_entity);
                    game::tposi_t pick_tposi = ecos::reg.get<game::com_tposi_t>(this->picker_entity);
                    geom::mover_t&hero_mover = ecos::reg.get<geom::com_mover_t>(this->gamer_entity);
                    hero_mover.move = {
                        .x = pick_tposi.x - hero_tposi.x,
                        .y = pick_tposi.y - hero_tposi.y,
                    };
                    ecos::reg.patch<geom::com_mover_t>(this->gamer_entity);
                    ecos::reg.replace<geom::com_aposi_t>(this->picker_entity);
                }
                ecos::ent_t picker_entity;
                ecos::ent_t gamer_entity;
            } static pick_listener{};
            game::init_bot_signal.binder.connect<
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
        } // picker
    } // keybinds
    return TRUTH;
} // init_unit_keyman
_DEFN_FUNC bool quit_unit_keyman()
{
    return TRUTH;
} // quit_unit_keyman

} } // namespace somigame { namespace main {

#endif//SOMIGAME_MAIN_UNIT_KEYMAN_CXX
