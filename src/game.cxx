#ifndef SOMIGAME_GAME_CXX
#define SOMIGAME_GAME_CXX 1

#include "head.hxx"

/* headers */

#include "game.hxx"
#include "main.hxx"
#include "gfix.hxx"
#include "ecos.hxx"
#include "iput.hxx"

#include "game/tile.hxx"
#include "game/somi.hxx"
#include "game/hero.hxx"
#include "game/helpgrid.hxx"
#include "game/picktile.hxx"

namespace somigame { /* content */

namespace game { /* signals */

signal_t<void(void)> init_top_signal;
signal_t<void(void)> init_bot_signal;
signal_t<void(void)> quit_top_signal;
signal_t<void(void)> quit_bot_signal;

} /* signals */

namespace game { /* actions */

bool init()
{
    using namespace ecos;
    game::init_top_signal.holder.publish();
    /* alive component signals */
    entt::sigh_helper{ ecs }
    .with<com_alive_t>().on_update<[](ecs_t&ecs, ent_t ent){
        auto&alive = ecs.get<game::com_alive_t>(ent);
        alive.timep = main::timer.now_mil;
    }>().on_construct<[](ecs_t&ecs, ent_t ent){
    }>().on_destroy<[](ecs_t&ecs, ent_t ent){
    }>();
    /* actor component signals */
    entt::sigh_helper{ ecs }
    .with<com_actor_t>().on_update<[](ecs_t&ecs, ent_t ent){
        auto&actor = ecs.get<com_actor_t>(ent);
        actor.timep = main::timer.now_mil;
    }>().on_construct<[](ecs_t&ecs, ent_t ent){
    }>().on_destroy<[](ecs_t&ecs, ent_t ent){
    }>();
    /* mover component signals */
    entt::sigh_helper{ ecs }
    .with<game::com_mover_t>().on_update<[](ecs_t&ecs, ent_t ent){
        auto&mover = ecs.get<game::com_mover_t>(ent);
        auto&apos2 = ecs.get<geom::com_apos2_t>(ent);
        apos2.x += mover.move.x * TCELL_ASIZE_X;
        apos2.y += mover.move.y * TCELL_ASIZE_Y;
        ecs.replace<geom::com_apos2_t>(ent, apos2.x, apos2.y);
        mover.move.x = 0;
        mover.move.y = 0;
#if FALSE
        auto zpos1 = ecs.get<com_zpos1_t>(ent);
        auto direc = ecs.get<com_direc_t>(ent);
        auto actor = ecs.get<com_actor_t>(ent);
        auto stepx = mover.move.x;
        auto stepy = mover.move.y;
        auto signx = get_num_sign(stepx);
        auto signy = get_num_sign(stepy);
        auto growx = TCELL_ASIZE_X * signx;
        auto growy = TCELL_ASIZE_Y * signy;
        if (mover.fall)
        {
        }
        else
        {
        }
#endif
    }>().on_construct<[](ecs_t&ecs, ent_t ent){
    }>().on_destroy<[](ecs_t&ecs, ent_t ent){
    }>();
    _EFNOT(somi_t::get().init(), return FALSE, "somi init failed");
    _EFNOT(hero_t::get().init(), return FALSE, "hero init failed");
    _EFNOT(picktile_t::get().init(), return FALSE, "picktile init failed");
    _EFNOT(helpgrid_t::get().init(), return FALSE, "helpgrid init failed");
    /* tilegrid signals */
    entt::sigh_helper{ ecs }
    .with<game::com_tcell_t>().on_update<[](ecs_t&ecs, ent_t ent){
    }>().on_construct<[](ecs_t&ecs, entt::entity ent)
    {
    }>().on_destroy<[](ecs_t&ecs, entt::entity ent)
    {
    }>();
    /* build the tilegrid */
    for (auto iter = 0; iter < TCELL_CFULL; iter++)
    { tgrid[iter] = entt::null; }
    if constexpr (TRUTH)
    { /* build */
        auto halfx = 0x10;
        auto halfy = 0x10;
        auto halfz = 0x02;
        auto clock = std::clock();
        for (auto iz = -halfz; iz < +halfz; iz++)
        {
            //_OLOG("[z]={:d}", iz);
            for (auto iy = -halfy; iy < +halfy; iy++)
            {
                //_OLOG("[y]={:d}", iy);
                for (auto ix = -halfx; ix < +halfx; ix++)
                {
                    if (rand()%5 == 0)
                    {
                    }
                    else
                    {
                        auto ent = set_tcell_into_tpos3(tpos3_t{ix,iy,iz});
                    }
                }
            }
        }
        _OLOG("[build-time]= {:d}", std::clock() - clock);
    }
    else if constexpr (FALSE)
    {
        auto halfx = 0x04, halfy = 0x02;
        for (auto iy = -halfy; iy < +halfy; iy++)
        {
            for (auto ix = -halfx; ix < +halfx; ix++)
            {
                auto ent = set_tcell_into_tpos3(tpos3_t{ix,iy,0},tcell_t{});
            }
        }
    }
    else if constexpr (FALSE)
    {
        auto halfx = TCELL_CHALF_X / 0x02;
        auto halfy = TCELL_CHALF_Y / 0x02;
        auto range = std::min(halfx, halfy);
        for (auto iy = -range; iy < +range; iy++)
        {
            for (auto ix = -range; ix < +range; ix++)
            {
                if (ix*ix + iy*iy < (range*range))
                {
                    set_tcell_into_tpos3({ix,iy,0});
                }
            }
        }
    } /* build */
    game::init_bot_signal.holder.publish();
    return TRUTH;
}
bool quit()
{
    game::quit_top_signal.holder.publish();
    _EFNOT(helpgrid_t::get().quit(), return FALSE, "helpgrid quit failed");
    _EFNOT(picktile_t::get().quit(), return FALSE, "picktile init failed");
    _EFNOT(hero_t::get().quit(), return FALSE, "hero quit failed");
    _EFNOT(somi_t::get().quit(), return FALSE, "somi quit failed");
    game::quit_bot_signal.holder.publish();
    return TRUTH;
}

} /* actions */

} /* content */

#endif/*SOMIGAME_GAME_CXX*/
