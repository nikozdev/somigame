#ifndef SOMIGAME_GAME_TILE_CXX
#define SOMIGAME_GAME_TILE_CXX 1

#include "../head.hxx"

/* headers */

#include "tile.hxx"
#include "somi.hxx"

#include "../ecos.hxx"
#include "../gfix/image.hxx"
#include "../gfix/visual.hxx"
#include "../gfix/layer.hxx"
#include "../gfix/color.hxx"
#include "../geom/sizes.hxx"
#include "../geom/coord.hxx"
#include "../geom/shape.hxx"

namespace somigame { /* content */

namespace game { /* datadef */

entity_t tgrid[TCELL_CFULL];

} /* datadef */

namespace game { /* getters */

ent_t get_tcell_from_tpos3(game::tpos3_t tpos3)
{
    game::tkey3_t tkey3 = get_tkey3_from_tpos3(tpos3);
    if (FALSE
        || tkey3.x < 0 || tkey3.x >= TCELL_CFULL_X
        || tkey3.y < 0 || tkey3.y >= TCELL_CFULL_Y
        || tkey3.z < 0 || tkey3.z >= TCELL_CFULL_Z
    ) { return entt::null; }
    return tgrid[get_tkey1_from_tkey3(tkey3)];
}

} /* getters */

namespace game { /* vetters */

bool vet_tcell_from_tpos3(game::tpos3_t tpos3)
{ return ecos::ecs.try_get<game::com_tcell_t>(game::get_tcell_from_tpos3(tpos3)); }

bool vet_floor_from_tpos3(game::tpos3_t tpos3)
{
    entity_t entity = game::get_tcell_from_tpos3(tpos3);
    if (auto*tcell = ecos::ecs.try_get<game::com_tcell_t>(entity))
    { return tcell->tilet == _TILET_TEST_FLOOR; }
    return FALSE;
}

bool vet_block_from_tpos3(game::tpos3_t tpos3)
{
    entity_t entity = game::get_tcell_from_tpos3(tpos3);
    if (tcell_t*tcell = ecos::ecs.try_get<game::com_tcell_t>(entity))
    { return tcell->tilet == _TILET_TEST_BLOCK; }
    return FALSE;
}

} /* vetters */

namespace game { /* setters */

ent_t set_tcell_into_tpos3(tpos3_t tpos3, tcell_t tcell)
{
    game::tkey3_t tkey3 = game::get_tkey3_from_tpos3(tpos3);
    if (FALSE
        || tkey3.x < 0 || tkey3.x >= TCELL_CFULL_X
        || tkey3.y < 0 || tkey3.y >= TCELL_CFULL_Y
        || tkey3.z < 0 || tkey3.z >= TCELL_CFULL_Z
    ) { return entt::null; }
    auto&entity = tgrid[game::get_tkey1_from_tkey3(tkey3)];
    if (ecos::ecs.valid(entity))
    { return entt::null; /* ecos::ecs.destroy(entity); */ }
    geom::gpos3_t gpos3 = game::get_gpos3_from_tpos3(tpos3);
    if constexpr (TRUTH)
    { /* entity construction */
        entity = ecos::ecs.create();
        /* sizes */
        ecos::ecs.emplace<geom::com_asize_t>(entity, geom::asize_t{
            .x = game::TCELL_ASIZE_X,
            .y = game::TCELL_ASIZE_Y,
        });
        /* coord */
        ecos::ecs.emplace<geom::com_apos2_t>(entity, gpos3.x, gpos3.y);
        ecos::ecs.emplace<geom::com_zpos1_t>(entity, gpos3.z);
        /* visual */
        ecos::ecs.emplace<gfix::com_visual_t>(entity);
        ecos::ecs.emplace<gfix::com_color_t>(entity, 0xff);
        ecos::ecs.emplace<gfix::com_imreg_t>(entity, gfix::imreg_t{
            .index = gfix::_IMORI_TILE_TEST,
            .rsize = geom::rsize_t{
                .x = geom::RSIZE_MAX,
                .y = geom::RSIZE_MAX,
            },
        });
        /* family */
        ecos::ecs.emplace<ecos::com_family_t>(entity,
            game::somi_t::get().get_ent()
        );
        /* tilegrid */
        ecos::ecs.emplace<game::com_tcell_t>(entity, tcell);
    }
    return entity;
}

} /* setters */

} /* content */

#endif/*SOMIGAME_GAME_TILE_CXX*/
