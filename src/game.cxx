#include "head.hxx"

/* headers */

#include "game.hxx"
#include "util.hxx"
#include "gfix.hxx"
#include "ecos.hxx"
#include "iput.hxx"

#include "../lib/entt/src/entity/registry.hpp"
#include "../lib/entt/src/entity/helper.hpp"

/* content */

_NAMESPACE_ENTER

/** datadef **/

struct somi_t somi = {
    .entity= entt::null,
    /* visual */
    .visual= _NULL,
    /* family */
    .family= _NULL
};
struct hero_t hero = {
    .entity = entt::null,
    /* logic */
    .alive = _NULL,
    .actor = _NULL,
    .mover = _NULL,
    /* sizes */
    .asize = _NULL,
    .gsize = _NULL,
    .tsize = _NULL,
    /* coord */
    .apos2 = _NULL,
    .zpos1 = _NULL,
    .gpos3 = _NULL,
    .tpos3 = _NULL,
    .pivot = _NULL,
    /* geometry */
    .direc = _NULL,
    /* visual */
    .visual= _NULL,
    /* family */
    .family= _NULL,
};
struct pick_t pick = {
    .entity = entt::null,
    /* visual */
    .visual= _NULL,
    /* coord */
    .apos2 = _NULL,
    .gpos3 = _NULL,
    .tpos3 = _NULL,
    /* sizes */
    .asize = _NULL,
    /* family */
    .family= _NULL,
};

ent_t tgrid[TCELL_CFULL];

_SIGNALDEF(hero_rise)
_SIGNALDEF(hero_died)

_SIGNALDEF(pick_step)

/** actions **/

void game_init()
{
    /* entity component system */
    if constexpr (_TRUTH)
    { /* alive component */
        entt::sigh_helper{ ecos }
        .with<com_alive_t>().on_update<[](ecos_t&ecos, ent_t ent){
            auto&alive = ecos.get<com_alive_t>(ent);
            alive.timep = timer.now_mil;
            if (ent == hero.entity)
            {
                if (alive.valid) { hero_rise_sigholder.publish(alive); }
                else { hero_died_sigholder.publish(alive); }
            }
        }>().on_construct<[](ecos_t&ecos, ent_t ent){
        }>().on_destroy<[](ecos_t&ecos, ent_t ent){
        }>();
    } /* alive component */
    if constexpr (_TRUTH)
    { /* actor component */
        entt::sigh_helper{ ecos }
        .with<com_actor_t>().on_update<[](ecos_t&ecos, ent_t ent){
            auto&actor = ecos.get<com_actor_t>(ent);
            actor.timep = timer.now_mil;
        }>().on_construct<[](ecos_t&ecos, ent_t ent){
        }>().on_destroy<[](ecos_t&ecos, ent_t ent){
        }>();
    } /* actor component */
    if constexpr (_TRUTH)
    { /* mover component */
        entt::sigh_helper{ ecos }
        .with<com_mover_t>().on_update<[](ecos_t&ecos, ent_t ent){
            auto&mover = ecos.get<com_mover_t>(ent);
            auto apos2 = ecos.get<com_apos2_t>(ent);
            auto zpos1 = ecos.get<com_zpos1_t>(ent);
            auto direc = ecos.get<com_direc_t>(ent);
            auto actor = ecos.get<com_actor_t>(ent);
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
            mover.move = apos2_t{ 0, 0 };
        }>().on_construct<[](ecos_t&ecos, ent_t ent){
        }>().on_destroy<[](ecos_t&ecos, ent_t ent){
        }>();
    } /* mover component */
    if constexpr (_TRUTH)
    { /* the game entity */
        somi.entity= ecos.create();
        /* family */
        somi.family=&ecos.emplace<com_family_t>(somi.entity);
    }
    if constexpr (_TRUTH)
    { /* hero entity */
        hero.entity = ecos.create();
        /* logic */
        hero.alive =&ecos.emplace<com_alive_t>(hero.entity, alive_t{
            .valid = _TRUTH,
            .timep = timer.now_mil,
        });
        hero.actor =&ecos.emplace<com_actor_t>(hero.entity, actor_t{
            .count = 0,
            .timep = timer.now_mil,
        });
        hero.mover =&ecos.emplace<com_mover_t>(hero.entity, mover_t{
            .fall = _TRUTH,
            .move = apos2_t{
                .x = 0,
                .y = 0,
            },
        });
        /* sizes */
        hero.asize =&ecos.emplace<com_asize_t>(hero.entity, asize_t{
            TCELL_ASIZE_X, TCELL_ASIZE_Y
        });
        hero.gsize = ecos.try_get<com_gsize_t>(hero.entity);
        hero.tsize = ecos.try_get<com_tsize_t>(hero.entity);
        /* coord */
        hero.apos2 =&ecos.emplace<com_apos2_t>(hero.entity);
        hero.zpos1 =&ecos.emplace<com_zpos1_t>(hero.entity);
        hero.gpos3 = ecos.try_get<com_gpos3_t>(hero.entity);
        hero.tpos3 = ecos.try_get<com_tpos3_t>(hero.entity);
        hero.pivot =&ecos.emplace<com_pivot_t>(hero.entity, pivot_t{
            .x = PIVOT_MID,
            .y = PIVOT_MID,
        });
        /* geometry */
        hero.direc =&ecos.emplace<com_direc_t>(hero.entity, direc_t{
            .x = +0,
            .y = -1
        });
        /* visual */
        hero.visual=&ecos.emplace<com_visual_t>(hero.entity, _TRUTH);
        ecos.emplace<com_rlayer_t>(hero.entity, 1);
        ecos.emplace<com_color_t>(hero.entity, 0xff);
        ecos.emplace<com_imreg_t>(hero.entity, com_imreg_t{
            .index = _IMORI_GAME_HERO,
            .rsize = rsize_t{
                .x = RSIZE_MAX/4,
                .y = RSIZE_MAX,
            },
        });
        ecos.emplace<com_faces_t>(hero.entity, com_faces_t{
            .ilist = {
                imreg_t{
                    .index = _IMORI_GAME_HERO,
                    .rpos2 = rpos2_t{
                        .x = RPOS2_MAX*0/4,
                        .y = RPOS2_MIN,
                    },
                    .rsize = rsize_t{
                        .x = RSIZE_MAX/4,
                        .y = RSIZE_MAX,
                    },
                },
                imreg_t{
                    .index = _IMORI_GAME_HERO,
                    .rpos2 = rpos2_t{
                        .x = RPOS2_MAX*1/4,
                        .y = RPOS2_MIN,
                    },
                    .rsize = rsize_t{
                        .x = RSIZE_MAX/4,
                        .y = RSIZE_MIN,
                    },
                },
                imreg_t{
                    .index = _IMORI_GAME_HERO,
                    .rpos2 = rpos2_t{
                        .x = RPOS2_MAX*2/4,
                        .y = RPOS2_MIN,
                    },
                    .rsize = rsize_t{
                        .x = RSIZE_MAX/4,
                        .y = RSIZE_MIN,
                    },
                },
                imreg_t{
                    .index = _IMORI_GAME_HERO,
                    .rpos2 = rpos2_t{
                        .x = RPOS2_MAX*3/4,
                        .y = RPOS2_MIN,
                    },
                    .rsize = rsize_t{
                        .x = RSIZE_MAX/4,
                        .y = RSIZE_MAX,
                    },
                },
            },
        });
        /* family */
        hero.family=&ecos.emplace<com_family_t>(hero.entity, family_t{
            .ancestor = somi.entity,
        });
        if constexpr (_TRUTH)
        { /* pick entity */
            pick.entity= ecos.create();
            /* coord */
            pick.apos2 =&ecos.emplace<com_apos2_t>(pick.entity, apos2_t{
                .x = 0,
                .y = 0,
            });
            pick.gpos3 = ecos.try_get<com_gpos3_t>(pick.entity);
            pick.tpos3 = ecos.try_get<com_tpos3_t>(pick.entity);
            pick.asize =&ecos.emplace<com_asize_t>(pick.entity, asize_t{
                    .x = TCELL_ASIZE_X,
                    .y = TCELL_ASIZE_Y,
            });
            /* visual */
            pick.visual=&ecos.emplace<com_visual_t>(pick.entity, _FALSE);
            ecos.emplace<com_rlayer_t>(pick.entity, 3);
            ecos.emplace<com_color_t>(pick.entity, 0xff);
            ecos.emplace<com_imreg_t>(pick.entity, imreg_t{
                .index = _IMORI_GAME_PICK,
                .rsize = {
                    .x = RSIZE_MAX,
                    .y = RSIZE_MAX,
                },
            });
            key_mode_set_sigbinder.connect<[](key_mode_e mode){
                ecos.replace<com_apos2_t>(pick.entity, apos2_t{ 0, 0 });
                pick_step_sigholder.publish();
                ecos.replace<com_visual_t>(pick.entity, mode == _KEY_MODE_PICK);
            }>();
            /* family */
            pick.family=&ecos.emplace<com_family_t>(pick.entity, family_t{
                .ancestor = hero.entity,
            });
        } /* pick */
        if constexpr (_TRUTH)
        { /* helpgrid */
            if (ecos.valid(helpgrid.entity))
            {
                if (!vet_ancestor(helpgrid.entity, hero.entity))
                {
                    set_ancestor(helpgrid.entity, hero.entity);
                }
            }
        } /* helpgrid */
        if constexpr (_TRUTH)
        { /* signals */
            struct listener_t {
                inline void on_update(update_event_t&update)
                {
                    if (!hero.alive->valid && ((timer.now_mil - hero.alive->timep) > 3'000))
                    {
                        hero.alive->valid = _TRUTH;
                        ecos.patch<com_alive_t>(hero.entity);
                    }
                }
            } static listener{};
            hero_rise_sigbinder.connect<[](const alive_t&alive){
                ecos.replace<com_apos2_t>(hero.entity, apos2_t{ 0, 0 });
                key_mode_set(_KEY_MODE_MAIN);
                timer.dispatcher.sink<update_event_t>()
                    .disconnect<&listener_t::on_update>(listener);
            }>();
            hero_died_sigbinder.connect<[](const alive_t&alive){
                key_mode_set(_KEY_MODE_DEAD);
                timer.dispatcher.sink<update_event_t>()
                    .connect<&listener_t::on_update>(listener);
            }>();
        } /* signals */
        /*
        static auto animator = animator_t(hero.entity);
        animator.insert<poses_t>(0, 10000, 32,&poses_t::y);
        animator.play();
        */
    } /* hero */
    if constexpr (_TRUTH)
    { /* tilemap */
        entt::sigh_helper{ ecos }
        .with<com_tcell_t>().on_update<[](ecos_t&ecos, ent_t ent){
        }>().on_construct<[](ecos_t&ecos, entt::entity ent)
        {
        }>().on_destroy<[](ecos_t&ecos, entt::entity ent)
        {
        }>();
    }
    if constexpr (_FALSE)
    { /* build */
        for (auto iter = 0; iter < TCELL_CFULL; iter++)
        { tgrid[iter] = entt::null; }
        auto halfx = TCELL_CHALF_X / 0x08;
        auto halfy = TCELL_CHALF_Y / 0x08;
        auto halfz = TCELL_CHALF_Z / 0x08;
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
                        auto ent = set_tcell_into_tpos3(tpos3_t{ix,iy,iz},tcell_t{});
                        ecos.emplace<com_floor_t>(ent);
                    }
                }
            }
        }
    }
    else if constexpr (_TRUTH)
    {
        for (auto iter = 0; iter < TCELL_CFULL; iter++)
        { tgrid[iter] = entt::null; }
        auto halfx = 0x04, halfy = 0x02;
        for (auto iy = -halfy; iy < +halfy; iy++)
        {
            for (auto ix = -halfx; ix < +halfx; ix++)
            {
                auto ent = set_tcell_into_tpos3(tpos3_t{ix,iy,0},tcell_t{});
                ecos.emplace<com_floor_t>(ent);
            }
        }
    } /* build */
}

/** getters **/

ent_t get_tcell_from_tpos3(tpos3_t tpos3)
{
    auto tkey3 = get_tkey3_from_tpos3(tpos3);
    if (_FALSE
        || tkey3.x < 0 || tkey3.x >= TCELL_CFULL_X
        || tkey3.y < 0 || tkey3.y >= TCELL_CFULL_Y
        || tkey3.z < 0 || tkey3.z >= TCELL_CFULL_Z
    ) { return entt::null; }
    return tgrid[get_tkey1_from_tkey3(tkey3)];
}

/** vetters **/

bool vet_tcell_from_tpos3(tpos3_t tpose)
{ return ecos.try_get<com_tcell_t>(get_tcell_from_tpos3(tpose)); }

bool vet_floor_from_tpos3(tpos3_t tpose)
{ return ecos.try_get<com_floor_t>(get_tcell_from_tpos3(tpose)); }

bool vet_block_from_tpos3(tpos3_t tpose)
{ return ecos.try_get<com_block_t>(get_tcell_from_tpos3(tpose)); }

/** setters **/

ent_t set_tcell_into_tpos3(tpos3_t tpos3, tcell_t tcell)
{
    tkey3_t tkey3 = get_tkey3_from_tpos3(tpos3);
    if (_FALSE
        || tkey3.x < 0 || tkey3.x >= TCELL_CFULL_X
        || tkey3.y < 0 || tkey3.y >= TCELL_CFULL_Y
        || tkey3.z < 0 || tkey3.z >= TCELL_CFULL_Z
    ) { return entt::null; }
    auto&entity = tgrid[get_tkey1_from_tkey3(tkey3)];
    if (ecos.valid(entity)) { ecos.destroy(entity); }
    gpos3_t gpos3 = get_gpos3_from_tpos3(tpos3);
    if constexpr (_TRUTH)
    { /* entity construction */
        entity = ecos.create();
        /* sizes */
        ecos.emplace<com_asize_t>(entity, asize_t{
            .x = TCELL_ASIZE_X,
            .y = TCELL_ASIZE_Y,
        });
        /* coord */
        ecos.emplace<com_apos2_t>(entity, com_apos2_t{ gpos3.x, gpos3.y });
        ecos.emplace<com_zpos1_t>(entity, com_zpos1_t{ gpos3.z });
        /* visual */
        ecos.emplace<com_visual_t>(entity, _TRUTH);
        ecos.emplace<com_color_t>(entity, color_t{
            .v = 0xff,
        });
        ecos.emplace<com_imreg_t>(entity, imreg_t{
            .index = _IMORI_TILE_TEST,
            .rsize = rsize_t{
                .x = RSIZE_MAX,
                .y = RSIZE_MAX,
            },
        });
        /* family */
        ecos.emplace<com_family_t>(entity, family_t{
            .ancestor = somi.entity,
        });
        /* tilegrid */
        ecos.emplace<com_tcell_t>(entity, tcell);
    }
    return entity;
}

_NAMESPACE_LEAVE
