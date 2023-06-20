/* headers */

#include "game.hxx"
#include "util.hxx"
#include "gfix.hxx"
#include "fsix.hxx"
#include "ecos.hxx"
#include "iput.hxx"

/* content */

_NAMESPACE_ENTER

/** datadef **/

ent_t ent_hero = entt::null;
ent_t ent_somi = entt::null;
ent_t tile_map[TILE_COUNT];

/** actions **/

void game_init()
{
    /* entity component system */
    if constexpr (_TRUTH)
    { /* somi entity */
        auto ent_somi = ecos.create();
        ecos.emplace<com_family_t>(ent_somi);
        ecos.emplace<com_anchor_t>(ent_somi, RELPOS_MID, RELPOS_MID);
        ecos.emplace<com_ename_t>(ent_somi, _ENAME_ENTT_HERO);
        ecos.emplace<com_coord_t>(ent_somi, 0, 0, 0);
        ecos.emplace<com_visual_t>(ent_somi, _TRUTH, 0);
    }
    if constexpr (_TRUTH)
    { /* hero entity */
        ent_hero = ecos.create();
        ecos.emplace<com_family_t>(ent_hero, ent_somi);
        ecos.emplace<com_visual_t>(ent_hero, _TRUTH, 1);
        ecos.emplace<com_anchor_t>(ent_hero, RELPOS_MID, RELPOS_MIN);
        ecos.emplace<com_ename_t>(ent_hero, _ENAME_ENTT_HERO);
        ecos.emplace<com_coord_t>(ent_hero, 0, 0, 0);
        ecos.emplace<com_force_t>(ent_hero, 0, 0);
        /*
        ecos.emplace<com_frict_t>(ent, FRICT_MID);
        ecos.emplace<com_weigh_t>(ent, WEIGH_MIN);
        */
        ecos.emplace<com_mover_t>(ent_hero, 0, 0);
        ecos.emplace<com_sizes_t>(ent_hero, TILE_SCALE_X, TILE_SCALE_Y);
        ecos.emplace<com_scale_t>(ent_hero, 1, 1);
        ecos.emplace<com_color_t>(ent_hero, 0xff);
        ecos.emplace<com_direc_t>(ent_hero, +0, -1);
        ecos.emplace<com_faces_t>(ent_hero, com_faces_t{ .ilist = {
            [_EFACE_F] = image_region_t{_IMAGE_ENTT_HERO, pos2d_t{0x00,0x00}, sizes_t{0x10,0x10}},
            [_EFACE_L] = image_region_t{_IMAGE_ENTT_HERO, pos2d_t{0x10,0x00}, sizes_t{0x10,0x10}},
            [_EFACE_B] = image_region_t{_IMAGE_ENTT_HERO, pos2d_t{0x20,0x00}, sizes_t{0x10,0x10}},
            [_EFACE_R] = image_region_t{_IMAGE_ENTT_HERO, pos2d_t{0x30,0x00}, sizes_t{0x10,0x10}},
        }});
    }
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
        for (auto iter = 1; iter < TILE_COUNT; iter++)
        { tile_map[iter] = entt::null; }
        auto halfy = 8, halfx = 16;
        for (auto iy = -halfy; iy < +halfy; iy++)
        {
            for (auto ix = -halfx; ix < +halfx; ix++)
            {
                auto ent = set_tile(coord_t{+ix,+iy,0},{});
                ecos.emplace<com_floor_t>(ent);
            }
        }
    }
    if constexpr (_TRUTH)
    { /* keyboard */
        if constexpr (_TRUTH)
        { /** hero **/
            if constexpr (_TRUTH)
            { /*** move ***/
                key_bind_set(_KEY_MODE_HERO, "gr", [](int narg){ hero_move_x(0); hero_move_y(0); });
                key_bind_set(_KEY_MODE_HERO, "ga", [](int narg){ hero_move_x(-1*(get_num_sign(narg)+narg)); });
                key_bind_set(_KEY_MODE_HERO, "gd", [](int narg){ hero_move_x(+1*(get_num_sign(narg)+narg)); });
                key_bind_set(_KEY_MODE_HERO, "gs", [](int narg){ hero_move_y(-1*(get_num_sign(narg)+narg)); });
                key_bind_set(_KEY_MODE_HERO, "gw", [](int narg){ hero_move_y(+1*(get_num_sign(narg)+narg)); });
            }
            if constexpr (_TRUTH)
            { /*** step ***/
                key_bind_set(_KEY_MODE_HERO, "sa", [](int narg){ hero_step_x(-1*(get_num_sign(narg)+narg)); });
                key_bind_set(_KEY_MODE_HERO, "sd", [](int narg){ hero_step_x(+1*(get_num_sign(narg)+narg)); });
                key_bind_set(_KEY_MODE_HERO, "ss", [](int narg){ hero_step_y(-1*(get_num_sign(narg)+narg)); });
                key_bind_set(_KEY_MODE_HERO, "sw", [](int narg){ hero_step_y(+1*(get_num_sign(narg)+narg)); });
            }
        }
    }
}

void game_loop()
{
    if constexpr (_FALSE)
    {
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
}

/*** hero ***/

/**** goto ****/

void hero_goto_x(v1s_t gotox)
{
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    coord_h.x = std::clamp(gotox, -GAME_HALFS_X, +GAME_HALFS_X);
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    direc_h.x = get_num_sign(gotox);
    direc_h.y = 0;
}
void hero_goto_y(v1s_t gotoy)
{
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    coord_h.y = std::clamp(gotoy, -GAME_HALFS_Y, +GAME_HALFS_Y);
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    direc_h.x = 0;
    direc_h.y = get_num_sign(gotoy);
}
void hero_goto_z(v1s_t gotoz)
{
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    coord_h.z = std::clamp(gotoz, -GAME_HALFS_Z, +GAME_HALFS_Z);
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    direc_h.x = 0;
    direc_h.y = get_num_sign(gotoz);
}

/**** move ****/

void hero_move_x(v1s_t movex)
{
    auto&&mover_h = ecos.get<com_mover_t>(ent_hero);
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    if (movex != 0)
    {
        direc_h.x = get_num_sign(movex);
        direc_h.y = 0;
    }
    mover_h.x = movex;
    mover_h.y = 0;
}
void hero_move_y(v1s_t movey)
{
    auto&&mover_h = ecos.get<com_mover_t>(ent_hero);
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    if (movey != 0)
    {
        direc_h.x = 0;
        direc_h.y = get_num_sign(movey);
    }
    mover_h.x = 0;
    mover_h.y = movey;
}

/**** step ****/

void hero_step_x(v1s_t stepx)
{
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    const auto anchor_h = ecos.get<com_anchor_t>(ent_hero);
    const auto sizes_h = ecos.get<com_sizes_t>(ent_hero);
    const auto scale_h = ecos.get<com_scale_t>(ent_hero);
    /*
    stepx *= TILE_SCALE_X;
    if (stepx != 0) { direc_h.x = get_num_sign(stepx); direc_h.y = 0; }
    if (vet_floor(get_tile_coord({coord_h.x+stepx,coord_h.y,coord_h.z})))
    { coord_h.x += stepx; }
    */
    auto coord_f = coord_h;
    coord_f.x += get_anchor_coord(anchor_h.x, sizes_h.w, scale_h.x);
    coord_f.y += get_anchor_coord(anchor_h.y, sizes_h.h, scale_h.y);
    coord_f = get_tile_coord(coord_f);
    if (stepx == 0) { return; }
    else if (stepx < 0)
    {
        coord_f.x--;
        while (vet_floor(coord_f) && stepx++ < 0)
        { coord_h.x -= TILE_SCALE_X; coord_f.x--; }
        direc_h.x = -1;
    }
    else if (stepx > 0)
    {
        coord_f.x++;
        while (vet_floor(coord_f) && stepx-- > 0)
        { coord_h.x += TILE_SCALE_X; coord_f.x++; }
        direc_h.x = +1;
    }
    direc_h.y = 0;
}
void hero_step_y(v1s_t stepy)
{
    auto&&coord_h = ecos.get<com_coord_t>(ent_hero);
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    const auto anchor_h = ecos.get<com_anchor_t>(ent_hero);
    const auto sizes_h = ecos.get<com_sizes_t>(ent_hero);
    const auto scale_h = ecos.get<com_scale_t>(ent_hero);
    /*
    stepy *= TILE_SCALE_Y;
    if (stepy != 0) { direc_h.x = 0; direc_h.y = get_num_sign(stepy); }
    if (vet_floor(get_tile_coord({coord_h.x,coord_h.y+stepy,coord_h.z})))
    { coord_h.y += stepy; }
    */
    auto coord_f = coord_h;
    coord_f.x += get_anchor_coord(anchor_h.x, sizes_h.w, scale_h.x);
    coord_f.y += get_anchor_coord(anchor_h.y, sizes_h.h, scale_h.y);
    coord_f = get_tile_coord(coord_f);
    if (stepy == 0) { return; }
    else if (stepy < 0)
    {
        coord_f.y--;
        while (vet_floor(coord_f) && stepy++ < 0)
        { coord_h.y -= TILE_SCALE_Y; coord_f.y--; }
        direc_h.y = -1;
    }
    else if (stepy > 0)
    {
        coord_f.y++;
        while (vet_floor(coord_f) && stepy-- > 0)
        { coord_h.y += TILE_SCALE_Y; coord_f.y++; }
        direc_h.y = +1;
    }
    direc_h.x = 0;
}

void hero_turn(bool_t lside)
{
    auto&&direc_h = ecos.get<com_direc_t>(ent_hero);
    direc_h = get_vec_turn(direc_h, lside);
}

/** getters **/

ent_t get_tile(coord_t coord)
{
    auto steps = get_tile_steps(coord);
    if (_FALSE
        || steps.x < 0 || steps.x >= TILE_COUNT_X
        || steps.y < 0 || steps.y >= TILE_COUNT_Y
        || steps.z < 0 || steps.z >= TILE_COUNT_Z
    ) { return entt::null; }
    auto mstep = (steps.z * TILE_COUNT_Y) + (steps.y * TILE_COUNT_X) + steps.x;
    return tile_map[mstep];
}

/** vetters **/

bool vet_tile(coord_t coord)
{ return ecos.valid(get_tile(coord)); }
bool vet_floor(coord_t coord)
{ return ecos.try_get<com_floor_t>(get_tile(coord)); }
bool vet_block(coord_t coord)
{ return ecos.try_get<com_block_t>(get_tile(coord)); }

/** setters **/

ent_t set_tile(coord_t coord, tile_t tile)
{
    auto steps = get_tile_steps(coord);
    if (_FALSE
        || steps.x < 0 || steps.x >= TILE_COUNT_X
        || steps.y < 0 || steps.y >= TILE_COUNT_Y
        || steps.z < 0 || steps.z >= TILE_COUNT_Z
    ) { return entt::null; }
    auto mstep = (steps.z * TILE_COUNT_Y) + (steps.y * TILE_COUNT_X) + steps.x;
    auto ent = tile_map[mstep];
    auto coord_e = coord_t{
        .x = (coord.x) * TILE_SCALE_X,
        .y = (coord.y + 1) * TILE_SCALE_Y - TILE_SCALE_Y/2,
        .z = (coord.z) * TILE_SCALE_Z,
    };
    auto anchor = anchor_t{RELPOS_MIN,RELPOS_MIN};
    auto sizes = sizes_t{TILE_SCALE_X, TILE_SCALE_Y};
    auto scale = scale_t{1,1};
    if (ecos.valid(ent)) { ecos.destroy(ent); }
    ent = ecos.create();
    ecos.emplace<com_family_t>(ent, ent_somi);
    ecos.emplace<com_visual_t>(ent, _TRUTH, 0);
    ecos.emplace<com_anchor_t>(ent, anchor);
    ecos.emplace<com_coord_t>(ent, coord_e);
    ecos.emplace<com_sizes_t>(ent, sizes);
    ecos.emplace<com_scale_t>(ent, scale);
    ecos.emplace<com_color_t>(ent, 0xff);
    ecos.emplace<com_image_t>(ent, _IMAGE_TILE_TEST, pos2d_t{0,0}, sizes_t{0,0});
    ecos.emplace<com_tile_t>(ent, tile);
    if constexpr (_TRUTH)
    {
        auto ent_text = ecos.create();
        ecos.emplace<com_family_t>(ent_text, ent);
        ecos.emplace<com_visual_t>(ent_text, _TRUTH, 0);
        ecos.emplace<com_anchor_t>(ent_text, RELPOS_MID, RELPOS_MID);
        ecos.emplace<com_relpos_t>(ent_text, RELPOS_MID, RELPOS_MID);
        ecos.emplace<com_coord_t>(ent_text, 0, 0, 0);
        ecos.emplace<com_sizes_t>(ent_text, sizes);
        ecos.emplace<com_scale_t>(ent_text, scale_t{1,1});
        ecos.emplace<com_color_t>(ent_text, 0x80);
        auto&cstring = ecos.emplace<com_cstring_t>(ent_text, 32);
        snprintf(cstring.mdata, cstring.msize, "x%+04d\ny%+04d", coord.x, coord.y);
    }
    tile_map[mstep] = ent;
    return ent;
}

_NAMESPACE_LEAVE
