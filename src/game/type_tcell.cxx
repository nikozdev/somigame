#ifndef SOMIGAME_GAME_TYPE_TCELL_CXX
#define SOMIGAME_GAME_TYPE_TCELL_CXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../ecos/type_family.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"
#include "../geom/type_rects.hxx"
#include "../gfix/type_image.hxx"
#include "../gfix/type_layer.hxx"
#include "../gfix/type_visual.hxx"
#include "../game/type_sizes.hxx"
#include "../game/type_coord.hxx"
#include "../game/type_rects.hxx"
#include "../game/type_tcell.hxx"
#include "../game/unit_world.hxx"

namespace somigame { namespace game {

// datadef

_DEFN_DATA ecos::ent_t tgrid[TCELL_CFULL];

// getters

ecos::ent_t get_tcell_from_tposi(game::tposi_t tposi)
{
    game::tkeys_t tkeys = get_tkeys_from_tposi(tposi);
    if (FALSE
        || tkeys.x < 0 || tkeys.x >= TCELL_CFULL_X
        || tkeys.y < 0 || tkeys.y >= TCELL_CFULL_Y
    ) { return entt::null; }
    return tgrid[get_mstep_from_tkeys(tkeys)];
}

// vetters

bool vet_tcell_from_tposi(game::tposi_t tposi)
{ return ecos::reg.try_get<game::com_tcell_t>(game::get_tcell_from_tposi(tposi)); }

bool vet_floor_from_tposi(game::tposi_t tposi)
{
    ecos::ent_t ent = game::get_tcell_from_tposi(tposi);
    if (auto*tcell = ecos::reg.try_get<game::com_tcell_t>(ent))
    { return tcell->tilet == _TILET_TEST_FLOOR; }
    return FALSE;
}

bool vet_block_from_tposi(game::tposi_t tposi)
{
    ecos::ent_t ent = game::get_tcell_from_tposi(tposi);
    if (tcell_t*tcell = ecos::reg.try_get<game::com_tcell_t>(ent))
    { return tcell->tilet == _TILET_TEST_BLOCK; }
    return FALSE;
}

// setters

ecos::ent_t set_tcell_into_tposi(tposi_t tposi, tcell_t tcell)
{
    game::tkeys_t tkeys = game::get_tkeys_from_tposi(tposi);
    if (vet_tkeys_oside(tkeys)) { return entt::null; }
    auto&ent = tgrid[game::get_mstep_from_tkeys(tkeys)];
    if (ecos::reg.valid(ent))
    { return entt::null; /* ecos::ecs.destroy(entity); */ }
    geom::gposi_t gposi = game::get_gposi_from_tposi(tposi);
    if constexpr (TRUTH)
    { // entity construction
        ent = ecos::reg.create();
        // sizes
        ecos::reg.emplace<geom::com_asize_t>(ent, geom::asize_t{
            .x = game::TCELL_ASIZE_X,
            .y = game::TCELL_ASIZE_Y,
        });
        // coord
        ecos::reg.emplace<geom::com_aposi_t>(ent, gposi.x, gposi.y);
        // visual
        ecos::reg.emplace<gfix::com_visual_t>(ent);
        ecos::reg.emplace<gfix::com_color_t>(ent, 0xff);
        ecos::reg.emplace<gfix::com_imreg_t>(ent, gfix::imreg_t{
            .index = gfix::_IMORI_TILE_TEST,
            .rsize = geom::rsize_t{
                .x = geom::RSIZE_MAX,
                .y = geom::RSIZE_MAX,
            },
        });
        // tcell
        ecos::reg.emplace<game::com_tcell_t>(ent, tcell);
        // family
        ecos::reg.emplace<ecos::com_family_t>(ent, game::get_world_ent());
    }
    return ent;
}

_DEFN_FUNC v1b_t init_type_tcell()
{
    for (auto iter = 0; iter < TCELL_CFULL; iter++)
    { tgrid[iter] = entt::null; }
    // build the tilegrid
    auto range = nums::get_min( TCELL_CHALF_X-1, TCELL_CHALF_Y-1 );
    auto limit = range*range + range;
    auto clock = std::clock();
    for (auto iy = -range; iy <= +range; iy++)
    {
        for (auto ix = -range; ix <= +range; ix++)
        {
            if ( ((ix*ix) + (iy*iy)) <= (limit) )
            {
                auto ent = game::set_tcell_into_tposi(tposi_t{ix,iy});
            }
        }
    }
    _OLOG("[build-time]= {:d}", std::clock() - clock);
    // final
    return TRUTH;
}
_DEFN_FUNC v1b_t quit_type_tcell()
{
    for (auto iter = 0; iter < TCELL_CFULL; iter++)
    { tgrid[iter] = entt::null; }
    // final
    return TRUTH;
}

} } // namespace somigame { namespace game {

#endif/*SOMIGAME_GAME_TYPE_TCELL_CXX*/
