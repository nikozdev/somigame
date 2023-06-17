/* headers */

#include "game.hxx"
#include "util.hxx"
#include "gfix.hxx"
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
    if constexpr (_TRUTH) { /* host entity */
        auto ent = reg.create();
        reg.emplace<com_family_t>(ent);
        reg.emplace<com_anchor_t>(ent, RELPOS_MID, RELPOS_MIN);
        reg.emplace<com_ename_t>(ent, _ENAME_ENTT_MAIN);
        reg.emplace<com_coord_t>(ent, 0, 0);
        reg.emplace<com_sizes_t>(ent, UNIT_SCALE_X, UNIT_SCALE_Y);
        reg.emplace<com_scale_t>(ent, 1, 1);
        reg.emplace<com_color_t>(ent, 255);
        reg.emplace<com_visual_t>(ent, _TRUTH, 0);
        reg.emplace<com_image_t>(ent,
            _IMAGE_ENTT_MAIN,
            coord_t{ .x = 0, .y = 0 },
            sizes_t{ .w = UNIT_SCALE_X, .h = UNIT_SCALE_Y }
        );
        game.ent_main = ent;
    }
    reg.on_construct<com_tile_t>().connect<[](reg_t&reg, entt::entity ent)
    {
        auto&tile = reg.get<com_tile_t>(ent);
        auto&coord = reg.get<com_coord_t>(ent);
        auto&visual = reg.get<com_visual_t>(ent);
        auto stepz = visual.layer + UNIT_COUNT_Z/2;
        auto stepx = coord.x + UNIT_COUNT_X/2;
        auto stepy = coord.y + UNIT_COUNT_Y/2;
        if ((stepx > UNIT_COUNT_X) || (stepy > UNIT_COUNT_Y) || (stepz > UNIT_COUNT_Z))
        { return; }
        /* TODO: assign the right image coordinates */
        stepz *= UNIT_COUNT_Y;
        stepy *= UNIT_COUNT_X; 
        game.tile_map[stepz + stepy + stepx] = entt::null;
    }>();
    reg.on_destroy<com_tile_t>().connect<[](reg_t&reg, entt::entity ent)
    {
        auto&tile = reg.get<com_tile_t>(ent);
        auto&coord = reg.get<com_coord_t>(ent);
        auto&visual = reg.get<com_visual_t>(ent);
        auto stepz = (visual.layer + UNIT_COUNT_Z/2);
        auto stepx = (coord.x + UNIT_COUNT_X/2);
        auto stepy = (coord.y + UNIT_COUNT_Y/2);
        if ((stepx > UNIT_COUNT_X) || (stepy > UNIT_COUNT_Y) || (stepz > UNIT_COUNT_Z))
        { return; }
        /* TODO: assign the proper image coordinates */
        stepz *= UNIT_COUNT_Y;
        stepy *= UNIT_COUNT_X; 
        game.tile_map[stepz + stepy + stepx] = entt::null;
    }>();
    /* tilemap */
    /* keyboard */
    /** char **/
    key_bind_set("ca", [](int narg){
        auto&coord = ecos.reg.get<com_coord_t>(game.ent_main);
        coord.x -= UNIT_SCALE_X / 2 * get_num_sign(narg)+narg;
    });
    key_bind_set("cd", [](int narg){
        auto&coord = ecos.reg.get<com_coord_t>(game.ent_main);
        coord.x += UNIT_SCALE_X / 2 * get_num_sign(narg)+narg;
    });
    key_bind_set("cs", [](int narg){
        auto&coord = ecos.reg.get<com_coord_t>(game.ent_main);
        coord.y -= UNIT_SCALE_Y / 2 * get_num_sign(narg)+narg;
    });
    key_bind_set("cw", [](int narg){
        auto&coord = ecos.reg.get<com_coord_t>(game.ent_main);
        coord.y += UNIT_SCALE_Y / 2 * get_num_sign(narg)+narg;
    });
}

/** getters **/

ent_t game_get_tile(coord_t coord, index_t layer)
{
    auto stepz = layer + UNIT_COUNT_X / 2;
    auto stepx = coord.x + UNIT_COUNT_Y / 2;
    auto stepy = coord.y + UNIT_COUNT_Z / 2;
    if (stepx > UNIT_COUNT_X || stepy > UNIT_COUNT_Y || stepz > UNIT_COUNT_Z)
    { return entt::null; }
    stepz *= UNIT_COUNT_Y;
    stepy *= UNIT_COUNT_X; 
    return game.tile_map[stepz + stepy + stepx];
}

/** vetters **/

bool game_vet_tile(coord_t coord, index_t layer)
{
    return ecos.reg.valid(game_get_tile(coord, layer));
}

/** setters **/

bool game_set_tile(coord_t coord, index_t layer, tile_t tile)
{
    auto&reg = ecos.reg;
    auto stepz = layer + UNIT_COUNT_X / 2;
    auto stepx = coord.x + UNIT_COUNT_Y / 2;
    auto stepy = coord.y + UNIT_COUNT_Z / 2;
    if (stepx > UNIT_COUNT_X || stepy > UNIT_COUNT_Y || stepz > UNIT_COUNT_Z)
    { return _FALSE; }
    stepz *= UNIT_COUNT_Y;
    stepy *= UNIT_COUNT_X; 
    auto ent = game.tile_map[stepz + stepy + stepx];
    if (reg.valid(ent)) { reg.destroy(ent); }
    ent = reg.create();
    reg.emplace<com_coord_t>(ent, coord);
    reg.emplace<com_sizes_t>(ent, UNIT_SCALE_X, UNIT_SCALE_Y);
    reg.emplace<com_visual_t>(ent, _TRUTH, layer);
    reg.emplace<com_tile_t>(ent, tile);
    game.tile_map[stepz + stepy + stepx] = ent;
    return _TRUTH;
}

_NAMESPACE_LEAVE
