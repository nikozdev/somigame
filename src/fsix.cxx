/* headers */

#include "fsix.hxx"
#include "util.hxx"
#include "ecos.hxx"
#include "game.hxx"

/* content */

_NAMESPACE_ENTER

/** consdef **/

/** typedef **/

/** datadef **/

/** actions **/

void fsix_init()
{
    if constexpr (_TRUTH)
    { /* sizing codetors */
        ecos.on_construct<com_sizes_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
        }>();
        ecos.on_destroy<com_sizes_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            if (ecos.try_get<com_scale_t>(ent) != _NULL)
            { ecos.emplace<com_scale_t>(ent); }
        }>();
        ecos.on_construct<com_scale_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            if (ecos.try_get<com_sizes_t>(ent) == _NULL)
            { ecos.emplace<com_sizes_t>(ent); }
        }>();
        ecos.on_destroy<com_scale_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
        }>();
        ecos.on_construct<com_coord_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
        }>();
        ecos.on_destroy<com_coord_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            if (ecos.try_get<com_force_t>(ent) != _NULL)
            { ecos.remove<com_force_t>(ent); }
            if (ecos.try_get<com_mover_t>(ent) != _NULL)
            { ecos.remove<com_mover_t>(ent); }
        }>();
        ecos.on_construct<com_force_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            if (ecos.try_get<com_coord_t>(ent) == _NULL)
            { ecos.emplace<com_coord_t>(ent); }
        }>();
        ecos.on_destroy<com_force_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            if (ecos.try_get<com_mover_t>(ent) != _NULL)
            { ecos.remove<com_mover_t>(ent); }
        }>();
        ecos.on_construct<com_mover_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            if (ecos.try_get<com_force_t>(ent) == _NULL)
            { ecos.emplace<com_force_t>(ent); }
        }>();
        ecos.on_destroy<com_mover_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
        }>();
    }
    if constexpr (_TRUTH)
    { /* positional codetors */
        ecos.on_construct<com_coord_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
        }>();
        ecos.on_destroy<com_coord_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            if (ecos.try_get<com_force_t>(ent) != _NULL)
            { ecos.remove<com_force_t>(ent); }
        }>();
        ecos.on_construct<com_force_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
            if (ecos.try_get<com_coord_t>(ent) == _NULL)
            { ecos.emplace<com_coord_t>(ent); }
        }>();
        ecos.on_destroy<com_force_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
        }>();
    }
    if constexpr(_TRUTH)
    { /* property codetors */
        ecos.on_construct<com_frict_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
        }>();
        ecos.on_destroy<com_frict_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
        }>();
        ecos.on_construct<com_weigh_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
        }>();
        ecos.on_destroy<com_weigh_t>().connect<[](ecos_t&ecos, ent_t ent)
        {
        }>();
    }
}

void fsix_loop()
{
    const v1s_t timer_dif = timer.dif_mil;
    /* entity-component-system */
    if constexpr (_TRUTH)
    { /* direction affected by mover */
        auto view = ecos.view<com_direc_t, com_mover_t>();
        for (auto&&[ent, direc, mover] : view.each())
        {
            if (mover.x == 0 && mover.y == 0) {}
            else if (std::abs(mover.x) > std::abs(mover.y))
            {
                direc.x = get_num_sign(mover.x);
                direc.y = 0;
            }
            else if (std::abs(mover.x) < std::abs(mover.y))
            {
                direc.x = 0;
                direc.y = get_num_sign(mover.y);
            }
        }
    }
    if constexpr (_TRUTH)
    { /* force affected by mover */
        auto view = ecos.view<com_force_t, com_mover_t>();
        for (auto&&[ent, force, mover] : view.each())
        {
#if _TRUTH
            force.x = mover.x;
            force.y = mover.y;
#else
            force.x = mover.x * FORCE_SCALE;
            force.y = mover.y * FORCE_SCALE;
#endif
        }
    }
    if constexpr (_TRUTH)
    { /* force affected by friction */
        auto view = ecos.view<com_force_t, com_frict_t>();
        for (auto&&[ent, force, frict] : view.each())
        {
            auto force_e = force;
            force.x = force.x * (FRICT_MAX-frict.value) / (FRICT_DIV * timer_dif);
            force.y = force.y * (FRICT_MAX-frict.value) / (FRICT_DIV * timer_dif);
        }
    }
    if constexpr (_TRUTH)
    { /* force affected by weigh */
        auto view = ecos.view<com_force_t, com_weigh_t>();
        for (auto&&[ent, force, weigh] : view.each())
        {
            auto force_e = force;
            force.x = force.x / (weigh.value * timer_dif);
            force.y = force.y / (weigh.value * timer_dif);
        }
    }
    if constexpr (_TRUTH)
    { /* coordinate affected by velocity */
        auto view = ecos.view<com_coord_t, com_force_t>();
        for (auto&&[ent, coord, force] : view.each())
        {
            auto force_e = force;
            auto coord_x = coord.x, coord_y = coord.y, coord_z = coord.z;
            auto moved_x = 0, moved_y = 0;
            if (auto*anchor = ecos.try_get<com_anchor_t>(ent))
            {
                auto sizes = ecos.get<com_sizes_t>(ent);
                auto scale = ecos.get<com_scale_t>(ent);
                auto coord_a = get_anchor_coord(*anchor, sizes, scale);
                coord_x += coord_a.x;
                coord_y += coord_a.y;
            }
#if _TRUTH
            moved_x += force.x;
            if (force.x == 0) { moved_x = 0; }
            else if (force.x > 0)
            {
                force.x = std::max(force.x - 1, 0);
            }
            else if (force.x < 0)
            {
                force.x = std::min(force.x + 1, 0);
            }
#else
            moved_x += force.x * timer_dif;
#endif
            coord_x += moved_x;
            {
                auto floor_coord = coord_t{coord_x,coord_y-moved_y,coord_z};
                auto floor = get_tile(get_tile_coord(floor_coord));
                if (ecos.valid(floor) && ecos.try_get<com_floor_t>(floor))
                { coord.x += moved_x; }
            }
#if _TRUTH
            moved_y += force.y;
            if (force.y == 0) { moved_y = 0; }
            else if (force.y > 0)
            {
                force.y = std::max(force.y - 1, 0);
            }
            else if (force.y < 0)
            {
                force.y = std::min(force.y + 1, 0);
            }
#else
            moved_y += force.y * timer_dif;
#endif
            coord_y += moved_y;
            {
                auto floor_coord = coord_t{coord_x-moved_x,coord_y,coord_z};
                auto floor = get_tile(get_tile_coord(floor_coord));
                if (ecos.valid(floor) && ecos.try_get<com_floor_t>(floor))
                { coord.y += moved_y; }
            }
        }
    } /* coordinate affected by velocity */
}

_NAMESPACE_LEAVE
