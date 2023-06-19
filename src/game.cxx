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

game_t game;

/** actions **/

void game_init()
{
    /* entity component system */
    using reg_t = entt::registry;
    constexpr auto&reg = ecos.reg; 
    if constexpr (_TRUTH)
    { /* somi entity */
        auto ent = reg.create();
        reg.emplace<com_family_t>(ent);
        reg.emplace<com_anchor_t>(ent, RELPOS_MID, RELPOS_MID);
        reg.emplace<com_ename_t>(ent, _ENAME_ENTT_HERO);
        reg.emplace<com_coord_t>(ent, 0, 0, 0);
        reg.emplace<com_visual_t>(ent, _TRUTH, 0);
        game.ent_somi = ent;
    }
    if constexpr (_TRUTH)
    { /* hero entity */
        auto ent = reg.create();
        reg.emplace<com_family_t>(ent, game.ent_somi);
        reg.emplace<com_visual_t>(ent, _TRUTH, 1);
        reg.emplace<com_anchor_t>(ent, RELPOS_MID, RELPOS_MIN);
        reg.emplace<com_ename_t>(ent, _ENAME_ENTT_HERO);
        reg.emplace<com_coord_t>(ent, 0, 0, 0);
        reg.emplace<com_force_t>(ent, 0, 0);
        reg.emplace<com_frict_t>(ent, FRICT_MID);
        reg.emplace<com_weigh_t>(ent, WEIGH_MIN);
        reg.emplace<com_mover_t>(ent, 0, 0);
        reg.emplace<com_sizes_t>(ent, UNIT_SCALE_X, UNIT_SCALE_Y);
        reg.emplace<com_scale_t>(ent, 1, 1);
        reg.emplace<com_color_t>(ent, 255);
        reg.emplace<com_direc_t>(ent, +0, -1);
        reg.emplace<com_faces_t>(ent, com_faces_t{ .ilist = {
            [_EFACE_F] = image_region_t{_IMAGE_ENTT_HERO, pos2d_t{0x00,0x00}, sizes_t{0x10,0x10}},
            [_EFACE_L] = image_region_t{_IMAGE_ENTT_HERO, pos2d_t{0x10,0x00}, sizes_t{0x10,0x10}},
            [_EFACE_B] = image_region_t{_IMAGE_ENTT_HERO, pos2d_t{0x20,0x00}, sizes_t{0x10,0x10}},
            [_EFACE_R] = image_region_t{_IMAGE_ENTT_HERO, pos2d_t{0x30,0x00}, sizes_t{0x10,0x10}},
        }});
        game.ent_hero = ent;
    }
    if constexpr (_TRUTH)
    { /* tilemap */
        reg.on_construct<com_tile_t>().connect<[](reg_t&reg, entt::entity ent)
        {
        }>();
        reg.on_destroy<com_tile_t>().connect<[](reg_t&reg, entt::entity ent)
        {
        }>();
    }
    if constexpr (_TRUTH)
    { /* build */
        for (auto iter = 1; iter < UNIT_COUNT; iter++)
        { game.tile_map[iter] = entt::null; }
        auto halfy = 8, halfx = 16;
        for (auto iy = -halfy; iy < +halfy; iy++)
        {
            printf(
                "worldgen:y(%03d/%03d);%c",
                iy+halfy, halfy*2,
                '\n'
            );
            for (auto ix = -halfx; ix < +halfx; ix++)
            {
                auto ent = set_tile(coord_t{+ix,+iy,0},{});
                reg.emplace<com_floor_t>(ent);
            }
        }
    }
    if constexpr (_TRUTH)
    { /* keyboard */
        if constexpr (_TRUTH)
        { /** hero **/
            if constexpr (_TRUTH)
            { /*** goto ***/
                key_bind_set(_KEY_MODE_HERO, "gr", [](int narg){ hero_move({0,0}); });
                key_bind_set(_KEY_MODE_HERO, "ga", [](int narg){ hero_move_x(-1*(get_num_sign(narg)+narg)); });
                key_bind_set(_KEY_MODE_HERO, "gd", [](int narg){ hero_move_x(+1*(get_num_sign(narg)+narg)); });
                key_bind_set(_KEY_MODE_HERO, "gs", [](int narg){ hero_move_y(-1*(get_num_sign(narg)+narg)); });
                key_bind_set(_KEY_MODE_HERO, "gw", [](int narg){ hero_move_y(+1*(get_num_sign(narg)+narg)); });
            }
        }
    }
}

void game_loop()
{
    constexpr auto&reg = ecos.reg;
    if constexpr (_FALSE)
    {
        for (auto iz = 0; iz < UNIT_COUNT_Z; iz++)
        {
            const auto stepz = iz * UNIT_COUNT_Y;
            for (auto iy = 0; iy < UNIT_COUNT_Y; iy++)
            {
                const auto stepy = iy * UNIT_COUNT_X;
                for (auto ix = 0; ix < UNIT_COUNT_X; ix++)
                {
                    auto stepx = ix;
                    auto ent = game.tile_map[stepz + stepy + stepx];
                    if (reg.valid(ent))
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

void hero_goto(const coord_t&coord)
{
    auto&&coord_h = ecos.reg.get<com_coord_t>(game.ent_hero);
    coord_h.x = (coord.x + UNIT_COUNT_X/2) % UNIT_COUNT_X;
    coord_h.y = (coord.y + UNIT_COUNT_Y/2) % UNIT_COUNT_Y;
    coord_h.z = (coord.z + UNIT_COUNT_Z/2) % UNIT_COUNT_Z;
}
void hero_goto_x(v1s_t gotox)
{
    auto&&coord_h = ecos.reg.get<com_coord_t>(game.ent_hero);
    coord_h.x = std::clamp(gotox, -UNIT_COUNT_X * UNIT_SCALE_X, +UNIT_COUNT_X * UNIT_SCALE_X);
}
void hero_goto_y(v1s_t gotoy)
{
    auto&&coord_h = ecos.reg.get<com_coord_t>(game.ent_hero);
    coord_h.y = std::clamp(gotoy, -UNIT_COUNT_Y * UNIT_SCALE_Y, +UNIT_COUNT_Y * UNIT_SCALE_Y);
}
void hero_goto_z(v1s_t gotoz)
{
    auto&&coord_h = ecos.reg.get<com_coord_t>(game.ent_hero);
    coord_h.z = std::clamp(gotoz, -UNIT_COUNT_Z * UNIT_SCALE_Z, +UNIT_COUNT_Z * UNIT_SCALE_Z);
}

void hero_move(const mover_t&mover)
{
    auto&&mover_h = ecos.reg.get<com_mover_t>(game.ent_hero);
    mover_h.x = mover.x;
    auto&&direc_h = ecos.reg.get<com_direc_t>(game.ent_hero);
    auto movex = mover.x;
    if (mover.x != 0)
    {
        direc_h.x = get_num_sign(movex);
        direc_h.y = 0;
    }
    auto movey = mover.y;
    if (movey != 0)
    {
        direc_h.x = 0;
        direc_h.y = get_num_sign(movey);
    }
}
void hero_move_x(v1s_t movex)
{
    auto&&mover_h = ecos.reg.get<com_mover_t>(game.ent_hero);
    auto&&direc_h = ecos.reg.get<com_direc_t>(game.ent_hero);
    mover_h.x = movex;
    if (movex != 0)
    {
        direc_h.x = get_num_sign(movex);
        direc_h.y = 0;
    }
}
void hero_move_y(v1s_t movey)
{
    auto&&mover_h = ecos.reg.get<com_mover_t>(game.ent_hero);
    auto&&direc_h = ecos.reg.get<com_direc_t>(game.ent_hero);
    mover_h.y = movey;
    if (movey != 0)
    {
        direc_h.x = 0;
        direc_h.y = get_num_sign(movey);
    }
}

void hero_turn(bool_t lside)
{
    auto&&direc_h = ecos.reg.get<com_direc_t>(game.ent_hero);
    direc_h = get_vec_turn(direc_h, lside);
}

/** getters **/

ent_t get_tile(coord_t coord)
{
        auto steps = get_tile_steps(coord);
        if (_FALSE
            || steps.x < 0 || steps.x >= UNIT_COUNT_X
            || steps.y < 0 || steps.y >= UNIT_COUNT_Y
            || steps.z < 0 || steps.z >= UNIT_COUNT_Z
        ) { return entt::null; }
        auto mstep = (steps.z * UNIT_COUNT_Y) + (steps.y * UNIT_COUNT_X) + steps.x;
        return game.tile_map[mstep];
    }

    /** vetters **/

    bool vet_tile(coord_t coord)
    {
        return ecos.reg.valid(get_tile(coord));
    }

    /** setters **/

ent_t set_tile(coord_t coord, tile_t tile)
{
    auto&reg = ecos.reg;
    auto steps = get_tile_steps(coord);
    if (_FALSE
        || steps.x < 0 || steps.x >= UNIT_COUNT_X
        || steps.y < 0 || steps.y >= UNIT_COUNT_Y
        || steps.z < 0 || steps.z >= UNIT_COUNT_Z
    ) { return entt::null; }
    auto mstep = (steps.z * UNIT_COUNT_Y) + (steps.y * UNIT_COUNT_X) + steps.x;
    auto ent = game.tile_map[mstep];
    coord.x = (coord.x) * UNIT_SCALE_X;
    coord.y = (coord.y + 1) * UNIT_SCALE_Y - UNIT_SCALE_Y/2;
    coord.z = (coord.z) * UNIT_SCALE_Z;
    auto anchor = anchor_t{RELPOS_MIN,RELPOS_MIN};
    auto sizes = sizes_t{UNIT_SCALE_X, UNIT_SCALE_Y};
    auto scale = scale_t{1,1};
    if (reg.valid(ent)) { reg.destroy(ent); }
    ent = reg.create();
    reg.emplace<com_family_t>(ent, game.ent_somi);
    reg.emplace<com_visual_t>(ent, _TRUTH, 0);
    reg.emplace<com_anchor_t>(ent, anchor);
    reg.emplace<com_coord_t>(ent, coord);
    reg.emplace<com_sizes_t>(ent, sizes);
    reg.emplace<com_scale_t>(ent, scale);
    reg.emplace<com_color_t>(ent, 255u);
    reg.emplace<com_image_t>(ent, _IMAGE_TILE_TEST, pos2d_t{0,0}, sizes_t{0,0});
    reg.emplace<com_tile_t>(ent, tile);
    game.tile_map[mstep] = ent;
    return ent;
}

_NAMESPACE_LEAVE
