/* headers */

#include "game.hxx"
#include "util.hxx"
#include "gfix.hxx"
#include "ecos.hxx"
#include "iput.hxx"

/* content */

_NAMESPACE_ENTER

/** datadef **/

ent_t ent_hero = entt::null;
ent_t ent_pick = entt::null;
ent_t ent_somi = entt::null;

ent_t tile_map[TILE_COUNT];

count_t action_count = 0;

hero_rise_signal_t hero_rise_signal;
hero_died_signal_t hero_died_signal;

pick_step_signal_t pick_step_signal;

/** actions **/

void game_init()
{
    /* entity component system */
    if constexpr (_TRUTH)
    { /* the game entity */
        auto ent_somi = ecos.create();
        ecos.emplace<com_family_t>(ent_somi);
        ecos.emplace<com_anchor_t>(ent_somi, RELPOS_MID, RELPOS_MID);
        ecos.emplace<com_ename_t>(ent_somi, _ENAME_GAME);
        ecos.emplace<com_coord_t>(ent_somi, 0, 0, 0);
        ecos.emplace<com_visual_t>(ent_somi, _TRUTH, 0);
    }
    if constexpr (_TRUTH)
    { /* hero entity */
        ent_hero = ecos.create();
        auto&family_h = ecos.emplace<com_family_t>(ent_hero, ent_somi);
        auto&visual_h = ecos.emplace<com_visual_t>(ent_hero, _TRUTH, 1);
        auto&anchor_h = ecos.emplace<com_anchor_t>(ent_hero, RELPOS_MID, RELPOS_MID);
        auto&ename_h = ecos.emplace<com_ename_t>(ent_hero, _ENAME_GAME_HERO);
        auto&alive_h = ecos.emplace<com_alive_t>(ent_hero, _TRUTH);
        auto&coord_h = ecos.emplace<com_coord_t>(ent_hero, 0, 0, 0);
        ecos.emplace<com_sizes_t>(ent_hero, TILE_SCALE_X, TILE_SCALE_Y);
        ecos.emplace<com_scale_t>(ent_hero, 1, 1);
        ecos.emplace<com_color_t>(ent_hero, 0xff);
        ecos.emplace<com_direc_t>(ent_hero, +0, -1);
        ecos.emplace<com_faces_t>(ent_hero, com_faces_t{ .ilist = {
            [_EFACE_F] = image_region_t{_IMAGE_GAME_HERO, pos2d_t{0x00,0x00}, sizes_t{0x10,0x10}},
            [_EFACE_L] = image_region_t{_IMAGE_GAME_HERO, pos2d_t{0x10,0x00}, sizes_t{0x10,0x10}},
            [_EFACE_B] = image_region_t{_IMAGE_GAME_HERO, pos2d_t{0x20,0x00}, sizes_t{0x10,0x10}},
            [_EFACE_R] = image_region_t{_IMAGE_GAME_HERO, pos2d_t{0x30,0x00}, sizes_t{0x10,0x10}},
        }});
        if constexpr (_TRUTH)
        { /* pick entity */
            ent_pick = ecos.create();
            auto&family_p = ecos.emplace<com_family_t>(ent_pick, ent_hero);
            auto&visual_p = ecos.emplace<com_visual_t>(ent_pick, _FALSE, 1);
            auto&ename_p = ecos.emplace<com_ename_t>(ent_pick, _ENAME_GAME_PICK);
            auto&coord_p = ecos.emplace<com_coord_t>(ent_pick, 0, 0, 0);
            auto&sizes_p = ecos.emplace<com_sizes_t>(ent_pick, TILE_SCALE_X, TILE_SCALE_Y);
            auto&scale_p = ecos.emplace<com_scale_t>(ent_pick, 1, 1);
            auto&color_p = ecos.emplace<com_color_t>(ent_pick, 0xff);
            auto&image_p = ecos.emplace<com_image_t>(ent_pick, _IMAGE_GAME_PICK);
            key_mode_set_signal.bind([&](key_mode_e mode){
                if (mode == _KEY_MODE_PICK)
                { visual_p.valid = _TRUTH; coord_p.x = 0; coord_p.y = 0; coord_p.z = 0; }
                else
                { visual_p.valid = _FALSE; }
            });
        } /* pick */
        if constexpr (_TRUTH)
        { /* grid */
            auto ent_grid = get_by_ename({_ENAME_GFIX_GRID});
            if (ecos.valid(ent_grid))
            { set_ancestor(ent_grid, ent_hero); }
        } /* grid */
        if constexpr (_TRUTH)
        { /* signals */
            hero_rise_signal.bind([&](alive_t&alive){
                coord_h.x = 0;
                coord_h.y = 0;
                coord_h.z = 0; 
                key_mode_set(_KEY_MODE_MAIN);
            });
            hero_died_signal.bind([&](alive_t&alive){
                key_mode_set(_KEY_MODE_DEAD);
                static auto die_mil = timer.now_mil;
                die_mil = timer.now_mil;
                using link_t = timer_t::signal_t::link_t;
                static link_t*link = _NULL;
                link = timer.sig_upd.bind([&](){
                    if ((timer.now_mil - die_mil) > 3'000)
                    {
                        link->quit();
                        link = _NULL;
                        alive.valid = _TRUTH;
                        hero_rise_signal.call(alive);
                    }
                });
            });
        } /* signals */
    } /* hero */
    if constexpr (_TRUTH)
    { /* tilemap */
        ecos.on_construct<com_tile_t>().connect<[](ecos_t&ecos, entt::entity ent)
        {
        }>();
        ecos.on_destroy<com_tile_t>().connect<[](ecos_t&ecos, entt::entity ent)
        {
        }>();
    }
    if constexpr (_TRUTH)
    { /* build */
        for (auto iter = 0; iter < TILE_COUNT; iter++)
        { tile_map[iter] = entt::null; }
        auto halfx = GAME_HALFC_X / 0x08, halfy = GAME_HALFC_Y / 0x08, halfz = GAME_HALFC_Z / 0x08;
        for (auto iz = -halfz; iz < +halfz; iz++)
        {
            for (auto iy = -halfy; iy < +halfy; iy++)
            {
                for (auto ix = -halfx; ix < +halfx; ix++)
                {
                    if (rand()%5 == 0)
                    {
                    }
                    else
                    {
                        auto ent = set_tile_into_tile_coord(coord_t{ix,iy,iz},{});
                        ecos.emplace<com_floor_t>(ent);
                    }
                }
            }
        }
    }
    else
    {
        for (auto iter = 0; iter < TILE_COUNT; iter++)
        { tile_map[iter] = entt::null; }
        auto halfx = 0x08, halfy = 0x04;
        for (auto iy = -halfy; iy < +halfy; iy++)
        {
            for (auto ix = -halfx; ix < +halfx; ix++)
            {
                auto ent = set_tile_into_tile_coord(coord_t{ix,iy,0},{});
                ecos.emplace<com_floor_t>(ent);
            }
        }
    } /* build */
}

void game_loop()
{
    if constexpr (_FALSE)
    { /* tilemap */
        for (auto iz = 0; iz < TILE_COUNT_Z; iz++)
        {
            const auto stepz = iz * TILE_COUNT_Z;
            for (auto iy = 0; iy < TILE_COUNT_Y; iy++)
            {
                const auto stepy = iy * TILE_COUNT_X;
                for (auto ix = 0; ix < TILE_COUNT_X; ix++)
                {
                    auto stepx = ix;
                    auto ent = tile_map[stepz + stepy + stepx];
                    if (ecos.valid(ent))
                    {
                    }
                    else
                    {
                    }
                }
            }
        }
    }
    if constexpr (_FALSE)
    { /* pick update */
        auto&coord_h = ecos.get<com_coord_t>(ent_hero);
        auto&coord_p = ecos.get<com_coord_t>(ent_pick);
    }
    if constexpr (_FALSE)
    {
        for (auto&&[ent, coord_e] : ecos.view<com_coord_t>().each())
        {
            if (vet_ancestor(ent, entt::null) || vet_ancestor(ent, ent_somi))
            {
                while (vet_floor_from_coord(coord_e) == _FALSE
                    && (coord_e.z > -GAME_HALFC_Z)
                ) { coord_e.z -= TILE_SCALE_Z; }
                if (vet_floor_from_coord(coord_e) == _FALSE)
                { kill_ent(ent); }
            }
        }
    }
}

void inc_action_count(count_t inc)
{
    if (inc < 1) { return; }
    action_count += inc;
}

bool proc_ent(ent_t ent)
{
    auto&coord_e = ecos.get<com_coord_t>(ent);
    if (vet_ancestor(ent, entt::null) || vet_ancestor(ent, ent_somi))
    {
        while (vet_floor_from_coord(coord_e) == _FALSE
            && (coord_e.z > -GAME_HALFC_Z)
        ) { coord_e.z -= TILE_SCALE_Z; }
        if (vet_floor_from_coord(coord_e) == _FALSE)
        { kill_ent(ent); }
    }
    return _TRUTH;
}
bool kill_ent(ent_t ent)
{
    if (auto*alive = ecos.try_get<com_alive_t>(ent))
    {
        if (alive->valid)
        {
            alive->valid = _FALSE;
            if (ent == ent_hero)
            { hero_died_signal.call(*alive); }
            return _TRUTH;
        }
        else { return _FALSE; }
    }
    else { return _FALSE; }
}

/*** hero ***/

/**** goto ****/

void hero_goto_x(v1s_t gotox)
{
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    if (gotox == coord_h.x) { return; }
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    direc_h.x = get_num_sign(gotox - coord_h.x);
    direc_h.y = 0;
    coord_h.x = std::clamp(gotox, -GAME_HALFS_X, +GAME_HALFS_X);
    proc_ent(ent_hero);
}
void hero_goto_y(v1s_t gotoy)
{
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    if (gotoy == coord_h.y) { return; }
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    direc_h.x = 0;
    direc_h.y = get_num_sign(gotoy - coord_h.y);
    coord_h.y = std::clamp(gotoy, -GAME_HALFS_Y, +GAME_HALFS_Y);
    proc_ent(ent_hero);
}
void hero_goto_z(v1s_t gotoz)
{
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    if (gotoz == coord_h.z) { return; }
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    direc_h.x = 0;
    direc_h.y = get_num_sign(gotoz - coord_h.z);
    coord_h.z = std::clamp(gotoz, -GAME_HALFS_Z, +GAME_HALFS_Z);
    proc_ent(ent_hero);
}

/**** step ****/

void hero_step_x(v1s_t stepx)
{
    if (stepx == 0) { return; }
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    const auto anchor_h = ecos.get<com_anchor_t>(ent_hero);
    const auto sizes_h = ecos.get<com_sizes_t>(ent_hero);
    const auto scale_h = ecos.get<com_scale_t>(ent_hero);
    auto coord_f = coord_h;
    coord_f.x += get_anchor_coord(anchor_h.x, sizes_h.w, scale_h.x);
    coord_f.y += get_anchor_coord(anchor_h.y, sizes_h.h, scale_h.y);
    coord_f = from_coord_into_tile_coord(coord_f);
    auto signx = get_num_sign(stepx);
    auto growx = TILE_SCALE_X * signx;
    coord_f.x += signx;
    while (vet_floor_from_tile_coord(coord_f) && stepx != 0)
    {
        coord_h.x += growx;
        coord_f.x += signx;
        stepx -= signx;
        inc_action_count(1);
    }
    direc_h.x = signx;
    direc_h.y = 0;
}
void hero_step_y(v1s_t stepy)
{
    if (stepy == 0) { return; }
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    const auto anchor_h = ecos.get<com_anchor_t>(ent_hero);
    const auto sizes_h = ecos.get<com_sizes_t>(ent_hero);
    const auto scale_h = ecos.get<com_scale_t>(ent_hero);
    auto coord_f = coord_h;
    coord_f.x += get_anchor_coord(anchor_h.x, sizes_h.w, scale_h.x);
    coord_f.y += get_anchor_coord(anchor_h.y, sizes_h.h, scale_h.y);
    coord_f = from_coord_into_tile_coord(coord_f);
    auto signy = get_num_sign(stepy);
    auto growy = TILE_SCALE_Y * signy;
    coord_f.y += signy;
    while (vet_floor_from_tile_coord(coord_f) && stepy != 0)
    {
        coord_h.y += growy;
        coord_f.y += signy;
        stepy -= signy;
        inc_action_count(1);
    }
    direc_h.x = 0;
    direc_h.y = signy;
}

void pick_step_x(v1s_t stepx)
{
    if (stepx == 0) { return; }
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    auto&&coord_p = ecos.get<com_coord_t>(ent_pick);
    auto coord_f = from_coord_into_tile_coord(coord_p);
    auto rng_x = 2;
    auto rng_y = 2;
    auto dif_x = std::abs(coord_f.x + stepx);
    auto dif_y = std::abs(coord_f.y);
    if ((dif_x > rng_x)
        || (dif_x > (rng_x - 1) && dif_y > (rng_y - 1))
    ) { return; }
    auto signx = get_num_sign(stepx);
    auto growx = TILE_SCALE_X * signx;
    while (stepx != 0)
    {
        coord_p.x += growx;
        stepx -= signx;
        inc_action_count(1);
    }
    pick_step_signal.call(coord_p);
}
void pick_step_y(v1s_t stepy)
{
    if (stepy == 0) { return; }
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    auto&&coord_p = ecos.get<com_coord_t>(ent_pick);
    auto coord_f = from_coord_into_tile_coord(coord_p);
    auto rng_x = 2;
    auto rng_y = 2;
    auto dif_x = std::abs(coord_f.x);
    auto dif_y = std::abs(coord_f.y + stepy);
    if ((dif_y > rng_y)
        || (dif_x > (rng_x - 1) && dif_y > (rng_y - 1))
    ) { return; }
    auto signy = get_num_sign(stepy);
    auto growy = TILE_SCALE_Y * signy;
    while (stepy != 0)
    {
        coord_p.y += growy;
        stepy -= signy;
        inc_action_count(1);
    }
    pick_step_signal.call(coord_p);
}

void hero_turn(bool_t lside)
{
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    direc_h = get_vec_turn(direc_h, lside);
}

/** getters **/

count_t get_action_count()
{
    return action_count;
}

ent_t get_tile_from_coord(coord_t coord)
{ return get_tile_from_tile_coord(from_coord_into_tile_coord(coord)); }
ent_t get_tile_from_tile_coord(coord_t coord)
{
    auto steps = from_tile_coord_into_tile_steps(coord);
    if (_FALSE
        || steps.x < 0 || steps.x >= TILE_COUNT_X
        || steps.y < 0 || steps.y >= TILE_COUNT_Y
        || steps.z < 0 || steps.z >= TILE_COUNT_Z
    ) { return entt::null; }
    return tile_map[from_tile_steps_into_tile_mstep(steps)];
}

/** vetters **/

bool vet_tile_from_coord(coord_t coord)
{ return ecos.valid(get_tile_from_coord(coord)); }
bool vet_tile_from_tile_coord(coord_t coord)
{ return ecos.valid(get_tile_from_tile_coord(coord)); }

bool vet_floor_from_coord(coord_t coord)
{ return ecos.try_get<com_floor_t>(get_tile_from_coord(coord)); }
bool vet_floor_from_tile_coord(coord_t coord)
{ return ecos.try_get<com_floor_t>(get_tile_from_tile_coord(coord)); }

bool vet_block_from_coord(coord_t coord)
{ return ecos.try_get<com_block_t>(get_tile_from_coord(coord)); }
bool vet_block_from_tile_coord(coord_t coord)
{ return ecos.try_get<com_block_t>(get_tile_from_tile_coord(coord)); }

/** setters **/

ent_t set_tile_into_tile_coord(coord_t coord, tile_t tile)
{
    auto steps = from_tile_coord_into_tile_steps(coord);
    if (_FALSE
        || steps.x < 0 || steps.x >= TILE_COUNT_X
        || steps.y < 0 || steps.y >= TILE_COUNT_Y
        || steps.z < 0 || steps.z >= TILE_COUNT_Z
    ) { return entt::null; }
    auto&ent = tile_map[from_tile_steps_into_tile_mstep(steps)];
    if (ecos.valid(ent)) { ecos.destroy(ent); }
    ent = ecos.create();
    ecos.emplace<com_family_t>(ent, ent_somi);
    ecos.emplace<com_visual_t>(ent, _TRUTH, 0);
    ecos.emplace<com_anchor_t>(ent, RELPOS_MID, RELPOS_MID);
    ecos.emplace<com_coord_t>(ent, from_tile_coord_into_coord(coord));
    ecos.emplace<com_sizes_t>(ent, TILE_SCALE_X, TILE_SCALE_Y);
    ecos.emplace<com_scale_t>(ent, 1, 1);
    ecos.emplace<com_color_t>(ent, 0xff);
    ecos.emplace<com_image_t>(ent, _IMAGE_TILE_TEST, pos2d_t{0,0}, sizes_t{0,0});
    ecos.emplace<com_tile_t>(ent, tile);
    if constexpr (_FALSE)
    {
        auto ent_text = ecos.create();
        ecos.emplace<com_family_t>(ent_text, ent);
        ecos.emplace<com_visual_t>(ent_text, _TRUTH, 0);
        ecos.emplace<com_anchor_t>(ent_text, RELPOS_MID, RELPOS_MID);
        ecos.emplace<com_relpos_t>(ent_text, RELPOS_MID, RELPOS_MID);
        ecos.emplace<com_coord_t>(ent_text, 0, 0, 0);
        ecos.emplace<com_sizes_t>(ent_text, TILE_SCALE_X, TILE_SCALE_Y);
        ecos.emplace<com_scale_t>(ent_text, 1, 1);
        ecos.emplace<com_color_t>(ent_text, 0x80);
        auto&cstring = ecos.emplace<com_cstring_t>(ent_text, 32);
        snprintf(cstring.mdata, cstring.msize,
            "x%+04d\ny%+04d\nz%+04d",
            coord.x, coord.y, coord.z
        );
    }
    return ent;
}

_NAMESPACE_LEAVE
